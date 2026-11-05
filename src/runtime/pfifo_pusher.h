#ifndef FF_PFIFO_PUSHER_H
#define FF_PFIFO_PUSHER_H
#include <stdint.h>
#include "pfifo_status.h"

/* Bounded PFIFO pusher prototype.
 *
 * This is the fetch/decode step of the pinned reference pusher
 * (xemu hw/xbox/nv2a/pfifo.c:298-375), reduced to what the retained evidence
 * supports: one method packet, a jump or a ring jump per call. It never
 * executes a method, never follows a call/return, and never writes guest
 * memory. DMA_GET is advanced only over words the caller confirms the guest
 * CPU wrote, and only when every pointer is inside the supplied ring.
 *
 * Callers supply the word reader and the CPU-written-span predicate; this
 * module owns no memory and performs no I/O. A refusal returns a
 * classification and leaves GET unchanged. */
#define FF_PFIFO_PUSHER_MAX_BYTES (64u * 1024u * 1024u)
#define FF_PFIFO_PUSHER_MAX_WORDS 4096u

enum {
    FF_PFIFO_PUSH_OK = 0,
    FF_PFIFO_PUSH_EMPTY = 1,
    FF_PFIFO_PUSH_RING = 2,
    FF_PFIFO_PUSH_GET_OUTSIDE = 3,
    FF_PFIFO_PUSH_PUT_OUTSIDE = 4,
    FF_PFIFO_PUSH_SPAN_NOT_WRITTEN = 5,
    FF_PFIFO_PUSH_UNKNOWN_WORD = 6,
    FF_PFIFO_PUSH_CALL_RETURN = 7,
    FF_PFIFO_PUSH_BUDGET = 8,
    FF_PFIFO_PUSH_PROTECTION = 9,
    FF_PFIFO_PUSH_READ_FAILED = 10
};

/* Returns 0 on success, 1 when the word cannot be read. */
typedef uint32_t (*ff_pfifo_read_word_fn)(void *context, uint32_t address, uint32_t *value);

/* Returns non-zero only when every byte of [address, address + bytes) was
 * written by the guest CPU. */
typedef uint32_t (*ff_pfifo_span_written_fn)(void *context, uint32_t address, uint32_t bytes);

typedef struct {
    uint32_t base, end, get, put;
    uint32_t words_consumed;
    uint32_t consumer_updates;
    uint32_t last_header;
    uint32_t classification;
} ff_pfifo_pusher;

void ff_pfifo_pusher_init(ff_pfifo_pusher *pusher, uint32_t base, uint32_t end,
                          uint32_t get, uint32_t put);

/* Run one packet. On FF_PFIFO_PUSH_OK or FF_PFIFO_PUSH_EMPTY the pusher's GET
 * and accounting are updated; on any refusal GET is unchanged. When status is
 * non-null, a dispatched method clears CACHE1_STATUS.LOW_MARK first and sets it
 * again after the packet, matching the pinned pusher ordering. */
uint32_t ff_pfifo_pusher_step(ff_pfifo_pusher *pusher, ff_pfifo_read_word_fn read_word,
                              ff_pfifo_span_written_fn span_written, void *context,
                              ff_pfifo_status *status, uint32_t max_words);

#endif
