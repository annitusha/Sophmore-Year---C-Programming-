#include "fn-trace.h"
#include "x86-64_lde.h"

#include "memalloc.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum { INIT_SIZE = 2 };

struct FnsDataImpl {
  int index;
  int nAlloc;
  Fninfo *elements[];
  
  //TODO
};

/** Return pointer to opaque data structure containing collection of
 *  FnInfo's for functions which are callable directly or indirectly
 *  from the function whose address is rootFn.
 */

void addElement(FnInfo *fInsert, FnsData *fnsArray) {
    if (fnsArray->index >= fnsArray->nAlloc) {
        reallocChk((void *)(fnsArray->elements), 2 * fnsArray->nAlloc*sizeof(fInsert));
        fnsArray->nAlloc *= 2;
    }
    fnsArray->elements[fnsArray->index + 1] = fInsert;
    fnsArray->index += 1;
}

const FnInfo *
findFuncByAddress(void *addr, FnsData *fnsData) {
    for (int i = 0; i < fnsData->index; i++)
        if(fnsData->elements[i]->address == addr)
            return fnsData->elements[i];
    //TODO
    return NULL;

}







void fn_trace(void *addr, FnsData fnsData) {
  FnInfo *fnInfo = findFuncByAddress(addr, fnsData);
    if(fnInfo == NULL) {
        fnInfo = malloc(sizeof(FnInfo));
        fnInfo->address = addr;
        fnInfo->nInCalls = 1;
        addElement(fnInfo, fnsData);
    }
    else
        fnInfo->nInCalls++;

    Lde * lde = new_lde();
    int len = get_op_length(lde, addr);
//    int len;
    int totalLength = len;
    while(len > 0) {
        char instr[len];
        memcpy(addr, instr, len);

        if(strcmp(instr, "E8") == 0) //found call
            fn_trace(addr, fnsData);
        if(strcmp(instr, "C3") == 0) break; //found return
        totalLength += len;
        addr += len;
        len = get_op_length(lde, addr);

    }
    free_lde(lde);
    fnInfo->length = totalLength;

}


const FnsData *
new_fns_data(void *rootFn)
{
    FnsData *fnsData;
    fnsData = malloc(sizeof(FnsData));
    fnsData->index = 0;
    reallocChk((void *)(fnsData->elements), 10*sizeof(FnInfo *));
    fnsData->nAlloc = 10;
    fn_trace(rootFn, fnsData);
//    qsort(fnsData->elements);
    return fnsData;
}




/** Free all resources occupied by fnsData. fnsData must have been
 *  returned by new_fns_data().  It is not ok to use to fnsData after
 *  this call.
 */
void free_fns_data(FnsData *fnsData)
{

  for (int i = 0; i <= fnsData->index; i++)
        free(fnsData->elements[i]);
    free(fnsData);
  

  //TODO
}

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
const FnInfo *
next_fn_info(const FnsData *fnsData, const FnInfo *lastFnInfo)
{
  //TODO
  for (int i = 0; i < fnsData->index; i++)
        if(fnsData->elements[i] == lastFnInfo)
            return fnsData->elements[i+1];
    //TODO
  return NULL;
}


/** recognized opcodes for calls and returns */
enum {
  CALL_OP = 0xE8,    //used to identify an external call which is traced

  //used to recognize the end of a function
  RET_FAR_OP = 0xCB,
  RET_FAR_WITH_POP_OP = 0xCA,
  RET_NEAR_OP = 0xC3,
  RET_NEAR_WITH_POP_OP = 0xC2
};

static inline bool is_call(unsigned op) { return op == CALL_OP; }
static inline bool is_ret(unsigned op) {
  return
    op == RET_NEAR_OP || op == RET_NEAR_WITH_POP_OP ||
    op == RET_FAR_OP || op == RET_FAR_WITH_POP_OP;
}

//TODO: add auxiliary functions
