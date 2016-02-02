#include "common.h"
#include <stdlib.h>
#include <elf.h>
#define FUNC 0x12
#define VARIABLE 0x11

char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[4096];
	/* Read the first 4096 bytes from the exec_file.
	 * They should contain the ELF header and program headers. */
	ret = fread(buf, 4096, 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}
	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

void print_asm_template4(swaddr_t eip, char * S)
{
	int i;
	extern char assembly[];
    printf ("%s\n", assembly);
	for (i = 0; i < nr_symtab_entry; ++i)
		if (symtab[i].st_name && symtab[i].st_info == FUNC && symtab[i].st_value == eip)
			Assert(snprintf(assembly, 80, "%s %x <%s>", S, eip, strtab + symtab[i].st_name) < 80, "buffer overflow!");
}

void print_asm_template5(swaddr_t eip, char * S)
{
	int i, ansp = -1;
	extern char assembly[];
	for (i = nr_symtab_entry - 1; ~i; --i)
		if (symtab[i].st_name && symtab[i].st_info == FUNC && symtab[i].st_value <= eip)
			if (!~ansp || symtab[ansp].st_value < symtab[i].st_value) ansp = i;
	Assert(snprintf(assembly, 80, "%s %x <%s+0x%x>", S, eip, strtab + symtab[ansp].st_name, eip - symtab[ansp].st_value) < 80, "buffer overflow!");
}

uint32_t get_value (char *s, char * Flag)
{
	int i;
	*Flag = 0;
	for (i = 0; i < nr_symtab_entry; ++i)
		if (symtab[i].st_name && !strcmp (s, strtab + symtab[i].st_name) && (symtab[i].st_info == FUNC || symtab[i].st_info == VARIABLE))
		{
			*Flag = 1;
			return symtab[i].st_value;
		}
	return 0;
}

char* find_FUNC (swaddr_t eip)
{
	int i, ansp = -1;
	for (i = 0; i < nr_symtab_entry; ++i)
		if (symtab[i].st_name && symtab[i].st_info == FUNC && symtab[i].st_value <= eip)
			if (!~ansp || symtab[ansp].st_value < symtab[i].st_value) ansp = i;
	return strtab + symtab[ansp].st_name;
}

void print_elf ()
{
	int i;
	for (i = 0; i < nr_symtab_entry; ++i)
		if (symtab[i].st_name)
			printf ("%s %x %x\n", symtab[i].st_name + strtab, symtab[i].st_value, symtab[i].st_info);
}
#undef FUNC 
#undef VARIABLE 
