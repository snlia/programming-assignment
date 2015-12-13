#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
	uint32_t result = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.eip = result;

	cpu.esp += op_src->val;

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
