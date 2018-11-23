


#include <acpi.h>
#include <mem.h>
#include <video.h>
#include <lapic.h>
#include <ioapic.h>
#include <cpu.h>
#include <mp.h>

struct acpi_rsdp *rsdp;
struct acpi_rsdt *rsdt;
int acpi_rsdt_n;
struct acpi_apic *apic;

static int acpi_get_rsdp();

static int acpi_check_rsdt(struct acpi_rsdt *);
static int acpi_get_rsdt();

static int acpi_apic_setup();
/*extern int ncpu; */
/* extern uint32 lapic; */
/*extern struct cpu cpus[]; */

extern int nioapic;
extern struct list ioapics;


int
acpi_setup(){


    if(acpi_get_rsdp() || rsdp==0)
        return -1;

    if(acpi_get_rsdt() || rsdt==0)
        return -1;

    int i;
    struct acpi_dhdr *descr_header;
    for(i=0;i<acpi_rsdt_n;i++){
        descr_header = (struct acpi_dhdr*) rsdt->entry[i];

        if(memcmp(descr_header->signature, ACPI_APIC_SIG, 
                    sizeof(descr_header->signature)) == 0){
            apic = (struct acpi_apic*) rsdt->entry[i];
            continue;
        }
    }

    if(acpi_apic_setup())
        return -1;



    return 0;
}

void
acpi_print_info(){

    printf("ACPI:\n");
    printf(" rsdp = %x; rsdp->rsdt_addr = %x\n", rsdp, rsdp->rsdt_addr);
    printf(" rsdt = %x; rsdt->length = %x (n = %x)\n", rsdt, rsdt->header.length,
           acpi_rsdt_n);
    int i;
    struct acpi_dhdr *descr_header;
    for(i=0;i<acpi_rsdt_n;i++){
        descr_header = (struct acpi_dhdr*) rsdt->entry[i];
        printf("  %x: %c%c%c%c (%x)\n", i, descr_header->signature[0],
                descr_header->signature[1], descr_header->signature[2],
                descr_header->signature[3], rsdt->entry[i]);
    }

}


static int
acpi_checksum(void *addr, uint32 length){
    int8 *bytes = addr;
    int checksum = 0;
    unsigned int i;
    for(i=0;i<length;i++){
        checksum += bytes[i];
    }
    return checksum & 0xff;
}

static int
acpi_check_rsdp(struct acpi_rsdp *rsdp){


    if(memcmp(rsdp->signature, ACPI_RSDP_SIG, sizeof(rsdp->signature)) != 0)
        return -1;

    uint32 checksum;
    checksum = acpi_checksum(rsdp, sizeof(*rsdp));

    if(checksum != 0)
        return -1;

    return 0;
}


static int
acpi_search_rsdp(void *addr, uint32 length){

    void *end;
    /* check alignment */
    if((uint32)addr & (ACPI_RSDP_ALIGN-1))
        return -1;

    for(end=addr+length;addr<end;addr+=ACPI_RSDP_ALIGN){
        if(acpi_check_rsdp(addr) == 0){
            rsdp = (struct acpi_rsdp*)addr;
            return 0;
        }

    }


    return -1;
}

static int
acpi_get_rsdp(){


    uint32 base = 0x0;

    base = *((uint16*) 0x040e);

    if(base!=0){
        base <<= 4;
        if(acpi_search_rsdp((void*)base,1024) == 0)
            return 0;
    }

    if(acpi_search_rsdp((void*)0x0e0000, 0x100000 - 0x0e0000) == 0)
        return 0;

    return -1;
}


static int
acpi_check_rsdt(struct acpi_rsdt *rsdt){

    return acpi_checksum(rsdt, rsdt->header.length);
}

static int
acpi_get_rsdt(){

    rsdt = (struct acpi_rsdt*)rsdp->rsdt_addr;

    if(memcmp(rsdt->header.signature, ACPI_RSDT_SIG, 
                sizeof(rsdt->header.signature)) != 0)
        return -1;

    if(acpi_check_rsdt(rsdt))
        return -1;

    acpi_rsdt_n = (rsdt->header.length - sizeof(rsdt->header)) 
        / sizeof(rsdt->entry[0]);


    return 0;
}


static int
acpi_apic_setup(){

    if(apic==0)
        return -1;

    if(acpi_checksum(apic, apic->header.length))
        return -1;


    ncpu = 0;
    nioapic = 0;
    lapic = (struct lapic*) (apic->lapic_addr);
    list_init(&ioapics);
    struct acpi_apic_dhdr *apic_entry = apic->entry;
    uint32 end = (uint32) apic + apic->header.length;
    while((uint32)apic_entry < end){
        struct acpi_apic_lapic *lapic_entry;
        struct acpi_apic_ioapic *ioapic_entry;
        switch(apic_entry->type){
            case ACPI_APIC_ENTRY_LAPIC:
                lapic_entry = (struct acpi_apic_lapic*) apic_entry;
                if((lapic_entry->flags & 0x1) && ncpu < NCPU){
                    cpus[ncpu].apic_id = (uint32) lapic_entry->apic_id;
                    ncpu++;
                }
                break;

            case ACPI_APIC_ENTRY_IOAPIC:
                ioapic_entry = (struct acpi_apic_ioapic*) apic_entry;

                struct ioapic *ioapic_last;

                ioapic_last = malloc(sizeof(struct ioapic));
                list_init_node(&ioapic_last->node);
                ioapic_last->apic_id = ioapic_entry->apic_id;
                ioapic_last->addr = ioapic_entry->addr;
                ioapic_last->base = ioapic_entry->base;


                list_insert_tail(&ioapics, &ioapic_last->node);

                nioapic++;
                break;
        }

        apic_entry =(struct acpi_apic_dhdr*)((uint32) apic_entry 
                + apic_entry->length);
    }


    if(ncpu == 0 || nioapic == 0)
        return -1;


    return 0;
}
