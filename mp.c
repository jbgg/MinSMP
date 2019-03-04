

#include <mp.h>
#include <cpu.h>
#include <lapic.h>
#include <ioapic.h>
#include <pit.h>
#include <video.h>
#include <list.h>
#include <mem.h>


#define AP_BOOT_ADDR (0x7000)


extern uint32 nioapic;
extern struct list ioapics;


extern void* *stack_ptr;

extern void *stack_bsp;

extern void* *apboot, *apbootend;

extern void dummyf(uint32);


/* mp_setup is called by BP processor */
int
mp_setup(){

    int i=0;

    /* setup BSP processor */
    cpus[i].stack_base = &stack_bsp;

    if(cpu_setup())
        return -1;

    /* setup bootstrap code for ap processors */
    memcpy((void*)AP_BOOT_ADDR, (void*)&apboot,
            (uint32)&apbootend - (uint32)&apboot);

    i++;

    for(/*i=1*/;i<ncpu;i++){

        /* allocating stack */
#define STACK_SIZE (4096 * 2)
        *stack_ptr = malloc(STACK_SIZE);
        cpus[i].stack_base = *stack_ptr;
        uint32 apic_id = cpus[i].apic_id;

        /* ipi init assert */
        lapic->icr_high = (apic_id << 24);
        lapic->icr_low = (1 << 15) | (1 << 14) | (5 << 8);
        dummyf(lapic->apic_id);
        pit_delay(1);

        /* ipi init deassert */
        lapic->icr_low = (1 << 15) | (5 << 8);
        dummyf(lapic->apic_id);
        pit_delay(10);

        /* ipi startup */
        lapic->icr_high = (apic_id << 24);
        lapic->icr_low = (6 << 8) | ((AP_BOOT_ADDR >> 12) & 0xff);
        dummyf(lapic->apic_id);
        pit_delay(1);
        
        /* ipi startup */
        lapic->icr_high = (apic_id << 24);
        lapic->icr_low = (6 << 8) | ((AP_BOOT_ADDR >> 12) & 0xff);
        dummyf(lapic->apic_id);
        pit_delay(1);

        volatile uint32 *flags_p = &cpus[i].flags;
        while((*flags_p & CPU_ENABLE) == 0)
            ;

    }

    return 0;
}


void
mp_print_info(){

    printf("LAPIC\n");
    printf(" lapic_addr = %x\n", lapic);

    printf("CPU:\n");
    int i;
    for(i=0;i<ncpu;i++){
        printf(" cpu %x: apic_id = %x", i, cpus[i].apic_id);
        if(cpus[i].flags & CPU_ENABLE)
            printf(" ENABLE");
        printf("\n");
    }

    printf("IOAPIC:\n");
    struct list *node;
    list_foreach(&ioapics, node){
        struct ioapic *ioapic;
        ioapic = list_get_entry(node, struct ioapic);

        printf(" ioapic: apic_id= %x addr = %x, base = %x\n",
                ioapic->apic_id, ioapic->addr,
                ioapic->base);
    }

}



