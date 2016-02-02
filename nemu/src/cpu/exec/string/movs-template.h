make_helper (concat (movs_, SUFFIX))
{
#if DATA_TYPE == 2
#define DESTindex (reg_w(R_DI))
#define SRCindex (reg_w(R_SI))
#else
#define DESTindex (reg_l(R_EDI))
#define SRCindex (reg_l(R_ESI))
#endif
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
#undef DESTindex
#undef SRCindex
	return 1;
}
