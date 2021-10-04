#ifndef INT_SET_STRINGS_H_
#define INT_SET_STRINGS_H_

#include <stddef.h>

/** Expects str to be of the form "{ I, I, ..., I, }", where the I's
 *  represent the integers in the IntSet.  The last ',' is
 *  optional. Read intSet from char array str[] up to the terminating
 *  '}'.  Permissive on whitespace.  If n is not NULL, it sets it to #
 *  of chars read from str[].  Returns scanned IntSet, NULL on error.
 */
void *sscanIntSet(const char str[], int *n);

/** Print intSet into buf as string "{ I, I, ..., I, }" terminated by
 *  '\0' (where the I's represent the integers in intSet).  No more
 *  than size bytes are ever written to buf[] (including the
 *  terminating NUL byte '\0').  Returns the total number of bytes
 *  which would have been written (excluding the terminating NUL
 *  byte).  It follows that a return value of size or more means that
 *  the output was truncated and the call should be retried with a
 *  larger buf[].
 */
int snprintIntSet(void *intSet, char *buf, size_t size);

#endif //#ifndef INT_SET_STRINGS_H_
