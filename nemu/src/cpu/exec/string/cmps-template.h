#define DESTindex (reg_l(R_EDI))
#define SRCindex (reg_l(R_ESI))

make_helper (concat (cmps_, SUFFIX))
{
    DATA_TYPE src = swaddr_read(DESTindex, DATA_BYTE);
    DATA_TYPE dest = swaddr_read(SRCindex, DATA_BYTE);
    DATA_TYPE result = dest + (DATA_TYPE_S) (-src);
    //	cpu.CF = (((uint64_t) (DATA_TYPE) dest + (uint64_t) (DATA_TYPE) (-src))  == result);
    cpu.CF = (DATA_TYPE_S) dest < (DATA_TYPE_S) src;
    uint32_t off = (DATA_BYTE * 8 - 1);
    cpu.SF = (result >> off) & 1;
    cpu.OF = (((src >> off) & 1) ^ ((dest >> off) & 1)) && (cpu.SF ^ ((dest >> off) & 1));
    cpu.ZF = !result;
    cpu.AF = ((dest & 0x7) + ((-src) & 0x7)) < 0x8;
    result = result & 0xff;
    result = (result ^ (result >> 4)) & 0xf;
    result = (result ^ (result >> 2)) & 0x3;
    cpu.PF = (1 ^ result ^ (result >> 1)) & 1;
    if (cpu.DF)
    {
        DESTindex -= DATA_BYTE;
        SRCindex -= DATA_BYTE;
    }
    else
    {
        DESTindex += DATA_BYTE;
        SRCindex += DATA_BYTE;
    }
    print_asm("cmps");
    return 1;
}

#undef DESTindex
#undef SRCindex