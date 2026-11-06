#ifndef FF_AV_QUERY_ADAPTER_H
#define FF_AV_QUERY_ADAPTER_H
#include "av_query.h"

/* One guest-call adapter around the accepted numeric-6 memory component.
 *
 * The observed call is (RegisterBase 0, option 6, Param 0, Result) with a
 * callee-cleaned four-argument frame. On a successful write the adapter removes
 * the return address and four arguments (esp += 20) and leaves eax untouched;
 * on denial it changes no output byte, esp or eax. It supplies no default and
 * does not decide that the service is original. */
typedef struct {
    uint32_t esp;
    uint32_t eax;
} ff_av_query_cpu;

ff_av_query_result ff_av_query_adapter(ff_virtual_memory *vm,
                                       const uint32_t arguments[4],
                                       const ff_av_query_config *config,
                                       ff_av_query_cpu *cpu);
#endif
