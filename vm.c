#include "program.h"
#include "command.h"
#include "stack.h"
#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

VM* vm_create()
{
  VM* vm = (VM*) malloc (sizeof(VM));
  vm->data_stack = stack_create();
  vm->call_stack = stack_create();
  return vm;
}

void vm_delete(VM* vm)
{
  stack_delete(vm->data_stack);
  stack_delete(vm->call_stack);
  free(vm);
}

inline void heap_store(VM* vm, int addr, int val)
{
  vm->heap[addr] = val;
}

inline int heap_retrieve(VM* vm, int addr)
{
  return vm->heap[addr];
}

#define POP2VALS(v1, v2, s) (v1 = stack_pop(s) && v2 = stack_pop(s))

void vm_run(VM* vm, Program* prog)
{
  vm->prog = prog;
  Command* commands = prog->commands;
  Command c;
  Stack* data_stack = vm->data_stack;
  Stack* call_stack = vm->call_stack;
  int pc = 0, v1, v2, val, l, *ip;

  while(1) {
    c = commands[pc];
    switch(c.op) {
      
    /* Stack Manipulation */
    case PUSH:
      stack_push(data_stack, c.param);
      break;
    case DUPLICATE:
      val = stack_pop(data_stack);
      stack_push(data_stack, val);
      stack_push(data_stack, val);
      break;
    case COPY:
      printf("No implemented Error.");
      break;
    case SWAP:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      stack_push(data_stack, v1);
      stack_push(data_stack, v2);
      break;
    case DISCARD:
      stack_pop(data_stack);
      break;
    case SLIDE:
      printf("No implemented Error.");
      break;

    /* Arithmetic */
    case ADD:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 + v2;
      stack_push(data_stack, val);
      break;
    case SUB:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 - v2;
      stack_push(data_stack, val);
      break;
    case MUL:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 * v2;
      stack_push(data_stack, val);
      break;
    case DIV:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 / v2;
      stack_push(data_stack, val);
      break;
    case MOD:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 % v2;
      stack_push(data_stack, val);
      break;

    /* Heap Access */
    case STORE:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      heap_store(vm, v2, v1);
      break;
    case RETRIEVE:
      v1 = stack_pop(data_stack);
      val = heap_retrieve(vm, v1);
      stack_push(data_stack, val);
      break;

    /* Flow Control */
    case MARK:
      break;
    case CALL:
      stack_push(call_stack, pc);
      pc = c.param;
      break;
    case JUMP:
      pc = c.param;
      break;
    case JUMPIFZERO:
      l = c.param;
      v1 = stack_pop(data_stack);
      if(v1 == 0) {
        pc = l;
      }
      break;
    case JUMPIFNEGATIVE:
      l = c.param;
      v1 = stack_pop(data_stack);
      if(v1 < 0) {
        pc = l;
      }
      break;
    case RETURN:
      pc = stack_pop(call_stack);
      break;
    case END:
      return;

    /* I/O */
    case PUTC:
      v1 = stack_pop(data_stack);
      printf("%c", v1);
      break;
    case PUTN:
      v1 = stack_pop(data_stack);
      printf("%d", v1);
      break;
    case READC:
      printf("No implemented Error.");
      break;
    case READN:
      printf("No implemented Error.");
      break;

    /* Optimize */
    case NOP:
      break;
    case PUSH_ADD:
      ip = stack_topp(data_stack);
      *ip = *ip + c.param;
      break;
    case DUPLICATE_PUSH_ADD:
      v1 = stack_top(data_stack);
      stack_push(data_stack, v1 + c.param);
      break;
    }

    pc += 1;
  }
}

