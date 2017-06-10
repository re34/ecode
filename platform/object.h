#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "types.h"
#include "link_list.h"

#define OBJECT_NAME_MAX     25

struct object{
    char name[OBJECT_NAME_MAX];
    e_uint8_t type;
    e_uint8_t flag;
    
    e_list_t list;
};

typedef struct object * object_t;




void object_init(struct object *object,
                    const char *name);
void object_detach(object_t object);             
object_t object_find(const char *name);                 
#endif