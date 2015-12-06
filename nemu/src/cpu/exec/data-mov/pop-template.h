#include "cpu/exec/template-start.h"
#define instr pop

static void do_execute () {
#if DATA_BYTE == 2
	OPERAND_W(op_dest, swaddr_read (cpu.esp, 2));
	cpu.esp += 2;
#else
	OPERAND_W(op_dest, swaddr_read (cpu.esp, 4));
	cpu.esp += 4;
#endif
	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}


#include "cpu/exec/template-end.h"
