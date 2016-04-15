#include <setjmp.h>
#include "cpu/exec/helper.h"
extern jmp_buf jbuf;

typedef struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
} GateDesc;

void raise_intr(uint8_t NO) {
    /* TODO: Trigger an interrupt/exception with ``NO''.
     *   * That is, use ``NO'' to index the IDT.
     *       */
//    GateDesc tmp = (lnaddr_read (cpu.IDTR_B | (NO << 3), 4));
    /* Jump back to cpu_exec() */
    longjmp(jbuf, 1);
}
