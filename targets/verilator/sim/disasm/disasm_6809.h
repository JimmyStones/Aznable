#pragma once
#include "verilated_heavy.h"
#include <sim_console.h>


struct Disasm_6809 {
public:

	CData* CpuDebug;
	SData* Address;
	CData* A;
	CData* encryptedData;
	CData* unencryptedData;
	CData* CpuState;
	CData* Inst1;
	SData* pc;

	bool Process(void);
	void LoadTrace(std::string filename);
	bool WriteLog(std::string c_line);
	void StopOnMismatch(bool value);
	void SuppressUntil(long value);

	Disasm_6809(DebugConsole c);
	~Disasm_6809();

	int cpustate;
	int cpustate_last;
	int disasm_pc;
	int disasm_ins;
	int disasm_is8bit;
	int disasm_add8;
	int disasm_add16;
	unsigned char lastA;
	std::vector<std::string> log_mame;
	std::vector<std::string> log_cpu;
	long instruction_count_6809;
	long log_breakpoint;
	long suppress_until;
	bool stop_on_log_mismatch = true;
};