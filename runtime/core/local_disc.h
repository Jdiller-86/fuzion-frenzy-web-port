#ifndef FF_LOCAL_DISC_H
#define FF_LOCAL_DISC_H
#include "object_namespace.h"

/* Bounded ownership record for a validated, retained local XISO session.
 * The provider is opaque; this does not copy or read image bytes. */
typedef struct {
    ff_object_namespace *space;
    uint32_t provider, device, bound, denied;
    uint64_t image_bytes;
} ff_local_disc;

uint32_t ff_disc_bind(ff_local_disc *, ff_object_namespace *, uint32_t provider,
                      uint64_t image_bytes);
uint32_t ff_disc_create(ff_local_disc *, const uint32_t args[9]);
void ff_disc_reset(ff_local_disc *);
#endif
