#include "pcrtc_state.h"
#include <string.h>

void ff_pcrtc_init(ff_pcrtc_state *pcrtc)
{
    if (pcrtc) memset(pcrtc, 0, sizeof *pcrtc);
}

uint32_t ff_pcrtc_read(const ff_pcrtc_state *pcrtc, uint32_t offset)
{
    if (!pcrtc) return 0;
    switch (offset) {
        case FF_PCRTC_INTR_0: return pcrtc->pending;
        case FF_PCRTC_INTR_EN_0: return pcrtc->enabled;
        default: return 0;
    }
}

uint32_t ff_pcrtc_write(ff_pcrtc_state *pcrtc, uint32_t offset, uint32_t value)
{
    if (!pcrtc) return 1;
    switch (offset) {
        case FF_PCRTC_INTR_0: pcrtc->pending &= ~value; return 0;
        case FF_PCRTC_INTR_EN_0: pcrtc->enabled = value; return 0;
        default: return 1;
    }
}

void ff_pcrtc_vblank(ff_pcrtc_state *pcrtc)
{
    if (!pcrtc) return;
    pcrtc->pending |= FF_PCRTC_INTR_VBLANK;
    ++pcrtc->vblanks;
}

uint32_t ff_pcrtc_pmc_irq(const ff_pcrtc_state *pcrtc)
{
    return pcrtc && (pcrtc->pending & pcrtc->enabled) ? FF_PCRTC_PMC_IRQ : 0;
}
