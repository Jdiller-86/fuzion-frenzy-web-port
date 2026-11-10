#pragma once
#include <cstdint>
#include <cstring>
#include <type_traits>
extern "C" {
#include "virtual_memory.h"
#include "thread_bootstrap.h"
#include "local_mutants.h"
}
#include "x86_context.h"
struct FFSDKRuntime;
struct FFSDKThreadState { FFSDKRuntime *runtime; ff_boot_thread *boot_thread; X86Context *context; uint32_t stopped; };
struct FFSDKMapping { uint32_t guest; X86Func *host; };
struct FFSDKRuntime {
    ff_virtual_memory *memory; ff_boot_runtime *boot; const FFSDKMapping *mapping; uint32_t mapping_count;
    ff_local_mutants *local_mutants; uint32_t kernel_wait234_enabled;
    uint8_t *base; X86Context contexts[FF_BOOT_MAX_THREADS]; FFSDKThreadState states[FF_BOOT_MAX_THREADS]; FFSDKThreadState *current;
};
extern "C" uint8_t *ff_sdk_checked_span(X86Context &, uint8_t *, uint32_t, uint32_t, uint32_t);
extern "C" void ff_sdk_raise(X86Context &, uint32_t, const char *);
template<class T> class FFSDKGuestRef {
    static_assert(std::is_trivially_copyable<T>::value, "scalar guest reference only");
    X86Context *ctx_; uint8_t *base_; uint32_t address_;
public:
    FFSDKGuestRef(X86Context &ctx, uint8_t *base, uint32_t address) : ctx_(&ctx), base_(base), address_(address) {}
    operator T() const { T v; std::memcpy(&v, ff_sdk_checked_span(*ctx_,base_,address_,sizeof(T),FF_VM_READ),sizeof(T)); return v; }
    FFSDKGuestRef &operator=(T v) { std::memcpy(ff_sdk_checked_span(*ctx_,base_,address_,sizeof(T),FF_VM_WRITE),&v,sizeof(T)); return *this; }
    FFSDKGuestRef &operator=(const FFSDKGuestRef &o) { return *this = static_cast<T>(o); }
};
template<class T> T *ff_sdk_forbidden_guest_pointer(X86Context &, uint8_t *, uint32_t) = delete;
bool ff_sdk_bind_boot_thread(FFSDKRuntime &, ff_boot_thread &);
X86Context *ff_sdk_current_context(FFSDKRuntime &);
void ff_sdk_dispatch(X86Context &, uint8_t *, uint32_t);
bool ff_sdk_context_active(X86Context &, uint8_t *);
bool ff_sdk_direct_entry(X86Context &, uint8_t *, X86Func *);
bool ff_sdk_read_stdcall_frame(X86Context &, uint8_t *, uint32_t, uint32_t *, uint32_t *, uint32_t);
void ff_sdk_complete_stdcall(X86Context &, uint32_t, uint32_t);
void ff_sdk_kernel_dispatch(X86Context &, uint8_t *, uint32_t);
extern "C" uint32_t ff_sdk_kernel_last_eax;
extern "C" uint32_t ff_sdk_kernel_last_esp;
