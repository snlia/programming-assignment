#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute () {
	printf ("%x\n", cpu.eip);
	if (op_src->type == OP_TYPE_IMM)
		cpu.eip += (DATA_TYPE_S) op_src->val;
	else 
		cpu.eip = op_src->val;
	printf ("%x\n", cpu.eip);

	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
