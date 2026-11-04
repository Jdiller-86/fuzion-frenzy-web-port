#ifndef FF_MEMORY_STATISTICS_H
#define FF_MEMORY_STATISTICS_H
#include "virtual_memory.h"

/* Statistics of the explicit modeled guest, not host/Wasm allocation size or
 * a claim about an original console's kernel/firmware reservations. */
#define FF_MEMORY_STATISTICS_WORDS 9u
enum {FF_MEMORY_POOL=1,FF_MEMORY_STACK,FF_MEMORY_IMAGE,FF_MEMORY_CACHE};
typedef struct {uint32_t physical_pages,baseline_pool_pages;} ff_memory_profile;
typedef struct {
    ff_virtual_memory *memory;
    ff_memory_profile profile;
    uint8_t fixed_role[FF_VM_MAX_PAGES];
} ff_memory_statistics;

uint32_t ff_memory_statistics_init(ff_memory_statistics *,ff_virtual_memory *,const ff_memory_profile *);
/* Classify fixed mapped ownership; reclassification does not allocate pages.
 * Private reservations/commitment are counted directly from VM ownership. */
uint32_t ff_memory_statistics_classify(ff_memory_statistics *,uint32_t base,uint32_t bytes,uint32_t role);
uint32_t ff_memory_statistics_snapshot(const ff_memory_statistics *,uint32_t words[FF_MEMORY_STATISTICS_WORDS]);
uint32_t ff_memory_statistics_query(ff_memory_statistics *,uint32_t output);
#endif
