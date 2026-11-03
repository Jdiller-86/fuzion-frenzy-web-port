#include "pfifo_dma.h"

uint32_t ff_pfifo_dma_classify(uint32_t address)
{
    if (address < FF_PFIFO_USER_BASE ||
        address - FF_PFIFO_USER_BASE >= FF_PFIFO_USER_CHANNEL_BYTES)
        return FF_PFIFO_NONE;
    switch (address - FF_PFIFO_USER_BASE) {
    case 0x40u: return FF_PFIFO_DMA_PUT;
    case 0x44u: return FF_PFIFO_DMA_GET;
    case 0x48u: return FF_PFIFO_DMA_REF;
    default: return FF_PFIFO_NONE;
    }
}
