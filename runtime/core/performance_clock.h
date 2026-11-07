#ifndef FF_PERFORMANCE_CLOCK_H
#define FF_PERFORMANCE_CLOCK_H

#include <stdint.h>

#define FF_PERFORMANCE_CLOCK_CAPACITY 64u

typedef enum {
    FF_PERFORMANCE_CLOCK_OK = 0,
    FF_PERFORMANCE_CLOCK_INVALID,
    FF_PERFORMANCE_CLOCK_OVERFLOW,
    FF_PERFORMANCE_CLOCK_ORDER,
    FF_PERFORMANCE_CLOCK_CAPACITY_REACHED
} ff_performance_clock_result;

typedef struct {
    uint32_t site;
    uint64_t interrupt_ticks;
    uint64_t counter;
} ff_performance_clock_sample;

typedef struct {
    uint32_t ready;
    uint32_t frequency_hz;
    uint32_t count;
    uint64_t last_ticks;
    ff_performance_clock_sample samples[FF_PERFORMANCE_CLOCK_CAPACITY];
} ff_performance_clock;

/* Explicit synthetic 100 ns ticks from a shared zero origin; no hardware
 * frequency is inferred or supplied by default. */
ff_performance_clock_result ff_performance_clock_init(ff_performance_clock *state, uint32_t frequency_hz);
/* out_counter must not overlap state; rejection preserves both. */
ff_performance_clock_result ff_performance_clock_read(ff_performance_clock *state,
    uint64_t interrupt_ticks, uint32_t site, uint64_t *out_counter);

#endif
