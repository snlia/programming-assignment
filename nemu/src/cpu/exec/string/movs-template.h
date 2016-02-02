make_helper (concat (movs_, SUFFIX))
{
	DATA_TYPE result = swaddr_read(REG (R_ESI), DATA_BYTE);
    swaddr_write (REG (R_EDI), DATA_BYTE, result);
    if (cpu.DF) 
    {
        REG (R_ESI) -= DATA_BYTE;
        REG (R_EDI) -= DATA_BYTE;
    }
    else
    {
        REG (R_ESI) += DATA_BYTE;
        REG (R_EDI) += DATA_BYTE;
    }
	print_asm("movs");

	return 1;
}
