#ifndef __ELF_H__
#define __ELF_H__

#include "common.h"

void print_asm_template4(swaddr_t , char *);
void print_asm_template5(swaddr_t , char *);
uint32_t get_value (char *, bool *);
char* find_FUNC (swaddr_t );
void print_elf ();

#endif
