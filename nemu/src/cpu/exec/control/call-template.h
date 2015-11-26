#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	cpu.esp -= DATA_BYTE;
	swaddr_write(cpu.esp, DATA_BYTE, (DATA_TYPE) cpu.eip);
	cpu.eip += (DATA_TYPE_S) op_src->val;
	if (ops_decoded.is_data_size_16) cpu.eip &= 0x0000ffff;
	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
