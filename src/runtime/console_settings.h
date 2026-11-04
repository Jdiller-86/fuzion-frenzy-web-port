#ifndef FF_CONSOLE_SETTINGS_H
#define FF_CONSOLE_SETTINGS_H
#include "virtual_memory.h"

/* Read only the explicitly configured DWORD for XC_P_CONTROL_GAMES (10) or
 * XC_MISC (17). No EEPROM, host settings, inferred defaults or persistence. */
uint32_t ff_query_console_dword(ff_virtual_memory *,const uint32_t args[5],uint32_t configured_value);
#endif
