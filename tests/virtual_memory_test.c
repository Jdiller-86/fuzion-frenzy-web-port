/* Owned guest-memory scenarios; no original instructions or native title run. */
#include "../src/runtime/virtual_memory.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[2u * 1024u * 1024u], saved_ram[sizeof ram];
static ff_virtual_memory vm, saved_vm;
enum { BP = 0x10000, SP = 0x10004, OLD = 0x10008 };
static void put(uint32_t p, uint32_t value) { for (unsigned i = 0; i < 4; ++i) ram[p + i] = (uint8_t)(value >> (i * 8)); }
static uint32_t get(uint32_t p) { return ram[p] | (uint32_t)ram[p+1] << 8 | (uint32_t)ram[p+2] << 16 | (uint32_t)ram[p+3] << 24; }
static void reset(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, 0x10000, 0x10000) == 0);
    assert(ff_vm_map_fixed(&vm, 0x10000, 0x10000, 4) == 0);
}
static void unchanged(void) { assert(!memcmp(&vm, &saved_vm, sizeof vm) && !memcmp(ram, saved_ram, sizeof ram)); }
static void save(void) { saved_vm = vm; memcpy(saved_ram, ram, sizeof ram); }
static uint32_t reserve(uint32_t size)
{
    put(BP, 0); put(SP, size);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 4) == 0);
    return get(BP);
}
static void reserve_commit_and_protection(void)
{
    reset(); uint32_t base = reserve(1024u * 1024u);
    assert(base == 0x20000 && get(SP) == 1024u * 1024u && ram[base] == 0xa5);
    assert(ff_vm_check(&vm, base, 1, FF_VM_READ) == FF_VM_ACCESS_VIOLATION);
    put(BP, base + 3); put(SP, 4096);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == 0 && get(BP) == base && get(SP) == 8192);
    for (unsigned i = 0; i < 8192; ++i) assert(ram[base + i] == 0);
    assert(ram[base + 8192] == 0xa5);
    assert(ff_vm_check(&vm, base, 8192, FF_VM_READ | FF_VM_WRITE) == 0);
    assert(ff_vm_check(&vm, base + 8191, 2, FF_VM_READ) == FF_VM_ACCESS_VIOLATION);
    ram[base] = 0x72;
    put(BP, base); put(SP, 12288);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == 0 && ram[base] == 0x72 && !ram[base + 8192]);
    put(BP, base + 1); put(SP, 1);
    assert(ff_vm_protect(&vm, BP, SP, 2, OLD) == 0 && get(BP) == base && get(SP) == 4096 && get(OLD) == 4);
    assert(ff_vm_check(&vm, base, 1, FF_VM_READ) == 0 && ff_vm_check(&vm, base, 1, FF_VM_WRITE) == FF_VM_ACCESS_VIOLATION);
    put(BP, base); put(SP, 4096);
    assert(ff_vm_protect(&vm, BP, SP, 1, OLD) == 0 && get(OLD) == 2);
    assert(ff_vm_check(&vm, base, 1, FF_VM_READ) == FF_VM_ACCESS_VIOLATION);
    put(BP, base); put(SP, 16384); save();
    assert(ff_vm_protect(&vm, BP, SP, 4, OLD) == FF_VM_NOT_COMMITTED); unchanged();
    put(BP, base); put(SP, 4096);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == 0 && ram[base] == 0x72);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x80000, 4) == 0 && ram[base] == 0x72);
}
static void decommit_release_and_split(void)
{
    reset(); uint32_t base = reserve(0x30000);
    put(BP, base); put(SP, 0x30000);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == 0);
    ram[base] = 0x79;
    put(BP, base); put(SP, 4096);
    assert(ff_vm_free(&vm, BP, SP, 0x4000) == 0 && vm.owner[base / 4096]);
    assert(ff_vm_check(&vm, base, 1, FF_VM_READ) != 0);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == 0 && ram[base] == 0);
    put(BP, base + 0x10000); put(SP, 0x10000);
    assert(ff_vm_free(&vm, BP, SP, 0x8000) == 0);
    assert(!vm.owner[(base + 0x10000) / 4096] && vm.owner[base / 4096] != vm.owner[(base + 0x20000) / 4096]);
    assert(reserve(0x10000) == base + 0x10000);
    put(BP, base + 4096); put(SP, 0); save();
    assert(ff_vm_free(&vm, BP, SP, 0x8000) == FF_VM_NOT_AT_BASE); unchanged();
    put(BP, base); put(SP, 0);
    assert(ff_vm_free(&vm, BP, SP, 0x8000) == 0 && get(SP) == 0x10000);
    assert(!vm.owner[base / 4096] && ff_vm_check(&vm, base, 1, FF_VM_READ) != 0);
    save(); assert(ff_vm_free(&vm, BP, SP, 0x8000) == FF_VM_NOT_ALLOCATED); unchanged();
    put(BP, base + 0x20000); put(SP, 0);
    assert(ff_vm_free(&vm, BP, SP, 0x4000) == 0 && get(SP) == 0x10000);
    assert(vm.owner[(base + 0x20000) / 4096] && !vm.committed[(base + 0x20000) / 4096]);
}
static void errors_are_transactional(void)
{
    reset(); uint32_t base = reserve(0x2000);
    const uint32_t types[] = {0, 0x4000, 0x82000, 0xffffffff};
    for (unsigned i = 0; i < sizeof types / sizeof *types; ++i) {
        put(BP, 0); put(SP, 4096); save();
        assert(ff_vm_allocate(&vm, BP, 0, SP, types[i], 4) == FF_BOOT_INVALID); unchanged();
    }
    put(BP, 0); put(SP, 4096); save();
    assert(ff_vm_allocate(&vm, BP, 22, SP, 0x2000, 4) == FF_BOOT_INVALID); unchanged();
    assert(ff_vm_allocate(&vm, BP, 1, SP, 0x2000, 4) == FF_BOOT_UNSUPPORTED); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 0x100) == FF_VM_BAD_PROTECTION); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 0x104) == FF_BOOT_UNSUPPORTED); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 8) == FF_BOOT_UNSUPPORTED); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x802000, 4) == FF_BOOT_UNSUPPORTED); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x80000, 4) == FF_BOOT_UNSUPPORTED); unchanged();
    put(BP, 0xfffff000); put(SP, 8192); save();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 4) == FF_BOOT_INVALID); unchanged();
    put(BP, 0x10000000); put(SP, 4096); save();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 4) == FF_BOOT_UNSUPPORTED); unchanged();
    put(BP, 0x10000); put(SP, 4096); save();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 4) == FF_VM_CONFLICT); unchanged();
    assert(ff_vm_free(&vm, BP, SP, 0x8000) == FF_VM_CONFLICT); unchanged();
    assert(ff_vm_allocate(&vm, BP + 1, 0, SP, 0x2000, 4) == FF_VM_ACCESS_VIOLATION); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, BP, 0x2000, 4) == FF_VM_ACCESS_VIOLATION); unchanged();
    put(BP, base + 4096); put(SP, 8192); save();
    assert(ff_vm_free(&vm, BP, SP, 0x8000) == FF_VM_UNABLE_TO_FREE); unchanged();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == FF_VM_CONFLICT); unchanged();
    put(BP, base); put(SP, 0); save();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == FF_BOOT_INVALID); unchanged();
    assert(ff_vm_check(&vm, sizeof ram, 0, FF_VM_READ) == 0);
    assert(ff_vm_check(&vm, UINT32_MAX, 0, FF_VM_READ) == 0);
    assert(ff_vm_check(&vm, sizeof ram, 1, FF_VM_READ) == FF_VM_ACCESS_VIOLATION);
    assert(ff_vm_check(&vm, 0, UINT64_MAX, FF_VM_READ) == FF_VM_ACCESS_VIOLATION);
    assert(ff_vm_check(&vm, base, 1, 8) == FF_VM_ACCESS_VIOLATION);
}
static void capacity_and_fixed_ownership(void)
{
    reset();
    for (unsigned i = 0; i < FF_VM_REGIONS - 1; ++i) assert(ff_vm_claim_fixed(&vm, 0x20000 + i * 4096, 4096) == 0);
    put(BP, 0); put(SP, 4096); save();
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x2000, 4) == FF_BOOT_NO_MEMORY); unchanged();
    assert(ff_vm_claim_fixed(&vm, 0x10000, 4096) == FF_VM_CONFLICT);
    assert(ff_vm_map_fixed(&vm, 0x20000, 8192, 4) == FF_VM_CONFLICT);
    assert(ff_vm_init(&vm, ram, sizeof ram - 1) == FF_BOOT_INVALID);
}
static void implicit_reservation_and_output_overlap(void)
{
    reset(); put(BP, 0); put(SP, 4097);
    assert(ff_vm_allocate(&vm, BP, 0, SP, 0x1000, 4) == 0 && get(SP) == 8192);
    uint32_t base = get(BP);
    assert(ff_vm_check(&vm, base, 8192, FF_VM_READ | FF_VM_WRITE) == 0);
    put(base, base); put(base + 4, 4096); save();
    assert(ff_vm_free(&vm, base, base + 4, 0x8000) == FF_BOOT_UNSUPPORTED); unchanged();
    assert(ff_vm_protect(&vm, base, base + 4, 2, OLD) == FF_BOOT_UNSUPPORTED); unchanged();
    put(BP, base); put(SP, 4096); save();
    assert(ff_vm_protect(&vm, BP, SP, 0x204, OLD) == FF_BOOT_UNSUPPORTED); unchanged();
}
int main(void)
{
    reserve_commit_and_protection(); decommit_release_and_split(); errors_are_transactional(); capacity_and_fixed_ownership(); implicit_reservation_and_output_overlap();
    puts("Owned virtual-memory reservation, commit, permission, split/release and failure scenarios passed");
    return 0;
}
