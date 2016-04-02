#include "cpu/exec/helper.h"
#include "leave.h"

/* for instruction encoding overloading */
#define DATA_BYTE 2
#include "cpu/exec/template-start.h"
make_helper (leave_w)
{
	cpu.esp = cpu.ebp;
    current_sreg = 2;
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	cpu.esp += DATA_BYTE;
	REG(R_BP) = result;
    
	print_asm("leave");

	return 1;
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper (leave_l)
{
	cpu.esp = cpu.ebp;
    current_sreg = 2;
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	cpu.esp += DATA_BYTE;
	cpu.ebp = result;

	print_asm("leave");

	return 1;
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

make_helper_v (leave);


