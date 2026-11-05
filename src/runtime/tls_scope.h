#ifndef FF_TLS_SCOPE_H
#define FF_TLS_SCOPE_H
#include <stdint.h>

/* Thread-scope predicate for register diagnostics. g_esi is thread-local in
 * the recompiled runtime, so a value change is only meaningful inside one TLS
 * context; a context switch is not a register clobber. */
uint32_t ff_tls_same_context(uint32_t entry_fs, uint32_t current_fs);

/* Coverage class of the call-boundary ABI check: a register write is only
 * observable there when it happens at a wrapped call boundary; an in-body
 * write (or a write on an unwrapped indirect/tail path) is not reported. */
uint32_t ff_reg_change_observable(uint32_t at_call_boundary);

#endif
