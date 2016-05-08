#include "misc.h"

extern uint32_t dram_read(hwaddr_t, size_t);
extern void dram_write(hwaddr_t, size_t, uint32_t);


static uint32_t seed;

#define OFF_MASK ~0x3f

//L2 cache

#define L2_SIZE 0x400000
#define L2_OFF 0x6
#define L2_NO 0x4
#define NR_L2_OFF (1 << L2_OFF)
#define NR_L2_NO (1 << L2_NO) 
#define NR_L2_SET (L2_SIZE / NR_L2_OFF / NR_L2_NO)
#define L2_MASK (NR_L2_SET - 1)

typedef union {
    struct {
        uint32_t off : L2_OFF;
        uint32_t no : L2_NO;
        uint32_t tag : (32 - L2_OFF - L2_NO);
    };
    uint32_t addr;
} L2_addr;

static uint8_t L2_cache [NR_L2_NO][NR_L2_SET][NR_L2_OFF];
static bool L2_vaild [NR_L2_NO][NR_L2_SET];
static bool L2_dirty [NR_L2_NO][NR_L2_SET];
static uint32_t L2_tag [NR_L2_NO][NR_L2_SET];

void L2_flush () {
    memset (L2_vaild, 0, sizeof (L2_vaild));
    memset (L2_dirty, 0, sizeof (L2_dirty));
}

static void read_L2 (hwaddr_t addr, void *data) {
    addr &= ~0x3;
    L2_addr tmp;
    tmp.addr = addr;
//hit
    for (int i = 0; i < NR_L2_SET; ++i) if (L2_vaild[tmp.no][i] && L2_tag[tmp.no][i] == tmp.tag) {
        memcpy (data, L2_cache[tmp.no][i] + tmp.off, 4);
        return ;
    }
//miss and empty
    for (int i = 0; i < NR_L2_SET; ++i) if (!L2_vaild[tmp.no][i]) {
        for (int j = 0; j < NR_L2_OFF; j += 4) *((uint32_t *) (L2_cache[tmp.no][i] + j)) = dram_read (j | (addr & OFF_MASK), 4);
        memcpy (data, L2_cache[tmp.no][i] + tmp.off, 4);
        L2_vaild[tmp.no][i] = 1;
        L2_dirty[tmp.no][i] = 0;
        L2_tag[tmp.no][i] = tmp.tag;
        return ;
    }
//replace randomly
    seed = ((addr >> 2) + seed) & ~L2_MASK;
    uint32_t i = seed;
    for (int j = 0; j < NR_L2_OFF; j += 4) *((uint32_t *) (L2_cache[tmp.no][i] + j)) = dram_read (j | (addr & OFF_MASK), 4);
    memcpy (data, L2_cache[tmp.no][i] + tmp.off, 4);
    L2_vaild[tmp.no][i] = 1;
    L2_dirty[tmp.no][i] = 0;
    L2_tag[tmp.no][i] = tmp.tag;
}

uint32_t L2_read (hwaddr_t addr, size_t len) {
    uint32_t offset = addr & 0x3;
    uint8_t temp [8];

    read_L2 (addr, temp);
    if (offset + len > 4) read_L2 (addr + 4, temp + 4);

    return unalign_rw(temp + offset, 4);
}

static void write_L2 (hwaddr_t addr, void *data, uint8_t *mask) {
	L2_addr tmp;
    addr &= ~0x3;
	tmp.addr = addr;
//hit
    for (int i = 0; i < NR_L2_SET; ++i) if (L2_vaild[tmp.no][i] && L2_tag[tmp.no][i] == tmp.tag) 
        memcpy_with_mask(L2_cache[tmp.no][i] + tmp.off, data, 4, mask);
//miss and empty
    for (int i = 0; i < NR_L2_SET; ++i) if (!L2_vaild[tmp.no][i]) {
        for (int j = 0; j < NR_L2_OFF; j += 4) *((uint32_t *) (L2_cache[tmp.no][i] + j)) = dram_read (j | (addr & OFF_MASK), 4);
        memcpy_with_mask(L2_cache[tmp.no][i] + tmp.off, data, 4, mask);
        L2_vaild[tmp.no][i] = 1;
        L2_dirty[tmp.no][i] = 0;
        L2_tag[tmp.no][i] = tmp.tag;
        return ;
    }
//replace randomly
    seed = ((addr >> 2) + seed) & L2_MASK;
    uint32_t i = seed;
    hwaddr_t Addr = (L2_tag[tmp.no][i] << (L2_OFF + L2_NO)) | (tmp.no << L2_OFF);
    printf ("%x %x %x\n", i, L2_tag[tmp.no][i], tmp.no);
    puts ("check it");
    for (int j = 0; j < NR_L2_OFF; j += 4) {
        dram_write (Addr | j , 4, *((uint32_t *) (L2_cache[tmp.no][i] + j)));
        *((uint32_t *) (L2_cache[tmp.no][i] + j)) = dram_read (j | (addr & OFF_MASK), 4);
    }
    memcpy_with_mask(L2_cache[tmp.no][i] + tmp.off, data, 4, mask);
    L2_vaild[tmp.no][i] = 1;
    L2_dirty[tmp.no][i] = 0;
    L2_tag[tmp.no][i] = tmp.tag;
    puts ("Check it");
}

