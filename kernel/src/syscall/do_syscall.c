#include "irq.h"

#include <sys/syscall.h>

int fs_open(const char *, int );   /* 在我们的实现中可以忽略flags */
int fs_read(int , void *, int );
int fs_write(int , void *, int );
int fs_lseek(int , int , int );
int fs_close(int );

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
        //    asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
        uint8_t *addr = (uint8_t *)tf->ecx;
        for (int i = 0; i < tf->edx; ++i)
            serial_printc(*(addr + i));
        tf->eax = tf->edx;
    }
    else 
        tf->eax = fs_write (tf->ebx, (void *) tf->ecx, tf->edx);
}

static void sys_read (TrapFrame *tf) {
    if (!tf->ebx) {
        panic ("try to read from stdin");
    }
    else 
        tf->eax = fs_read (tf->ebx, (void *) tf->ecx, tf->edx);
}

static void sys_open (TrapFrame *tf) {
    tf->eax = fs_open ((char*)tf->ebx, tf->ecx);
}

static void sys_close (TrapFrame *tf) {
    tf->eax = fs_close (tf->ebx);
}

static void sys_lseek (TrapFrame *tf) {
    tf->eax = fs_lseek (tf->ebx, tf->ecx, tf->edx);
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

        case SYS_open: sys_open(tf); break;

        case SYS_lseek: sys_lseek(tf); break;

        case SYS_close: sys_close(tf); break;

                        /* TODO: Add more system calls. */

        default: panic("Unhandled system call: id = %d", tf->eax);
    }
}

