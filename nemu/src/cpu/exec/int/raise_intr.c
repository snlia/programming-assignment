#include "cpu/exec/helper.h"
#include <setjmp.h>
extern jmp_buf jbuf;

typedef union {
    struct {
        uint32_t offset_15_0      : 16;
        uint32_t segment          : 16;
        uint32_t pad0             : 8;
        uint32_t type             : 4;
        uint32_t system           : 1;
        uint32_t privilege_level  : 2;
        uint32_t present          : 1;
        uint32_t offset_31_16     : 16;
    };
    uint64_t val;
} GateDesc;

void raise_intr(uint8_t NO) {
    /* TODO: Trigger an interrupt/exception with ``NO''.
     *   * That is, use ``NO'' to index the IDT.
     *       */
    GateDesc tmp;
    tmp.val = (lnaddr_read (cpu.IDTR_B + (NO << 3), 4) | ((uint64_t) lnaddr_read (cpu.IDTR_B + (NO << 3) + 4, 4) << 32));
    cpu.CS = tmp.segment;
    cache_SEG[SR_CS].val = lnaddr_read ((tmp.segment & 0xfff8) + cpu.GDTR_B, 4) | ((uint64_t) lnaddr_read ((tmp.segment & 0xfff8) + cpu.GDTR_B + 4, 4) << 32);
    cpu.eip = tmp.offset_15_0 | (tmp.offset_31_16 << 16);
    /* Jump back to cpu_exec() */
    longjmp(jbuf, 1);
}
