#include "common.h"
#include "memory/cache.h"
#include "cpu/reg.h"
#include "cpu/seg.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
extern int is_mmio (hwaddr_t);
extern uint32_t mmio_read (hwaddr_t, size_t, int);
extern void mmio_write (hwaddr_t, size_t, uint32_t, int);

/* Memory accessing interfaces */

typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

extern PTE TLB_read (lnaddr_t addr); 

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    int tmp = is_mmio (addr);
    if (!~tmp) {
#ifdef CACHE 
    return L1_read (addr, len);
#else
    return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif
    }
    else return mmio_read (addr, len, tmp) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    int tmp = is_mmio (addr);
    if (!~tmp) {
#ifdef CACHE 
    L1_write (addr, len, data);
#else
    dram_write(addr, len, data);
#endif
    }
    else mmio_write (addr, len, data, tmp);
}

hwaddr_t page_translate (lnaddr_t addr) {
    if (!cpu.CR0.protect_enable || !cpu.CR0.paging) return addr;
    PTE pte = TLB_read (addr);
    return (addr & 0xfff) | (pte.page_frame << 12);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
    assert(len == 1 || len == 2 || len == 4);
    if ((addr + len - 1) > (addr | 0xfff)) {
        /* this is a special case, you can handle it later. */
        uint32_t result = 0;
        puts ("hahaha");
        for (int i = 0; i < len; ++i)
            result |= lnaddr_read (addr + i, 1) << (i << 3);
        printf ("%x\n", result);
        return result;
    }
    else {
        hwaddr_t hwaddr = page_translate(addr);
        return hwaddr_read(hwaddr, len);
    }
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
    assert(len == 1 || len == 2 || len == 4);
    if ((addr + len - 1) > (addr | 0xfff)) {
        /* this is a special case, you can handle it later. */
        puts ("heiheihei");
        for (int i = 0; i < len; ++i, data >>= 8) lnaddr_write (addr + i, 1, data & 0xff);
        return ;
    }
    else {
        hwaddr_t hwaddr = page_translate(addr);
        return hwaddr_write(hwaddr, len, data);
    }
}

lnaddr_t seg_translate (swaddr_t swaddr, uint8_t sreg) {
    if (!cpu.CR0.protect_enable) return swaddr;
    Assert (!(cpu.CR0.protect_enable) || (cpu.spr[sreg].index << 3) < cpu.GDTR_L, "At addr 0x%x, seg : %x, eip : 0x%x\n", swaddr, sreg, cpu.eip);
    Assert ((seg_limit (cache_SEG[sreg]) << 12) > swaddr, "At addr 0x%x, seg : %x, eip : 0x%x\n", swaddr, sreg, cpu.eip);
    return swaddr + seg_base (cache_SEG[sreg]);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
    assert(len == 1 || len == 2 || len == 4);
#endif
#ifndef O1 
    addr = seg_translate(addr, sreg);
#endif
    return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, size_t sreg) {
#ifdef DEBUG
    assert(len == 1 || len == 2 || len == 4);
#endif
#ifndef O1
    addr = seg_translate(addr, sreg);
#endif
    lnaddr_write(addr, len, data);
}

