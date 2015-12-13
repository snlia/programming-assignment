#include "cpu/exec/template-start.h"
#include "monitor/elf.h"

#define instr jmp

static void do_execute () {
	if (op_src->type == OP_TYPE_IMM)
		cpu.eip += (DATA_TYPE_S) op_src->val;
	else 
		cpu.eip = op_src->val;

#ifdef DEBUG
	char s[] = "jmp";
	print_asm_template5(cpu.eip, s);
#endif
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
