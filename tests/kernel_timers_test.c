/* Owned clock/object scenarios. No firmware, original code or wall-clock wait. */
#include "../src/runtime/kernel_timers.h"
#include "../src/runtime/kernel_objects.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[16384];
static ff_timer_queue queue;
static uint32_t word(unsigned address)
{
    return (uint32_t)ram[address] | (uint32_t)ram[address + 1] << 8 |
        (uint32_t)ram[address + 2] << 16 | (uint32_t)ram[address + 3] << 24;
}
static void put(unsigned address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (i * 8));
}
static uint64_t wide(unsigned address) { return word(address) | (uint64_t)word(address + 4) << 32; }
static uint32_t timer(unsigned i) { return 128 + i * 64; }
static uint32_t dpc(unsigned i) { return 4096 + i * 32; }
static void reset(uint64_t interrupt_time, uint64_t system_time)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_timers_init(&queue, ram, sizeof ram, 16, 24) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, interrupt_time, system_time) == FF_BOOT_OK);
    for (unsigned i = 0; i < 34; ++i) {
        assert(ff_initialize_timer(ram, sizeof ram, timer(i), i & 1) == FF_BOOT_OK);
        assert(ff_initialize_dpc(ram, sizeof ram, dpc(i), 0x1200 + i * 16, 0x7000 + i) == FF_BOOT_OK);
    }
}
static void six_hour_boundary(void)
{
    const uint64_t start = 0x1fffffffcu, system = 0x123456789abcULL, delay = 216000000000ULL;
    for (unsigned type = 0; type < 2; ++type) {
        reset(start, system); uint32_t previous = 99, present = 99; ff_dpc_call call = {0};
        assert(ff_timer_set(&queue, timer(type), -(int64_t)delay, dpc(0), &previous) == FF_BOOT_OK && previous == 0);
        assert(ram[timer(type) + 1] == 0 && ram[timer(type) + 3] == 1 && word(timer(type) + 4) == 0);
        assert(wide(timer(type) + 16) == start + delay && word(timer(type) + 32) == dpc(0));
        assert(word(16) == timer(type) + 24 && word(timer(type) + 24) == 16 && word(timer(type) + 28) == 16);
        assert(ff_timers_advance(&queue, start + delay - 1, system + delay - 1) == FF_BOOT_OK);
        assert(ff_dpc_peek(&queue, 0, &call, &present) == FF_BOOT_OK && !present && queue.timer_count == 1);
        assert(ff_timers_advance(&queue, start + delay, system + delay) == FF_BOOT_OK);
        assert(!queue.timer_count && ram[timer(type) + 3] == 0 && word(timer(type) + 4) == 1);
        assert(ff_dpc_peek(&queue, 2, &call, &present) == FF_BOOT_OK && !present && queue.dpc_count == 1);
        assert(ff_dpc_pop(&queue, 2, dpc(0)) == FF_BOOT_UNSUPPORTED);
        assert(ff_dpc_peek(&queue, 0, &call, &present) == FF_BOOT_OK && present);
        assert(call.object == dpc(0) && call.routine == 0x1200 && call.context == 0x7000);
        assert(call.argument1 == (uint32_t)(system + delay) && call.argument2 == (uint32_t)((system + delay) >> 32));
        assert(ram[dpc(0) + 2] == 1 && ff_dpc_pop(&queue, 0, dpc(1)) == FF_BOOT_UNSUPPORTED);
        assert(ff_dpc_pop(&queue, 0, call.object) == FF_BOOT_OK && ram[dpc(0) + 2] == 0);
        assert(ff_timers_advance(&queue, start + delay * 2, system + delay * 2) == FF_BOOT_OK && !queue.dpc_count);
    }
}
static void rearm_cancel_and_pending(void)
{
    reset(0, 1000); uint32_t previous = 99, present; ff_dpc_call call;
    assert(ff_timer_set(&queue, timer(0), -10, dpc(0), &previous) == FF_BOOT_OK && previous == 0);
    assert(ff_timer_set(&queue, timer(0), -20, dpc(1), &previous) == FF_BOOT_OK && previous == 1);
    assert(ff_timers_advance(&queue, 10, 1010) == FF_BOOT_OK && !queue.dpc_count);
    assert(ff_timer_cancel(&queue, timer(0), &previous) == FF_BOOT_OK && previous == 1);
    assert(ff_timer_cancel(&queue, timer(0), &previous) == FF_BOOT_OK && previous == 0);
    assert(ff_timers_advance(&queue, 30, 1030) == FF_BOOT_OK && !queue.dpc_count);
    assert(ff_timer_set(&queue, timer(0), 0, dpc(1), &previous) == FF_BOOT_OK && previous == 0);
    assert(ff_timer_cancel(&queue, timer(0), &previous) == FF_BOOT_OK && previous == 0);
    assert(word(timer(0) + 4) == 1); /* Cancellation cannot revoke an already queued callback. */
    assert(ff_dpc_peek(&queue, 1, &call, &present) == FF_BOOT_OK && present && call.object == dpc(1));
    assert(ff_dpc_pop(&queue, 1, call.object) == FF_BOOT_OK);
}
static void clock_changes_and_order(void)
{
    reset(100, 1000); uint32_t previous, present; ff_dpc_call call;
    assert(ff_timer_set(&queue, timer(0), 1100, dpc(0), &previous) == FF_BOOT_OK);
    assert(ff_timer_set(&queue, timer(1), -100, dpc(1), &previous) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, 150, 500) == FF_BOOT_OK); /* Wall clock went back; relative timer is unchanged. */
    assert(wide(timer(0) + 16) == 750 && wide(timer(1) + 16) == 200);
    assert(ff_timers_advance(&queue, 200, 550) == FF_BOOT_OK && queue.dpc_count == 1);
    assert(ff_dpc_peek(&queue, 0, &call, &present) == FF_BOOT_OK && present && call.object == dpc(1));
    assert(ff_dpc_pop(&queue, 0, call.object) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, 201, 1200) == FF_BOOT_OK && !queue.timer_count);
    assert(ff_dpc_peek(&queue, 0, &call, &present) == FF_BOOT_OK && present && call.object == dpc(0));
    assert(call.argument1 == 1200 && !call.argument2 && ram[timer(0) + 1] == 1);
    reset(0, 1000);
    assert(ff_timer_set(&queue, timer(0), -20, dpc(0), &previous) == FF_BOOT_OK);
    assert(ff_timer_set(&queue, timer(1), -10, dpc(1), &previous) == FF_BOOT_OK);
    assert(ff_timer_set(&queue, timer(2), -30, dpc(0), &previous) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, 25, 1025) == FF_BOOT_OK && queue.dpc_count == 2);
    assert(ff_dpc_peek(&queue, 0, &call, &present) == FF_BOOT_OK && call.object == dpc(1));
    assert(ff_dpc_pop(&queue, 0, call.object) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, 30, 1030) == FF_BOOT_OK && queue.dpc_count == 1);
    assert(ff_dpc_peek(&queue, 0, &call, &present) == FF_BOOT_OK && call.object == dpc(0) && call.argument1 == 1025);
}
static void capacity_and_atomic_failure(void)
{
    reset(0, 1000); uint32_t previous;
    for (unsigned i = 0; i < FF_TIMER_CAPACITY; ++i)
        assert(ff_timer_set(&queue, timer(i), -100, 0, &previous) == FF_BOOT_OK && previous == 0);
    assert(ff_timer_set(&queue, timer(0), -200, 0, &previous) == FF_BOOT_OK && previous == 1);
    uint8_t before[sizeof ram]; ff_timer_queue saved = queue; memcpy(before, ram, sizeof ram);
    previous = 99;
    assert(ff_timer_set(&queue, timer(32), -100, 0, &previous) == FF_BOOT_NO_MEMORY && previous == 99);
    assert(!memcmp(before, ram, sizeof ram) && !memcmp(&saved, &queue, sizeof queue));
    reset(0, 1000);
    for (unsigned i = 0; i < FF_DPC_CAPACITY; ++i)
        assert(ff_timer_set(&queue, timer(i), 0, dpc(i), &previous) == FF_BOOT_OK);
    assert(ff_timer_set(&queue, timer(32), -10, dpc(32), &previous) == FF_BOOT_OK);
    saved = queue; memcpy(before, ram, sizeof ram);
    assert(ff_timers_advance(&queue, 10, 1010) == FF_BOOT_NO_MEMORY);
    assert(!memcmp(before, ram, sizeof ram) && !memcmp(&saved, &queue, sizeof queue));
    assert(ff_dpc_pop(&queue, 0, dpc(0)) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, 10, 1010) == FF_BOOT_OK && queue.dpc_count == 32 && !queue.timer_count);
}
static void rejected_states(void)
{
    reset(0, 1000); uint32_t previous = 99;
    assert(ff_timers_advance(&queue, UINT64_MAX - 1, 1000) == FF_BOOT_OK);
    assert(ff_timer_set(&queue, timer(0), -2, dpc(0), &previous) == FF_BOOT_UNSUPPORTED && previous == 99);
    reset(5, 1000);
    assert(ff_timers_advance(&queue, 4, 1001) == FF_BOOT_INVALID && queue.interrupt_time == 5);
    assert(ff_timer_set(&queue, 0, -1, 0, &previous) == FF_BOOT_INVALID);
    assert(ff_timer_set(&queue, timer(0) + 1, -1, 0, &previous) == FF_BOOT_INVALID);
    assert(ff_timer_set(&queue, UINT32_MAX - 3, -1, 0, &previous) == FF_BOOT_INVALID);
    put(timer(0) + 8, timer(0) + 12);
    uint8_t before[sizeof ram]; memcpy(before, ram, sizeof ram); ff_timer_queue saved = queue;
    assert(ff_timer_set(&queue, timer(0), -1, dpc(0), &previous) == FF_BOOT_UNSUPPORTED);
    assert(!memcmp(before, ram, sizeof ram) && !memcmp(&saved, &queue, sizeof queue));
    reset(0, 1000); ram[dpc(0) + 2] = 1;
    assert(ff_timer_set(&queue, timer(0), -1, dpc(0), &previous) == FF_BOOT_UNSUPPORTED);
    reset(0, 1000); put(timer(0) + 36, 10);
    assert(ff_timer_set(&queue, timer(0), -1, dpc(0), &previous) == FF_BOOT_UNSUPPORTED);
    reset(0, 1000); ram[timer(0) + 3] = 1;
    assert(ff_timer_cancel(&queue, timer(0), &previous) == FF_BOOT_UNSUPPORTED);
    reset(0, 1000); put(dpc(0) + 12, 0);
    assert(ff_timer_set(&queue, timer(0), -1, dpc(0), &previous) == FF_BOOT_INVALID);
    reset(0, 1000); assert(ff_timer_set(&queue, timer(0), -1, dpc(0), &previous) == FF_BOOT_OK);
    put(timer(0) + 24, 0);
    assert(ff_timers_advance(&queue, 1, 1001) == FF_BOOT_UNSUPPORTED && !queue.dpc_count);
    reset(0, 1000); put(16, 0);
    assert(ff_timer_set(&queue, timer(0), -1, 0, &previous) == FF_BOOT_UNSUPPORTED);
    assert(ff_timers_init(&queue, ram, sizeof ram, 16, 20) == FF_BOOT_INVALID);
    assert(ff_timers_init(&queue, ram, sizeof ram, 0, 24) == FF_BOOT_INVALID);
    reset(0, 1000);
    assert(ff_timer_set(&queue, timer(0), INT64_MIN, 0, &previous) == FF_BOOT_OK);
    assert(wide(timer(0) + 16) == (UINT64_C(1) << 63));
}
int main(void)
{
    six_hour_boundary(); rearm_cancel_and_pending(); clock_changes_and_order();
    capacity_and_atomic_failure(); rejected_states();
    puts("One-shot timers: exact expiry, 64-bit clocks, wall-clock changes, rearm/cancel, ordered/deduplicated DPC arguments, IRQL deferral, capacity and rejected-state checks passed");
    return 0;
}
