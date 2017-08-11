#ifndef __MEM_H__
#define __MEM_H__
#include "types.h"

void *mem_alloc(e_uint32_t size);
void mem_free(void *mem);

#endif