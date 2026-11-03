#include "guest_mirror.h"

void ff_guest_mirror_init(ff_guest_mirror *mirror)
{
    if (mirror) {
        mirror->base = FF_GUEST_MIRROR_BASE;
        mirror->bytes = FF_GUEST_MIRROR_BYTES;
    }
}

int ff_guest_mirror_contains(const ff_guest_mirror *mirror, uint32_t address, uint32_t bytes)
{
    return mirror && bytes && address >= mirror->base &&
           (uint64_t)address + bytes <= (uint64_t)mirror->base + mirror->bytes;
}

uint32_t ff_guest_mirror_offset(uint32_t address)
{
    return address - FF_GUEST_MIRROR_BASE;
}
