#include "device_registers.h"
#include "thread_bootstrap.h"
#include <string.h>

static ff_device_entry *find(ff_device_registers *device, uint32_t address)
{
    for (uint32_t i = 0; i < device->count; ++i) if (device->entries[i].address == address) return &device->entries[i];
    return NULL;
}

static uint32_t seed(ff_device_registers *device, uint32_t address, uint32_t value)
{
    if (device->count >= FF_DEVICE_ENTRIES) return FF_BOOT_NO_MEMORY;
    for (uint32_t i = 0; i < device->count; ++i) if (device->entries[i].address == address) return FF_BOOT_INVALID;
    device->entries[device->count++] = (ff_device_entry){address, value, 0, 0, 1};
    return 0;
}

uint32_t ff_device_registers_init(ff_device_registers *device, uint32_t base, uint32_t bytes,
                                  uint32_t first_address, uint32_t first_value,
                                  uint32_t second_address, uint32_t second_value)
{
    if (!device || !bytes || (base & 3u) || (bytes & 3u) || (uint64_t)base + bytes > 0x100000000ull ||
        (first_address & 3u) || (second_address & 3u) || first_address == second_address ||
        first_address - base >= bytes || second_address - base >= bytes) return FF_BOOT_INVALID;
    memset(device, 0, sizeof *device);
    device->base = base; device->bytes = bytes;
    if (seed(device, first_address, first_value)) return FF_BOOT_INVALID;
    if (seed(device, second_address, second_value)) return FF_BOOT_INVALID;
    device->ready = 1;
    return 0;
}

uint32_t ff_device_registers_contains(const ff_device_registers *device, uint32_t address, uint32_t bytes)
{
    if (!device || !device->ready || bytes != 4u || (address & 3u)) return FF_BOOT_UNSUPPORTED;
    uint64_t end = (uint64_t)address + 4u;
    if (address < device->base || end > (uint64_t)device->base + device->bytes) return FF_BOOT_UNSUPPORTED;
    return 0;
}

uint32_t *ff_device_registers_slot(ff_device_registers *device, uint32_t address)
{
    ff_device_entry *entry = device ? find(device, address) : NULL;
    return entry ? &entry->value : NULL;
}
uint32_t ff_device_registers_touch(ff_device_registers *device, uint32_t address, uint32_t direction,
                                   uint32_t site, uint32_t *value)
{
    if (!value || ff_device_registers_contains(device, address, 4u)) return FF_BOOT_UNSUPPORTED;
    if (direction != FF_DEVICE_READ && direction != FF_DEVICE_WRITE) return FF_BOOT_INVALID;
    if (device->event_count >= FF_DEVICE_EVENTS) return FF_BOOT_NO_MEMORY;
    ff_device_entry *entry = find(device, address);
    if (direction == FF_DEVICE_READ && (!entry || !entry->known)) return FF_BOOT_UNSUPPORTED;
    if (!entry) {
        if (device->count >= FF_DEVICE_ENTRIES) return FF_BOOT_NO_MEMORY;
        entry = &device->entries[device->count++];
        *entry = (ff_device_entry){address, 0, 0, 0, 0};
    }
    device->events[device->event_count++] =
        (ff_device_event){address, site, direction, direction == FF_DEVICE_WRITE ? *value : entry->value, entry->known};
    if (direction == FF_DEVICE_READ) { ++entry->reads; *value = entry->value; }
    else { ++entry->writes; entry->value = *value; entry->known = 1; }
    return 0;
}
