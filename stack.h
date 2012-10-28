#ifndef _STACK_H_
#define _STACK_H_

#define STACK_SIZE 1024

struct Stack_T {
  int data[STACK_SIZE];
  int index;
};

typedef struct Stack_T Stack;

Stack* stack_create();
void stack_delete(Stack* s);
Stack* stack_push(Stack* s, int val);
int stack_pop(Stack* s);
inline int stack_top(Stack* s);
inline int* stack_topp(Stack* s);
inline void stack_swap(Stack* s);
inline void stack_add(Stack* s);

#endif /* _STACK_H_ */
