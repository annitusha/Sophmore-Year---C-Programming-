#include "int-set.h"
#include "int-set-strings.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/** Expects str to be of the form "{ I, I, ..., I, }", where the I's
 *  represent the integers in the IntSet. The last ',' is optional.
 *  Read intSet from char array str[] up to the terminating '}'.
 *  Permissive on whitespace.  If n is not NULL, it sets it to # of
 *  chars read from str[].  Returns scanned IntSet, NULL on error.
 */
void *
sscanIntSet(const char str[], int *n)
{
  int i = 0;
  int isErr = 0;
  void *set = NULL;
  do {
    while (isspace(str[i])) i++;
    if (str[i] != '{') { isErr = 1; break; }
    i++;
    set = newIntSet();
    do { //look for /\s*(\-?\d+\s*,)?/
      while (isspace(str[i])) i++;
      if (!isdigit(str[i]) && str[i] != '-')  break;
      char *p;
      long val = strtol(&str[i], &p, 10);
      i += p - &str[i];
      addIntSet(set, val);
      while (isspace(str[i])) i++;
      if (str[i] != ',') break; //don't set isErr, making last , optional
      i++;
    } while (1);
    isErr = str[i++] != '}';
  } while (0);
  if (isErr && set) { freeIntSet(set); set = NULL; }
  if (n) *n = i;
  return set;
}

static inline char *bufNull(char *buf, int n) {
  return buf == NULL ? NULL : buf + n;
}

static inline int sizeNull(char *buf, int n) {
  return buf == NULL ? 0 : n;
}

/** Print intSet into buf as string "{ I, I, ..., I, }" terminated by
 *  '\0' (where the I's represent the integers in intSet).  No more
 *  than size bytes are ever written to buf[] (including the
 *  terminating NUL byte '\0').  Returns the total number of bytes
 *  which would have been written (excluding the terminating NUL
 *  byte).  It follows that a return value of size or more means that
 *  the output was truncated and the call should be retried with a
 *  larger buf[] (at least the return value + 1 (for the terminating
 *  NUL)).
 */
int
snprintIntSet(void *intSet, char *buf, size_t size)
{
  int n = 0;
  n += snprintf(bufNull(buf, n), sizeNull(buf, size - n), "{ ");
  for (const void *iter = newIntSetIterator(intSet); iter != NULL;
       iter = stepIntSetIterator(iter)) {
    int val = intSetIteratorElement(iter);
    n += snprintf(bufNull(buf,  n), sizeNull(buf, size - n), "%d, ", val);
  }
  n += snprintf(bufNull(buf, n), sizeNull(buf, size - n), "}");
  return n;
}
