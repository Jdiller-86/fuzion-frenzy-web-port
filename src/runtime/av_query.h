#ifndef FF_AV_QUERY_H
#define FF_AV_QUERY_H
#include <stdint.h>
#include "virtual_memory.h"

/* Reusable, unconnected synthetic numeric-6 query component.
 *
 * The observed call pushes (RegisterBase 0, numeric option 6, Param 0, Result).
 * This component models only that memory contract: it validates the fixed
 * argument shape and writes a complete synthetic DWORD supplied by an explicit,
 * nullable configuration pointer to the checked four-byte guest span. A null
 * configuration means no DWORD was supplied and the query is denied; there is
 * no default. It is not the kernel export, assigns no meaning to option 6,
 * mutates no guest CPU, stack or service state, and nothing in the runtime
 * links it. Only the native fixture is expected to call it.
 */
#define FF_AV_QUERY_NUMERIC_OPTION 6u

typedef struct {
    uint32_t register_base;
    uint32_t option;
    uint32_t param;
    uint32_t result_pointer;
} ff_av_query_arguments;

typedef struct {
    /* The caller supplies the complete synthetic DWORD. A null config pointer,
     * not a zero word, represents the absent case. */
    uint32_t word;
} ff_av_query_config;

typedef enum {
    FF_AV_QUERY_WROTE = 0,
    FF_AV_QUERY_DENIED = 1
} ff_av_query_result;

/* Write the configured word only when the arguments match the observed
 * numeric-6 shape and the real VM grants the whole four-byte span. The store is
 * bytewise little-endian and tolerates an unaligned guest pointer; a denied
 * query, including a null config, leaves every backing byte unchanged. The
 * result is an internal component outcome, not a guest NTSTATUS or EAX value. */
ff_av_query_result ff_av_query_write_result(ff_virtual_memory *vm,
                                            const ff_av_query_arguments *arguments,
                                            const ff_av_query_config *config);
#endif
