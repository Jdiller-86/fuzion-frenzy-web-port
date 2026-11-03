/* Owned port-output log scenarios; no device model or original code. */
#include "../src/runtime/port_io.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    ff_port_io io;
    ff_port_io_init(&io);
    assert(io.count == 0);
    assert(ff_port_io_write(&io, 0x80C0u, 0x1Fu, 1) == 0);
    assert(io.count == 1 && io.events[0].port == 0x80C0u && io.events[0].value == 0x1Fu &&
           io.events[0].width == 1u);
    assert(ff_port_io_write(&io, 0x12345678u, 0xABu, 2) == 0);
    assert(io.count == 2 && io.events[1].port == 0x5678u && io.events[1].value == 0xABu &&
           io.events[1].width == 2u);
    assert(ff_port_io_write(&io, 0x80C0u, 0, 3) == 1);
    assert(io.count == 2);
    for (uint32_t i = 2; i < FF_PORT_EVENTS; ++i)
        assert(ff_port_io_write(&io, 0x100u + i, i, 4) == 0);
    assert(io.count == FF_PORT_EVENTS);
    assert(ff_port_io_write(&io, 0x200u, 0x55u, 1) == 0);
    assert(io.count == FF_PORT_EVENTS && io.events[FF_PORT_EVENTS - 1].port == 0x100u + FF_PORT_EVENTS - 1u);
    uint32_t value = 0, untouched = 0xdead;
    assert(ff_port_io_read(&io, 0x80C0u, 1, &value) == 0 && value == 0x20u);
    assert(ff_port_io_read(&io, 0x80C0u, 1, &value) == 0 && value == 0u);
    assert(ff_port_io_read(&io, 0x1234u, 1, &untouched) == 1 && untouched == 0xdeadu);
    assert(io.reads == 2u);
    assert(ff_port_io_read(&io, 0x80C0u, 3, &value) == 1);
    assert(ff_port_io_read(&io, 0x80C0u, 1, NULL) == 1);
    assert(io.reads == 2u);
    ff_port_io_init(&io);
    assert(io.count == 0 && io.reads == 0u);
    puts("owned port surface: output log plus explicit 0x80C0 field-pin reads passed");
    return 0;
}
