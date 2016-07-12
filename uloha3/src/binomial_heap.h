#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

struct edge {
	unsigned int src;
	unsigned int dst;
	int price;
	int mod;
	
	struct forest_node *node;
};

struct heap {
	struct heap_node* head;
	int mod;
};

extern inline void heap_init(struct heap* h);
extern inline void heap_node_init(struct heap_node* h, int key, struct edge* e);
extern inline void heap_node_delete(struct heap_node *n);
extern inline void heap_delete(struct heap *h);
extern inline int heap_empty(struct heap* heap);
extern void heap_insert(struct heap* h, int key, int src, int dst);
extern void heap_union(struct heap* target, struct heap* addition);
extern struct edge* heap_min(struct heap* h);
extern inline void heap_increase_all(struct heap* h, int c);

#endif
