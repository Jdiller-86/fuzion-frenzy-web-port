#include "kernel_objects.h"
#include "thread_bootstrap.h"

static int fits(uint8_t *ram, uint32_t bytes, uint32_t address, uint32_t size)
{
    return ram && address && !(address & 3u) && bytes >= size && address <= bytes - size;
}
static void word(uint8_t *ram, uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (i * 8));
}
uint32_t ff_initialize_dpc(uint8_t *ram, uint32_t bytes, uint32_t address, uint32_t routine, uint32_t context)
{
    if (!fits(ram, bytes, address, 28)) return FF_BOOT_INVALID;
    /* Xbox KDPC: short Type, byte Inserted, padding, list, routine/context,
     * two system arguments. Unspecified fields retain their prior contents. */
    ram[address] = 0x13; ram[address + 1] = 0; ram[address + 2] = 0;
    word(ram, address + 12, routine); word(ram, address + 16, context);
    return FF_BOOT_OK;
}
uint32_t ff_initialize_timer(uint8_t *ram, uint32_t bytes, uint32_t address, uint32_t type)
{
    if (!fits(ram, bytes, address, 40) || type > 1) return FF_BOOT_INVALID;
    /* Xbox KTIMER; initialized but unqueued. Header.Absolute and Dpc are not
     * assigned by this operation. Arming requires a separate real clock gate. */
    ram[address] = (uint8_t)(8u + type); ram[address + 2] = 10; ram[address + 3] = 0;
    word(ram, address + 4, 0);
    word(ram, address + 8, address + 8); word(ram, address + 12, address + 8);
    word(ram, address + 16, 0); word(ram, address + 20, 0);
    word(ram, address + 24, 0); word(ram, address + 28, 0); word(ram, address + 36, 0);
    return FF_BOOT_OK;
}
