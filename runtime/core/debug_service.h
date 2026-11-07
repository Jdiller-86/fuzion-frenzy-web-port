#ifndef FF_DEBUG_SERVICE_H
#define FF_DEBUG_SERVICE_H
#include "guest_strings.h"

#define FF_DEBUG_SERVICE_WINDOW_CAPACITY 8u
#define FF_DEBUG_SERVICE_START_MAX 8u
#define FF_DEBUG_SERVICE_WORDS 6u

typedef struct {
    uint32_t requested_start;
    uint32_t capacity;
    uint32_t limit;
    uint32_t prefix_completed;
    uint32_t captured_completed;
    uint32_t total_completed;
    uint32_t sealed;
    uint32_t last_service;
    uint32_t last_bytes;
    uint32_t last_status;
} ff_debug_service;

void ff_debug_service_init(ff_debug_service *);
/* Select the fixed 8-call window at start 0 or 8. Selection is pre-run only. */
uint32_t ff_debug_service_profile(ff_debug_service *, uint32_t requested_start);
void ff_debug_service_begin(ff_debug_service *);
/* Validate one service-1 counted ANSI descriptor. No payload is copied. */
uint32_t ff_debug_service_call(ff_debug_service *, uint32_t service,
                               const ff_virtual_memory *, uint32_t descriptor);
uint32_t ff_debug_service_window_word(const ff_debug_service *, uint32_t field);
#endif
