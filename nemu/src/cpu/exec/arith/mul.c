#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mul-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mul-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mul-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mul_rm)

make_helper(mul_rm2a_b) {return mul_rm_b (eip);}
make_helper(mul_rm2a_w) {return mul_rm_w (eip);}
make_helper(mul_rm2a_l) {return mul_rm_l (eip);}

make_helper_v(mul_rm2a)
