/* Owned contiguous-free scenarios; no original code or device. */
#include "../runtime/core/mm_free_contiguous.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* A declared, mapped, aligned address is accepted; the observed address is
     * diagnostics only, not part of the predicate. */
    assert(ff_mm_free_accept(1u, 0x037F2000u, 1u) == FF_MM_FREE_ACCEPT);
    assert(ff_mm_free_accept(1u, 0x00100000u, 1u) == FF_MM_FREE_ACCEPT);

    /* Zero, unmapped and undeclared calls are refused. */
    assert(ff_mm_free_accept(1u, 0u, 1u) == FF_MM_FREE_ADDRESS);
    assert(ff_mm_free_accept(1u, 0x037F2002u, 0u) == FF_MM_FREE_ADDRESS);
    assert(ff_mm_free_accept(1u, 0x04000000u, 0u) == FF_MM_FREE_ADDRESS);
    assert(ff_mm_free_accept(0u, 0x037F2000u, 1u) == FF_MM_FREE_UNDECLARED);

    printf("mm_free_contiguous: ok\n");
    return 0;
}
