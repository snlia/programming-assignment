#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	DATA_TYPE result = op_src;
	cpu.esp -= DATA_TYPE;
	/* TODO: Update EFLAGS. */
	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
