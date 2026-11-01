#ifndef FF_GUEST_STRINGS_H
#define FF_GUEST_STRINGS_H
#include "virtual_memory.h"

/* Internal admission status; the guest RtlInitAnsiString ABI returns void.
 * Only disjoint, fully admitted strings up to 65,534 bytes are supported. */
uint32_t ff_init_ansi_string(ff_virtual_memory *, uint32_t destination, uint32_t source);
/* Counted byte comparison, including embedded NUL. Internal admission status
 * is separate from the guest BOOLEAN result. */
uint32_t ff_equal_ansi_string(ff_virtual_memory *, uint32_t first, uint32_t second,
                              uint8_t insensitive, uint32_t *equal);
#endif
