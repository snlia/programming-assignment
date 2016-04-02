#include "cpu/exec/template-start.h"

#define instr mov

/*do_mov_[bwl]*/
static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	/*write_operand_[bwd] (op, src)*/
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE == 4
make_helper (mov_rm2cr_l) {
    size_t len = decode_r2rm_l (eip + 1);
    OPERAND_W (op_dest, cpu.CR[op_src->reg]);
#ifdef DEBUG
    sprintf(op_src->str, "%%cr%d", op_src->reg);
#endif
    print_asm_template2 ();
    return len + 1;
}

make_helper (mov_cr2rm_l) {
    size_t len = decode_rm2r_l (eip + 1);
    cpu.CR[op_dest->reg] = op_src->val;
#ifdef DEBUG
    sprintf(op_dest->str, "%%cr%d", op_dest->reg);
#endif
    print_asm_template2 ();
    return len + 1;
}
#endif

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
    current_sreg = (addr < 0x7000000) + 2;
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
    current_sreg = (addr < 0x7000000) + 2;
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
