#ifndef FOREST_H
#define FOREST_H

#include "binomial_heap.h"

struct forest_node {
	struct forest_node *parent;
	struct forest_node *next;
	struct forest_node *prev;
	struct forest_node *child;

	struct edge *e;
};

struct forest {
	struct forest_node *head;
};

inline void forest_init(struct forest *f);
inline void forest_delete_tree(struct forest_node *n);
inline void forest_delete(struct forest *f);
inline int forest_empty(struct forest *f);
inline struct forest_node* forest_insert(struct forest *f, struct edge *e);
inline void forest_add_child(struct forest* f, struct forest_node* node, struct edge* e);
inline void forest_delete_path(struct forest *f, struct forest_node* from);

#endif
