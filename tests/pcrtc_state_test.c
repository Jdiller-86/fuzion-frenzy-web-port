/* Owned PCRTC interrupt-state contract. No firmware, original code or timer. */
#include "../runtime/core/pcrtc_state.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    ff_pcrtc_state pcrtc;
    ff_pcrtc_init(&pcrtc);
    assert(pcrtc.pending == 0 && pcrtc.enabled == 0 && pcrtc.vblanks == 0);
    assert(ff_pcrtc_read(&pcrtc, FF_PCRTC_INTR_0) == 0);
    assert(ff_pcrtc_read(&pcrtc, FF_PCRTC_INTR_EN_0) == 0);
    assert(ff_pcrtc_pmc_irq(&pcrtc) == 0);

    /* A pending event alone does not reach PMC until the guest enables it. */
    ff_pcrtc_vblank(&pcrtc);
    assert(pcrtc.vblanks == 1 && ff_pcrtc_read(&pcrtc, FF_PCRTC_INTR_0) == FF_PCRTC_INTR_VBLANK);
    assert(ff_pcrtc_pmc_irq(&pcrtc) == 0);

    /* Enabling aggregates the pending bit into PMC bit 24. */
    assert(ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_EN_0, FF_PCRTC_INTR_VBLANK) == 0);
    assert(ff_pcrtc_read(&pcrtc, FF_PCRTC_INTR_EN_0) == FF_PCRTC_INTR_VBLANK);
    assert(ff_pcrtc_pmc_irq(&pcrtc) == FF_PCRTC_PMC_IRQ);

    /* A second event keeps one pending bit but counts the supplied events. */
    ff_pcrtc_vblank(&pcrtc);
    assert(pcrtc.vblanks == 2 && pcrtc.pending == FF_PCRTC_INTR_VBLANK);

    /* The guest acknowledges by writing the bit back; aggregation drops. */
    assert(ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_0, FF_PCRTC_INTR_VBLANK) == 0);
    assert(pcrtc.pending == 0 && ff_pcrtc_pmc_irq(&pcrtc) == 0);

    /* An unrelated write clears nothing, and disabling stops aggregation even
     * while a bit stays pending. */
    ff_pcrtc_vblank(&pcrtc);
    assert(ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_0, 0x2u) == 0);
    assert(pcrtc.pending == FF_PCRTC_INTR_VBLANK);
    assert(ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_EN_0, 0) == 0);
    assert(ff_pcrtc_pmc_irq(&pcrtc) == 0);

    /* Unknown registers are not modelled and are rejected, not invented. */
    assert(ff_pcrtc_read(&pcrtc, 0x104u) == 0);
    assert(ff_pcrtc_write(&pcrtc, 0x104u, 1u) == 1);
    assert(ff_pcrtc_read(NULL, FF_PCRTC_INTR_0) == 0);
    assert(ff_pcrtc_write(NULL, FF_PCRTC_INTR_0, 1u) == 1);
    assert(ff_pcrtc_pmc_irq(NULL) == 0);
    ff_pcrtc_vblank(NULL);

    /* Re-initialising clears both masks. */
    ff_pcrtc_init(&pcrtc);
    assert(pcrtc.pending == 0 && pcrtc.enabled == 0 && ff_pcrtc_pmc_irq(&pcrtc) == 0);
    puts("owned PCRTC state: pending, enable gating, acknowledge, PMC aggregation and rejection passed");
    return 0;
}
