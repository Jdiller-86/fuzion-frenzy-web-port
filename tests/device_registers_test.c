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
    for (uint32_t i = 0; i < FF_DEVICE_ZERO_RANGES; ++i)
        assert(ff_device_registers_add_zero_range(&device, 0xFD100000u + i * 0x100u, 0x10u) == 0);
    assert(ff_device_registers_add_zero_range(&device, 0xFD100800u, 0x10u) == FF_BOOT_INVALID);
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

static void interleaved_reads_of_one_register_stop_at_the_wait_limit(void)
{
    reset();
    uint32_t status = 0x2u, other = 0x3u;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &status) == 0);
    assert(ff_device_registers_touch(&device, 0xFD009180u, FF_DEVICE_WRITE, 0x1FE05Au, &other) == 0);
    /* A loop body that reads the watched register at one site and another
     * register at a different site each pass defeats the single-site detector;
     * the per-register counter still names the wait. */
    uint32_t stopped = 0;
    for (uint32_t i = 0; i < FF_DEVICE_WAIT_LIMIT + 2u && !stopped; ++i) {
        if (!ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100020u)) { stopped = 1; break; }
        if (!ff_device_registers_access(&device, 0xFD009180u, FF_DEVICE_ACCESS_READ, 0x00100024u)) { stopped = 1; break; }
    }
    assert(stopped && ff_device_registers_poll_limited(&device));
    assert(ff_device_registers_wait_address(&device) == 0xFD009140u);
    /* A store to the watched register restarts its count. */
    uint32_t changed = 0x4u;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &changed) == 0);
    uint32_t readback = 0;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_READ, 0x00100028u, &readback) == 0 && readback == 0x4u);
    assert(ff_device_registers_wait_address(&device) == 0xFD009140u);
}

static void capacity_exhaustion_is_reported_separately_from_a_denied_access(void)
{
    reset();
    assert(!ff_device_registers_capacity_reached(&device));
    uint32_t value = 0x5u;
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    assert(ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100020u) != NULL);
    /* The ordered log is the bounded resource: once full, the next access fails
     * for storage reasons instead of for an unmodeled address. */
    device.event_count = FF_DEVICE_EVENTS;
    assert(ff_device_registers_capacity_reached(&device));
    assert(ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100020u) == NULL);
    assert(ff_device_registers_touch(&device, 0xFD009140u, FF_DEVICE_WRITE, 0x1FE05Au, &value) != 0);
}

static void a_declared_status_wait_trips_despite_a_busy_register_file(void)
{
    reset();
    /* Declare the polled status input, then read more other registers than the
     * generic wait table can hold every pass: the declared wait still trips
     * because it is tracked by declaration, not by table capacity. */
    assert(ff_device_registers_add_status_wait(&device, 0xFD009140u, 0x01000000u) == 0);
    assert(ff_device_registers_add_status_wait(&device, 0xFD009140u, 1u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_status_wait(&device, 0xFD009142u, 1u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_status_wait(&device, 0xFD009144u, 0u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_initial(&device, 0xFD009140u, 0u) == 0);
    for (uint32_t i = 0; i < FF_DEVICE_WAITS + 8u; ++i)
        assert(ff_device_registers_add_initial(&device, 0xFD009180u + i * 4u, i + 2u) == 0);
    uint32_t stopped = 0;
    for (uint32_t pass = 0; pass < FF_DEVICE_WAIT_LIMIT + 2u && !stopped; ++pass) {
        if (!ff_device_registers_access(&device, 0xFD009140u, FF_DEVICE_ACCESS_READ, 0x00100020u)) stopped = 1;
        for (uint32_t i = 0; i < FF_DEVICE_WAITS + 8u && !stopped; ++i)
            if (!ff_device_registers_access(&device, 0xFD009180u + i * 4u, FF_DEVICE_ACCESS_READ, 0x00100024u)) stopped = 1;
    }
    assert(stopped && ff_device_registers_poll_limited(&device));
    assert(ff_device_registers_wait_address(&device) == 0xFD009140u);
}

static void self_clearing_bits_complete_after_the_write(void)
{
    reset();
    assert(ff_device_registers_add_self_clearing(&device, 0xFD009200u, 0x10000u) == 0);
    assert(ff_device_registers_add_self_clearing(&device, 0xFD009200u, 0x20000u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_self_clearing(&device, 0xFD009202u, 0x1u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_self_clearing(&device, 0xFD009200u, 0) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_self_clearing(&device, 0xFD800000u, 1) == FF_BOOT_INVALID);
    uint32_t value = 0x50000u;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    uint32_t readback = 0xffffffffu;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_READ, 0x00100020u, &readback) == 0);
    assert(readback == 0x40000u);
    assert(device.events[0].direction == FF_DEVICE_WRITE && device.events[0].value == 0x50000u);
    assert(ff_device_registers_add_self_clearing(&device, 0xFD009204u, 0x10u) == FF_BOOT_INVALID);
    uint32_t *slot = ff_device_registers_access(&device, 0xFD009200u, FF_DEVICE_ACCESS_READ, 0x00100020u);
    assert(slot && *slot == 0x40000u);
}

static void write_clearing_registers_acknowledge_only_the_stored_bits(void)
{
    reset();
    /* Pending status is a hardware state, so seed it before traffic starts. */
    assert(ff_device_registers_add_initial(&device, 0xFD009200u, 0x3000u) == 0);
    assert(ff_device_registers_add_write_clearing(&device, 0xFD009200u) == 0);
    assert(ff_device_registers_add_write_clearing(&device, 0xFD009200u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_write_clearing(&device, 0xFD009202u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_write_clearing(&device, 0xFD800000u) == FF_BOOT_INVALID);
    uint32_t acknowledge = 0x1000u;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_WRITE, 0x00100024u, &acknowledge) == 0);
    assert(device.events[0].direction == FF_DEVICE_WRITE && device.events[0].value == 0x1000u);
    uint32_t readback = 0xffffffffu;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_READ, 0x00100028u, &readback) == 0);
    assert(readback == 0x2000u);
    /* A store that acknowledges nothing leaves the remaining status set, and a
     * status bit can never be latched by the guest writing it. */
    acknowledge = 0;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_WRITE, 0x00100024u, &acknowledge) == 0);
    acknowledge = 0x4000u;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_WRITE, 0x0010002Cu, &acknowledge) == 0);
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_READ, 0x00100028u, &readback) == 0);
    assert(readback == 0x2000u);
    /* A partial store cannot express an exclusive acknowledge mask. */
    uint32_t *half = ff_device_registers_access_width(&device, 0xFD009200u, 2u, FF_DEVICE_ACCESS_WRITE, 0x00100030u);
    assert(half != NULL);
    *half = 0x2000u;
    assert(ff_device_registers_commit(&device) == FF_BOOT_UNSUPPORTED);
}

