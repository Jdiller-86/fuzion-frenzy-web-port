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

/* Append one ordered event. A zero site is not a translated block and is
 * rejected so it can never be mistaken for provenance. */
static uint32_t record(ff_device_registers *device, uint32_t address, uint32_t direction,
                       uint32_t site, uint32_t *value)
{
    if (!value || !site || ff_device_registers_contains(device, address, 4u)) return FF_BOOT_UNSUPPORTED;
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

uint32_t ff_device_registers_init_many(ff_device_registers *device, uint32_t base, uint32_t bytes,
                                       const ff_device_init_entry *entries, uint32_t count)
{
    if (!device || !entries || !count || count > FF_DEVICE_INITIAL || !bytes ||
        (base & 3u) || (bytes & 3u) || (uint64_t)base + bytes > 0x100000000ull) return FF_BOOT_INVALID;
    for (uint32_t i = 0; i < count; ++i) {
        if ((entries[i].address & 3u) || entries[i].address < base ||
            entries[i].address - base >= bytes) return FF_BOOT_INVALID;
        for (uint32_t j = 0; j < i; ++j)
            if (entries[j].address == entries[i].address) return FF_BOOT_INVALID;
    }
    memset(device, 0, sizeof *device);
    device->base = base; device->bytes = bytes;
    for (uint32_t i = 0; i < count; ++i)
        if (seed(device, entries[i].address, entries[i].value)) return FF_BOOT_INVALID;
    device->ready = 1;
    return 0;
}

uint32_t ff_device_registers_init(ff_device_registers *device, uint32_t base, uint32_t bytes,
                                  uint32_t first_address, uint32_t first_value,
                                  uint32_t second_address, uint32_t second_value)
{
    const ff_device_init_entry entries[2] = { { first_address, first_value }, { second_address, second_value } };
    return ff_device_registers_init_many(device, base, bytes, entries, 2);
}

uint32_t ff_device_registers_add_initial(ff_device_registers *device, uint32_t address, uint32_t value)
{
    if (!device || !device->ready || device->event_count || device->pending ||
        device->count >= FF_DEVICE_INITIAL || (address & 3u) || address < device->base ||
        address - device->base >= device->bytes) return FF_BOOT_INVALID;
    return seed(device, address, value);
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

uint32_t ff_device_registers_commit(ff_device_registers *device)
{
    if (!device) return FF_BOOT_INVALID;
    if (!device->pending) return 0;
    uint32_t address = device->pending_address, site = device->pending_site, value = device->pending_value;
    device->pending = 0;
    return record(device, address, FF_DEVICE_WRITE, site, &value);
}

uint32_t ff_device_registers_touch(ff_device_registers *device, uint32_t address, uint32_t direction,
                                   uint32_t site, uint32_t *value)
{
    if (!device || !value) return FF_BOOT_UNSUPPORTED;
    uint32_t status = ff_device_registers_commit(device);
    if (status) return status;
    return record(device, address, direction, site, value);
}

uint32_t *ff_device_registers_stage_write(ff_device_registers *device, uint32_t address, uint32_t site)
{
    if (!device || !site || ff_device_registers_contains(device, address, 4u)) return NULL;
    if (ff_device_registers_commit(device)) return NULL;
    if (device->event_count >= FF_DEVICE_EVENTS) return NULL;
    if (!find(device, address) && device->count >= FF_DEVICE_ENTRIES) return NULL;
    device->pending = 1;
    device->pending_address = address;
    device->pending_site = site;
    device->pending_value = 0;
    return &device->pending_value;
}

uint32_t *ff_device_registers_access(ff_device_registers *device, uint32_t address, uint32_t access,
                                     uint32_t site)
{
    if (!device || !site || ff_device_registers_contains(device, address, 4u)) return NULL;
    if (access == FF_DEVICE_ACCESS_READ) {
        uint32_t value = 0;
        if (ff_device_registers_touch(device, address, FF_DEVICE_READ, site, &value)) return NULL;
        return ff_device_registers_slot(device, address);
    }
    if (access == FF_DEVICE_ACCESS_WRITE)
        return ff_device_registers_stage_write(device, address, site);
    if (access == FF_DEVICE_ACCESS_READWRITE) {
        uint32_t value = 0;
        if (ff_device_registers_touch(device, address, FF_DEVICE_READ, site, &value)) return NULL;
        uint32_t *stage = ff_device_registers_stage_write(device, address, site);
        if (!stage) return NULL;
        *stage = value;
        return stage;
    }
    return NULL;
}
