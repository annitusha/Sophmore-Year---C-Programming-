#include "cache-sim.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
usage(const char *program, const char *msg)
{
  fprintf(stderr, "%susage: %s [-r lru|mru|rand] [-s seed] [-v] s-E-b-m\n"
          "where s-E-b-m specified cache parameters:\n"
          "  s: # of bits in address used to specify set\n"
          "  E: # of cache lines per set\n"
          "  b: # of bits in address used to specify offset in cache line\n"
          "  m: total # of bits used to address primary memory\n"
          "  must have all non-negative and 2 <= b and b + s < m\n",
          msg, program);
    exit(1);
}

typedef struct {
  const char *name;
  Replacement replacement;
} ReplacementName;

static ReplacementName REPLACEMENTS[] = {
  { "lru", LRU_R },
  { "mru", MRU_R },
  { "rand", RANDOM_R },
};

/** Translate from name to Replacement enum.  Return < 0 on error */
static int
get_replacement(const char *name) {
  for (int i = 0; i < sizeof(REPLACEMENTS)/sizeof(REPLACEMENTS[0]); i++) {
    if (strcmp(name, REPLACEMENTS[i].name) == 0) {
      return REPLACEMENTS[i].replacement;
    }
  }
  return -1;
}

/** Somewhat non-elegant allocation here to force new_cache_sim() to
 *  make copies of *params.  Sets *nMemAddrBitsP which is used for
 *  verbose formatting in do_cache_sim().  Returns NULL on error.
 */
static CacheSim *
make_cache_sim(const char *paramsSpec, Replacement replacement,
               unsigned *nMemAddrBitsP)
{
  CacheParams params;
  params.replacement = replacement;
  unsigned *fieldsP[] = {
    &params.nSetBits, &params.nLinesPerSet,
    &params.nLineBits, &params.nMemAddrBits,
  };
  int i = 0;
  const char *p;
  for (p = paramsSpec; *p != '\0' && i < 4; i++, p += (i < 4)) {
    char *q;
    long v = strtol(p, &q, 10);
    p = q;
    if (v < 0 || ((i < 3) ? (*p != '-') : (*p != '\0'))) return NULL;
    *fieldsP[i] = v;
  }
  *nMemAddrBitsP = params.nMemAddrBits;
  return (*p == '\0') && (i == 4) && (params.nLineBits >= 2) &&
         (params.nLineBits + params.nSetBits < params.nMemAddrBits)
         ? new_cache_sim(&params)
         : NULL;
}

static void
out_cache_stats(unsigned long stats[], unsigned long nTotal, FILE *out)
{
  for (int i = 0; i < CACHE_N_STATUS; i++) {
    switch (i) {
    case CACHE_HIT:
      fprintf(out, "hits: ");
      break;
    case CACHE_MISS_WITHOUT_REPLACE:
      fprintf(out, "misses without replace: ");
      break;
    case CACHE_MISS_WITH_REPLACE:
      fprintf(out, "misses with replace: ");
      break;
    }
    fprintf(out, "%lu/%lu (%.2f%%) hits\n", stats[i], nTotal,
          (nTotal == 0) ? 0 : stats[i] * 100.0/nTotal);
  }
}

//must be in sync with CACHE_STATUS enum
static const char *STATUS_STRS[] = {
  "hit", "miss-without-replace", "miss-with-replace"
};

static void
do_cache_sim(CacheSim *cache, bool isVerbose, unsigned nMemAddrBits,
             FILE *in, FILE *out)
{
  unsigned long stats[] = { 0UL, 0UL, 0UL };
  unsigned addrWidth = (nMemAddrBits + 3)/4;
  while (1) {
    MemAddr addr;
    if (fscanf(in, "%lx", &addr) != 1) break;
    CacheResult result = cache_sim_result(cache, addr);
    stats[result.status]++;
    if (isVerbose) {
      fprintf(out, "%0*lx: %s", addrWidth, addr, STATUS_STRS[result.status]);
      if (result.status == CACHE_MISS_WITH_REPLACE) {
        fprintf(out, " %0*lx", addrWidth, result.replaceAddr);
      }
      fprintf(out, "\n");
    }
  } // while (1)
  unsigned long nTotal = 0UL;
  for (int i = 0; i < CACHE_N_STATUS; i++) {
    nTotal += stats[i];
  }
  out_cache_stats(stats, nTotal, out);
}

int
main(int argc, const char *argv[])
{
  const char *program = argv[0];
  if (argc <= 1) usage(program, "");
  bool isVerbose = false;
  int replacement = LRU_R;
  int seed = 0;
  int i;
  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strcmp(argv[i], "-v") == 0) {
      isVerbose = true;
    }
    else if (strcmp(argv[i], "-r") == 0) {
      if (i >= argc - 1) {
        usage(program, "-r requires lru|mru|rand additional argument\n");
      }
      replacement = get_replacement(argv[++i]);
      if (replacement < 0) {
        usage(program, "replacement must be lru|mru|rand\n");
      }
    }
    else if (strcmp(argv[i], "-s") == 0) {
      if (i >= argc - 1) {
        usage(program, "-s requires seed additional argument\n");
      }
      char *p;
      seed = strtol(argv[++i], &p, 10);
      if (seed < 0 || *p != '\0') {
        usage(program, "seed must be a non-negative integer\n");
      }
    }
    else {
      usage(program, "invalid option\n");
    }
  }
  if (i != argc - 1) {
    usage(program, "cache spec s-E-b-m required\n");
  }

  srand(seed);
  const char *paramsSpec = argv[i];

  unsigned nMemAddrBits;
  CacheSim *cacheSim = make_cache_sim(paramsSpec, replacement, &nMemAddrBits);
  if (!cacheSim) usage(program, "invalid cache params\n");
  do_cache_sim(cacheSim, isVerbose, nMemAddrBits, stdin, stdout);
  free_cache_sim(cacheSim);
  return 0;

}
