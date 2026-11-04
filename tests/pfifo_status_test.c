/* Owned PFIFO status seam: guest stores, predicate, and the explicit consumer
 * entry point. No firmware, original code or engine is used. */
#include "../src/runtime/pfifo_status.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    ff_pfifo_status status;
    ff_pfifo_status_init(&status);
    assert(status.cache1_status == 0 && status.runout_status == 0 && status.dma_push == 0);
    assert(status.consumer_updates == 0);
    assert(ff_pfifo_status_satisfied(&status) == 0);

    /* Only the three reached offsets are owned. */
    assert(ff_pfifo_status_read(&status, FF_PFIFO_CACHE1_STATUS_OFFSET) == 0);
    assert(ff_pfifo_status_read(&status, 0x1210u) == UINT32_MAX);
    assert(ff_pfifo_status_write(&status, 0x1210u, 1u) == 1);
    assert(ff_pfifo_status_consumer_update(&status, 0x1210u, 1u) == 1);

    /* Guest stores are preserved verbatim, including DMA_PUSH. */
    assert(ff_pfifo_status_write(&status, FF_PFIFO_CACHE1_DMA_PUSH_OFFSET, 0x10u) == 0);
    assert(ff_pfifo_status_read(&status, FF_PFIFO_CACHE1_DMA_PUSH_OFFSET) == 0x10u);
    assert(ff_pfifo_status_satisfied(&status) == 0);
    assert(ff_pfifo_status_write(&status, FF_PFIFO_CACHE1_DMA_PUSH_OFFSET, 0u) == 0);

    /* The predicate needs all three conditions. */
    assert(ff_pfifo_status_consumer_update(&status, FF_PFIFO_CACHE1_STATUS_OFFSET, 0x10u) == 0);
    assert(status.consumer_updates == 1);
    assert(ff_pfifo_status_satisfied(&status) == 0);
    assert(ff_pfifo_status_consumer_update(&status, FF_PFIFO_RUNOUT_STATUS_OFFSET, 0x10u) == 0);
    assert(ff_pfifo_status_satisfied(&status) != 0);

    /* Setting the polled bit on DMA_PUSH breaks it again, as the guest expects. */
    assert(ff_pfifo_status_consumer_update(&status, FF_PFIFO_CACHE1_DMA_PUSH_OFFSET, 0x10u) == 0);
    assert(ff_pfifo_status_satisfied(&status) == 0);
    assert(ff_pfifo_status_consumer_update(&status, FF_PFIFO_CACHE1_DMA_PUSH_OFFSET, 0u) == 0);
    assert(ff_pfifo_status_satisfied(&status) != 0);

    /* Unrelated bits do not satisfy the predicate. */
    ff_pfifo_status_init(&status);
    assert(ff_pfifo_status_consumer_update(&status, FF_PFIFO_CACHE1_STATUS_OFFSET, 0x20u) == 0);
    assert(ff_pfifo_status_consumer_update(&status, FF_PFIFO_RUNOUT_STATUS_OFFSET, 0x08u) == 0);
    assert(ff_pfifo_status_satisfied(&status) == 0);

    /* Distinct registers stay distinct. */
    ff_pfifo_status_init(&status);
    assert(ff_pfifo_status_write(&status, FF_PFIFO_CACHE1_STATUS_OFFSET, 0x11u) == 0);
    assert(ff_pfifo_status_read(&status, FF_PFIFO_RUNOUT_STATUS_OFFSET) == 0);
    assert(ff_pfifo_status_read(&status, FF_PFIFO_CACHE1_DMA_PUSH_OFFSET) == 0);

    /* Null handling stays fail-closed rather than crashing. */
    assert(ff_pfifo_status_read(NULL, FF_PFIFO_CACHE1_STATUS_OFFSET) == UINT32_MAX);
    assert(ff_pfifo_status_write(NULL, FF_PFIFO_CACHE1_STATUS_OFFSET, 1u) == 1);
    assert(ff_pfifo_status_consumer_update(NULL, FF_PFIFO_CACHE1_STATUS_OFFSET, 1u) == 1);
    assert(ff_pfifo_status_satisfied(NULL) == 0);
    ff_pfifo_status_init(NULL);

    puts("owned PFIFO status seam: guest stores, distinct registers, predicate and consumer entry point passed");
    return 0;
}
