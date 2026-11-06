/* Owned callee-saved save/pop order scenarios; no original code or device. */
#include "../src/runtime/reg_save_order.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* ebx, ebp, esi, edi pushed; the literal source pops the exact reverse. */
    const uint8_t push[4] = {0u, 1u, 2u, 3u};   /* ebx, ebp, esi, edi */
    const uint8_t reverse[4] = {3u, 2u, 1u, 0u};
    /* Observed runtime order: the saved-EDI slot is consumed first (as ESI). */
    const uint8_t observed[4] = {3u, 0u, 1u, 2u};

    assert(ff_reg_order_is_reverse(push, reverse, 4u) == 1u);
    assert(ff_reg_order_is_reverse(push, observed, 4u) == 0u);

    /* Degenerate input is refused rather than guessed. */
    assert(ff_reg_order_is_reverse(push, reverse, 1u) == 0u);
    assert(ff_reg_order_is_reverse(push, reverse, 9u) == 0u);
    assert(ff_reg_order_is_reverse(NULL, reverse, 4u) == 0u);
    assert(ff_reg_order_is_reverse(push, NULL, 4u) == 0u);

    printf("reg_save_order: ok\n");
    return 0;
}
