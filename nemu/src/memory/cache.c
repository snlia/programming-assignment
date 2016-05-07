#include "common.h"

#define L1_SIZE 0x10000

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef union {
    struct {
        uint32_t off : 6;
        uint32_t no : 3;
        uint32_t tag : 23;
    };
    uint32_t addr;
} L1_addr;

static uint32_t seed;

static uint8_t L1_cache [L1_SIZE];
static bool L1_vaild [L1_SIZE >> 6];
static uint32_t L1_tag [L1_SIZE >> 6];

void L1_flush ();
void L1_read ();
void L1_write ();

void L1_flush () {
    memset (L1_vaild, 0, sizeof (L1_vaild));
}

void read_L1 (hwaddr_t addr, void *data) {
    addr &= ~0x3;
    L1_addr tmp;
    tmp.addr = addr;

    for (int i = 0; i < )
}

uint32_t L1_read (hwaddr_t addr, size_t len) {
    L1_addr tmp; 
    tmp.val = addr;
    uint32_t offset = addr & 0x3;
    uint8_t temp [8];

    read_L1 (addr, temp);
    if(offset + len > BURST_LEN) read_L1 (addr, temp + 4);

    return unalign_rw(temp + offset, 4);
}

void L1_write (hwaddr_t addr, size_t len, uint32_t data) {

}

