

#ifndef __LAPIC_H__
#define __LAPIC_H__

#include <types.h>

#define LAPIC_RESERVED3(off) uint32 res ## off ## 4; \
    uint32 res ## off ## 8; \
    uint32 res ## off ## c;

#define LAPIC_RESERVED4(off) uint32 res ## off ## 0; \
    uint32 res ## off ## 4; \
    uint32 res ## off ## 8; \
    uint32 res ## off ## c;


struct lapic {

    /* 0x00 */
    LAPIC_RESERVED4(0)
    /* 0x10 */
    LAPIC_RESERVED4(1)
    /* 0x20 */
    uint32 apic_id;
    LAPIC_RESERVED3(2)
    /* 0x30 */
    LAPIC_RESERVED4(3)
    /* 0x40 */
    LAPIC_RESERVED4(4)
    /* 0x50 */
    LAPIC_RESERVED4(5)
    /* 0x60 */
    LAPIC_RESERVED4(6)
    /* 0x70 */
    LAPIC_RESERVED4(7)
    /* 0x80 */
    LAPIC_RESERVED4(8)
    /* 0x90 */
    LAPIC_RESERVED4(9)
    /* 0xa0 */
    LAPIC_RESERVED4(a)
    /* 0xb0 */
    LAPIC_RESERVED4(b)
    /* 0xc0 */
    LAPIC_RESERVED4(c)
    /* 0xd0 */
    LAPIC_RESERVED4(d)
    /* 0xe0 */
    LAPIC_RESERVED4(e)
    /* 0xf0 */
    LAPIC_RESERVED4(f)

    /* 0x100 */
    LAPIC_RESERVED4(10)
    /* 0x110 */
    LAPIC_RESERVED4(11)
    /* 0x120 */
    LAPIC_RESERVED4(12)
    /* 0x130 */
    LAPIC_RESERVED4(13)
    /* 0x140 */
    LAPIC_RESERVED4(14)
    /* 0x150 */
    LAPIC_RESERVED4(15)
    /* 0x160 */
    LAPIC_RESERVED4(16)
    /* 0x170 */
    LAPIC_RESERVED4(17)
    /* 0x180 */
    LAPIC_RESERVED4(18)
    /* 0x190 */
    LAPIC_RESERVED4(19)
    /* 0x1a0 */
    LAPIC_RESERVED4(1a)
    /* 0x1b0 */
    LAPIC_RESERVED4(1b)
    /* 0x1c0 */
    LAPIC_RESERVED4(1c)
    /* 0x1d0 */
    LAPIC_RESERVED4(1d)
    /* 0x1e0 */
    LAPIC_RESERVED4(1e)
    /* 0x1f0 */
    LAPIC_RESERVED4(1f)

    /* 0x200 */
    LAPIC_RESERVED4(20)
    /* 0x210 */
    LAPIC_RESERVED4(21)
    /* 0x220 */
    LAPIC_RESERVED4(22)
    /* 0x230 */
    LAPIC_RESERVED4(23)
    /* 0x240 */
    LAPIC_RESERVED4(24)
    /* 0x250 */
    LAPIC_RESERVED4(25)
    /* 0x260 */
    LAPIC_RESERVED4(26)
    /* 0x270 */
    LAPIC_RESERVED4(27)
    /* 0x280 */
    LAPIC_RESERVED4(28)
    /* 0x290 */
    LAPIC_RESERVED4(29)
    /* 0x2a0 */
    LAPIC_RESERVED4(2a)
    /* 0x2b0 */
    LAPIC_RESERVED4(2b)
    /* 0x2c0 */
    LAPIC_RESERVED4(2c)
    /* 0x2d0 */
    LAPIC_RESERVED4(2d)
    /* 0x2e0 */
    LAPIC_RESERVED4(2e)
    /* 0x2f0 */
    LAPIC_RESERVED4(2f)
    /* 0x300 */
    uint32 icr_low;
    LAPIC_RESERVED3(30)
    /* 0x310 */
    uint32 icr_high;
    LAPIC_RESERVED3(31)

};


extern struct lapic *lapic;



static inline uint32
get_lapic_id(){

    return lapic->apic_id;

}


#endif /* __LAPIC_H__ */
