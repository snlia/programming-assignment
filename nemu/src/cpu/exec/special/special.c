#include "cpu/exec/helper.h"
#include "monitor/monitor.h"

make_helper(inv) {
	/* invalid opcode */

	uint32_t temp[8];
	temp[0] = instr_fetch(eip, 4);
	temp[1] = instr_fetch(eip + 4, 4);

	uint8_t *p = (void *)temp;
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n", 
			eip, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

	extern char logo [];
	printf("There are two cases which will trigger this unexpected exception:\n\
1. The instruction at eip = 0x%08x is not implemented.\n\
2. Something is implemented incorrectly.\n", eip);
	printf("Find this eip value(0x%08x) in the disassembling result to distinguish which case it is.\n\n", eip);
	printf("\33[1;31mIf it is the first case, see\n%s\nfor more details.\n\nIf it is the second case, remember:\n\
* The machine is always right!\n\
* Every line of untested code is always wrong!\33[0m\n\n", logo);
    nemu_state = STOP;
    return 0;
}

make_helper(nemu_trap) {
	print_asm("nemu trap (eax = %d)", cpu.eax);

	switch(cpu.eax) {
		case 2:
            printf ("(stdout)");
            int len = cpu.edx;
            for (int i = 0; i < len; ++i)
                putchar (swaddr_read (cpu.ecx + i, 1, SR_CS));
		   	break;

		default:
			printf("\33[1;31mnemu: HIT %s TRAP\33[0m at eip = 0x%08x\n\n",
					(cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
			nemu_state = END;
	}

	return 1;
}

make_helper (nop_mul) {
    printf ("%x\n", cpu.eip);
    uint8_t rm = instr_fetch (cpu.eip + 1, 1);
    switch (rm) {
        case 0: return 2;
        case 0x40 : return 3;
        case 0x44 : return 4;
        case 0x80 : return 5;
        case 0x84 : return 6;
        default : Assert (0, "Missing instr");
    }
    return 0;
}
