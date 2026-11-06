/* Owned compiled-artifact probe for the callee-saved push/pop macro pattern.
 * No original code: the sequence and macro shapes mirror recomp_memory_access.h
 * and the emitted prologue/epilogue order.
 *
 * The probe is compiled to Wasm and executed under Node; it returns a packed
 * value of the four restored registers so the compiled mapping can be compared
 * with the emitted text. Returns 1 when the compiled restore is LIFO-correct.
 */
#include <stdint.h>
#include <stdio.h>

typedef struct __attribute__((packed)) { uint32_t value; } ff_guest_u32;

static uint32_t guest[16];
static uint32_t g_esp;

static void *guest_pointer(uint32_t address)
{
    return &guest[(address >> 2) & 15u];
}
#define FF_MEMORY_U32(a) (((volatile ff_guest_u32 *)guest_pointer(a))->value)
#define FF_MEMORY_PUSH(sp, value) do { uint32_t ff_push_value = (uint32_t)(value); (sp) -= 4u; FF_MEMORY_U32(sp) = ff_push_value; } while (0)
#define FF_MEMORY_POP(sp, destination) do { uint32_t ff_pop_value = FF_MEMORY_U32(sp); (sp) += 4u; (destination) = ff_pop_value; } while (0)

#ifdef __EMSCRIPTEN__
__attribute__((export_name("run_probe")))
#endif
uint32_t run_probe(void)
{
    /* Mirror the emitted order: push ebx, ebp, esi, edi then pop edi, esi, ebp, ebx. */
    uint32_t esp = 32u;
    uint32_t ebx = 0x11u, ebp = 0x22u, esi = 0x33u, edi = 0x44u;
    FF_MEMORY_PUSH(esp, ebx);
    FF_MEMORY_PUSH(esp, ebp);
    FF_MEMORY_PUSH(esp, esi);
    FF_MEMORY_PUSH(esp, edi);
    ebx = ebp = esi = edi = 0u;
    FF_MEMORY_POP(esp, edi);
    FF_MEMORY_POP(esp, esi);
    FF_MEMORY_POP(esp, ebp);
    FF_MEMORY_POP(esp, ebx);
    g_esp = esp;
    /* Correct LIFO restore after push ebx,ebp,esi,edi and pop edi,esi,ebp,ebx. */
    return ((edi << 24) | (esi << 16) | (ebp << 8) | ebx) == 0x44332211u ? 1u : 0u;
}

#ifndef __EMSCRIPTEN__
int main(void)
{
    uint32_t ok = run_probe();
    if (ok != 1u) { printf("reg_save_compiled_probe: shifted\n"); return 1; }
    printf("reg_save_compiled_probe: ok\n");
    return 0;
}
#endif
