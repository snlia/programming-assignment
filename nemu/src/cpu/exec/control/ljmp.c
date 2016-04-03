#include "cpu/exec/helper.h"
#include "ljmp.h"
#include "cpu/seg.h"

extern uint32_t lnaddr_read(lnaddr_t , size_t );

make_helper (ljmp) {
//    swaddr_t addr = instr_fetch (eip + 1, 4);
    uint16_t off = instr_fetch (eip + 5, 2);
    SEG_des seg_d;
    seg_d.val = lnaddr_read (off + cpu.GDTR_B, 4) | ((uint64_t) lnaddr_read (off + cpu.GDTR_B + 4, 4) << 32);
    printf ("%lx\n", seg_d.val);
    return 7;
}
