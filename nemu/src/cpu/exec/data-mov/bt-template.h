#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute () {
    cpu.CF = ((DATA_TYPE)op_dest->val >> (op_src->val & ((DATA_BYTE << 3) - 1))) & 1;
	print_asm_template2();
}

make_instr_helper(rm_imm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

