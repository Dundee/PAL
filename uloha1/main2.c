#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0
int t = 2;
int root_node;

struct _item {
	char type;
	int input;
	int ret;
	int child1;
	int child2;
};
typedef struct _item item;

void insert(item  * cur, int child)
{
	if (cur->child1 == 0) {
		cur->child1 = child;
	} else {
		cur->child2 = child;
	}
}

void printItems(item * items, int size)
{
	int i;
	for (i = 1; i <= size; i++) {
		printf("%d: %c(%d,%d) ", i, items[i].type, items[i].input, items[i].ret);
		printf(" CH:");
		if (items[i].child1) printf("%d ", items[i].child1);
		if (items[i].child2) printf("%d ", items[i].child2);
		printf("\n");
	}
	printf("\n");
}

int getRoot(item * items, int items_count)
{
	int i;

	for (i = 1; i <= items_count; i++) {
		if (items[i].input == 0) return i;
	}
	return 0;
}

int DFS(item * items, int items_count, int root)
{
	int i1, i2;
	char t1, t2;
	item * node;

	node = &items[root];

	if (node->child1) {
		if (!items[node->child1].ret) {
			i1 = DFS(items, items_count, node->child1);
		} else {
			i1 = items[node->child1].ret;
		}
		t1 = items[node->child1].type == 'R' ? 'R' : 'T';
	}
	if (node->child2) {
		if (!items[node->child2].ret) {
			i2 = DFS(items, items_count, node->child2);
		} else {
			i2 = items[node->child2].ret;
		}
		t2 = items[node->child2].type == 'R' ? 'R' : 'T';
	}

	if (root == root_node) t = 1;

	switch (node->type) {
		case 'R':
		case 'r':
			node->ret = root;
			break;
		case 'N':
		case 'n':
			node->ret = t++;
			printf("NOT T%d,%c%d;\n", node->ret, t1, i1);
			break;
		case 'A':
		case 'a':
			node->ret = t++;
			printf("AND T%d,%c%d,%c%d;\n", node->ret, t1, i1, t2, i2);
			break;
		case 'O':
		case 'o':
			node->ret = t++;
			printf("OR T%d,%c%d,%c%d;\n", node->ret, t1, i1, t2, i2);
			break;
	}
	return node->ret;
}

void printAsm(item * items, int items_count)
{
	root_node = getRoot(items, items_count);
	DFS(items, items_count, root_node);
}

item * items;
int items_count;
int edge_count;

int main(int argc, char* argv[]) {
	int i, src, dst;

	scanf("%d\n", &items_count);
	/*if (DEBUG) printf("items count %d\n", items_count);*/

	items = (item *) calloc(items_count+1, sizeof(item));

	for (i=1; i <= items_count; i++) {
		scanf("%c\n", &(items[i].type));
	}

	scanf("%d\n", &edge_count);
	/*if (DEBUG) printf("edge count %d\n", edge_count);*/

	for (i=1; i <= edge_count; i++) {
		scanf("%d %d\n", &src, &dst);
		/*if (DEBUG) printf("edge: %d->%d\n", src, dst);*/

		items[src].input++;
		insert(&items[dst], src);
	}

	if (DEBUG) printItems(items, items_count);

	printAsm(items, items_count);

	/*if (DEBUG) printItems(items, items_count);*/

	/*freeItems(items, items_count);
	free(items);*/
	return (0);
}

