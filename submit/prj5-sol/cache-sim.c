#include "cache-sim.h"
#include "memalloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */

int power(int x, int y){
  int result = 1;
  for (int i = 0; i < y; i++){
    result *= x;
  }
  return result;
}

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

  CacheSim * sim = malloc(sizeof(param) * sizeof(struct CacheSimImpl));
  int nSets = power(2, params->nSetBits);
  sim->param = param;
  sim->blocks = malloc(nSets * sizeof(struct Block *) + 1);

  for(int i = 0; i < nSets; i++){
    sim->blocks[i] = malloc(params->nLinesPerSet * sizeof(struct Block));
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
  CacheStatus status;
  MemAddr newAddr = addr >> cache->param.nLineBits;
  newAddr <<= cache->param.nLineBits;

  int set = (((power(2, cache->param.nSetBits) - 1) << cache->param.nLineBits) & addr) >> cache->param.nLineBits;
  int nLines = cache->param.nLinesPerSet;

  for(int i = 0; i < nLines; i++){
    cache->blocks[set][i].index++;
    if(cache->blocks[set][i].address == newAddr){
      cache->blocks[set][i].index = 0;
      status = 0;
      return (CacheResult) { .status = status, .replaceAddr = 0x0};
    }
    else if(!cache->blocks[set][i].isValid){
      cache->blocks[set][i].address = newAddr;
      cache->blocks[set][i].isValid = true;
      cache->blocks[set][i].index = 0;
      status = 1;
      return (CacheResult) { .status = status, .replaceAddr = 0x0};
    }
  }

  MemAddr tempAddress;
  int replace = cache->param.replacement;
  if(replace == LRU_R){
    int index = 0;
    int greatest = 0;
    for(int i = 0; i < nLines; i++){
      if(greatest < cache->blocks[set][i].index){
	greatest = cache->blocks[set][i].index;
	index = i;
      }
    }
    tempAddress = cache->blocks[set][index].address;
    cache->blocks[set][index].address = newAddr;
  }
  else if(replace == MRU_R){
    int index = 0;
    int least = cache->blocks[set][0].index;
    for(int i = 0; i < nLines; i++){
      if(least > cache->blocks[set][i].index){
	least = cache->blocks[set][i].index;
	index = i;
      }
    }
    tempAddress = cache->blocks[set][index].address;
    cache->blocks[set][index].address = newAddr;
  }
  else{
    int index = rand() % nLines;
    tempAddress = cache->blocks[set][index].address;
    cache->blocks[set][index].address = newAddr;
  }
  status = 2;
  return (CacheResult) { .status = status, .replaceAddr = tempAddress};
}
