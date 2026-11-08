#include "../runtime/core/unnamed_mutants.h"
#include "../runtime/core/thread_bootstrap.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RAM_BYTES (32u * FF_VM_PAGE)
#define OUT_A 0x11000u
#define OUT_B 0x12000u

static ff_virtual_memory vm;
static ff_unnamed_mutants mutants;
static uint8_t ram[RAM_BYTES];

static void setup(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, 0x10000, 0x10000) == 0);
    assert(ff_vm_map_fixed(&vm, 0x10000, FF_VM_PAGE, 2) == 0); /* read only */
    assert(ff_vm_map_fixed(&vm, 0x11000, FF_VM_PAGE, 4) == 0); /* writable */
    assert(ff_vm_map_fixed(&vm, 0x12000, FF_VM_PAGE, 4) == 0);
    assert(ff_vm_map_fixed(&vm, 0x13000, FF_VM_PAGE, 2) == 0);
    assert(ff_unnamed_mutants_init(&mutants, &vm) == 0);
}

static void atomic_failure(uint32_t output, uint32_t attrs, uint32_t owner, uint32_t expected)
{
    ff_unnamed_mutants before = mutants;
    uint8_t bytes[sizeof ram];
    memcpy(bytes, ram, sizeof ram);
    assert(ff_unnamed_mutants_create(&mutants, output, attrs, owner) == expected);
    assert(memcmp(&before, &mutants, sizeof before) == 0);
    assert(memcmp(bytes, ram, sizeof ram) == 0);
}

int main(void)
{
    setup();
    assert(ff_unnamed_mutants_create(&mutants, OUT_A, 0, 0) == 0);
    assert(ram[OUT_A] == 4 && ram[OUT_A + 1] == 0 && ram[OUT_A + 2] == 8 && ram[OUT_A + 3] == 0);
    assert(ram[OUT_A - 1] == 0xa5 && ram[OUT_A + 4] == 0xa5);
    assert(mutants.count == 1 && mutants.handles[0] == FF_MUTANT_HANDLE_BASE + 4);
    assert(ff_unnamed_mutants_create(&mutants, OUT_B, 0, 0) == 0);
    assert(ram[OUT_B] == 8 && ram[OUT_B + 1] == 0 && ram[OUT_B + 2] == 8 && ram[OUT_B + 3] == 0);
    assert(mutants.count == 2 && mutants.handles[1] == FF_MUTANT_HANDLE_BASE + 8);

    atomic_failure(OUT_A, 1, 0, FF_BOOT_UNSUPPORTED);
    atomic_failure(OUT_A, 0, 1, FF_BOOT_UNSUPPORTED);
    atomic_failure(0, 0, 0, FF_BOOT_INVALID);
    atomic_failure(OUT_A + 1, 0, 0, FF_BOOT_INVALID);
    atomic_failure(RAM_BYTES, 0, 0, FF_VM_ACCESS_VIOLATION);
    atomic_failure(0x10000u, 0, 0, FF_VM_ACCESS_VIOLATION); /* read-only */
    atomic_failure(0x13000u, 0, 0, FF_VM_ACCESS_VIOLATION); /* read-only */
    atomic_failure(0x14000u, 0, 0, FF_VM_ACCESS_VIOLATION); /* unmapped */

    /* An aligned DWORD at the final four bytes of a writable page is valid. */
    assert(ff_unnamed_mutants_create(&mutants, 0x11ffcu, 0, 0) == 0);
    assert(ram[0x11ffcu] == 12 && ram[0x11ffdu] == 0 && ram[0x11ffeu] == 8 && ram[0x11fffu] == 0);
    assert(ram[0x11ffbu] == 0xa5);

    assert(ff_unnamed_mutants_create(&mutants, OUT_A, 0, 0) == 0);
    assert(mutants.count == 4);
    assert(mutants.handles[2] == FF_MUTANT_HANDLE_BASE + 12 && mutants.handles[3] == FF_MUTANT_HANDLE_BASE + 16);
    atomic_failure(OUT_B, 0, 0, FF_MUTANT_NO_SLOTS);

    ff_unnamed_mutants corrupted = mutants;
    corrupted.handles[1] += 4;
    ff_unnamed_mutants before = corrupted;
    uint8_t bytes[sizeof ram]; memcpy(bytes, ram, sizeof ram);
    assert(ff_unnamed_mutants_create(&corrupted, OUT_B, 0, 0) == FF_BOOT_INVALID);
    assert(memcmp(&corrupted, &before, sizeof before) == 0 && memcmp(bytes, ram, sizeof ram) == 0);
    corrupted = mutants; corrupted.count = FF_MUTANT_CAPACITY + 1;
    before = corrupted; memcpy(bytes, ram, sizeof ram);
    assert(ff_unnamed_mutants_create(&corrupted, OUT_B, 0, 0) == FF_BOOT_INVALID);
    assert(memcmp(&corrupted, &before, sizeof before) == 0 && memcmp(bytes, ram, sizeof ram) == 0);

    assert(ff_unnamed_mutants_init(NULL, &vm) == FF_BOOT_INVALID);
    assert(ff_unnamed_mutants_init(&mutants, NULL) == FF_BOOT_INVALID);
    ff_virtual_memory invalid = vm; invalid.ram = NULL;
    assert(ff_unnamed_mutants_init(&mutants, &invalid) == FF_BOOT_INVALID);
    assert(ff_unnamed_mutants_init((ff_unnamed_mutants *)&vm, &vm) == FF_BOOT_INVALID);
    uint8_t alias_ram[sizeof(ff_virtual_memory) + FF_VM_GRANULARITY];
    ff_virtual_memory alias_vm;
    assert(ff_vm_init(&alias_vm, alias_ram, FF_VM_GRANULARITY) == FF_BOOT_INVALID);
    assert(ff_vm_init(&alias_vm, alias_ram, 2u * FF_VM_GRANULARITY) == 0);
    assert(ff_unnamed_mutants_init((ff_unnamed_mutants *)alias_ram, &alias_vm) == FF_BOOT_INVALID);
    puts("unnamed mutants: bounded creation checks passed");
    return 0;
}
