/* Owned interrupt-vector service; no original code or hardware model. */
#include "../runtime/core/interrupt_vector.h"
#include "../runtime/core/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[0x40000];
static ff_virtual_memory vm;

static void setup(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(!ff_vm_init(&vm, ram, sizeof ram));
    assert(!ff_vm_claim_fixed(&vm, 0x10000, 0x20000));
    assert(!ff_vm_map_fixed(&vm, 0x10000, 0x1000, 4));
    assert(!ff_vm_map_fixed(&vm, 0x12000, 0x1000, 4));  /* read/write */
    assert(!ff_vm_map_fixed(&vm, 0x13000, 0x1000, 2));  /* read-only */
}

int main(void)
{
    uint32_t vector = 0;
    setup();
    assert(!ff_hal_get_interrupt_vector(&vm, ram, 3, 0x10004, &vector));
    assert(vector == 3 && ram[0x10004] == 3);
    assert(!ff_hal_get_interrupt_vector(&vm, ram, 0x1234, 0, &vector));
    assert(vector == 0x1234 && ram[0x10004] == 3);
    assert(ff_hal_get_interrupt_vector(&vm, ram, 3, 0x20000, &vector) == FF_VM_ACCESS_VIOLATION && ram[0x20000] == 0xa5);
    assert(ff_hal_get_interrupt_vector(NULL, ram, 3, 0x10004, &vector) == FF_BOOT_INVALID);
    assert(ff_hal_get_interrupt_vector(&vm, NULL, 3, 0x10004, &vector) == FF_BOOT_INVALID);
    assert(ff_hal_get_interrupt_vector(&vm, ram, 3, 0x10004, NULL) == FF_BOOT_INVALID);
    memset(ram + 0x10100, 0xa5, 44);
    assert(!ff_ke_initialize_interrupt(&vm, ram, 0x10100, 0x11223344, 0x55667788, 3));
    assert(ram[0x10100] == 0x44 && ram[0x10101] == 0x33 && ram[0x10102] == 0x22 && ram[0x10103] == 0x11);
    assert(ram[0x10104] == 0x88 && ram[0x10105] == 0x77 && ram[0x10106] == 0x66 && ram[0x10107] == 0x55);
    assert(ram[0x10108] == 3 && ram[0x10109] == 0 && ram[0x1012b] == 0);
    assert(ff_ke_initialize_interrupt(&vm, ram, 0x20000, 1, 2, 3) == FF_VM_ACCESS_VIOLATION);
    assert(ff_ke_initialize_interrupt(NULL, ram, 0x10100, 1, 2, 3) == FF_BOOT_INVALID);
    assert(ff_ke_initialize_interrupt(&vm, NULL, 0x10100, 1, 2, 3) == FF_BOOT_INVALID);
    assert(ff_ke_initialize_interrupt(&vm, ram, 0, 1, 2, 3) == FF_BOOT_INVALID);
    assert(ff_ke_connect_interrupt(&vm, ram, 0x10100) == 1);
    assert(ff_ke_connect_interrupt(&vm, ram, 0) == 1);
    assert(ff_ke_connect_interrupt(&vm, ram, 0x20000) == FF_VM_ACCESS_VIOLATION);
    assert(ff_ke_connect_interrupt(NULL, ram, 0x10100) == FF_BOOT_INVALID);
    assert(ff_ke_connect_interrupt(&vm, NULL, 0x10100) == FF_BOOT_INVALID);
    assert(ff_hal_register_shutdown_notification(&vm, ram, 0x10100, 1) == 0);
    assert(ff_hal_register_shutdown_notification(&vm, ram, 0, 0) == 0);
    assert(ff_hal_register_shutdown_notification(&vm, ram, 0x20000, 1) == FF_VM_ACCESS_VIOLATION);
    assert(ff_hal_register_shutdown_notification(&vm, ram, 0x10100, 2) == FF_BOOT_UNSUPPORTED);
    assert(ff_hal_register_shutdown_notification(NULL, ram, 0x10100, 1) == FF_BOOT_INVALID);
    assert(ff_hal_register_shutdown_notification(&vm, NULL, 0x10100, 1) == FF_BOOT_INVALID);
    /* Ordinal 100 model: previous Connected byte, then clear. */
    memset(ram + 0x12100, 0, 44);
    assert(ff_ke_connect_interrupt(&vm, ram, 0x12100) == 1);
    assert(ram[0x12110] == 1);
    assert(ff_ke_disconnect_interrupt(&vm, ram, 0x12100) == 1);
    assert(ram[0x12110] == 0);
    assert(ff_ke_disconnect_interrupt(&vm, ram, 0x12100) == 0);
    assert(ff_ke_disconnect_interrupt(&vm, ram, 0) == FF_BOOT_INVALID);
    assert(ff_ke_disconnect_interrupt(&vm, ram, 0x12102) == FF_BOOT_INVALID);
    assert(ff_ke_disconnect_interrupt(&vm, ram, 0x20000) == FF_VM_ACCESS_VIOLATION);
    assert(ff_ke_disconnect_interrupt(NULL, ram, 0x12100) == FF_BOOT_INVALID);
    assert(ff_ke_disconnect_interrupt(&vm, NULL, 0x12100) == FF_BOOT_INVALID);
    /* A read-only object must not be silently connected or disconnected. */
    assert(ff_ke_connect_interrupt(&vm, ram, 0x13100) == FF_VM_ACCESS_VIOLATION);
    assert(ff_ke_disconnect_interrupt(&vm, ram, 0x13100) == FF_VM_ACCESS_VIOLATION);
    /* The connect write is confined to object+16. */
    memset(ram + 0x12200, 0x5a, 44);
    assert(ff_ke_connect_interrupt(&vm, ram, 0x12200) == 1);
    assert(ram[0x12210] == 1);
    for (unsigned i = 0; i < 44; ++i) {
        if (i != 16) assert(ram[0x12200 + i] == 0x5a);
    }
    puts("Owned interrupt initialization/connect/shutdown-registration output and failure checks passed");
    return 0;
}
