/* Owned vblank-wait decision scenarios; no original code or device. */
#include "../src/runtime/vblank_wait.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* Full match with one declared delivery: type 0 (notification) and type 1
     * (synchronization) title-built events, both Size = 4 dwords. */
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ACCEPT);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00040001u, 0u) ==
           FF_VBLANK_WAIT_ACCEPT);
    assert(ff_vblank_wait_accept(3u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ACCEPT);

    /* No declared delivery means no modeled success. */
    assert(ff_vblank_wait_accept(0u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_NO_DELIVERY);

    /* Every argument must match exactly. */
    assert(ff_vblank_wait_accept(1u, 0x00206084u, 6u, 1u, 0u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 0u, 1u, 0u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 0u, 0u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 1u, 0u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ARGS);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 4u, 0x00040000u, 0u) ==
           FF_VBLANK_WAIT_ARGS);

    /* The full first header dword is decoded: low byte 0/1 and Size byte 4. */
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00100000u, 0u) ==
           FF_VBLANK_WAIT_EVENT);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00000000u, 0u) ==
           FF_VBLANK_WAIT_EVENT);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00000001u, 0u) ==
           FF_VBLANK_WAIT_EVENT);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00040002u, 0u) ==
           FF_VBLANK_WAIT_EVENT);
    assert(ff_vblank_wait_accept(1u, FF_VBLANK_WAIT_OBJECT, 6u, 1u, 0u, 0u, 0x00040000u, 1u) ==
           FF_VBLANK_WAIT_EVENT);

    printf("vblank_wait: ok\n");
    return 0;
}
