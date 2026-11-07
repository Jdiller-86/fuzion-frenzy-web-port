#include "trace_capture.h"

#include <string.h>

static void hash_word(ff_sha256 *digest, uint32_t value)
{
    const uint8_t bytes[4] = {
        (uint8_t)value, (uint8_t)(value >> 8),
        (uint8_t)(value >> 16), (uint8_t)(value >> 24)
    };
    ff_sha256_update(digest, bytes, sizeof bytes);
}

static int zero_bytes(const uint8_t *bytes, uint32_t count)
{
    for (uint32_t i = 0; i < count; ++i) if (bytes[i]) return 0;
    return 1;
}

static int valid_state(const ff_trace_capture *capture)
{
    uint32_t seen[1024] = { 0 };
    if ((capture->requested_start != 0u &&
         capture->requested_start != FF_TRACE_CAPTURE_MAX_START) ||
        capture->total > capture->requested_start + FF_TRACE_CAPTURE_ROWS ||
        capture->entries > capture->total ||
        capture->routine_count > FF_TRACE_CAPTURE_ROUTINES ||
        capture->routine_count > capture->entries ||
        ((capture->entries == 0u) != (capture->routine_count == 0u))) return 0;

    if (capture->captured_start == 0u) {
        if ((capture->requested_start == FF_TRACE_CAPTURE_MAX_START &&
             capture->total > FF_TRACE_CAPTURE_MAX_START) ||
            capture->prefix_entries || capture->prefix_routines ||
            !zero_bytes(capture->prefix_digest, sizeof capture->prefix_digest)) return 0;
    } else if (capture->captured_start != FF_TRACE_CAPTURE_MAX_START ||
               capture->requested_start != FF_TRACE_CAPTURE_MAX_START ||
               capture->total <= FF_TRACE_CAPTURE_MAX_START ||
               capture->prefix_entries > FF_TRACE_CAPTURE_MAX_START ||
               capture->prefix_routines > capture->prefix_entries ||
               capture->prefix_entries > capture->entries ||
               capture->prefix_routines > capture->routine_count ||
               ((capture->prefix_entries == 0u) != (capture->prefix_routines == 0u)) ||
               capture->entries - capture->prefix_entries >
                   capture->total - capture->captured_start ||
               capture->routine_count - capture->prefix_routines >
                   capture->entries - capture->prefix_entries) {
        return 0;
    }

    for (uint32_t i = 0; i < capture->routine_count; ++i) {
        if (!capture->routines[i]) return 0;
        uint32_t slot = (capture->routines[i] * 2654435761u) >> 22;
        while (seen[slot] && seen[slot] != capture->routines[i])
            slot = (slot + 1u) & 1023u;
        if (seen[slot]) return 0;
        seen[slot] = capture->routines[i];
    }
    for (uint32_t i = capture->routine_count; i < FF_TRACE_CAPTURE_ROUTINES; ++i)
        if (capture->routines[i]) return 0;
    return 1;
}

uint32_t ff_trace_capture_init(ff_trace_capture *capture, uint32_t start)
{
    if (!capture || (start != 0u && start != FF_TRACE_CAPTURE_MAX_START))
        return FF_TRACE_CAPTURE_INVALID;
    memset(capture, 0, sizeof *capture);
    capture->requested_start = start;
    ff_sha256_init(&capture->digest);
    static const uint8_t domain[] = "ff-original-trace-v1";
    ff_sha256_update(&capture->digest, domain, sizeof domain);
    return FF_TRACE_CAPTURE_OK;
}

uint32_t ff_trace_capture_add(ff_trace_capture *capture, uint32_t kind,
                              uint32_t address, uint32_t stack, uint32_t eax,
                              uint32_t fs, uint32_t exception_head)
{
    if (!capture || !valid_state(capture) || (kind != 1u && kind != 2u) || !address)
        return FF_TRACE_CAPTURE_INVALID;

    const uint32_t horizon = capture->requested_start + FF_TRACE_CAPTURE_ROWS;
    if (capture->total >= horizon) return FF_TRACE_CAPTURE_FULL;

    int new_entry = 0;
    if (kind == 1u) {
        new_entry = 1;
        for (uint32_t i = 0; i < capture->routine_count; ++i) {
            if (capture->routines[i] == address) {
                new_entry = 0;
                break;
            }
        }
        if (new_entry && capture->routine_count == FF_TRACE_CAPTURE_ROUTINES)
            return FF_TRACE_CAPTURE_FULL;
    }

    uint32_t result = FF_TRACE_CAPTURE_OK;
    if (capture->requested_start == FF_TRACE_CAPTURE_MAX_START &&
        capture->total == FF_TRACE_CAPTURE_MAX_START && !capture->captured_start) {
        ff_sha256_final(&capture->digest, capture->prefix_digest);
        capture->prefix_entries = capture->entries;
        capture->prefix_routines = capture->routine_count;
        capture->captured_start = FF_TRACE_CAPTURE_MAX_START;
        result = FF_TRACE_CAPTURE_ROTATE;
    }

    hash_word(&capture->digest, kind);
    hash_word(&capture->digest, address);
    hash_word(&capture->digest, stack);
    hash_word(&capture->digest, eax);
    hash_word(&capture->digest, fs);
    hash_word(&capture->digest, exception_head);
    ++capture->total;
    if (kind == 1u) {
        ++capture->entries;
        if (new_entry) capture->routines[capture->routine_count++] = address;
    }
    return result;
}
