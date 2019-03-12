

#include <video.h>
#include <asm.h>
#include <types.h>


int
pit_setup(){

    /* setting counter 1, 1ms = 0x4a9 ticks */
    outb((1 << 6) | (3 << 4) | (2 << 1), 0x43);
    outb(0xa9, 0x41);
    outb(0x04, 0x41);

    return 0;
}


void
pit_delay(uint32 ms){

    uint16 prev, diff;
    uint8 low, high;

    outb((1 << 6), 0x43);
    inb(0x41, low);
    inb(0x41, high);
    prev = ((high & 0xff) << 8) | (low & 0xff);

    while(ms>0){
        do{
            outb((1 << 6), 0x43);
            inb(0x41, low);
            inb(0x41, high);
            diff = prev - (((high & 0xff) << 8) | (low & 0xff));
            if(diff <= 0)
                diff += 0x4a9;
        }while(diff <= 0x254);

        /* half ms */

        do{
            outb((1 << 6), 0x43);
            inb(0x41, low);
            inb(0x41, high);
            diff = prev - (((high & 0xff) << 8) | (low & 0xff));
            if(diff <= 0)
                diff += 0x4a9;
        }while(diff >= 0x254);

        ms--;
    }

}


