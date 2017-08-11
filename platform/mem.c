#include "mem.h"
#include "rtos.h"


void *mem_alloc(e_uint32_t size)
{
    return pvPortMalloc(size);
}

void mem_free(void *mem)
{
    vPortFree(mem);
}