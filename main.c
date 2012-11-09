#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "program.h"
#include "parse.h"
#include "vm.h"

int main(int argc, char *argv[])
{
  struct option long_options[] = {
    {"code-print", no_argument,          NULL, 'c'},
    {"vm-switchcase", no_argument,       NULL, 's'},
    {"vm-goto",  no_argument,            NULL, 'g'},
    {"program-optimize",  no_argument,   NULL, 'o'},
    {0, 0, 0, 0}
  };

  int opt, option_index;
  int code_print = 0;
  int use_switchcase = 1, use_goto = 0;
  int use_program_optimize = 0;
  while((opt = getopt_long(argc, argv, "csgo", long_options, &option_index)) != -1)
  {
    switch(opt){
    case 'c':
      code_print = 1;
      break;
    case 's':
      use_switchcase = 1;
      use_goto = 0;
      break;
    case 'g':
      use_switchcase = 0;
      use_goto = 1;
      break;
    case 'o':
      use_program_optimize = 1;
      break;
    case '?':
      printf("Unknown or required argument option -%c\n", optopt);
      printf("Usage: COMMAND [--code-print] [--vm-switchcase | --vm-goto] [--program-optimize]\n");
      return 1;
    }
  }

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
  if(use_program_optimize) {
    program_optimize(program);
  }
  program_replace_label(program);
  if(code_print) {
    program_print(program);
    return 0;
  }

  VM* vm = vm_create();  

  if(use_switchcase) {
    vm_run(vm, program);
  } else if(use_goto) {
    vm_fast_run(vm, program);
  }

  program_delete(program);
  vm_delete(vm);

  return 0;
}

