#include "time_source.h"

void ff_time_source_reset(ff_time_source *state)
{
    if (!state) return;
    state->started = 0;
    state->origin = 0;
}

uint64_t ff_time_source_frequency(void)
{
    return FF_TIME_SOURCE_HZ;
}

uint64_t ff_time_source_scale(uint64_t system_100ns)
{
    /* Split scaling keeps every intermediate inside 64 bits; the remainder is
     * below one second, so rem * HZ cannot overflow. */
    uint64_t seconds = system_100ns / FF_TIME_SOURCE_100NS;
    uint64_t remainder = system_100ns % FF_TIME_SOURCE_100NS;
    return seconds * FF_TIME_SOURCE_HZ + (remainder * FF_TIME_SOURCE_HZ) / FF_TIME_SOURCE_100NS;
}

uint64_t ff_time_source_tsc(ff_time_source *state, uint64_t system_100ns)
{
    if (!state) return ff_time_source_scale(system_100ns);
    if (!state->started) {
        state->started = 1;
        state->origin = system_100ns;
        return 0;
    }
    /* Scale the delta from the first sample so callers see a linear counter;
     * a non-monotonic sample never runs the counter backwards. */
    if (system_100ns < state->origin) return 0;
    return ff_time_source_scale(system_100ns - state->origin);
}
