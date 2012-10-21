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

#define POP2VALS(v1, v2, s) (v1 = stack_pop(s) && v2 = stack_pop(s))

void vm_run(VM* vm, Program* prog)
{
  vm->prog = prog;
  Command* commands = prog->commands;
  Command c;
  Stack* data_stack = vm->data_stack;
  Stack* call_stack = vm->call_stack;
  int pc = 0, v1, v2, val, l;

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

    /* Srithmetic */
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
      vm->heap[v2] = v1;
      break;
    case RETRIEVE:
      v1 = stack_pop(data_stack);
      val = vm->heap[v1];
      stack_push(data_stack, val);
      break;

    /* Flow Control */
    case MARK:
      break;
    case CALL:
      stack_push(call_stack, pc);
      pc = program_search_label(prog, c.param);
      break;
    case JUMP:
      pc = program_search_label(prog, c.param);
      break;
    case JUMPIFZERO:
      l = program_search_label(prog, c.param);
      v1 = stack_pop(data_stack);
      if(v1 == 0) {
        pc = l;
      }
      break;
    case JUMPIFNEGATIVE:
      l = program_search_label(prog, c.param);
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
    }

    pc += 1;
  }
}
