#include "unnamed_mutants.h"
#include "thread_bootstrap.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static int range_end(uintptr_t start, size_t size, uintptr_t *end)
{
    if (size > UINTPTR_MAX - start) return 0;
    *end = start + size;
    return 1;
}

static int overlaps(uintptr_t a, uintptr_t ae, uintptr_t b, uintptr_t be)
{
    return a < be && b < ae;
}

static int valid_model(const ff_unnamed_mutants *model)
{
    if (!model || !model->memory || !model->memory->ram ||
        model->memory->bytes < 2u * FF_VM_GRANULARITY ||
        model->memory->bytes > FF_VM_MAX_PAGES * FF_VM_PAGE ||
        (model->memory->bytes & (FF_VM_PAGE - 1u)) || model->count > FF_MUTANT_CAPACITY)
        return 0;
    for (uint32_t i = 0; i < model->count; ++i)
        if (model->handles[i] != FF_MUTANT_HANDLE_BASE + 4u * (i + 1u)) return 0;
    return 1;
}

uint32_t ff_unnamed_mutants_init(ff_unnamed_mutants *model, ff_virtual_memory *memory)
{
    uintptr_t model_start, model_end, vm_start, vm_end, ram_start, ram_end;
    if (!model || !memory || !memory->ram ||
        memory->bytes < 2u * FF_VM_GRANULARITY ||
        memory->bytes > FF_VM_MAX_PAGES * FF_VM_PAGE ||
        (memory->bytes & (FF_VM_PAGE - 1u))) return FF_BOOT_INVALID;
    if (!range_end((uintptr_t)model, sizeof *model, &model_end) ||
        !range_end((uintptr_t)memory, sizeof *memory, &vm_end) ||
        !range_end((uintptr_t)memory->ram, memory->bytes, &ram_end)) return FF_BOOT_INVALID;
    model_start = (uintptr_t)model; vm_start = (uintptr_t)memory; ram_start = (uintptr_t)memory->ram;
    if (overlaps(model_start, model_end, vm_start, vm_end) ||
        overlaps(model_start, model_end, ram_start, ram_end)) return FF_BOOT_INVALID;
    memset(model, 0, sizeof *model);
    model->memory = memory;
    return 0;
}

uint32_t ff_unnamed_mutants_create(ff_unnamed_mutants *model, uint32_t output,
                                   uint32_t object_attributes, uint32_t initial_owner)
{
    if (!valid_model(model)) return FF_BOOT_INVALID;
    if (!output || (output & 3u)) return FF_BOOT_INVALID;
    if (object_attributes || initial_owner) return FF_BOOT_UNSUPPORTED;
    uint32_t status = ff_vm_check(model->memory, output, 4, FF_VM_WRITE);
    if (status) return status;
    if (model->count == FF_MUTANT_CAPACITY) return FF_MUTANT_NO_SLOTS;

    uint32_t handle = FF_MUTANT_HANDLE_BASE + 4u * (model->count + 1u);
    uint8_t *p = model->memory->ram + output;
    p[0] = (uint8_t)handle;
    p[1] = (uint8_t)(handle >> 8);
    p[2] = (uint8_t)(handle >> 16);
    p[3] = (uint8_t)(handle >> 24);
    model->handles[model->count] = handle;
    ++model->count;
    return 0;
}
