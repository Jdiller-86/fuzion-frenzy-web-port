#include "av_unknown_option.h"

uint32_t ff_av_unknown_accept(uint32_t handle, uint32_t option, uint32_t param,
                              uint32_t result, uint32_t result_mapped_aligned)
{
    (void)result;
    if (handle != FF_AV_UNKNOWN_HANDLE || option != FF_AV_UNKNOWN_OPTION || param != 0u)
        return FF_AV_UNKNOWN_ARGS;
    if (!result_mapped_aligned) return FF_AV_UNKNOWN_POINTER;
    return FF_AV_UNKNOWN_ACCEPT;
}
