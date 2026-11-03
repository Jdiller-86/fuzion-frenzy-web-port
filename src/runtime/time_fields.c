#include "time_fields.h"
#include "thread_bootstrap.h"

/* Howard Hinnant's civil-from-days algorithm; z is days since 1970-01-01. */
static void civil_from_days(int64_t z, int64_t *year, unsigned *month, unsigned *day)
{
    z += 719468;
    const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460u + doe / 36524u - doe / 146096u) / 365u;
    const int64_t y = (int64_t)yoe + era * 400;
    const unsigned doy = doe - (365u * yoe + yoe / 4u - yoe / 100u);
    const unsigned mp = (5u * doy + 2u) / 153u;
    const unsigned d = doy - (153u * mp + 2u) / 5u + 1u;
    const unsigned m = mp < 10u ? mp + 3u : mp - 9u;
    *year = y + (m <= 2u);
    *month = m;
    *day = d;
}

uint32_t ff_time_to_fields(uint64_t time, uint16_t fields[8])
{
    if (!fields) return FF_BOOT_INVALID;
    const uint64_t seconds = time / 10000000u;
    const uint64_t subsecond = time % 10000000u;
    const uint64_t days = seconds / 86400u;
    const uint32_t second_of_day = (uint32_t)(seconds % 86400u);
    const unsigned hour = second_of_day / 3600u;
    const unsigned minute = (second_of_day / 60u) % 60u;
    const unsigned second = second_of_day % 60u;
    const unsigned millis = (unsigned)(subsecond / 10000u);

    int64_t year;
    unsigned month, day;
    /* 1601-01-01 is 134774 days before 1970-01-01. */
    civil_from_days((int64_t)days - 134774, &year, &month, &day);
    if (year < 1601 || year > 32767) return FF_BOOT_UNSUPPORTED;

    fields[0] = (uint16_t)year;
    fields[1] = (uint16_t)month;
    fields[2] = (uint16_t)day;
    fields[3] = (uint16_t)hour;
    fields[4] = (uint16_t)minute;
    fields[5] = (uint16_t)second;
    fields[6] = (uint16_t)millis;
    fields[7] = (uint16_t)((days + 1u) % 7u); /* 1601-01-01 was a Monday. */
    return 0;
}
