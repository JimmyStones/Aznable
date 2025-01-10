#pragma once
#include "verilated_heavy.h"
#include <sim_console.h>


struct Disasm_Z80 {
public:

	CData* reset_n;
	CData* cen;
	CData* CpuDebug;
	SData* Address;
	CData* A;
	CData* F;
	CData* m1_n;
	CData* mreq_n;
	CData* din;
	CData* cpu_tstate;
	CData* cpu_mstate;
	CData* Inst1;
	SData* pc;

	bool Process(void);
	void LoadTrace(std::string filename);
	bool WriteLog(std::string c_line);
	void StopOnMismatch(bool value);
	void SuppressUntil(long value);

	Disasm_Z80(DebugConsole c);
	~Disasm_Z80();

private:

	int tstate;
	int mstate;
	int cpustate_last;
	int disasm_pc;
	int disasm_ins;
	int disasm_is8bit;
	int disasm_add8;
	int disasm_add16;
	std::vector<std::string> log_mame;
	std::vector<std::string> log_cpu;
	long instruction_count;
	long log_breakpoint;
	long suppress_until;
	bool stop_on_log_mismatch = true;
};