#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dyn-array.h"

struct DynArray {
  size_t elementSize;  /** size of a base element */
  size_t index;        /** index of next free element */
  size_t size;         /** max number of elements for current elements[] */
  char *elements;      /** storage for elements */
};

/** return newly allocated dynamic array for elements of size elementSize */
DynArray *
newDynArray(size_t elementSize)
{
  DynArray *dynArray = calloc(1, sizeof(DynArray));
  if (!dynArray) {
    fprintf(stderr, "cannot alloc DynArray: %s\n", strerror(errno));
    exit(1);
  }
  dynArray->elementSize = elementSize;
  return dynArray;
}

/** free all resources used by dynArray */
void
freeDynArray(DynArray *dynArray)
{
  free(dynArray->elements);
  free(dynArray);
}


/** Return &dynArray[i].  Element i must exist.  */
void *
getElementDynArray(const DynArray *dynArray, int i)
{
  assert(i < dynArray->index);
  return dynArray->elements + i*dynArray->elementSize;
}

/** Set dynArray[i] to *element.  Element i must exist.  Return
 * pointer to set element.
 */
void *
setElementDynArray(DynArray *dynArray, int i, const void *element)
{
  assert(i < dynArray->index);
  memcpy(dynArray->elements + i*dynArray->elementSize,
         element, dynArray->elementSize);
  return dynArray->elements + i*dynArray->elementSize;
}


/** Add element to end of dynArray.  Return pointer to newly
 *  added element.
 */
void *
addElementDynArray(DynArray *dynArray, const void *element)
{
  enum { INIT_SIZE = 4 };
  if (dynArray->index >= dynArray->size) {
    const size_t size = (dynArray->size > 0) ? dynArray->size*2 : INIT_SIZE;
    dynArray->elements =
      realloc(dynArray->elements, size*dynArray->elementSize);
    if (!dynArray->elements) {
      fprintf(stderr, "dynArray realloc() failed: %s\n", strerror(errno));
      exit(1);
    }
    dynArray->size = size;
  }
  assert(dynArray->index < dynArray->size);
  dynArray->index++;
  return setElementDynArray(dynArray, dynArray->index - 1, element);
}

/** Return # of elements stored in dynArray */
const size_t
nElementsDynArray(const DynArray *dynArray)
{
  return dynArray->index;
}

/** Sort dynArray by provided comparison function */
void
sortDynArray(DynArray *dynArray,
             int (*cmp)(const void *element1, const void *element2))
{
  qsort(dynArray->elements, dynArray->index, dynArray->elementSize, cmp);
}
