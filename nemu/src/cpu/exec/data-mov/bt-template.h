#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;
    DATA_TYPE off = src % (DATA_BYTE << 3);
    cpu.CF = (dest >> off) & 1;
	print_asm_template2();
}

make_instr_helper(rm_imm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

