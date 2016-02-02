#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute () {
	DATA_TYPE result = op_dest->val + (DATA_TYPE_S) (-(op_src->val + 1));
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
//	cpu.CF = (((uint64_t) (DATA_TYPE) op_dest->val + (uint64_t) (DATA_TYPE) (-(op_src->val + 1))) == result);
    cpu.CF = (DATA_TYPE_S) op_dest->val < (DATA_TYPE_S) (op_src->val + 1);
	uint32_t off = (DATA_BYTE * 8 - 1);
	cpu.SF = (result >> off) & 1;
	cpu.OF = (((op_src->val >> off) & 1) ^ ((op_dest->val >> off) & 1)) && (cpu.SF ^ ((op_dest->val >> off) & 1));
	cpu.ZF = !result;
	cpu.AF = ((op_dest->val & 0x7) - (op_src->val & 0x7)) < 0x8;
	result = result & 0xff;
	result = (result ^ (result >> 4)) & 0xf;
	result = (result ^ (result >> 2)) & 0x3;
	cpu.PF = (1 ^ result ^ (result >> 1)) & 1;

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

