#include "cpu/exec/helper.h"
#include "iret.h"

#define instr iret
make_helper (iret) {
    //Protected mode return
    //Same privilege level
    //PE=1, VM=0 in flags image, RPL=CPL
    //EIP = Pop();
    cpu.esp += 4;
    cpu.eip = swaddr_read (cpu.esp, 4, SR_SS);
    cpu.esp += 4;

    //CS = Pop();
    cpu.CS = (uint16_t) swaddr_read (cpu.esp, 4, SR_SS);
    cache_SEG[SR_CS].val = lnaddr_read ((cpu.CS & 0xfff8) + cpu.GDTR_B, 4) | ((uint64_t) lnaddr_read ((cpu.CS & 0xfff8) + cpu.GDTR_B + 4, 4) << 32);
    cpu.esp += 4;

    //EFLAGS = Pop();
    cpu.eflags = swaddr_read (cpu.esp, 4, SR_SS);
    cpu.esp += 4;

    print_asm ("iret");
    return 0;
}
#undef instr

