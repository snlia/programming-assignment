#include "cpu/exec/helper.h"

extern void pio_write(ioaddr_t, size_t, uint32_t);

#define DATA_BYTE 1
#include "out-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "out-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "out-template.h"
#undef DATA_BYTE

/* for outstruction encodoutg overloadoutg */

make_helper_v(out_i)
make_helper_v(out)
