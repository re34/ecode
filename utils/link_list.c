#include "link_list.h"


#ifndef NULL
#define NULL    ((void *)0)
#endif


static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->prev=NULL;
    entry->next=NULL;
}

static inline void __list_add(struct list_head *entry,struct list_head *prev, struct list_head *next)
{
    next->prev = entry;
    entry->next = next;
    entry->prev = prev;
    prev->next = entry;
}

void list_add(struct list_head *head, struct list_head *entry)
{
    __list_add(entry, head, head->next);

}

int list_empty(const struct list_head *head)
{
    return (head->next==head);
}

