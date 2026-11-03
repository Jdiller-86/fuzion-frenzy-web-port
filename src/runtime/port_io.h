#ifndef FF_PORT_IO_H
#define FF_PORT_IO_H
#include <stdint.h>

/* Bounded synthetic x86 port-output log for reached translated OUT sites.
 * This is not a device model: the write is recorded and ignored. No port
 * input value, timing or hardware effect is fabricated. */
#define FF_PORT_EVENTS 8u
typedef struct {
    uint32_t port, value, width;
} ff_port_event;
typedef struct {
    uint32_t count;
    ff_port_event events[FF_PORT_EVENTS];
} ff_port_io;

void ff_port_io_init(ff_port_io *);
/* Returns 0 for a valid 1/2/4-byte output, 1 for an invalid width. Events past
 * the bounded capacity are dropped without changing earlier records. */
uint32_t ff_port_io_write(ff_port_io *, uint32_t port, uint32_t value, uint32_t width);
#endif
