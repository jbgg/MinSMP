


#ifndef __IOAPIC_H__
#define __IOAPIC_H__

#include <types.h>
#include <list.h>


struct ioapic {
    uint8 apic_id;
    uint32 addr;
    uint32 base;
    struct list node;
};



#endif /* __IOAPIC_H__ */
