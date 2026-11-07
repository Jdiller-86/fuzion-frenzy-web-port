#ifndef FF_NAMED_EVENTS_H
#define FF_NAMED_EVENTS_H
#include "virtual_memory.h"

#define FF_EVENT_CAPACITY 4u
#define FF_EVENT_NAME_BYTES 63u
#define FF_EVENT_HANDLE_BASE 0x00090000u
#define FF_EVENT_NAME_EXISTS 0x40000000u
#define FF_EVENT_NO_SLOTS 0xc000009au

typedef struct { uint32_t name_bytes; uint8_t name[64]; } ff_named_event_object;
/* object is a one-based stable object ID; zero is never a valid reference. */
typedef struct { uint32_t handle, object; } ff_named_event_handle;
typedef struct {
    ff_virtual_memory *memory;
    uint32_t object_count, handle_count;
    ff_named_event_object objects[FF_EVENT_CAPACITY];
    ff_named_event_handle handles[FF_EVENT_CAPACITY];
} ff_named_events;

uint32_t ff_named_events_init(ff_named_events *, ff_virtual_memory *);
uint32_t ff_named_events_create(ff_named_events *, uint32_t output, uint32_t attributes,
                                uint32_t event_type, uint32_t initial_state);
#endif
