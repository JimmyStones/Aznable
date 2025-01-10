
#include <fstream>
#include <sstream>
#include "stdio.h"

#include "disasm_6809.h"
#include "sim_console.h"
#include <Vemu.h>

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <6x09dasm.h>
#include <disasm/endianness.h>
#include <functional>
#include <cstring>

static DebugConsole console;
m6809_disassembler disasm_6809;

// CPU tracking variables

void Disasm_6809::StopOnMismatch(bool value) {
	stop_on_log_mismatch = value;
}
void Disasm_6809::SuppressUntil(long value) {
	suppress_until = value;
}

void Disasm_6809::LoadTrace(std::string filename) {

	// Load MAME debug log
	std::string line;
	std::ifstream fin(filename);
	while (getline(fin, line)) {
		log_mame.push_back(line);
	}
}

bool Disasm_6809::WriteLog(std::string c_line) {
	// Compare with MAME log
	bool match = true;

	std::string c = fmt::format("{0:5d}]  CPU > " + c_line, instruction_count_6809);
	//std::string c_start = c_line.substr(0, c_line.find_first_of(" "));

	if (instruction_count_6809 < (log_mame).size()) {
		std::string m_line = (log_mame).at(instruction_count_6809);
		std::string m = fmt::format("{0:5d}] MAME > " + m_line, instruction_count_6809);
		//std::string m_start = m_line.substr(0, m_line.find_first_of(" "));

		if (stop_on_log_mismatch && m_line != c_line) {
			console.AddLog("DIFF at %d", instruction_count_6809);
			console.AddLog(m.c_str());
			console.AddLog(c.c_str());
			match = false;
		}
	}
	if (match && suppress_until < instruction_count_6809 && suppress_until>-1) {
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


class unidasm_data_buffer_6809 : public util::disasm_interface::data_buffer
{
public:
	std::vector<util::disasm_interface::u8> data;
	util::disasm_interface::offs_t base_pc;
	util::disasm_interface::u32 size;

	std::string name;

	unidasm_data_buffer_6809(const char* buffer_name);
	virtual ~unidasm_data_buffer_6809() = default;

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

unidasm_data_buffer_6809::unidasm_data_buffer_6809(const char* buffer_name) {
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
		//console.AddLog("%s > lr16 pc=%X p0=%X p1=%X", name.c_str(), pc, p[0], p[1]);
		return p ? p[1] | (p[0] << 8) : 0x0000;
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

unidasm_data_buffer_6809 opcode_buffer_6809("opcode");
unidasm_data_buffer_6809 param_buffer_6809("param");

bool Disasm_6809::Process() {
	cpustate = *CpuState;

	// if address is in range of ROM, filll params with 'encrypted' data?
	if (*Address >= 0xa000 && *Address < 0x10000) {
		opcode_buffer_6809.data.at(*Address) = *unencryptedData;
		param_buffer_6809.data.at(*Address) = *encryptedData;
	}

	if (cpustate == 8 && cpustate_last == 8) {
		lastA = *A;
		disasm_ins = *Inst1;
	}

	bool matchBreak = false;

	if (cpustate == 4 && cpustate_last != 4) {
		// Time to dump last instruction data

		if (disasm_ins != 0) {

			std::ostringstream stream;
			disasm_6809.disassemble(stream, disasm_pc, opcode_buffer_6809, param_buffer_6809);
			std::string log = fmt::format("A={0:X} | {1:04X}: ", lastA, disasm_pc);
			log.append(stream.str());
			matchBreak = !WriteLog(log);

			instruction_count_6809++;
		}
		disasm_pc = *pc;
		disasm_ins = 0;
		disasm_is8bit = true;
	}
	cpustate_last = cpustate;
	bool doBreak = (log_breakpoint > 0 && instruction_count_6809 >= log_breakpoint) || matchBreak;
	if (suppress_until > -1) {
		*CpuDebug = instruction_count_6809 > suppress_until;
	}

	return doBreak;

}

Disasm_6809::Disasm_6809(DebugConsole c) {
	console = c;
}

Disasm_6809::~Disasm_6809() {

}
