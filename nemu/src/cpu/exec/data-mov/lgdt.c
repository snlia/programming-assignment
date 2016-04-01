#include "cpu/exec/helper.h"

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr lgdt

static void do_execute () {
    print_asm_template1 ();
}

make_instr_helper (i);
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

