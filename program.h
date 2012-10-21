#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "command.h"

#define PROGRAM_SIZE_MAX 16384

struct Program_T {
  Command commands[PROGRAM_SIZE_MAX];
  int size;
};

typedef struct Program_T Program;

Program* program_create();
void program_delete(Program* program);
Program* program_insert_last(Program* program, Command command);
void program_print(Program* program);
void program_print(Program* program);
int program_search_label(Program* program, int label);

#endif /* _PROGRAM_H_ */
