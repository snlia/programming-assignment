#include "common.h"
#include "memory/cache.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

static uint32_t seed;

static CacheSet_L1 L1set [1 << L1_SET];

static uint32_t read_B (CacheBlock *this, hwaddr_t addr, size_t len) {
    printf ("%x, %x\n", addr, this->buf[0]);
    return *((uint32_t *)this->buf + (addr & (BlockSize - 1))) & ((1 << (len << 3)) - 1);
}

static void write_B (CacheBlock *this, hwaddr_t addr, size_t len, uint32_t data) {
    memcpy ((this->buf + (addr & (BlockSize - 1))), (void *) (&data), len);
}
//common function in cache L1
static void flush_L1 (CacheSet_L1 *this) {
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i) this->block[i].valid = 0;
}

static uint32_t read_L1 (CacheSet_L1 *this, hwaddr_t addr, size_t len) {
    L1_addr temp;
	temp.addr = addr;
    uint32_t tag = temp.tag;
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i)
        if (this->block[i].valid && this->block[i].tag == tag)
            return this->block[i].read (this->block + i, addr, len);
    this->load (this, addr & (~(BlockSize - 1)));
    return this->read (this, addr, len);
}

static void load_L1 (CacheSet_L1 *this, hwaddr_t addr) {
    uint8_t buf [BlockSize];
    for (uint8_t i = 0; i < (BlockSize >> 2); ++i) *((uint32_t *) (buf + (i << 2))) = dram_read (addr + (i << 2), 4);
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i) if (!this->block[i].valid) {
        memcpy (this->block[i].buf, buf, BlockSize);
        this->block[i].tag = addr >> (OFF_SIZE + L1_SET);
        this->block[i].valid = 1;
        return ;
    }
    seed = (((buf[0] * seed) & L1_BMASK) + L1_SEED) & L1_BMASK;
    memcpy (this->block[seed].buf, buf, BlockSize);
    this->block[seed].tag = addr >> (OFF_SIZE + L1_SET);
    this->block[seed].valid = 1;
}

static void write_L1 (CacheSet_L1 *this, hwaddr_t addr, size_t len, uint32_t data) {
    L1_addr temp;
    temp.addr = addr;
    uint32_t tag = temp.tag;
    for (uint8_t i = 0; i < (1 << L1_BLOCK); ++i)
        if (this->block[i].valid && this->block[i].tag == tag)
            this->block[i].write (this->block + i, addr, len, data);
}

//public funciton

void L1_flush () {
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].flush (L1set + i);
}

uint32_t L1_read (hwaddr_t addr, size_t len) {
    L1_addr temp;
    temp.addr = addr;
    uint32_t off = temp.off;
    uint32_t set = temp.set;
    size_t Len = off + len <= 64 ? len : 64 - off;
    uint32_t result = L1set[set].read (L1set + set, addr, Len);
    if (len - Len)
        return result + (L1_read (addr + Len, len - Len) << Len);
    return result;
}

void L1_write (hwaddr_t addr, size_t len, uint32_t data) {
    dram_write (addr, len, data);
    L1_addr temp;
    temp.addr = addr;
    uint32_t off = temp.off;
    uint32_t set = temp.set;
    size_t Len = off + len <= 64 ? len : 64 - off;
    L1set[set].write (L1set + set, addr, Len, data);
    if (len - Len)
        L1_write (addr + Len, len - Len, data >> Len);
}

void L1_init () {
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].flush = flush_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].read = read_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].write = write_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i) L1set[i].load = load_L1;
    for (uint8_t i = 0; i < (1 << L1_SET); ++i)
        for (uint8_t j = 0; j < (1 << L1_BLOCK); ++j) {
            L1set[i].block[j].read = read_B;
            L1set[i].block[j].write = write_B;
        }
    L1_flush ();
}
