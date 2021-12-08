#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Output number of chars in file specified by single command-line
 *  argument.
 */
int
main(int argc, const char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s FILE_NAME\n", argv[0]);
    exit(1);
  }
  const char *fName = argv[1];
  FILE *in = fopen(fName, "r");
  if (!in) {
    fprintf(stderr, "cannot read %s: %s\n", fName, strerror(errno));
    exit(1);
  }
  int c;
  int count = 0;
  while ((c = fgetc(in)) != EOF){
  count++;
  }
  printf("%d\n", count);
  free(in);
  return 0;
}
