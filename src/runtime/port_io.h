#ifndef FF_PORT_IO_H
#define FF_PORT_IO_H
#include <stdint.h>

/* Bounded synthetic x86 port surface for reached translated sites.
 * Output writes are recorded and ignored. Input reads model only the pinned
 * bridge's 0x80C0 vblank field pin by toggling bit 5 per read; every other
 * port reads zero. This is not a general device model or timing source. */
#define FF_PORT_EVENTS 8u
typedef struct {
    uint32_t port, value, width;
} ff_port_event;
typedef struct {
    uint32_t count, reads;
    ff_port_event events[FF_PORT_EVENTS];
} ff_port_io;

void ff_port_io_init(ff_port_io *);
/* Returns 0 for a valid 1/2/4-byte output, 1 for an invalid width. Events past
 * the bounded capacity are dropped without changing earlier records. */
uint32_t ff_port_io_write(ff_port_io *, uint32_t port, uint32_t value, uint32_t width);
/* Returns the pinned field-pin value for 0x80C0 (bit 5 toggles per read) and
 * zero for every other port. Invalid widths return zero without counting. */
uint32_t ff_port_io_read(ff_port_io *, uint32_t port, uint32_t width);
#endif
