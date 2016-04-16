#include "cpu/exec/helper.h"
#include "cli.h"

make_helper (cli)
{
    cpu.IF = 0;
	print_asm("cli");
	return 1;
}





