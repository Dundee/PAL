#include <stdlib.h>
#include "list.h"

struct list_node {
	struct list_node* next;
	struct edge* e;
};

inline void list_init(struct list* l)
{
	l->head = NULL;
	l->cur  = NULL;
}

inline int list_empty(struct list* l)
{
	return (l->head == NULL);
}

inline void list_insert(struct list* l, struct edge * e)
{
	struct list_node * n;
	n = (struct list_node *) malloc(sizeof(struct list_node));
	n->e = e;
	n->next = l->head;
	
	l->head = l->cur = n;
}

inline struct edge * list_next(struct list* l)
{
	struct edge * e;
	
	if (l->head == NULL) return NULL;
	if (l->cur == NULL) return NULL;
	e = l->cur->e;
	l->cur = l->cur->next;
	return e;
}

inline void list_reset(struct list* l)
{
	l->cur = l->head;
}

inline void list_delete(struct list *l)
{
	struct list_node *node, *next;
	node = l->head;
	while (node) {
		next = node->next;
		node->next = NULL;
		free(node);
		node = next;
	}
	l->head = NULL;
}
