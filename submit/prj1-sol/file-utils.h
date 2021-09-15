#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

#include <stdio.h>

/** Return contents of file f in a dynamically allocated buffer buf;
 *  return # of bytes read, < 0 on error
 */
int readFile(FILE *f, unsigned char **buf);

/** Write bytes[nBytes] to file f; return # of bytes written,
 *  < 0 on error
 */
int writeFile(unsigned char bytes[], unsigned nBytes, FILE *f);



#endif //ifndef FILE_UTILS_H_
