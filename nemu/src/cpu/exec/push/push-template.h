#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	cpu.esp -= DATA_BYTE;
	swaddr_write(cpu.esp, DATA_BYTE, (DATA_TYPE) op_src->val);
	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
