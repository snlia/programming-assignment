#include "cpu/exec/template-start.h"
#include "monitor/elf.h"

#define instr cmov

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
    char s [8];
    memset (s, 0, sizeof (s));
#endif 
    switch (ops_decoded.opcode)
    {
        case 0x40 : flag = OF;
#ifdef DEBUG
                    strcpy (s, "cmovo");
#endif
                    break;
        case 0x41 : flag = !OF;
#ifdef DEBUG
                    strcpy (s, "cmovno");
#endif
                    break;
        case 0x42 : flag = CF;
#ifdef DEBUG
                    strcpy (s, "cmovb");
#endif
                    break;
        case 0x43 : flag = !CF;
#ifdef DEBUG
                    strcpy (s, "cmovae");
#endif
                    break;
        case 0x44 : flag = ZF;
#ifdef DEBUG
                    strcpy (s, "cmove");
#endif
                    break;
        case 0x45 : flag = !ZF;
#ifdef DEBUG
                    strcpy (s, "cmovne");
#endif
                    break;
        case 0x46 : flag = CF || ZF;
#ifdef DEBUG
                    strcpy (s, "cmovbe");
#endif
                    break;
        case 0x47 : flag = !CF && !ZF;
#ifdef DEBUG
                    strcpy (s, "cmova");
#endif
                    break;
        case 0x48 : flag = SF;
#ifdef DEBUG
                    strcpy (s, "cmovs");
#endif
                    break;
        case 0x49 : flag = !SF;
#ifdef DEBUG
                    strcpy (s, "cmovns");
#endif
                    break;
        case 0x4A : flag = PF;
#ifdef DEBUG
                    strcpy (s, "cmovp");
#endif
                    break;
        case 0x4B : flag = !PF;
#ifdef DEBUG
                    strcpy (s, "cmovnp");
#endif
                    break;
        case 0x4C : flag = SF != OF;
#ifdef DEBUG
                    strcpy (s, "cmovl");
#endif
                    break;
        case 0x4D : flag = SF == OF;
#ifdef DEBUG
                    strcpy (s, "cmovge");
#endif
                    break;
        case 0x4E : flag = ZF || (SF!=OF);
#ifdef DEBUG
                    strcpy (s, "cmovle");
#endif
                    break;
        case 0x4F : flag = !ZF && (SF==OF);
#ifdef DEBUG
                    strcpy (s, "cmovg");
#endif
                    break;
        default : Assert (1, "missing instr");
    }
#ifdef DEBUG
//    print_asm_template5(cpu.eip + (DATA_TYPE_S) op_src->val, s);
#endif 
    if (flag)
    {
        OPERAND_W(op_dest, op_src->val);
        Assert(snprintf(assembly, 80, "%s", s) < 80, "buffer overflow!");
    }
    /* TODO: Update EFLAGS. */
    //	panic("please implement me");

}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

