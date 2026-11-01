#include "virtual_memory.h"
#include "thread_bootstrap.h"
#include <string.h>

static uint32_t get(const ff_virtual_memory *vm, uint32_t p)
{
    uint32_t n = 0;
    for (unsigned i = 0; i < 4; ++i) n |= (uint32_t)vm->ram[p + i] << (8u * i);
    return n;
}
static void put(ff_virtual_memory *vm, uint32_t p, uint32_t n)
{
    for (unsigned i = 0; i < 4; ++i) vm->ram[p + i] = (uint8_t)(n >> (8u * i));
}
static uint32_t permissions(uint32_t protection)
{
    switch (protection) {
        case 1: return 0;
        case 2: return FF_VM_READ;
        case 4: return FF_VM_READ | FF_VM_WRITE;
        case 0x10: return FF_VM_EXECUTE;
        case 0x20: return FF_VM_READ | FF_VM_EXECUTE;
        case 0x40: return FF_VM_READ | FF_VM_WRITE | FF_VM_EXECUTE;
        default: return UINT32_MAX;
    }
}
static uint32_t protection_for(uint32_t access)
{
    if (access & FF_VM_EXECUTE) return access & FF_VM_WRITE ? 0x40u : access & FF_VM_READ ? 0x20u : 0x10u;
    return access & FF_VM_WRITE ? 4u : access & FF_VM_READ ? 2u : 1u;
}
static uint32_t protection_error(uint32_t protection)
{
    /* Known copy/guard/cache variants need their own semantics. Do not turn a
     * valid but unimplemented request into a fabricated guest error path. */
    uint32_t base = protection & 0xffu;
    int known = base == 1 || base == 2 || base == 4 || base == 8 || base == 0x10 || base == 0x20 || base == 0x40 || base == 0x80;
    return known && !(protection & ~0x7ffu) ? FF_BOOT_UNSUPPORTED : FF_VM_BAD_PROTECTION;
}
static int span(const ff_virtual_memory *vm, uint32_t base, uint32_t bytes, uint32_t *low, uint32_t *high)
{
    uint64_t end = (uint64_t)base + bytes;
    if (!vm || !vm->ram || !bytes || base < FF_VM_GRANULARITY || end > vm->bytes) return 0;
    *low = base & ~(FF_VM_PAGE - 1u);
    *high = (uint32_t)((end + FF_VM_PAGE - 1u) & ~(uint64_t)(FF_VM_PAGE - 1u));
    return *high <= vm->bytes;
}
static unsigned empty_region(const ff_virtual_memory *vm)
{
    for (unsigned i = 0; i < FF_VM_REGIONS; ++i) if (!vm->regions[i].kind) return i;
    return FF_VM_REGIONS;
}
static int free_span(const ff_virtual_memory *vm, uint32_t low, uint32_t high)
{
    for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) if (vm->owner[p]) return 0;
    return 1;
}
static unsigned containing(const ff_virtual_memory *vm, uint32_t low, uint32_t high)
{
    unsigned owner = vm->owner[low / FF_VM_PAGE];
    if (!owner) return FF_VM_REGIONS;
    const ff_vm_region *r = &vm->regions[owner - 1];
    return r->base <= low && high <= r->base + r->bytes ? owner - 1 : FF_VM_REGIONS;
}
static int outputs(const ff_virtual_memory *vm, uint32_t a, uint32_t b)
{
    return !(a & 3u) && !(b & 3u) && a != b &&
        ff_vm_check(vm, a, 4, FF_VM_READ | FF_VM_WRITE) == 0 && ff_vm_check(vm, b, 4, FF_VM_READ | FF_VM_WRITE) == 0;
}
static int overlaps(uint32_t p, uint32_t low, uint32_t high) { return p < high && (uint64_t)p + 4 > low; }

