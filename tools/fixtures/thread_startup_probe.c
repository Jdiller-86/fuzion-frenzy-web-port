/* Owned boot-thread host. Generated title code and upstream types stay private.
 * The previous stop-only harness is preserved separately and remains reproducible. */
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include "title-profile.h"
#include "../../src/runtime/thread_bootstrap.h"
#include "../../src/runtime/kernel_objects.h"
#include "../../src/runtime/local_settings.h"
#if !defined(__wasm32__) || defined(__wasm64__)
#error This proof requires wasm32
#endif
#define FF_GUEST_BYTES (64u * 1024u * 1024u)
#define FF_STACK_TOP (FF_GUEST_BYTES - 4096u)
#define FF_KERNEL_BASE 0xfe000000u

ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo, g_xbox_code_hi;
int g_force_return;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_ebp;
RECOMP_TLS uint32_t g_fs_base, g_seh_ebp, g_eflags, g_mxcsr;
RECOMP_TLS int g_df, g_fp_top, g_fp_cmp;
RECOMP_TLS uint16_t g_fp_control_word, g_fp_cc;
RECOMP_TLS double g_fp_stack[8];
RECOMP_TLS RecompXmm g_xmm0, g_xmm1, g_xmm2, g_xmm3, g_xmm4, g_xmm5, g_xmm6, g_xmm7;
RECOMP_TLS RecompMmx g_mm0, g_mm1, g_mm2, g_mm3, g_mm4, g_mm5, g_mm6, g_mm7;
volatile uint32_t g_icall_trace[ICALL_TRACE_SIZE], g_icall_trace_idx;
volatile uint64_t g_icall_count;
static uint8_t guest_ram[FF_GUEST_BYTES] __attribute__((aligned(16)));
static uint32_t probe_state[16], probe_trace[64 * 3], probe_trace_count;
static uint32_t kernel_calls[16][12], kernel_call_count, parent_returned;
static ff_boot_runtime boot;
static uint32_t misc_flags, misc_flags_ready;
static const char source_digest[] = FF_SOURCE_SHA256;
static const struct { uint32_t slot, ordinal; } bindings[] = FF_KERNEL_BINDINGS;

static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal)
{
    probe_state[0] = status; probe_state[1] = target; probe_state[2] = ordinal; probe_state[3] = g_esp;
    if (g_esp <= FF_GUEST_BYTES - 44u && !(g_esp & 3u)) {
        probe_state[4] = 10; probe_state[5] = MEM32(g_esp);
        for (unsigned i = 0; i < 10; ++i) probe_state[6 + i] = MEM32(g_esp + 4u + i * 4u);
    } else probe_state[0] = 4;
    __builtin_trap();
}
static _Noreturn void ff_probe_stop_direct(uint32_t target) { stop(2, target, 0); }
static void trace(uint32_t kind, uint32_t address)
{
    if (probe_trace_count == 64) stop(5, address, 0);
    uint32_t i = probe_trace_count++ * 3;
    probe_trace[i] = kind; probe_trace[i + 1] = address; probe_trace[i + 2] = g_esp;
}
void recomp_trace_enter(const char *name, uint32_t address) { (void)name; trace(1, address); }
void recomp_trace_exit(const char *name, uint32_t address) { (void)name; trace(2, address); }
void recomp_trace_esp(const char *name, const char *tag) { (void)name; (void)tag; }
void recomp_icall_fail_log(uint32_t address) { stop(3, address, 0); }
void recomp_icall_not_code_log(uint32_t address) { stop(3, address, 0); }
void recomp_unimpl(const char *text, uint32_t address) { (void)text; stop(7, address, 0); }
recomp_func_t recomp_lookup_manual(uint32_t address) { (void)address; return 0; }
recomp_func_t recomp_lookup(uint32_t address) { switch (address) { FF_TITLE_LOOKUP_CASES default: return 0; } }

