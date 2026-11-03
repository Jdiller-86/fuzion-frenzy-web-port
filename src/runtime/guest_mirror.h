#ifndef FF_GUEST_MIRROR_H
#define FF_GUEST_MIRROR_H
#include <stdint.h>

/* Bounded guest-visible contiguous window. The pinned public bridge documents
 * that physical P is visible at 0x80000000 + P and that D3D stores a physical
 * address while reading the GPU position back as `GET | 0x80000000`. This
 * proof maps the 64 MiB window directly onto the modeled guest physical RAM:
 * accesses are bounds-checked here and the caller returns guest_ram + offset.
 * It is not a general virtual-memory mapping or permission model. */
#define FF_GUEST_MIRROR_BASE 0x80000000u
#define FF_GUEST_MIRROR_BYTES 0x04000000u

typedef struct {
    uint32_t base, bytes;
} ff_guest_mirror;

void ff_guest_mirror_init(ff_guest_mirror *);
/* True only when the whole non-empty access lies inside the window. */
int ff_guest_mirror_contains(const ff_guest_mirror *, uint32_t address, uint32_t bytes);
/* Physical offset in the mirrored RAM; the caller must have checked contains. */
uint32_t ff_guest_mirror_offset(uint32_t address);
#endif
