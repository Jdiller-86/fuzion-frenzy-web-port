/* Unconnected synthetic query fixture.
 *
 * Models only the memory contract an AV query would need: a 4-byte write to a
 * guest pointer succeeds only when the checked virtual-memory window grants
 * write access; on denial the backing bytes are unchanged. It is not linked to
 * the probe, the kernel-service handler or any runtime path.
 */
#include "../src/runtime/virtual_memory.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_ONLY_BASE 0x00010000u
#define READ_WRITE_BASE 0x00020000u
#define UNMAPPED_BASE 0x00030000u
#define REGION_END 0x00040000u

static uint8_t ram[REGION_END];
static ff_virtual_memory vm;

static uint32_t query_write(uint32_t pointer, uint32_t word)
{
    if (!pointer || (pointer & 3u) || ff_vm_check(&vm, pointer, 4, FF_VM_WRITE)) return 0;
    memcpy(ram + pointer, &word, 4);
    return 1;
}

static void fill(uint32_t base, uint8_t value)
{
    memset(ram + base, value, 8);
}

static void assert_unchanged(uint32_t base, uint8_t value)
{
    for (uint32_t i = 0; i < 8; ++i) assert(ram[base + i] == value);
}

int main(void)
{
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, READ_ONLY_BASE, REGION_END - READ_ONLY_BASE) == 0);
    assert(ff_vm_map_fixed(&vm, READ_ONLY_BASE, READ_WRITE_BASE - READ_ONLY_BASE, 0x02u) == 0);
    assert(ff_vm_map_fixed(&vm, READ_WRITE_BASE, UNMAPPED_BASE - READ_WRITE_BASE, 0x04u) == 0);

    /* A granted query writes exactly four little-endian bytes. */
    fill(READ_WRITE_BASE, 0xAA);
    assert(query_write(READ_WRITE_BASE, 0x00400104u) == 1);
    assert(ram[READ_WRITE_BASE + 0] == 0x04);
    assert(ram[READ_WRITE_BASE + 1] == 0x01);
    assert(ram[READ_WRITE_BASE + 2] == 0x40);
    assert(ram[READ_WRITE_BASE + 3] == 0x00);
    for (uint32_t i = 4; i < 8; ++i) assert(ram[READ_WRITE_BASE + i] == 0xAA);

    /* Read-only, unaligned, null and unmapped pointers are denied with no
     * partial write. */
    fill(READ_ONLY_BASE, 0xAA);
    assert(query_write(READ_ONLY_BASE, 0x00400104u) == 0);
    assert_unchanged(READ_ONLY_BASE, 0xAA);

    fill(READ_WRITE_BASE, 0xAA);
    assert(query_write(READ_WRITE_BASE + 1u, 0x00400104u) == 0);
    assert_unchanged(READ_WRITE_BASE, 0xAA);

    assert(query_write(0, 0x00400104u) == 0);

    fill(UNMAPPED_BASE, 0xAA);
    assert(query_write(UNMAPPED_BASE, 0x00400104u) == 0);
    assert_unchanged(UNMAPPED_BASE, 0xAA);

    puts("synthetic AV query fixture: exact four-byte write and unchanged denial passed");
    return 0;
}
