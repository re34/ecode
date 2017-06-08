#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "types.h"
#include "link_list.h"

#define OBJECT_NAME_MAX     25

struct e_object{
    char name[OBJECT_NAME_MAX];
    e_uint8_t type;
    e_uint8_t flag;
    
    e_list_t list;
};

typedef struct e_object * e_object_t;




void e_object_init(struct e_object *object,
                    const char *name);
void e_object_detach(e_object_t object);             
e_object_t e_object_find(const char *name);                 
#endif