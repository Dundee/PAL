#ifndef DISCOINT_SET_H
#define DISCOINT_SET_H

struct dset {
	int size;
	struct dset_node* nodes;
};

inline void dset_init(struct dset * x, int size);
inline void dset_delete(struct dset *x);
inline void dset_makeset(struct dset * x, int vertex);
inline int dset_find(struct dset * x, int vertex);
inline void dset_union(struct dset * x, int v1, int v2);

#endif
