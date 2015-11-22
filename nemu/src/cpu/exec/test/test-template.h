#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	uint32_t result = op_src->val & op_dest->val;
	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.CF = cpu.OF = 0;
	cpu.SF = result >> 31;
	cpu.ZF = result ? 0 : 1;
	result = result & 0xff;
	result = (result ^ result) & 0xf;
	result = (result ^ result) & 0x3;
	cpu.PF = (result ^ result) & 1;
	print_asm_template1();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
