/* Compiles the transformed owned fixture with actual checked scalar/stack/bulk
 * accessors. Native/sanitizer/Wasm runs execute only these independent sources. */
#include "../src/runtime/device_registers.h"
#include "../src/runtime/recomp_memory_access.h"
#include "../src/runtime/virtual_memory.h"
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
static uint8_t ram[0x40000];
static ff_virtual_memory vm;
static ff_device_registers device;
static uint32_t host_last_site, host_last_address;
static jmp_buf fault_return;
static uint32_t fault_address, fault_access;
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site)
{
    host_last_site = site; host_last_address = address;
    if (device.ready) {
        if (ff_device_registers_commit(&device)) {
            fault_address = address; fault_access = access; longjmp(fault_return, 1);
        }
        if (!ff_device_registers_contains(&device, address, (uint32_t)bytes)) {
            uint32_t device_access = access == FF_VM_READ ? FF_DEVICE_ACCESS_READ
                                   : access == FF_VM_WRITE ? FF_DEVICE_ACCESS_WRITE
                                   : access == (FF_VM_READ | FF_VM_WRITE) ? FF_DEVICE_ACCESS_READWRITE : 0;
            uint32_t *slot = device_access ? ff_device_registers_access(&device, address, device_access, site) : NULL;
            if (!slot) { fault_address = address; fault_access = access; longjmp(fault_return, 1); }
            return slot;
        }
    }
    if (ff_vm_check(&vm, address, bytes, access)) { fault_address = address; fault_access = access; longjmp(fault_return, 1); }
    return ram + (bytes ? address : 0);
}
#include "owned-memory.inc"
int main(void)
{
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, 0x10000, 0x20000) == 0);
    assert(ff_vm_map_fixed(&vm, 0x10000, 0x1000, 4) == 0);
    assert(ff_vm_map_fixed(&vm, 0x11000, 0x1000, 2) == 0);
    assert(ff_device_registers_init(&device, 0xFD000000u, 0x1000u, 0xFD000000u, 0x55, 0xFD000004u, 0x66) == 0);
    FF_MEMORY_U32(0x10001, 2u) = 0x12345678;
    assert(!setjmp(fault_return));
    assert(owned_memory_and(0x00ff00ff, 0x10001) == 0x00340078);
    assert(FF_MEMORY_U32(0x10001, 1u) == 0x12345678);
    assert(owned_memory_and(UINT32_MAX, 0x11001) == 0);
    if (!setjmp(fault_return)) { (void)owned_memory_and(UINT32_MAX, 0x11fff); assert(0); }
    assert(fault_address == 0x11fff && fault_access == FF_VM_READ);
    assert(!setjmp(fault_return));
    assert(owned_memory_expression(0x10000, 0x10100, 0x10ff0) == 0x12345678);
    assert(FF_MEMORY_U32(0x10103, 1u) == 0x12345678 && FF_MEMORY_U16(0x10107, 1u) == 2);
    if (!setjmp(fault_return)) { (void)owned_memory_expression(0x10000, 0x11000, 0x10ff0); assert(0); }
    assert(fault_address == 0x11003 && fault_access == FF_VM_WRITE && ram[0x11003] == 0);
    if (!setjmp(fault_return)) { owned_memory_bulk(0x10ffc, 0x10000, 2); assert(0); }
    assert(fault_address == 0x10ffc && fault_access == FF_VM_WRITE && ram[0x10ffc] == 0);
    if (!setjmp(fault_return)) { owned_memory_bulk(0x10200, 0x12000, 1); assert(0); }
    assert(fault_address == 0x12000 && fault_access == FF_VM_READ && ram[0x10200] == 0);
    assert(!setjmp(fault_return));
    owned_memory_bulk(0x10200, 0x10000, 2);
    assert(!memcmp(ram + 0x10200, ram + 0x10000, 8));
    owned_memory_bulk(UINT32_MAX, UINT32_MAX, 0);
    assert(!memcmp(ram + 0x10200, ram + 0x10000, 8));
    FF_MEMORY_U32(0x10100, 2u) = 0xabcdef01;
    owned_memory_move(0x10200, 0x10100);
    assert(FF_MEMORY_U32(0x10200, 1u) == 0xabcdef01);
    /* The nearest emitted block label, not the function or the previous
     * function, identifies each transformed scalar access. */
    assert(owned_memory_site(0, 0x10001) == 0x12345678);
    assert(host_last_site == 0x00100000u && host_last_address == 0x10001u);
    assert(owned_memory_site(1, 0x10001) == 0x0012u);
    assert(host_last_site == 0x00100010u && host_last_address == 0x10004u);
    (void)owned_memory_site_float(0x10001);
    assert(host_last_site == 0x00100020u && host_last_address == 0x10009u);
    assert(FF_MEMORY_SITE_U32(0xFD000000u, 1u, 0x00100010u) == 0x55u);
    assert(device.event_count == 1 && device.events[0].address == 0xFD000000u &&
           device.events[0].site == 0x00100010u && device.events[0].direction == FF_DEVICE_READ);
    /* A generated store fills the staged slot; commit records its actual value. */
    FF_MEMORY_SITE_U32(0xFD000008u, 2u, 0x00100000u) = 0x77u;
    assert(device.pending && device.event_count == 1);
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.event_count == 2 && device.events[1].address == 0xFD000008u &&
           device.events[1].site == 0x00100000u && device.events[1].direction == FF_DEVICE_WRITE &&
           device.events[1].value == 0x77u);
    assert(*ff_device_registers_slot(&device, 0xFD000008u) == 0x77u);
    /* A second write to the same address keeps its own event value. */
    FF_MEMORY_SITE_U32(0xFD000008u, 2u, 0x00100010u) = 0x78u;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.event_count == 3 && device.events[2].value == 0x78u && device.events[2].site == 0x00100010u);
    /* Read-modify-write records the read and the actual stored write. */
    FF_MEMORY_SITE_U32(0xFD000000u, 3u, 0x00100020u) += 1u;
    assert(ff_device_registers_commit(&device) == 0);
    assert(device.event_count == 5);
    assert(device.events[3].direction == FF_DEVICE_READ && device.events[3].value == 0x55u);
    assert(device.events[4].direction == FF_DEVICE_WRITE && device.events[4].value == 0x56u);
    assert(*ff_device_registers_slot(&device, 0xFD000000u) == 0x56u);
    /* A following access commits the staged write before its own event. */
    FF_MEMORY_SITE_U32(0xFD00000Cu, 2u, 0x00100000u) = 0x9u;
    assert(device.pending && device.event_count == 5);
    assert(FF_MEMORY_SITE_U32(0xFD00000Cu, 1u, 0x00100010u) == 0x9u);
    assert(device.event_count == 7 && device.events[5].direction == FF_DEVICE_WRITE &&
           device.events[5].value == 0x9u && device.events[6].direction == FF_DEVICE_READ &&
           device.events[6].value == 0x9u);
    /* An unknown read half fails before a write is staged or an event added. */
    if (!setjmp(fault_return)) { FF_MEMORY_SITE_U32(0xFD000010u, 3u, 0x00100020u) += 1u; assert(0); }
    assert(fault_address == 0xFD000010u && fault_access == (FF_VM_READ | FF_VM_WRITE) && device.event_count == 7);
    if (!setjmp(fault_return)) { (void)FF_MEMORY_U32(0xFD000000u, 1u); assert(0); }
    assert(fault_address == 0xFD000000u && fault_access == FF_VM_READ && device.event_count == 7);
    puts("Owned transformed scalar/unaligned/stack/bulk permission and block-site checks passed");
    return 0;
}
