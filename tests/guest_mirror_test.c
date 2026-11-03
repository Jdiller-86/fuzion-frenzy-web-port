/* Owned bounded guest physical-mirror page; no original code or device model. */
#include "../src/runtime/guest_mirror.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    ff_guest_mirror mirror, saved;
    memset(&mirror, 0x5a, sizeof mirror);
    ff_guest_mirror_init(&mirror);
    for (uint32_t i = 0; i < FF_GUEST_MIRROR_BYTES; ++i) assert(mirror.bytes[i] == 0);

    assert(ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE, 4));
    assert(ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE, FF_GUEST_MIRROR_BYTES));
    assert(ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + 0xffc, 4));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE - 4, 4));
    assert(ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + 1, 4));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + 0xffe, 4));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE + FF_GUEST_MIRROR_BYTES, 1));
    assert(!ff_guest_mirror_contains(&mirror, FF_GUEST_MIRROR_BASE, 0));
    assert(!ff_guest_mirror_contains(&mirror, 0, 4));
    assert(!ff_guest_mirror_contains(NULL, FF_GUEST_MIRROR_BASE, 4));
    assert(!ff_guest_mirror_contains(&mirror, 0xffffffffu, 2));

    uint8_t *base = ff_guest_mirror_pointer(&mirror, FF_GUEST_MIRROR_BASE);
    uint8_t *last = ff_guest_mirror_pointer(&mirror, FF_GUEST_MIRROR_BASE + 0xffc);
    assert(base == mirror.bytes && last == mirror.bytes + 0xffc && base + 0xffc == last);
    *(uint32_t *)base = 0x12345678u;
    assert(*(uint32_t *)(mirror.bytes) == 0x12345678u);
    *(uint32_t *)last = 0x9abcdef0u;
    assert(*(uint32_t *)(mirror.bytes + 0xffc) == 0x9abcdef0u);
    assert(ff_guest_mirror_pointer(NULL, FF_GUEST_MIRROR_BASE) == NULL);
    saved = mirror;
    ff_guest_mirror_init(&mirror);
    assert(!memcmp(&mirror, &(ff_guest_mirror){0}, sizeof mirror) && saved.bytes[0] == 0x78);
    printf("owned bounded guest mirror: page bounds, partial rejection, reset and round-trip passed\n");
    return 0;
}
