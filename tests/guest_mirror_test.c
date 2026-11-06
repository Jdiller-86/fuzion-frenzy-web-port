/* Owned guest contiguous-window descriptor; no original code or device model. */
#include "../runtime/core/guest_mirror.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    ff_guest_mirror mirror;
    ff_guest_mirror_init(&mirror);
    assert(mirror.base == FF_GUEST_MIRROR_BASE && mirror.bytes == FF_GUEST_MIRROR_BYTES);
    assert(ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE, 4));
    assert(ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + FF_GUEST_MIRROR_BYTES - 4, 4));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE - 4, 4));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + FF_GUEST_MIRROR_BYTES, 1));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + FF_GUEST_MIRROR_BYTES - 2, 4));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE, 0));
    assert(!ff_guest_mirror_contains(&mirror, 0, 4));
    assert(!ff_guest_mirror_contains(NULL, FF_GUEST_MIRROR_BASE, 4));
    assert(ff_guest_mirror_offset(FF_GUEST_MIRROR_BASE) == 0);
    assert(ff_guest_mirror_offset(0x83B7D000u) == 0x03B7D000u);
    printf("owned guest contiguous window: base, 64 MiB bounds, partial rejection and offset passed\n");
    return 0;
}
