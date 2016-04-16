#include "cpu/exec/helper.h"
#include <setjmp.h>
#include "int.h"

#define instr int

jmp_buf jbuf;

extern void raise_intr(uint8_t );

make_helper (int_imm) {
    uint8_t NO = instr_fetch (eip + 1, 1);
    cpu.eip += 2;
    if (setjmp (jbuf) == 0)
        raise_intr (NO);
    print_asm ("int");
    return 0;
}

#undef instr
