#include "display_mode.h"

uint32_t ff_display_mode_accept(uint32_t register_base_declared, uint32_t addr, uint32_t step,
                                uint32_t mode, uint32_t format, uint32_t pitch, uint32_t fb)
{
    if (!register_base_declared) return FF_DISPLAY_BAD_ADDR;
    if (addr != FF_DISPLAY_TUPLE_ADDR || step != FF_DISPLAY_TUPLE_STEP ||
        mode != FF_DISPLAY_TUPLE_MODE || format != FF_DISPLAY_TUPLE_FORMAT ||
        pitch != FF_DISPLAY_TUPLE_PITCH)
        return FF_DISPLAY_BAD_ARGS;
    if (fb >= FF_DISPLAY_CONTIG_SIZE) return FF_DISPLAY_BAD_FB;
    return FF_DISPLAY_OK;
}

uint32_t ff_display_mode_window(uint32_t fb)
{
    if (!fb || fb >= FF_DISPLAY_CONTIG_SIZE) return 0u;
    return FF_DISPLAY_CONTIG_BASE + fb;
}
