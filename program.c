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
  char *op;
  char val[32];
  memset(val, '\0', 32);
  switch(command.op) {
  case PUSH:
    op = "PUSH";
    ITOA(command.param, val);
    break;
  case DUPLICATE:
    op = "DUPLICATE";
    break;
  case COPY:
    op = "COPY";
    ITOA(command.param, val);
    break;
  case SWAP:
    op = "SWAP";
    break;
  case DISCARD:
    op = "DISCARD";
    break;
  case SLIDE:
    op = "SLIDE";
    ITOA(command.param, val);
    break;

  case ADD:
    op = "ADD";
    break;
  case SUB:
    op = "SUB";
    break;
  case MUL:
    op = "MUL";
    break;
  case DIV:
    op = "DIV";
    break;
  case MOD:
    op = "MOD";
    break;

  case STORE:
    op = "STORE";
    break;
  case RETRIEVE:
    op = "RETRIEVE";
    break;

  case MARK:
    op = "MARK";
    ITOA(command.param, val);
    break;
  case CALL:
    op = "CALL";
    ITOA(command.param, val);
    break;
  case JUMP:
    op = "JUMP";
    ITOA(command.param, val);
    break;
  case JUMPIFZERO:
    op = "JUMPIFZERO";
    ITOA(command.param, val);
    break;
  case JUMPIFNEGATIVE:
    op = "JUMPIFNEGATIVE";
    ITOA(command.param, val);
    break;
  case RETURN:
    op = "RETURN";
    break;
  case END:
    op = "END";
    break;

  case PUTC:
    op = "PUTC";
    break;
  case PUTN:
    op = "PUTN";
    break;
  case READC:
    op = "READC";
    break;
  case READN:
    op = "READN";
    break;
  }

  sprintf(s, "%s %s", op, val);
#undef ITOA
}
