#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	cpu.esp -= ops_decoded.is_data_size_16 ? 2 : 4;
	swaddr_write(cpu.esp, DATA_BYTE, cpu.eip);
	cpu.eip += op_src.val;
	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
