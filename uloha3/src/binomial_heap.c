#ifndef BIONOM_HEAP
#define BIONOM_HEAP

#include <stdlib.h>
#include "binomial_heap.h"

struct heap_node {
	struct heap_node* parent;
	struct heap_node* next;
	struct heap_node* child;

	unsigned int degree;
	int key;
	int mod;
	struct edge * e;
};

static void __heap_connect(struct heap_node* root, struct heap_node* child);
static struct heap_node* __heap_reverse(struct heap_node* h);
static struct heap_node* __heap_merge(struct heap_node* a, struct heap_node* b);
static void __heap_union(struct heap* h, struct heap_node* h2);

inline void heap_init(struct heap* h)
{
	h->head = NULL;
	h->mod  = 0;
}

inline void heap_node_init(struct heap_node* h, int key, struct edge* e)
{
	h->parent = NULL;
	h->next   = NULL;
	h->child  = NULL;
	h->degree = 0;
	h->e      = e;
	h->key    = key;
}

inline void heap_node_delete(struct heap_node *n)
{
	struct heap_node *next;
	
	while (n) {
		if (n->child) heap_node_delete(n->child);
		n->child = NULL;
		next = n->next;
		free(n->e);
		free(n);
		n = next;
	}
}

inline void heap_delete(struct heap *h)
{
	if (h->head) heap_node_delete(h->head);
	h->head = NULL;
}

inline int heap_empty(struct heap* heap)
{
	return (heap->head == NULL);
}

/* udelej z child podstrom rootu */
static void __heap_connect(struct heap_node* root, struct heap_node* child)
{
	struct heap_node * n;
	/* propagace mod synum */
	if (root->child != NULL) {
		n = root->child;
		while(n != NULL) {
			n->mod += root->mod;
			n->key += root->mod;
			n = n->next;
		}
	}
	root->e->mod += root->mod;
	root->mod = 0;
	
	child->parent = root;
	child->next   = root->child;
	root->child   = child;
	root->degree++;
}

static struct heap_node* __heap_reverse(struct heap_node* h)
{
	struct heap_node* tail = NULL;
	struct heap_node* next;

	if (!h)
		return h;

	h->parent = NULL;
	while (h->next) {
		next    = h->next;
		h->next = tail;
		tail    = h;
		h       = next;
		h->parent = NULL;
	}
	h->next = tail;
	return h;
}

/* sluc korenove seznamy */
static struct heap_node* __heap_merge(struct heap_node* a, struct heap_node* b)
{
	struct heap_node* head = NULL;
	struct heap_node** pos = &head;

	while (a && b) {
		if (a->degree < b->degree) {
			*pos = a;
			a = a->next;
		} else {
			*pos = b;
			b = b->next;
		}
		pos = &(*pos)->next;
	}
	
	if (a) *pos = a;
	else   *pos = b;
	return head;
}

static void __heap_union(struct heap* h, struct heap_node* h2)
{
	struct heap_node* h1;
	struct heap_node *prev, *n, *next;
	if (!h2)
		return;
	h1 = h->head;
	if (!h1) {
		h->head = h2;
		return;
	}
	
	/* propagace mod */
	while (h1) {
		h1->mod += h->mod;
		h1->key += h->mod;
		h1 = h1->next;
	}
	h->mod = 0;
	h1 = h->head;
	h1 = __heap_merge(h1, h2);
	prev = NULL;
	n    = h1;
	next = n->next;
	while (next) {
		if (n->degree != next->degree || 
		   (next->next && next->next->degree == n->degree) /* tri stejne vysoke stromy za sebou */
		   ) {
			/* nic, pokracuj */
			prev = n;
			n    = next;
		} else if (n->key < next->key) {
			/* x se stav korenem next */
			n->next = next->next;
			__heap_connect(n, next);
		} else {
			/* next se stava korenem x */
			if (prev)
				prev->next = next;
			else
				h1 = next;
			__heap_connect(next, n);
			n = next;
		}
		next = n->next;
	}
	h->head = h1;
}

/* vloz hranu do haldy */
void heap_insert(struct heap* h, int key, int src, int dst)
{
	struct heap_node * node;
	struct edge * e;
	
	node = (struct heap_node *) malloc(sizeof(struct heap_node));
	e    = (struct edge *) malloc(sizeof(struct edge));
	
	e->src        = src;
	e->dst        = dst;
	e->price      = key;
	e->mod        = 0;
	e->node       = NULL;
	heap_node_init(node, key, e);
	
	node->child  = NULL;
	node->parent = NULL;
	node->next   = NULL;
	node->degree = 0;
	
	__heap_union(h, node);
}

/* sluc pridavek do cile */
void heap_union(struct heap* target, struct heap* addition)
{
	struct heap_node *n;
	n = addition->head;
	while (n) {
		n->mod += addition->mod;
		n->key += addition->mod;
		n = n->next;
	}
	addition->mod = 0;
	__heap_union(target, addition->head);
	addition->head = NULL;
}

struct edge* heap_min(struct heap* h)
{
	struct heap_node *prev, *node, *m, *n;
	struct edge *e;
	node = h->head;
	m = h->head;
	/* hledam nejmensi */
	if (node) {
		prev = NULL;
		n = h->head->next;
		while (n) {
			if (n->key < node->key) {
				node = n;
				prev = m;
			}
			m = n;
			n = n->next;
		}
	}
	
	/* odebiram */
	if (!node) return NULL;
	if (prev) prev->next = node->next;
	else h->head = node->next;
	if (node->child != NULL) {
		n = node->child;
		while (n != NULL) {
			n->mod += node->mod;
			n->key += node->mod;
			n = n->next;
		}
	}
	__heap_union(h, __heap_reverse(node->child));
	node->degree  = 0;
	node->e->mod += h->mod + node->mod;
	
	e = node->e;
	free(node);
	return e;
}

inline void heap_increase_all(struct heap* h, int c)
{
	if (h->head == NULL) return;
	h->mod += c;
}

#endif /* BIONOM_HEAP */
