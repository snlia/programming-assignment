#include "cpu/exec/helper.h"

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr lgdt

static void do_execute () {
    printf ("%x\n", op_dest->val);
    print_asm_template2 ();
}

make_instr_helper (rm);
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

