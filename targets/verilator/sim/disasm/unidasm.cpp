// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    Universal disassembler.

****************************************************************************/

// the disassemblers assume they're in MAME and emu.h is a PCH, so we minimally pander to them
#include "disasm/coretmpl.h"
#include "disasmintf.h"

using offs_t = osd::u32;
using util::BIT;

#include "6x09dasm.h"

#include "disasm/corestr.h"
#include "disasm/eminline.h"
#include "disasm/endianness.h"
#include "disasm/ioprocs.h"
#include "disasm/osdfile.h"
#include "disasm/strformat.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#endif


using u8 = util::u8;
using u16 = util::u16;
using u32 = util::u32;
using u64 = util::u64;

// Configuration classes


static constexpr auto le = util::endianness::little;
static constexpr auto be = util::endianness::big;

struct dasm_table_entry
{
	const char *            name;
	util::endianness        endian;
	int8_t                  pcshift;
	std::function<util::disasm_interface *()> alloc;
};


struct options
{
	const char *            filename;
	offs_t                  basepc;
	uint8_t                 norawbytes;
	uint8_t                 xchbytes;
	uint8_t                 lower;
	uint8_t                 upper;
	uint8_t                 flipped;
	const dasm_table_entry *dasm;
	uint32_t                skip;
	uint32_t                count;
	bool                    octal;
};

static const dasm_table_entry dasm_table[] =
{
	{ "m6809",           be,  0, []() -> util::disasm_interface * { return new m6809_disassembler; } }
};

class unidasm_data_buffer : public util::disasm_interface::data_buffer
{
public:
	std::vector<u8> data;
	offs_t base_pc = 0;
	u32 size;

	unidasm_data_buffer(util::disasm_interface *disasm, const dasm_table_entry *entry);
	virtual ~unidasm_data_buffer() = default;

	void decrypt(const unidasm_data_buffer &buffer, bool opcode);

	virtual u8  r8 (offs_t pc) const override { return lr8 (pc); }
	virtual u16 r16(offs_t pc) const override { return lr16(pc); }
	virtual u32 r32(offs_t pc) const override { return lr32(pc); }
	virtual u64 r64(offs_t pc) const override { return lr64(pc); }

private:
	std::function<u8  (offs_t pc)> lr8;
	std::function<u16 (offs_t pc)> lr16;
	std::function<u32 (offs_t pc)> lr32;
	std::function<u64 (offs_t pc)> lr64;

	util::disasm_interface *disasm;
	const dasm_table_entry *entry;

	template<typename T> const T *get_ptr(offs_t pc) const {
		if(pc < base_pc)
			return nullptr;
		offs_t delta = (pc - base_pc) * sizeof(T);
		if(delta >= size)
			return nullptr;
		return reinterpret_cast<const T *>(&data[delta]);
	}

	template<typename T> T get(offs_t pc) const {
		auto p = get_ptr<T>(pc);
		return p ? *p : 0;
	}
};

struct dasm_line
{
	offs_t pc;
	offs_t size;
	std::string dasm;
};

