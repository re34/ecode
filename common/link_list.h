#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

struct list_head{
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD_INIT(name)    {&(name), &(name)}
#define LIST_HEAD(name)\
        struct list_head name = LIST_HEAD_INIT(name)


#define list_for_each(pos, head)\
        for(pos=(head)->next;pos!=(head);pos=pos->next)
            
#define list_for_each_safe(pos, n, head)\
        for(pos = (head)->next,n=pos->next;pos!=(head);\
            pos=n,n=pos->next)

static inline void __list_del(struct list_head *prev, struct list_head *next);
void list_del(struct list_head *entry);
void list_add(struct list_head *head, struct list_head *node);
int list_empty(const struct list_head *head);

#endif