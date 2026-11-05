#include "av_null_result.h"

uint32_t ff_av_null_result_accept(uint32_t declared, uint32_t handle, uint32_t option,
                                  uint32_t param, uint32_t result)
{
    (void)handle;
    (void)option;
    (void)param;
    if (!declared) return FF_AV_NULL_UNDECLARED;
    /* The pinned early return is unconditional on the other arguments: only a
     * null Result is admitted. A nonzero Result reaches the option switch. */
    if (result != 0u) return FF_AV_NULL_RESULT;
    return FF_AV_NULL_ACCEPT;
}
