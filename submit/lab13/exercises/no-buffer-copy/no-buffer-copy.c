#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
doCopy(const char *inName, FILE *in, const char *outName, FILE *out)
{
  int c;
  while ((c = fgetc(in)) != EOF) {
    if (fputc(c, out) == EOF) {
      fprintf(stderr, "error writing to %s: %s\n", outName, strerror(errno));
      return; //return rather than exit to ensure files closed
    }
  }
}

/** Copy file specified by first argument to file specified by
 *  second argument.  An optional additional argument is used
 *  to specify whether the I/O should be buffered.
 */
int
main(int argc, const char *argv[])
{
  if (argc != 3 && argc != 4) {
    fprintf(stderr, "usage: %s SRC_NAME DEST_NAME [IS_BUFFERED]\n", argv[0]);
    exit(1);
  }
  const bool isBuffered = argc == 4 && atoi(argv[3]) != 0;
  const int bufMode = (isBuffered) ? _IOFBF : _IONBF;
  const char *srcName = argv[1];
  const char *destName = argv[2];
  FILE *in = fopen(srcName, "r");
  if (!in) {
    fprintf(stderr, "cannot read %s: %s\n", srcName, strerror(errno));
    exit(1);
  }
  if (setvbuf(in, NULL, bufMode, 0) != 0) {
    fprintf(stderr, "cannot set %d buffering for  %s: %s\n", isBuffered,
            srcName, strerror(errno));
    exit(1);
  }
  FILE *out = fopen(destName, "w");
  if (!out) {
    fprintf(stderr, "cannot write %s: %s\n", destName, strerror(errno));
    exit(1);
  }
  if (setvbuf(out, NULL, bufMode, 0) != 0) {
    fprintf(stderr, "cannot set %d buffering for  %s: %s\n", isBuffered,
            destName, strerror(errno));
    exit(1);
  }
  doCopy(srcName, in, destName, out);
  if (fclose(in) != 0) {
    fprintf(stderr, "cannot close %s: %s\n", srcName, strerror(errno));
    exit(1);
  }
  if (fclose(out) != 0) {
    fprintf(stderr, "cannot close %s: %s\n", destName, strerror(errno));
    exit(1);
  }
  return 0;
}
