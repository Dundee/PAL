#ifndef LIST_H
#define LIST_H

#include "binomial_heap.h"

struct list {
	struct list_node* head;
	struct list_node* cur;
};

inline void list_init(struct list* l);
inline int list_empty(struct list* l);
inline void list_insert(struct list* l, struct edge * e);
inline struct edge * list_next(struct list* l);
inline void list_reset(struct list* l);
inline void list_delete(struct list *l);

#endif
