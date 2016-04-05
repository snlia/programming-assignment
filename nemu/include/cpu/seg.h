#ifndef __SEG_H__
#define __SEG_H__

#include "common.h"

typedef union {
    struct {
        uint16_t limit0;
        uint16_t base0;
        uint8_t base1;
        uint8_t A : 1;
        uint8_t TYPE : 3;
        uint8_t ONE : 1;
        uint8_t DPL : 2;
        uint8_t P : 1;
        uint8_t limit1 : 4;
        uint8_t AVL : 1;
        uint8_t O : 1;
        uint8_t X : 1;
        uint8_t G : 1;
        uint8_t base2;
    };
    uint64_t val;
} SEG_des;

SEG_des cache_SEG [4];

#define seg_base(x) ((x).base0 | ((x).base1 << 16) | ((x).base2 << 24))
#define seg_limit(x) ((x).limit0 | ((x).limit1 << 16))

#endif
