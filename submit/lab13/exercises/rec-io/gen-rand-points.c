#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "point2.h"

static void __attribute__((noreturn))
usage(const char *progName)
{
  fprintf(stderr, "usage: %s N_POINTS OUT_FILE\n", progName);
  exit(1);
}

static int
randCoord(void)
{
  enum { MAX = 1000 }; //exclusive
  //statistically unsound; see
  //<https://stackoverflow.com/questions/4195958/how-do-i-scale-down-numbers-from-rand>
  return rand() % MAX;
}

int
main(int argc, const char *argv[])
{
  if (argc != 3) usage(argv[0]);
  int nPoints = atoi(argv[1]);
  if (nPoints <= 0) usage(argv[0]);
  const char *outName = argv[2];
  FILE *out = fopen(outName, "wb");
  if (!out) {
    fprintf(stderr, "cannot write %s: %s\n", outName, strerror(errno));
    exit(1);
  }
  double sum = 0.0;
  for (int i = 0; i < nPoints; i++) {
    Point2 point2 = { .x = randCoord(), .y = randCoord(), };
    if (fwrite(&point2, sizeof(Point2), 1, out) != 1) {
      fprintf(stderr, "cannot write Point2(%d, %d) to %s: %s\n",
              point2.x, point2.y, outName, strerror(errno));
      exit(1);
    }
    double mag = magnitudePoint2(&point2);
    sum += mag;
  }
  fprintf(stderr, "average magnitude = %g\n", sum/nPoints);
  if (fclose(out) != 0) {
    fprintf(stderr, "cannot close %s: %s\n", outName, strerror(errno));
    exit(1);
  }
  return 0;
}
