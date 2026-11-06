#ifndef FF_RECOMP_MEMORY_ACCESS_H
#define FF_RECOMP_MEMORY_ACCESS_H
#include <stdint.h>
#if !defined(__GNUC__) && !defined(__clang__)
#error Checked guest scalar references require packed unaligned fields
#endif

/* Packed scalar lvalues preserve x86 unaligned loads/stores. The host checks
 * the exact range and read/write intent before returning a private RAM pointer.
 * site is the guest address of the translated basic block that performed the
 * access; it is never inferred from a host address. Stack and bulk helpers pass
 * zero, and the host admits zero only against owned RAM, never a device window. */
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site);
#define FF_MEMORY_POINTER(address, bytes, access) ff_probe_guest_memory((uint32_t)(address), (uint64_t)(bytes), (access), 0u)
#define FF_MEMORY_SITE_POINTER(address, bytes, access, site) ff_probe_guest_memory((uint32_t)(address), (uint64_t)(bytes), (access), (site))
/* Emitted vector operand whose original instruction requires 16-byte alignment
 * (movaps/movapd/movdqa and the non-temporal stores). The check lives in the host
 * hook, not in this macro: a ternary returning NULL would be folded away by the
 * optimizer because the generated code immediately dereferences the result.
 * The hook stops the machine on a misaligned address, exactly like a guest #GP.
 * Unaligned vector moves (movups/movupd/movdqu) keep FF_MEMORY_SITE_POINTER. */
void *ff_probe_guest_vector(uint32_t address, uint32_t access, uint32_t site);
#define FF_MEMORY_SITE_VECTOR(address, access, site) \
    ff_probe_guest_vector((uint32_t)(address), (access), (site))
#define FF_MEMORY_TYPE(name, type) typedef struct __attribute__((packed)) { type value; } ff_guest_##name;
FF_MEMORY_TYPE(U8, uint8_t) FF_MEMORY_TYPE(U16, uint16_t) FF_MEMORY_TYPE(U32, uint32_t) FF_MEMORY_TYPE(U64, uint64_t)
FF_MEMORY_TYPE(I8, int8_t) FF_MEMORY_TYPE(I16, int16_t) FF_MEMORY_TYPE(I32, int32_t) FF_MEMORY_TYPE(I64, int64_t)
FF_MEMORY_TYPE(F32, float) FF_MEMORY_TYPE(F64, double)
#undef FF_MEMORY_TYPE
#define FF_MEMORY_VALUE(name, address, access, site) (((volatile ff_guest_##name *)FF_MEMORY_SITE_POINTER(address, sizeof(ff_guest_##name), access, site))->value)
/* Site-less forms exist for independently written fixtures only; emitted
 * original-code accessors always name their guest block site. */
#define FF_MEMORY_U8(a, p) FF_MEMORY_VALUE(U8, a, p, 0u)
#define FF_MEMORY_U16(a, p) FF_MEMORY_VALUE(U16, a, p, 0u)
#define FF_MEMORY_U32(a, p) FF_MEMORY_VALUE(U32, a, p, 0u)
#define FF_MEMORY_U64(a, p) FF_MEMORY_VALUE(U64, a, p, 0u)
#define FF_MEMORY_I8(a, p) FF_MEMORY_VALUE(I8, a, p, 0u)
#define FF_MEMORY_I16(a, p) FF_MEMORY_VALUE(I16, a, p, 0u)
#define FF_MEMORY_I32(a, p) FF_MEMORY_VALUE(I32, a, p, 0u)
#define FF_MEMORY_I64(a, p) FF_MEMORY_VALUE(I64, a, p, 0u)
#define FF_MEMORY_F32(a, p) FF_MEMORY_VALUE(F32, a, p, 0u)
#define FF_MEMORY_F64(a, p) FF_MEMORY_VALUE(F64, a, p, 0u)
#define FF_MEMORY_SITE_U8(a, p, s) FF_MEMORY_VALUE(U8, a, p, s)
#define FF_MEMORY_SITE_U16(a, p, s) FF_MEMORY_VALUE(U16, a, p, s)
#define FF_MEMORY_SITE_U32(a, p, s) FF_MEMORY_VALUE(U32, a, p, s)
#define FF_MEMORY_SITE_U64(a, p, s) FF_MEMORY_VALUE(U64, a, p, s)
#define FF_MEMORY_SITE_I8(a, p, s) FF_MEMORY_VALUE(I8, a, p, s)
#define FF_MEMORY_SITE_I16(a, p, s) FF_MEMORY_VALUE(I16, a, p, s)
#define FF_MEMORY_SITE_I32(a, p, s) FF_MEMORY_VALUE(I32, a, p, s)
#define FF_MEMORY_SITE_I64(a, p, s) FF_MEMORY_VALUE(I64, a, p, s)
#define FF_MEMORY_SITE_F32(a, p, s) FF_MEMORY_VALUE(F32, a, p, s)
#define FF_MEMORY_SITE_F64(a, p, s) FF_MEMORY_VALUE(F64, a, p, s)
#define FF_MEMORY_PUSH(sp, value) do { uint32_t ff_push_value = (uint32_t)(value); (sp) -= 4u; FF_MEMORY_U32(sp, 2u) = ff_push_value; } while (0)
/* Optional destination capture for register diagnostics. It records the
 * destination lvalue's address and the popped value; it never changes the
 * assignment, so it cannot mask a binding defect. TUs opt in by defining
 * FF_REG_TRACE_POP_ENABLED and providing ff_reg_trace_note_pop. */
#ifdef FF_REG_TRACE_POP_ENABLED
void ff_reg_trace_note_pop(const void *destination, uint32_t stack, uint32_t value);
#define FF_REG_TRACE_POP(destination, stack, value) ff_reg_trace_note_pop((const void *)(destination), (stack), (value))
#else
#define FF_REG_TRACE_POP(destination, stack, value) ((void)0)
#endif
#define FF_MEMORY_POP(sp, destination) do { uint32_t ff_pop_value = FF_MEMORY_U32(sp, 1u); (sp) += 4u; (destination) = ff_pop_value; FF_REG_TRACE_POP(&(destination), (sp), ff_pop_value); } while (0)
/* The pinned dispatch macros use these for their implicit guest call frames. */
#undef PUSH32
#undef POP32
#define PUSH32(sp, value) FF_MEMORY_PUSH(sp, value)
#define POP32(sp, destination) FF_MEMORY_POP(sp, destination)
#endif
