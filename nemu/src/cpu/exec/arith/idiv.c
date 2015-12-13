#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "idiv-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "idiv-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "idiv-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(idiv_rm)

make_helper(idiv_rm2a_b) {return idiv_rm_b (eip);}
make_helper(idiv_rm2a_w) {return idiv_rm_w (eip);}
make_helper(idiv_rm2a_l) {return idiv_rm_l (eip);}

make_helper_v(idiv_rm2a)
