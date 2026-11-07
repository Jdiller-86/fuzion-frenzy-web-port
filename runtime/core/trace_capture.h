#ifndef FF_TRACE_CAPTURE_H
#define FF_TRACE_CAPTURE_H

#include "sha256.h"

#define FF_TRACE_CAPTURE_ROWS 8192u
#define FF_TRACE_CAPTURE_MAX_START 8192u
#define FF_TRACE_CAPTURE_ROUTINES 512u

enum {
    FF_TRACE_CAPTURE_OK = 0,
    FF_TRACE_CAPTURE_ROTATE = 1,
    FF_TRACE_CAPTURE_FULL = 2,
    FF_TRACE_CAPTURE_INVALID = 3
};

typedef struct ff_trace_capture {
    ff_sha256 digest;
    uint32_t requested_start, captured_start, total, entries, prefix_entries;
    uint32_t routine_count, prefix_routines;
    uint32_t routines[FF_TRACE_CAPTURE_ROUTINES];
    uint8_t prefix_digest[32];
} ff_trace_capture;

uint32_t ff_trace_capture_init(ff_trace_capture *capture, uint32_t start);
/* Hashes accepted rows as six little-endian u32 words in kind/address/stack/
 * EAX/FS/exception-head order after the NUL-terminated v1 domain. */
uint32_t ff_trace_capture_add(ff_trace_capture *capture, uint32_t kind,
                              uint32_t address, uint32_t stack, uint32_t eax,
                              uint32_t fs, uint32_t exception_head);

#endif
