#include "cache-sim.h"
//#include "memalloc.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */



typedef struct Block Block;
struct Block{
  MemAddr address;
  long index;
  bool isValid;
};

struct CacheSimImpl {
  Block ** blocks;
  CacheParams param;
};


CacheSim *new_cache_sim(const CacheParams *params)
{
    CacheParams param;
  param.nSetBits = params->nSetBits;
  param.nLinesPerSet = params->nLinesPerSet;
  param.nLineBits = params->nLineBits;
  param.nMemAddrBits = params->nMemAddrBits;
  param.replacement = params->replacement;

  CacheSim * sim = (CacheSim *)malloc(sizeof(param) * sizeof(struct CacheSimImpl));
  int nSets = pow(2, params->nSetBits);
  sim->param = param;
  sim->blocks = (Block **)malloc(nSets * sizeof(struct Block *) + 1);

  for(int i = 0; i < nSets; i++){
    sim->blocks[i] = (Block *)malloc(params->nLinesPerSet * sizeof(struct Block));
    for (int j = 0; j < params->nLinesPerSet; j++){
      sim->blocks[i][j].address = 0x0;
      sim->blocks[i][j].index = 0;
      sim->blocks[i][j].isValid = false;
    }
  }
  return sim;
}

/** Free all resources used by cache-simulation structure *cache */
void
free_cache_sim(CacheSim *cache)
{
  for(int i = 0; i < (1 << cache->param.nSetBits); i++){
    free(cache->blocks[i]);
  }
  free(cache->blocks);
  free(cache);
}

/** Return result for requesting addr from cache */
CacheResult
cache_sim_result(CacheSim *cache, MemAddr addr)
{
  //  CacheResult result = { 0, 0 };
  //TODO
  //return result;
    CacheResult res = {CACHE_N_STATUS, 0};
  MemAddr newAddr = addr >> cache->param.nLineBits;
  newAddr <<= cache->param.nLineBits;

  int setIndex= (((int)(pow(2, cache->param.nSetBits) - 1) << cache->param.nLineBits) & addr);
  setIndex >>= cache->param.nLineBits;
  int nLines = cache->param.nLinesPerSet;

  for(int i = 0; i < nLines; i++){
    cache->blocks[setIndex][i].index++;
    if(cache->blocks[setIndex][i].address == newAddr){
      cache->blocks[setIndex][i].index = 0;
      res.status = CACHE_HIT;
      res.replaceAddr = 0x0;
      return res;
    }
    else if(!cache->blocks[setIndex][i].isValid){
      cache->blocks[setIndex][i].address = newAddr;
      cache->blocks[setIndex][i].isValid = true;
      cache->blocks[setIndex][i].index = 0;
      res.status = CACHE_MISS_WITHOUT_REPLACE;
      res.replaceAddr = 0x0;
      return res;
    }
  }

  MemAddr tempAddr;
  int replace = cache->param.replacement;
  if(replace == LRU_R){
    int index = 0;
    int greatest = 0;
    for(int i = 0; i < nLines; i++){
      if(greatest < cache->blocks[setIndex][i].index){
	greatest = cache->blocks[setIndex][i].index;
	index = i;
      }
    }
    tempAddr = cache->blocks[setIndex][index].address;
    cache->blocks[setIndex][index].address = newAddr;
  }
  else if(replace == MRU_R){
    int index = 0;
    int least = cache->blocks[setIndex][0].index;
    for(int i = 0; i < nLines; i++){
      if(least > cache->blocks[setIndex][i].index){
	least = cache->blocks[setIndex][i].index;
	index = i;
      }
    }
    tempAddr = cache->blocks[setIndex][index].address;
    cache->blocks[setIndex][index].address = newAddr;
  }
  else{
    int index = rand() % nLines;
    tempAddr = cache->blocks[setIndex][index].address;
    cache->blocks[setIndex][index].address = newAddr;
  }
  res.status = CACHE_MISS_WITH_REPLACE;
  res.replaceAddr = tempAddr;

  return res;
      
}
