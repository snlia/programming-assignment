#include "common.h"
#include "memory/cache.h"
#include "cpu/reg.h"
#include "cpu/seg.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
#ifdef CACHE 
    return L1_read (addr, len);
#else
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
#ifdef CACHE 
    L1_write (addr, len, data);
#else
	dram_write(addr, len, data);
#endif
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

lnaddr_t seg_translate (swaddr_t swaddr, uint8_t sreg) {
    if (!cpu.CR0.protect_enable) return swaddr;
	assert (!(cpu.CR0.protect_enable) || (cpu.spr[sreg].index << 3) < cpu.GDTR_L);
    assert ((seg_limit (cache_SEG[sreg]) << 12) > swaddr);
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

