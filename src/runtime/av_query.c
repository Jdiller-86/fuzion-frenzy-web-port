#include "av_query.h"
#include <string.h>

ff_av_query_result ff_av_query_write_result(ff_virtual_memory *vm,
                                            const ff_av_query_arguments *arguments)
{
    if (!vm || !arguments) return FF_AV_QUERY_DENIED;
    if (arguments->register_base != 0u ||
        arguments->option != FF_AV_QUERY_NUMERIC_OPTION ||
        arguments->param != 0u) return FF_AV_QUERY_DENIED;
    if (!arguments->result_pointer || (arguments->result_pointer & 3u))
        return FF_AV_QUERY_DENIED;
    /* ff_vm_check validates every page touched by the whole four-byte span, so
     * a pointer that crosses into an uncommitted or read-only page is denied
     * before any byte is written. */
    if (ff_vm_check(vm, arguments->result_pointer, 4u, FF_VM_WRITE) != 0u)
        return FF_AV_QUERY_DENIED;
    memcpy(vm->ram + arguments->result_pointer, &arguments->word, 4u);
    return FF_AV_QUERY_WROTE;
}
