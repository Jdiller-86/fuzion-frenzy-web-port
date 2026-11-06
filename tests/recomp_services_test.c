/* Owned INT 0x2D debug-service record: the pinned translator's documented
 * service-1 ANSI_STRING print with no guest-register effect. */
#include "../src/runtime/recomp_services.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void put16(uint8_t *ram, uint32_t at, uint32_t value)
{
    ram[at] = (uint8_t)value;
    ram[at + 1] = (uint8_t)(value >> 8);
}

static void put32(uint8_t *ram, uint32_t at, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[at + i] = (uint8_t)(value >> (i * 8));
}

int main(void)
{
    uint8_t ram[256];
    memset(ram, 0, sizeof ram);
    ff_debug_service_memory memory = { ram, (uint32_t)sizeof ram };
    ff_debug_service_log log;
    ff_debug_service_reset(&log);
    assert(log.count == 0 && log.dropped == 0);

    /* Service 1 keeps the length and buffer of a readable ANSI_STRING. */
    put16(ram, 8, 5);
    put16(ram, 10, 6);
    put32(ram, 12, 20);
    memcpy(ram + 20, "hello", 5);
    assert(ff_debug_service_record(&log, &memory, 1, 8) == FF_DEBUG_SERVICE_OK);
    assert(log.count == 1);
    assert(log.calls[0].service == 1 && log.calls[0].argument == 8);
    assert(log.calls[0].length == 5 && log.calls[0].buffer == 20);
    assert(log.calls[0].textBytes == 5 && !memcmp(log.calls[0].text, "hello", 5));

    /* A longer string is copied at call time, bounded to the fixed record. */
    put16(ram, 100, 40); put16(ram, 102, 41); put32(ram, 104, 120);
    for (unsigned i = 0; i < 40; ++i) ram[120 + i] = (uint8_t)('A' + i % 26u);
    assert(ff_debug_service_record(&log, &memory, 1, 100) == FF_DEBUG_SERVICE_OK);
    assert(log.count == 2 && log.calls[1].length == 40);
    assert(log.calls[1].textBytes == FF_DEBUG_SERVICE_TEXT_BYTES);
    assert(!memcmp(log.calls[1].text, ram + 120, FF_DEBUG_SERVICE_TEXT_BYTES));

    /* A non-print service is recorded but carries no string descriptor. */
    assert(ff_debug_service_record(&log, &memory, 7, 8) == FF_DEBUG_SERVICE_OK);
    assert(log.count == 3);
    assert(log.calls[2].service == 7 && log.calls[2].length == 0 && log.calls[2].buffer == 0);
    assert(log.calls[2].textBytes == 0);

    /* A null argument is the documented early return. */
    assert(ff_debug_service_record(&log, &memory, 1, 0) == FF_DEBUG_SERVICE_OK);
    assert(log.count == 4 && log.calls[3].length == 0);

    /* A truncated descriptor or string is refused rather than guessed. */
    assert(ff_debug_service_record(&log, &memory, 1, 250) == FF_DEBUG_SERVICE_UNREADABLE);
    assert(log.count == 5 && log.calls[4].length == 0 && log.calls[4].buffer == 0);
    put16(ram, 40, 32);
    put32(ram, 44, 240);
    assert(ff_debug_service_record(&log, &memory, 1, 40) == FF_DEBUG_SERVICE_UNREADABLE);
    assert(log.count == 6 && log.calls[5].length == 0 && log.calls[5].textBytes == 0);

    /* The log is bounded and counts what it drops. */
    while (log.count < FF_DEBUG_SERVICE_CAPACITY)
        assert(ff_debug_service_record(&log, &memory, 7, 0) == FF_DEBUG_SERVICE_OK);
    assert(ff_debug_service_record(&log, &memory, 7, 0) == FF_DEBUG_SERVICE_DROPPED);
    assert(log.dropped == 1 && log.count == FF_DEBUG_SERVICE_CAPACITY);

    /* A missing log or aperture is refused, not dereferenced. */
    assert(ff_debug_service_record(0, &memory, 1, 8) == FF_DEBUG_SERVICE_INVALID);
    ff_debug_service_memory absent = { 0, 0 };
    ff_debug_service_reset(&log);
    assert(ff_debug_service_record(&log, &absent, 1, 8) == FF_DEBUG_SERVICE_UNREADABLE);

    ff_debug_service_reset(&log);
    assert(log.count == 0 && log.dropped == 0 && log.calls[0].service == 0);

    puts("Owned debug-service record and refusal checks passed");
    return 0;
}
