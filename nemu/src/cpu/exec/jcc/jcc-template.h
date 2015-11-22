#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute () {
	if (cpu.ZF)
		cpu.eip += op_src->val;
	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
