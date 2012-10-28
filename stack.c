#include "stack.h"
#include <stdlib.h>
#include <string.h>

Stack* stack_create()
{
  Stack* s = (Stack*) malloc(sizeof(Stack));
  s->index = 0;
  memset(s->data, 0, sizeof(s->data));
  return s;
}

void stack_delete(Stack* s)
{
  free(s);
}

inline Stack* stack_push(Stack* s, int val)
{
  s->data[s->index] = val;
  s->index++;
  return s;
}

inline int stack_pop(Stack* s)
{
  s->index--;
  int val = s->data[s->index];
  return val;
}

inline int stack_top(Stack* s)
{
  return s->data[s->index - 1];
}

inline int* stack_topp(Stack* s)
{
  return &(s->data[s->index - 1]);
}

inline void stack_swap(Stack* s)
{
  int v = s->data[s->index - 2];
  s->data[s->index - 2] = s->data[s->index - 1];
  s->data[s->index - 1] = v;
}

inline void stack_add(Stack* s)
{
  s->data[s->index - 2] = s->data[s->index - 1] + s->data[s->index - 2];
  s->index--;
}
