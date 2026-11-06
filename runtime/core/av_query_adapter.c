#include "av_query_adapter.h"

ff_av_query_result ff_av_query_adapter(ff_virtual_memory *vm,
                                       const uint32_t arguments[4],
                                       const ff_av_query_config *config,
                                       ff_av_query_cpu *cpu)
{
    if (!vm || !arguments || !config || !cpu) return FF_AV_QUERY_DENIED;
    if (cpu->esp > 0xffffffffu - 20u) return FF_AV_QUERY_DENIED;
    const ff_av_query_arguments query = {
        arguments[0], arguments[1], arguments[2], arguments[3],
    };
    const ff_av_query_result result = ff_av_query_write_result(vm, &query, config);
    if (result == FF_AV_QUERY_WROTE) cpu->esp += 20u; /* return address and four stdcall arguments */
    return result;
}
