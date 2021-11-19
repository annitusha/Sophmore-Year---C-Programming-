#ifndef FN_TRACE_H_
#define FN_TRACE_H_

//.1
/** Information associated with a function. */
typedef struct {
  void *address;        /** start address of function */
  unsigned length;      /** # of bytes taken by function code. */
  unsigned nInCalls;    /** # of direct calls to this function */
  unsigned nOutCalls;   /** # of direct function calls in function body */
} FnInfo;

/** FnsData is an opaque struct FnsDataImpl which can be implemented as
 *  desired by an implementation. This struct will hold a collection
 *  of FnInfo's.
 */
typedef struct FnsDataImpl FnsData;

/** Return pointer to opaque data structure containing collection of
 *  FnInfo's for functions which are callable directly or indirectly
 *  from the function whose address is rootFn.
 */
const FnsData *new_fns_data(void *rootFn);

/** Free all resources occupied by fnsData. fnsData must have been
 *  returned by new_fns_data().  It is not ok to use to fnsData after
 *  this call.
 */
void free_fns_data(FnsData *fnsData);

/** Iterate over all FnInfo's in fnsData.  Make initial call with NULL
 *  lastFnInfo.  Keep calling with return value as lastFnInfo, until
 *  next_fn_info() returns NULL.  Values must be returned sorted in
 *  increasing order by fnInfoP->address.
 *
 *  Sample iteration:
 *
 *  for (FnInfo *fnInfoP = next_fn_info(fnsData, NULL); fnInfoP != NULL;
 *       fnInfoP = next_fn_info(fnsData, fnInfoP)) {
 *    //body of iteration
 *  }
 *
 */
const FnInfo *next_fn_info(const FnsData *fnData, const FnInfo *lastFnInfo);
//.2

#endif //#ifndef FN_TRACE_H_
