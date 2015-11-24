#include "cpu/exec/template-start.h"

#define instr add

static void do_execute () {
	DATA_TYPE result = op_dest->val + op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.CF = ((uint64_t) op_src->val - (uint64_t) op_dest->val) != (result & 0xffffffff);
	cpu.OF = cpu.CF ^ cpu.SF;
	cpu.SF = (result >> 31) & 1;
	cpu.ZF = !result;
	cpu.AF = ((op_src->val & 0x7) - (op_dest->val & 0x7)) < 0x8;
	result = result & 0xff;
	result = (result ^ result) & 0xf;
	result = (result ^ result) & 0x3;
	cpu.PF = (1 ^ result ^ result) & 1;

	print_asm_template2();
}

make_instr_helper(r_ib)
make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