unidasm_data_buffer::unidasm_data_buffer(util::disasm_interface *_disasm, const dasm_table_entry *_entry) : disasm(_disasm), entry(_entry)
{
	u32 flags = disasm->interface_flags();
	u32 page_mask = flags & util::disasm_interface::PAGED ? (1 << disasm->page_address_bits()) - 1 : 0;

	if(flags & util::disasm_interface::NONLINEAR_PC) {
		switch(entry->pcshift) {
		case -1:
			lr8  = [](offs_t pc) -> u8  { throw std::logic_error("debug_disasm_buffer::debug_data_buffer: r8 access on 16-bits granularity bus\n"); };
			switch(entry->endian) {
			case le:
				lr16 = [this](offs_t pc) -> u16 {
					const u16 *src = get_ptr<u16>(pc);
					return little_endianize_int16(src[0]);
				};
				lr32 = [this, page_mask](offs_t pc) -> u32 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u32 r = 0;
					for(int j=0; j != 2; j++) {
						r |= little_endianize_int16(get<u16>(disasm->pc_linear_to_real(lpc))) << (j*16);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				lr64 = [this, page_mask](offs_t pc) -> u64 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u64 r = 0;
					for(int j=0; j != 4; j++) {
						r |= u64(little_endianize_int16(get<u16>(disasm->pc_linear_to_real(lpc)))) << (j*16);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				break;

			case be:
				lr16 = [this](offs_t pc) -> u16 {
					const u16 *src = get_ptr<u16>(pc);
					return big_endianize_int16(src[0]);
				};
				lr32 = [this, page_mask](offs_t pc) -> u32 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u32 r = 0;
					for(int j=0; j != 2; j++) {
						r |= big_endianize_int16(get<u16>(disasm->pc_linear_to_real(lpc))) << ((1-j)*16);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				lr64 = [this, page_mask](offs_t pc) -> u64 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u64 r = 0;
					for(int j=0; j != 4; j++) {
						r |= u64(big_endianize_int16(get<u16>(disasm->pc_linear_to_real(lpc)))) << ((3-j)*16);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				break;
			}
			break;

		case 0:
			lr8 = [this](offs_t pc) -> u8 {
				const u8 *src = get_ptr<u8>(pc);
				return src[0];
			};

			switch(entry->endian) {
			case le:
				lr16 = [this, page_mask](offs_t pc) -> u16 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u16 r = 0;
					for(int j=0; j != 2; j++) {
						r |= get<u8>(disasm->pc_linear_to_real(lpc)) << (j*8);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				lr32 = [this, page_mask](offs_t pc) -> u32 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u32 r = 0;
					for(int j=0; j != 2; j++) {
						r |= get<u8>(disasm->pc_linear_to_real(lpc)) << (j*8);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				lr64 = [this, page_mask](offs_t pc) -> u64 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u64 r = 0;
					for(int j=0; j != 8; j++) {
						r |= u64(get<u8>(disasm->pc_linear_to_real(lpc))) << (j*8);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				break;

			case be:
				lr16 = [this, page_mask](offs_t pc) -> u16 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u16 r = 0;
					for(int j=0; j != 2; j++) {
						r |= get<u8>(disasm->pc_linear_to_real(lpc)) << ((1-j)*8);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				lr32 = [this, page_mask](offs_t pc) -> u32 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u32 r = 0;
					for(int j=0; j != 2; j++) {
						r |= get<u8>(disasm->pc_linear_to_real(lpc)) << ((3-j)*8);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				lr64 = [this, page_mask](offs_t pc) -> u64 {
					offs_t lpc = disasm->pc_real_to_linear(pc);
					u64 r = 0;
					for(int j=0; j != 8; j++) {
						r |= u64(get<u8>(disasm->pc_linear_to_real(lpc))) << ((7-j)*8);
						lpc = (lpc & ~page_mask) | ((lpc + 1) & page_mask);
					}
					return r;
				};
				break;
			}
			break;
		}
	} else {
		switch(entry->pcshift) {
		case 0:
			lr8 = [this](offs_t pc) -> u8 {
				const u8 *p = get_ptr<u8>(pc);
				return p ?
				p[0]
				: 0x00;
			};
			switch(entry->endian) {
			case le:
				lr16 = [this](offs_t pc) -> u16 {
					const u8 *p = get_ptr<u8>(pc);
					return p ?
					p[0] |
					(p[1] << 8)
					: 0x0000;
				};
				lr32 = [this](offs_t pc) -> u32 {
					const u8 *p = get_ptr<u8>(pc);
					return p ?
					p[0] |
					(p[1] << 8) |
					(p[2] << 16) |
					(p[3] << 24)
					: 0x00000000;
				};
				lr64 = [this](offs_t pc) -> u64 {
					const u8 *p = get_ptr<u8>(pc);
					return p ?
					p[0] |
					(p[1] << 8) |
					(p[2] << 16) |
					(p[3] << 24) |
					(u64(p[4]) << 32) |
					(u64(p[5]) << 40) |
					(u64(p[6]) << 48) |
					(u64(p[7]) << 56)
					: 0x0000000000000000; };
				break;
			case be:
				lr16 = [this](offs_t pc) -> u16 {
					const u8 *p = get_ptr<u8>(pc);
					return p ?
					(p[0] << 8) |
					p[1]
					: 0x0000;
				};
				lr32 = [this](offs_t pc) -> u32 {
					const u8 *p = get_ptr<u8>(pc);
					return p ?
					(p[0] << 24) |
					(p[1] << 16) |
					(p[2] << 8) |
					p[3]
					: 0x00000000;
				};
				lr64 = [this](offs_t pc) -> u64 {
					const u8 *p = get_ptr<u8>(pc);
					return p ?
					(u64(p[0]) << 56) |
					(u64(p[1]) << 48) |
					(u64(p[2]) << 40) |
					(u64(p[3]) << 32) |
					(p[4] << 24) |
					(p[5] << 16) |
					(p[6] << 8) |
					p[7]
					: 0x0000000000000000; };
				break;
			}
			break;

		case -1:
			lr8 = [](offs_t pc) -> u8 { abort(); };
			switch(entry->endian) {
			case le:
				lr16 = [this](offs_t pc) -> u16 {
					const u16 *p = get_ptr<u16>(pc);
					return p ?
					little_endianize_int16(p[0])
					: 0x0000;
				};
				lr32 = [this](offs_t pc) -> u32 {
					const u16 *p = get_ptr<u16>(pc);
					return p ?
					little_endianize_int16(p[0]) |
					(little_endianize_int16(p[1]) << 16)
					: 0x00000000;
				};
				lr64 = [this](offs_t pc) -> u64 {
					const u16 *p = get_ptr<u16>(pc);
					return p ?
					little_endianize_int16(p[0]) |
					(little_endianize_int16(p[1]) << 16) |
					(u64(little_endianize_int16(p[2])) << 32) |
					(u64(little_endianize_int16(p[3])) << 48)
					: 0x0000000000000000;
				};
				break;
			case be:
				lr16 = [this](offs_t pc) -> u16 {
					const u16 *p = get_ptr<u16>(pc);
					return p ?
					big_endianize_int16(p[0])
					: 0x0000;
				};
				lr32 = [this](offs_t pc) -> u32 {
					const u16 *p = get_ptr<u16>(pc);
					return p ?
					(big_endianize_int16(p[0]) << 16)|
					big_endianize_int16(p[1])
					: 0x00000000;
				};
				lr64 = [this](offs_t pc) -> u64 {
					const u16 *p = get_ptr<u16>(pc);
					return p ?
					(u64(big_endianize_int16(p[0])) << 48) |
					(u64(big_endianize_int16(p[1])) << 32) |
					(big_endianize_int16(p[2]) << 16) |
					big_endianize_int16(p[3])
					: 0x0000000000000000;
				};
				break;
			}
			break;

		case -2:
			lr8 = [](offs_t pc) -> u8 { abort(); };
			lr16 = [](offs_t pc) -> u16 { abort(); };
			switch(entry->endian) {
			case le:
				lr32 = [this](offs_t pc) -> u32 {
					const u32 *p = get_ptr<u32>(pc);
					return p ?
					little_endianize_int32(p[0])
					: 0x00000000;
				};
				lr64 = [this](offs_t pc) -> u64 {
					const u32 *p = get_ptr<u32>(pc);
					return p ?
					little_endianize_int32(p[0]) |
					(u64(little_endianize_int32(p[1])) << 32)
					: 0x0000000000000000;
				};
				break;
			case be:
				lr32 = [this](offs_t pc) -> u32 {
					const u32 *p = get_ptr<u32>(pc);
					return p ?
					big_endianize_int32(p[0])
					: 0x00000000;
				};
				lr64 = [this](offs_t pc) -> u64 {
					const u32 *p = get_ptr<u32>(pc);
					return p ?
					(u64(big_endianize_int32(p[0])) << 32) |
					big_endianize_int32(p[1])
					: 0x0000000000000000;
				};
				break;
			}
			break;

		case -3:
			lr8 = [](offs_t pc) -> u8 { abort(); };
			lr16 = [](offs_t pc) -> u16 { abort(); };
			lr32 = [](offs_t pc) -> u32 { abort(); };
			switch(entry->endian) {
			case le:
				lr64 = [this](offs_t pc) -> u64 {
					const u64 *p = get_ptr<u64>(pc);
					return p ?
					little_endianize_int64(p[0])
					: 0x0000000000000000;
				};
				break;
			case be:
				lr64 = [this](offs_t pc) -> u64 {
					const u64 *p = get_ptr<u64>(pc);
					return p ?
					big_endianize_int64(p[0])
					: 0x0000000000000000;
				};
				break;
			};
			break;

		case 3:
			lr8 = [](offs_t pc) -> u8 { abort(); };
			switch(entry->endian) {
			case le:
				lr16 = [this](offs_t pc) -> u16 {
					if(pc < base_pc)
						return 0x0000;
					offs_t delta = (pc - base_pc) >> 3;
					if(delta >= size)
						return 0x0000;
					return little_endianize_int16(reinterpret_cast<const u16 *>(&data[delta])[0]);
				};
				lr32 = [this](offs_t pc) -> u32 {
					if(pc < base_pc)
						return 0x00000000;
					offs_t delta = (pc - base_pc) >> 3;
					if(delta >= size + 2)
						return 0x00000000;
					auto p = reinterpret_cast<const u16 *>(&data[delta]);
					return little_endianize_int16(p[0]) | (u32(little_endianize_int16(p[1])) << 16);
				};
				break;
			case be:
				lr16 = [this](offs_t pc) -> u16 {
					if(pc < base_pc)
						return 0x0000;
					offs_t delta = (pc - base_pc) >> 3;
					if(delta >= size)
						return 0x0000;
					return big_endianize_int16(reinterpret_cast<const u16 *>(&data[delta])[0]);
				};
				lr32 = [this](offs_t pc) -> u32 {
					if(pc < base_pc)
						return 0x00000000;
					offs_t delta = (pc - base_pc) >> 3;
					if(delta >= size + 2)
						return 0x00000000;
					auto p = reinterpret_cast<const u16 *>(&data[delta]);
					return (u32(big_endianize_int16(p[0])) << 16) | big_endianize_int16(p[1]);
				};
				break;
			}
			lr64 = [](offs_t pc) -> u64 { abort(); };
			break;

		default:
			abort();
		}
	}
}

void unidasm_data_buffer::decrypt(const unidasm_data_buffer &buffer, bool opcode)
{
	abort();
}

static int parse_options(int argc, char *argv[], options *opts)
{
	bool pending_base = false;
	bool pending_arch = false;
	bool pending_skip = false;
	bool pending_count = false;

	memset(opts, 0, sizeof(*opts));

	// loop through arguments
	for(unsigned arg = 1; arg < argc; arg++) {
		char *curarg = argv[arg];

		// is it a switch?
		if(curarg[0] == '-' && curarg[1] != '\0') {
			if(pending_base || pending_arch || pending_skip || pending_count)
				goto usage;

			if(tolower((uint8_t)curarg[1]) == 'a')
				pending_arch = true;
			else if(tolower((uint8_t)curarg[1]) == 'b')
				pending_base = true;
			else if(tolower((uint8_t)curarg[1]) == 'f')
				opts->flipped = true;
			else if(tolower((uint8_t)curarg[1]) == 'l')
				opts->lower = true;
			else if(tolower((uint8_t)curarg[1]) == 's')
				pending_skip = true;
			else if(tolower((uint8_t)curarg[1]) == 'c')
				pending_count = true;
			else if(tolower((uint8_t)curarg[1]) == 'n')
				opts->norawbytes = true;
			else if(tolower((uint8_t)curarg[1]) == 'u')
				opts->upper = true;
			else if(tolower((uint8_t)curarg[1]) == 'x')
				opts->xchbytes = true;
			else if(tolower((uint8_t)curarg[1]) == 'o')
				opts->octal = true;
			else
				goto usage;

		} else if(pending_base) {
		// base PC
			int result;
			if(curarg[0] == '0') {
				if(tolower((uint8_t)curarg[1]) == 'x')
					result = sscanf(&curarg[2], "%x", &opts->basepc);
				else if(tolower((uint8_t)curarg[1]) == 'o')
					result = sscanf(&curarg[2], "%o", &opts->basepc);
				else
					result = sscanf(&curarg[1], "%o", &opts->basepc);
			}
			else if(curarg[0] == '$')
				result = sscanf(&curarg[1], "%x", &opts->basepc);
			else
				result = sscanf(&curarg[0], "%x", &opts->basepc);
			if(result != 1)
				goto usage;
			pending_base = false;

		} else if(pending_arch) {
			// architecture
			auto const arch = std::find_if(
					std::begin(dasm_table),
					std::end(dasm_table),
					[&curarg] (dasm_table_entry const &e) { return !core_stricmp(curarg, e.name); });
			if (std::end(dasm_table) == arch)
				goto usage;
			opts->dasm = &*arch;
			pending_arch = false;

		} else if(pending_skip) {
			// skip bytes
			int result;
			if(curarg[0] == '0') {
				if(tolower((uint8_t)curarg[1]) == 'x')
					result = sscanf(&curarg[2], "%x", &opts->skip);
				else if(tolower((uint8_t)curarg[1]) == 'o')
					result = sscanf(&curarg[2], "%o", &opts->skip);
				else
					result = sscanf(&curarg[1], "%o", &opts->skip);
			}
			else if(curarg[0] == '$')
				result = sscanf(&curarg[1], "%x", &opts->skip);
			else
				result = sscanf(curarg, "%d", &opts->skip);
			if(result != 1)
				goto usage;
			pending_skip = false;

		} else if(pending_count) {
			// size
			if(sscanf(curarg, "%d", &opts->count) != 1)
				goto usage;
			pending_count = false;

		} else if(opts->filename == nullptr) {
			// filename
			opts->filename = curarg;

		} else {
			// fail
			goto usage;
		}
	}

	// if we have a dangling option, error
	if(pending_base || pending_arch || pending_skip || pending_count)
		goto usage;

	// if no file or no architecture, fail
	if(opts->filename == nullptr || opts->dasm == nullptr)
		goto usage;

	return 0;

usage:
	printf("Usage: %s <filename> -arch <architecture> [-basepc <pc>] \n", argv[0]);
	printf("   [-norawbytes] [-xchbytes] [-flipped] [-upper] [-lower]\n");
	printf("   [-skip <n>] [-count <n>] [-octal]\n");
	printf("\n");
	printf("Supported architectures:");
	const int colwidth = 1 + std::strlen(std::max_element(std::begin(dasm_table), std::end(dasm_table), [](const dasm_table_entry &a, const dasm_table_entry &b) { return std::strlen(a.name) < std::strlen(b.name); })->name);
	const int columns = std::max(1, 80 / colwidth);
	const int numrows = (std::size(dasm_table) + columns - 1) / columns;
	for(unsigned curarch = 0; curarch < numrows * columns; curarch++) {
		const int row = curarch / columns;
		const int col = curarch % columns;
		const int index = col * numrows + row;
		if(col == 0)
			printf("\n  ");
		printf("%-*s", colwidth, (index < std::size(dasm_table)) ? dasm_table[index].name : "");
	}
	printf("\n");
	return 1;
};


int disasm_file(util::random_read &file, u64 length, options &opts)
{
	if(opts.skip >= length) {
		std::fprintf(stderr, "File '%s' is too short (only %llu bytes found)\n", opts.filename, (unsigned long long)length);
		return 1;
	}

	// Build the disasm object
	std::unique_ptr<util::disasm_interface> disasm(opts.dasm->alloc());
	u32 flags = disasm->interface_flags();

	// Compute the granularity in bytes (1-8)
	offs_t granularity = opts.dasm->pcshift < 0 ? disasm->opcode_alignment() << -opts.dasm->pcshift : disasm->opcode_alignment() >> opts.dasm->pcshift;

	// Build the base buffer and fill it (with a margin)
	unidasm_data_buffer base_buffer(disasm.get(), opts.dasm);
	u32 rounded_size = (((length - opts.skip) + (granularity - 1)) & ~(granularity - 1));
	base_buffer.data.resize(rounded_size + 8, 0x00);
	base_buffer.size = length;
	base_buffer.base_pc = opts.basepc;
	std::size_t actual;
	std::error_condition filerr = file.read_at(opts.skip, &base_buffer.data[0], length - opts.skip, actual);
	if(filerr) {
		std::fprintf(stderr, "Error reading from file '%s' (%s)\n", opts.filename, filerr.message().c_str());
		return 1;
	}
	if(opts.xchbytes) {
		for(u64 offset = 0; offset < (length - opts.skip) - 1; offset += 2)
			std::swap(base_buffer.data[offset], base_buffer.data[offset + 1]);
	}

	// Build the decryption buffers if needed
	unidasm_data_buffer opcodes_buffer(disasm.get(), opts.dasm), params_buffer(disasm.get(), opts.dasm);
	if(flags & util::disasm_interface::INTERNAL_DECRYPTION) {
		opcodes_buffer.decrypt(base_buffer, true);
		if((flags & util::disasm_interface::SPLIT_DECRYPTION) == util::disasm_interface::SPLIT_DECRYPTION)
			params_buffer.decrypt(base_buffer, false);
	}

	// Select the buffers to actually use
	unidasm_data_buffer *popcodes = opcodes_buffer.data.empty() ? &base_buffer : &opcodes_buffer;
	unidasm_data_buffer *pparams  = params_buffer.data.empty() ? popcodes : &params_buffer;

	// Compute the pc wraparound
	offs_t pclength = opts.dasm->pcshift < 0 ? rounded_size >> -opts.dasm->pcshift : rounded_size << opts.dasm->pcshift;
	offs_t limit = opts.basepc + pclength;
	offs_t pc_mask = limit ? util::make_bitmask<offs_t>(32 - count_leading_zeros_32(limit - 1)) : 0xffffffff;

	// Compute the page wraparound
	offs_t page_mask = flags & util::disasm_interface::PAGED ? (1 << disasm->page_address_bits()) - 1 : 0;

	// Next pc computation lambdas
	std::function<offs_t (offs_t, offs_t)> next_pc;
	std::function<offs_t (offs_t, offs_t)> next_pc_wrap;
	if(flags & util::disasm_interface::NONLINEAR_PC) {
		// lfsr pc is always paged
		next_pc = [pc_mask, page_mask, dis = disasm.get()](offs_t pc, offs_t size) {
			offs_t lpc = dis->pc_real_to_linear(pc);
			offs_t lpce = lpc + size;
			if((lpc ^ lpce) & ~page_mask)
				lpce = (lpc | page_mask) + 1;
			lpce &= pc_mask;
			return dis->pc_linear_to_real(lpce);
		};
		next_pc_wrap = [page_mask, dis = disasm.get()](offs_t pc, offs_t size) {
			offs_t lpc = dis->pc_real_to_linear(pc);
			offs_t lpce = (lpc & ~page_mask) | ((lpc + size) & page_mask);
			return dis->pc_linear_to_real(lpce);
		};

	} else if(flags & util::disasm_interface::PAGED) {
		next_pc = [pc_mask, page_mask](offs_t pc, offs_t size) {
			offs_t pce = pc + size;
			if((pc ^ pce) & ~page_mask)
				pce = (pc | page_mask) + 1;
			pce &= pc_mask;
			return pce;
		};
		next_pc_wrap = [page_mask](offs_t pc, offs_t size) {
			offs_t pce = (pc & ~page_mask) | ((pc + size) & page_mask);
			return pce;
		};

	} else {
		next_pc = [pc_mask](offs_t pc, offs_t size) {
			return (pc + size) & pc_mask;
		};
		next_pc_wrap = [pc_mask](offs_t pc, offs_t size) {
			return (pc + size) & pc_mask;
		};
	}

	// Compute the shift amount from pc delta to granularity-sized elements
	u32 granularity_shift = 31 - count_leading_zeros_32(disasm->opcode_alignment());

	// Number of pc steps to disassemble
	u32 count = pclength;

	if((count > opts.count) && (opts.count != 0))
		count = opts.count;

	// pc to string conversion
	std::function<std::string (offs_t pc)> pc_to_string;
	int aw = 32 - count_leading_zeros_32(pc_mask);
	bool is_octal = opts.octal; // Parameter?  Per-cpu config?
	if((flags & util::disasm_interface::PAGED2LEVEL) == util::disasm_interface::PAGED2LEVEL) {
		int bits1 = disasm->page_address_bits();
		int bits2 = disasm->page2_address_bits();
		int bits3 = aw - bits1 - bits2;
		offs_t sm1 = (1 << bits1) - 1;
		int sh2 = bits1;
		offs_t sm2 = (1 << bits2) - 1;
		int sh3 = bits1+bits2;

		if(is_octal) {
			int nc1 = (bits1+2)/3;
			int nc2 = (bits2+2)/3;
			int nc3 = (bits3+2)/3;
			pc_to_string = [nc1, nc2, nc3, sm1, sm2, sh2, sh3](offs_t pc) -> std::string {
				return util::string_format("%0*o:%0*o:%0*o",
										   nc3, pc >> sh3,
										   nc2, (pc >> sh2) & sm2,
										   nc1, pc & sm1);
			};
		} else {
			int nc1 = (bits1+3)/4;
			int nc2 = (bits2+3)/4;
			int nc3 = (bits3+3)/4;
			pc_to_string = [nc1, nc2, nc3, sm1, sm2, sh2, sh3](offs_t pc) -> std::string {
				return util::string_format("%0*x:%0*x:%0*x",
										   nc3, pc >> sh3,
										   nc2, (pc >> sh2) & sm2,
										   nc1, pc & sm1);
			};
		}

	} else if(flags & util::disasm_interface::PAGED) {
		int bits1 = disasm->page_address_bits();
		int bits2 = aw - bits1;
		offs_t sm1 = (1 << bits1) - 1;
		int sh2 = bits1;

		if(is_octal) {
			int nc1 = (bits1+2)/3;
			int nc2 = (bits2+2)/3;
			pc_to_string = [nc1, nc2, sm1, sh2](offs_t pc) -> std::string {
				return util::string_format("%0*o:%0*o",
										   nc2, pc >> sh2,
										   nc1, pc & sm1);
			};
		} else {
			int nc1 = (bits1+3)/4;
			int nc2 = (bits2+3)/4;
			pc_to_string = [nc1, nc2, sm1, sh2](offs_t pc) -> std::string {
				return util::string_format("%0*x:%0*x",
										   nc2, pc >> sh2,
										   nc1, pc & sm1);
			};
		}

	} else {
		int bits1 = aw;

		if(is_octal) {
			int nc1 = (bits1+2)/3;
			pc_to_string = [nc1](offs_t pc) -> std::string {
				return util::string_format("%0*o",
										   nc1, pc);
			};
		} else {
			int nc1 = (bits1+3)/4;
			pc_to_string = [nc1](offs_t pc) -> std::string {
				return util::string_format("%0*x",
										   nc1, pc);
			};
		}
	}

	// Lower/upper optional transform
	std::function<std::string (const std::string &)> tf;
	if(opts.lower)
		tf = [](const std::string &str) -> std::string {
			std::string result = str;
			std::transform(result.begin(), result.end(), result.begin(), [](char c) { return tolower(c); });
			return result;
		};
	else if(opts.upper)
		tf = [](const std::string &str) -> std::string {
			std::string result = str;
			std::transform(result.begin(), result.end(), result.begin(), [](char c) { return toupper(c); });
			return result;
		};
	else
		tf = [](const std::string &str) -> std::string {
			return str;
		};


	// Do the disassembly
	std::vector<dasm_line> dasm_lines;
	offs_t curpc = opts.basepc;
	for(u32 i=0; i < count;) {
		std::ostringstream stream;
		offs_t result = disasm->disassemble(stream, curpc, *popcodes, *pparams);
		offs_t len = result & util::disasm_interface::LENGTHMASK;
		dasm_lines.emplace_back(dasm_line{ curpc, len, stream.str() });
		curpc = next_pc(curpc, len);
		i += len;
	}

	// Compute the extrema
	offs_t max_len = 0;
	size_t max_text = 0;
	for(const auto &l : dasm_lines) {
		if(max_len < l.size)
			max_len = l.size;
		size_t s = l.dasm.size();
		if(max_text < s)
			max_text = s;
	}

	// Build the raw bytes generator and compute the field size
	max_len >>= granularity_shift;
	std::function<std::string (offs_t pc, offs_t size)> dump_raw_bytes;

	if(is_octal) {
		switch(granularity) {
		case 1:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%03o", base_buffer.r8(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 4) - 1;
			break;

		case 2:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%06o", base_buffer.r16(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 7) - 1;
			break;

		case 4:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%011o", base_buffer.r32(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 12) - 1;
			break;

		case 8:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%022o", base_buffer.r64(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 23) - 1;
			break;
		}
	} else {
		switch(granularity) {
		case 1:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%02x", base_buffer.r8(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 3) - 1;
			break;

		case 2:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%04x", base_buffer.r16(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 5) - 1;
			break;

		case 4:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%08x", base_buffer.r32(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 9) - 1;
			break;

		case 8:
			dump_raw_bytes = [step = disasm->opcode_alignment(), &next_pc, &base_buffer](offs_t pc, offs_t size) -> std::string {
				std::string result = "";
				for(offs_t i=0; i != size; i++) {
					if(i)
						result += ' ';
					result += util::string_format("%016x", base_buffer.r64(pc));
					pc = next_pc(pc, step);
				}
				return result;
			};
			max_len = (max_len * 17) - 1;
			break;
		}
	}

	if(opts.flipped) {
		if(opts.norawbytes)
			for(const auto &l : dasm_lines)
				util::stream_format(std::cout, "%-*s ; %s\n", max_text, tf(l.dasm), tf(pc_to_string(l.pc)));
		else
			for(const auto &l : dasm_lines)
				util::stream_format(std::cout, "%-*s ; %s: %s\n", max_text, tf(l.dasm), tf(pc_to_string(l.pc)), tf(dump_raw_bytes(l.pc, l.size >> granularity_shift)));
	} else {
		if(opts.norawbytes)
			for(const auto &l : dasm_lines)
				util::stream_format(std::cout, "%s: %s\n", tf(pc_to_string(l.pc)), tf(l.dasm));
		else
			for(const auto &l : dasm_lines)
				util::stream_format(std::cout, "%s: %-*s  %s\n", tf(pc_to_string(l.pc)), max_len, tf(dump_raw_bytes(l.pc, l.size >> granularity_shift)), tf(l.dasm));
	}

	return 0;
}


int main(int argc, char *argv[])
{
	// Parse options first
	options opts;
	if(parse_options(argc, argv, &opts))
		return 1;

	// Load the file
	void *data = nullptr;
	util::random_read::ptr file;
	u64 length = 0;
	if(std::strcmp(opts.filename, "-") != 0) {
		osd_file::ptr f;
		std::error_condition filerr = osd_file::open(std::string(opts.filename), OPEN_FLAG_READ, f, length);
		if(filerr) {
			std::fprintf(stderr, "Error opening file '%s' (%s)\n", opts.filename, filerr.message().c_str());
			return 1;
		}
		file = util::osd_file_read(std::move(f));
		if(!file) {
			std::fprintf(stderr, "Error when opening file '%s'\n", opts.filename);
			return 1;
		}
	}
	else
	{
#ifdef _WIN32
		if (_setmode(_fileno(stdin), _O_BINARY) == -1) {
#else
		if (!std::freopen(nullptr, "rb", stdin)) {
#endif
			std::fprintf(stderr, "Error reopening stdin in binary mode\n");
			return 1;
		}
		std::size_t allocated = 0x1000;
		data = std::malloc(allocated);
		while(!std::ferror(stdin) && !std::feof(stdin)) {
			if(length == allocated) {
				allocated <<= 1;
				data = std::realloc(data, allocated);
			}
			if(!data) {
				std::fprintf(stderr, "Error allocating buffer\n");
				return 1;
			}

			length += std::fread((u8 *)data + length, 1, allocated - length, stdin);
		}
		if(!length || (std::ferror(stdin) && !std::feof(stdin))) {
			std::fprintf(stderr, "Error reading from stdin\n");
			std::free(data);
			return 1;
		}
		file = util::ram_read(data, length);
		if(!file) {
			std::fprintf(stderr, "Error opening stdin as file\n");
			std::free(data);
			return 1;
		}
	}

	int result = disasm_file(*file, length, opts);

	file.reset();
	std::free(data);

	return result;
}
