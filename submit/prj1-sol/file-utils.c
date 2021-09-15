#include "file-utils.h"

#include <stdio.h>
#include <stdlib.h>

/** Return contents of file f in a dynamically allocated buffer buf;
 *  return # of bytes read, < 0 on error
 */
int
readFile(FILE *f, unsigned char **buf)
{
  if (fseek(f, 0L, SEEK_END) != 0) return -1; //not strictly portable
  long size = ftell(f);
  if (fseek(f, 0L, SEEK_SET) != 0) return -1;
  if ((*buf = malloc(size)) == NULL) return -1;
  if (fread(*buf, size, 1, f) != 1) return -1;
  return size;
}

/** Write bytes[nBytes] to file f; return # of bytes written,
 *  < 0 on error
 */
int
writeFile(unsigned char bytes[], unsigned nBytes, FILE *f)
{
  if (fwrite(bytes, nBytes, 1, f) != 1) return -1;
  return nBytes;
}
