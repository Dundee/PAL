#ifndef STACK_H
#define STACK_H

struct stack {
	struct stack_node* head;
};

inline void stack_node_init(struct stack_node * n);
inline void stack_init(struct stack * s);
inline void stack_delete(struct stack *s);
inline void stack_push(struct stack * s, int v);
inline int stack_pop(struct stack * s);

#endif
