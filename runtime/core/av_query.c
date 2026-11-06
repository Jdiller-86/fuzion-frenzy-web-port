#include "av_query.h"

ff_av_query_result ff_av_query_write_result(ff_virtual_memory *vm,
                                            const ff_av_query_arguments *arguments,
                                            const ff_av_query_config *config)
{
    if (!vm || !arguments || !config) return FF_AV_QUERY_DENIED;
    if (arguments->register_base != 0u ||
        arguments->option != FF_AV_QUERY_NUMERIC_OPTION ||
        arguments->param != 0u) return FF_AV_QUERY_DENIED;
    if (!arguments->result_pointer) return FF_AV_QUERY_DENIED;
    /* ff_vm_check validates every page touched by the whole four-byte span, so
     * the write is bounded even when the pointer is unaligned or would wrap a
     * 32-bit addition. */
    if (ff_vm_check(vm, arguments->result_pointer, 4u, FF_VM_WRITE) != 0u)
        return FF_AV_QUERY_DENIED;
    uint8_t *out = vm->ram + arguments->result_pointer;
    out[0] = (uint8_t)(config->word & 0xffu);
    out[1] = (uint8_t)((config->word >> 8) & 0xffu);
    out[2] = (uint8_t)((config->word >> 16) & 0xffu);
    out[3] = (uint8_t)((config->word >> 24) & 0xffu);
    return FF_AV_QUERY_WROTE;
}
