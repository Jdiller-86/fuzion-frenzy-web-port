/* Compiled generated-vector access against the real virtual-memory component.
 *
 * The emitted original code performs a checked 16-byte vector access as
 *     (*(RecompXmm*)FF_MEMORY_SITE_POINTER((address), 16u, access, site))
 * so this fixture compiles that exact form and resolves the hook with
 * ff_vm_check over the owned identity-mapped window. RecompXmm mirrors the
 * pinned translation type; the memory header itself stays independent of it. */
#include "../src/runtime/recomp_memory_access.h"
#include "../src/runtime/virtual_memory.h"
#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef union {
    uint8_t u8[16];
    uint32_t u32[4];
    float f32[4];
} RecompXmm;

static uint8_t ram[0x40000];
static ff_virtual_memory vm;
static uint32_t last_address, last_bytes, last_access, last_site, hook_calls, alignment_faults;
static jmp_buf alignment_fault;

#define FF_READ_ONLY_BASE 0x00010000u
#define FF_READ_WRITE_BASE 0x00020000u
#define FF_REGION_END 0x00030000u

void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site)
{
    ++hook_calls;
    last_address = address;
    last_bytes = (uint32_t)bytes;
    last_access = access;
    last_site = site;
    if (!access || (access & ~(uint64_t)(FF_VM_READ | FF_VM_WRITE | FF_VM_EXECUTE))) return NULL;
    if (ff_vm_check(&vm, address, bytes, access)) return NULL;
    return ram + address;
}

/* The host stops the machine on a misaligned alignment-requiring move; the test
 * unwinds to observe it, so the misaligned result is never dereferenced. */
void *ff_probe_guest_vector(uint32_t address, uint32_t access, uint32_t site)
{
    if (address & 15u) {
        ++alignment_faults;
        last_address = address;
        last_access = access;
        last_site = site;
        longjmp(alignment_fault, 1);
    }
    return ff_probe_guest_memory(address, 16u, access, site);
}

static void generated_vector_read(void)
{
    for (unsigned i = 0; i < 16; ++i) ram[FF_READ_ONLY_BASE + i] = (uint8_t)(0x40u + i);
    hook_calls = 0;
    RecompXmm value = (*(RecompXmm*)FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE), 16u, 1u, 0x001FA460u));
    assert(hook_calls == 1 && last_address == FF_READ_ONLY_BASE && last_bytes == 16 && last_access == 1);
    assert(last_site == 0x001FA460u);
    for (unsigned i = 0; i < 16; ++i) assert(value.u8[i] == (uint8_t)(0x40u + i));

    /* A generated offset form reads one vector further into the region. */
    RecompXmm second = (*(RecompXmm*)FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE + 0x10), 16u, 1u, 0x001FA460u));
    for (unsigned i = 0; i < 16; ++i) assert(second.u8[i] == 0u);

    /* Read-modify-write within the writable region: the store half is a write. */
    RecompXmm patch = (*(RecompXmm*)FF_MEMORY_SITE_POINTER((FF_READ_WRITE_BASE), 16u, 1u, 0x001FA460u));
    for (unsigned i = 0; i < 16; ++i) patch.u8[i] = (uint8_t)(0xA0u + i);
    (*(RecompXmm*)FF_MEMORY_SITE_POINTER((FF_READ_WRITE_BASE), 16u, 2u, 0x001FA460u)) = patch;
    for (unsigned i = 0; i < 16; ++i) assert(ram[FF_READ_WRITE_BASE + i] == (uint8_t)(0xA0u + i));
}

