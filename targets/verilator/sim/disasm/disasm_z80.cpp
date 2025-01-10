
#include <fstream>
#include <sstream>
#include "stdio.h"

#include "disasm_z80.h"
#include "sim_console.h"
#include <Vemu.h>

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <z80dasm.h>
#include <disasm/endianness.h>
#include <functional>
#include <cstring>

static DebugConsole console;
z80_disassembler disasm_z80;


void Disasm_Z80::StopOnMismatch(bool value) {
	stop_on_log_mismatch = value;
}
void Disasm_Z80::SuppressUntil(long value) {
	suppress_until = value;
}

void Disasm_Z80::LoadTrace(std::string filename) {

	// Load MAME debug log
	std::string line;
	std::ifstream fin(filename);
	while (getline(fin, line)) {
		log_mame.push_back(line);
	}
}

bool Disasm_Z80::WriteLog(std::string c_line) {
	// Compare with MAME log
	bool match = true;

	std::string c = fmt::format("{0:5d}]  CPU > " + c_line, instruction_count);
	//std::string c_start = c_line.substr(0, c_line.find_first_of(" "));

	if (instruction_count < log_mame.size()) {
		std::string m_line = log_mame.at(instruction_count);

		// Remove F from line
		//m_line = m_line.substr(0, 5) + m_line.substr(7, m_line.length() - 7);

//		std::string m = fmt::format("{0:5d}] MAME > " + m_line, instruction_count);
		//std::string m_start = m_line.substr(0, m_line.find_first_of(" "));

		if (stop_on_log_mismatch && m_line != c_line) {
			console.AddLog("DIFF at %d", instruction_count);
			console.AddLog(m_line.c_str());
			console.AddLog(c.c_str());
			match = false;
		}
	}
	if (match && suppress_until <= instruction_count && suppress_until > -1) {
		console.AddLog(c.c_str());
	}
	return match;
}

static constexpr auto le = util::endianness::little;
static constexpr auto be = util::endianness::big;

struct dasm_table_entry
{
	const char* name;
	util::endianness        endian;
	int8_t                  pcshift;
	std::function<util::disasm_interface* ()> alloc;
};


class unidasm_data_buffer : public util::disasm_interface::data_buffer
{
public:
	std::vector<util::disasm_interface::u8> data;
	util::disasm_interface::offs_t base_pc;
	util::disasm_interface::u32 size;

	std::string name;

	unidasm_data_buffer(const char* buffer_name);
	virtual ~unidasm_data_buffer() = default;

	//void decrypt(const unidasm_data_buffer& buffer, bool opcode);

	virtual util::disasm_interface::u8  r8(util::disasm_interface::offs_t pc) const override { return lr8(pc); }
	virtual util::disasm_interface::u16 r16(util::disasm_interface::offs_t pc) const override { return lr16(pc); }
	virtual util::disasm_interface::u32 r32(util::disasm_interface::offs_t pc) const override { return lr32(pc); }
	virtual util::disasm_interface::u64 r64(util::disasm_interface::offs_t pc) const override { return lr64(pc); }

private:
	std::function<util::disasm_interface::u8(util::disasm_interface::offs_t pc)> lr8;
	std::function<util::disasm_interface::u16(util::disasm_interface::offs_t pc)> lr16;
	std::function<util::disasm_interface::u32(util::disasm_interface::offs_t pc)> lr32;
	std::function<util::disasm_interface::u64(util::disasm_interface::offs_t pc)> lr64;

	//util::disasm_interface* disasm;
	//const dasm_table_entry* entry;

	template<typename T> const T* get_ptr(util::disasm_interface::offs_t pc) const {
		if (pc < base_pc)
			return nullptr;
		util::disasm_interface::offs_t delta = (pc - base_pc) * sizeof(T);
		if (delta >= size)
			return nullptr;
		return reinterpret_cast<const T*>(&data[delta]);
	}

	//template<typename T> T get(util::disasm_interface::offs_t pc) const {
	//	auto p = get_ptr<T>(pc);
	//	return p ? *p : 0;
	//}
};

