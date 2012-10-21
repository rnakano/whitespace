#include <stdio.h>
#include <assert.h>

#define S " "
#define L "\n"
#define T "\t"

int parse_number(char** c);

void test_parse_number()
{
  char* s = S S L;
  assert(0 == parse_number(&s));
  s = S T L;
  assert(1 == parse_number(&s));
  s = T T S T T L;
  assert(-11 == parse_number(&s));
}

int main(int argc, char *argv[])
{
  test_parse_number();
  return 0;
}
