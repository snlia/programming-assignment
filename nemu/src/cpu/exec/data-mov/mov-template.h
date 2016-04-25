#include "cpu/exec/template-start.h"

#define instr mov

extern void TLB_flush ();

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
    uint32_t src;
    if (op_dest->reg == 0) src = cpu.CR0.val;
    else if (op_dest->reg == 3)
        src = cpu.CR3.val;
    else assert (0);
    OPERAND_W (op_dest, src);
#ifdef DEBUG
    sprintf(op_src->str, "%%cr%d", op_src->reg);
#endif
    print_asm_template2 ();
    return len + 1;
}

make_helper (mov_cr2rm_l) {
    size_t len = decode_rm2r_l (eip + 1);
    if (op_dest->reg == 0)
        cpu.CR0.val = op_src->val;
    else if (op_dest->reg == 3) {
        cpu.CR3.val = op_src->val;
        TLB_flush ();
    }
    else assert (0);
#ifdef DEBUG
    sprintf(op_dest->str, "%%cr%d", op_dest->reg);
#endif
    print_asm_template2 ();
    return len + 1;
}
#endif

#if DATA_BYTE == 2
make_helper (mov_rm2rs) {
   size_t len = decode_rm2r_w (eip + 1);
   cpu.spr[op_dest->reg].val = (DATA_TYPE) op_src->val;
   cache_SEG[op_dest->reg].val = lnaddr_read ((DATA_TYPE) (op_src->val & 0xfff8) + cpu.GDTR_B, 4) | ((uint64_t) lnaddr_read ((DATA_TYPE) (op_src->val & 0xfff8) + cpu.GDTR_B + 4, 4) << 32);
   return len + 1;
}
#endif

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), SR_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, SR_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
