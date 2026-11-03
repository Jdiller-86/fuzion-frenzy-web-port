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
    reset();
    value = 3;
    for (uint32_t i = 0; i < FF_DEVICE_EVENTS; ++i)
        assert(ff_device_registers_touch(&device, 0xFD300000u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    save();
    assert(ff_device_registers_touch(&device, 0xFD300000u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == FF_BOOT_NO_MEMORY);
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
    known_values_and_unknown_reads(); capacity_limits_are_atomic(); invalid_configuration_is_rejected();
    puts("owned synthetic aperture: known values, unknown-read failure, bounds, capacities and atomicity passed");
    return 0;
}
