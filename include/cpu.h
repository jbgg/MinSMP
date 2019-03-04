

#ifndef __CPU_H__
#define __CPU_H__

#include <types.h>


#define NCPU 128


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

