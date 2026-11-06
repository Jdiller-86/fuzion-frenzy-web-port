#include "gpu_instance.h"
#include "thread_bootstrap.h"

uint32_t ff_claim_gpu_instance_memory(ff_virtual_memory *vm, uint32_t bytes, uint32_t padding,
                                      uint32_t lowest, uint32_t highest, uint32_t *base)
{
    if (!vm || !vm->ram || !base) return FF_BOOT_INVALID;
    if (bytes == 0xFFFFFFFFu) bytes = FF_GPU_INSTANCE_DEFAULT;
    if (!bytes) return FF_BOOT_INVALID;
    /* Validate the optional output before committing a region so a bad pointer
     * cannot leave a claim behind. */
    if (padding && ((padding & 3u) || ff_vm_check(vm, padding, 4u, FF_VM_WRITE)))
        return FF_VM_ACCESS_VIOLATION;
    uint32_t chosen = 0;
    uint32_t status = ff_vm_allocate_contiguous(vm, bytes, lowest, highest, 0, 4u, &chosen);
    if (status) return status;
    if (padding)
        for (unsigned i = 0; i < 4; ++i) vm->ram[padding + i] = 0;
    *base = chosen;
    return 0;
}
