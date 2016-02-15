#include "cpu/exec/template-start.h"
#include "monitor/elf.h"

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
	uint32_t tx = ops_decoded.opcode - (DATA_BYTE > 1 ? 0x10 : 0);
#ifdef DEBUG
	char s [5];
	memset (s, 0, sizeof (s));
#endif 
	switch (tx & 0xff)
	{
		case 0x70 : flag =  OF; 
#ifdef DEBUG
					strcpy (s, "jo");
#endif
					break; 
		case 0x71 : flag = !OF;
#ifdef DEBUG
					strcpy (s, "jno");
#endif
					break; 
		case 0x72 : flag = CF;
#ifdef DEBUG
					strcpy (s, "jnae");
#endif
					break; 
		case 0x73 : flag = !CF;
#ifdef DEBUG
					strcpy (s, "jnb");
#endif
					break; 
		case 0x74 : flag = ZF;
#ifdef DEBUG
					strcpy (s, "je");
#endif
					break; 
		case 0x75 : flag = !ZF;
#ifdef DEBUG
					strcpy (s, "jne");
#endif
					break; 
		case 0x76 : flag = CF || ZF;
#ifdef DEBUG
					strcpy (s, "jna");
#endif
					break; 
		case 0x77 : flag = !CF && !ZF;
#ifdef DEBUG
					strcpy (s, "jnbe");
#endif
					break; 
		case 0x78 : flag =  SF;
#ifdef DEBUG
					strcpy (s, "js");
#endif
					break; 
		case 0x79 : flag = !SF;
#ifdef DEBUG
					strcpy (s, "jns");
#endif
					break; 
		case 0x7a : flag =  PF;
#ifdef DEBUG
					strcpy (s, "jpe");
#endif
					break; 
		case 0x7b : flag = !PF;
#ifdef DEBUG
					strcpy (s, "jnp");
#endif
					break; 
		case 0x7c : flag = SF != OF;
#ifdef DEBUG
					strcpy (s, "jnge");
#endif
					break; 
		case 0x7d : flag = SF == OF;
#ifdef DEBUG
					strcpy (s, "jge");
#endif
					break; 
		case 0x7e : flag = ZF || (SF != OF);
#ifdef DEBUG
					strcpy (s, "jng");
#endif
					break; 
		case 0x7f : flag = !ZF && (SF == OF);
#ifdef DEBUG
					strcpy (s, "jg");
#endif
					break; 
		case 0xe3 : 
#if DATA_BYTE == 2
				flag = !reg_w(R_CX);
#else
				flag = reg_l(R_ECX);
#endif
#ifdef DEBUG
					strcpy (s, "jcxz");
#endif
					break;
		default : Assert (1, "missing instr");
	}
#ifdef DEBUG
	print_asm_template5(cpu.eip + (DATA_TYPE_S) op_src->val, s);
#endif 
	if (flag)
	{
		cpu.eip += (DATA_TYPE_S) op_src->val;
#if DATE_BYTE == 2
		cpu.eip &= 0x0000ffff;
#endif
	}
	/* TODO: Update EFLAGS. */
	//	panic("please implement me");

}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
