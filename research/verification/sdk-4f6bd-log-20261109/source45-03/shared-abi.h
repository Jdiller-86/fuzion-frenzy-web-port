#ifndef FF_SDK_SHARED_ABI_H
#define FF_SDK_SHARED_ABI_H
#include <stdint.h>
#include "virtual_memory.h"
#include "thread_bootstrap.h"
typedef struct {
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi, fs_base, df;
} ff_sdk_scalar;
#ifdef __cplusplus
extern "C" {
#endif
uint8_t *ff_sdk_shared_ram(void);
ff_virtual_memory *ff_sdk_shared_vm(void);
ff_boot_runtime *ff_sdk_shared_boot(void);
void ff_sdk_shared_read(ff_sdk_scalar *);
void ff_sdk_shared_write(const ff_sdk_scalar *);
void ff_sdk_shared_kernel(uint32_t ordinal, ff_sdk_scalar *);
uint8_t *ff_sdk_shared_memory(uint32_t address, uint32_t bytes, uint32_t access, ff_sdk_scalar *);
void ff_sdk_shared_enter(uint32_t guest, ff_sdk_scalar *);
void ff_sdk_shared_exit(uint32_t guest, ff_sdk_scalar *);
void ff_sdk_shared_stop(uint32_t target);
uint32_t ff_sdk_shared_invoke(void);
uint32_t ff_sdk_shared_invoke_target(uint32_t target);
void ff_sdk_shared_memory_fault(uint32_t address,uint32_t bytes,uint32_t access,const ff_sdk_scalar *s);
void ff_sdk_shared_stos_event(uint32_t complete);
void ff_sdk_shared_device_stos_event(uint32_t complete);
void ff_sdk_shared_xinit(ff_sdk_scalar *);
void ff_sdk_shared_log_bridge(ff_sdk_scalar *, uint32_t, uint32_t, uint32_t, uint32_t);
void ff_sdk_shared_log_bridge_second(ff_sdk_scalar *, uint32_t, uint32_t, uint32_t, uint32_t);
void ff_sdk_shared_log_bridge_third(ff_sdk_scalar *, uint32_t, uint32_t, uint32_t);
uint32_t ff_probe_sdk_host_input(uint32_t source,uint32_t ports,uint32_t mask);
uint32_t ff_probe_sdk_chain_profile(uint32_t enable);
uint32_t ff_probe_sdk_chain_word(uint32_t index);
#ifdef __cplusplus
}
#endif
#endif
