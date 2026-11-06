/* Owned performance-clock contract: the console counter and its frequency. */
#include "../src/runtime/time_source.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    ff_time_source state;
    ff_time_source_reset(&state);

    /* The documented Xbox pair: a counter and the 733,333,333 Hz frequency. */
    assert(ff_time_source_frequency() == 733333333ull);
    assert(ff_time_source_scale(0) == 0);
    assert(ff_time_source_scale(FF_TIME_SOURCE_100NS) == 733333333ull);
    assert(ff_time_source_scale(2 * FF_TIME_SOURCE_100NS) == 2 * 733333333ull);
    assert(ff_time_source_scale(FF_TIME_SOURCE_100NS / 2) == 366666666ull);
    assert(ff_time_source_scale(FF_TIME_SOURCE_100NS / 4) == 183333333ull);

    /* The first read establishes the origin, so callers see a counter that
     * starts at zero and advances with the sampled system time. */
    const uint64_t base = 134384413252940000ull;
    assert(ff_time_source_tsc(&state, base) == 0);
    assert(ff_time_source_tsc(&state, base + FF_TIME_SOURCE_100NS) == 733333333ull);
    assert(ff_time_source_tsc(&state, base + FF_TIME_SOURCE_100NS / 2) == 366666666ull);

    /* No overflow at the largest representable sampled time. */
    ff_time_source_reset(&state);
    const uint64_t huge = 0xffffffffffffffffull;
    const uint64_t scaled = ff_time_source_scale(huge);
    assert(scaled > 0 && scaled == (huge / FF_TIME_SOURCE_100NS) * FF_TIME_SOURCE_HZ +
        ((huge % FF_TIME_SOURCE_100NS) * FF_TIME_SOURCE_HZ) / FF_TIME_SOURCE_100NS);

    /* Reset forgets the origin; a missing state degrades to the scaled value. */
    ff_time_source_reset(&state);
    assert(!state.started && state.origin == 0);
    assert(ff_time_source_scale(FF_TIME_SOURCE_100NS) == 733333333ull);

    puts("Owned time-source frequency, scaling and origin checks passed");
    return 0;
}
