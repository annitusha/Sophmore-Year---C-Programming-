#include "file-utils.h"
#include "morse.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void
morseEncode(FILE *in, FILE *out)
{
  Byte *text;
  int nChars = readFile(in, &text);
  if (nChars < 0) {
    fprintf(stderr, "cannot read input file\n");
    exit(1);
  }
  //worst case is text is all '0's; each 0 is 5 dashes which will be
  //4*4 + 3 + 3 == 22 bits;
  int maxNBytes = (nChars + 1) * ((22 + CHAR_BIT - 1)/CHAR_BIT) + 1;
  Byte *bytes = calloc(1, maxNBytes);
  if (bytes == NULL) {
    fprintf(stderr, "cannot alloc bytes: %s\n", strerror(errno));
    exit(1);
  }
  int nBytes = textToMorse(text, nChars, bytes);
  free(text);
  if (nBytes < 0) {
    fprintf(stderr, "cannot encode text\n");
    exit(1);
  }
  if (writeFile(bytes, nBytes, out) != nBytes) {
    fprintf(stderr, "cannot write output\n");
    exit(1);
  }
  free(bytes);
}

static void
morseDecode(FILE *in, FILE *out)
{
  Byte *bytes;
  int nBytes = readFile(in, &bytes);
  if (nBytes < 0) {
    fprintf(stderr, "cannot read input file\n");
    exit(1);
  }
  //worst case is text is all 'E's; each E is 1 dash which will be
  //1 + 3 bits
  int maxNChars = nBytes / (CHAR_BIT/4) + 1;
  Byte *text = malloc(maxNChars);
  if (text == NULL) {
    fprintf(stderr, "cannot alloc text: %s\n", strerror(errno));
    exit(1);
  }
  int nChars = morseToText(bytes, nBytes, text);
  free(bytes);
  if (nChars < 0) {
    fprintf(stderr, "cannot decode bytes\n");
    exit(1);
  }
  if (writeFile(text, nChars, out) != nChars) {
    fprintf(stderr, "cannot write output\n");
    exit(1);
  }
  free(text);
}

int
main(int argc, const char *argv[])
{
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "usage: %s SRC_FILE [DEST_FILE]\n", argv[0]);
    exit(1);
  }
  const char *lastSlash = strrchr(argv[0], '/');
  const char *basename = (lastSlash == NULL) ? argv[0] : lastSlash + 1;
  const int isEncode = strcmp(basename, "morse-encode") == 0;
  const char *inMode = isEncode ? "r" : "rb";
  FILE *in = fopen(argv[1], inMode);
  if (!in) {
    fprintf(stderr, "cannot read %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  const char *outMode = isEncode ? "wb" : "w";
  FILE *out = (argc == 3) ? fopen(argv[2], outMode) : stdout;
  if (!out) {
    fprintf(stderr, "cannot write %s: %s\n", argv[2], strerror(errno));
    exit(1);
  }
  if (isEncode) {
    morseEncode(in, out);
  }
  else {
    morseDecode(in, out);
  }
  if (fclose(in) != 0) {
    fprintf(stderr, "cannot close %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  if (argc == 3 && fclose(out) != 0) {
    fprintf(stderr, "cannot close %s: %s\n", argv[2], strerror(errno));
    exit(1);
  }
  return 0;
}
