#include "cpu/exec/template-start.h"

#define instr setcc

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
#ifdef DEBUG
	char s [5];
	memset (s, 0, sizeof (s));
#endif 
	switch (ops_decoded.opcode & 0xff)
    {
        case 0x97 : flag = (CF==0 && ZF==0);
#ifdef DEBUG
                    strcpy (s, "seta");
#endif
                    break;
        case 0x93 : flag = (CF==0);
#ifdef DEBUG
                    strcpy (s, "setae");
#endif
                    break;
        case 0x92 : flag = (CF==1);
#ifdef DEBUG
                    strcpy (s, "setb");
#endif
                    break;
        case 0x96 : flag = (ZF==1);
#ifdef DEBUG
                    strcpy (s, "setbe");
#endif
                    break;
        case 0x94 : flag = (ZF==1);
#ifdef DEBUG
                    strcpy (s, "sete");
#endif
                    break;
        case 0x9F : flag = (ZF==0 && SF==OF);
#ifdef DEBUG
                    strcpy (s, "setg");
#endif
                    break;
        case 0x9D : flag = (SF==OF);
#ifdef DEBUG
                    strcpy (s, "setge");
#endif
                    break;
        case 0x9C : flag = (SF!=OF);
#ifdef DEBUG
                    strcpy (s, "setl");
#endif
                    break;
        case 0x9E : flag = (ZF==1 || SF!=OF);
#ifdef DEBUG
                    strcpy (s, "setle");
#endif
                    break;
        case 0x95 : flag = (ZF==0);
#ifdef DEBUG
                    strcpy (s, "setne");
#endif
                    break;
        case 0x91 : flag = (OF==0);
#ifdef DEBUG
                    strcpy (s, "setno");
#endif
                    break;
        case 0x9B : flag = (PF==0);
#ifdef DEBUG
                    strcpy (s, "setnp");
#endif
                    break;
        case 0x99 : flag = (SF==0);
#ifdef DEBUG
                    strcpy (s, "setns");
#endif
                    break;
        case 0x90 : flag = (OF==1);
#ifdef DEBUG
                    strcpy (s, "seto");
#endif
                    break;
        case 0x9A : flag = (PF==1);
#ifdef DEBUG
                    strcpy (s, "setp");
#endif
                    break;
        case 0x98 : flag = (SF==1);
#ifdef DEBUG
                    strcpy (s, "sets");
#endif
                    break;
        default : assert ("missing instr");
    }
#ifdef DEBUG
	Assert(snprintf(assembly, 80, "%s", s) < 80, "buffer overflow!");
#endif 
    if (flag) OPERAND_W(op_src, 1);
    else OPERAND_W(op_src, 0);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

