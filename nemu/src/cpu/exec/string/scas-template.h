#define DESTindex (reg_l(R_EDI))

make_helper (concat (scas_, SUFFIX))
{
    DATA_TYPE dest = REG (R_EAX);
    DATA_TYPE src = swaddr_read(DESTindex, DATA_BYTE, SR_ES);
    DATA_TYPE result = dest + (DATA_TYPE_S) (-src);
    //	cpu.CF = (((uint64_t) (DATA_TYPE) dest + (uint64_t) (DATA_TYPE) (-src))  == result);
    cpu.CF = (DATA_TYPE) dest < (DATA_TYPE) src;
    uint32_t off = (DATA_BYTE * 8 - 1);
    cpu.SF = (result >> off) & 1;
    cpu.OF = (((src >> off) & 1) ^ ((dest >> off) & 1)) && (cpu.SF ^ ((dest >> off) & 1));
    cpu.ZF = !result;
    cpu.AF = ((dest & 0x7) + ((-src) & 0x7)) < 0x8;
    result = result & 0xff;
    result = (result ^ (result >> 4)) & 0xf;
    result = (result ^ (result >> 2)) & 0x3;
    cpu.PF = (1 ^ result ^ (result >> 1)) & 1;
    print_asm("scas");
    if (cpu.DF)
        DESTindex -= DATA_BYTE;
    else
        DESTindex += DATA_BYTE;
    return 1;
}

#undef DESTindex
#undef SRCindex
