#include "device_registers.h"
#include "thread_bootstrap.h"
#include <string.h>

uint32_t ff_device_registers_init(ff_device_registers *device, uint32_t first_address, uint32_t first_value,
                                  uint32_t second_address, uint32_t second_value)
{
    if (!device || (first_address & 3u) || (second_address & 3u) || !first_address || !second_address ||
        first_address == second_address) return FF_BOOT_INVALID;
    memset(device, 0, sizeof *device);
    device->registers[0] = (ff_device_register){first_address, first_value, first_value, 0, 0};
    device->registers[1] = (ff_device_register){second_address, second_value, second_value, 0, 0};
    device->ready = 1;
    return 0;
}

uint32_t ff_device_registers_lookup(const ff_device_registers *device, uint32_t address, uint32_t bytes)
{
    if (!device || !device->ready || bytes != 4u) return FF_BOOT_UNSUPPORTED;
    for (unsigned i = 0; i < FF_DEVICE_REGISTERS; ++i)
        if (device->registers[i].address == address) return i;
    return FF_BOOT_UNSUPPORTED;
}

uint32_t ff_device_registers_touch(ff_device_registers *device, uint32_t address, uint32_t direction,
                                   uint32_t site, uint32_t *value)
{
    uint32_t index = ff_device_registers_lookup(device, address, 4u);
    if (index == FF_BOOT_UNSUPPORTED || index >= FF_DEVICE_REGISTERS || !value) return FF_BOOT_UNSUPPORTED;
    if (direction != FF_DEVICE_READ && direction != FF_DEVICE_WRITE) return FF_BOOT_INVALID;
    if (device->event_count >= FF_DEVICE_EVENTS) return FF_BOOT_NO_MEMORY;
    ff_device_register *reg = &device->registers[index];
    device->events[device->event_count++] =
        (ff_device_event){address, site, direction, reg->value, direction == FF_DEVICE_WRITE ? *value : reg->value};
    if (direction == FF_DEVICE_READ) {
        ++reg->reads;
        *value = reg->value;
    } else {
        ++reg->writes;
        reg->value = *value;
    }
    return 0;
}

uint32_t ff_device_registers_done(const ff_device_registers *device)
{
    return device && device->ready && device->event_count == FF_DEVICE_EVENTS;
}
