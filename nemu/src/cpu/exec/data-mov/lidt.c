#include "cpu/exec/helper.h"

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr lidt

static void do_execute () {
    cpu.IDTR_L = lnaddr_read (op_src->addr, 2);
    cpu.IDTR_B = lnaddr_read (op_src->addr + 2, 4);
    print_asm_template2 ();
}

make_instr_helper (rm);
#include "cpu/exec/template-end.h"
#undef DATA_BYTE


