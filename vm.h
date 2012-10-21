#ifndef _VM_H_
#define _VM_H_

#include "program.h"
#include "stack.h"

struct VM_T {
  Program* prog;
  Stack* data_stack;
  Stack* call_stack;
  int heap[1024];
};

typedef struct VM_T VM;

VM* vm_create();
void vm_delete(VM* vm);
void vm_run(VM* vm, Program* prog);

#endif /* _VM_H_ */
