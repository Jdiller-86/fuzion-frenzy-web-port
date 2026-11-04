#ifndef FF_AV_QUERY_H
#define FF_AV_QUERY_H
#include <stdint.h>
#include "virtual_memory.h"

/* Reusable, unconnected synthetic numeric-6 query component.
 *
 * The observed call pushes (RegisterBase 0, numeric option 6, Param 0, Result).
 * This component models only that memory contract: it validates the fixed
 * argument shape and writes a complete synthetic DWORD supplied by the caller
 * to the checked four-byte guest span. It is not the kernel export, assigns no
 * meaning to option 6, has no default word and mutates no guest CPU, stack or
 * service state. Nothing in the runtime links it; only the native fixture is
 * expected to call it.
 */
#define FF_AV_QUERY_NUMERIC_OPTION 6u

typedef struct {
    uint32_t register_base;
    uint32_t option;
    uint32_t param;
    uint32_t result_pointer;
    /* The caller must supply the complete synthetic DWORD. No default exists. */
    uint32_t word;
} ff_av_query_arguments;

typedef enum {
    FF_AV_QUERY_WROTE = 0,
    FF_AV_QUERY_DENIED = 1
} ff_av_query_result;

/* Write the explicit word only when the arguments match the observed numeric-6
 * shape and the real VM grants the whole four-byte span. A denied query leaves
 * every backing byte unchanged. The result is an internal component outcome,
 * not a guest NTSTATUS or EAX value. */
ff_av_query_result ff_av_query_write_result(ff_virtual_memory *vm,
                                            const ff_av_query_arguments *arguments);
#endif
