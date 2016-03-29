#ifndef __CACHE__
#define __CACHE__
#define BlockSize 64
#define OFF_SIZE 6
#define L1_SET 3
#define L1_TAG 23
#define L1_BLOCK 7
#define L1_BMASK 0x7f
#define L1_SEED 0x3d

#define L2_SET 4
#define L2_TAG 22
#define L2_BLOCK 12
#define L2_BMASK 0xfff
#define L2_SEED 0x3b

typedef struct CacheBlock {
    bool valid;
    uint32_t tag;
    uint8_t buf [BlockSize];
    uint32_t (* read) (struct CacheBlock *this, hwaddr_t addr, size_t len);
    void (* write) (struct CacheBlock *this, hwaddr_t addr, size_t len, uint32_t data);
} CacheBlock;

typedef struct CacheSet_L1 {
    CacheBlock block [1 << L1_BLOCK];
    void (* flush) (struct CacheSet_L1 *this);
    uint32_t (* read) (struct CacheSet_L1 *this, hwaddr_t addr, size_t len);
    void (* write) (struct CacheSet_L1 *this, hwaddr_t addr, size_t len, uint32_t data);
    void (* load) (struct CacheSet_L1 *this, hwaddr_t addr);
} CacheSet_L1;

typedef struct CacheSet_L1 {
    CacheBlock block [1 << L1_BLOCK];
    void (* flush) (struct CacheSet_L1 *this);
    uint32_t (* read) (struct CacheSet_L1 *this, hwaddr_t addr, size_t len);
    void (* write) (struct CacheSet_L1 *this, hwaddr_t addr, size_t len, uint32_t data);
    void (* load) (struct CacheSet_L1 *this, hwaddr_t addr);
} CacheSet_L1;

typedef union {
	struct {
       uint32_t off : OFF_SIZE; 
       uint32_t set : L1_SET;
       uint32_t tag : L1_TAG;
	};
	uint32_t addr;
} L1_addr;

typedef union {
	struct {
       uint32_t off : OFF_SIZE; 
       uint32_t set : L2_SET;
       uint32_t tag : L2_TAG;
	};
	uint32_t addr;
} L2_addr;

extern uint32_t L1_read (hwaddr_t, size_t );
extern void L1_write (hwaddr_t, size_t, uint32_t);
extern void L1_flush ();
extern void L1_init ();

#endif
