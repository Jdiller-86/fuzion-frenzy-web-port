/* Owned synthetic aperture scenarios; no original code or device model. */
#include "../src/runtime/device_registers.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static ff_device_registers device, saved;
static void reset(void)
{
    assert(ff_device_registers_init(&device, 0xFD000000u, 0x00800000u, 0xFD001804u, 0x10, 0xFD600140u, 0x20) == 0);
}
static void save(void) { saved = device; }
static void unchanged(void) { assert(!memcmp(&device, &saved, sizeof device)); }

static void known_values_and_unknown_reads(void)
{
    reset();
    uint32_t value = 0;
    assert(ff_device_registers_contains(&device, 0xFD000000u, 4) == 0);
    assert(ff_device_registers_contains(&device, 0xFD7FFFFCu, 4) == 0);
    assert(ff_device_registers_contains(&device, 0xFD800000u, 4) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_contains(&device, 0xFD000002u, 4) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_contains(&device, 0xFD000000u, 2) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x1FE05Au, &value) == 0 && value == 0x10);
    value = 0x14;
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    value = 0;
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x1FE05Au, &value) == 0 && value == 0x14);
    value = 0x7;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    value = 0;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_READ, 0x1FE05Au, &value) == 0 && value == 0x7);
    assert(device.event_count == 5 && device.events[3].before_known == 0 && device.events[3].address == 0xFD009140u);
    save(); value = 0;
    assert(ff_device_registers_touch(&device, 0xFD009240u, FF_DEVICE_READ, 0x1FE05Au, &value) == FF_BOOT_UNSUPPORTED);
    unchanged();
    assert(ff_device_registers_touch(&device, 0xFD009240u, 3, 0x1FE05Au, &value) == FF_BOOT_INVALID);
    unchanged();
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0, &value) == FF_BOOT_UNSUPPORTED);
    unchanged();
    assert(ff_device_registers_stage_write(&device, 0xFD001804u, 0) == NULL);
    unchanged();
    assert(ff_device_registers_commit(&device) == 0);
    unchanged();
}

static void staged_writes_record_the_stored_value(void)
{
    reset();
    uint32_t *stage = ff_device_registers_stage_write(&device, 0xFD001804u, 0x00100020u);
    assert(stage && device.pending && device.event_count == 0);
    *stage = 0x14;
    assert(ff_device_registers_commit(&device) == 0);
    assert(!device.pending && device.event_count == 1);
    assert(device.events[0].address == 0xFD001804u && device.events[0].direction == FF_DEVICE_WRITE &&
           device.events[0].value == 0x14 && device.events[0].site == 0x00100020u);
    assert(device.entries[0].value == 0x14 && device.entries[0].known == 1 && device.entries[0].writes == 1);
    /* A second stage commits the previous write before staging its own. */
    stage = ff_device_registers_stage_write(&device, 0xFD001804u, 0x00100010u);
    assert(stage);
    *stage = 0x24;
    stage = ff_device_registers_stage_write(&device, 0xFD001804u, 0x00100000u);
    assert(stage);
    assert(device.event_count == 2 && device.events[1].value == 0x24 && device.events[1].site == 0x00100010u);
    *stage = 0x34;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.event_count == 3 && device.events[2].value == 0x34 && device.events[2].site == 0x00100000u);
    assert(device.entries[0].value == 0x34 && device.entries[0].writes == 3);
    /* A read commits the pending write first and observes it. */
    stage = ff_device_registers_stage_write(&device, 0xFD001804u, 0x00100000u);
    assert(stage);
    *stage = 0x44;
    uint32_t value = 0;
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x00100000u, &value) == 0 && value == 0x44);
    assert(device.event_count == 5);
    assert(device.events[3].direction == FF_DEVICE_WRITE && device.events[3].value == 0x44);
    assert(device.events[4].direction == FF_DEVICE_READ && device.events[4].value == 0x44);
}

static void read_modify_write_records_both_halves(void)
{
    reset();
    uint32_t *stage = ff_device_registers_access(&device, 0xFD001804u, FF_DEVICE_ACCESS_READWRITE, 0x00100010u);
    assert(stage && *stage == 0x10);
    *stage += 4;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.event_count == 2);
    assert(device.events[0].direction == FF_DEVICE_READ && device.events[0].value == 0x10);
    assert(device.events[1].direction == FF_DEVICE_WRITE && device.events[1].value == 0x14);
    assert(device.entries[0].value == 0x14 && device.entries[0].writes == 1);
    /* An unknown read half fails before a write is staged or an event added. */
    save();
    assert(ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READWRITE, 0x00100010u) == NULL);
    unchanged();
}

static void access_masks_sites_and_widths_fail_closed(void)
{
    reset();
    uint32_t *slot = ff_device_registers_access(&device, 0xFD001804u, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(slot && *slot == 0x10);
    assert(device.event_count == 1 && device.events[0].site == 0x00100020u);
    save();
    assert(ff_device_registers_access(&device, 0xFD001804u, FF_DEVICE_ACCESS_READ, 0) == NULL);
    assert(ff_device_registers_access(&device, 0xFD001804u, 4, 0x00100020u) == NULL);
    assert(ff_device_registers_access(&device, 0xFD001802u, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    assert(ff_device_registers_access(&device, 0xFD800000u, FF_DEVICE_ACCESS_WRITE, 0x00100020u) == NULL);
    unchanged();
}

static void capacity_limits_are_atomic(void)
{
    reset();
    uint32_t value = 1;
    for (uint32_t i = 0; i < FF_DEVICE_ENTRIES - 2; ++i)
        assert(ff_device_registers_touch(&device, 0xFD100000u + i * 4u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    value = 2; save();
    assert(ff_device_registers_touch(&device, 0xFD200000u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == FF_BOOT_NO_MEMORY);
    unchanged();
    assert(ff_device_registers_stage_write(&device, 0xFD200000u, 0x1FE05Au) == NULL);
    unchanged();
    reset();
    value = 3;
    for (uint32_t i = 0; i < FF_DEVICE_EVENTS; ++i)
        assert(ff_device_registers_touch(&device, 0xFD300000u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    save();
    assert(ff_device_registers_touch(&device, 0xFD300000u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == FF_BOOT_NO_MEMORY);
    unchanged();
    assert(ff_device_registers_stage_write(&device, 0xFD300000u, 0x1FE05Au) == NULL);
    unchanged();
}

static void invalid_configuration_is_rejected(void)
{
    reset();
    assert(ff_device_registers_init(&device, 0xFD000002u, 0x800000u, 0xFD001804u, 0, 0xFD600140u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_init(&device, 0xFD000000u, 0x7FFFFFu, 0xFD001804u, 0, 0xFD600140u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_init(&device, 0xFD000000u, 0x800000u, 0xFD001804u, 0, 0xFD600140u, 0) == 0);
    assert(ff_device_registers_init(&device, 0xFD000000u, 0x800000u, 0x00000004u, 0, 0xFD600140u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_init(&device, 0xFD000000u, 0x800000u, 0xFD001804u, 0, 0xFD001804u, 0) == FF_BOOT_INVALID);
}

int main(void)
{
    known_values_and_unknown_reads(); staged_writes_record_the_stored_value();
    read_modify_write_records_both_halves(); access_masks_sites_and_widths_fail_closed();
    capacity_limits_are_atomic(); invalid_configuration_is_rejected();
    puts("owned synthetic aperture: values, staged writes, read-modify-write, sites, bounds, capacities and atomicity passed");
    return 0;
}
