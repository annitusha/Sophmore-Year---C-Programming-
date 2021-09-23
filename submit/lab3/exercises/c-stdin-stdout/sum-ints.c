#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Sum pairs of ints read from the first argument; if first arg is
 *  "-", then read from stdin.  Write line containing each sum to file
 *  specified by second argument.  If second argument is not
 *  specified, then write sum to stdout.
 */

static void
sumInts(FILE *in, FILE *out)
{
  do {
    int i1, i2;
    if (fscanf(in, "%d %d", &i1, &i2) != 2) break;
    fprintf(out, "%d\n", i1 + i2);
  } while (1);
}


int
main(int argc, const char *argv[])
{
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "usage: %s IN_FILE|- [OUT_FILE]\n", argv[0]);
    exit(1);
  }
  const char *inName = argv[1];
  FILE *in = strcmp(inName, "-") == 0 ? stdin : fopen(inName, "r");
  if (!in) {
    fprintf(stderr, "cannot read %s: %s\n", inName, strerror(errno));
    exit(1);
  }

  FILE *out = argc < 3 ? stdout : fopen(argv[2], "w");
  if (!out) {
    fprintf(stderr, "cannot write %s: %s\n", argv[2], strerror(errno));
    exit(1);
  }

  sumInts(in, out);

  if (fclose(in) != 0 || fclose(out) != 0) {
    fprintf(stderr, "cannot close files: %s\n", strerror(errno));
  }
}