uint32_t ff_vm_init(ff_virtual_memory *vm, uint8_t *ram, uint32_t bytes)
{
    if (!vm || !ram || bytes < 2u * FF_VM_GRANULARITY || bytes > FF_VM_MAX_PAGES * FF_VM_PAGE || (bytes & (FF_VM_PAGE - 1u))) return FF_BOOT_INVALID;
    memset(vm, 0, sizeof *vm); vm->ram = ram; vm->bytes = bytes;
    return 0;
}
uint32_t ff_vm_claim_fixed(ff_virtual_memory *vm, uint32_t base, uint32_t bytes)
{
    uint32_t low, high;
    if (!span(vm, base, bytes, &low, &high)) return FF_BOOT_INVALID;
    if (!free_span(vm, low, high)) return FF_VM_CONFLICT;
    unsigned index = empty_region(vm);
    if (index == FF_VM_REGIONS) return FF_BOOT_NO_MEMORY;
    vm->regions[index] = (ff_vm_region){low, high - low, 0, FF_VM_FIXED};
    for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) vm->owner[p] = (uint16_t)(index + 1);
    return 0;
}
uint32_t ff_vm_map_fixed(ff_virtual_memory *vm, uint32_t base, uint32_t bytes, uint32_t protection)
{
    uint32_t low, high, access = permissions(protection);
    if (!span(vm, base, bytes, &low, &high) || access == UINT32_MAX) return FF_BOOT_INVALID;
    unsigned index = containing(vm, low, high);
    if (index == FF_VM_REGIONS || vm->regions[index].kind != FF_VM_FIXED) return FF_VM_CONFLICT;
    /* Sections sharing a hardware page contribute their required permissions. */
    for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) {
        vm->committed[p] = 1; vm->permission[p] |= (uint8_t)access;
    }
    return 0;
}
uint32_t ff_vm_check(const ff_virtual_memory *vm, uint32_t address, uint64_t bytes, uint32_t access)
{
    if (!vm || !vm->ram || !access || (access & ~7u)) return FF_VM_ACCESS_VIOLATION;
    if (!bytes) return 0;
    if (address > vm->bytes || bytes > vm->bytes - address) return FF_VM_ACCESS_VIOLATION;
    uint32_t end = address + (uint32_t)bytes;
    for (uint32_t p = address / FF_VM_PAGE; p <= (end - 1u) / FF_VM_PAGE; ++p) {
        if (!vm->owner[p] || !vm->committed[p] || (vm->permission[p] & access) != access) return FF_VM_ACCESS_VIOLATION;
    }
    return 0;
}
uint32_t ff_vm_allocate(ff_virtual_memory *vm, uint32_t bp, uint32_t zero_bits, uint32_t sp, uint32_t type, uint32_t protection)
{
    if (!outputs(vm, bp, sp)) return FF_VM_ACCESS_VIOLATION;
    uint32_t base = get(vm, bp), bytes = get(vm, sp), access = permissions(protection);
    if (!bytes || zero_bits > 21 || (type & ~(0x1000u | 0x2000u | 0x80000u | 0x100000u | 0x800000u)) ||
        !(type & (0x1000u | 0x2000u | 0x80000u)) || ((type & 0x80000u) && type != 0x80000u) ||
        (uint64_t)base + bytes > 0x80000000ull) return FF_BOOT_INVALID;
    if (access == UINT32_MAX) return protection_error(protection);
    if (zero_bits || (type & (0x100000u | 0x800000u))) return FF_BOOT_UNSUPPORTED;
    if (!base && type == 0x80000u) return FF_BOOT_UNSUPPORTED;
    uint32_t low, high;
    unsigned index;
    int reserve = (type & 0x2000u) || !base;
    if (reserve) {
        if (base & (FF_VM_GRANULARITY - 1u)) return FF_BOOT_UNSUPPORTED;
        uint64_t rounded = ((uint64_t)bytes + FF_VM_PAGE - 1u) & ~(uint64_t)(FF_VM_PAGE - 1u);
        if (rounded > vm->bytes - FF_VM_GRANULARITY) return FF_BOOT_UNSUPPORTED;
        if (!base) {
            for (base = FF_VM_GRANULARITY; base <= vm->bytes - rounded; base += FF_VM_GRANULARITY) {
                if (free_span(vm, base, base + (uint32_t)rounded)) break;
            }
            if (base > vm->bytes - rounded) return FF_BOOT_NO_MEMORY;
        }
        if (!span(vm, base, (uint32_t)rounded, &low, &high)) return FF_BOOT_UNSUPPORTED;
        if (!free_span(vm, low, high)) return FF_VM_CONFLICT;
        index = empty_region(vm);
        if (index == FF_VM_REGIONS) return FF_BOOT_NO_MEMORY;
        vm->regions[index] = (ff_vm_region){low, high - low, protection, FF_VM_PRIVATE};
        for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) vm->owner[p] = (uint16_t)(index + 1);
    } else {
        if (!span(vm, base, bytes, &low, &high)) return FF_BOOT_UNSUPPORTED;
        index = containing(vm, low, high);
        if (index == FF_VM_REGIONS || vm->regions[index].kind != FF_VM_PRIVATE) return FF_VM_CONFLICT;
    }
    if (type & 0x1000u) for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) {
        if (!vm->committed[p]) memset(vm->ram + p * FF_VM_PAGE, 0, FF_VM_PAGE);
        vm->committed[p] = 1; vm->permission[p] = (uint8_t)access;
    }
    put(vm, bp, low); put(vm, sp, high - low);
    return 0;
}
uint32_t ff_vm_free(ff_virtual_memory *vm, uint32_t bp, uint32_t sp, uint32_t type)
{
    if (!outputs(vm, bp, sp)) return FF_VM_ACCESS_VIOLATION;
    if (type != 0x4000u && type != 0x8000u) return FF_BOOT_INVALID;
    uint32_t base = get(vm, bp), bytes = get(vm, sp), low, high;
    if (!span(vm, base, bytes ? bytes : 1u, &low, &high)) return FF_BOOT_UNSUPPORTED;
    unsigned owner = vm->owner[low / FF_VM_PAGE];
    if (!owner) return FF_VM_NOT_ALLOCATED;
    unsigned index = owner - 1;
    ff_vm_region region = vm->regions[index];
    if (region.kind != FF_VM_PRIVATE) return FF_VM_CONFLICT;
    if (!bytes) {
        if (low != region.base) return FF_VM_NOT_AT_BASE;
        high = region.base + region.bytes;
    }
    if (high > region.base + region.bytes) return FF_VM_UNABLE_TO_FREE;
    if (overlaps(bp, low, high) || overlaps(sp, low, high)) return FF_BOOT_UNSUPPORTED;
    unsigned right = FF_VM_REGIONS;
    if (type == 0x8000u && low > region.base && high < region.base + region.bytes) {
        right = empty_region(vm);
        if (right == FF_VM_REGIONS) return FF_BOOT_NO_MEMORY;
    }
    for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) {
        vm->committed[p] = vm->permission[p] = 0;
        if (type == 0x8000u) vm->owner[p] = 0;
    }
    if (type == 0x8000u) {
        uint32_t end = region.base + region.bytes;
        if (low == region.base && high == end) memset(&vm->regions[index], 0, sizeof region);
        else if (low == region.base) { vm->regions[index].base = high; vm->regions[index].bytes = end - high; }
        else {
            vm->regions[index].bytes = low - region.base;
            if (right != FF_VM_REGIONS) {
                vm->regions[right] = (ff_vm_region){high, end - high, region.protection, FF_VM_PRIVATE};
                for (uint32_t p = high / FF_VM_PAGE; p < end / FF_VM_PAGE; ++p) vm->owner[p] = (uint16_t)(right + 1);
            }
        }
    }
    put(vm, bp, low); put(vm, sp, high - low);
    return 0;
}
uint32_t ff_vm_protect(ff_virtual_memory *vm, uint32_t bp, uint32_t sp, uint32_t protection, uint32_t oldp)
{
    if (!outputs(vm, bp, sp) || (oldp & 3u) || ff_vm_check(vm, oldp, 4, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    if (oldp == bp || oldp == sp) return FF_BOOT_UNSUPPORTED;
    uint32_t low, high, base = get(vm, bp), bytes = get(vm, sp), access = permissions(protection);
    if (access == UINT32_MAX) return protection_error(protection);
    if (!span(vm, base, bytes, &low, &high)) return bytes ? FF_BOOT_UNSUPPORTED : FF_BOOT_INVALID;
    unsigned index = containing(vm, low, high);
    if (index == FF_VM_REGIONS || vm->regions[index].kind != FF_VM_PRIVATE) return FF_VM_CONFLICT;
    if (overlaps(bp, low, high) || overlaps(sp, low, high) || overlaps(oldp, low, high)) return FF_BOOT_UNSUPPORTED;
    for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) if (!vm->committed[p]) return FF_VM_NOT_COMMITTED;
    uint32_t old = protection_for(vm->permission[low / FF_VM_PAGE]);
    for (uint32_t p = low / FF_VM_PAGE; p < high / FF_VM_PAGE; ++p) vm->permission[p] = (uint8_t)access;
    put(vm, bp, low); put(vm, sp, high - low); put(vm, oldp, old);
    return 0;
}
