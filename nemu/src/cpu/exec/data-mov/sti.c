#include "cpu/exec/helper.h"
#include "sti.h"

make_helper (sti)
{
    cpu.IF = 1;
	print_asm("sti");
	return 1;
}






