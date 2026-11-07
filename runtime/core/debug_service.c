#include "debug_service.h"
#include "thread_bootstrap.h"
#include <string.h>

void ff_debug_service_init(ff_debug_service *state)
{
    if (!state) return;
    memset(state, 0, sizeof *state);
    state->capacity = FF_DEBUG_SERVICE_WINDOW_CAPACITY;
    state->limit = FF_DEBUG_SERVICE_WINDOW_CAPACITY;
}

uint32_t ff_debug_service_profile(ff_debug_service *state, uint32_t requested_start)
{
    if (!state || state->sealed || state->total_completed ||
        (requested_start != 0u && requested_start != FF_DEBUG_SERVICE_START_MAX))
        return FF_BOOT_INVALID;
    state->requested_start = requested_start;
    state->capacity = FF_DEBUG_SERVICE_WINDOW_CAPACITY;
    state->limit = requested_start + FF_DEBUG_SERVICE_WINDOW_CAPACITY;
    state->prefix_completed = state->captured_completed = state->total_completed = 0;
    state->last_service = state->last_bytes = state->last_status = 0;
    return 0;
}

void ff_debug_service_begin(ff_debug_service *state)
{
    if (state) state->sealed = 1;
}

uint32_t ff_debug_service_call(ff_debug_service *state, uint32_t service,
                               const ff_virtual_memory *memory, uint32_t descriptor)
{
    uint32_t length = 0, status;
    if (!state) return FF_BOOT_INVALID;
    ff_debug_service_begin(state);
    if (state->total_completed >= state->limit) status = FF_BOOT_UNSUPPORTED;
    else if (service != 1u) status = FF_BOOT_UNSUPPORTED;
    else if (!memory) status = FF_BOOT_UNSUPPORTED;
    else status = ff_debug_ansi_length(memory, descriptor, &length);

    state->last_service = service;
    state->last_bytes = status ? 0u : length;
    state->last_status = status;
    if (status) return status;

    ++state->total_completed;
    if (state->total_completed <= state->requested_start)
        ++state->prefix_completed;
    else
        ++state->captured_completed;
    return 0;
}

uint32_t ff_debug_service_window_word(const ff_debug_service *state, uint32_t field)
{
    if (!state) return 0;
    switch (field) {
    case 0: return state->requested_start;
    case 1: return state->capacity;
    case 2: return state->limit;
    case 3: return state->prefix_completed;
    case 4: return state->captured_completed;
    case 5: return state->total_completed;
    default: return 0;
    }
}
