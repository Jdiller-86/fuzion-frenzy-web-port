#include "local_settings.h"
#include "thread_bootstrap.h"
#include <string.h>

static int fits(uint8_t *ram, uint32_t bytes, uint32_t address, uint32_t size, uint32_t alignment)
{
    return ram && address && !(address % alignment) && bytes >= size && address <= bytes - size;
}
static void word(uint8_t *ram, uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (i * 8));
}
uint32_t ff_query_misc_flags(uint8_t *ram, uint32_t bytes, const uint32_t args[5], uint32_t misc_flags)
{
    if (!args || args[0] != 17) return FF_BOOT_UNSUPPORTED;
    if (args[4] && !fits(ram, bytes, args[4], 4, 4)) return FF_BOOT_INVALID;
    if (args[3] < 4) {
        if (args[4]) word(ram, args[4], 4);
        return 0xc0000023u; /* STATUS_BUFFER_TOO_SMALL */
    }
    if (!fits(ram, bytes, args[1], 4, 4) || !fits(ram, bytes, args[2], args[3], 1)) return FF_BOOT_INVALID;
    word(ram, args[1], 4); /* REG_DWORD */
    memset(ram + args[2], 0, args[3]);
    word(ram, args[2], misc_flags);
    if (args[4]) word(ram, args[4], 4);
    return FF_BOOT_OK;
}
