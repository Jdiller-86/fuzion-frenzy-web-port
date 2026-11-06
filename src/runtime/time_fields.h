#ifndef FF_TIME_FIELDS_H
#define FF_TIME_FIELDS_H
#include <stdint.h>

/* Deterministic conversion of a 100 ns interval since 1601-01-01 to the
 * eight-field Xbox TIME_FIELDS layout. No host timezone or locale is used.
 * Returns 0 on success, FF_BOOT_INVALID for a null output or
 * FF_BOOT_UNSUPPORTED when the year does not fit the supported 1601..32767
 * range. */
uint32_t ff_time_to_fields(uint64_t time, uint16_t fields[8]);
#endif
