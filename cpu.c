
#include <cpu.h>
#include <mp.h>
#include <lapic.h>
#include <ioapic.h>
#include <video.h>
#include <list.h>

int ncpu;
struct cpu cpus[NCPU];



int
cpu_setup(){

    int i = 0;
    while(i<ncpu && (cpus[i].flags & CPU_ENABLE))
        i++;

    /* panic? */
    if(i>=ncpu)
        return -1;

    cpus[i].apic_id = (get_lapic_id() >> 24) & 0xf;
    cpus[i].flags |= CPU_ENABLE;

    return 0;
}




void
cpu_ap_main(){


    if(cpu_setup())
        goto idle;

idle:
    for(;;)
        asm volatile("hlt");

}





