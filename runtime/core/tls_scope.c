#include "tls_scope.h"

uint32_t ff_tls_same_context(uint32_t entry_fs, uint32_t current_fs)
{
    return entry_fs && entry_fs == current_fs;
}

uint32_t ff_reg_change_observable(uint32_t at_call_boundary)
{
    return at_call_boundary ? 1u : 0u;
}
