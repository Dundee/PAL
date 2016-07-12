#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "disjoint_set.h"

struct dset_node {
	unsigned int r; /* representant */
	unsigned int size;
};

void dset_init(struct dset * x, int size)
{
	int i;
	
	x->nodes = (struct dset_node *) malloc(size * sizeof(struct dset_node));
	for (i = 0; i < size; i++) {
		x->nodes[i].r = i;
		x->nodes[i].size = 0;
	}
	x->size = size;
}

inline void dset_delete(struct dset *x)
{
	if (x->nodes) free(x->nodes);
	x->nodes = NULL;
}

inline void dset_makeset(struct dset * x, int vertex)
{
	if (vertex >= x->size) return;
	x->nodes[vertex].r = vertex;
}

/* returns set representant */
int dset_find(struct dset * x, int vertex)
{
	int cur = vertex;
	if (vertex >= x->size) return 0;
	while (x->nodes[cur].r != cur) { /* x->nodes[cur].r != 0 &&  */
		cur = dset_find(x, x->nodes[cur].r);
	}
	x->nodes[vertex].r = cur;
	return cur;
}

void dset_union(struct dset * x, int v1, int v2)
{
	if (v1 > x->size || v2 > x->size) return;
	v1 = dset_find(x, v1);
	v2 = dset_find(x, v2);
	
	if (x->nodes[v2].size > x->nodes[v1].size) {
		x->nodes[v1].r = v2;
		(x->nodes[v2].size)++;
	} else {
		x->nodes[v2].r = v1;
		(x->nodes[v1].size)++;
	}
}

#ifdef DEBUG
inline void dset_print(struct dset * x)
{
	int i;
	for (i = 0; i < x->size; i++) {
		printf ("%d(%d) -> %d\n", i, x->nodes[i].size, x->nodes[i].r);
	}
}
#endif
