#include <stdlib.h>
#include "binomial_heap.h"
#include "forest.h"

inline void forest_init(struct forest *f)
{
	f->head = NULL;
}

void forest_delete_tree(struct forest_node *n)
{
	struct forest_node *next;
	
	while (n) {
		if (n->child) forest_delete_tree(n->child);
		n->child = NULL;
		next = n->next;
		free(n->e);
		free(n);
		n = next;
	}
}

inline void forest_delete(struct forest *f)
{
	if (f->head) forest_delete_tree(f->head);
	f->head = NULL;
}

inline int forest_empty(struct forest *f)
{
	return (f->head == NULL);
}

struct forest_node* forest_insert(struct forest *f, struct edge *e)
{
	struct forest_node *n;
	n = (struct forest_node *) malloc(sizeof(struct forest_node));
	n->e = e;
	n->next = f->head;
	n->parent = NULL;
	n->child  = NULL;
	n->prev   = NULL;
	e->node   = n;
	if (f->head != NULL) f->head->prev = n;
	
	f->head = n;
	return n;
}

void forest_add_child(struct forest* f, struct forest_node* node, struct edge* e)
{
	struct forest_node * n;
	
	n = e->node;
	if (n->prev == NULL) { /* n je prvni uzel */
		f->head = n->next;
	} else {
		n->prev->next = n->next;
	}
	if (n->next != NULL) n->next->prev = n->prev; /* neni posledni */
		
	n->prev = NULL;
	if (node->child != NULL) node->child->prev = n;
	n->next     = node->child;
	n->parent   = node;
	node->child = n;
}

void forest_delete_path(struct forest *f, struct forest_node* from)
{
	struct forest_node *n;
	if (from == NULL) return;
	
	/* zvednuti potomku o rad vys */
	if (from->child != NULL) {
		n = from->child;
		n->parent = from->parent;
		n->prev = from;
		while (n->next != NULL) { /* hledame posledni ditko */
			n->parent = from->parent;
			n = n->next; 
		}
		n->parent = from->parent;
		n->next = from->next;
		if (from->next != NULL) from->next->prev = n;
		from->next = from->child;
	}
	
	/* odstraneni hrany z lesa */
	if (from->prev == NULL) { /* prvni */
		if (from->parent != NULL) { /* prvni dite */
			from->parent->child = from->next;
		} else {
			f->head = from->next;
		}
	} else {
		from->prev->next = from->next;
	}
	if (from->next != NULL) from->next->prev = from->prev; /* neni posledni */
	
	n = from->parent;
	from->prev = from->next = from->parent = from->child = NULL;
	
	free(from->e);
	free(from);
	
	if (n != NULL) forest_delete_path(f, n);
	
}
