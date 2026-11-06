#ifndef FF_MM_FREE_CONTIGUOUS_H
#define FF_MM_FREE_CONTIGUOUS_H
#include <stdint.h>

/* Bounded model for the observed ordinal-171 MmFreeContiguousMemory call.
 * The pinned bridge frees the address from its contiguous arena (with a heap
 * fallback) and sets EAX to 0; neither path changes guest memory or the guest
 * mapping. This harness owns no host allocator and never reuses freed blocks,
 * so the model reproduces only that declaration-level effect. It is not
 * allocator fidelity and makes no later-reuse claim. Any mapped, aligned guest
 * address is the honest contract for this service shape. */
enum {
    FF_MM_FREE_ACCEPT = 0,
    FF_MM_FREE_UNDECLARED = 1,
    FF_MM_FREE_ADDRESS = 2
};

uint32_t ff_mm_free_accept(uint32_t declared, uint32_t address, uint32_t mapped_aligned);

#endif
