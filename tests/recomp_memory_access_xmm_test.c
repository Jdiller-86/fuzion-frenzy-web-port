/* Owned 16-byte checked-pointer permission/boundary fixture.
 * Includes only the source-owned memory header: it must not depend on the
 * pinned RecompXmm type. */
#include "../src/runtime/recomp_memory_access.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint8_t memory[64];
static uint32_t calls, last_address, last_bytes, last_access, last_site;
static int deny_write;

void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site)
{
    ++calls;
    last_address = address;
    last_bytes = (uint32_t)bytes;
    last_access = access;
    last_site = site;
    if (deny_write && access == 2u) return NULL;
    if ((address & 15u) || bytes != 16u || address > sizeof memory - 16u) return NULL;
    return memory + address;
}

int main(void)
{
    void *read = FF_MEMORY_SITE_POINTER(16u, 16u, 1u, 0x1234u);
    assert(read == memory + 16);
    assert(calls == 1 && last_address == 16 && last_bytes == 16 && last_access == 1 && last_site == 0x1234u);
    memset(read, 0x5a, 16);
    assert(memory[16] == 0x5a && memory[31] == 0x5a);

    void *write = FF_MEMORY_SITE_POINTER(32u, 16u, 2u, 0x5678u);
    assert(write == memory + 32);
    assert(last_address == 32 && last_bytes == 16 && last_access == 2 && last_site == 0x5678u);

    assert(FF_MEMORY_SITE_POINTER(1u, 16u, 1u, 0u) == NULL);
    assert(FF_MEMORY_SITE_POINTER(48u, 16u, 1u, 0u) == memory + 48);
    assert(FF_MEMORY_SITE_POINTER(49u, 16u, 1u, 0u) == NULL);
    assert(FF_MEMORY_SITE_POINTER(64u, 16u, 1u, 0u) == NULL);
    deny_write = 1;
    assert(FF_MEMORY_SITE_POINTER(32u, 16u, 2u, 0x9abcu) == NULL);
    assert(last_address == 32 && last_bytes == 16 && last_access == 2 && last_site == 0x9abcu);
    deny_write = 0;
    assert(FF_MEMORY_SITE_POINTER(32u, 16u, 2u, 0u) == memory + 32);
    puts("owned 16-byte checked pointer: permission, size, alignment, site and bounds passed");
    return 0;
}
