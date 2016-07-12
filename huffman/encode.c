#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.c"

int getMin(struct list *l1, struct list *l2, struct list_node *min[2])
{
	int i;
	if (!l1->head) {
		if (!l2->head || !l2->head->next) return 0;
	}
	if (!l2->head) {
		if (!l1->head || !l1->head->next) return 0;
	}
	
	for (i=0; i < 2; ++i) {
		if (l1->head && (!l2->head || l1->head->key <= l2->head->key)) {
			min[i] = l1->head;
			list_delete_first(l1);
			min[i]->next = NULL;
		} else {
			min[i] = l2->head;
			list_delete_first(l2);
			min[i]->next = NULL;
		}
	}
	return 1;
}

void insertParent(struct list *l2, struct list_node *min[2])
{
	struct list_node * parent;
	parent = (struct list_node *) malloc(sizeof(struct list_node));
	list_node_init(parent, min[0]->key + min[1]->key, '\0');
	parent->lchild = min[0];
	parent->rchild = min[1];
	min[0]->parent = min[1]->parent = parent;
	
	if (l2->tail) {
		l2->tail->next = parent;
		parent->prev = l2->tail;
	} else l2->head = parent;
	l2->tail = parent;
}

void printCode(struct list_node * from)
{
	int i = 7;
	int pos = 0x00000001;
	char c[8] = "        ";
	int x = 0;
	
	while (from->parent) {
		if (from == from->parent->lchild) {
			*(c+i) = '0';
		}
		else {
			*(c+i) = '1';
			x |= pos;
		}
		from = from->parent;
		--i;
		pos = pos << 1;
	}
	printf("%s %x\n", c, x);
}

int main(int argc, char * argv[])
{
	int alphabet[255];
	char * text;
	struct list nodes;
	struct list multinodes;
	struct list_node *leaves[255];
	struct list_node * min[2];
	
	int buff_size = 255;
	int i = 0, size = buff_size;
	char c;
	
	for (i=0;i<255;i++) {
		alphabet[i] = 0;
		leaves[i] = NULL;
	}
	list_init(&nodes);
	list_init(&multinodes);
	
	text = (char *) calloc(buff_size, sizeof(char));
	
	i = 0;
	while (1) {
		c = getchar();
		while( (c != EOF)
			   && i < size-1 ) {
			text[i] = c;
			alphabet[(int)c] += 1;
			++i;
			c = getchar();
		}
		if (c == EOF) break;
		size += buff_size;
		text = (char *) realloc(text, size);
		memset(text+i, 0, buff_size);
		text[i] = c;
		++i;
	}
	text[i] = '\0';
	size = strlen(text);
	
	for(i=0;i<255;i++) {
		if (alphabet[i]) {
			leaves[i] = list_insert(&nodes, alphabet[i], (char)i);
			printf("%c:%d\n", (char)i, alphabet[i]);
		}
	}
	list_sort(&nodes);
	while(getMin(&nodes, &multinodes, min)) {
		insertParent(&multinodes, min);
	}
	
	for (i=0; i < size; ++i) {
		printf("%c ", text[i]);
		printCode(leaves[(int)text[i]]);
	}
	
	
	free(text);
	return 0;
}