unidasm_data_buffer::unidasm_data_buffer(const char* buffer_name) {
	data = std::vector<util::disasm_interface::u8>();
	size = 0x10000;
	data.resize(size);
	base_pc = 0;
	name = std::string(buffer_name);

	lr8 = [this](util::disasm_interface::offs_t pc) -> util::disasm_interface::u8 {
		const util::disasm_interface::u8* p = get_ptr<util::disasm_interface::u8>(pc);
		//console.AddLog("%s > lr8 pc=%X p=%X", name.c_str(), pc, p[0]);
		return p ? p[0] : 0x00;
	};
	lr16 = [this](util::disasm_interface::offs_t pc) -> util::disasm_interface::u16 {
		const util::disasm_interface::u8* p = get_ptr<util::disasm_interface::u8>(pc);
		///console.AddLog("%s > lr16 pc=%X p0=%X p1=%X", name.c_str(), pc, p[0], p[1]);
		return p ? p[0] | (p[1] << 8) : 0x0000;
	};
	lr32 = [this](util::disasm_interface::offs_t pc) -> util::disasm_interface::u32 {
		//console.AddLog("%s > lr32 pc=%X size=%d data.size=%d", name.c_str(), pc,  size, data.size());
		abort();
		return 0;
	};
	lr64 = [this](util::disasm_interface::offs_t pc) -> util::disasm_interface::u32 {
		//console.AddLog("%s > lr64 pc=%X size=%d data.size=%d", name.c_str(), pc, size, data.size());
		abort();
		return 0;
	};
}

unidasm_data_buffer opcode_buffer("opcode");
unidasm_data_buffer param_buffer("param");
unsigned char lastA;

bool ins_fresh;
bool last_m1_n;

bool Disasm_Z80::Process() {
	if (!*cen) {
		return false;
	}
	if (*reset_n == 0) {
		return false;
	}

	tstate = *cpu_tstate;

	// if address is in range of ROM, filll params with 'encrypted' data?
	if (*mreq_n == 0 && *Address >= 0x0000 && *Address < 0x0fff) {
		opcode_buffer.data.at(*Address) = *din;
		param_buffer.data.at(*Address) = *din;
	}
	bool matchBreak = false;
	//std::string log = fmt::format("T={0:X} M={1:X} I={2:X} | A={3:X} M1_n={4:X} MREQ_n={5:X} dIN={6:X} | {7:04X}: ", tstate, mstate, *Inst1, *A, *m1_n, *mreq_n, *din, *pc);
	//console.AddLog(log.c_str());
	//disasm_pc = *pc;

	if (!*m1_n) {

		bool ext = false;
		if ((tstate == 0x02) && cpustate_last != 0x02 && !*m1_n && ins_fresh) {

			if (*Inst1 == 0xCB || *Inst1 == 0xDD || *Inst1 == 0xED || *Inst1 == 0xFD) {
				// Extended 
				ext = true;	//console.AddLog("EXT");
			}
			else {

				// Time to dump last instruction data
				ins_fresh = false;

				std::ostringstream stream;
				//long af = *F + (*A << 8);
				disasm_z80.disassemble(stream, disasm_pc, opcode_buffer, param_buffer);
				std::string log = fmt::format("AF={0:02X} {1:04X}: ", *A, disasm_pc);
				log.append(stream.str());
				matchBreak = !Disasm_Z80::WriteLog(log);

				instruction_count++;
				disasm_is8bit = true;
			}
		}

		if (!ext && last_m1_n != *m1_n) {
			disasm_ins = *Inst1;
			disasm_pc = *pc;
			//std::string log = fmt::format("fresh ins: PC={0:X} INS={1:X}", disasm_ins, disasm_pc);
			//console.AddLog(log.c_str());
			ins_fresh = true;
		}
	}
	last_m1_n = *m1_n;

	cpustate_last = tstate;
	bool doBreak = (log_breakpoint > 0 && instruction_count >= log_breakpoint) || matchBreak;
	if (suppress_until > -1) {
		//*CpuDebug = instruction_count > suppress_until;
	}

	return doBreak;

}

Disasm_Z80::Disasm_Z80(DebugConsole c) {
	console = c;
}

Disasm_Z80::~Disasm_Z80() {

}
