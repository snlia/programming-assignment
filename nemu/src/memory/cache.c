#include "common.h"

#define L1_SIZE 0x10000
#define L1_OFF 0x6
#define L1_NO 0x3
#define NR_L1_OFF (1 << L1_OFF)
#define NR_L1_SET 0x80
#define NR_L1_NO (1 << L1_NO) 
#define L1_MASK (~(NR_L1_OFF - 1))

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef union {
    struct {
        uint32_t off : L1_OFF;
        uint32_t no : L1_NO;
        uint32_t tag : (32 - L1_OFF - L1_NO);
    };
    uint32_t addr;
} L1_addr;

static uint32_t seed;

static uint8_t L1_cache [NR_L1_NO][NR_L1_SET][NR_L1_OFF];
static bool L1_vaild [NR_L1_NO][NR_L1_SET];
static uint32_t L1_tag [NR_L1_NO][NR_L1_SET];

void L1_flush ();
uint32_t L1_read (hwaddr_t, size_t);
void L1_write (hwaddr_t, size_t, uint32_t);

void L1_flush () {
    memset (L1_vaild, 0, sizeof (L1_vaild));
}

void read_L1 (hwaddr_t addr, void *data) {
    addr &= ~0x3;
    L1_addr tmp;
    tmp.addr = addr;

    for (int i = 0; i < NR_L1_SET; ++i) if (L1_vaild[tmp.no][i] && L1_tag[tmp.no][i] == tmp.tag) {
        memcpy (data, L1_cache[tmp.no][i] + tmp.off, 4);
        return ;
    }
    for (int i = 0; i < NR_L1_SET; ++i) if (!L1_vaild[tmp.no][i]) {
        for (int j = 0; j < NR_L1_OFF; j += 4) *((uint32_t *) (L1_cache[tmp.no][i] + j)) = dram_read (j | (addr & L1_MASK), 4);
        memcpy (data, L1_cache[tmp.no][i] + tmp.off, 4);
        return ;
    }
    seed = ((addr >> 2) + seed) & ~L1_MASK;
    uint32_t i = seed;
    for (int j = 0; j < NR_L1_OFF; j += 4) *((uint32_t *) (L1_cache[tmp.no][i] + j)) = dram_read (j | (addr & L1_MASK), 4);
    memcpy (data, L1_cache[tmp.no][i] + tmp.off, 4);
}

uint32_t L1_read (hwaddr_t addr, size_t len) {
    printf ("read : addr %x, len %zu\n", addr, len);
    uint32_t offset = addr & 0x3;
    uint8_t temp [8];

    read_L1 (addr, temp);
    if (offset + len > 4) read_L1 (addr + 4, temp + 4);

    printf ("dram %x\n", dram_read (addr, len));
    printf ("L1_read %x\n", unalign_rw(temp + offset, 4));
    return unalign_rw(temp + offset, 4);
}

void L1_write (hwaddr_t addr, size_t len, uint32_t data) {
    dram_write (addr, len, data);
}

