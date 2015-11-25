#include "cpu/exec/template-start.h"

#define instr add

static void do_execute () {
	DATA_TYPE result = op_dest->val + op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.CF = ((uint64_t) (DATA_TYPE) op_src->val + (uint64_t) (DATA_TYPE) op_dest->val) != result;
	cpu.OF = cpu.CF ^ cpu.SF;
	cpu.SF = (result >> (DATA_BYTE * 8 - 1)) & 1;
	cpu.ZF = !result;
	cpu.AF = ((op_src->val & 0x7) + (op_dest->val & 0x7)) < 0x8;
	result = result & 0xff;
	result = (result ^ result) & 0xf;
	result = (result ^ result) & 0x3;
	cpu.PF = (1 ^ result ^ result) & 1;

	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
