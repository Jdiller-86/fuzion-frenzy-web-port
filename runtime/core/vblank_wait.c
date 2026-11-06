#include "vblank_wait.h"

uint32_t ff_vblank_wait_accept(uint32_t declared, uint32_t object, uint32_t reason,
                               uint32_t mode, uint32_t alertable, uint32_t timeout,
                               uint32_t header, uint32_t signal_state)
{
    /* Full five-argument equality with the observed call. */
    if (object != FF_VBLANK_WAIT_OBJECT || reason != 6u || mode != 1u ||
        alertable != 0u || timeout != 0u)
        return FF_VBLANK_WAIT_ARGS;
    if (!declared) return FF_VBLANK_WAIT_NO_DELIVERY;
    /* Pinned title-built event recognition: low byte 0 or 1, Size byte 4. */
    if ((header & 0xFFu) > 1u || ((header >> 16) & 0xFFu) != 4u || signal_state != 0u)
        return FF_VBLANK_WAIT_EVENT;
    return FF_VBLANK_WAIT_ACCEPT;
}
