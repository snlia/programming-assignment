#include "cpu/exec/helper.h"
#include "ljmp.h"
#include "cpu/seg.h"
#include "cpu/reg.h"

extern uint32_t lnaddr_read(lnaddr_t , size_t );

make_helper (ljmp) {
    swaddr_t addr = instr_fetch (eip + 1, 4);
    uint16_t off = instr_fetch (eip + 5, 2);
    cache_SEG[SR_CS].val = lnaddr_read ((off & 0xfff8) + cpu.GDTR_B, 4) | ((uint64_t) lnaddr_read ((off & 0xfff8) + cpu.GDTR_B + 4, 4) << 32);
    cpu.eip = addr - 7;
    cpu.CS = off;
    print_asm("ljmp 0x%x,0x%x", off, addr);
    return 7;
}
