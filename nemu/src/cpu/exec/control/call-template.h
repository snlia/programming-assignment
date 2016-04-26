#include "cpu/exec/template-start.h"
#include "monitor/elf.h"

#define instr call

static void do_execute () {
	cpu.esp -= DATA_BYTE;
	swaddr_write(cpu.esp, DATA_BYTE, (DATA_TYPE) cpu.eip, SR_SS);
    switch (ops_decoded.opcode & 0xff) 
    {
        case 0xe8 :
            cpu.eip += (DATA_TYPE_S) op_src->val;
            break;
        case 0xff :
            cpu.eip = (DATA_TYPE) op_src->val;
            break;
        default :
            assert (0);
    }
#if DATA_BYTE == 2
	cpu.eip &= 0x0000ffff;
#endif
	/* TODO: Update EFLAGS. */
//	panic("please implement me");
#ifdef DEBUG
	char s [] = "call";
	print_asm_template4 (cpu.eip, s);
#endif
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
