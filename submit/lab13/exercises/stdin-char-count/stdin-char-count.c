#include <stdio.h>

int
main(int argc, const char *argv[])
{
  printf("EOF = %d\n", EOF);
  char c;
  int count = 0;
  while ((c = getchar()) != EOF) count++;
  printf("%d\n", count);
  return 0;
}
