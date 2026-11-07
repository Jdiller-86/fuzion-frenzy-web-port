/* Owned display-mode declaration scenarios; no original code or device. */
#include "../runtime/core/display_mode.h"
#include <assert.h>
#include <stdio.h>

#define A (FF_DISPLAY_TUPLE_ADDR)
#define S (FF_DISPLAY_TUPLE_STEP)
#define M (FF_DISPLAY_TUPLE_MODE)
#define F (FF_DISPLAY_TUPLE_FORMAT)
#define P (FF_DISPLAY_TUPLE_PITCH)
#define B (FF_DISPLAY_TUPLE_FB)

int main(void)
{
    /* The corrected tuple at the declared MMIO register base, not guest RAM. */
    assert(ff_display_mode_accept(1u, A, S, M, F, P, B) == FF_DISPLAY_OK);
    assert(ff_display_mode_window(B) == FF_DISPLAY_CONTIG_BASE + B);
    assert(ff_display_mode_window(0u) == 0u);
    assert(ff_display_mode_window(FF_DISPLAY_CONTIG_SIZE) == 0u);
    assert(ff_display_mode_window(FF_DISPLAY_CONTIG_SIZE - 4u) ==
           FF_DISPLAY_CONTIG_BASE + FF_DISPLAY_CONTIG_SIZE - 4u);

    /* Missing device-aperture declaration. */
    assert(ff_display_mode_accept(0u, A, S, M, F, P, B) == FF_DISPLAY_BAD_ADDR);

    /* Every observed argument must match exactly. */
    assert(ff_display_mode_accept(1u, A + 4u, S, M, F, P, B) == FF_DISPLAY_BAD_ARGS);
    assert(ff_display_mode_accept(1u, A, S + 1u, M, F, P, B) == FF_DISPLAY_BAD_ARGS);
    assert(ff_display_mode_accept(1u, A, S, M + 1u, F, P, B) == FF_DISPLAY_BAD_ARGS);
    assert(ff_display_mode_accept(1u, A, S, M, F + 1u, P, B) == FF_DISPLAY_BAD_ARGS);
    assert(ff_display_mode_accept(1u, A, S, M, F, P + 4u, B) == FF_DISPLAY_BAD_ARGS);
    /* The old damaged-frame tuple must no longer be admitted. */
    assert(ff_display_mode_accept(1u, 0x03C10CFCu, S, M, 0u, P, B) == FF_DISPLAY_BAD_ARGS);

    /* Framebuffer outside the documented contiguous rule. */
    assert(ff_display_mode_accept(1u, A, S, M, F, P, FF_DISPLAY_CONTIG_SIZE) ==
           FF_DISPLAY_BAD_FB);
    assert(ff_display_mode_accept(1u, A, S, M, F, P, 0xFFFFFFFFu) == FF_DISPLAY_BAD_FB);

    printf("display_mode: ok\n");
    return 0;
}
