/* Owned null-result AV query scenarios; no original code or device. */
#include "../src/runtime/av_null_result.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* Declared, exact observed tuple with a null Result. */
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_HANDLE, FF_AV_NULL_OPTION, 0u, 0u) ==
           FF_AV_NULL_ACCEPT);

    /* Any other handle, option or param keeps the refusal. */
    assert(ff_av_null_result_accept(1u, 0xFD000000u, FF_AV_NULL_OPTION, 0u, 0u) ==
           FF_AV_NULL_ARGS);
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_HANDLE, 6u, 0u, 0u) == FF_AV_NULL_ARGS);
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_HANDLE, FF_AV_NULL_OPTION, 1u, 0u) ==
           FF_AV_NULL_ARGS);

    /* A nonzero Result reaches the query path and is a different behavior. */
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_HANDLE, FF_AV_NULL_OPTION, 0u, 0x1000u) ==
           FF_AV_NULL_RESULT);
    assert(ff_av_null_result_accept(1u, FF_AV_NULL_HANDLE, 6u, 0u, 0x1000u) == FF_AV_NULL_RESULT);

    /* Undeclared profiles refuse even the exact tuple. */
    assert(ff_av_null_result_accept(0u, FF_AV_NULL_HANDLE, FF_AV_NULL_OPTION, 0u, 0u) ==
           FF_AV_NULL_UNDECLARED);

    printf("av_null_result: ok\n");
    return 0;
}
