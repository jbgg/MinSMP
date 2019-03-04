

#ifndef __ASM_H__
#define __ASM_H__


#define outb(val, port)                         \
    __asm__ __volatile ("outb %%al, %%dx"       \
            :                                   \
            : "a"(val), "d"(port)               \
            )

#define inb(port, val)                          \
    __asm__ __volatile ("inb %%dx, %%al"        \
            : "=a"(val)                         \
            : "d"(port)                         \
            )


#endif /* __ASM_H__ */
