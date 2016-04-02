#include "cpu/exec/helper.h"
#include "ljmp.h"

make_helper (ljmp) {
    printf ("%x %x\n", instr_fetch (eip + 1, 4), instr_fetch (eip + 5, 2));
    return 6;
}
