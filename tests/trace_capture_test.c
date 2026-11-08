#include "trace_capture.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void digest(const ff_trace_capture *capture, uint8_t out[32])
{
    ff_sha256_final(&capture->digest, out);
}

static void expected_row_hash(void)
{
    ff_trace_capture capture;
    uint8_t actual[32];
    static const uint8_t expected[32] = {
        0x8b,0x1c,0x9a,0xb8,0x84,0xba,0xeb,0x07,
        0x16,0xe5,0x2f,0xe9,0xae,0x55,0x37,0xb3,
        0xff,0xb9,0x7c,0x14,0x4d,0xd7,0x62,0xb0,
        0x9b,0x82,0x83,0x8d,0x1e,0x40,0x4b,0x78
    };
    assert(ff_trace_capture_init(&capture, 0) == FF_TRACE_CAPTURE_OK);
    assert(ff_trace_capture_add(&capture, 1, 0x12345678u, 0x9abcdef0u,
                                0x01020304u, 0xaabbccddu, 0x76543210u) == FF_TRACE_CAPTURE_OK);
    digest(&capture, actual);
    assert(!memcmp(actual, expected, sizeof expected));
}

static void windows_and_prefix(void)
{
    ff_trace_capture full, selected;
    uint8_t first_window[32], current[32];
    assert(!ff_trace_capture_init(&full, 0));
    assert(!ff_trace_capture_init(&selected, FF_TRACE_CAPTURE_MAX_START));

    for (uint32_t i = 0; i < FF_TRACE_CAPTURE_ROWS; ++i) {
        uint32_t kind = (i % 3u) ? 2u : 1u;
        uint32_t address = kind == 1u ? 0x1000u + (i % 17u) : 0x8000u + i;
        assert(ff_trace_capture_add(&full, kind, address, i + 10u, i + 20u,
                                    i + 30u, i + 40u) == FF_TRACE_CAPTURE_OK);
        assert(ff_trace_capture_add(&selected, kind, address, i + 10u, i + 20u,
                                    i + 30u, i + 40u) == FF_TRACE_CAPTURE_OK);
    }
    assert(full.total == FF_TRACE_CAPTURE_ROWS && full.captured_start == 0 &&
           full.prefix_entries == 0 && full.prefix_routines == 0);
    assert(selected.total == FF_TRACE_CAPTURE_ROWS && selected.captured_start == 0);
    digest(&full, first_window);
    digest(&selected, current);
    assert(!memcmp(first_window, current, sizeof first_window));
    ff_trace_capture before = full;
    assert(ff_trace_capture_add(&full, 2, 9, 8, 7, 6, 5) == FF_TRACE_CAPTURE_FULL);
    assert(!memcmp(&before, &full, sizeof full));

    assert(ff_trace_capture_add(&selected, 2, 9, 8, 7, 6, 5) == FF_TRACE_CAPTURE_ROTATE);
    assert(selected.captured_start == FF_TRACE_CAPTURE_MAX_START &&
           selected.prefix_entries == full.entries &&
           selected.prefix_routines == full.routine_count &&
           !memcmp(selected.prefix_digest, first_window, sizeof first_window));
    for (uint32_t i = FF_TRACE_CAPTURE_ROWS + 1u;
         i < 2u * FF_TRACE_CAPTURE_ROWS; ++i) {
        uint32_t kind = (i & 1u) ? 1u : 2u;
        uint32_t address = kind == 1u ? 0x1000u + (i % 17u) : 0x9000u + i;
        assert(ff_trace_capture_add(&selected, kind, address, i, i + 1u,
                                    i + 2u, i + 3u) == FF_TRACE_CAPTURE_OK);
    }
    assert(selected.total == 2u * FF_TRACE_CAPTURE_ROWS);
    before = selected;
    assert(ff_trace_capture_add(&selected, 1, 0x1001u, 0, 0, 0, 0) == FF_TRACE_CAPTURE_FULL);
    assert(!memcmp(&before, &selected, sizeof selected));
}

