#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	uint32_t result = op_src->val - op_dest->val;
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

#include "cpu/exec/template-end.h"
