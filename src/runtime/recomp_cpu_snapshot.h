#ifndef FF_RECOMP_CPU_SNAPSHOT_H
#define FF_RECOMP_CPU_SNAPSHOT_H
#include "dpc_context.h"
#include <string.h>

/* Include after the recompiler's CPU-global declarations. The owned fixture
 * provides same-sized independent globals; no vendor header is redistributed. */
_Static_assert(sizeof(g_xmm0) == 16 && sizeof(g_mm0) == 8 && sizeof(g_fp_stack) == 64, "Unsupported recompiler CPU layout");
static inline void ff_capture_cpu(ff_guest_cpu *cpu)
{
    memset(cpu, 0, sizeof *cpu);
#define FF_CAPTURE(name) cpu->r_##name = g_##name;
    FF_CPU_U32_FIELDS(FF_CAPTURE) FF_CPU_INT_FIELDS(FF_CAPTURE) FF_CPU_U16_FIELDS(FF_CAPTURE)
#undef FF_CAPTURE
    memcpy(cpu->fp_values, g_fp_stack, sizeof cpu->fp_values);
#define FF_CAPTURE_VECTOR(n) memcpy(cpu->xmm_bytes[n], &g_xmm##n, 16); memcpy(cpu->mm_bytes[n], &g_mm##n, 8);
    FF_CAPTURE_VECTOR(0) FF_CAPTURE_VECTOR(1) FF_CAPTURE_VECTOR(2) FF_CAPTURE_VECTOR(3)
    FF_CAPTURE_VECTOR(4) FF_CAPTURE_VECTOR(5) FF_CAPTURE_VECTOR(6) FF_CAPTURE_VECTOR(7)
#undef FF_CAPTURE_VECTOR
}
static inline void ff_restore_cpu(const ff_guest_cpu *cpu)
{
#define FF_RESTORE(name) g_##name = cpu->r_##name;
    FF_CPU_U32_FIELDS(FF_RESTORE) FF_CPU_INT_FIELDS(FF_RESTORE) FF_CPU_U16_FIELDS(FF_RESTORE)
#undef FF_RESTORE
    memcpy(g_fp_stack, cpu->fp_values, sizeof cpu->fp_values);
#define FF_RESTORE_VECTOR(n) memcpy(&g_xmm##n, cpu->xmm_bytes[n], 16); memcpy(&g_mm##n, cpu->mm_bytes[n], 8);
    FF_RESTORE_VECTOR(0) FF_RESTORE_VECTOR(1) FF_RESTORE_VECTOR(2) FF_RESTORE_VECTOR(3)
    FF_RESTORE_VECTOR(4) FF_RESTORE_VECTOR(5) FF_RESTORE_VECTOR(6) FF_RESTORE_VECTOR(7)
#undef FF_RESTORE_VECTOR
}
#endif
