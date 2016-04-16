#include "cpu/exec/helper.h"
#include "iret.h"

#define instr iret
/*
make_helper (iret) {
    //Protected mode return
    //Same privilege level
    //PE=1, VM=0 in flags image, RPL=CPL
    EIP = TemporaryEIP;
    CS = TemporaryCS; //segment descriptor information also loaded
    EFLAGS.CF = TemporaryEFLAGS.CF;
    EFLAGS.PF = TemporaryEFLAGS.PF;
    EFLAGS.AF = TemporaryEFLAGS.ZF;
    EFLAGS.SF = TemporaryEFLAGS.SF;
    EFLAGS.TF = TemporaryEFLAGS.DF;
    EFLAGS.OF = TemporaryEFLAGS.OF;
    EFLAGS.NT = TemporaryEFLAGS.NT;
    if(OperandSize == 32) {
        EFLAGS.RF = TemporaryEFLAGS.RF;
        EFLAGS.AC = TemporaryEFLAGS.AC;
        EFLAGS.ID = TemporaryEFLAGS.ID;
    }
    if(CPL <= IOPL) EFLAGS.IF = TemporaryEFLAGS.IF;
    if(CPL == 0) {
        EFLAGS.IOPL = TemporaryEFLAGS.IOPL;
        if(OperandSize == 32) {
            EFLAGS.VM = TemporaryEFLAGS.VM;
            EFLAGS.VIF = TemporaryEFLAGS.VIF;
            EFLAGS.VIP = TemporaryEFLAGS.VIP;
        }
    }
    //END
}
}
print_asm ("iret");
}
*/
#undef instr

