#include "guest_mirror.h"
#include <string.h>

void ff_guest_mirror_init(ff_guest_mirror *mirror)
{
    if (mirror) memset(mirror, 0, sizeof *mirror);
}

int ff_guest_mirror_contains(const ff_guest_mirror *mirror, uint32_t address, uint32_t bytes)
{
    return mirror && bytes && address >= FF_GUEST_MIRROR_BASE &&
           (uint64_t)address + bytes <= (uint64_t)FF_GUEST_MIRROR_BASE + FF_GUEST_MIRROR_BYTES;
}

uint8_t *ff_guest_mirror_pointer(ff_guest_mirror *mirror, uint32_t address)
{
    return mirror ? mirror->bytes + (address - FF_GUEST_MIRROR_BASE) : NULL;
}