static void denied_vector_accesses_fail_closed(void)
{
    uint8_t before[16];
    memcpy(before, ram + FF_READ_ONLY_BASE, sizeof before);

    /* A 16-byte store into a read-only region is refused, and the bytes stay. */
    hook_calls = 0;
    assert(FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE), 16u, 2u, 0x001FA460u) == NULL);
    assert(hook_calls == 1 && last_address == FF_READ_ONLY_BASE && last_bytes == 16 && last_access == 2 &&
           last_site == 0x001FA460u);
    assert(memcmp(before, ram + FF_READ_ONLY_BASE, sizeof before) == 0);

    /* An alignment-requiring move (movaps/movapd/movdqa) stops the host on a
     * misaligned address without touching memory, and serves the aligned one. */
    hook_calls = 0;
    alignment_faults = 0;
    if (!setjmp(alignment_fault)) {
        (void)FF_MEMORY_SITE_VECTOR((FF_READ_ONLY_BASE + 4u), 1u, 0x001FA460u);
        assert(0 && "misaligned vector move must fault");
    }
    assert(alignment_faults == 1 && last_address == FF_READ_ONLY_BASE + 4u && last_access == 1);
    assert(last_site == 0x001FA460u && hook_calls == 0);
    assert(FF_MEMORY_SITE_VECTOR((FF_READ_ONLY_BASE), 1u, 0x001FA460u) == ram + FF_READ_ONLY_BASE);
    assert(hook_calls == 1 && last_address == FF_READ_ONLY_BASE && last_bytes == 16 && last_access == 1);
    /* An aligned move reaches the host and is denied by the region's permission. */
    assert(FF_MEMORY_SITE_VECTOR((FF_READ_ONLY_BASE), 2u, 0x001FA460u) == NULL);
    assert(alignment_faults == 1 && hook_calls == 2 && last_access == 2);
    /* An unaligned-tolerant move (movups/movupd/movdqu) keeps the plain access. */
    assert(FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE + 4u), 16u, 1u, 0x001FA460u) == ram + FF_READ_ONLY_BASE + 4u);

    /* Unaligned, spanning an unmapped page, uncommitted and outside the window. */
    assert(FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE + 1u), 16u, 1u, 0x001FA460u) == ram + FF_READ_ONLY_BASE + 1u);
    assert(FF_MEMORY_SITE_POINTER((FF_REGION_END - 8u), 16u, 1u, 0x001FA460u) == NULL);
    assert(FF_MEMORY_SITE_POINTER((FF_REGION_END), 16u, 1u, 0x001FA460u) == NULL);
    assert(FF_MEMORY_SITE_POINTER((0x00000000u), 16u, 1u, 0x001FA460u) == NULL);
    assert(FF_MEMORY_SITE_POINTER((0x00040000u), 16u, 1u, 0x001FA460u) == NULL);
    /* A move crossing a page boundary inside a mapped region still resolves. */
    assert(FF_MEMORY_SITE_POINTER((0x00020FF8u), 16u, 1u, 0x001FA460u) == ram + 0x00020FF8u);

    /* An access mask the translated surface never emits is refused. */
    assert(FF_MEMORY_SITE_POINTER((FF_READ_WRITE_BASE), 16u, 4u, 0x001FA460u) == NULL);
    /* Read-modify-write needs write permission, so it is refused where a plain
     * read succeeds. */
    assert(FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE), 16u, 3u, 0x001FA460u) == NULL);
    assert(FF_MEMORY_SITE_POINTER((FF_READ_ONLY_BASE), 16u, 1u, 0x001FA460u) == ram + FF_READ_ONLY_BASE);
}

int main(void)
{
    memset(ram, 0, sizeof ram);
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, FF_READ_ONLY_BASE, FF_REGION_END - FF_READ_ONLY_BASE) == 0);
    /* PAGE_READONLY and PAGE_READWRITE over the same fixed claim. */
    assert(ff_vm_map_fixed(&vm, FF_READ_ONLY_BASE, FF_READ_WRITE_BASE - FF_READ_ONLY_BASE, 0x02u) == 0);
    assert(ff_vm_map_fixed(&vm, FF_READ_WRITE_BASE, FF_REGION_END - FF_READ_WRITE_BASE, 0x04u) == 0);

    generated_vector_read();
    denied_vector_accesses_fail_closed();
    puts("compiled generated-vector access: real-VM permissions, alignment form, spans and site passed");
    return 0;
}
