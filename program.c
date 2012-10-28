#include "program.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Program* program_create()
{
  Program* program = (Program*) malloc(sizeof(Program));
  program->size = 0;
  return program;
}

void program_delete(Program* program)
{
  if(program != NULL) {
    free(program);
  }
}

Program* program_insert_last(Program* program, Command command)
{
  program->commands[program->size] = command;
  program->size++;
  return program;
}

int program_search_label(Program* program, int label)
{
  int i;
  for(i = 0; i < program->size; i++)
  {
    if(program->commands[i].op == MARK && 
       program->commands[i].param == label) {
      return i;
    }
  }
  return -1;
}

void program_replace_label(Program* program)
{
  int i;
  for(i = 0; i < program->size; i++)
  {
    enum Operation op = program->commands[i].op;
    if(op == CALL || op == JUMP || op == JUMPIFZERO || op == JUMPIFNEGATIVE) {
      int pc = program_search_label(program, program->commands[i].param);
      program->commands[i].param = pc;
    }
  }
}

void program_optimize(Program* program)
{
  int i, size = program->size;
  for(i = 0; i < size - 1; i++)
  {
    enum Operation op1 = program->commands[i].op;
    enum Operation op2 = program->commands[i+1].op;
    if(op1 == PUSH && op2 == ADD) {
      program->commands[i].op = PUSH_ADD;
      program->commands[i].param = program->commands[i].param;
      program->commands[i+1].op = NOP;
      memmove(&(program->commands[i+1]), &(program->commands[i+2]), sizeof(*(program->commands)) * (size - i - 1));
      size--;
    }
  }

  for(i = 0; i < size - 1; i++)
  {
    enum Operation op1 = program->commands[i].op;
    enum Operation op2 = program->commands[i+1].op;
    if(op1 == DUPLICATE && op2 == PUSH_ADD) {
      program->commands[i].op = DUPLICATE_PUSH_ADD;
      program->commands[i].param = program->commands[i+1].param;
      program->commands[i+1].op = NOP;
      memmove(&(program->commands[i+1]), &(program->commands[i+2]), sizeof(*(program->commands)) * (size - i - 1));
      size--;
    }
  }
  program->size = size;
}

void command_string(Command command, char* s);

void program_print(Program* program)
{
  int i;
  char code[64];
  for(i = 0; i < program->size; i++)
  {
    command_string(program->commands[i], code);
    printf("%d: %s\n", i, code);
  }
}

void command_string(Command command, char* s)
{
#define ITOA(x, c) sprintf(c, "%d", x)
#define OP(x) case x: op = #x; break
#define OPP(x) case x: op = #x; ITOA(command.param, val); break
  char *op;
  char val[32];
  memset(val, '\0', 32);
  switch(command.op) {
    OPP(PUSH);
    OP(DUPLICATE);
    OP(COPY);
    OP(SWAP);
    OP(DISCARD);
    OPP(SLIDE);

    OP(ADD);
    OP(SUB);
    OP(MUL);
    OP(DIV);
    OP(MOD);

    OP(STORE);
    OP(RETRIEVE);

    OPP(MARK);
    OPP(CALL);
    OPP(JUMP);
    OPP(JUMPIFZERO);
    OPP(JUMPIFNEGATIVE);
    OP(RETURN);
    OP(END);

    OP(PUTC);
    OP(PUTN);
    OP(READC);
    OP(READN);

    OPP(PUSH_ADD);
    OP(NOP);
    OPP(DUPLICATE_PUSH_ADD);
  }

  sprintf(s, "%s %s", op, val);
#undef ITOA
}
