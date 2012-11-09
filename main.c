#include <stdio.h>
#include <stdlib.h>
#include "program.h"
#include "parse.h"
#include "vm.h"

int main(int argc, char *argv[])
{
  char c;
  char* code = (char*) malloc (16000);
  int i = 0;
  while((c = getchar()) != -1) {
    if(c == ' ' || c == '\t' || c == '\n') {
      code[i++] = c;
    }
  }
  code[i++] = '\0';

  Program* program = program_create();
  char **s =  &code;
  program = parse(program, s);
  program_optimize(program);
  program_replace_label(program);
  program_print(program);

  VM* vm = vm_create();
  vm_fast_run(vm, program);

  program_delete(program);
  vm_delete(vm);

  return 0;
}

