#ifndef FF_CONSOLE_SETTINGS_H
#define FF_CONSOLE_SETTINGS_H
#include "virtual_memory.h"

#define FF_CONSOLE_OS_SETTINGS_BYTES 96u

/* Read only the explicitly configured DWORD for XC_P_CONTROL_GAMES (10) or
 * XC_MISC (17). No EEPROM, host settings, inferred defaults or persistence. */
uint32_t ff_query_console_dword(ff_virtual_memory *,const uint32_t args[5],uint32_t configured_value);

/* Synthetic provider input only: these bytes are not an EEPROM image, and
 * their contents are opaque to this component. */
uint32_t ff_query_console_os(ff_virtual_memory *,const uint32_t args[5],
                             const uint8_t configured[FF_CONSOLE_OS_SETTINGS_BYTES]);
#endif
