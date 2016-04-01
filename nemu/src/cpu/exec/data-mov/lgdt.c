#include "cpu/exec/helper.h"
#include "lgdt.h"
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr lgdt
void static do_execute () {
    print_asm ("lgdt");
}
make_instr_helper (i);

#include "cpu/exec/template-end.h"
#undef DATA_BYTE
