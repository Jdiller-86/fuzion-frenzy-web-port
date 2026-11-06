/* Owned unknown-AV-option scenarios; no original code or device. */
#include "../runtime/core/av_unknown_option.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    assert(ff_av_unknown_accept(FF_AV_UNKNOWN_HANDLE, FF_AV_UNKNOWN_OPTION, 0u, 0x1000u, 1u) ==
           FF_AV_UNKNOWN_ACCEPT);
    assert(ff_av_unknown_accept(0xFD000004u, FF_AV_UNKNOWN_OPTION, 0u, 0x1000u, 1u) ==
           FF_AV_UNKNOWN_ARGS);
    assert(ff_av_unknown_accept(FF_AV_UNKNOWN_HANDLE, 6u, 0u, 0x1000u, 1u) ==
           FF_AV_UNKNOWN_ARGS);
    assert(ff_av_unknown_accept(FF_AV_UNKNOWN_HANDLE, 0x0Eu, 0u, 0x1000u, 1u) ==
           FF_AV_UNKNOWN_ARGS);
    assert(ff_av_unknown_accept(FF_AV_UNKNOWN_HANDLE, FF_AV_UNKNOWN_OPTION, 1u, 0x1000u, 1u) ==
           FF_AV_UNKNOWN_ARGS);
    assert(ff_av_unknown_accept(FF_AV_UNKNOWN_HANDLE, FF_AV_UNKNOWN_OPTION, 0u, 0u, 0u) ==
           FF_AV_UNKNOWN_POINTER);
    assert(ff_av_unknown_accept(FF_AV_UNKNOWN_HANDLE, FF_AV_UNKNOWN_OPTION, 0u, 0x1002u, 0u) ==
           FF_AV_UNKNOWN_POINTER);

    printf("av_unknown_option: ok\n");
    return 0;
}
