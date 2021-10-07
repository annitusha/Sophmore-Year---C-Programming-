#include <stdio.h>
#include "int-set.h"
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
/** Abstract data type for set of int's.  Note that sets do not allow
 *  duplicates.
 */

typedef struct NodeStruct {
  int element;
  struct NodeStruct *succ;
} Node;
  
typedef struct {
  int nElements; //tracking numbers of elements in the set
  Node dummy;
} Header; 


static Node * linkNewNodeAfter(Node *p0, int element) {
  Node *p = malloc(sizeof(Node));
  if (!p) return NULL;
  p->element = element;
  p->succ = p0->succ;
  p0->succ = p;
  return p;
}



/** Return a new empty int-set.  Returns NULL on error with errno set.
 */
void *newIntSet() {
  //TODO
  return calloc(1, sizeof(Header));
}

/** Return # of elements in intSet */
int nElementsIntSet(void *intSet) {
  //TODO
  const Header *header = (Header *)intSet;
  return header->nElements;
}

/** Return non-zero iff intSet contains element. */
int isInIntSet(void *intSet, int element) {
  //TODO
  Header *header = (Header *)intSet;
  Node *p0;
  for(p0 = &header->dummy; p0->succ != NULL && p0->succ->element < element; p0 = p0->succ) {
    if(p0->succ->element == element) {
      return 1;
    } 
      return 0;
  }
}


/** Change intSet by adding element to it.  Returns # of elements
 *  in intSet after addition.  Returns < 0 on error with errno
 *  set.
 */
int addIntSet(void *intSet, int element) {
  //TODO
  Header *header = (Header *)intSet;
  Node *p0;
  for(p0 = &header->dummy; p0-> succ != NULL && p0->succ->element < element; p0 = p0->succ) {
    //    if(p0->succ->element == element)
    //      return header->nElements;
    //  assert(p0->succ->element == element);
  }


  //  assert(p0->succ == NULL || p0->succ->element > element);
  if(p0->succ != NULL && p0->succ->element == element)
    return header->nElements;
 
  if (!linkNewNodeAfter(p0, element)) {
    return -1;
      }
  return ++header->nElements;
}

/** Change intSet by adding all elements in array elements[nElements] to
 *  it.  Returns # of elements in intSet after addition.  Returns
 *  < 0 on error with errno set.
 */
int addMultipleIntSet(void *intSet, const int elements[], int nElements) {
  //TODO
  int ret = -1;
  for(int i = 0; i < nElements; i++) {
    if ((ret = addIntSet(intSet, elements[i])) < 0) break;
  }
  return ret;
}

/** Set intSetA to the union of intSetA and intSetB.  Return # of
 *  elements in the updated intSetA.  Returns < 0 on error.
 */
int unionIntSet(void *intSetA, void *intSetB) {
  //TODO
  Header *header = (Header *)intSetA;
  Node *pA0;
  Node *pB;
  pA0 = &header->dummy;
  while(pA0->succ != NULL && pB->succ != NULL) {
  //while(pA0->succ && pB->succ) {
    if(pA0->succ->element < pB->succ->element) {
      pA0 = pA0->succ;
    }
    else if(pA0->succ->element == pB->succ->element) {
      // pA0 = pA0->succ;
      pA0++;
      //pB = pB->succ;
      pB++;
    }
    else if (pA0->succ->element > pB->succ->element) {
      linkNewNodeAfter(pA0, pB->succ->element);
      pA0 = pA0->succ;
      pB++;
    }
  }    
  //linkNewNodeAfter(pA0, pB->succ->element);
  return header->nElements;
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
  Header *header = (Header *)intSet;
  Node *p1;
  for(Node *p = header->dummy.succ; p != NULL; p = p1) {
    p1 = p->succ;
    free(p);
  }
  free(header);
}

/** Return a new iterator for intSet.  Returns NULL if intSet
 *  is empty.
 */
const void *newIntSetIterator(const void *intSet) {
  //TODO
  const Header *header = (Header *)intSet;
  return header->dummy.succ;
}

/** Return current element for intSetIterator. */
int intSetIteratorElement(const void *intSetIterator) {
  //TODO
  const Node *p = (Node *)intSetIterator;
  return p->element;
}

/** Step intSetIterator and return stepped iterator.  Return
 *  NULL if no more iterations are possible.
 */
const void *stepIntSetIterator(const void *intSetIterator) {
  //TODO
  const Node *p = (Node *)intSetIterator;
  return p->succ;
}


//ifndef INT_SET_H_
