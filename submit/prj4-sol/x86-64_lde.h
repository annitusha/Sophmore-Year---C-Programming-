#ifndef X86_64_LDE_H_
#define X86_64_LDE_H_

//.1
/** Length decoder for x86-64 instructions. Requires linking with
 *  libcapstone.  See implementation file for details.
 */

typedef struct LdeImpl Lde;

/** Return a new x86-64 length decoder */
Lde *new_lde(void);

/** Free previously created x86-64 length decoder */
void free_lde(Lde *lde);

/** Return length of x86-64 instruction pointed to by p.  Returns < 0
 *  on error.
 */
int get_op_length(const Lde *ldeP, const unsigned char *p);
//.2

#endif //ifndef X86_64_LDE_H_
