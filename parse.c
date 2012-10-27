#include <stdio.h>
#include <string.h>
#include "command.h"
#include "program.h"

#define C1EQ(x, y) (*x[0] == y[0])
#define C2EQ(x, y) (*x[0] == y[0] && *x[1] == y[1])
#define C3EQ(x, y) (*x[0] == y[0] && *x[1] == y[1] && *x[2] == y[2])

#define S " "
#define L "\n"
#define T "\t"

#define CS ' '
#define CL '\n'
#define CT '\t'

void error_parse(char* s)
{
  fprintf(stderr, "Parse Error. %s\n", s);
}

int chars_eq(char** x, char* y)
{
  int n = strlen(y);
  return memcmp(*x, y, n) == 0;
}

int chars_eq_s(char** x, char* y)
{
  if(chars_eq(x, y)) {
    int n = strlen(y);
    *x += n * sizeof(char);
    return 1;
  } else {
    return 0;
  }
}

int parse_number(char** c)
{
  int sign;
  if(chars_eq_s(c, S)) {
    sign = 1;
  } else if(chars_eq_s(c, T)) {
    sign = -1;
  } else {
    error_parse("Number format: sign part is [LF].");
    return -1;
  }

  int num = 0;
  while(1) {
    if(**c == CT) num++;
    *c += 1;
    if(**c == CL) break;
    num *= 2;
  }

  num *= sign;
  *c += 1;
  return num;
}

int parse_label(char** c)
{
  return parse_number(c);
}

Command parse_stack_manipulation(char** c)
{
  Command command;
  if(chars_eq_s(c, S)) {
    command.op = PUSH;
    command.param = parse_number(c);
  } else if(chars_eq_s(c, L S)) {
    command.op = DUPLICATE;
  } else if(chars_eq_s(c, T S)) {
    command.op = COPY;
    command.param = parse_number(c);
  } else if(chars_eq_s(c, L T)) {
    command.op = SWAP;
  } else if(chars_eq_s(c, L L)) {
    command.op = DISCARD;
  } else if(chars_eq_s(c, T L)) {
    command.op = SLIDE;
    command.param = parse_number(c);
  }
  return command;
}

Command parse_arithmetic(char** c)
{
  Command command;
  if(chars_eq_s(c, S S)) {
    command.op = ADD;
  } else if(chars_eq_s(c, S T)) {
    command.op = SUB;
  } else if(chars_eq_s(c, S L)) {
    command.op = MUL;
  } else if(chars_eq_s(c, T S)) {
    command.op = DIV;
  } else if(chars_eq_s(c, T T)) {
    command.op = MOD;
  }
  return command;
}

Command parse_heap_access(char** c)
{
  Command command;
  if(chars_eq_s(c, S)) {
    command.op = STORE;
  } else if(chars_eq_s(c, T)){
    command.op = RETRIEVE;
  }
  return command;
}

Command parse_flow_control(char** c)
{
  Command command;
  if(chars_eq_s(c, S S)) {
    command.op = MARK;
    command.param = parse_label(c);
  } else if(chars_eq_s(c, S T)) {
    command.op = CALL;
    command.param = parse_label(c);
  } else if(chars_eq_s(c, S L)) {
    command.op = JUMP;
    command.param = parse_label(c);
  } else if(chars_eq_s(c, T S)) {
    command.op = JUMPIFZERO;
    command.param = parse_label(c);
  } else if(chars_eq_s(c, T T)) {
    command.op = JUMPIFNEGATIVE;
    command.param = parse_label(c);
  } else if(chars_eq_s(c, T L)) {
    command.op = RETURN;
  } else if(chars_eq_s(c, L L)) {
    command.op = END;
  }
  return command;
}

Command parse_io(char** c)
{
  Command command;
  if(chars_eq_s(c, S S)) {
    command.op = PUTC;
  } else if(chars_eq_s(c, S T)) {
    command.op = PUTN;
  } else if(chars_eq_s(c, T S)) {
    command.op = READC;
  } else if(chars_eq_s(c, T T)) {
    command.op = READN;
  }
  return command;
}

Program* parse(Program* program, char** c)
{
  Command command;

  while(**c != '\0') {
    if(chars_eq_s(c, S)) {
      command = parse_stack_manipulation(c);
    } else if(chars_eq_s(c, T S)) {
      command = parse_arithmetic(c);
    } else if(chars_eq_s(c, T T)) {
      command = parse_heap_access(c);
    } else if(chars_eq_s(c, L)) {
      command = parse_flow_control(c);
    } else if(chars_eq_s(c, T L)) {
      command = parse_io(c);
    }
    program = program_insert_last(program, command);
  }

  return program;
}
