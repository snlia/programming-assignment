#define DESTindex (reg_l(R_EDI))
#define SRCindex (reg_l(R_ESI))

make_helper (concat (movs_, SUFFIX))
{
	DATA_TYPE result = swaddr_read(SRCindex, DATA_BYTE);
    swaddr_write (DESTindex, DATA_BYTE, result);
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
