#include "av_null_result.h"

uint32_t ff_av_null_result_accept(uint32_t declared, uint32_t handle, uint32_t option,
                                  uint32_t param, uint32_t result)
{
    if (!declared) return FF_AV_NULL_UNDECLARED;
    /* A nonzero Result is never the observed shape: the pinned early return is
     * gated on a null pointer, so this model must not absorb the query path. */
    if (result != 0u) return FF_AV_NULL_RESULT;
    if (handle != FF_AV_NULL_HANDLE || option != FF_AV_NULL_OPTION || param != 0u)
        return FF_AV_NULL_ARGS;
    return FF_AV_NULL_ACCEPT;
}