static void aliases_share_storage_and_keep_event_addresses(void)
{
    reset();
    assert(ff_device_registers_add_alias(&device, 0xFD009200u, 0xFD009240u) == 0);
    assert(ff_device_registers_add_alias(&device, 0xFD009200u, 0xFD009280u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_alias(&device, 0xFD009202u, 0xFD0092C0u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_alias(&device, 0xFD009200u, 0xFD009200u) == FF_BOOT_INVALID);
    assert(ff_device_registers_add_alias(&device, 0xFD800000u, 0xFD0092C0u) == FF_BOOT_INVALID);
    uint32_t value = 0x11223344u;
    assert(ff_device_registers_touch(&device, 0xFD009240u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    uint32_t read = 0;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_READ, 0x00100020u, &read) == 0);
    assert(read == 0x11223344u);
    assert(device.count == 3);
    assert(device.events[0].address == 0xFD009240u && device.events[0].value == 0x11223344u);
    assert(device.events[1].address == 0xFD009200u && device.events[1].value == 0x11223344u);
    value = 0x55667788u;
    assert(ff_device_registers_touch(&device, 0xFD009200u, FF_DEVICE_WRITE, 0x1FE05Au, &value) == 0);
    assert(ff_device_registers_touch(&device, 0xFD009240u, FF_DEVICE_READ, 0x00100020u, &read) == 0);
    assert(read == 0x55667788u);
    assert(ff_device_registers_add_alias(&device, 0xFD009280u, 0xFD0092C0u) == FF_BOOT_INVALID);
}

int main(void)
{
    known_values_and_unknown_reads(); staged_writes_record_the_stored_value();
    read_modify_write_records_both_halves(); sub_dword_access_uses_the_containing_register();
    sub_dword_writes_extend_unknown_registers(); zero_ranges_serve_reads_and_yield_to_writes();
    access_masks_sites_and_widths_fail_closed();
    capacity_limits_are_atomic(); invalid_configuration_is_rejected(); initial_registers_are_explicit_and_bounded();
    repeated_identical_reads_stop_at_the_poll_limit(); self_clearing_bits_complete_after_the_write();
    interleaved_reads_of_one_register_stop_at_the_wait_limit(); capacity_exhaustion_is_reported_separately_from_a_denied_access();
    a_declared_status_wait_trips_despite_a_busy_register_file();
    write_clearing_registers_acknowledge_only_the_stored_bits();
    aliases_share_storage_and_keep_event_addresses();
    puts("owned synthetic aperture: values, staged writes, read-modify-write, sites, initial registers, bounds, capacities, polling, self-clearing bits, write-clearing acknowledgement, aliases and atomicity passed");
    return 0;
}
