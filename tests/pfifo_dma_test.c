/* Owned PFIFO DMA register classification; no original code or device model. */
#include "../src/runtime/pfifo_dma.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    assert(ff_pfifo_dma_classify(0xFD800040u) == FF_PFIFO_DMA_PUT);
    assert(ff_pfifo_dma_classify(0xFD800044u) == FF_PFIFO_DMA_GET);
    assert(ff_pfifo_dma_classify(0xFD800048u) == FF_PFIFO_DMA_REF);
    assert(ff_pfifo_dma_classify(0xFD80004Cu) == FF_PFIFO_NONE);
    assert(ff_pfifo_dma_classify(0xFD80003Cu) == FF_PFIFO_NONE);
    assert(ff_pfifo_dma_classify(0xFD800000u) == FF_PFIFO_NONE);
    assert(ff_pfifo_dma_classify(0xFD80FFFFu) == FF_PFIFO_NONE);
    assert(ff_pfifo_dma_classify(0xFD810040u) == FF_PFIFO_NONE); /* channel 1 */
    assert(ff_pfifo_dma_classify(0xFD7FFFFFu) == FF_PFIFO_NONE);
    assert(ff_pfifo_dma_classify(0x00000040u) == FF_PFIFO_NONE);
    printf("owned PFIFO DMA map: channel-0 put/get/ref classification and bounds passed\n");
    return 0;
}
