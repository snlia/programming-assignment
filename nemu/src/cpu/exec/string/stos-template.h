#define DESTREG (reg_l(R_EDI))

make_helper (concat (stos_, SUFFIX))
{
    swaddr_write (DESTREG, DATA_BYTE, REG (R_EAX));
    if (cpu.DF) 
        DESTREG -= DATA_BYTE;
    else
        DESTREG += DATA_BYTE;
	print_asm("stos");
	return 1;
}

#undef DESTindex
#undef SRCindex
