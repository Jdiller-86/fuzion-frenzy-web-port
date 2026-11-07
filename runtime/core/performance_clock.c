#include "performance_clock.h"

#include <limits.h>
#include <string.h>

#define FF_TICKS_PER_SECOND UINT64_C(10000000)

ff_performance_clock_result ff_performance_clock_init(ff_performance_clock *state, uint32_t frequency_hz)
{
    if (!state || frequency_hz < 1u || frequency_hz > 1000000000u)
        return FF_PERFORMANCE_CLOCK_INVALID;
    memset(state, 0, sizeof *state);
    state->frequency_hz = frequency_hz;
    state->ready = 1u;
    return FF_PERFORMANCE_CLOCK_OK;
}

ff_performance_clock_result ff_performance_clock_scale(uint32_t frequency_hz,
    uint64_t interrupt_ticks, uint64_t *out_counter)
{
    uint64_t quotient, remainder, whole, fraction, counter;

    if (!out_counter || frequency_hz < 1u || frequency_hz > 1000000000u)
        return FF_PERFORMANCE_CLOCK_INVALID;
    quotient = interrupt_ticks / FF_TICKS_PER_SECOND;
    remainder = interrupt_ticks % FF_TICKS_PER_SECOND;
    if (quotient && frequency_hz > UINT64_MAX / quotient)
        return FF_PERFORMANCE_CLOCK_OVERFLOW;
    whole = quotient * frequency_hz;
    fraction = (remainder * frequency_hz) / FF_TICKS_PER_SECOND;
    if (fraction > UINT64_MAX - whole)
        return FF_PERFORMANCE_CLOCK_OVERFLOW;
    counter = whole + fraction;
    *out_counter = counter;
    return FF_PERFORMANCE_CLOCK_OK;
}

ff_performance_clock_result ff_performance_clock_read(ff_performance_clock *state,
    uint64_t interrupt_ticks, uint32_t site, uint64_t *out_counter)
{
    uintptr_t state_begin, state_end, out_begin, out_end;
    uint64_t counter;
    ff_performance_clock_result scale_result;

    if (!state || !out_counter || !site || !state->ready ||
        state->frequency_hz < 1u || state->frequency_hz > 1000000000u ||
        state->count > FF_PERFORMANCE_CLOCK_CAPACITY)
        return FF_PERFORMANCE_CLOCK_INVALID;
    state_begin = (uintptr_t)(void *)state;
    out_begin = (uintptr_t)(void *)out_counter;
    if (state_begin > UINTPTR_MAX - sizeof *state ||
        out_begin > UINTPTR_MAX - sizeof *out_counter)
        return FF_PERFORMANCE_CLOCK_INVALID;
    state_end = state_begin + sizeof *state;
    out_end = out_begin + sizeof *out_counter;
    if (state_begin < out_end && out_begin < state_end)
        return FF_PERFORMANCE_CLOCK_INVALID;
    if (interrupt_ticks < state->last_ticks)
        return FF_PERFORMANCE_CLOCK_ORDER;
    if (state->count == FF_PERFORMANCE_CLOCK_CAPACITY)
        return FF_PERFORMANCE_CLOCK_CAPACITY_REACHED;

    scale_result = ff_performance_clock_scale(state->frequency_hz, interrupt_ticks, &counter);
    if (scale_result != FF_PERFORMANCE_CLOCK_OK)
        return scale_result;

    state->samples[state->count].site = site;
    state->samples[state->count].interrupt_ticks = interrupt_ticks;
    state->samples[state->count].counter = counter;
    ++state->count;
    state->last_ticks = interrupt_ticks;
    *out_counter = counter;
    return FF_PERFORMANCE_CLOCK_OK;
}
