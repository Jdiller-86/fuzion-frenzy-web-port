#ifndef FF_GUEST_MIRROR_H
#define FF_GUEST_MIRROR_H
#include <stdint.h>

/* Bounded guest-visible physical-mirror page reached by original cache-flush
 * code. The pinned public bridge documents that physical P is visible at
 * 0x80000000 + P on hardware and keeps the window as separate storage because
 * it does not model physical aliasing. This proof does the same for one page:
 * accesses inside the page round-trip through this storage and are counted by
 * the caller; anything outside still fails the identity-window check. This is
 * not a general 0x80000000 window or a true physical alias. */
#define FF_GUEST_MIRROR_BASE 0x80000000u
#define FF_GUEST_MIRROR_BYTES 0x1000u

typedef struct {
    uint8_t bytes[FF_GUEST_MIRROR_BYTES];
} ff_guest_mirror;

void ff_guest_mirror_init(ff_guest_mirror *);
/* True only when the whole non-empty access lies inside the bounded page. */
int ff_guest_mirror_contains(const ff_guest_mirror *, uint32_t address, uint32_t bytes);
/* Pointer to the same offset in the mirror; the caller must have checked
 * contains for the exact access first. */
uint8_t *ff_guest_mirror_pointer(ff_guest_mirror *, uint32_t address);
#endif
