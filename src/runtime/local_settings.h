#ifndef FF_LOCAL_SETTINGS_H
#define FF_LOCAL_SETTINGS_H
#include <stdint.h>
/* Bounded REG_DWORD read for explicitly supplied XC_MISC. No EEPROM/defaults. */
uint32_t ff_query_misc_flags(uint8_t *ram, uint32_t bytes, const uint32_t args[5], uint32_t misc_flags);
#endif
