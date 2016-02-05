#include "cpu/exec/template-start.h"

#define instr shrd

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in = op_dest->val;
	DATA_TYPE out = op_src2->val;
	uint8_t count = op_src->val;
	count &= 0x1f;
    printf ("in :%x out : %x count :%x\n", in, out, count);
    cpu.CF = (out >> (count - 1)) & 1;
	while(count != 0) {
		out >>= 1;
		out |= (in & 1) << ((DATA_BYTE << 3) - 1);
		in >>= 1;
		count --;
        printf ("!!!%x\n", out);
	}

	OPERAND_W(op_src2, out);
    printf ("out : %x\n", out);
    cpu.SF = (out >> (DATA_BYTE * 8 - 1)) & 1;
	cpu.ZF = !out;
	out = out & 0xff;
	out = (out ^ (out >> 4)) & 0xf;
	out = (out ^ (out >> 2)) & 0x3;
	cpu.PF = (1 ^ out ^ (out >> 1)) & 1;
	print_asm("shrd" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

make_helper(concat(shrdi_, SUFFIX)) {
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}

make_helper(concat(shrdc_, SUFFIX)) {
	int len = concat(decode_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
