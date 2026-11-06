#ifndef FF_VBLANK_WAIT_H
#define FF_VBLANK_WAIT_H
#include <stdint.h>

/* Bounded decision for the observed ordinal-159 KeWaitForSingleObject call on
 * the D3D vblank event. It authorizes one synthetic vblank delivery and event
 * consumption per matching call; it never models success without a declared
 * delivery. The caller supplies the five observed arguments and the guest
 * event's type and signal word. */
#define FF_VBLANK_WAIT_OBJECT 0x00206080u

enum {
    FF_VBLANK_WAIT_ACCEPT = 0,
    FF_VBLANK_WAIT_NO_DELIVERY = 1,
    FF_VBLANK_WAIT_ARGS = 2,
    FF_VBLANK_WAIT_EVENT = 3
};

uint32_t ff_vblank_wait_accept(uint32_t declared, uint32_t object, uint32_t reason,
                               uint32_t mode, uint32_t alertable, uint32_t timeout,
                               uint32_t event_type, uint32_t signal_state);

#endif
