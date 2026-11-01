/* Source-owned, isolated host for a bounded original-startup Wasm proof.
 * The generated title includes and upstream type header remain private.
 * This is not a complete Xbox runtime. Every unsupported call stops execution.
 */
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include "title-profile.h"

#if !defined(__wasm32__) || defined(__wasm64__)
#error This proof ABI requires wasm32
#endif

#define FF_GUEST_BYTES (64u * 1024u * 1024u)
#define FF_STACK_TOP (FF_GUEST_BYTES - 4096u)
#define FF_KERNEL_BASE 0xfe000000u

ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo, g_xbox_code_hi;
int g_force_return;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_ebp;
RECOMP_TLS uint32_t g_fs_base, g_seh_ebp, g_eflags;
RECOMP_TLS int g_df;
volatile uint32_t g_icall_trace[ICALL_TRACE_SIZE], g_icall_trace_idx;
volatile uint64_t g_icall_count;

static uint8_t guest_ram[FF_GUEST_BYTES] __attribute__((aligned(16)));
/* status,target,ordinal,sp,arg_count,return_address,10 argument words */
static uint32_t probe_state[16];
/* Each trace record: enter=1/exit=2, guest address, simulated stack pointer. */
static uint32_t probe_trace[16 * 3], probe_trace_count;
static const char source_digest[] = FF_SOURCE_SHA256;
static const struct { uint32_t slot, ordinal; } bindings[] = FF_KERNEL_BINDINGS;

static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal)
{
    probe_state[0] = status;
    probe_state[1] = target;
    probe_state[2] = ordinal;
    probe_state[3] = g_esp;
    if (g_esp <= FF_GUEST_BYTES - 44u && !(g_esp & 3u)) {
        probe_state[4] = 10;
        probe_state[5] = MEM32(g_esp);
        for (unsigned i = 0; i < 10; ++i) probe_state[6 + i] = MEM32(g_esp + 4u + i * 4u);
    } else {
        probe_state[0] = 4; /* invalid stack, not a successful guest call */
    }
    __builtin_trap();
}

static _Noreturn void ff_probe_stop_direct(uint32_t target)
{
    stop(2, target, 0);
}

static void trace(uint32_t kind, uint32_t address)
{
    if (probe_trace_count == 16) stop(5, address, 0);
    uint32_t i = probe_trace_count++ * 3;
    probe_trace[i] = kind;
    probe_trace[i + 1] = address;
    probe_trace[i + 2] = g_esp;
}

void recomp_trace_enter(const char *name, uint32_t address) { (void)name; trace(1, address); }
void recomp_trace_exit(const char *name, uint32_t address) { (void)name; trace(2, address); }
void recomp_trace_esp(const char *name, const char *tag) { (void)name; (void)tag; }
void recomp_icall_fail_log(uint32_t address) { stop(3, address, 0); }
void recomp_icall_not_code_log(uint32_t address) { stop(3, address, 0); }
void recomp_unimpl(const char *text, uint32_t address) { (void)text; stop(7, address, 0); }
recomp_func_t recomp_lookup_manual(uint32_t address) { (void)address; return 0; }
recomp_func_t recomp_lookup(uint32_t address)
{
    switch (address) { FF_TITLE_LOOKUP_CASES default: return 0; }
}
recomp_func_t recomp_lookup_kernel(uint32_t address)
{
    uint32_t offset = address - FF_KERNEL_BASE;
    if (!(offset & 3u) && offset / 4u < FF_KERNEL_BINDING_COUNT) {
        stop(1, address, bindings[offset / 4u].ordinal);
    }
    return 0;
}

uint32_t ff_probe_abi(void) { return 1; }
uint32_t ff_probe_ram(void) { return (uint32_t)(uintptr_t)guest_ram; }
uint32_t ff_probe_ram_bytes(void) { return FF_GUEST_BYTES; }
uint32_t ff_probe_stack_top(void) { return FF_STACK_TOP; }
uint32_t ff_probe_source_digest(void) { return (uint32_t)(uintptr_t)source_digest; }
uint32_t ff_probe_entry(void) { return FF_ENTRY_ADDRESS; }
uint32_t ff_probe_state(void) { return (uint32_t)(uintptr_t)probe_state; }
uint32_t ff_probe_trace(void) { return (uint32_t)(uintptr_t)probe_trace; }
uint32_t ff_probe_trace_count(void) { return probe_trace_count; }
uint32_t ff_probe_binding_count(void) { return FF_KERNEL_BINDING_COUNT; }
uint32_t ff_probe_binding_slot(uint32_t i) { return i < FF_KERNEL_BINDING_COUNT ? bindings[i].slot : 0; }
uint32_t ff_probe_binding_ordinal(uint32_t i) { return i < FF_KERNEL_BINDING_COUNT ? bindings[i].ordinal : 0; }

void ff_probe_reset(void)
{
    for (unsigned i = 0; i < 16; ++i) probe_state[i] = 0;
    for (unsigned i = 0; i < 48; ++i) probe_trace[i] = 0;
    for (unsigned i = 0; i < ICALL_TRACE_SIZE; ++i) g_icall_trace[i] = 0;
    probe_trace_count = g_icall_trace_idx = 0;
    g_icall_count = 0;
    g_xbox_mem_offset = (ptrdiff_t)(uintptr_t)guest_ram;
    g_xbox_code_lo = FF_CODE_LOW; g_xbox_code_hi = FF_CODE_HIGH;
    g_eax = g_ecx = g_edx = g_esp = g_ebx = g_esi = g_edi = g_ebp = 0;
    g_fs_base = g_seh_ebp = g_eflags = 0;
    g_df = 0;
}

void ff_probe_bind_kernels(void)
{
    for (unsigned i = 0; i < FF_KERNEL_BINDING_COUNT; ++i) {
        if ((bindings[i].slot & 3u) || bindings[i].slot > FF_GUEST_BYTES - 4u) stop(3, bindings[i].slot, 0);
        MEM32(bindings[i].slot) = FF_KERNEL_BASE + i * 4u;
    }
}

void ff_probe_run(void)
{
    g_esp = FF_STACK_TOP;
    MEM32(g_esp) = 0;
    FF_ENTRY_FUNCTION();
    probe_state[0] = 6; /* selected entry returned; not a full-game success */
}

#include "title-functions.inc"