static uint32_t *kernel_record(uint32_t ordinal, unsigned argc)
{
    if (kernel_call_count == 16 || (g_esp & 3u) || g_esp > FF_GUEST_BYTES - 4u * (argc + 1u)) stop(4, 0, ordinal);
    uint32_t *row = kernel_calls[kernel_call_count++]; row[0] = ordinal;
    for (unsigned i = 0; i < argc; ++i) row[i + 2] = MEM32(g_esp + 4 + i * 4);
    return row;
}
static void create_thread(void)
{
    uint32_t *row = kernel_record(255, 10);
    if (!recomp_lookup(row[7]) || !recomp_lookup(row[11])) stop(9, row[11], 255);
    row[1] = ff_boot_create(&boot, row + 2);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 255);
    g_eax = row[1]; g_esp += 44; /* NTAPI return address and ten arguments. */
}
static void close_thread_handle(void)
{
    uint32_t *row = kernel_record(187, 1);
    row[1] = ff_boot_close(&boot, row[2]);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 187);
    g_eax = row[1]; g_esp += 8;
}
static void enter_critical(void)
{
    uint32_t *row = kernel_record(277, 1);
    row[1] = ff_boot_enter_critical(&boot, row[2]);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 277);
    g_esp += 8;
}
static void leave_critical(void)
{
    uint32_t *row = kernel_record(294, 1);
    row[1] = ff_boot_leave_critical(&boot, row[2]);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 294);
    g_esp += 8;
}
static void initialize_dpc(void)
{
    uint32_t *row = kernel_record(107, 3);
    row[1] = ff_initialize_dpc(guest_ram, FF_GUEST_BYTES, row[2], row[3], row[4]);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 107);
    g_esp += 16;
}
static void initialize_timer(void)
{
    uint32_t *row = kernel_record(113, 2);
    row[1] = ff_initialize_timer(guest_ram, FF_GUEST_BYTES, row[2], row[3]);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 113);
    g_esp += 12;
}
static void query_nonvolatile(void)
{
    uint32_t *row = kernel_record(24, 5);
    if (!misc_flags_ready || row[2] != 17) stop(9, row[2], 24);
    row[1] = ff_query_misc_flags(guest_ram, FF_GUEST_BYTES, row + 2, misc_flags);
    if (row[1] != FF_BOOT_OK && row[1] != 0xc0000023u) stop(9, row[1], 24);
    if (row[1] == FF_BOOT_OK) {
        row[7] = MEM32(row[3]); row[8] = MEM32(row[4]); row[9] = row[6] ? MEM32(row[6]) : 4;
    }
    g_eax = row[1]; g_esp += 24;
}
static void status_to_dos_error(void)
{
    uint32_t *row = kernel_record(301, 1);
    /* The observed STATUS_SUCCESS -> ERROR_SUCCESS conversion is exact.
     * Other statuses remain unsupported, never silently mapped to success. */
    if (row[2] != 0) stop(9, row[2], 301);
    row[1] = 0; g_eax = 0; g_esp += 8;
}
recomp_func_t recomp_lookup_kernel(uint32_t address)
{
    uint32_t offset = address - FF_KERNEL_BASE;
    if (!(offset & 3u) && offset / 4u < FF_KERNEL_BINDING_COUNT) {
        uint32_t ordinal = bindings[offset / 4u].ordinal;
        if (ordinal == 255) return create_thread;
        if (ordinal == 187) return close_thread_handle;
        if (ordinal == 277) return enter_critical;
        if (ordinal == 294) return leave_critical;
        if (ordinal == 107) return initialize_dpc;
        if (ordinal == 113) return initialize_timer;
        if (ordinal == 24) return query_nonvolatile;
        if (ordinal == 301) return status_to_dos_error;
        stop(1, address, ordinal);
    }
    return 0;
}

uint32_t ff_probe_abi(void) { return 3; }
void ff_probe_misc_flags(uint32_t value) { misc_flags = value; misc_flags_ready = 1; }
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
uint32_t ff_probe_thread_count(void) { return boot.count; }
uint32_t ff_probe_parent_returned(void) { return parent_returned; }
uint32_t ff_probe_kernel_count(void) { return kernel_call_count; }
uint32_t ff_probe_kernel_word(uint32_t i, uint32_t n) { return i < kernel_call_count && n < 12 ? kernel_calls[i][n] : 0; }
uint32_t ff_probe_thread_word(uint32_t i, uint32_t n)
{
    if (i >= boot.count) return 0;
    const ff_boot_thread *t = &boot.threads[i];
    switch (n) {
        case 0: return t->id; case 1: return t->handle; case 2: return t->handle_open; case 3: return t->state;
        case 4: return t->object; case 5: return t->fs; case 6: return t->stack_low; case 7: return t->stack_top;
        case 8: return t->tls; case 9: return t->tls_size; case 10: return t->routine; case 11: return t->context;
        case 12: return t->system; case 13: return t->initial_sp; default: return 0;
    }
}
void ff_probe_reset(void)
{
    memset(probe_state, 0, sizeof probe_state); memset(probe_trace, 0, sizeof probe_trace);
    memset(kernel_calls, 0, sizeof kernel_calls);
    probe_trace_count = kernel_call_count = parent_returned = 0;
    misc_flags = misc_flags_ready = 0;
    for (unsigned i = 0; i < ICALL_TRACE_SIZE; ++i) g_icall_trace[i] = 0;
    g_icall_trace_idx = 0; g_icall_count = 0; g_force_return = 0;
    g_xbox_mem_offset = (ptrdiff_t)(uintptr_t)guest_ram;
    g_xbox_code_lo = FF_CODE_LOW; g_xbox_code_hi = FF_CODE_HIGH;
    g_eax = g_ecx = g_edx = g_esp = g_ebx = g_esi = g_edi = g_ebp = 0;
    g_fs_base = g_seh_ebp = g_eflags = 0; g_df = g_fp_top = 0;
    g_fp_control_word = 0x37f; g_mxcsr = 0x1f80;
    if (ff_boot_init(&boot, guest_ram, FF_GUEST_BYTES, 0x03c00000u, 0x03e00000u) != FF_BOOT_OK) stop(9, 0, 0);
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
    g_esp = FF_STACK_TOP; MEM32(g_esp) = 0;
    FF_ENTRY_FUNCTION(); parent_returned = 1;
    ff_boot_thread *thread = ff_boot_next(&boot);
    if (!thread) stop(9, 0, 0);
    g_eax = g_ecx = g_edx = g_ebx = g_esi = g_edi = g_ebp = g_seh_ebp = 0;
    g_esp = thread->initial_sp; g_fs_base = thread->fs; g_eflags = 0x202; g_df = 0;
    recomp_func_t entry = recomp_lookup(thread->system);
    if (!entry) stop(2, thread->system, 0);
    entry(); stop(6, thread->system, 0);
}
#include "title-functions.inc"