static void entries_and_atomic_rejection(void)
{
    ff_trace_capture capture, before;
    assert(!ff_trace_capture_init(&capture, 0));
    assert(ff_trace_capture_add(&capture, 2, 1, 0, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    assert(capture.entries == 0 && capture.routine_count == 0);
    assert(ff_trace_capture_add(&capture, 1, 2, 0, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    assert(ff_trace_capture_add(&capture, 1, 2, 1, 1, 1, 1) == FF_TRACE_CAPTURE_OK);
    assert(ff_trace_capture_add(&capture, 1, 3, 0, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    assert(capture.entries == 3 && capture.routine_count == 2 &&
           capture.routines[0] == 2 && capture.routines[1] == 3);

    before = capture;
    assert(ff_trace_capture_add(&capture, 3, 4, 0, 0, 0, 0) == FF_TRACE_CAPTURE_INVALID);
    assert(!memcmp(&before, &capture, sizeof capture));
    assert(ff_trace_capture_add(&capture, 1, 0, 0, 0, 0, 0) == FF_TRACE_CAPTURE_INVALID);
    assert(!memcmp(&before, &capture, sizeof capture));

    for (uint32_t i = 0; i < FF_TRACE_CAPTURE_ROUTINES - 2u; ++i)
        assert(ff_trace_capture_add(&capture, 1, 0x100u + i, 0, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    assert(capture.routine_count == FF_TRACE_CAPTURE_ROUTINES);
    before = capture;
    assert(ff_trace_capture_add(&capture, 1, 0xdeadbeefu, 0, 0, 0, 0) == FF_TRACE_CAPTURE_FULL);
    assert(!memcmp(&before, &capture, sizeof capture));
    assert(ff_trace_capture_add(&capture, 1, capture.routines[0], 0, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    assert(capture.routine_count == FF_TRACE_CAPTURE_ROUTINES);

    before = capture;
    assert(ff_trace_capture_init(&capture, 1) == FF_TRACE_CAPTURE_INVALID);
    assert(!memcmp(&before, &capture, sizeof capture));
}

static void start_validation_and_default_horizon(void)
{
    ff_trace_capture capture, before;
    assert(!ff_trace_capture_init(&capture, 0));
    before = capture;
    assert(ff_trace_capture_init(&capture, FF_TRACE_CAPTURE_MAX_START + 1u) == FF_TRACE_CAPTURE_INVALID);
    assert(!memcmp(&before, &capture, sizeof capture));
    for (uint32_t i = 0; i < FF_TRACE_CAPTURE_ROWS; ++i)
        assert(ff_trace_capture_add(&capture, 2, i + 1u, 0, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    before = capture;
    assert(ff_trace_capture_add(&capture, 2, 1, 0, 0, 0, 0) == FF_TRACE_CAPTURE_FULL);
    assert(!memcmp(&before, &capture, sizeof capture));
}

static void invalid_state_is_atomic(void)
{
    ff_trace_capture capture, before;
    assert(!ff_trace_capture_init(&capture, 0));
    capture.entries = 1;
    before = capture;
    assert(ff_trace_capture_add(&capture, 2, 1, 0, 0, 0, 0) == FF_TRACE_CAPTURE_INVALID);
    assert(!memcmp(&before, &capture, sizeof capture));
}

static void failed_first_suffix_does_not_rotate(void)
{
    ff_trace_capture capture, before;
    assert(!ff_trace_capture_init(&capture, FF_TRACE_CAPTURE_MAX_START));
    for (uint32_t i = 0; i < FF_TRACE_CAPTURE_ROUTINES; ++i)
        assert(ff_trace_capture_add(&capture, 1, 0x10000u + i, i, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    for (uint32_t i = FF_TRACE_CAPTURE_ROUTINES; i < FF_TRACE_CAPTURE_ROWS; ++i)
        assert(ff_trace_capture_add(&capture, 2, 0x20000u + i, i, 0, 0, 0) == FF_TRACE_CAPTURE_OK);
    assert(capture.total == FF_TRACE_CAPTURE_MAX_START && capture.captured_start == 0);
    before = capture;
    assert(ff_trace_capture_add(&capture, 1, 0xdeadbeefu, 0, 0, 0, 0) == FF_TRACE_CAPTURE_FULL);
    assert(!memcmp(&before, &capture, sizeof capture));
    assert(ff_trace_capture_add(&capture, 2, 0xbeefdeadu, 0, 0, 0, 0) == FF_TRACE_CAPTURE_ROTATE);
    assert(capture.captured_start == FF_TRACE_CAPTURE_MAX_START &&
           capture.prefix_entries == FF_TRACE_CAPTURE_ROUTINES &&
           capture.prefix_routines == FF_TRACE_CAPTURE_ROUTINES);
}

int main(void)
{
    expected_row_hash();
    windows_and_prefix();
    entries_and_atomic_rejection();
    start_validation_and_default_horizon();
    failed_first_suffix_does_not_rotate();
    invalid_state_is_atomic();
    puts("trace hash, bounded windows, prefix rotation and atomic caps passed");
    return 0;
}
