#include "device_registers.h"
#include "thread_bootstrap.h"
#include <string.h>

static ff_device_entry *find(ff_device_registers *device, uint32_t address)
{
    for (uint32_t i = 0; i < device->count; ++i) if (device->entries[i].address == address) return &device->entries[i];
    return NULL;
}

static int in_zero_range(const ff_device_registers *device, uint32_t address, uint32_t bytes)
{
    const uint64_t end = (uint64_t)address + bytes;
    for (uint32_t i = 0; i < device->zero_count; ++i) {
        const ff_device_zero_range *range = &device->zero_ranges[i];
        if (address >= range->base && end <= (uint64_t)range->base + range->bytes) return 1;
    }
    return 0;
}

/* Consecutive identical reads at one site model a wait for a hardware
 * transition this synthetic aperture cannot provide. */
static uint32_t note_read(ff_device_registers *device, uint32_t address, uint32_t site, uint32_t value)
{
    if (device->poll_count && device->poll_address == address &&
        device->poll_site == site && device->poll_value == value)
        return ++device->poll_count > FF_DEVICE_POLL_LIMIT ? FF_BOOT_UNSUPPORTED : 0;
    device->poll_address = address; device->poll_site = site;
    device->poll_value = value; device->poll_count = 1;
    return 0;
}

uint32_t ff_device_registers_poll_limited(const ff_device_registers *device)
{
    return device && device->poll_count > FF_DEVICE_POLL_LIMIT;
}

static uint32_t record_zero_read(ff_device_registers *device, uint32_t address, uint32_t site)
{
    if (!device || !site || device->event_count >= FF_DEVICE_EVENTS) return FF_BOOT_UNSUPPORTED;
    device->events[device->event_count++] =
        (ff_device_event){address, site, FF_DEVICE_READ, 0, 1};
    return 0;
}

static uint32_t seed(ff_device_registers *device, uint32_t address, uint32_t value)
{
    if (device->count >= FF_DEVICE_ENTRIES) return FF_BOOT_NO_MEMORY;
    for (uint32_t i = 0; i < device->count; ++i) if (device->entries[i].address == address) return FF_BOOT_INVALID;
    device->entries[device->count++] = (ff_device_entry){address, value, 0, 0, 1, 0xFu};
    return 0;
}

static void self_clear(ff_device_registers *device, uint32_t address);

/* Append one ordered event. A zero site is not a translated block and is
 * rejected so it can never be mistaken for provenance. */
