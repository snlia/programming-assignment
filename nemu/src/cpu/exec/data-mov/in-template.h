#include "cpu/exec/template-start.h"

#define instr in

static void do_execute () {
    REG(R_EAX) = pio_read(op_src->val, DATA_BYTE);
	print_asm_template1();
}

make_helper (concat (in_, SUFFIX)) {
    REG(R_EAX) = pio_read(reg_w (R_DX), DATA_BYTE);
	print_asm("in dx (eax)");
    return 1;
}

make_instr_helper(i)
#include "cpu/exec/template-end.h"
