OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(_start)

SECTIONS
{
    . = _KERNEL_ADDR;

    .boot : {
        *(.boot*)
    }

    .text : {
        *(.text)
        *(.text*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata*)
    }

    .data : {
        *(.data)
        *(.data*)
    }

    .bss : {
        *(.bss)
        *(.bss*)
    }

    _end = .;

}




