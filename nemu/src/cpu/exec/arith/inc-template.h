#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.SF = (result >> (DATA_BYTE * 8 - 1)) & 1;
	cpu.OF = (DATA_TYPE) op_src->val == (((DATA_TYPE) (-1)) ^ (1 << (DATA_BYTE * 8 - 1)));
	cpu.ZF = !result;
	cpu.AF = ((op_src->val & 0x7) + (op_dest->val & 0x7)) < 0x8;
	result = result & 0xff;
	result = (result ^ result) & 0xf;
	result = (result ^ result) & 0x3;
	cpu.PF = (1 ^ result ^ result) & 1;


	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
