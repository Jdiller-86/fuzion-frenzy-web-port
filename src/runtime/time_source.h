#ifndef FF_TIME_SOURCE_H
#define FF_TIME_SOURCE_H

#include <stdint.h>

/* Counter and frequency contract for the guest's performance clock.
 *
 * The pinned translator documents the Xbox pair: `rdtsc` is the performance
 * counter and the frequency is the console CPU clock, 733,333,333 Hz, which a
 * frame timer divides the counter by. The admitted browser clock supplies
 * 100 ns units instead of a host frequency, so this module scales the sampled
 * system time into that console rate. The counter starts at zero on its first
 * read and is monotonic for a monotonic input. */
#define FF_TIME_SOURCE_HZ 733333333ull
#define FF_TIME_SOURCE_100NS 10000000ull

typedef struct
{
    uint32_t started;
    uint64_t origin; /* first sampled system time, in 100 ns units */
} ff_time_source;

void ff_time_source_reset(ff_time_source *state);
uint64_t ff_time_source_frequency(void);
uint64_t ff_time_source_scale(uint64_t system_100ns);
uint64_t ff_time_source_tsc(ff_time_source *state, uint64_t system_100ns);

#endif
