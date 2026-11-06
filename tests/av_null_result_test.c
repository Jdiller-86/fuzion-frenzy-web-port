/* Owned null-result AV query scenarios; no original code or device. */
#include "../runtime/core/av_null_result.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* Both observed pairs are accepted under the pinned unconditional
     * null-Result early return. */
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_HANDLE, FF_AV_NULL_OPTION, 0u, 0u) ==
           FF_AV_NULL_ACCEPT);
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_DEVICE, FF_AV_NULL_OPTION, 1u, 0u) ==
           FF_AV_NULL_ACCEPT);

    /* Mechanism test, not a coverage claim: the pinned early return does not
     * read the other arguments, so any null-Result tuple is admitted. */
    assert(ff_av_null_result_accept(1u, 0x12345678u, 2u, 7u, 0u) == FF_AV_NULL_ACCEPT);

    /* A nonzero Result reaches the unmodeled option switch and is refused. */
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_DEVICE, FF_AV_NULL_OPTION, 1u, 0x1000u) ==
           FF_AV_NULL_RESULT);
    assert(ff_av_null_result_accept(1u, 0u, 6u, 0u, 0x1000u) == FF_AV_NULL_RESULT);

    /* Undeclared profiles refuse even a null Result. */
    assert(ff_av_null_result_accept(0u, FF_AV_NULL_HANDLE, FF_AV_NULL_OPTION, 0u, 0u) ==
           FF_AV_NULL_UNDECLARED);

    printf("av_null_result: ok\n");
    return 0;
}
