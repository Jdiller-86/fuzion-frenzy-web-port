/* Owned MmClaimGpuInstanceMemory scenarios; no original code or device model. */
#include "../src/runtime/gpu_instance.h"
#include "../src/runtime/thread_bootstrap.h"
#include "../src/runtime/virtual_memory.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RAM_BYTES (4u * 1024u * 1024u)
static uint8_t ram[RAM_BYTES];
static ff_virtual_memory vm;

static uint32_t word(uint32_t address)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= (uint32_t)vm.ram[address + i] << (8u * i);
    return value;
}

static void put_word(uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) vm.ram[address + i] = (uint8_t)(value >> (8u * i));
}

static unsigned regions(void)
{
    unsigned count = 0;
    for (unsigned i = 0; i < FF_VM_REGIONS; ++i) if (vm.regions[i].kind) ++count;
    return count;
}

static void reset(void)
{
    assert(ff_vm_init(&vm, ram, RAM_BYTES) == 0);
    assert(ff_vm_claim_fixed(&vm, 0x10000u, 0x2000u) == 0);
    assert(ff_vm_map_fixed(&vm, 0x10000u, 0x1000u, 4) == 0);
    assert(ff_vm_map_fixed(&vm, 0x11000u, 0x1000u, 2) == 0);
    put_word(0x10000u, 0x12345678u);
}

static void claim_and_default(void)
{
    reset();
    uint32_t base = 0;
    assert(ff_claim_gpu_instance_memory(&vm, 0x5000u, 0x10000u, 0, RAM_BYTES, &base) == 0);
    assert(base && !(base & (FF_VM_PAGE - 1u)));
    assert(word(0x10000u) == 0);
    assert(ff_vm_check(&vm, base, 0x5000u, FF_VM_READ | FF_VM_WRITE) == 0);
    uint32_t second = 0;
    assert(ff_claim_gpu_instance_memory(&vm, 0xFFFFFFFFu, 0, 0, RAM_BYTES, &second) == 0);
    assert(second && !(second & (FF_VM_PAGE - 1u)) && second != base);
    assert(ff_vm_check(&vm, second, FF_GPU_INSTANCE_DEFAULT, FF_VM_READ | FF_VM_WRITE) == 0);
}

static void invalid_requests_do_not_commit(void)
{
    reset();
    unsigned before = regions();
    uint32_t base = 0xdeadbeefu;
    assert(ff_claim_gpu_instance_memory(&vm, 0, 0x10000u, 0, RAM_BYTES, &base) == FF_BOOT_INVALID);
    assert(base == 0xdeadbeefu && word(0x10000u) == 0x12345678u && regions() == before);
    assert(ff_claim_gpu_instance_memory(&vm, 0x5000u, 0x10001u, 0, RAM_BYTES, &base) == FF_VM_ACCESS_VIOLATION);
    assert(base == 0xdeadbeefu && word(0x10000u) == 0x12345678u && regions() == before);
    assert(ff_claim_gpu_instance_memory(&vm, 0x5000u, 0x11000u, 0, RAM_BYTES, &base) == FF_VM_ACCESS_VIOLATION);
    assert(base == 0xdeadbeefu && regions() == before);
    assert(ff_claim_gpu_instance_memory(&vm, 0x5000u, 0xFFFFFFFFu, 0, RAM_BYTES, &base) == FF_VM_ACCESS_VIOLATION);
    assert(base == 0xdeadbeefu && regions() == before);
    assert(ff_claim_gpu_instance_memory(&vm, RAM_BYTES + 1u, 0, 0, RAM_BYTES, &base) != 0);
    assert(base == 0xdeadbeefu && regions() == before);
}

static void capacity_failure_is_atomic(void)
{
    ff_virtual_memory small;
    assert(ff_vm_init(&small, ram, 0x20000u) == 0);
    assert(ff_vm_claim_fixed(&small, 0x10000u, 0x10000u) == 0);
    uint32_t base = 0xdeadbeefu;
    assert(ff_claim_gpu_instance_memory(&small, 0x5000u, 0, 0, 0x20000u, &base) == FF_BOOT_NO_MEMORY);
    assert(base == 0xdeadbeefu);
}

int main(void)
{
    claim_and_default();
    invalid_requests_do_not_commit();
    capacity_failure_is_atomic();
    puts("owned MmClaimGpuInstanceMemory: bounded claim, default size, padding, rejection and atomicity passed");
    return 0;
}
