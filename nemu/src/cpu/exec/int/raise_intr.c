#include "cpu/exec/helper.h"
#include <setjmp.h>
extern jmp_buf jbuf;

typedef union {
    struct {
        uint32_t offset_15_0      : 16;
        uint32_t segment          : 16;
        uint32_t pad0             : 8;
        uint32_t type             : 4;
        uint32_t system           : 1;
        uint32_t privilege_level  : 2;
        uint32_t present          : 1;
        uint32_t offset_31_16     : 16;
    };
    uint64_t val;
} GateDesc;

void raise_intr(uint8_t NO) {
    /* TODO: Trigger an interrupt/exception with ``NO''.
     *   * That is, use ``NO'' to index the IDT.
     *       */
    //The following operational description applies not only to the INT n and INTO instructions, but also to external interrupts and exceptions.
    //Trap interrupt gate
    
    //IDTDescriptor = ReadSegmentSelector();
    GateDesc tmp;
    tmp.val = (lnaddr_read (cpu.IDTR_B + (NO << 3), 4) | ((uint64_t) lnaddr_read (cpu.IDTR_B + (NO << 3) + 4, 4) << 32));
    //PE = 1, interrupt or trap gate, DPL >= CPL
    //Intra-privilege level interrupt
    //PE == 1, DPL == CPL or conforming segment
    
    //Push(EFLAGS);
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eflags, SR_SS);

    //Push(FarPointer(ReturnInstruction)); //3 words padded to 4
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.CS, SR_SS);
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eip, SR_SS);

    //CS:EIP = Gate(CS:EIP); //segment descriptor information also loaded
    cpu.CS = tmp.segment;
    cache_SEG[SR_CS].val = lnaddr_read ((tmp.segment & 0xfff8) + cpu.GDTR_B, 4) | ((uint64_t) lnaddr_read ((tmp.segment & 0xfff8) + cpu.GDTR_B + 4, 4) << 32);
    cpu.eip = tmp.offset_15_0 | (tmp.offset_31_16 << 16);

    //Push(ErrorCode); //if any
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, NO, SR_SS);

    if (tmp.type == 0xe) cpu.IF = 0; //interrupt flag is set to 0: disabled
    cpu.TF = 0;
    cpu.NT = 0;
    cpu.VM = 0;
    cpu.RF = 0;
    /* Jump back to cpu_exec() */
    longjmp(jbuf, 1);
}
