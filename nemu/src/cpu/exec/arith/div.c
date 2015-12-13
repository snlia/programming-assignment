#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "div-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "div-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "div-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(div_rm)

make_helper(div_rm2a_b) {return div_rm_b (eip);}
make_helper(div_rm2a_w) {return div_rm_w (eip);}
make_helper(div_rm2a_l) {return div_rm_l (eip);}

make_helper_v(div_rm2a)
