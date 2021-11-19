#include "x86-64_lde.h"

#include "errors.h"
#include "memalloc.h"

#include "capstone/capstone.h"

#include <string.h>

/** Length decoder for x86_64 instructions. Uses capstone disassembler
 *  library.
 *
 *  Dependencies:
 *
 *  Preprocessing: When compiling, requires include files installed
 *  in course include directory.  Hence course include directory must
 *  be specified via -I option usually specified using make's CPPFLAGS.
 *
 *  Linking: Requires access to the Capstone library libcapstone
 *  installed in the system lib directories and the course library
 *  installed in the course library directory. Hence that directory
 *  must be added to the set of directories searched for libraries
 *  using the -L compiler option; the libraries must be specified
 *  using -l capstone and -l cs220.  Usually, these options are
 *  specified using make's LDFLAGS and LDLIBS variables respectively.
 *
 *  Runtime: The capstone library must be accessed at load-time.  Hence
 *  the LD_LIBRARY_PATH environmental variable must contain the course
 *  library directory when any program linked with this code is run.
 */

struct LdeImpl {
  csh capstone;
  cs_insn *insn;
};

/** Return a new x86-64 length decoder */
Lde *
new_lde(void)
{
  Lde *lde = mallocChk(sizeof(Lde));
  cs_err err = cs_open(CS_ARCH_X86, CS_MODE_64, &lde->capstone);
  if (err) {
    fatal("cannot create capstone disassembler: %u\n", err);
  }
  lde->insn = cs_malloc(lde->capstone);
  return lde;
}

/** Free previously created x86-64 length decoder */
void
free_lde(Lde *lde) {
  cs_free(lde->insn, 1);
  cs_close(&lde->capstone);
  free(lde);
}

/** Return length of x86-64 instruction pointed to by p.  Returns < 0
 *  on error.
 */
int
get_op_length(const Lde *lde, const unsigned char *p)
{
  enum { MAX_INSN_SIZE = 15 };
  size_t size = MAX_INSN_SIZE;
  const unsigned char *p1 = p;
  uint64_t addr = (uint64_t)p;
  int isOk = cs_disasm_iter(lde->capstone, &p1, &size, &addr, lde->insn);
  if (!isOk) {
    for (int i = 0; i < MAX_INSN_SIZE; i++) { //assume p[i] addr ok
      fprintf(stderr, "0x%02x ", p[i]);
    }
    fatal("\ncannot decode instruction at %p: %u\n",
          p, cs_errno(lde->capstone));
  }
  return p1 - p;
}
