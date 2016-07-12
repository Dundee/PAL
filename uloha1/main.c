#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

struct list_el {
	int val;
	struct list_el * next;
};
typedef struct list_el list;


struct _item {
	char type;
	int input;
	int output;
	list * parents;
	list * childs;
};
typedef struct _item item;

list * insert(list * head, int val)
{
	list * cur = head;

	if (head == NULL) {
		head = (list *) calloc(1, sizeof(list));
		head->val = val;
	} else {
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = (list *) calloc(1, sizeof(list));
		cur->next->val = val;
	}
	return head;
}

int getRoot(item * items, int size)
{
	int i;

	for (i = 1; i <= size; i++) {
		if (items[i].input == 0) {
			return i;
		}
	}
	return 0;
}

void decreaseChilds(item * items, item * root)
{
	list * childs = root->childs;

	while (childs != NULL) {
		--(items[childs->val].input);
		childs = childs->next;
	}
}

char ** getOperands(item * items, item * root)
{
	char ** op;
	int i = 0;
	list * parents = root->parents;

	op = (char **) calloc(2, sizeof(char**));
	op[0] = (char *) calloc(20, sizeof(char*));
	op[1] = (char *) calloc(20, sizeof(char*));

	while(parents != NULL) {
		if (items[parents->val].type == 'R' || items[parents->val].type == 'r') {
			sprintf(op[i++], "R%d", items[parents->val].output);
		} else {
			sprintf(op[i++], "T%d", items[parents->val].output);
		}
		parents = parents->next;
	}
	return op;
}

void printItems(item * items, int size)
{
	int i;
	list * cur;
	for (i = 1; i <= size; i++) {
		printf("%d: %c(%d,%d) ", i, items[i].type, items[i].input, items[i].output);
		printf("P:");
		cur = items[i].parents;
		while (cur != NULL) {
			printf("%d ", cur->val);
			cur = cur->next;
		}
		printf(" CH:");
		cur = items[i].childs;
		while (cur != NULL) {
			printf("%d ", cur->val);
			cur = cur->next;
		}
		printf("\n");
	}
	printf("\n");
}

/*void freeItems(item * items, int items_count)
{
	list * cur;
	int i;

	for (i = 1; i <= items_count; i++) {
		while(items[i].parents != NULL) {
			cur = items[i].parents->next;
			free(items[i].parents);
			items[i].parents = cur;
		}
		while(items[i].childs != NULL) {
			cur = items[i].childs->next;
			free(items[i].childs);
			items[i].childs = cur;
		}
	}
}*/

void printAsm(item * items, int items_count)
{
	item * root;
	int index;
	char ** op;
	int t = 2;
	int i = 1;

	while ((index = getRoot(items, items_count)) != 0) {
		root = &(items[index]);
		root->input = -1;
		decreaseChilds(items, root);
		op = getOperands(items, root);

		if (i == items_count) t = 1;

		switch (root->type) {
			case 'R':
			case 'r':
				root->output = index;
				break;
			case 'N':
			case 'n':
				root->output = t;
				printf("NOT T%d,%s;\n", t++, op[0]);
				break;
			case 'A':
			case 'a':
				root->output = t;
				printf("AND T%d,%s,%s;\n", t++, op[0], op[1]);
				break;
			case 'O':
			case 'o':
				root->output = t;
				printf("OR T%d,%s,%s;\n", t++, op[0], op[1]);
				break;
		}
		i++;
	}
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

		items[dst].input++;
		items[src].childs  = insert(items[src].childs, dst);
		items[dst].parents = insert(items[dst].parents, src);
	}

	if (DEBUG) printItems(items, items_count);

	printAsm(items, items_count);

	if (DEBUG) printItems(items, items_count);

	/*freeItems(items, items_count);
	free(items);*/
	return (0);
}

