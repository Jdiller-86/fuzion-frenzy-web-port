/* Owned synthetic register-storage scenarios; no original code or device model. */
#include "../src/runtime/device_registers.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static ff_device_registers device, saved;
static void reset(void)
{
    assert(ff_device_registers_init(&device, 0xFD001804u, 0, 0xFD600140u, 0) == 0);
}
static void save(void) { saved = device; }
static void unchanged(void) { assert(!memcmp(&device, &saved, sizeof device)); }

static void explicit_values_and_read_modify_write(void)
{
    assert(ff_device_registers_init(&device, 0xFD001804u, 0x10, 0xFD600140u, 0x20) == 0);
    uint32_t value = 0;
    assert(ff_device_registers_lookup(&device, 0xFD001804u, 4) == 0);
    assert(ff_device_registers_lookup(&device, 0xFD001800u, 4) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_lookup(&device, 0xFD001804u, 2) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x1FE05Au, &value) == 0 && value == 0x10);
    value = 0x14;
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    value = 0;
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x1FE05Au, &value) == 0 && value == 0x14);
    assert(device.registers[0].reads == 2 && device.registers[0].writes == 1);
    assert(device.event_count == 3 && ff_device_registers_done(&device));
    value = 0;
    save();
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x1FE05Au, &value) == FF_BOOT_NO_MEMORY);
    unchanged();
}

static void invalid_requests_preserve_state(void)
{
    reset();
    assert(ff_device_registers_init(&device, 0xFD001805u, 0, 0xFD600140u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_init(&device, 0xFD001804u, 0, 0xFD001804u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_init(&device, 0, 0, 0xFD600140u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_init(NULL, 0xFD001804u, 0, 0xFD600140u, 0) == FF_BOOT_INVALID);
    reset();
    uint32_t value = 0;
    save();
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == FF_BOOT_UNSUPPORTED);
    unchanged();
    assert(ff_device_registers_touch(&device, 0xFD001804u, 3, 0x1FE05Au, &value) == FF_BOOT_INVALID);
    unchanged();
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_READ, 0x1FE05Au, NULL) == FF_BOOT_UNSUPPORTED);
    unchanged();
}

int main(void)
{
    explicit_values_and_read_modify_write(); invalid_requests_preserve_state();
    puts("owned synthetic register storage: explicit values, ordering, bounds and failure atomicity passed");
    return 0;
}
