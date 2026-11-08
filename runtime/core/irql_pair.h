#ifndef FF_IRQL_PAIR_H
#define FF_IRQL_PAIR_H
#include <stdint.h>
#include "thread_bootstrap.h"
#include "virtual_memory.h"

/* A single-thread, boot-PCR 0 -> 2 -> 0 pair. This component does not own
 * shared-memory observer metering; callers meter the PCR preflight separately. */
typedef struct {
    uint32_t active, thread_id, fs, saved_level;
    uint32_t raise_count, lower_count;
} ff_irql_pair;

void ff_irql_pair_init(ff_irql_pair *);
uint32_t ff_irql_pair_raise_to_dpc(ff_irql_pair *, const ff_boot_runtime *,
                                   const ff_virtual_memory *, uint8_t *old_level);
uint32_t ff_irql_pair_lower_saved(ff_irql_pair *, const ff_boot_runtime *,
                                  const ff_virtual_memory *, uint32_t value);

#endif
