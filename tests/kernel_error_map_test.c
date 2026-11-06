/* Owned NTSTATUS -> Win32 error conversions for the emitted status vocabulary. */
#include "../src/runtime/kernel_error_map.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* Success stays success. */
    assert(ff_kernel_error_from_status(0) == 0);

    /* Pinned documented pairs, including the stop's invalid-handle word. */
    assert(ff_kernel_error_from_status(0xc0000008u) == 6u);
    assert(ff_kernel_error_from_status(0xc0000022u) == 5u);
    assert(ff_kernel_error_from_status(0xc0000034u) == 2u);
    assert(ff_kernel_error_from_status(0xc0000035u) == 80u);
    assert(ff_kernel_error_from_status(0xc000003au) == 3u);
    assert(ff_kernel_error_from_status(0xc0000043u) == 32u);

    /* An unlisted high status is not mapped and is not success. */
    assert(ff_kernel_error_from_status(0xc0000203u) == FF_KERNEL_ERROR_UNMAPPED);

    /* Deferred vocabulary: two candidate winerror columns each, so these stay
     * unmapped rather than guessed. */
    assert(ff_kernel_error_from_status(0xc0000023u) == FF_KERNEL_ERROR_UNMAPPED);
    assert(ff_kernel_error_from_status(0xc000007fu) == FF_KERNEL_ERROR_UNMAPPED);
    assert(ff_kernel_error_from_status(0xc000009au) == FF_KERNEL_ERROR_UNMAPPED);
    assert(ff_kernel_error_from_status(0xc00000bau) == FF_KERNEL_ERROR_UNMAPPED);
    assert(ff_kernel_error_from_status(0xc0000103u) == FF_KERNEL_ERROR_UNMAPPED);

    puts("Owned status-to-error mapping and unmapped-status checks passed");
    return 0;
}
