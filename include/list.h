

#ifndef __LIST_H__
#define __LIST_H__


struct list {
    struct list *prev;
    struct list *next;
};



static inline  void
list_init(struct list *list){
    list->prev = list;
    list->next = list;
}

static inline void
list_init_node(struct list *node){
    node->prev = 0;
    node->next = 0;
}

static inline void
list_add(struct list *prev, struct list *next, struct list *node){
    next->prev = node;
    node->next = next;
    prev->next = node;
    node->prev = prev;
}

static inline void
list_insert_tail(struct list *list, struct list *node){
    list_add(list->prev, list, node);
}

#define list_foreach(list, node)    \
    for( (node) = (list)->next;     \
            (node) != (list);       \
            (node) = (node)->next)

#define list_get_entry(nod, type)  \
    (type *)  ( (void *)nod - (void *)&(((type *)0)->node))
    





#endif /* __LIST_H__ */
