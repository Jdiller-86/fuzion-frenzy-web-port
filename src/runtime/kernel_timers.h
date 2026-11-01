#ifndef FF_KERNEL_TIMERS_H
#define FF_KERNEL_TIMERS_H
#include <stdint.h>

#define FF_TIMER_CAPACITY 32u
#define FF_DPC_CAPACITY 32u
typedef struct {
    uint32_t object, dpc, absolute;
    uint64_t deadline, system_due;
} ff_timer_entry;
typedef struct {
    uint32_t object, routine, context, argument1, argument2;
} ff_dpc_call;
typedef struct {
    uint8_t *ram;
    uint32_t bytes, timer_head, dpc_head, timer_count, dpc_count, clock_ready;
    uint64_t interrupt_time, system_time;
    ff_timer_entry timers[FF_TIMER_CAPACITY];
    uint32_t dpcs[FF_DPC_CAPACITY];
} ff_timer_queue;

/* Single-worker, one-shot timers with empty wait lists only. All times are
 * explicit 100 ns samples; the system clock uses the January 1601 UTC epoch.
 * The caller owns the two reserved guest list heads, eight bytes each.
 * Unsupported state fails rather than dropping timers, waiters or callbacks. */
uint32_t ff_timers_init(ff_timer_queue *, uint8_t *ram, uint32_t bytes, uint32_t timer_head, uint32_t dpc_head);
uint32_t ff_timers_advance(ff_timer_queue *, uint64_t interrupt_time, uint64_t system_time);
uint32_t ff_timer_set(ff_timer_queue *, uint32_t timer, int64_t due_time, uint32_t dpc, uint32_t *was_inserted);
uint32_t ff_timer_cancel(ff_timer_queue *, uint32_t timer, uint32_t *was_inserted);
/* Peek before admitting the callback's executable address and CPU/stack/IRQL
 * context. Pop only after admission. Neither operation executes guest code.
 * At IRQL >= DISPATCH_LEVEL (2), dequeue is deferred to prevent nesting. */
uint32_t ff_dpc_peek(const ff_timer_queue *, uint32_t irql, ff_dpc_call *, uint32_t *present);
uint32_t ff_dpc_pop(ff_timer_queue *, uint32_t irql, uint32_t expected_object);
#endif
