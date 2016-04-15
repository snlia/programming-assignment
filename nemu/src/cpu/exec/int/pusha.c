#include "cpu/exec/helper.h"
#include "pusha.h"

#define instr pusha

make_helper (pusha) {
    uint32_t tmp = cpu.esp;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_EAX), SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_ECX), SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_EDX), SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_EBX), SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, tmp, SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_EBP), SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_ESI), SR_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, reg_l(R_EDI), SR_SS);
    print_asm ("pusha");
    return 1;
}

#undef instr

