/* #define DEBUG */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binomial_heap.h"
#include "disjoint_set.h"
#include "stack.h"
#include "forest.h"
#include "list.h"

struct heap* readInput(int size);

int main(int argc, char * argv[])
{
	int size;
	struct heap *incoming;
	struct dset W;
	struct dset S;
	struct stack roots;
	struct forest F;
	int * min;
	struct list *cycle;
	struct edge **lambda;
	struct edge **enter;
	struct list C;
	struct list B;
	int root, r;
	
	/* pomocne promenne*/
	unsigned int i, repr, m, cycle_length, *X, k;
	int maxw, price;
	struct edge *e, *f, *max_e;
	struct forest_node *n;
	
	
	scanf("%d\n", &size);
	incoming = readInput(size);
	
	/* initialize structures */
	dset_init(&W, size);
	dset_init(&S, size);
	min = (int *) malloc(size * sizeof(int));
	cycle  = (struct list *) malloc(size * sizeof(struct list)); 
	enter  = (struct edge **) malloc(size * sizeof(struct edge *)); 
	lambda = (struct edge **) malloc(size * sizeof(struct edge *));
	memset(min, 0, size);
	list_init(&B);
	stack_init(&roots);
	forest_init(&F);
	root = -1;
	
	/* first loop */
	for (i=0; i < size; ++i) {
		/*dset_makeset(&W, i);
		dset_makeset(&S, i);*/
		min[i] = i;
		enter[i]  = NULL;
		lambda[i] = NULL;
		list_init(&cycle[i]);
		stack_push(&roots, i);
	}
	
	/* main loop */
	while ((r = stack_pop(&roots)) != -1) {
		/* nalezeni hrany, ktera ale nesmi smerovat z Sr do Sr */
		while (1) {
			e = heap_min( &incoming[r] );
			if (e == NULL) {
				if (root != -1) { /* invalid graph */
					printf("-1\n");
					return 0;
				}
				root = r;
				break;
			}
			if (dset_find(&S, e->src) != dset_find(&S, e->dst)) break;
			if (e) free(e);
		}
		if (e == NULL) continue;
		
		/* pridani hrany do lesa */
		n = forest_insert(&F, e);
		
		/* a pridani hran cycle[r] jako deti */
		if (!list_empty(&cycle[r])) {
			list_reset(&cycle[r]);
			while ((f = list_next(&cycle[r])) != NULL) {
				forest_add_child(&F, n, f);
			}
		} else { /* nebo vlozime odkaz na hranu do lambdy */
			lambda[e->dst] = e;
		}
		
		if (dset_find(&W, e->src) != dset_find(&W, e->dst)) {
			/* spojujeme slabe komponenty u a v */
			dset_union(&W, e->src, e->dst);
			enter[r] = e;
		} else {
			if (root >= 0) continue;
			/* kopirovani hran cyklu do C */
			list_init(&C);
			list_insert(&C, e);
			cycle_length = 1;
			f = e;
			maxw = f->price + f->mod;
			max_e = f;
			while (1) {
				f = enter[dset_find(&S, f->src)];
				if (f == NULL) break;
				list_insert(&C, f);
				
				/* hledani nejmensi hrany v C */
				if ((f->price + f->mod) > maxw) {
					maxw     = (f->price + f->mod);
					max_e    = f;
				}
				
				++cycle_length;
				if (dset_find(&S,f->src) == dset_find(&S,e->dst)) break;
			}
			
			/* prochazeni hran cyklu */
			X = (unsigned int *) malloc(cycle_length * sizeof(unsigned int));
			memset(X, 0, cycle_length);
			list_reset(&C);
			i = 0;
			while ((e = list_next(&C)) != NULL) {
				k = dset_find(&S, e->dst);
				X[i++] = k;
				heap_increase_all(&incoming[k], maxw - (e->price + e->mod));
			}
			
			/* sjednoceni komponent do S */
			m = min[dset_find(&S, max_e->dst)];
			for (i=1; i < cycle_length; i++) {
				dset_union(&S, X[i-1], X[i]);
			}
			repr = dset_find(&S, r);
			min[repr] = m;
			stack_push(&roots, repr);
			
			/* uloz C do cycle[repr] */
			list_delete(&cycle[repr]);
			cycle[repr].head = C.head;
			for (i=0; i < cycle_length; i++) {
				if (incoming[X[i]].head == NULL || X[i] == repr) continue;
				heap_union(&incoming[repr], &incoming[X[i]]);
			}
			free(X);
		}
	}
	
	#ifdef DEBUG
	printf("root=%d min[root]=%d\n", root, min[root]);
	printf("roots empty? %d\n", stack_pop(&roots));
	dset_print(&W);
	dset_print(&S);
	#endif
	/* mame koren, hura!!!! */
	
	if (lambda[min[root]] != NULL) {
		forest_delete_path(&F, lambda[min[root]]->node);
	}
	
	price = 0;
	while (!forest_empty(&F)) {
		e = (struct edge *) malloc(sizeof(struct edge));
		e->dst   = F.head->e->dst;
		e->src   = F.head->e->src;
		e->price = F.head->e->price;
		e->node  = NULL;
		e->mod   = 0;
		
		list_insert(&B, e);
		price += e->price;
		forest_delete_path(&F, lambda[e->dst]->node);
		lambda[e->dst] = NULL;
	}
	
	printf("%d\n%d\n", price, min[root]);
	
	list_reset(&B);
	while ((e = list_next(&B)) != NULL) {
		printf("%d %d %d\n", e->src, e->dst, e->price);
		free(e);
	}
	printf("0 0 0\n");
	
	/* dealokace */
	for (i=0; i < size; ++i) {
		/*if (enter[i]) free(enter[i]);
		if (lambda[i]) free(lambda[i]);*/
		list_delete(&cycle[i]);
		heap_delete(&incoming[i]);
	}
	stack_delete(&roots);
	/*heap_delete(&incoming[min[root]]);*/
	dset_delete(&W);
	dset_delete(&S);
	
	list_delete(&B);
	/*list_delete(&C);*/
	
	free(min);
	free(cycle);
	free(enter);
	free(lambda);
	free(incoming);
	
	return 0;
}

struct heap* readInput(int size)
{
	struct heap* incoming;
	int src, dst, price;
	int i;
	
	incoming = (struct heap*) malloc(size * sizeof(struct heap));
	for (i=0; i < size; i++) {
		incoming[i].head = NULL;
		incoming[i].mod  = 0;
	}
	
	scanf("%d %d %d\n", &src, &dst, &price);
	while (src != 0 || dst != 0 || price != 0) {
		if (incoming[dst].head == NULL) {
			heap_init(&incoming[dst]);
		}
		heap_insert(&incoming[dst], price, src, dst);
		if (scanf("%d %d %d\n", &src, &dst, &price) == -1) return incoming;
		
	}
	return incoming;
}