static uint32_t record(ff_device_registers *device, uint32_t address, uint32_t direction,
                       uint32_t site, uint32_t *value)
{
    if (!value || !site || ff_device_registers_contains(device, address, 4u)) return FF_BOOT_UNSUPPORTED;
    if (direction != FF_DEVICE_READ && direction != FF_DEVICE_WRITE) return FF_BOOT_INVALID;
    if (device->event_count >= FF_DEVICE_EVENTS) return FF_BOOT_NO_MEMORY;
    ff_device_entry *entry = find(device, address);
    if (direction == FF_DEVICE_READ && (!entry || !entry->known || entry->known_bytes != 0xFu))
        return FF_BOOT_UNSUPPORTED;
    if (!entry) {
        if (device->count >= FF_DEVICE_ENTRIES) return FF_BOOT_NO_MEMORY;
        entry = &device->entries[device->count++];
        *entry = (ff_device_entry){address, 0, 0, 0, 0, 0};
    }
    device->events[device->event_count++] =
        (ff_device_event){address, site, direction, direction == FF_DEVICE_WRITE ? *value : entry->value, entry->known};
    if (direction == FF_DEVICE_READ) { ++entry->reads; *value = entry->value; }
    else { ++entry->writes; entry->value = *value; entry->known = 1; entry->known_bytes = 0xFu;
           self_clear(device, address); }
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

uint32_t ff_device_registers_add_zero_range(ff_device_registers *device, uint32_t base, uint32_t bytes)
{
    if (!device || !device->ready || device->event_count || device->pending) return FF_BOOT_INVALID;
    if (!bytes || (base & 3u) || (bytes & 3u) || base < device->base ||
        (uint64_t)base + bytes > (uint64_t)device->base + device->bytes) return FF_BOOT_INVALID;
    if (device->zero_count >= FF_DEVICE_ZERO_RANGES) return FF_BOOT_INVALID;
    for (uint32_t i = 0; i < device->zero_count; ++i)
        if (device->zero_ranges[i].base == base) return FF_BOOT_INVALID;
    device->zero_ranges[device->zero_count++] = (ff_device_zero_range){base, bytes};
    return 0;
}

uint32_t ff_device_registers_add_self_clearing(ff_device_registers *device, uint32_t address, uint32_t mask)
{
    if (!device || !device->ready || device->event_count || device->pending || !mask ||
        (address & 3u) || address < device->base || address - device->base >= device->bytes)
        return FF_BOOT_INVALID;
    if (device->self_clearing_count >= FF_DEVICE_SELF_CLEARING) return FF_BOOT_INVALID;
    for (uint32_t i = 0; i < device->self_clearing_count; ++i)
        if (device->self_clearing[i].address == address) return FF_BOOT_INVALID;
    device->self_clearing[device->self_clearing_count++] = (ff_device_self_clearing){address, mask};
    return 0;
}

uint32_t ff_device_registers_contains(const ff_device_registers *device, uint32_t address, uint32_t bytes)
{
    if (!device || !device->ready || (bytes != 1u && bytes != 2u && bytes != 4u))
        return FF_BOOT_UNSUPPORTED;
    uint64_t end = (uint64_t)address + bytes;
    if (address < device->base || end > (uint64_t)device->base + device->bytes) return FF_BOOT_UNSUPPORTED;
    if (bytes == 4u && (address & 3u)) return FF_BOOT_UNSUPPORTED;
    /* A sub-dword access must stay inside one dword register. */
    if (bytes == 2u && (address & 3u) > 2u) return FF_BOOT_UNSUPPORTED;
    return 0;
}

uint32_t *ff_device_registers_slot(ff_device_registers *device, uint32_t address)
{
    ff_device_entry *entry = device ? find(device, address) : NULL;
    return entry ? &entry->value : NULL;
}

/* A modeled write-completion bit reads back clear after its completed write.
 * The event keeps the value the guest actually stored. */
static void self_clear(ff_device_registers *device, uint32_t address)
{
    for (uint32_t i = 0; i < device->self_clearing_count; ++i)
        if (device->self_clearing[i].address == address) {
            ff_device_entry *entry = find(device, address);
            if (entry) entry->value &= ~device->self_clearing[i].mask;
        }
}

uint32_t ff_device_registers_commit(ff_device_registers *device)
{
    if (!device) return FF_BOOT_INVALID;
    if (!device->pending) return 0;
    uint32_t address = device->pending_address, site = device->pending_site, value = device->pending_value;
    uint32_t bytes = device->pending_bytes ? device->pending_bytes : 4u;
    uint32_t offset = device->pending_offset;
    device->pending = 0;
    if (bytes == 4u) return record(device, address, FF_DEVICE_WRITE, site, &value);
    /* Sub-dword writes may create or extend the containing dword. Untouched
     * bytes stay unknown, so a later read still fails until they are written. */
    uint32_t base = address & ~3u;
    ff_device_entry *entry = find(device, base);
    if (device->event_count >= FF_DEVICE_EVENTS) return FF_BOOT_NO_MEMORY;
    if (!entry) {
        if (device->count >= FF_DEVICE_ENTRIES) return FF_BOOT_NO_MEMORY;
        entry = &device->entries[device->count++];
        *entry = (ff_device_entry){base, 0, 0, 0, 0, 0};
    }
    uint32_t before = entry->known;
    uint32_t shift = offset * 8u, mask = bytes == 1u ? 0xFFu : 0xFFFFu;
    uint32_t written = (value >> shift) & mask;
    uint32_t merged = (entry->value & ~(mask << shift)) | (written << shift);
    device->events[device->event_count++] =
        (ff_device_event){address, site, FF_DEVICE_WRITE, written, before};
    ++entry->writes;
    entry->value = merged;
    entry->known_bytes |= (bytes == 1u ? 1u : 3u) << offset;
    entry->known = entry->known_bytes == 0xFu;
    self_clear(device, base);
    return 0;
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
    device->pending_bytes = 4;
    device->pending_offset = 0;
    return &device->pending_value;
}

static uint32_t *stage_write_width(ff_device_registers *device, uint32_t address, uint32_t bytes,
                                   uint32_t site)
{
    if (!device || !site || (bytes != 1u && bytes != 2u)) return NULL;
    if (ff_device_registers_contains(device, address, bytes)) return NULL;
    if (ff_device_registers_commit(device)) return NULL;
    ff_device_entry *entry = find(device, address & ~3u);
    if (device->event_count >= FF_DEVICE_EVENTS ||
        (!entry && device->count >= FF_DEVICE_ENTRIES)) return NULL;
    device->pending = 1;
    device->pending_address = address;
    device->pending_site = site;
    device->pending_value = 0;
    device->pending_bytes = bytes;
    device->pending_offset = address & 3u;
    return (uint32_t *)((uint8_t *)&device->pending_value + device->pending_offset);
}

uint32_t *ff_device_registers_access(ff_device_registers *device, uint32_t address, uint32_t access,
                                     uint32_t site)
{
    if (!device || !site || ff_device_registers_contains(device, address, 4u)) return NULL;
    if (access == FF_DEVICE_ACCESS_READ) {
        if (!find(device, address) && in_zero_range(device, address, 4u)) {
            if (ff_device_registers_commit(device) || record_zero_read(device, address, site)) return NULL;
            if (note_read(device, address, site, 0)) return NULL;
            return &device->zero_word;
        }
        uint32_t value = 0;
        if (ff_device_registers_touch(device, address, FF_DEVICE_READ, site, &value)) return NULL;
        if (note_read(device, address, site, value)) return NULL;
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
        if (note_read(device, address, site, value)) return NULL;
        return stage;
    }
    return NULL;
}

uint32_t *ff_device_registers_access_width(ff_device_registers *device, uint32_t address, uint32_t bytes,
                                           uint32_t access, uint32_t site)
{
    if (!device || !site || (bytes != 1u && bytes != 2u && bytes != 4u)) return NULL;
    if (bytes == 4u) return ff_device_registers_access(device, address, access, site);
    if (access == FF_DEVICE_ACCESS_WRITE) return stage_write_width(device, address, bytes, site);
    if (access != FF_DEVICE_ACCESS_READ && access != FF_DEVICE_ACCESS_READWRITE) return NULL;
    if (ff_device_registers_contains(device, address, bytes) ||
        ff_device_registers_commit(device)) return NULL;
    ff_device_entry *entry = find(device, address & ~3u);
    if (access == FF_DEVICE_ACCESS_READ && !entry && in_zero_range(device, address, bytes)) {
        if (record_zero_read(device, address, site)) return NULL;
        if (note_read(device, address, site, 0)) return NULL;
        return (uint32_t *)((uint8_t *)&device->zero_word + (address & 3u));
    }
    uint32_t byte_bits = (bytes == 1u ? 1u : 3u) << (address & 3u);
    if (!entry || (entry->known_bytes & byte_bits) != byte_bits ||
        device->event_count >= FF_DEVICE_EVENTS ||
        (access == FF_DEVICE_ACCESS_READWRITE && device->event_count >= FF_DEVICE_EVENTS - 1u))
        return NULL;
    uint32_t shift = (address & 3u) * 8u;
    uint32_t mask = bytes == 1u ? 0xFFu : 0xFFFFu;
    uint32_t value = (entry->value >> shift) & mask;
    device->events[device->event_count++] =
        (ff_device_event){address, site, FF_DEVICE_READ, value, entry->known};
    ++entry->reads;
    if (note_read(device, address, site, value)) return NULL;
    if (access == FF_DEVICE_ACCESS_READ) return (uint32_t *)((uint8_t *)&entry->value + (address & 3u));
    uint32_t *stage = stage_write_width(device, address, bytes, site);
    if (!stage) return NULL;
    for (uint32_t i = 0; i < bytes; ++i)
        ((uint8_t *)stage)[i] = (uint8_t)(value >> (i * 8u));
    return stage;
}
