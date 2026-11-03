#ifndef FF_PFIFO_CHANNEL_H
#define FF_PFIFO_CHANNEL_H
#include <stdint.h>

/* Bounded PFIFO channel state for the observed empty-ring case. The model
 * tracks base/end/put/get, validates aligned bounded pointers, and initializes
 * GET to the base when the channel is enabled with PUT at the base (empty).
 * It does not consume a non-empty ring, follow a jump or execute a method. */
#define FF_PFIFO_CHANNEL_MAX_BYTES (64u * 1024u * 1024u)

typedef struct {
    uint32_t base, end, put, get;
    uint8_t ready, enabled;
} ff_pfifo_channel;

void ff_pfifo_channel_init(ff_pfifo_channel *);
/* Configure an explicit ring. PUT starts at the base; GET starts at zero. */
uint32_t ff_pfifo_channel_configure(ff_pfifo_channel *, uint32_t base, uint32_t end);
/* Enable the channel; an empty ring initializes GET to PUT. */
uint32_t ff_pfifo_channel_enable(ff_pfifo_channel *);
uint32_t ff_pfifo_channel_set_put(ff_pfifo_channel *, uint32_t put);
/* Explicit software GET write; zero is accepted as an uninitialized value. */
uint32_t ff_pfifo_channel_set_get(ff_pfifo_channel *, uint32_t get);
uint32_t ff_pfifo_channel_read_get(const ff_pfifo_channel *);
uint32_t ff_pfifo_channel_empty(const ff_pfifo_channel *);
#endif
