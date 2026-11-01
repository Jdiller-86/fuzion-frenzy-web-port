#ifndef FF_RECOMP_MEMORY_ACCESS_H
#define FF_RECOMP_MEMORY_ACCESS_H
#include <stdint.h>
#if !defined(__GNUC__) && !defined(__clang__)
#error Checked guest scalar references require packed unaligned fields
#endif

/* Packed scalar lvalues preserve x86 unaligned loads/stores. The host checks
 * the exact range and read/write intent before returning a private RAM pointer. */
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access);
#define FF_MEMORY_POINTER(address, bytes, access) ff_probe_guest_memory((uint32_t)(address), (uint64_t)(bytes), (access))
#define FF_MEMORY_TYPE(name, type) typedef struct __attribute__((packed)) { type value; } ff_guest_##name;
FF_MEMORY_TYPE(U8, uint8_t) FF_MEMORY_TYPE(U16, uint16_t) FF_MEMORY_TYPE(U32, uint32_t) FF_MEMORY_TYPE(U64, uint64_t)
FF_MEMORY_TYPE(I8, int8_t) FF_MEMORY_TYPE(I16, int16_t) FF_MEMORY_TYPE(I32, int32_t) FF_MEMORY_TYPE(I64, int64_t)
FF_MEMORY_TYPE(F32, float) FF_MEMORY_TYPE(F64, double)
#undef FF_MEMORY_TYPE
#define FF_MEMORY_VALUE(name, address, access) (((volatile ff_guest_##name *)FF_MEMORY_POINTER(address, sizeof(ff_guest_##name), access))->value)
#define FF_MEMORY_U8(a, p) FF_MEMORY_VALUE(U8, a, p)
#define FF_MEMORY_U16(a, p) FF_MEMORY_VALUE(U16, a, p)
#define FF_MEMORY_U32(a, p) FF_MEMORY_VALUE(U32, a, p)
#define FF_MEMORY_U64(a, p) FF_MEMORY_VALUE(U64, a, p)
#define FF_MEMORY_I8(a, p) FF_MEMORY_VALUE(I8, a, p)
#define FF_MEMORY_I16(a, p) FF_MEMORY_VALUE(I16, a, p)
#define FF_MEMORY_I32(a, p) FF_MEMORY_VALUE(I32, a, p)
#define FF_MEMORY_I64(a, p) FF_MEMORY_VALUE(I64, a, p)
#define FF_MEMORY_F32(a, p) FF_MEMORY_VALUE(F32, a, p)
#define FF_MEMORY_F64(a, p) FF_MEMORY_VALUE(F64, a, p)
#define FF_MEMORY_PUSH(sp, value) do { uint32_t ff_push_value = (uint32_t)(value); (sp) -= 4u; FF_MEMORY_U32(sp, 2u) = ff_push_value; } while (0)
#define FF_MEMORY_POP(sp, destination) do { uint32_t ff_pop_value = FF_MEMORY_U32(sp, 1u); (sp) += 4u; (destination) = ff_pop_value; } while (0)
/* The pinned dispatch macros use these for their implicit guest call frames. */
#undef PUSH32
#undef POP32
#define PUSH32(sp, value) FF_MEMORY_PUSH(sp, value)
#define POP32(sp, destination) FF_MEMORY_POP(sp, destination)
#endif
