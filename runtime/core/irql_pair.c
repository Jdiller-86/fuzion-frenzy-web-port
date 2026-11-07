#include "irql_pair.h"
#include <string.h>

#define FF_IRQL_PCR_LEVEL 0x24u
#define FF_IRQL_PCR_BYTES (FF_IRQL_PCR_LEVEL + 1u)
#define FF_IRQL_DPC_LEVEL 2u

void ff_irql_pair_init(ff_irql_pair *pair)
{
    if (pair) memset(pair, 0, sizeof(*pair));
}

static uint32_t current_pcr(const ff_boot_runtime *boot, const ff_virtual_memory *vm,
                            uint32_t *thread_id, uint32_t *fs, uint32_t *level)
{
    if (!boot || !vm || !boot->ram || !vm->ram || boot->ram != vm->ram ||
        !boot->bytes || boot->bytes > vm->bytes || boot->current >= boot->count ||
        boot->count > FF_BOOT_MAX_THREADS) return FF_BOOT_INVALID;
    const ff_boot_thread *thread = &boot->threads[boot->current];
    if (thread->id != boot->current + 1u || thread->state != FF_BOOT_RUNNING || !thread->fs ||
        thread->fs > boot->bytes || FF_IRQL_PCR_BYTES > boot->bytes - thread->fs ||
        !thread->object || thread->object >= boot->bytes || !thread->stack_low ||
        thread->stack_low >= thread->stack_top || thread->stack_top > boot->bytes ||
        thread->fs < thread->object || thread->fs - thread->object != 0x200u ||
        thread->stack_low < thread->fs + FF_IRQL_PCR_BYTES) return FF_BOOT_INVALID;
    uint32_t status = ff_vm_check(vm, thread->fs + FF_IRQL_PCR_LEVEL, 1,
                                  FF_VM_READ | FF_VM_WRITE);
    if (status) return status;
    *thread_id = thread->id;
    *fs = thread->fs;
    *level = boot->ram[thread->fs + FF_IRQL_PCR_LEVEL];
    return FF_BOOT_OK;
}

uint32_t ff_irql_pair_raise_to_dpc(ff_irql_pair *pair, const ff_boot_runtime *boot,
                                   const ff_virtual_memory *vm, uint8_t *old_level)
{
    if (!pair || !old_level) return FF_BOOT_INVALID;
    uint32_t thread_id, fs, level;
    uint32_t status = current_pcr(boot, vm, &thread_id, &fs, &level);
    if (status) return status;
    if (pair->active) return FF_BOOT_UNSUPPORTED;
    if (pair->raise_count == UINT32_MAX) return FF_BOOT_UNSUPPORTED;
    if (level != 0) return FF_BOOT_UNSUPPORTED;

    boot->ram[fs + FF_IRQL_PCR_LEVEL] = FF_IRQL_DPC_LEVEL;
    pair->active = 1;
    pair->thread_id = thread_id;
    pair->fs = fs;
    pair->saved_level = level;
    ++pair->raise_count;
    *old_level = (uint8_t)level;
    return FF_BOOT_OK;
}

uint32_t ff_irql_pair_lower_saved(ff_irql_pair *pair, const ff_boot_runtime *boot,
                                  const ff_virtual_memory *vm, uint32_t value)
{
    if (!pair || !pair->active) return FF_BOOT_INVALID;
    uint32_t thread_id, fs, level;
    uint32_t status = current_pcr(boot, vm, &thread_id, &fs, &level);
    if (status) return status;
    if (pair->lower_count == UINT32_MAX) return FF_BOOT_UNSUPPORTED;
    if (value != pair->saved_level || pair->saved_level != 0 || level != FF_IRQL_DPC_LEVEL ||
        thread_id != pair->thread_id || fs != pair->fs) return FF_BOOT_INVALID;

    boot->ram[fs + FF_IRQL_PCR_LEVEL] = (uint8_t)pair->saved_level;
    pair->active = 0;
    pair->thread_id = 0;
    pair->fs = 0;
    pair->saved_level = 0;
    ++pair->lower_count;
    return FF_BOOT_OK;
}
