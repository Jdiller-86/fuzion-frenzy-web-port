#include "controller_device.h"
#include "thread_bootstrap.h"

void ff_controller_device_init(ff_controller_device *device)
{
    if (!device) return;
    for (uint32_t i = 0; i < FF_CONTROLLER_MAX_PORTS; ++i)
        ff_controller_port_init(&device->ports[i]);
}

uint32_t ff_controller_device_set_port(ff_controller_device *device, uint32_t port, uint8_t connected,
                                       uint16_t buttons, const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT],
                                       int16_t lx, int16_t ly, int16_t rx, int16_t ry)
{
    if (!device) return FF_BOOT_INVALID;
    if (port >= FF_CONTROLLER_MAX_PORTS) return FF_CONTROLLER_NOT_CONNECTED;
    return ff_controller_port_set(&device->ports[port], connected, buttons, analog, lx, ly, rx, ry);
}

uint32_t ff_controller_device_query(const ff_controller_device *device, uint32_t port, uint8_t *out, uint32_t bytes)
{
    if (!device || !out || bytes < FF_CONTROLLER_PACKET_BYTES) return FF_BOOT_INVALID;
    if (port >= FF_CONTROLLER_MAX_PORTS) return FF_CONTROLLER_NOT_CONNECTED;
    return ff_controller_port_query(&device->ports[port], out, bytes);
}
