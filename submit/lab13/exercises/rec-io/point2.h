#ifndef POINT2_H_
#define POINT2_H

#include <math.h>

typedef struct {
  int x, y;
} Point2;

static double inline magnitudePoint2(const Point2 *p) {
  return sqrt(1.0*p->x*p->x + 1.0*p->y*p->y);
}

#endif //ifndef POINT2_H_
