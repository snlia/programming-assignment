#include "common.h"
#include "cpu/reg.h"

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

typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1; 
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;

#define PAGE_SIZE 12
#define TLB_SEED 31
#define TAG_SIZE (32 - PAGE_SIZE)
#define SET_SIZE 64

typedef union {
    struct {
        uint32_t off : PAGE_SIZE;
        uint32_t tag : TAG_SIZE;
    };
    uint32_t val;
} TLB_addr;

typedef struct {
    bool valid;
    uint32_t tag;
    PTE val;
} set_TLB;

static set_TLB TLBset [SET_SIZE];
static uint32_t seed = 0;
extern uint32_t hwaddr_read(hwaddr_t , size_t );

PTE TLB_read (lnaddr_t addr) {
    TLB_addr tmp;
    tmp.val = addr;
    for (int i = 0; i < SET_SIZE; ++i) if (TLBset[i].valid && TLBset[i].tag == tmp.tag)
        return TLBset[i].val;
    PDE pde;
    pde.val = hwaddr_read (((addr >> 20) & 0xffc) + ((cpu.CR3.page_directory_base) << 12), 4);
    if (!pde.present) Assert (0, "PDE no present at addr :%x\n", addr);
    PTE pte;
    pte.val = hwaddr_read (((addr >> 10) & 0xffc) + (pde.page_frame << 12), 4);
    if (!pte.present) Assert (0, "PTE no present at addr :%x\n", addr);
    for (int i = 0; i < SET_SIZE; ++i) if (!TLBset[i].valid) {
        TLBset[i].val = pte;
        TLBset[i].tag = tmp.tag;
        TLBset[i].valid = 1;
        return pte;
    }
    seed = ((tmp.tag * seed)  + TLB_SEED) & (SET_SIZE - 1);
    TLBset[seed].val = pte;
    TLBset[seed].tag = tmp.tag;
    TLBset[seed].valid = 1;
    return pte;
}

void TLB_flush () {
    for (int i = 0; i < SET_SIZE; ++i)  TLBset[i].valid = 0;
}
