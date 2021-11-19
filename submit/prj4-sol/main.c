#include "fn-trace.h"

#include "errors.h"

#include <dlfcn.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/** Driver program for project.  Loads module which contains function
 *  to be traced.  Calls function and then recursively traces that function
 *  and all functions called by it.  Uses API defined by fn_trace module
 *  to perform tracing.
 *
 *  Dependencies:
 *
 *  Preprocessing: When compiling, has a dependency on include files
 *  for libcs220. Specifically, the course include directory must be
 *  included in the compiler include-path using the -I option which is
 *  usually defined using make's CPPFLAGS.
 *
 *  Link Time: has a depency on libdl, capstone and libcs220,
 *  hence -ldl, -lcapstone and -lcs220 must be specified in make's
 *  LDLIBS and the course lib directory in LDFLAGS.
 *
 *  Run-time: In order to load the libraries, the course lib directory
 *  must be included in the value of the LD_LIBRARY_PATH environmental
 *  variable.
 */

/** Declare pointer top-level function called in functions module. */
typedef int (*FnP)(void);

/** Return pointer to function with name fn in shared-object file with
 *  name module.
 */
static FnP
get_module_fn(const char *module, const char *fn, void **handleP)
{
  void *handle = dlopen(module, RTLD_NOW);
  if (!handle) {
    fatal("cannot load %s: %s\n", module, dlerror());
  }
  FnP f = dlsym(handle, fn);
  if (!f) {
    dlclose(handle);
    fatal("cannot find %s in %s: %s\n", fn, module, dlerror());
  }
  *handleP = handle;
  return f;
}

#if 0
/** Output result of calling function f module::fn() on out. */
static void
out_fn_call_result(FILE *out, const char *module, const char *fn, FnP f)
{
  fprintf(out, "%s:%s() = %d\n", module, fn, f());
}
#endif

/** For each function called directly or indirectly by f, print
 *  address of function (relative address if isRelative is true), the
 *  # of direct calls made by that function and the # of bytes in the
 *  code for the function.
 */
static void
out_fn_trace(FILE *out, FnP f, bool isRelative)
{
  const FnsData *fnsData = new_fns_data(f);
  char *firstAddress = NULL;
  for (const FnInfo *fnInfo = next_fn_info(fnsData, NULL); fnInfo != NULL;
       fnInfo = next_fn_info(fnsData, fnInfo)) {
    if (firstAddress == NULL) firstAddress = fnInfo->address;
    if (isRelative) {
      fprintf(out, "%8ld: ", (char *)(fnInfo->address) - firstAddress);
    }
    else {
      fprintf(out, "%p: ", fnInfo->address);
    }
    fprintf(out, "nInCalls: %7d; nOutCalls: %7d; length: %7d\n",
            fnInfo->nInCalls, fnInfo->nOutCalls, fnInfo->length);
  }
  free_fns_data((FnsData *)fnsData);
}

/** Usage: fn-trace [-r] MODULE FUNCTION: static trace of all calls
 *  made by function FUNCTION in shared-object MODULE.
 *
 *  First prints the result of calling FUNCTION with no arguments.
 *
 *  Then for each function called directly or indirectly by FUNCTION,
 *  print address of function (relative address if -r option
 *  specified), the # of direct calls made by that function and the #
 *  of bytes in the code for the function.
 */
int
main(int argc, const char *argv[]) {
  if ((argc != 3 && argc != 4) || (argc == 4 && strcmp(argv[1], "-r") != 0)) {
    fatal("usage: %s [-r] MODULE FUNCTION\n", argv[0]);
  }
  bool isRelative = (argc == 4);
  int nonOptionArgIndex = (isRelative) ? 2 : 1;
  const char *module = argv[nonOptionArgIndex];
  const char *fn = argv[nonOptionArgIndex + 1];

  void *handle;
  FnP f = get_module_fn(module, fn, &handle);

  FILE *out = stdout;
  //out_fn_call_result(out, module, fn, f);
  out_fn_trace(out, f, isRelative);

  dlclose(handle);

  return 0;
}
