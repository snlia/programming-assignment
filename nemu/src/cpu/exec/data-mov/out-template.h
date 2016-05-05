#include "cpu/exec/template-start.h"

#define instr out

static void do_execute () {
    pio_write(op_src->val, DATA_BYTE, (DATA_TYPE) REG(R_EAX));
	print_asm_template1();
}

make_helper (concat (out_, SUFFIX)) {
    pio_write(reg_w (R_DX), DATA_BYTE, (DATA_TYPE) REG(R_EAX));
	print_asm_template1();
    return 1;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
