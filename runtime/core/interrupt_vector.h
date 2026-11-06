#ifndef FF_INTERRUPT_VECTOR_H
#define FF_INTERRUPT_VECTOR_H

#include "virtual_memory.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Modeled HalGetInterruptVector initialization plumbing. It returns the
 * supplied bus interrupt level as the vector and writes its low byte to the
 * caller's IRQL output when non-null. No interrupt is delivered, no NV2A
 * interrupt object is synthesized and no controller/device state is inferred.
 * Returns 0 on success, FF_VM_ACCESS_VIOLATION for an invalid IRQL output, or
 * FF_BOOT_INVALID for null inputs.
 */
uint32_t ff_hal_get_interrupt_vector(ff_virtual_memory *vm, uint8_t *ram,
                                     uint32_t level, uint32_t irql, uint32_t *vector);

/* Modeled KeInitializeInterrupt initialization. Writes a zeroed 44-byte
 * KINTERRUPT object with routine/context/vector at 0/4/8. IRQL, mode and
 * share-vector are accepted by the caller but not modeled; no interrupt is
 * delivered. Returns 0, FF_VM_ACCESS_VIOLATION or FF_BOOT_INVALID. */
uint32_t ff_ke_initialize_interrupt(ff_virtual_memory *vm, uint8_t *ram,
                                    uint32_t object, uint32_t routine,
                                    uint32_t context, uint32_t vector);

/* Modeled KeConnectInterrupt: validates a non-null 44-byte KINTERRUPT object,
 * records no interrupt delivery and returns TRUE (1), matching the public
 * initialization plumbing. Returns 0/FF_VM_ACCESS_VIOLATION for an invalid
 * object or FF_BOOT_INVALID for missing state. */
uint32_t ff_ke_connect_interrupt(ff_virtual_memory *vm, uint8_t *ram, uint32_t object);

/* Modeled KeDisconnectInterrupt: returns the previous Connected byte at
 * object+16 and clears it. Requires a writable mapped 17-byte object.
 * Returns 0/1 (previous state), FF_VM_ACCESS_VIOLATION, or FF_BOOT_INVALID.
 * The +16 offset is modeled from retained pinned references, not observed
 * original layout. */
uint32_t ff_ke_disconnect_interrupt(ff_virtual_memory *vm, uint8_t *ram, uint32_t object);

/* Modeled HalRegisterShutdownNotification: accepts an explicit registration
 * pointer and boolean, without ever delivering a console-shutdown callback.
 * Returns 0, FF_VM_ACCESS_VIOLATION for an unreadable registration, or
 * FF_BOOT_INVALID/FF_BOOT_UNSUPPORTED for missing state or an invalid flag. */
uint32_t ff_hal_register_shutdown_notification(ff_virtual_memory *vm, uint8_t *ram,
                                               uint32_t registration, uint32_t enable);

#ifdef __cplusplus
}
#endif
#endif
