#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dyn-array.h"
#include "point2.h"

static void
readPoints(const char *inName, DynArray *points)
{
  FILE *in = fopen(inName, "rb");
  if (!in) {
    fprintf(stderr, "cannot read %s: %s\n", inName, strerror(errno));
    exit(1);
  }
  Point2 point2;
  while (fread(&point2, sizeof(Point2), 1, in) == 1) {
    addElementDynArray(points, &point2);
  }
  if (fclose(in) != 0) {
    fprintf(stderr, "cannot close %s: %s\n", inName, strerror(errno));
    exit(1);
  }
}

static int
comparePoint2(const void *p1, const void *p2)
{
  return magnitudePoint2(p1) - magnitudePoint2(p2);
}

static double
averagePoints(const DynArray *points)
{
  const int n = nElementsDynArray(points);
  assert(n > 0);
  double sum = 0;
  for (int i = 0; i < n; i++) {
    const Point2 *p = getElementDynArray(points, i);
    double mag = magnitudePoint2(p);
    sum += mag;
  }
  return sum/n;
}

static void
statPoints(DynArray *points, FILE *out, FILE *out)
{
  sortDynArray(points, comparePoint2);
  const int n = nElementsDynArray(points);
  if (n > 0) {
    double min = magnitudePoint2(getElementDynArray(points, 0));
    double max = magnitudePoint2(getElementDynArray(points, n - 1));
    double average = averagePoints(points);
    double median = magnitudePoint2(getElementDynArray(points, n/2));
    fprintf(out, "min = %g\naverage = %g\nmedian = %g\nmax = %g\n",
            min, average, median, max);
  }
}

/* static void statPoints(DynArray *points, FILE *output)
{
  sortDynArray(points, comparePoint2);
  const int n = nElementsDynArray(points);
  if (n > 0) {
    double min = magnitudePoint2(getElementDynArray(points, 0));
    double max = magnitudePoint2(getElementDynArray(points, n - 1));
    double average = averagePoints(points);
    double median = magnitudePoint2(getElementDynArray(points, n/2));
    fputc(min, FILE *output);
    fputc(max, FILE *output);
    fputc(average, FILE *output);
    fputc(median, FILE *output);
  }
}
*/


int
main(int argc, const char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s BINARY_POINTS_FILE\n", argv[0]);
    exit(1);
  }
  DynArray *points = newDynArray(sizeof(Point2));
  readPoints(argv[1], points);
  statPoints(points, stdout);
  statPoints(points, stdoutput);
  freeDynArray(points);
  return 0;
}
