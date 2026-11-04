#ifndef FF_PFIFO_DMA_H
#define FF_PFIFO_DMA_H
#include <stdint.h>

/* Reached NV2A PFIFO DMA submission registers in the USER aperture.
 * Evidence: the pinned xemu model maps USER at NV2A offset 0x800000 and its
 * user.c maps writes/reads of NV_USER_DMA_PUT (0x40), NV_USER_DMA_GET (0x44)
 * and NV_USER_REF (0x48). Only channel 0 is classified here; another channel
 * or offset returns none and stays outside this bounded model. */
#define FF_PFIFO_USER_BASE 0xFD800000u
#define FF_PFIFO_USER_CHANNEL_BYTES 0x10000u

enum {
    FF_PFIFO_NONE = 0,
    FF_PFIFO_DMA_PUT = 1,
    FF_PFIFO_DMA_GET = 2,
    FF_PFIFO_DMA_REF = 3
};

uint32_t ff_pfifo_dma_classify(uint32_t address);
#endif
