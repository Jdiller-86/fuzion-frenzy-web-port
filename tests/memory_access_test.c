/* Compiles the transformed owned fixture with actual checked scalar/stack/bulk
 * accessors. Native/sanitizer/Wasm runs execute only these independent sources. */
#include "../src/runtime/recomp_memory_access.h"
#include "../src/runtime/virtual_memory.h"
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
static uint8_t ram[0x40000];
static ff_virtual_memory vm;
static jmp_buf fault_return;
static uint32_t fault_address, fault_access;
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access)
{
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
    FF_MEMORY_U32(0x10001, 2u) = 0x12345678;
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
    puts("Owned transformed scalar/unaligned/stack/bulk permission checks passed");
    return 0;
}
