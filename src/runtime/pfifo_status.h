#ifndef FF_PFIFO_STATUS_H
#define FF_PFIFO_STATUS_H
#include <stdint.h>

/* Bounded PFIFO status seam for the reached wait loop's exit predicates.
 *
 * The reached code leaves its wait only when NV_PFIFO_CACHE1_STATUS bit 0x10 is
 * set, NV_PFIFO_RUNOUT_STATUS bit 0x10 is set and NV_PFIFO_CACHE1_DMA_PUSH bit
 * 0x10 is clear (measured order: cache status blocks first, DMA push is polled,
 * runout status is never reached).
 *
 * No producer exists: the retained NV2A reference models no FIFO consumer, and
 * nothing in this project calls ff_pfifo_status_consumer_update yet. This header
 * therefore only provides the typed seam and the predicate evaluation, with the
 * registers defaulting to zero. Guest stores are preserved so a translated write
 * to DMA_PUSH is not lost. */
#define FF_PFIFO_CACHE1_STATUS_OFFSET 0x1214u
#define FF_PFIFO_RUNOUT_STATUS_OFFSET 0x0400u
#define FF_PFIFO_CACHE1_DMA_PUSH_OFFSET 0x1220u
/* The single bit the reached loop tests on all three registers. Its identity was
 * resolved from the full retained reference
 * (.tools/emulation/xemu-source/hw/xbox/nv2a/nv2a_regs.h:137-145): bit 4 is
 * LOW_MARK on CACHE1_STATUS and on RUNOUT_STATUS, and STATE (busy) on
 * CACHE1_DMA_PUSH, whose ACCESS is bit 0 and suspended STATUS is bit 12.
 * Naming the bits here does not model a producer. */
#define FF_PFIFO_STATUS_POLLED_BIT 0x10u
#define FF_PFIFO_CACHE1_STATUS_LOW_MARK 0x10u
#define FF_PFIFO_RUNOUT_STATUS_LOW_MARK 0x10u
#define FF_PFIFO_CACHE1_DMA_PUSH_STATE 0x10u
#define FF_PFIFO_CACHE1_DMA_PUSH_ACCESS 0x01u
#define FF_PFIFO_CACHE1_DMA_PUSH_SUSPENDED 0x1000u

typedef struct {
    uint32_t cache1_status, runout_status, dma_push;
    uint32_t consumer_updates;
} ff_pfifo_status;

void ff_pfifo_status_init(ff_pfifo_status *);
/* Register reads: returns 0 or the stored value; UINT32_MAX for an offset this
 * seam does not own. */
uint32_t ff_pfifo_status_read(const ff_pfifo_status *, uint32_t offset);
/* Guest stores are preserved verbatim. Returns 0 for an owned offset, 1
 * otherwise. This does not model engine behaviour. */
uint32_t ff_pfifo_status_write(ff_pfifo_status *, uint32_t offset, uint32_t value);
/* The reached loop's exit predicate, evaluated from the current values. */
uint32_t ff_pfifo_status_satisfied(const ff_pfifo_status *);
/* The only way consumer-side state can change. Nothing in this project calls it;
 * whoever owns the FIFO consumer contract must supply the rules that decide when
 * to call it. */
uint32_t ff_pfifo_status_consumer_update(ff_pfifo_status *, uint32_t offset, uint32_t value);
/* Reference reset adapter, default-off and initialization-only.
 *
 * The pinned xemu reset zeroes the PFIFO register block and then sets
 * NV_PFIFO_CACHE1_STATUS.LOW_MARK because the method cache is empty
 * (hw/xbox/nv2a/nv2a.c:307-317), and pfifo_read returns
 * NV_PFIFO_RUNOUT_STATUS.LOW_MARK for every read (pfifo.c:51-52). The pusher
 * clears LOW_MARK only before a dispatch (pfifo.c:340) and sets it again after
 * processing (pfifo.c:242-243), so an empty, unsubmitted cache keeps the reset
 * value.
 *
 * This is not a consumer update: it leaves DMA_PUSH and consumer_updates
 * untouched, adds no capability, and is idempotent. Call it once before the
 * guest runs, only when the explicit reference-reset experiment is selected. It
 * must never be called from a poll or a read. */
void ff_pfifo_status_reference_reset(ff_pfifo_status *);
#endif
