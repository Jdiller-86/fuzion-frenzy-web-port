#ifndef FF_KERNEL_CAPTURE_H
#define FF_KERNEL_CAPTURE_H
#include "sha256.h"

#define FF_CAPTURE_ROWS 512u
#define FF_CAPTURE_MAX_START 512u
enum { FF_CAPTURE_OK=0, FF_CAPTURE_ROTATE=1, FF_CAPTURE_FULL=2, FF_CAPTURE_INVALID=3 };

typedef struct {
    ff_sha256 digest;
    uint32_t requested_start,captured_start,total,completed,prefix_completed,pending;
    uint8_t prefix_digest[32];
} ff_kernel_capture;

uint32_t ff_kernel_capture_init(ff_kernel_capture *,uint32_t start);
/* Canonical v1 stream is the NUL-terminated ASCII domain "ff-kernel-capture-v1",
 * followed by ordinal, original argc, original arguments, then status, each
 * encoded as a little-endian u32. Outputs and clocks are excluded. */
uint32_t ff_kernel_capture_begin(ff_kernel_capture *,uint32_t ordinal,const uint32_t *args,uint32_t argc);
void ff_kernel_capture_finish(ff_kernel_capture *,uint32_t status);
#endif