void L2_write (hwaddr_t addr, size_t len, uint32_t data) {
    dram_write (addr, len, data);
	uint32_t offset = addr & 0x3;
	uint8_t temp[8];
	uint8_t mask[8];
	memset(mask, 0, 8);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	write_L2 (addr, temp, mask);

	if(offset + len > 4) 
		write_L2 (addr + 4, temp + 4, mask + 4);
}

//L1 cache

#define L1_SIZE 0x10000
#define L1_OFF 0x6
#define L1_NO 0x3
#define NR_L1_OFF (1 << L1_OFF)
#define NR_L1_NO (1 << L1_NO) 
#define NR_L1_SET (L1_SIZE / NR_L1_OFF / NR_L1_NO)
#define L1_MASK (NR_L1_SET - 1)

typedef union {
    struct {
        uint32_t off : L1_OFF;
        uint32_t no : L1_NO;
        uint32_t tag : (32 - L1_OFF - L1_NO);
    };
    uint32_t addr;
} L1_addr;

static uint8_t L1_cache [NR_L1_NO][NR_L1_SET][NR_L1_OFF];
static bool L1_vaild [NR_L1_NO][NR_L1_SET];
static uint32_t L1_tag [NR_L1_NO][NR_L1_SET];

void L1_flush () {
    memset (L1_vaild, 0, sizeof (L1_vaild));
}

static void read_L1 (hwaddr_t addr, void *data) {
    addr &= ~0x3;
    L1_addr tmp;
    tmp.addr = addr;
//hit
    for (int i = 0; i < NR_L1_SET; ++i) if (L1_vaild[tmp.no][i] && L1_tag[tmp.no][i] == tmp.tag) {
        memcpy (data, L1_cache[tmp.no][i] + tmp.off, 4);
        return ;
    }
//miss and empty
    for (int i = 0; i < NR_L1_SET; ++i) if (!L1_vaild[tmp.no][i]) {
        for (int j = 0; j < NR_L1_OFF; j += 4) *((uint32_t *) (L1_cache[tmp.no][i] + j)) = L2_read (j | (addr & OFF_MASK), 4);
        memcpy (data, L1_cache[tmp.no][i] + tmp.off, 4);
        L1_vaild[tmp.no][i] = 1;
        L1_tag[tmp.no][i] = tmp.tag;
        return ;
    }
//replace randomly
    seed = ((addr >> 2) + seed) & L1_MASK;
    uint32_t i = seed;
    for (int j = 0; j < NR_L1_OFF; j += 4) *((uint32_t *) (L1_cache[tmp.no][i] + j)) = L2_read (j | (addr & OFF_MASK), 4);
    memcpy (data, L1_cache[tmp.no][i] + tmp.off, 4);
    L1_vaild[tmp.no][i] = 1;
    L1_tag[tmp.no][i] = tmp.tag;
}

uint32_t L1_read (hwaddr_t addr, size_t len) {
    uint32_t offset = addr & 0x3;
    uint8_t temp [8];

    read_L1 (addr, temp);
    if (offset + len > 4) read_L1 (addr + 4, temp + 4);

    return unalign_rw(temp + offset, 4);
}

static void write_L1 (hwaddr_t addr, void *data, uint8_t *mask) {
	L1_addr tmp;
    addr &= ~0x3;
	tmp.addr = addr;
//only write if hit
    for (int i = 0; i < NR_L1_SET; ++i) if (L1_vaild[tmp.no][i] && L1_tag[tmp.no][i] == tmp.tag) 
        memcpy_with_mask(L1_cache[tmp.no][i] + tmp.off, data, 4, mask);
}

void L1_write (hwaddr_t addr, size_t len, uint32_t data) {
    L2_write (addr, len, data);
	uint32_t offset = addr & 0x3;
	uint8_t temp[8];
	uint8_t mask[8];
	memset(mask, 0, 8);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	write_L1 (addr, temp, mask);

	if(offset + len > 4) 
		write_L1 (addr + 4, temp + 4, mask + 4);
}


