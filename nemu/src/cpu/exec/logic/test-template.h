#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	uint32_t result = op_src->val & op_dest->val;
	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.CF = cpu.OF = 0;
	cpu.SF = (result >> (DATA_BYTE * 8 - 1)) & 1;
	cpu.ZF = !result;
    printf ("%x\n", cpu.ZF);
	result = result & 0xff;
	result = (result ^ result) & 0xf;
	result = (result ^ result) & 0x3;
	cpu.PF = (1 ^ result ^ result) & 1;
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
