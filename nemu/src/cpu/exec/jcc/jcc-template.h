#include "cpu/exec/template-start.h"

#define instr jcc

#define CF cpu.CF
#define VM cpu.VM
#define RF cpu.RF
#define NT cpu.NT
#define IOPL cpu.IOPL
#define OF cpu.OF
#define DF cpu.DF
#define IF cpu.IF
#define TF cpu.TF
#define SF cpu.SF
#define ZF cpu.ZF
#define AF cpu.AF
#define PF cpu.PF


static void do_execute () {
	bool flag = 0;
	uint32_t tx = ops_decoded.opcode - (DATA_BYTE > 1 ? 110 : 0);
	printf ("%d %d\n", ops_decoded.opcode, ZF);
	switch (tx)
	{
		case 0x70 : flag =  OF; break; 
		case 0x71 : flag = !OF; break; 
		case 0x72 : flag = CF; break; 
		case 0x73 : flag = !CF; break; 
		case 0x74 : flag = ZF; break; 
		case 0x75 : flag = !ZF; break; 
		case 0x76 : flag = CF || ZF; break; 
		case 0x77 : flag = !CF && !ZF; break; 
		case 0x78 : flag =  SF; break; 
		case 0x79 : flag = !SF; break; 
		case 0x7a : flag =  PF; break; 
		case 0x7b : flag = !PF; break; 
		case 0x7c : flag = SF != OF; break; 
		case 0x7d : flag = SF == OF; break; 
		case 0x7e : flag = ZF || (SF != OF); break; 
		case 0x7f : flag = !ZF && (SF == OF); break; 
		case 0xe3 : flag = ops_decoded.is_data_size_16 ? !reg_w(R_CX) : !reg_l(R_ECX); break; 
		default : assert ("missing instr");
	}
	if (flag)
	{
		printf ("%x %x\n", cpu.eip, op_src->val);
		cpu.eip += (DATA_TYPE_S) op_src->val;
	}
	/* TODO: Update EFLAGS. */
	//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
