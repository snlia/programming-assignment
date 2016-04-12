#define DESTindex (reg_l(R_EDI))
#define SRCindex (reg_l(R_ESI))

make_helper (concat (movs_, SUFFIX))
{
    swaddr_write (DESTindex, DATA_BYTE, swaddr_read(SRCindex, DATA_BYTE, SR_DS), SR_ES);
    if (cpu.DF) 
    {
        SRCindex -= DATA_BYTE;
        DESTindex -= DATA_BYTE;
    }
    else
    {
        SRCindex += DATA_BYTE;
        DESTindex += DATA_BYTE;
    }
	print_asm("movs");
	return 1;
}

#undef DESTindex
#undef SRCindex
