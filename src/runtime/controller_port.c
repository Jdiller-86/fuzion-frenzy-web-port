#include "controller_port.h"
#include "thread_bootstrap.h"

void ff_controller_port_init(ff_controller_port *port)
{
    if (port) ff_controller_init(&port->state);
}

uint32_t ff_controller_port_set(ff_controller_port *port, uint8_t connected, uint16_t buttons,
                                const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT],
                                int16_t lx, int16_t ly, int16_t rx, int16_t ry)
{
    if (!port) return FF_BOOT_INVALID;
    return ff_controller_update(&port->state, connected, buttons, analog, lx, ly, rx, ry);
}

uint32_t ff_controller_port_query(const ff_controller_port *port, uint8_t *out, uint32_t bytes)
{
    if (!port || !out || bytes < FF_CONTROLLER_PACKET_BYTES) return FF_BOOT_INVALID;
    if (!port->state.connected) return FF_CONTROLLER_NOT_CONNECTED;
    return ff_controller_encode(&port->state, out, bytes);
}
