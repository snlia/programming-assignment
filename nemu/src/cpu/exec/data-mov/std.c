#include "cpu/exec/helper.h"
#include "std.h"

make_helper (std)
{
    cpu.DF = 1;
	print_asm("std");
	return 1;
}





