#include "common.h"
#include <stdio.h>
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0
#define PHSIZE 0x20
#ifndef ENSIZE
#define EHSIZE 0x34
#endif

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[EHSIZE];
	uint8_t buf2[PHSIZE];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, EHSIZE);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, EHSIZE);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);
    //e_phoff e_phnum
    Elf32_Off Off = elf->e_phoff;
    uint16_t phnum = elf->e_phnum;
    /* Load each program segment */
    while (phnum--){
        /* Scan the program header table, load each segment into memory */
#ifdef HAS_DEVICE
        ide_read(buf2, Off + ELF_OFFSET_IN_DISK, PHSIZE);
#else
        ramdisk_read(buf2, Off + ELF_OFFSET_IN_DISK, PHSIZE);
#endif
        ph = (void *) buf2;
        Off += PHSIZE;
        if(ph->p_type == PT_LOAD) {
            /* TODO: read the content of the segment from the ELF file 
             * to the memory region [VirtAddr, VirtAddr + FileSiz)
             */
            uint32_t va_off = 0;
#ifdef IA32_PAGE
            va_off = mm_malloc (ph->p_vaddr, ph->p_memsz) - ph->p_vaddr;
#endif
#ifdef HAS_DEVICE
            ide_read((void *) (ph->p_vaddr +  va_off), ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
#else
            ramdisk_read((void *) (ph->p_vaddr + va_off), ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
#endif
            /* TODO: zero the memory region 
             * [VirtAddr + FileSiz, VirtAddr + MemSiz)
             */
            memset ((void *) (ph->p_vaddr + ph->p_filesz + va_off), 0, ph->p_memsz - ph->p_filesz);

#ifdef IA32_PAGE
            /* Record the program break for future use. */
            extern uint32_t brk;
            uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
            if(brk < new_brk) { brk = new_brk; }
#endif
        }
    }

    volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
    mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
    create_video_mapping();
#endif
    write_cr3(get_ucr3());
#endif

    return entry;
}
