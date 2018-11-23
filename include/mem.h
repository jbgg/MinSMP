
#ifndef __MEM_H__
#define __MEM_H__


#include <types.h>


void mem_setup();


void* malloc(uint32);


int memcmp(void*, void*, uint32);

int memcpy(void*, void*, uint32);





#endif /* __MEM_H__ */
