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
  }

  sprintf(s, "%s %s", op, val);
#undef ITOA
}
