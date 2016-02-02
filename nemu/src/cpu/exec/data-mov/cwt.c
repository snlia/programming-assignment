#include "cpu/exec/helper.h"
#include "cwt.h"

make_helper (cwt_w)
{
    reg_w (R_AX) = (int8_t) reg_b (R_AL);

	print_asm("cwt");
	return 1;
}

make_helper (cwt_l)
{
    reg_l (R_EAX) = (int16_t) reg_w (R_AX);

	print_asm("cwt");
	return 1;
}

make_helper_v (cwt);



