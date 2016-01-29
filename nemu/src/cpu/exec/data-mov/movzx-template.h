#include "cpu/exec/template-start.h"

#define instr movzx

/*do_movsx_[bwl]*/
static void do_execute() {
    if (op_src->size == 1)
        OPERAND_W(op_dest, (uint8_t)op_src->val);
    else 
        OPERAND_W(op_dest, (uint16_t)op_src->val);
	/*write_operand_[bwd] (op, src)*/
	print_asm_template2();
}

make_instr_helper(rm2rx)
#if DATA_BYTE == 4
make_instr_helper(rm2rX)
#endif

#include "cpu/exec/template-end.h"


