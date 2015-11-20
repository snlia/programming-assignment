#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	cpu.esp -= ops_decoded.is_data_size_16 ? 2 : 4;
	swaddr_write(cpu.esp, ops_decoded.is_data_size_16 ? 2 : 4, op_src->val);
	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
