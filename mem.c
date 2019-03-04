

#include <mem.h>


extern void* _end;
void *last;


void
mem_setup(){
    last = (void*) ( (((uint32)&_end + 0x10 - 1 ) / 0x10) * 0x10 );
}


void*
malloc(uint32 size){

    void *addr = last;

    last += size;
    last = (void*) ( (((uint32)last + 0x10 - 1 ) / 0x10) * 0x10 );

    return addr;
}


int
memcmp(void *ptr1, void *ptr2, uint32 size){
    uint8 *s1 = ptr1;
    uint8 *s2 = ptr2;
    unsigned int i;

    for(i=0;i<size;i++){
        if(s1[i] != s2[i])
            return -1;
    }

    return 0;
}

int
memcpy(void *dest, void *src, uint32 size){
    uint8 *d = dest;
    uint8 *s = src;
    unsigned int i;

    for(i=0;i<size;i++){
        d[i] = s[i];
    }

    return 0;
}

