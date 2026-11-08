#ifndef FF_LOCAL_MUTANTS_H
#define FF_LOCAL_MUTANTS_H
#include "named_events.h"

#define FF_LOCAL_MUTANT_CAPACITY 4u
#define FF_LOCAL_MUTANT_HANDLE_BASE 0x00080000u
#define FF_LOCAL_MUTANT_NAME_BYTES 63u
#define FF_LOCAL_MUTANT_NAME_EXISTS 0x40000000u
#define FF_LOCAL_MUTANT_NO_SLOTS 0xc000009au
#define FF_LOCAL_MUTANT_TYPE_MISMATCH 0xc0000024u

typedef struct { uint32_t name_bytes, owner_thread; uint8_t name[64]; } ff_local_mutant_object;
typedef struct { uint32_t handle, object; } ff_local_mutant_handle;
typedef struct {
    ff_virtual_memory *memory;
    const ff_named_events *events;
    uint32_t object_count, handle_count;
    ff_local_mutant_object objects[FF_LOCAL_MUTANT_CAPACITY];
    ff_local_mutant_handle handles[FF_LOCAL_MUTANT_CAPACITY];
} ff_local_mutants;

uint32_t ff_local_mutants_init(ff_local_mutants *, ff_virtual_memory *, const ff_named_events *);
uint32_t ff_local_mutants_create(ff_local_mutants *, uint32_t output, uint32_t attributes,
                                 uint32_t initial_owner);
uint32_t ff_local_mutants_guard_event(const ff_local_mutants *, uint32_t output,
                                      uint32_t attributes);
uint32_t ff_local_mutants_acquire(ff_local_mutants *, uint32_t handle,
                                  uint32_t wait_mode, uint32_t alertable,
                                  uint32_t timeout, uint32_t thread_id);
uint32_t ff_local_mutants_release(ff_local_mutants *, uint32_t handle,
                                  uint32_t previous_count_pointer, uint32_t thread_id);
#endif
