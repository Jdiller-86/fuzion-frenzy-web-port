#ifndef FF_CONSOLE_SETTINGS_H
#define FF_CONSOLE_SETTINGS_H
#include "virtual_memory.h"

/* Read only the explicitly configured DWORD for XC_P_CONTROL_GAMES (10) or
 * XC_MISC (17). No EEPROM, host settings, inferred defaults or persistence. */
uint32_t ff_query_console_dword(ff_virtual_memory *,const uint32_t args[5],uint32_t configured_value);

/* The whole user-settings block read through index 0xFF (XC_MAX_OS), 0x60
 * bytes in the layout the pinned translator documents. Every field is an
 * explicit caller input; nothing is read from the host, an EEPROM or a save
 * file. The fixed width, type and short-buffer status follow that pin. */
#define FF_CONSOLE_SETTINGS_BLOCK_BYTES 0x60u
typedef struct
{
    int32_t bias;
    uint8_t standard_name[4];
    uint8_t daylight_name[4];
    uint32_t standard_date;
    uint32_t daylight_date;
    int32_t standard_bias;
    int32_t daylight_bias;
    uint32_t language;
    uint32_t video;
    uint32_t audio;
    uint32_t parental;
    uint32_t online[4];
    uint32_t misc;
    uint32_t dvd_region;
} ff_console_settings_block;
uint32_t ff_query_console_block(ff_virtual_memory *,const uint32_t args[5],const ff_console_settings_block *);
#endif
