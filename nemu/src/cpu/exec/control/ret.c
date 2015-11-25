#include "cpu/exec/helper.h"
#include "ret.h"

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper (ret)
{
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	printf ("%x\n", result);
	cpu.esp += DATA_BYTE;
	cpu.eip = result;

	print_asm("ret");

	return 1;
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

