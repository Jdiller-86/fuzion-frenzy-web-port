#include "pfifo_status.h"
#include <string.h>

void ff_pfifo_status_init(ff_pfifo_status *status)
{
    if (status) memset(status, 0, sizeof *status);
}

static uint32_t *owned(ff_pfifo_status *status, uint32_t offset)
{
    if (!status) return 0;
    switch (offset) {
        case FF_PFIFO_CACHE1_STATUS_OFFSET: return &status->cache1_status;
        case FF_PFIFO_RUNOUT_STATUS_OFFSET: return &status->runout_status;
        case FF_PFIFO_CACHE1_DMA_PUSH_OFFSET: return &status->dma_push;
        default: return 0;
    }
}

uint32_t ff_pfifo_status_read(const ff_pfifo_status *status, uint32_t offset)
{
    if (!status) return UINT32_MAX;
    switch (offset) {
        case FF_PFIFO_CACHE1_STATUS_OFFSET: return status->cache1_status;
        case FF_PFIFO_RUNOUT_STATUS_OFFSET: return status->runout_status;
        case FF_PFIFO_CACHE1_DMA_PUSH_OFFSET: return status->dma_push;
        default: return UINT32_MAX;
    }
}

uint32_t ff_pfifo_status_write(ff_pfifo_status *status, uint32_t offset, uint32_t value)
{
    uint32_t *field = owned(status, offset);
    if (!field) return 1;
    *field = value;
    return 0;
}

uint32_t ff_pfifo_status_satisfied(const ff_pfifo_status *status)
{
    if (!status) return 0;
    return (status->cache1_status & FF_PFIFO_STATUS_POLLED_BIT) &&
           (status->runout_status & FF_PFIFO_STATUS_POLLED_BIT) &&
           !(status->dma_push & FF_PFIFO_STATUS_POLLED_BIT);
}

uint32_t ff_pfifo_status_consumer_update(ff_pfifo_status *status, uint32_t offset, uint32_t value)
{
    uint32_t *field = owned(status, offset);
    if (!field) return 1;
    *field = value;
    ++status->consumer_updates;
    return 0;
}
