#ifndef FF_VIRTUAL_MEMORY_H
#define FF_VIRTUAL_MEMORY_H
#include <stdint.h>

/* Bounded identity-mapped guest VA window. This is not the full Xbox virtual
 * address space or physical-page manager. Unsupported variants stay explicit. */
#define FF_VM_PAGE 4096u
#define FF_VM_GRANULARITY 65536u
#define FF_VM_MAX_PAGES 16384u
#define FF_VM_REGIONS 128u
#define FF_VM_READ 1u
#define FF_VM_WRITE 2u
#define FF_VM_EXECUTE 4u
#define FF_VM_ACCESS_VIOLATION 0xc0000005u
#define FF_VM_CONFLICT 0xc0000018u
#define FF_VM_NOT_COMMITTED 0xc000002du
#define FF_VM_BAD_PROTECTION 0xc0000045u
#define FF_VM_NOT_ALLOCATED 0xc00000a0u
#define FF_VM_NOT_AT_BASE 0xc000009fu
#define FF_VM_UNABLE_TO_FREE 0xc000001au

enum { FF_VM_FIXED = 1, FF_VM_PRIVATE = 2 };
typedef struct { uint32_t base, bytes, protection, kind; } ff_vm_region;
typedef struct {
    uint8_t *ram;
    uint32_t bytes;
    ff_vm_region regions[FF_VM_REGIONS];
    uint16_t owner[FF_VM_MAX_PAGES];
    uint8_t permission[FF_VM_MAX_PAGES], committed[FF_VM_MAX_PAGES];
} ff_virtual_memory;

uint32_t ff_vm_init(ff_virtual_memory *, uint8_t *ram, uint32_t bytes);
uint32_t ff_vm_claim_fixed(ff_virtual_memory *, uint32_t base, uint32_t bytes);
uint32_t ff_vm_map_fixed(ff_virtual_memory *, uint32_t base, uint32_t bytes, uint32_t protection);
uint32_t ff_vm_check(const ff_virtual_memory *, uint32_t address, uint64_t bytes, uint32_t access);
uint32_t ff_vm_allocate(ff_virtual_memory *, uint32_t base_pointer, uint32_t zero_bits,
                        uint32_t size_pointer, uint32_t type, uint32_t protection);
uint32_t ff_vm_free(ff_virtual_memory *, uint32_t base_pointer, uint32_t size_pointer, uint32_t type);
uint32_t ff_vm_protect(ff_virtual_memory *, uint32_t base_pointer, uint32_t size_pointer,
                       uint32_t protection, uint32_t old_protection_pointer);
#endif