void vm_fast_run(VM* vm, Program* prog)
{
  vm->prog = prog;
  Command* commands = prog->commands;
  Command c;
  Stack* data_stack = vm->data_stack;
  Stack* call_stack = vm->call_stack;
  int pc = 0, v1, v2, val, l, *ip;

#define ARR_ADD(x) arr[x] = && L_ ## x;
  void* arr[64];
  ARR_ADD(PUSH);
  ARR_ADD(DUPLICATE);
  ARR_ADD(COPY);
  ARR_ADD(SWAP);
  ARR_ADD(DISCARD);
  ARR_ADD(SLIDE);
  ARR_ADD(ADD);
  ARR_ADD(SUB);
  ARR_ADD(MUL);
  ARR_ADD(DIV);
  ARR_ADD(MOD);
  ARR_ADD(STORE);
  ARR_ADD(RETRIEVE);
  ARR_ADD(MARK);
  ARR_ADD(CALL);
  ARR_ADD(JUMP);
  ARR_ADD(JUMPIFZERO);
  ARR_ADD(JUMPIFNEGATIVE);
  ARR_ADD(RETURN);
  ARR_ADD(END);
  ARR_ADD(PUTC);
  ARR_ADD(PUTN);
  ARR_ADD(READC);
  ARR_ADD(READN);
  ARR_ADD(PUSH_ADD);
  ARR_ADD(DUPLICATE_PUSH_ADD);
  ARR_ADD(NOP);
#undef ARR_ADD

  while(1) {
    c = commands[pc];
    goto *arr[c.op];
      
    /* Stack Manipulation */
    L_PUSH:
      stack_push(data_stack, c.param);
      goto L_C_NEXT;
    L_DUPLICATE:
      val = stack_pop(data_stack);
      stack_push(data_stack, val);
      stack_push(data_stack, val);
      goto L_C_NEXT;
    L_COPY:
      printf("No implemented Error.");
      goto L_C_NEXT;
    L_SWAP:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      stack_push(data_stack, v1);
      stack_push(data_stack, v2);
      /* stack_swap(data_stack); */
      goto L_C_NEXT;
    L_DISCARD:
      stack_pop(data_stack);
      goto L_C_NEXT;
    L_SLIDE:
      printf("No implemented Error.");
      goto L_C_NEXT;

    /* Arithmetic */
    L_ADD:
      /* v1 = stack_pop(data_stack); */
      /* v2 = stack_pop(data_stack); */
      /* val = v1 + v2; */
      /* stack_push(data_stack, val); */
      stack_add(data_stack);
      goto L_C_NEXT;
    L_SUB:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 - v2;
      stack_push(data_stack, val);
      goto L_C_NEXT;
    L_MUL:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 * v2;
      stack_push(data_stack, val);
      goto L_C_NEXT;
    L_DIV:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 / v2;
      stack_push(data_stack, val);
      goto L_C_NEXT;
    L_MOD:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      val = v1 % v2;
      stack_push(data_stack, val);
      goto L_C_NEXT;

    /* Heap Access */
    L_STORE:
      v1 = stack_pop(data_stack);
      v2 = stack_pop(data_stack);
      heap_store(vm, v2, v1);
      goto L_C_NEXT;
    L_RETRIEVE:
      v1 = stack_pop(data_stack);
      val = heap_retrieve(vm, v1);
      stack_push(data_stack, val);
      goto L_C_NEXT;

    /* Flow Control */
    L_MARK:
      goto L_C_NEXT;
    L_CALL:
      stack_push(call_stack, pc);
      pc = c.param;
      goto L_C_NEXT;
    L_JUMP:
      pc = c.param;
      goto L_C_NEXT;
    L_JUMPIFZERO:
      l = c.param;
      v1 = stack_pop(data_stack);
      if(v1 == 0) {
        pc = l;
      }
      goto L_C_NEXT;
    L_JUMPIFNEGATIVE:
      l = c.param;
      v1 = stack_pop(data_stack);
      if(v1 < 0) {
        pc = l;
      }
      goto L_C_NEXT;
    L_RETURN:
      pc = stack_pop(call_stack);
      goto L_C_NEXT;
    L_END:
      return;

    /* I/O */
    L_PUTC:
      v1 = stack_pop(data_stack);
      printf("%c", v1);
      goto L_C_NEXT;
    L_PUTN:
      v1 = stack_pop(data_stack);
      printf("%d", v1);
      goto L_C_NEXT;
    L_READC:
      printf("No implemented Error.");
      goto L_C_NEXT;
    L_READN:
      printf("No implemented Error.");
      goto L_C_NEXT;

    /* Optimize */
    L_NOP:
      goto L_C_NEXT;
    L_PUSH_ADD:
      ip = stack_topp(data_stack);
      *ip = *ip + c.param;
      goto L_C_NEXT;
    L_DUPLICATE_PUSH_ADD:
      v1 = stack_top(data_stack);
      stack_push(data_stack, v1 + c.param);
      goto L_C_NEXT;

  L_C_NEXT:
    pc += 1;
  }
}
