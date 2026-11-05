/* Owned TLS-context scope scenarios; no original code or device. */
#include "../src/runtime/tls_scope.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    /* Same nonzero TLS base means one register context. */
    assert(ff_tls_same_context(0x03f00020u, 0x03f00020u) == 1u);
    /* A different base is a thread switch, not a register change. */
    assert(ff_tls_same_context(0x03f00020u, 0x03f01020u) == 0u);
    /* An absent base cannot establish a context. */
    assert(ff_tls_same_context(0u, 0u) == 0u);
    assert(ff_tls_same_context(0u, 0x03f00020u) == 0u);

    /* ABI coverage class: only a wrapped call boundary can report a change;
     * an in-body register write is outside the logger's coverage. */
    assert(ff_reg_change_observable(1u) == 1u);
    assert(ff_reg_change_observable(0u) == 0u);

    printf("tls_scope: ok\n");
    return 0;
}
