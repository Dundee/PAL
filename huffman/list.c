#ifndef LIST
#define LIST

#include <stdlib.h>

struct list_node {
	struct list_node* next;
	struct list_node* prev;
	int key;
	char c;
	struct list_node* parent;
	struct list_node* lchild;
	struct list_node* rchild;
};

struct list {
	struct list_node* head;
	struct list_node* tail;
};

static inline void list_init(struct list* l)
{
	l->head = NULL;
	l->tail = NULL;
}

static inline void list_node_init(struct list_node *n, int key, char c)
{
	n->key = key;
	n->c   = c;
	n->next = NULL;
	n->prev = NULL;
	n->parent = NULL;
	n->lchild = NULL;
	n->rchild = NULL;
}

static inline int list_empty(struct list* l)
{
	return (l->head == NULL);
}

static inline struct list_node * list_insert(struct list* l, int key, char c)
{
	struct list_node * n;
	n = (struct list_node *) malloc(sizeof(struct list_node));
	list_node_init(n, key, c);
	
	n->next = l->head;
	if (l->head) l->head->prev = n;
	else l->tail = n;
	
	l->head = n;
	return n;
}

static inline void list_delete(struct list *l)
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

static inline void list_delete_first(struct list *l)
{
	l->head = l->head->next;
	if (l->head) l->head->prev = NULL;
	else l->tail = NULL;
}

static inline void list_sort(struct list *l)
{
	int mod = 1;
	struct list_node *n;
	struct list_node *next;
	
	if (!l->head || !l->head->next) return;
	
	while (mod) {
		mod = 0;
		n = l->head;
		
		while (n->next) {
			next = n->next;
			if (n->key > next->key) {
				next->prev    = n->prev;
				if (n->prev) n->prev->next = next;
				else {
					l->head = next;
				}
				n->next       = next->next;
				if (next->next) next->next->prev = n;
				next->next = n;
				n->prev    = next;
				mod = 1;
			} else {
				n = n->next;
			}
		}
	}
	
	n = l->head;
	while (n->next) {
		n = n->next;
	}
	l->tail = n;
}

#endif /* LIST */
