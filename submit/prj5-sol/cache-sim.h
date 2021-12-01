#ifndef CACHE_SIM_
#define CACHE_SIM_

/** Opaque implementation */
typedef struct CacheSimImpl CacheSim;

/** Replacement strategy */
typedef enum {
  LRU_R,         /** Least Recently Used */
  MRU_R,         /** Most Recently Used */
  RANDOM_R       /** Random replacement */
} Replacement;

/** A primary memory address */
typedef unsigned long MemAddr;

/** Parameters which specify a cache.
 *  Must have nMemAddrBits > nLineBits >= 2.
 */
typedef struct {
  unsigned nSetBits;       /** Slides notation: s; # of sets is 2**this */
  unsigned nLinesPerSet;   /** Slides notation: E; # of cache lines/set */
  unsigned nLineBits;      /** Slides notation: b; # of bytes/line is 2**this */
  unsigned nMemAddrBits;   /** Slides notation: m; # of bits in primary mem
                               addr; total primary addr space is 2**this */
  Replacement replacement; /** replacement strategy */
} CacheParams;


/** Create and return a new cache-simulation structure for a
 *  cache for main memory with the specified cache parameters params.
 *  No guarantee that *params remains valid after this call.
 */
CacheSim *new_cache_sim(const CacheParams *params);

/** Free all resources used by cache-simulation structure *cache */
void free_cache_sim(CacheSim *cache);

typedef enum {
  CACHE_HIT,                  /** address found in cache */
  CACHE_MISS_WITHOUT_REPLACE, /** address not found, no line replaced */
  CACHE_MISS_WITH_REPLACE,    /** address not found in cache, line replaced */
  CACHE_N_STATUS              /** dummy value: # of status values possible */
} CacheStatus;

typedef struct {
  CacheStatus status;  /** status of requested address */
  MemAddr replaceAddr; /** address of replaced line if status is
                        *  CACHE_MISS_WITH_REPLACE */
} CacheResult;

/** Return result for requesting addr from cache */
CacheResult cache_sim_result(CacheSim *cache, MemAddr addr);

#endif //ifndef CACHE_SIM_
