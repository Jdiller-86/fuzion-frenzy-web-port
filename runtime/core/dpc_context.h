#ifndef FF_DPC_CONTEXT_H
#define FF_DPC_CONTEXT_H
#include <stdint.h>
#include "kernel_timers.h"

#define FF_CPU_U32_FIELDS(X) X(eax) X(ecx) X(edx) X(esp) X(ebx) X(esi) X(edi) X(ebp) X(fs_base) X(seh_ebp) X(eflags) X(mxcsr)
#define FF_CPU_INT_FIELDS(X) X(df) X(fp_top) X(fp_cmp) X(force_return)
#define FF_CPU_U16_FIELDS(X) X(fp_control_word) X(fp_cc)
typedef struct {
#define FF_FIELD_WORD(name) uint32_t r_##name;
    FF_CPU_U32_FIELDS(FF_FIELD_WORD)
#undef FF_FIELD_WORD
#define FF_FIELD_INT(name) int r_##name;
    FF_CPU_INT_FIELDS(FF_FIELD_INT)
#undef FF_FIELD_INT
#define FF_FIELD_SHORT(name) uint16_t r_##name;
    FF_CPU_U16_FIELDS(FF_FIELD_SHORT)
#undef FF_FIELD_SHORT
    double fp_values[8];
    uint8_t xmm_bytes[8][16], mm_bytes[8][8];
} ff_guest_cpu;

#define FF_DPC_RETURN_SENTINEL 0xfffffffcu
typedef struct {
    ff_guest_cpu saved;
    uint32_t active, fs, exception_head, stack_base, stack_limit, irql, dpc_active;
    uint32_t stack_top, frame_sp;
} ff_dpc_frame;

/* A bounded dispatcher bridge, not general interrupt or exception dispatch.
 * Caller verifies the executable address before begin, then pops the admitted
 * DPC before execution. The dedicated stack must not overlap the thread/PCR.
 * End restores context even on bad cleanup; abort handles trapped callbacks. */
uint32_t ff_dpc_begin(ff_dpc_frame *, ff_guest_cpu *, uint8_t *ram, uint32_t bytes,
                      uint32_t stack_low, uint32_t stack_top, const ff_dpc_call *);
uint32_t ff_dpc_end(ff_dpc_frame *, ff_guest_cpu *, uint8_t *ram, uint32_t bytes);
uint32_t ff_dpc_abort(ff_dpc_frame *, ff_guest_cpu *, uint8_t *ram, uint32_t bytes);
#endif
