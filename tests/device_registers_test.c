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
    assert(ff_device_registers_contains(&device, 0xFD000000u, 1) == 0);
    assert(ff_device_registers_contains(&device, 0xFD000003u, 1) == 0);
    assert(ff_device_registers_contains(&device, 0xFD000000u, 2) == 0);
    assert(ff_device_registers_contains(&device, 0xFD000002u, 2) == 0);
    assert(ff_device_registers_contains(&device, 0xFD000003u, 2) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_contains(&device, 0xFD000000u, 3) == FF_BOOT_UNSUPPORTED);
    assert(ff_device_registers_contains(&device, 0xFD7FFFFFu, 1) == 0);
    assert(ff_device_registers_contains(&device, 0xFD800000u, 1) == FF_BOOT_UNSUPPORTED);
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

static void sub_dword_access_uses_the_containing_register(void)
{
    reset();
    uint32_t value = 0x11223344u;
    assert(ff_device_registers_touch(&device, 0xFD001804u, FF_DEVICE_WRITE, 0x00100020u, &value) == 0);
    uint32_t before_events = device.event_count;
    uint8_t *byte = (uint8_t *)ff_device_registers_access_width(
        &device, 0xFD001805u, 1, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(byte && *byte == 0x33);
    assert(device.event_count == before_events + 1);
    assert(device.events[before_events].address == 0xFD001805u);
    assert(device.events[before_events].direction == FF_DEVICE_READ);
    assert(device.events[before_events].value == 0x33);
    assert(device.events[before_events].before_known == 1);
    uint16_t *half = (uint16_t *)ff_device_registers_access_width(
        &device, 0xFD001806u, 2, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(half && *half == 0x1122);
    assert(device.event_count == before_events + 2);
    assert(device.events[before_events + 1].value == 0x1122);
    assert(device.entries[0].reads == 2);
    uint8_t *byte_stage = (uint8_t *)ff_device_registers_access_width(
        &device, 0xFD001805u, 1, FF_DEVICE_ACCESS_WRITE, 0x00100020u);
    assert(byte_stage && device.pending);
    *byte_stage = 0xAA;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.entries[0].value == 0x1122AA44u && device.entries[0].writes == 2);
    assert(device.events[before_events + 2].address == 0xFD001805u);
    assert(device.events[before_events + 2].direction == FF_DEVICE_WRITE);
    assert(device.events[before_events + 2].value == 0xAA);
    assert(device.events[before_events + 2].before_known == 1);
    uint16_t *half_stage = (uint16_t *)ff_device_registers_access_width(
        &device, 0xFD001806u, 2, FF_DEVICE_ACCESS_WRITE, 0x00100020u);
    assert(half_stage && device.pending);
    *half_stage = 0xBBCC;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.entries[0].value == 0xBBCCAA44u && device.entries[0].writes == 3);
    assert(device.events[before_events + 3].value == 0xBBCC);
    save();
    assert(ff_device_registers_access_width(&device, 0xFD009241u, 1, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    unchanged();
    assert(ff_device_registers_access_width(&device, 0xFD001805u, 3, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    unchanged();
}

static void sub_dword_writes_extend_unknown_registers(void)
{
    reset();
    uint32_t before_events = device.event_count;
    uint8_t *byte = (uint8_t *)ff_device_registers_access_width(
        &device, 0xFD009140u, 1, FF_DEVICE_ACCESS_WRITE, 0x00100020u);
    assert(byte && device.pending);
    *byte = 0x11;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.count == 3 && device.entries[2].address == 0xFD009140u);
    assert(device.entries[2].value == 0x11u && device.entries[2].known == 0 && device.entries[2].known_bytes == 1u);
    assert(device.events[before_events].before_known == 0);
    assert(ff_device_registers_access_width(&device, 0xFD009141u, 1, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    for (uint32_t offset = 1; offset < 4; ++offset) {
        byte = (uint8_t *)ff_device_registers_access_width(
            &device, 0xFD009140u + offset, 1, FF_DEVICE_ACCESS_WRITE, 0x00100020u);
        assert(byte && device.pending);
        *byte = (uint8_t)(0x11u * (offset + 1u));
        assert(ff_device_registers_commit(&device) == 0);
    }
    assert(device.entries[2].value == 0x44332211u && device.entries[2].known == 1 && device.entries[2].known_bytes == 0xFu);
    uint32_t *dword = (uint32_t *)ff_device_registers_access_width(
        &device, 0xFD009140u, 4, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(dword && *dword == 0x44332211u);
}

static void zero_ranges_serve_reads_and_yield_to_writes(void)
{
    reset();
    assert(ff_device_registers_add_zero_range(&device, 0xFD100200u, 0x100u) == 0);
    uint32_t *word = (uint32_t *)ff_device_registers_access_width(
        &device, 0xFD100204u, 4, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(word && *word == 0);
    assert(device.event_count == 1 && device.events[0].value == 0 && device.events[0].before_known == 1);
    uint8_t *byte = (uint8_t *)ff_device_registers_access_width(
        &device, 0xFD100207u, 1, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(byte && *byte == 0);
    assert(device.event_count == 2 && device.events[1].value == 0);
    uint32_t *stage = ff_device_registers_stage_write(&device, 0xFD100208u, 0x00100020u);
    assert(stage);
    *stage = 0x12345678u;
    assert(ff_device_registers_commit(&device) == 0);
    word = (uint32_t *)ff_device_registers_access_width(
        &device, 0xFD100208u, 4, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(word && *word == 0x12345678u);
    save();
    assert(ff_device_registers_access_width(&device, 0xFD100300u, 1, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    unchanged();
    assert(ff_device_registers_add_zero_range(&device, 0xFD200000u, 0x100u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_zero_range(&device, 0xFD100202u, 0x100u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_zero_range(&device, 0xFD100200u, 0x100u) == FF_BOOT_INVALID);
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

static void initial_registers_are_explicit_and_bounded(void)
{
    ff_device_init_entry entries[3] = {
        { 0xFD001804u, 0x10 }, { 0xFD600140u, 0x20 }, { 0xFD009140u, 0x30 }
    };
    assert(!ff_device_registers_init_many(&device, 0xFD000000u, 0x00800000u, entries, 3));
    assert(device.ready && device.count == 3 && device.entries[2].address == 0xFD009140u &&
           device.entries[2].value == 0x30);
    reset();
    assert(!ff_device_registers_add_initial(&device, 0xFD009140u, 0x44));
    uint32_t value = 0;
    assert(!ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_READ, 0x00100020u, &value) && value == 0x44);
    save();
    assert(ff_device_registers_add_initial(&device, 0xFD001804u, 0x55) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_initial(&device, 0xFD000002u, 0x55) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_initial(&device, 0xFC000000u, 0x55) == FF_BOOT_INVALID);
    unchanged();
    reset();
    uint32_t *stage = ff_device_registers_stage_write(&device, 0xFD009140u, 0x00100020u);
    assert(stage);
    save();
    assert(ff_device_registers_add_initial(&device, 0xFD009240u, 0x66) == FF_BOOT_INVALID);
    unchanged();
    assert(!ff_device_registers_commit(&device));
    ff_device_init_entry duplicate[2] = { { 0xFD001804u, 1 }, { 0xFD001804u, 2 } };
    save();
    assert(ff_device_registers_init_many(&device, 0xFD000000u, 0x00800000u, duplicate, 2) == FF_BOOT_INVALID);
    unchanged();
    ff_device_init_entry many[FF_DEVICE_INITIAL + 1];
    for (uint32_t i = 0; i < FF_DEVICE_INITIAL + 1; ++i) many[i] = (ff_device_init_entry){ 0xFD000000u + i * 4u, i };
    assert(ff_device_registers_init_many(&device, 0xFD000000u, 0x00800000u, many, FF_DEVICE_INITIAL + 1) == FF_BOOT_INVALID);
    unchanged();
    ff_device_init_entry unaligned[1] = { { 0xFD000002u, 1 } };
    assert(ff_device_registers_init_many(&device, 0xFD000000u, 0x00800000u, unaligned, 1) == FF_BOOT_INVALID);
    unchanged();
}

static void repeated_identical_reads_stop_at_the_poll_limit(void)
{
    reset();
    uint32_t value = 0x11;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    for (uint32_t i = 0; i < FF_DEVICE_POLL_LIMIT; ++i) {
        uint32_t *slot = ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100020u);
        assert(slot && *slot == 0x11);
        assert(!ff_device_registers_poll_limited(&device));
    }
    assert(ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    assert(ff_device_registers_poll_limited(&device));
    assert(ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100024u) != NULL);
    assert(!ff_device_registers_poll_limited(&device));
    value = 0x12;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    assert(ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100024u) != NULL);
    assert(!ff_device_registers_poll_limited(&device));
}

int main(void)
{
    known_values_and_unknown_reads(); staged_writes_record_the_stored_value();
    read_modify_write_records_both_halves(); sub_dword_access_uses_the_containing_register();
    sub_dword_writes_extend_unknown_registers(); zero_ranges_serve_reads_and_yield_to_writes();
    access_masks_sites_and_widths_fail_closed();
    capacity_limits_are_atomic(); invalid_configuration_is_rejected(); initial_registers_are_explicit_and_bounded();
    repeated_identical_reads_stop_at_the_poll_limit();
    puts("owned synthetic aperture: values, staged writes, read-modify-write, sites, initial registers, bounds, capacities, polling and atomicity passed");
    return 0;
}
