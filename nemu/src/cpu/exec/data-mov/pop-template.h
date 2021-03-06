#include "cpu/exec/template-start.h"
#define instr pop

static void do_execute () {
#if DATA_BYTE == 2
	OPERAND_W(op_src, swaddr_read (cpu.esp, 2, SR_SS));
	cpu.esp += 2;
#else
	OPERAND_W(op_src, swaddr_read (cpu.esp, 4, SR_SS));
	cpu.esp += 4;
#endif
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
