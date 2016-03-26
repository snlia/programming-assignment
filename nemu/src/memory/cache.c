#include "common.h"
#include "cache.h"

static uint32_t seed;

static CacheSet_L1 L1set [1 << L1_SET];

static uint32_t read_B (CacheBlock *this, hwaddr_t addr, size_t len) {
    return unalign_rw (this->buf + addr & (BlockSize - 1), len);
}
//common function in cache L1
static void flush_L1 (CacheSet_L1 *this) {
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i) this->block[i].valid = 0;
}

static uint32_t read_L1 (CacheSet_L1 *this, hwaddr_t addr, size_t len) {
	temp.addr = addr;
    uint32_t off = temp.off;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i)
        if (this->block[i].valid && this->block[i].tag == tag)
            return this->block[i].read (addr, len);
    this->load (addr & (~(BlockSize - 1)));
    return this->read_L1 (addr, len);
}

static void load_L1 (CacheSet_L1 *this, hwaddr_t addr) {
    uint8_t buf [BlockSize];
    for (uint8_t i = 0; i < (BlockSize >> 2); ++i) *((uint32_t *) (buf + i << 2)) = dram_read (addr + i << 2, 4);
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i) if (!this->block[i].valid) {
        memcpy (this->block[i].buf, buf, BlockSize);
        return ;
    }
    seed = ((*((uint16_t *) buf) & L1_BMASK * seed) + L1_SEED) & L1_BMASK;
    memcpy (this->block[seed].buf, buf, BlockSize);
}

//public funciton

void L1_flush () {
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].flush;
}

uint32_t L1_read (hwaddr_t addr, size_t len) {
    L1_addr temp;
	temp.addr = addr;
    uint32_t off = temp.off;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    size_t Len = off + len <= 64 ? len : 64 - off;
    uint32_t result = L1set[set].read (addr, Len);
    if (len - Len)
        return result + (L1_read (addr + Len, len - Len) << Len);
    return result;
}

void L1_init () {
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].flush = flush_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].read = read_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].write = write_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].load = load_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i)
        for (uint8_t j = 0; j < (1 << L1_BLOCK); ++j)
            L1set[i].block[j].read = read_B;
    seed = 0;
    L1_flush ();
}
