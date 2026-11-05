/* Owned vblank-wait decision scenarios; no original code or device. */
#include "../src/runtime/vblank_wait.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* Full match with one declared delivery on an unsignaled sync event. */
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_ACCEPT);
    assert(ff_vblank_wait_accept(3u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_ACCEPT);

    /* No declared delivery means no modeled success. */
    assert(ff_vblank_wait_accept(0u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_NO_DELIVERY);

    /* Every argument must match exactly. */
    assert(ff_vblank_wait_accept(1u, 0x00206084u, 6u, 1u, 0u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 0u, 1u, 0u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 0u, 0u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 1u, 0u, 1u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 4u, 1u, 0u) ==
           FF_VBLANK_WAIT_ARGS);

    /* The event must resolve as an unsignaled synchronization event. */
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0u, 0u) ==
           FF_VBLANK_WAIT_EVENT);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 1u, 1u) ==
           FF_VBLANK_WAIT_EVENT);

    printf("vblank_wait: ok\n");
    return 0;
}
