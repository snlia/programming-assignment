#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
extern void serial_printc (char);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

uint32_t edi, esi, ebp, old_esp, ebx, edx, ecx, eax; //push to stack by pusha

static void sys_write (TrapFrame *tf) {
    if (tf->ebx == 1 || tf->ebx == 2) {
        uint8_t *addr = (uint8_t *)tf->ecx;
        for (int i = 0; i < tf->edx; ++i)
            serial_printc(*(addr + i));
        tf->eax = tf->edx;
    }
    else {
        Log ("%d %d %d %d\n", tf->ebx, tf->ecx, tf->edx);
        assert (0);
    }
    //    asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
}

static void sys_read (TrapFrame *tf) {
    set_bp ();
    if (!tf->ebx) {
        panic ("try to read from stdin");
    }
    else {
        Log ("%d %d %d %d\n", tf->ebx, tf->ecx, tf->edx);
        assert (0);
    }
    //    asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
}

void do_syscall(TrapFrame *tf) {
    switch(tf->eax) {
        /* The ``add_irq_handle'' system call is artificial. We use it to 
         * let user program register its interrupt handlers. But this is 
         * very dangerous in a real operating system. Therefore such a 
         * system call never exists in GNU/Linux.
         */
        case 0: 
            cli();
            add_irq_handle(tf->ebx, (void*)tf->ecx);
            sti();
            break;

        case SYS_brk: sys_brk(tf); break;

        case SYS_write: sys_write(tf); break;

        case SYS_read: sys_read(tf); break;
/*
        case SYS_lseek: sys_lseek(tf); break;

        case SYS_close: sys_close(tf); break;
        */

                        /* TODO: Add more system calls. */

        default: panic("Unhandled system call: id = %d", tf->eax);
    }
}

