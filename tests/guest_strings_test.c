#include "guest_strings.h"
#include "thread_bootstrap.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

enum {
    RAM_BYTES = 2 * FF_VM_GRANULARITY,
    DESCRIPTOR = FF_VM_GRANULARITY,
    PAYLOAD = DESCRIPTOR + FF_VM_PAGE,
};
static uint8_t ram[RAM_BYTES];
static ff_virtual_memory vm;

static void put16(uint32_t address, uint16_t value)
{
    ram[address] = (uint8_t)value;
    ram[address + 1] = (uint8_t)(value >> 8);
}

static void put32(uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (8 * i));
}

static void descriptor(uint16_t bytes, uint16_t maximum, uint32_t buffer)
{
    put16(DESCRIPTOR, bytes);
    put16(DESCRIPTOR + 2, maximum);
    put32(DESCRIPTOR + 4, buffer);
}

static void reset(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, DESCRIPTOR, 3 * FF_VM_PAGE) == 0);
    assert(ff_vm_map_fixed(&vm, DESCRIPTOR, FF_VM_PAGE, 4) == 0); /* PAGE_READWRITE */
    assert(ff_vm_map_fixed(&vm, PAYLOAD, FF_VM_PAGE, 2) == 0); /* PAGE_READONLY */
}

static void success_and_empty(void)
{
    reset(); descriptor(7, 12, PAYLOAD);
    uint32_t length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, DESCRIPTOR, &length) == 0 && length == 7);
    descriptor(0, 0, 0);
    length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, DESCRIPTOR, &length) == 0 && length == 0);
}

static void failures_are_transactional(void)
{
    uint32_t length;
    reset(); descriptor(1025, 1025, PAYLOAD); length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, DESCRIPTOR, &length) == FF_BOOT_UNSUPPORTED && length == 0xdeadbeef);
    descriptor(5, 4, PAYLOAD); length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, DESCRIPTOR, &length) == FF_BOOT_INVALID && length == 0xdeadbeef);
    descriptor(8, 8, 0); length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, DESCRIPTOR, &length) == FF_BOOT_INVALID && length == 0xdeadbeef);
    descriptor(8, 8, PAYLOAD + FF_VM_PAGE); length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, DESCRIPTOR, &length) == FF_VM_ACCESS_VIOLATION && length == 0xdeadbeef);
    length = 0xdeadbeef;
    assert(ff_debug_ansi_length(&vm, RAM_BYTES, &length) == FF_VM_ACCESS_VIOLATION && length == 0xdeadbeef);
}

int main(void)
{
    success_and_empty();
    failures_are_transactional();
    return 0;
}
