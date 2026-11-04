/* Owned FILETIME-to-TIME_FIELDS scenarios; no host timezone or locale. */
#include "../src/runtime/time_fields.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>

static void expect(uint64_t time, uint16_t year, uint16_t month, uint16_t day,
                   uint16_t hour, uint16_t minute, uint16_t second, uint16_t millis,
                   uint16_t weekday)
{
    uint16_t fields[8] = {0};
    assert(ff_time_to_fields(time, fields) == 0);
    assert(fields[0] == year && fields[1] == month && fields[2] == day);
    assert(fields[3] == hour && fields[4] == minute && fields[5] == second);
    assert(fields[6] == millis && fields[7] == weekday);
}

int main(void)
{
    expect(0, 1601, 1, 1, 0, 0, 0, 0, 1);
    expect(116444736000000000ull, 1970, 1, 1, 0, 0, 0, 0, 4);
    expect(125912036967890000ull, 2000, 1, 1, 12, 34, 56, 789, 6);
    expect(133536384000000000ull, 2024, 2, 29, 0, 0, 0, 0, 4);
    expect(94404960000000000ull, 1900, 2, 28, 0, 0, 0, 0, 3);
    expect(94405824000000000ull, 1900, 3, 1, 0, 0, 0, 0, 4);
    uint16_t fields[8] = {0};
    assert(ff_time_to_fields(UINT64_MAX, fields) == FF_BOOT_UNSUPPORTED);
    assert(ff_time_to_fields(0, NULL) == FF_BOOT_INVALID);
    puts("owned FILETIME-to-TIME_FIELDS: epochs, leap day, non-leap 1900, bounds and null rejection passed");
    return 0;
}
