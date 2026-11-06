#ifndef FF_GPU_INSTANCE_H
#define FF_GPU_INSTANCE_H
#include <stdint.h>
#include "virtual_memory.h"

/* Modeled MmClaimGpuInstanceMemory (ordinal 168). The pinned public bridge
 * returns the top of its 64 MiB contiguous window minus the request; this
 * runtime has a different explicit window, so the claim is a genuine bounded
 * allocation from the topmost free span rather than a copied console address.
 * MAXULONG_PTR selects the public bridge's 128 KiB default. */
#define FF_GPU_INSTANCE_DEFAULT (128u * 1024u)

/* Validate and zero the optional padding output, claim the requested bytes
 * with read/write permission, and return the chosen base. The claim is
 * all-or-nothing: padding validation and capacity checks happen before any
 * virtual-memory region is committed. */
uint32_t ff_claim_gpu_instance_memory(ff_virtual_memory *, uint32_t bytes, uint32_t padding,
                                      uint32_t lowest, uint32_t highest, uint32_t *base);
#endif
