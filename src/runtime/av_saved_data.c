#include "av_saved_data.h"

uint32_t ff_av_saved_data_prior_setter(const uint32_t *prior_ordinals, uint32_t count)
{
    if (count && !prior_ordinals) return 1u; /* cannot prove absence: fail closed */
    for (uint32_t i = 0; i < count; ++i)
        if (prior_ordinals[i] == 4u) return 1u;
    return 0u;
}

uint32_t ff_av_saved_data_value(uint32_t declared, uint32_t prior_setter, uint32_t *value)
{
    if (!value) return FF_AV_SAVED_OUT;
    if (!declared) return FF_AV_SAVED_UNDECLARED;
    if (prior_setter) return FF_AV_SAVED_SETTER;
    *value = 0u;
    return FF_AV_SAVED_OK;
}
