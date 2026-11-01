#include "kernel_timers.h"
#include "thread_bootstrap.h"
#include <string.h>

static int fits(const ff_timer_queue *q, uint32_t address, uint32_t size)
{
    return q && q->ram && address && !(address & 3u) && size <= q->bytes && address <= q->bytes - size;
}
static int overlaps(uint32_t a, uint32_t an, uint32_t b, uint32_t bn)
{
    return (uint64_t)a < (uint64_t)b + bn && (uint64_t)b < (uint64_t)a + an;
}
static uint32_t read32(const ff_timer_queue *q, uint32_t address)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= (uint32_t)q->ram[address + i] << (i * 8);
    return value;
}
static uint64_t read64(const ff_timer_queue *q, uint32_t address)
{
    return read32(q, address) | (uint64_t)read32(q, address + 4) << 32;
}
static void write32(ff_timer_queue *q, uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) q->ram[address + i] = (uint8_t)(value >> (i * 8));
}
static void write64(ff_timer_queue *q, uint32_t address, uint64_t value)
{
    write32(q, address, (uint32_t)value); write32(q, address + 4, (uint32_t)(value >> 32));
}
static int timer_index(const ff_timer_queue *q, uint32_t object)
{
    for (uint32_t i = 0; i < q->timer_count; ++i) if (q->timers[i].object == object) return (int)i;
    return -1;
}
static int dpc_index(const ff_timer_queue *q, uint32_t object)
{
    for (uint32_t i = 0; i < q->dpc_count; ++i) if (q->dpcs[i] == object) return (int)i;
    return -1;
}
static int object_range(const ff_timer_queue *q, uint32_t address, uint32_t size)
{
    if (!fits(q, address, size) || overlaps(address, size, q->timer_head, 8) ||
        overlaps(address, size, q->dpc_head, 8)) return 0;
    for (uint32_t i = 0; i < q->timer_count; ++i) {
        const ff_timer_entry *t = &q->timers[i];
        if (!(size == 40 && address == t->object) && overlaps(address, size, t->object, 40)) return 0;
        if (t->dpc && !(size == 28 && address == t->dpc) && overlaps(address, size, t->dpc, 28)) return 0;
    }
    for (uint32_t i = 0; i < q->dpc_count; ++i) if (!(size == 28 && address == q->dpcs[i]) &&
        overlaps(address, size, q->dpcs[i], 28)) return 0;
    return 1;
}
static uint32_t timer_valid(const ff_timer_queue *q, uint32_t address)
{
    if (!object_range(q, address, 40) || (q->ram[address] != 8 && q->ram[address] != 9) ||
        q->ram[address + 2] != 10) return FF_BOOT_INVALID;
    if (read32(q, address + 8) != address + 8 || read32(q, address + 12) != address + 8 ||
        read32(q, address + 36)) return FF_BOOT_UNSUPPORTED;
    return FF_BOOT_OK;
}
static uint32_t dpc_valid(const ff_timer_queue *q, uint32_t address)
{
    if (!object_range(q, address, 28) || q->ram[address] != 0x13 || q->ram[address + 1] ||
        !read32(q, address + 12)) return FF_BOOT_INVALID;
    return q->ram[address + 2] == (dpc_index(q, address) >= 0) ? FF_BOOT_OK : FF_BOOT_UNSUPPORTED;
}
static uint32_t validate(const ff_timer_queue *q)
{
    if (!q || q->timer_count > FF_TIMER_CAPACITY || q->dpc_count > FF_DPC_CAPACITY ||
        !fits(q, q->timer_head, 8) || !fits(q, q->dpc_head, 8) ||
        overlaps(q->timer_head, 8, q->dpc_head, 8)) return FF_BOOT_INVALID;
    for (uint32_t i = 0; i < q->timer_count; ++i) {
        const ff_timer_entry *t = &q->timers[i];
        uint32_t status = timer_valid(q, t->object);
        if (status != FF_BOOT_OK) return status;
        if (q->ram[t->object + 3] != 1 || q->ram[t->object + 1] != t->absolute ||
            read64(q, t->object + 16) != t->deadline || read32(q, t->object + 32) != t->dpc ||
            (i && q->timers[i - 1].deadline > t->deadline)) return FF_BOOT_UNSUPPORTED;
        if (t->dpc && (status = dpc_valid(q, t->dpc)) != FF_BOOT_OK) return status;
        uint32_t next = i + 1 < q->timer_count ? q->timers[i + 1].object + 24 : q->timer_head;
        uint32_t previous = i ? q->timers[i - 1].object + 24 : q->timer_head;
        if (read32(q, t->object + 24) != next || read32(q, t->object + 28) != previous) return FF_BOOT_UNSUPPORTED;
        for (uint32_t j = 0; j < i; ++j) if (q->timers[j].object == t->object) return FF_BOOT_UNSUPPORTED;
    }
    for (uint32_t i = 0; i < q->dpc_count; ++i) {
        uint32_t status = dpc_valid(q, q->dpcs[i]);
        if (status != FF_BOOT_OK) return status;
        uint32_t next = i + 1 < q->dpc_count ? q->dpcs[i + 1] + 4 : q->dpc_head;
        uint32_t previous = i ? q->dpcs[i - 1] + 4 : q->dpc_head;
        if (read32(q, q->dpcs[i] + 4) != next || read32(q, q->dpcs[i] + 8) != previous) return FF_BOOT_UNSUPPORTED;
        for (uint32_t j = 0; j < i; ++j) if (q->dpcs[j] == q->dpcs[i]) return FF_BOOT_UNSUPPORTED;
    }
    if (read32(q, q->timer_head) != (q->timer_count ? q->timers[0].object + 24 : q->timer_head) ||
        read32(q, q->timer_head + 4) != (q->timer_count ? q->timers[q->timer_count - 1].object + 24 : q->timer_head) ||
        read32(q, q->dpc_head) != (q->dpc_count ? q->dpcs[0] + 4 : q->dpc_head) ||
        read32(q, q->dpc_head + 4) != (q->dpc_count ? q->dpcs[q->dpc_count - 1] + 4 : q->dpc_head)) return FF_BOOT_UNSUPPORTED;
    return FF_BOOT_OK;
}
static void relink(ff_timer_queue *q)
{
    /* Stable deadline order; this is an owned queue, not a copy of kernel buckets. */
    for (uint32_t i = 1; i < q->timer_count; ++i) {
        ff_timer_entry t = q->timers[i]; uint32_t j = i;
        while (j && q->timers[j - 1].deadline > t.deadline) { q->timers[j] = q->timers[j - 1]; --j; }
        q->timers[j] = t;
    }
    write32(q, q->timer_head, q->timer_count ? q->timers[0].object + 24 : q->timer_head);
    write32(q, q->timer_head + 4, q->timer_count ? q->timers[q->timer_count - 1].object + 24 : q->timer_head);
    for (uint32_t i = 0; i < q->timer_count; ++i) {
        write64(q, q->timers[i].object + 16, q->timers[i].deadline);
        write32(q, q->timers[i].object + 24, i + 1 < q->timer_count ? q->timers[i + 1].object + 24 : q->timer_head);
        write32(q, q->timers[i].object + 28, i ? q->timers[i - 1].object + 24 : q->timer_head);
    }
    write32(q, q->dpc_head, q->dpc_count ? q->dpcs[0] + 4 : q->dpc_head);
    write32(q, q->dpc_head + 4, q->dpc_count ? q->dpcs[q->dpc_count - 1] + 4 : q->dpc_head);
    for (uint32_t i = 0; i < q->dpc_count; ++i) {
        write32(q, q->dpcs[i] + 4, i + 1 < q->dpc_count ? q->dpcs[i + 1] + 4 : q->dpc_head);
        write32(q, q->dpcs[i] + 8, i ? q->dpcs[i - 1] + 4 : q->dpc_head);
    }
}
static void remove_timer(ff_timer_queue *q, uint32_t i)
{
    q->ram[q->timers[i].object + 3] = 0;
    --q->timer_count;
    memmove(q->timers + i, q->timers + i + 1, (q->timer_count - i) * sizeof *q->timers);
}
static void queue_dpc(ff_timer_queue *q, uint32_t address)
{
    if (!address || dpc_index(q, address) >= 0) return;
    q->dpcs[q->dpc_count++] = address; q->ram[address + 2] = 1;
    write32(q, address + 20, (uint32_t)q->system_time);
    write32(q, address + 24, (uint32_t)(q->system_time >> 32));
}
static int future_deadline(uint64_t now, uint64_t delay, uint64_t *result)
{
    if (delay > UINT64_MAX - now) return 0;
    *result = now + delay; return 1;
}
uint32_t ff_timers_init(ff_timer_queue *q, uint8_t *ram, uint32_t bytes, uint32_t timer_head, uint32_t dpc_head)
{
    ff_timer_queue candidate = { .ram = ram, .bytes = bytes, .timer_head = timer_head, .dpc_head = dpc_head };
    if (!q || !fits(&candidate, timer_head, 8) || !fits(&candidate, dpc_head, 8) ||
        overlaps(timer_head, 8, dpc_head, 8)) return FF_BOOT_INVALID;
    *q = candidate; relink(q); return FF_BOOT_OK;
}
uint32_t ff_timer_set(ff_timer_queue *q, uint32_t timer, int64_t due, uint32_t dpc, uint32_t *was_inserted)
{
    uint32_t status = validate(q);
    if (status != FF_BOOT_OK) return status;
    if (!was_inserted || !q->clock_ready) return FF_BOOT_INVALID;
    if ((status = timer_valid(q, timer)) != FF_BOOT_OK) return status;
    int old = timer_index(q, timer);
    if (q->ram[timer + 3] != (old >= 0)) return FF_BOOT_UNSUPPORTED;
    if (dpc && ((status = dpc_valid(q, dpc)) != FF_BOOT_OK || overlaps(timer, 40, dpc, 28)))
        return status == FF_BOOT_OK ? FF_BOOT_INVALID : status;
    uint64_t deadline = 0;
    uint64_t delay = due < 0 ? 0u - (uint64_t)due : (uint64_t)due > q->system_time ? (uint64_t)due - q->system_time : 0;
    if (delay && !future_deadline(q->interrupt_time, delay, &deadline)) return FF_BOOT_UNSUPPORTED;
    if ((delay && old < 0 && q->timer_count == FF_TIMER_CAPACITY) ||
        (!delay && dpc && dpc_index(q, dpc) < 0 && q->dpc_count == FF_DPC_CAPACITY)) return FF_BOOT_NO_MEMORY;
    if (old >= 0) remove_timer(q, (uint32_t)old);
    q->ram[timer + 1] = due >= 0; q->ram[timer + 3] = delay != 0;
    write32(q, timer + 4, delay ? 0 : 1); write32(q, timer + 32, dpc); write32(q, timer + 36, 0);
    write64(q, timer + 16, deadline);
    if (delay) q->timers[q->timer_count++] = (ff_timer_entry){ .object = timer, .dpc = dpc,
        .absolute = due >= 0, .deadline = deadline, .system_due = due >= 0 ? (uint64_t)due : 0 };
    else { write32(q, timer + 24, 0); queue_dpc(q, dpc); }
    relink(q); *was_inserted = old >= 0; return FF_BOOT_OK;
}
uint32_t ff_timer_cancel(ff_timer_queue *q, uint32_t timer, uint32_t *was_inserted)
{
    uint32_t status = validate(q);
    if (status != FF_BOOT_OK) return status;
    if (!was_inserted) return FF_BOOT_INVALID;
    if ((status = timer_valid(q, timer)) != FF_BOOT_OK) return status;
    int old = timer_index(q, timer);
    if (q->ram[timer + 3] != (old >= 0)) return FF_BOOT_UNSUPPORTED;
    if (old >= 0) remove_timer(q, (uint32_t)old);
    relink(q); *was_inserted = old >= 0; return FF_BOOT_OK;
}
uint32_t ff_timers_advance(ff_timer_queue *q, uint64_t interrupt_time, uint64_t system_time)
{
    uint32_t status = validate(q);
    if (status != FF_BOOT_OK) return status;
    if (q->clock_ready && interrupt_time < q->interrupt_time) return FF_BOOT_INVALID;
    uint64_t deadlines[FF_TIMER_CAPACITY];
    uint32_t new_dpcs[FF_TIMER_CAPACITY], new_count = 0;
    for (uint32_t i = 0; i < q->timer_count; ++i) {
        ff_timer_entry *t = &q->timers[i]; deadlines[i] = t->deadline;
        if (t->absolute) {
            uint64_t delay = t->system_due > system_time ? t->system_due - system_time : 0;
            if (delay && !future_deadline(interrupt_time, delay, &deadlines[i])) return FF_BOOT_UNSUPPORTED;
            if (!delay) deadlines[i] = 0;
        }
        if (deadlines[i] <= interrupt_time && t->dpc && dpc_index(q, t->dpc) < 0) {
            uint32_t j = 0; while (j < new_count && new_dpcs[j] != t->dpc) ++j;
            if (j == new_count) new_dpcs[new_count++] = t->dpc;
        }
    }
    if (new_count > FF_DPC_CAPACITY - q->dpc_count) return FF_BOOT_NO_MEMORY;
    q->interrupt_time = interrupt_time; q->system_time = system_time; q->clock_ready = 1;
    for (uint32_t i = 0; i < q->timer_count; ++i) q->timers[i].deadline = deadlines[i];
    relink(q);
    while (q->timer_count && q->timers[0].deadline <= interrupt_time) {
        ff_timer_entry t = q->timers[0]; remove_timer(q, 0);
        write32(q, t.object + 4, 1); write32(q, t.object + 24, 0); queue_dpc(q, t.dpc);
    }
    relink(q); return FF_BOOT_OK;
}
uint32_t ff_dpc_peek(const ff_timer_queue *q, uint32_t irql, ff_dpc_call *call, uint32_t *present)
{
    uint32_t status = validate(q);
    if (status != FF_BOOT_OK) return status;
    if (!call || !present || irql > 31) return FF_BOOT_INVALID;
    *present = q->dpc_count && irql < 2;
    if (*present) {
        uint32_t address = q->dpcs[0];
        *call = (ff_dpc_call){address, read32(q, address + 12), read32(q, address + 16),
            read32(q, address + 20), read32(q, address + 24)};
    }
    return FF_BOOT_OK;
}
uint32_t ff_dpc_pop(ff_timer_queue *q, uint32_t irql, uint32_t expected_object)
{
    ff_dpc_call call; uint32_t present;
    uint32_t status = ff_dpc_peek(q, irql, &call, &present);
    if (status != FF_BOOT_OK) return status;
    if (!present || call.object != expected_object) return FF_BOOT_UNSUPPORTED;
    q->ram[call.object + 2] = 0; --q->dpc_count;
    memmove(q->dpcs, q->dpcs + 1, q->dpc_count * sizeof *q->dpcs);
    relink(q); return FF_BOOT_OK;
}
