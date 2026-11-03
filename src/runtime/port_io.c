#include "port_io.h"
#include <string.h>

void ff_port_io_init(ff_port_io *io)
{
    if (io) memset(io, 0, sizeof *io);
}

uint32_t ff_port_io_write(ff_port_io *io, uint32_t port, uint32_t value, uint32_t width)
{
    if (!io || (width != 1u && width != 2u && width != 4u)) return 1;
    if (io->count < FF_PORT_EVENTS)
        io->events[io->count++] = (ff_port_event){port & 0xFFFFu, value, width};
    return 0;
}

uint32_t ff_port_io_read(ff_port_io *io, uint32_t port, uint32_t width)
{
    if (!io || (width != 1u && width != 2u && width != 4u)) return 0;
    port &= 0xFFFFu;
    if (port != 0x80C0u) return 0;
    ++io->reads;
    return (io->reads & 1u) << 5;
}
