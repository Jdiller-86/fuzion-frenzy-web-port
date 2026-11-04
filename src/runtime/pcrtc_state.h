#ifndef FF_PCRTC_STATE_H
#define FF_PCRTC_STATE_H
#include <stdint.h>

/* Bounded PCRTC interrupt state for the reached display-interrupt path.
 *
 * The contract is taken from the retained local NV2A reference
 * (.tools/emulation/xboxrecomp-source/src/nv2a): NV_PCRTC_INTR_0 reads the
 * pending mask and clears the bits written to it, NV_PCRTC_INTR_EN_0 stores the
 * enable mask, and PMC bit 24 (NV_PMC_INTR_0_PCRTC) is set exactly while
 * pending & enabled. Pending bit 0 is the vertical-blank event.
 *
 * This component only holds that state. It has no timer and raises nothing by
 * itself: the caller must supply a vertical-blank event explicitly, so a
 * profile with no timing source keeps every read at zero. */
#define FF_PCRTC_INTR_VBLANK 1u
#define FF_PCRTC_INTR_0 0x100u
#define FF_PCRTC_INTR_EN_0 0x140u
#define FF_PCRTC_PMC_IRQ 0x01000000u

typedef struct {
    uint32_t pending, enabled, vblanks;
} ff_pcrtc_state;

void ff_pcrtc_init(ff_pcrtc_state *);
/* Register reads: INTR_0 returns pending, INTR_EN_0 returns the enable mask,
 * anything else returns zero. */
uint32_t ff_pcrtc_read(const ff_pcrtc_state *, uint32_t offset);
/* Register writes: INTR_0 clears the written bits, INTR_EN_0 stores the enable
 * mask. Returns 0 when the offset is one of those registers, 1 otherwise. */
uint32_t ff_pcrtc_write(ff_pcrtc_state *, uint32_t offset, uint32_t value);
/* The display timing input. Nothing in the recompiled runtime calls this yet;
 * the selected profile decides whether any event is supplied. */
void ff_pcrtc_vblank(ff_pcrtc_state *);
/* PMC aggregation for NV_PMC_INTR_0. */
uint32_t ff_pcrtc_pmc_irq(const ff_pcrtc_state *);
#endif
