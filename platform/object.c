#include "object.h"

static LIST_INIT(object_list);


void e_object_init(struct e_object *object,
                    const char *name)
{
    
    strncpy(object->name, name, E_NAME_MAX);
    
    list_add(&object_list, &object->list);
}

void e_object_detach(e_object object)
{
    
    list_del(&(object->list));
    
}


e_object_t e_object_find(const char *name)
{
    struct e_object *object = NULL;
    struct list_head *node = NULL;
    
    
    if(name == NULL)
    {
        return NULL;
    }
    
    list_for_each_safe(node, &object_list)
    {
        object = e_list_entry(node, struct e_object, list);
        if(strncmp(object->name, name, E_NAME_MAX)==0)
        {
            return object;
        }
    }
    
    return NULL;
}