#include <stdlib.h>
#include <string.h>
#include "stack.h"

struct stack_node {
	int v;
	struct stack_node* next;
};

inline void stack_node_init(struct stack_node * n)
{
	n->v = -1;
	n->next = NULL;
}

inline void stack_init(struct stack * s)
{
	s->head = NULL; /*(struct stack_node *) malloc(sizeof(struct stack_node));
	stack_node_init(s->head);/*/
}

inline void stack_delete(struct stack *s)
{
	struct stack_node *node, *next;
	node = s->head;
	while (node) {
		next = node->next;
		free(node);
		node = next;
	}
	s->head = NULL;
}

inline void stack_push(struct stack * s, int v)
{
	struct stack_node * n;
	
	/*if (s->head->v == -1) {
		s->head->v = v;
		return;
	}*/
	n = (struct stack_node *) malloc(sizeof(struct stack_node));
	stack_node_init(n);
	n->next = s->head;
	n->v = v;
	s->head = n;
}

inline int stack_pop(struct stack * s)
{
	struct stack_node *n;
	int v;
	if (s->head == NULL) return -1;
	v = s->head->v;
	n = s->head;
	s->head = s->head->next;
	free(n);
	return v;
}
