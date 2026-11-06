#include "interrupt_vector.h"
#include "thread_bootstrap.h"
#include <string.h>

uint32_t ff_hal_get_interrupt_vector(ff_virtual_memory *vm, uint8_t *ram,
                                     uint32_t level, uint32_t irql, uint32_t *vector)
{
    if (!vm || !ram || !vector) return FF_BOOT_INVALID;
    if (irql) {
        if (ff_vm_check(vm, irql, 1, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
        ram[irql] = (uint8_t)level;
    }
    *vector = level;
    return 0;
}

uint32_t ff_ke_initialize_interrupt(ff_virtual_memory *vm, uint8_t *ram,
                                    uint32_t object, uint32_t routine,
                                    uint32_t context, uint32_t vector)
{
    if (!vm || !ram || !object) return FF_BOOT_INVALID;
    if (ff_vm_check(vm, object, 44, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    memset(ram + object, 0, 44);
    for (unsigned i = 0; i < 4; ++i) {
        ram[object + i] = (uint8_t)(routine >> (8 * i));
        ram[object + 4 + i] = (uint8_t)(context >> (8 * i));
        ram[object + 8 + i] = (uint8_t)(vector >> (8 * i));
    }
    return 0;
}

uint32_t ff_ke_connect_interrupt(ff_virtual_memory *vm, uint8_t *ram, uint32_t object)
{
    if (!vm || !ram) return FF_BOOT_INVALID;
    if (object && ff_vm_check(vm, object, 44, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    /* The pinned connect marks the interrupt connected; a read-only object must
     * be refused rather than silently "connected". */
    if (object && ff_vm_check(vm, object + 16u, 1, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    if (object) ram[object + 16u] = 1u;
    return 1;
}

uint32_t ff_ke_disconnect_interrupt(ff_virtual_memory *vm, uint8_t *ram, uint32_t object)
{
    if (!vm || !ram) return FF_BOOT_INVALID;
    if (!object || (object & 3u)) return FF_BOOT_INVALID;
    if (ff_vm_check(vm, object, 17u, FF_VM_READ | FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    uint32_t previous = ram[object + 16u] ? 1u : 0u;
    ram[object + 16u] = 0u;
    return previous;
}

uint32_t ff_hal_register_shutdown_notification(ff_virtual_memory *vm, uint8_t *ram,
                                               uint32_t registration, uint32_t enable)
{
    if (!vm || !ram) return FF_BOOT_INVALID;
    if (enable > 1) return FF_BOOT_UNSUPPORTED;
    if (registration && ff_vm_check(vm, registration, 4, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    return 0;
}
