#include "debug_service.h"
#include "thread_bootstrap.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

enum { RAM_BYTES = 2 * FF_VM_GRANULARITY, DESCRIPTOR = FF_VM_GRANULARITY,
       PAYLOAD = DESCRIPTOR + FF_VM_PAGE };
static uint8_t ram[RAM_BYTES];
static ff_virtual_memory vm;
static ff_debug_service service;

static void put16(uint32_t at, uint16_t value)
{ ram[at] = (uint8_t)value; ram[at + 1] = (uint8_t)(value >> 8); }
static void descriptor(uint16_t bytes, uint16_t maximum, uint32_t buffer)
{
    put16(DESCRIPTOR, bytes); put16(DESCRIPTOR + 2, maximum);
    for (unsigned i = 0; i < 4; ++i) ram[DESCRIPTOR + 4 + i] = (uint8_t)(buffer >> (i * 8));
}
static void reset(void)
{
    memset(ram, 0, sizeof ram);
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, DESCRIPTOR, 3 * FF_VM_PAGE) == 0);
    assert(ff_vm_map_fixed(&vm, DESCRIPTOR, FF_VM_PAGE, 4) == 0);
    assert(ff_vm_map_fixed(&vm, PAYLOAD, FF_VM_PAGE, 2) == 0);
    descriptor(7, 12, PAYLOAD);
    ff_debug_service_init(&service);
}
static uint32_t word(unsigned field) { return ff_debug_service_window_word(&service, field); }

static void defaults_and_eighth_window(void)
{
    reset();
    for (unsigned i = 0; i < 8; ++i) assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == 0);
    assert(word(0) == 0 && word(1) == 8 && word(2) == 8);
    assert(word(3) == 0 && word(4) == 8 && word(5) == 8);
    assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == FF_BOOT_UNSUPPORTED);
    assert(word(5) == 8 && service.last_bytes == 0);

    reset();
    assert(ff_debug_service_profile(&service, 8) == 0);
    for (unsigned i = 0; i < 16; ++i) assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == 0);
    assert(word(0) == 8 && word(1) == 8 && word(2) == 16);
    assert(word(3) == 8 && word(4) == 8 && word(5) == 16);
    assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == FF_BOOT_UNSUPPORTED);
    assert(word(5) == 16 && service.last_bytes == 0);
}

static void selection_is_validated_and_sealed(void)
{
    reset();
    assert(ff_debug_service_profile(&service, 1) == FF_BOOT_INVALID);
    assert(ff_debug_service_profile(&service, 16) == FF_BOOT_INVALID);
    ff_debug_service_begin(&service);
    assert(ff_debug_service_profile(&service, 8) == FF_BOOT_INVALID);
    reset();
    assert(ff_debug_service_profile(&service, 8) == 0);
    assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == 0);
    assert(ff_debug_service_profile(&service, 0) == FF_BOOT_INVALID);
    reset();
    assert(ff_debug_service_call(&service, 2, &vm, DESCRIPTOR) == FF_BOOT_UNSUPPORTED);
    assert(ff_debug_service_profile(&service, 8) == FF_BOOT_INVALID);
}

static void refusals_do_not_advance(void)
{
    reset();
    assert(ff_debug_service_call(&service, 2, &vm, DESCRIPTOR) == FF_BOOT_UNSUPPORTED);
    assert(word(5) == 0 && service.last_bytes == 0);
    assert(ff_debug_service_call(&service, 1, &vm, RAM_BYTES) == FF_VM_ACCESS_VIOLATION);
    assert(word(5) == 0 && service.last_bytes == 0);
    descriptor(8, 7, PAYLOAD);
    assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == FF_BOOT_INVALID);
    assert(word(5) == 0 && service.last_bytes == 0);
    descriptor(8, 8, PAYLOAD + FF_VM_PAGE);
    assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == FF_VM_ACCESS_VIOLATION);
    assert(word(5) == 0 && service.last_bytes == 0);
    descriptor(1025, 1025, PAYLOAD);
    assert(ff_debug_service_call(&service, 1, &vm, DESCRIPTOR) == FF_BOOT_UNSUPPORTED);
    assert(word(5) == 0 && service.last_bytes == 0);
}

int main(void)
{
    defaults_and_eighth_window();
    selection_is_validated_and_sealed();
    refusals_do_not_advance();
    return 0;
}
