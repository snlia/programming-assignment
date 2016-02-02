#include "cpu/exec/helper.h"
#include "movs.h"

/* for instruction encoding overloading */

#define DATA_BYTE 2
#include "cpu/exec/template-start.h"
#include "movs-template.h"
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper (movs_b)
{
	DATA_TYPE result = swaddr_read(REG (R_ESI), 1);
    swaddr_write (REG (R_EDI), 1, result);
    if (cpu.DF) 
    {
        REG (R_ESI) -= 1;
        REG (R_EDI) -= 1;
    }
    else
    {
        REG (R_ESI) += 1;
        REG (R_EDI) += 1;
    }
	print_asm("movs");

	return 1;
}
#include "movs-template.h"
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

make_helper_v (movs);


