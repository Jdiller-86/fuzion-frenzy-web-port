#include "../runtime/core/performance_clock.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static ff_performance_clock state;

static void expect_read(uint32_t frequency, uint64_t ticks, uint64_t expected, uint32_t site)
{
    uint64_t output = UINT64_C(0xfeedfacecafebeef);
    assert(ff_performance_clock_init(&state, frequency) == FF_PERFORMANCE_CLOCK_OK);
    assert(ff_performance_clock_read(&state, ticks, site, &output) == FF_PERFORMANCE_CLOCK_OK);
    assert(output == expected);
    assert(state.count == 1 && state.last_ticks == ticks);
    assert(state.samples[0].site == site);
    assert(state.samples[0].interrupt_ticks == ticks);
    assert(state.samples[0].counter == expected);
}

static void vectors(void)
{
    expect_read(10000000u, UINT64_C(10000000), UINT64_C(10000000), 1u);
    expect_read(733333333u, UINT64_C(1234567), UINT64_C(90534913), 2u);
    expect_read(1u, UINT64_C(10000000), UINT64_C(1), 3u);
    expect_read(10000000u, UINT64_C(987654321), UINT64_C(987654321), 4u);
    expect_read(10000000u, UINT64_MAX, UINT64_MAX, 7u);
    expect_read(1000000000u, UINT64_C(10000000), UINT64_C(1000000000), 5u);
    expect_read(733333333u, UINT64_C(251546510210378663),
        UINT64_C(18446744073709551612), 6u);
}

static void atomic_failure(ff_performance_clock_result expected, uint64_t ticks,
    uint32_t site, uint64_t *out, uint32_t initial_count)
{
    ff_performance_clock before = state;
    uint64_t output_before = *out;
    assert(state.count == initial_count);
    assert(ff_performance_clock_read(&state, ticks, site, out) == expected);
    assert(memcmp(&state, &before, sizeof state) == 0);
    assert(*out == output_before);
}

static void refusals(void)
{
    ff_performance_clock before;
    uint64_t output = UINT64_C(0x0123456789abcdef);
    unsigned i;

    memset(&state, 0x5a, sizeof state);
    before = state;
    assert(ff_performance_clock_init(&state, 0u) == FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&state, &before, sizeof state) == 0);
    assert(ff_performance_clock_init(&state, 1000000001u) == FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&state, &before, sizeof state) == 0);
    assert(ff_performance_clock_init(NULL, 1u) == FF_PERFORMANCE_CLOCK_INVALID);

    assert(ff_performance_clock_init(&state, 1000000000u) == FF_PERFORMANCE_CLOCK_OK);
    atomic_failure(FF_PERFORMANCE_CLOCK_OVERFLOW, UINT64_MAX, 9u, &output, 0u);

    assert(ff_performance_clock_read(&state, 11u, 1u, &output) == FF_PERFORMANCE_CLOCK_OK);
    assert(output == 1100u);
    atomic_failure(FF_PERFORMANCE_CLOCK_ORDER, 10u, 2u, &output, 1u);
    atomic_failure(FF_PERFORMANCE_CLOCK_INVALID, 12u, 0u, &output, 1u);
    before = state;
    assert(ff_performance_clock_read(&state, 12u, 2u, NULL) == FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&state, &before, sizeof state) == 0);
    assert(ff_performance_clock_read(&state, 12u, 2u, &state.last_ticks) == FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&state, &before, sizeof state) == 0);
    assert(ff_performance_clock_read(&state, 12u, 2u, &state.samples[0].counter) == FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&state, &before, sizeof state) == 0);
    assert(state.count == 1 && state.last_ticks == 11u);

    /* Equal timestamps are accepted and retain call-site journal order. */
    assert(ff_performance_clock_read(&state, 11u, 3u, &output) == FF_PERFORMANCE_CLOCK_OK);
    assert(output == 1100u && state.count == 2 && state.samples[1].site == 3u);
    for (i = 2; i < FF_PERFORMANCE_CLOCK_CAPACITY; ++i)
        assert(ff_performance_clock_read(&state, 11u, i + 2u, &output) == FF_PERFORMANCE_CLOCK_OK);
    assert(state.count == FF_PERFORMANCE_CLOCK_CAPACITY);
    atomic_failure(FF_PERFORMANCE_CLOCK_CAPACITY_REACHED, 11u, 99u, &output,
        FF_PERFORMANCE_CLOCK_CAPACITY);
    assert(ff_performance_clock_read(NULL, 0u, 1u, &output) == FF_PERFORMANCE_CLOCK_INVALID);

    assert(ff_performance_clock_init(&state, 733333333u) == FF_PERFORMANCE_CLOCK_OK);
    atomic_failure(FF_PERFORMANCE_CLOCK_OVERFLOW, UINT64_C(251546510210378664), 7u, &output, 0u);
    puts("Synthetic performance clock arithmetic, journal, bounds and atomic refusal checks passed");
}

int main(void)
{
    vectors();
    refusals();
    return 0;
}
