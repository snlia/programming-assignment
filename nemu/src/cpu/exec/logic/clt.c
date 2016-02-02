#include "cpu/exec/helper.h"
#include "clt.h"

make_helper (clt_w)
{
    if ((int16_t) reg_w (R_AX) < 0) reg_w (R_DX) = 0XFFFF;
    else reg_w (R_DX) = 0;
	print_asm("clt");
	return 1;
}

make_helper (clt_l)
{
    if ((int32_t) reg_l (R_EAX) < 0) reg_l (R_EDX) = 0XFFFFFFFF;
    else reg_l (R_DX) = 0;
	print_asm("clt");
	return 1;
}

make_helper_v (clt);



