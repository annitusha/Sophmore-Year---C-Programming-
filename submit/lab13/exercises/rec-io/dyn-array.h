#ifndef DYN_ARRAY_H_
#define DYN_ARRAY_H_

#include <stddef.h>

//set up as an ADT
typedef struct DynArray DynArray;

/** return newly allocated dynamic array for elements of size elementSize */
DynArray *newDynArray(size_t elementSize);

/** free all resources used by dynArray */
void freeDynArray(DynArray *dynArray);

/** Return &dynArray[i].  Element i must exist. */
void *getElementDynArray(const DynArray *dynArray, int i);

/** Set dynArray[i] to *element.  Element i must exist.  Return
 * pointer to set element.
 */
void *setElementDynArray(DynArray *dynArray, int i, const void *element);

/** Add element to end of dynArray.  Return pointer to newly
 *  added element.
 */
void *addElementDynArray(DynArray *dynArray, const void *element);

/** Return # of elements stored in dynArray */
const size_t nElementsDynArray(const DynArray *dynArray);

/** Sort dynArray by provided comparison function */
void sortDynArray(DynArray *dynArray,
                  int (*cmp)(const void *element1, const void *element2));

#endif //ifndef DYN_ARRAY_H_
