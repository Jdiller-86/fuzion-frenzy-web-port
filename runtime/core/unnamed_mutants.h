#ifndef FF_UNNAMED_MUTANTS_H
#define FF_UNNAMED_MUTANTS_H
#include "virtual_memory.h"

#define FF_MUTANT_CAPACITY 4u
#define FF_MUTANT_HANDLE_BASE 0x00080000u
#define FF_MUTANT_NO_SLOTS 0xc000009au

typedef struct {
    ff_virtual_memory *memory;
    uint32_t count;
    uint32_t handles[FF_MUTANT_CAPACITY];
} ff_unnamed_mutants;

/* This bounded model creates only unnamed, initially unowned objects. Handles
 * have live records for the model lifetime; there are no close, wait, acquire, or
 * reuse operations. Reinitialize only when resetting the whole probe. */
uint32_t ff_unnamed_mutants_init(ff_unnamed_mutants *, ff_virtual_memory *);
uint32_t ff_unnamed_mutants_create(ff_unnamed_mutants *, uint32_t output,
                                   uint32_t object_attributes, uint32_t initial_owner);
#endif
