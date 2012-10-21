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
    code[i++] = c;
  }
  code[i++] = '\0';

  Program* program = program_create();
  char **s =  &code;
  program = parse(program, s);
  program_replace_label(program);
  program_print(program);
  VM* vm = vm_create();
  vm_fast_run(vm, program);
  return 0;
}

