#include "cpu/exec/template-start.h"

#define instr nop

static void do_execute () {
	print_asm ("nop");
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"


