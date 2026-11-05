#ifndef FF_DISPLAY_MODE_H
#define FF_DISPLAY_MODE_H
#include <stdint.h>

/* Bounded declaration model for the observed ordinal-3 AvSetDisplayMode call.
 * It accepts only the exact pinned tuple, never decodes the descriptor, never
 * writes display state, and returns success as a declaration acceptance. */
#define FF_DISPLAY_TUPLE_ADDR   0x3C10CFCu
#define FF_DISPLAY_TUPLE_STEP   0u
#define FF_DISPLAY_TUPLE_MODE   0x60030307u
#define FF_DISPLAY_TUPLE_FORMAT 0u
#define FF_DISPLAY_TUPLE_PITCH  0xA00u
#define FF_DISPLAY_TUPLE_FB     0x03920000u
#define FF_DISPLAY_CONTIG_BASE 0x80000000u
#define FF_DISPLAY_CONTIG_SIZE (64u * 1024u * 1024u)

enum {
    FF_DISPLAY_OK = 0,
    FF_DISPLAY_BAD_ADDR = 1,
    FF_DISPLAY_BAD_ARGS = 2,
    FF_DISPLAY_BAD_FB = 3
};

/* addr_mapped_aligned: non-zero only for a mapped, 4-byte-aligned guest pointer.
 * The descriptor contents are never interpreted. */
uint32_t ff_display_mode_accept(uint32_t addr_mapped_aligned, uint32_t addr, uint32_t step,
                                uint32_t mode, uint32_t format, uint32_t pitch, uint32_t fb);
/* Documented contiguous rule: a framebuffer below the window size is visible at
 * XBOX_CONTIG_BASE + fb. Returns 0 for a fb outside that rule. */
uint32_t ff_display_mode_window(uint32_t fb);

#endif
