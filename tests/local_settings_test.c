#include "../src/runtime/local_settings.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint8_t ram[128];
static uint32_t word(unsigned offset)
{
    return (uint32_t)ram[offset] | (uint32_t)ram[offset + 1] << 8 | (uint32_t)ram[offset + 2] << 16 | (uint32_t)ram[offset + 3] << 24;
}
int main(void)
{
    uint32_t args[5] = {17, 16, 33, 8, 24};
    memset(ram, 0xa5, sizeof ram);
    assert(ff_query_misc_flags(ram, sizeof ram, args, 0x87654321) == FF_BOOT_OK);
    assert(word(16) == 4 && word(24) == 4 && word(33) == 0x87654321 && word(37) == 0);
    assert(ram[32] == 0xa5 && ram[41] == 0xa5);
    args[4] = 0;
    assert(ff_query_misc_flags(ram, sizeof ram, args, 0) == FF_BOOT_OK && word(33) == 0);
    memset(ram, 0xa5, sizeof ram);
    args[3] = 3; args[4] = 24;
    assert(ff_query_misc_flags(ram, sizeof ram, args, 1) == 0xc0000023u);
    assert(word(24) == 4 && word(16) == 0xa5a5a5a5 && word(33) == 0xa5a5a5a5);
    uint8_t before[sizeof ram]; memcpy(before, ram, sizeof ram);
    args[3] = 8; args[0] = 7;
    assert(ff_query_misc_flags(ram, sizeof ram, args, 1) == FF_BOOT_UNSUPPORTED);
    assert(!memcmp(before, ram, sizeof ram));
    args[0] = 17; args[2] = 124;
    assert(ff_query_misc_flags(ram, sizeof ram, args, 1) == FF_BOOT_INVALID);
    assert(!memcmp(before, ram, sizeof ram));
    args[2] = 33; args[4] = 127;
    assert(ff_query_misc_flags(ram, sizeof ram, args, 1) == FF_BOOT_INVALID);
    assert(!memcmp(before, ram, sizeof ram));
    puts("Local settings: explicit DWORD value/type/length, optional length, short buffer and unsupported/range cases passed");
    return 0;
}
