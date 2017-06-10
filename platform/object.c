#include "object.h"
#include <string.h>
#include "link_list.h"

static LIST_INIT(object_list);


void object_init(struct object *object,
                    const char *name)
{
    
    strncpy(object->name, name, E_NAME_MAX);
    
    list_add(&object_list, &object->list);
}

void object_detach(object_t object)
{
    
    list_del(&(object->list));
    
}


object_t object_find(const char *name)
{
    struct object *object = NULL;
    struct list_head *node = NULL;
    struct list_head *tnode = NULL;
    
    
    if(name == NULL)
    {
        return NULL;
    }
    
    list_for_each_safe(node, tnode, &object_list)
    {
        object = list_entry(node, struct object, list);
        if(strncmp(object->name, name, E_NAME_MAX)==0)
        {
            return object;
        }
    }
    
    return NULL;
}