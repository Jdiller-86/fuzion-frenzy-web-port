#include "mm_free_contiguous.h"

uint32_t ff_mm_free_accept(uint32_t declared, uint32_t address, uint32_t mapped_aligned)
{
    if (!declared) return FF_MM_FREE_UNDECLARED;
    if (!address || !mapped_aligned) return FF_MM_FREE_ADDRESS;
    return FF_MM_FREE_ACCEPT;
}
