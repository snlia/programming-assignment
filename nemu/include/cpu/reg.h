#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { SR_ES, SR_CS, SR_SS, SR_DS };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
    union {
        union {
            uint32_t _32;
            uint16_t _16;
            uint8_t _8[2];
        } gpr[8];

        /* Do NOT change the order of the GPRs' definitions. */
        struct {
            uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        };
    };
    swaddr_t eip;
    union
    {
        struct
        {
            uint32_t CF			:1;
            uint32_t void0		:1;
            uint32_t PF 		:1;
            uint32_t void1 		:1;
            uint32_t AF 		:1;
            uint32_t void2 		:1;
            uint32_t ZF 		:1;
            uint32_t SF 		:1;
            uint32_t TF 		:1;
            uint32_t IF 		:1;
            uint32_t DF 		:1;
            uint32_t OF 		:1;
            uint32_t IOPL 		:2;
            uint32_t NT 		:1;
            uint32_t void3 		:1;
            uint32_t RF 		:1;
            uint32_t VM 		:1;
            uint32_t void4 		:14;
        };
        uint32_t eflags;
    };
    uint16_t GDTR_L;
    uint32_t GDTR_B;
    uint16_t IDTR_L;
    uint32_t IDTR_B;
    union {
        struct {
            uint32_t protect_enable      : 1;
            uint32_t monitor_coprocessor : 1;
            uint32_t emulation           : 1;
            uint32_t task_switched       : 1;
            uint32_t extension_type      : 1;
            uint32_t numeric_error       : 1;
            uint32_t pad0                : 10;
            uint32_t write_protect       : 1; 
            uint32_t pad1                : 1; 
            uint32_t alignment_mask      : 1;
            uint32_t pad2                : 10;
            uint32_t no_write_through    : 1;
            uint32_t cache_disable       : 1;
            uint32_t paging              : 1;
        };
        uint32_t val;
    } CR0;

    union {
        struct {
            uint32_t pad0                : 3;
            uint32_t page_write_through  : 1;
            uint32_t page_cache_disable  : 1;
            uint32_t pad1                : 7;
            uint32_t page_directory_base : 20;
        };
        uint32_t val;
    } CR3;

    union {
        union {
            struct {
                uint16_t rpl : 2;
                uint16_t ti : 1;
                uint16_t index : 13;
            };
            uint16_t val;
        } spr [4];
        struct {
            uint16_t ES, CS, SS, DS;
        };
    };

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
    assert(index >= 0 && index < 8);
    return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[(index) >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
