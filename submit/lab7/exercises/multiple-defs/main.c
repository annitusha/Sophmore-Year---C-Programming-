#include <stdio.h>
#include "def.h"
void f(void);

int x = 0x2234;
int y = 0x3924;

int
main()
{
  f();
  printf("x = 0x%x, y = 0x%x\n", x, y);
  return 0;
}
