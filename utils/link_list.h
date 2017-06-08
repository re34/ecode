#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

struct list_head{
    struct list_head *next;
    struct list_head *prev;
};

typedef struct list_head e_list_t;

#define LIST_HEAD_INIT(name)    {&(name), &(name)}
#define LIST_HEAD(name)\
        struct list_head name = LIST_HEAD_INIT(name)

#define LIST_INIT(name)\
        struct list_head name = LIST_HEAD_INIT(name)

#define list_for_each(pos, head)\
        for(pos=(head)->next;pos!=(head);pos=pos->next)
            
#define list_for_each_safe(pos, n, head)\
        for(pos = (head)->next,n=pos->next;pos!=(head);\
            pos=n,n=pos->next)
            

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define list_entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = rt_list_entry(pos->member.next, typeof(*pos), member))
         
/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)
            
static inline void __list_del(struct list_head *prev, struct list_head *next);
void list_del(struct list_head *entry);
void list_add(struct list_head *head, struct list_head *node);
int list_empty(const struct list_head *head);

#endif