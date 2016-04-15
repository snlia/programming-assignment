#include "cpu/exec/helper.h"
#include "popa.h"

#define instr pusha

make_helper (pusha) {
    print_asm ("popa");
	reg_l(R_EDI) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 4;
	reg_l(R_ESI) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 4;
	reg_l(R_EBP) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 8;
	reg_l(R_EBX) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 4;
	reg_l(R_EDX) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 4;
	reg_l(R_ECX) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 4;
	reg_l(R_EAX) = swaddr_read (cpu.esp, 4, SR_SS);
	cpu.esp += 4;
    return 1;
}

#undef instr


