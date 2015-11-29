#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE result = op_src->val + (DATA_TYPE_S) (-op_dest->val);
	printf ("%x %x\n", op_src->val, (DATA_TYPE_S) (-op_dest->val));
	cpu.CF = (((uint64_t) (DATA_TYPE) op_src->val + (uint64_t) (DATA_TYPE) (-op_dest->val))  == result);
	cpu.SF = (result >> (DATA_BYTE * 8 - 1)) & 1;
	cpu.OF = 1 ^ cpu.CF ^ cpu.SF ^ ((op_src->val >> (DATA_BYTE * 8 - 1)) & 1) ^ (((-op_dest->val) >> (DATA_BYTE * 8 - 1)) & 1);
	cpu.ZF = !result;
	cpu.AF = ((op_src->val & 0x7) + ((-op_dest->val) & 0x7)) < 0x8;
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
