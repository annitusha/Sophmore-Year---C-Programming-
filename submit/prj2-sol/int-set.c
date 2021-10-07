#include <stdio.h>
#include "int-set.h"

/** Abstract data type for set of int's.  Note that sets do not allow
 *  duplicates.
 */

/** Return a new empty int-set.  Returns NULL on error with errno set.
 */
void *newIntSet() {
  //TODO
  return NULL;
}

/** Return # of elements in intSet */
int nElementsIntSet(void *intSet) {
  //TODO
  return 0;
}

/** Return non-zero iff intSet contains element. */
int isInIntSet(void *intSet, int element) {
  //TODO
  return 0;
}

/** Change intSet by adding element to it.  Returns # of elements
 *  in intSet after addition.  Returns < 0 on error with errno
 *  set.
 */
int addIntSet(void *intSet, int element) {
  //TODO
  return 0;
}

/** Change intSet by adding all elements in array elements[nElements] to
 *  it.  Returns # of elements in intSet after addition.  Returns
 *  < 0 on error with errno set.
 */
int addMultipleIntSet(void *intSet, const int elements[], int nElements) {
  //TODO
  return 0;
}

/** Set intSetA to the union of intSetA and intSetB.  Return # of
 *  elements in the updated intSetA.  Returns < 0 on error.
 */
int unionIntSet(void *intSetA, void *intSetB) {
  //TODO
  return 0;
}

/** Set intSetA to the intersection of intSetA and intSetB.  Return #
 *  of elements in the updated intSetA.  Returns < 0 on error.
 */
int intersectionIntSet(void *intSetA, void *intSetB) {
  //TODO
  return 0;
}

/** Free all resources used by previously created intSet. */
void freeIntSet(void *intSet) {
  //TODO
  return;
}

/** Return a new iterator for intSet.  Returns NULL if intSet
 *  is empty.
 */
const void *newIntSetIterator(const void *intSet) {
  //TODO
  return NULL;
}

/** Return current element for intSetIterator. */
int intSetIteratorElement(const void *intSetIterator) {
  //TODO
  return 0;
}

/** Step intSetIterator and return stepped iterator.  Return
 *  NULL if no more iterations are possible.
 */
const void *stepIntSetIterator(const void *intSetIterator) {
  //TODO
  return NULL;
}


//ifndef INT_SET_H_
