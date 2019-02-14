

#ifndef __CPU_H__

#include <types.h>


/* assume Pentium 4, Xeon, or later processors
 * where APIC ID is 8 bits wide
 */
#define NCPU 255


struct cpu {
    uint32 apic_id;
    uint32 flags;
    void *stack_base;
};

#define CPU_ENABLE (1 << 0)

extern int ncpu;
extern struct cpu cpus[NCPU];

int cpu_setup();

void cpu_ap_main();








#endif /* __CPU_H__ */

