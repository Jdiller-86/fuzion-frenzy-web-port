#ifndef FF_TLS_SCOPE_H
#define FF_TLS_SCOPE_H
#include <stdint.h>

/* Thread-scope predicate for register diagnostics. g_esi is thread-local in
 * the recompiled runtime, so a value change is only meaningful inside one TLS
 * context; a context switch is not a register clobber. */
uint32_t ff_tls_same_context(uint32_t entry_fs, uint32_t current_fs);

#endif
