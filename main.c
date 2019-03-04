

#include <video.h>
#include <mem.h>
#include <acpi.h>
#include <pit.h>
#include <mp.h>
#include <list.h>

void
main(){

    mem_setup();

    video_setup();

    if(acpi_setup()){
        puts("ACPI error");
        goto idle;
    }

    if(pit_setup()){
        puts("PIT error");
        goto idle;
    }

    if(mp_setup()){
        puts("MP error");
        goto idle;
    }

    puts("MinSMP");

    acpi_print_info();

    mp_print_info();

idle:
    for(;;)
        __asm__("hlt;");


}

