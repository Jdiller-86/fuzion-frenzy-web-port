/* Independent ABI cases with nonzero canaries; no original game bytes. */
#include "../src/runtime/kernel_objects.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[256];
static uint32_t word(unsigned offset)
{
    return (uint32_t)ram[offset] | (uint32_t)ram[offset + 1] << 8 |
        (uint32_t)ram[offset + 2] << 16 | (uint32_t)ram[offset + 3] << 24;
}
int main(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_initialize_dpc(ram, sizeof ram, 32, 0x12345678, 0x87654321) == FF_BOOT_OK);
    assert(ram[32] == 0x13 && ram[33] == 0 && ram[34] == 0 && ram[35] == 0xa5);
    assert(word(44) == 0x12345678 && word(48) == 0x87654321);
    assert(word(36) == 0xa5a5a5a5 && word(52) == 0xa5a5a5a5 && word(56) == 0xa5a5a5a5);
    assert(ram[31] == 0xa5 && ram[60] == 0xa5);
    for (unsigned type = 0; type < 2; ++type) {
        memset(ram, 0xa5, sizeof ram);
        assert(ff_initialize_timer(ram, sizeof ram, 64, type) == FF_BOOT_OK);
        assert(ram[64] == 8 + type && ram[65] == 0xa5 && ram[66] == 10 && ram[67] == 0);
        assert(word(68) == 0 && word(72) == 72 && word(76) == 72);
        assert(word(80) == 0 && word(84) == 0 && word(88) == 0 && word(92) == 0 && word(100) == 0);
        assert(word(96) == 0xa5a5a5a5 && ram[63] == 0xa5 && ram[104] == 0xa5);
    }
    memset(ram, 0xa5, sizeof ram);
    assert(ff_initialize_critical(ram, sizeof ram, 128) == FF_BOOT_OK);
    assert(ram[128] == 1 && ram[129] == 0xa5 && ram[130] == 4 && ram[131] == 0xa5);
    assert(word(132) == 0 && word(136) == 136 && word(140) == 136);
    assert(word(144) == UINT32_MAX && word(148) == 0 && word(152) == 0);
    assert(ram[127] == 0xa5 && ram[156] == 0xa5);
    for (unsigned address = 0; address < 5; ++address) {
        if (address == 4) continue;
        uint8_t before[sizeof ram]; memcpy(before, ram, sizeof ram);
        assert(ff_initialize_dpc(ram, sizeof ram, address, 1, 2) == FF_BOOT_INVALID);
        assert(ff_initialize_timer(ram, sizeof ram, address, 0) == FF_BOOT_INVALID);
        assert(ff_initialize_critical(ram, sizeof ram, address) == FF_BOOT_INVALID);
        assert(!memcmp(before, ram, sizeof ram));
    }
    uint8_t before[sizeof ram]; memcpy(before, ram, sizeof ram);
    assert(ff_initialize_dpc(ram, sizeof ram, 232, 1, 2) == FF_BOOT_INVALID);
    assert(ff_initialize_timer(ram, sizeof ram, 220, 0) == FF_BOOT_INVALID);
    assert(ff_initialize_timer(ram, sizeof ram, 64, 2) == FF_BOOT_INVALID);
    assert(ff_initialize_timer(ram, sizeof ram, UINT32_MAX - 3, 0) == FF_BOOT_INVALID);
    assert(ff_initialize_dpc(0, sizeof ram, 32, 1, 2) == FF_BOOT_INVALID);
    assert(ff_initialize_critical(ram, sizeof ram, 232) == FF_BOOT_INVALID);
    assert(ff_initialize_critical(0, sizeof ram, 128) == FF_BOOT_INVALID);
    assert(!memcmp(before, ram, sizeof ram));
    puts("Kernel object initialization: DPC, both timer types, preserved fields, range/alignment and failure atomicity passed");
    return 0;
}
