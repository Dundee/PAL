#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	FRESH,
	OPEN,
	CLOSED
} t_status;

struct _list{
	int val;
	struct _list * next;
};
typedef struct _list list;

typedef struct{
	t_status status;
	int level;
	list *head;
} node;

int size;
node *nodes;

void addChild(int src, int dst)
{
	list * head;

	if (nodes[src].head == NULL) {
		head = (list *) calloc(1, sizeof(list));
		head->val = dst;
		nodes[src].head = head;
	} else {
		/*if (end->val == dst) return;*/
		head = (list *) calloc(1, sizeof(list));
		head->val = dst;
		head->next = nodes[src].head;
		nodes[src].head = head;
	}
}

void read()
{
	int /*i,*/ src, dst;

	nodes = (node*) malloc(size * sizeof(node));
	nodes = memset(nodes, 0, size);
	/*for (i = 0; i < size; ++i) {
		nodes[i].status = FRESH;
	}*/

	do {
		scanf("%d %d\n", &src, &dst);
		if (src == 0 && dst == 0) break;
		/*if (src == dst) continue;*/
		/*if (src >= size || dst >= size) continue;*/

		addChild(dst, src);
		addChild(src, dst);
	} while (1);
}

int DFS(int i, int parent, int level)
{
	int min_level = level, ret;
	list * cur;

	nodes[i].status = OPEN;
	nodes[i].level  = level;
	cur = nodes[i].head;

	while (cur != NULL) {
		if (cur->val != parent) {
			if (nodes[cur->val].status == FRESH) {
				ret = DFS(cur->val, i, level + 1);
				if (ret < min_level) min_level = ret;
				if (ret > level) {
					printf("%d %d\n", i, cur->val);
				}
			} else {
				if (nodes[cur->val].level < min_level) {
					min_level = nodes[cur->val].level;
				}
			}
		}
		cur = cur->next;
	}

	return min_level;
}

int main(int argc, char* argv[]) {
	scanf("%d\n", &size);

	read();
	DFS(0, 0, 0);
	printf("0 0\n");
	free(nodes);

	return 0;
}
