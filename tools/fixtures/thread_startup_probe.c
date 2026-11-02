/* Owned boot-thread host. Generated title code and upstream types stay private.
 * The previous stop-only harness is preserved separately and remains reproducible. */
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#define FF_RETURN_FRAME(address, frame) ff_probe_return_frame(address, frame)
static void ff_probe_return_frame(uint32_t address, uint32_t frame);
#include "title-profile.h"
#include "../../src/runtime/thread_bootstrap.h"
#include "../../src/runtime/kernel_objects.h"
#include "../../src/runtime/local_settings.h"
#ifdef FF_VM_PROOF
#include "../../src/runtime/virtual_memory.h"
#ifndef FF_GUARDED_MEMORY
#error Virtual-memory proof requires checked translated accesses
#endif
#endif
#ifdef FF_TIMER_PROOF
#include "../../src/runtime/kernel_timers.h"
#include "../../src/runtime/dpc_context.h"
#endif
#ifdef FF_VOLUME_PROOF
#ifndef FF_VM_PROOF
#error Volume services require checked guest memory
#endif
#include "../../src/runtime/local_volume.h"
#endif
#ifdef FF_NAMESPACE_PROOF
#ifndef FF_VOLUME_PROOF
#error Namespace proof retains the explicit local volume contract
#endif
#include "../../src/runtime/object_namespace.h"
#endif
#if !defined(__wasm32__) || defined(__wasm64__)
#error This proof requires wasm32
#endif
#define FF_GUEST_BYTES (64u * 1024u * 1024u)
#define FF_STACK_TOP (FF_GUEST_BYTES - 4096u)
#define FF_KERNEL_BASE 0xfe000000u
#if defined(FF_METADATA_TRACE_PROOF) || defined(FF_METADATA_FORMAT_PROOF)
#ifndef FF_REGULAR_FILE_PROOF
#error Expanded metadata traces retain the explicit fresh regular-file contract
#endif
#define FF_TRACE_CAPACITY 1024u
#else
#define FF_TRACE_CAPACITY 128u
#endif
#define FF_KERNEL_CAPACITY 128u

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
#ifdef FF_TIMER_PROOF
#include "../../src/runtime/recomp_cpu_snapshot.h"
#endif
volatile uint32_t g_icall_trace[ICALL_TRACE_SIZE], g_icall_trace_idx;
volatile uint64_t g_icall_count;
static uint8_t guest_ram[FF_GUEST_BYTES] __attribute__((aligned(16)));
static uint32_t probe_state[16], probe_trace[FF_TRACE_CAPACITY * 3], probe_trace_count;
static uint32_t probe_cpu_trace[FF_TRACE_CAPACITY * 3];
static uint32_t kernel_calls[FF_KERNEL_CAPACITY][20], kernel_call_count, parent_returned;
static ff_boot_runtime boot;
static uint32_t misc_flags, misc_flags_ready;
#ifdef FF_VOLUME_PROOF
static ff_local_volume local_volume;
static uint32_t volume_ready;
#endif
#ifdef FF_DIRECTORY_PROOF
#ifndef FF_NAMESPACE_PROOF
#error Directory proof retains the explicit volume and namespace contracts
#endif
#include "../../src/runtime/guest_strings.h"
#endif
#ifdef FF_FILE_LINK_PROOF
#ifndef FF_DIRECTORY_PROOF
#error File-target links retain the explicit fresh-directory contract
#endif
#include "../../src/runtime/file_namespace.h"
#endif
#ifdef FF_REGULAR_FILE_PROOF
#if !defined(FF_FILE_LINK_PROOF) || !defined(FF_TIMER_PROOF)
#error Regular files retain the existing file-target and fresh-directory contract
#endif
static uint32_t file_information[FF_KERNEL_CAPACITY][14];
static uint32_t create_request_words[FF_KERNEL_CAPACITY][6];
static uint8_t create_request_names[FF_KERNEL_CAPACITY][256];
#endif
#ifdef FF_NAMESPACE_PROOF
static ff_object_namespace object_namespace;
static uint32_t namespace_ready;
#endif
#ifdef FF_VM_PROOF
static ff_virtual_memory virtual_memory;
static uint32_t vm_ready, vm_access_count, vm_fault[4];
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access);
#endif
#ifdef FF_FORMAT_PROOF
#ifndef FF_VM_PROOF
#error Format observations require checked guest memory
#endif
static struct { uint32_t words[13]; uint8_t format[64], output[128]; } format_observation;
#endif
#ifdef FF_METADATA_FORMAT_PROOF
#ifndef FF_FORMAT_PROOF
#error Metadata observations retain the checked format observer contract
#endif
static struct { uint32_t words[21]; uint8_t format[256], output[520], input[3][256]; } metadata_observation;
#endif
#ifdef FF_FILE_WRITE_PROOF
#ifndef FF_METADATA_FORMAT_PROOF
#error File writes retain the bounded original metadata observation contract
#endif
#endif
#ifdef FF_TIMER_PROOF
static ff_timer_queue timers;
static ff_dpc_frame dpc_frame;
static uint32_t timers_ready, dpc_delivered, clock_words[4];
__attribute__((import_module("env"), import_name("ff_host_clock")))
extern void ff_host_clock(uint32_t output);
void ff_probe_poll(void);
void ff_probe_dpc_recover(void);
#endif
#ifdef FF_COPY_PROOF
#ifndef FF_TIMER_PROOF
#error Copy scenarios require the checked CPU snapshot bridge
#endif
static ff_guest_cpu copy_saved;
static uint32_t copy_checking;
static uint32_t copy_target, copy_return_frame, copy_return_seen;
void ff_probe_copy_recover(void);
#endif
static const char source_digest[] = FF_SOURCE_SHA256;
static const struct { uint32_t slot, ordinal; } bindings[] = FF_KERNEL_BINDINGS;

static void ff_probe_return_frame(uint32_t address, uint32_t frame)
{
#ifdef FF_COPY_PROOF
    if (copy_checking && address == copy_target) { copy_return_frame = frame; ++copy_return_seen; }
#else
    (void)address; (void)frame;
#endif
}

static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal)
{
    probe_state[0] = status; probe_state[1] = target; probe_state[2] = ordinal; probe_state[3] = g_esp;
    if (g_esp <= FF_GUEST_BYTES - 44u && !(g_esp & 3u)) {
        probe_state[4] = 10; probe_state[5] = MEM32(g_esp);
        for (unsigned i = 0; i < 10; ++i) probe_state[6 + i] = MEM32(g_esp + 4u + i * 4u);
    } else probe_state[0] = 4;
#ifdef FF_TIMER_PROOF
    ff_probe_dpc_recover();
#endif
#ifdef FF_COPY_PROOF
    ff_probe_copy_recover();
#endif
    __builtin_trap();
}
static _Noreturn void ff_probe_stop_direct(uint32_t target) { stop(2, target, 0); }
#ifdef FF_VM_PROOF
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access)
{
    if (!vm_ready || ff_vm_check(&virtual_memory, address, bytes, access)) {
        vm_fault[0] = address; vm_fault[1] = (uint32_t)bytes; vm_fault[2] = (uint32_t)(bytes >> 32); vm_fault[3] = access;
        stop(10, address, access);
    }
    ++vm_access_count;
    /* Zero-count string operations access no guest address. Return a harmless
     * in-array pointer even if their unused register is outside this window. */
    return guest_ram + (bytes ? address : 0);
}
uint32_t ff_probe_vm_image(uint32_t base, uint32_t bytes)
{
    if (vm_ready || ff_vm_claim_fixed(&virtual_memory, base, bytes) ||
        ff_vm_claim_fixed(&virtual_memory, 0x03c00000u, 0x00400000u) ||
        ff_vm_map_fixed(&virtual_memory, 0x03c00000u, 0x00400000u, 4)) return FF_BOOT_INVALID;
    vm_ready = 1; return 0;
}
uint32_t ff_probe_vm_map_image(uint32_t base, uint32_t bytes, uint32_t protection)
{
    return vm_ready ? ff_vm_map_fixed(&virtual_memory, base, bytes, protection) : FF_BOOT_INVALID;
}
uint32_t ff_probe_vm_region_count(void)
{
    uint32_t n = 0;
    for (unsigned i = 0; i < FF_VM_REGIONS; ++i) if (virtual_memory.regions[i].kind) ++n;
    return n;
}
uint32_t ff_probe_vm_region_word(uint32_t ordinal, uint32_t field)
{
    for (unsigned i = 0; i < FF_VM_REGIONS; ++i) if (virtual_memory.regions[i].kind) {
        if (ordinal--) continue;
        const ff_vm_region *r = &virtual_memory.regions[i];
        switch (field) { case 0: return r->base; case 1: return r->bytes; case 2: return r->kind; case 3: return r->protection; default: return 0; }
    }
    return 0;
}
uint32_t ff_probe_vm_page_word(uint32_t address, uint32_t field)
{
    if (address >= FF_GUEST_BYTES) return 0;
    uint32_t p = address / FF_VM_PAGE;
    switch (field) { case 0: return virtual_memory.owner[p]; case 1: return virtual_memory.committed[p]; case 2: return virtual_memory.permission[p]; default: return 0; }
}
uint32_t ff_probe_vm_access_count(void) { return vm_access_count; }
uint32_t ff_probe_vm_fault_word(uint32_t index) { return index < 4 ? vm_fault[index] : 0; }
uint32_t ff_probe_vm_access_probe(uint32_t address, uint32_t access)
{
    if (access == FF_VM_READ) return FF_MEMORY_U8(address, FF_VM_READ);
    if (access == FF_VM_WRITE) { FF_MEMORY_U8(address, FF_VM_WRITE) = 0xb3; return 0; }
    stop(9, address, access);
}
#endif
#ifdef FF_FORMAT_PROOF
static uint32_t format_word(uint32_t address)
{
    return guest_ram[address] | (uint32_t)guest_ram[address+1]<<8 | (uint32_t)guest_ram[address+2]<<16 | (uint32_t)guest_ram[address+3]<<24;
}
static void observe_format(uint32_t kind,uint32_t address)
{
    if (address != 0x001dee6eu) return;
    uint32_t *words=format_observation.words;
    if (kind==1) {
        if (++words[0] != 1) return;
        words[2]=g_esp; words[8]=g_fs_base;
        if (g_fs_base && !ff_vm_check(&virtual_memory,g_fs_base,4,FF_VM_READ)) { words[9]=format_word(g_fs_base); words[7]|=4; }
        if (ff_vm_check(&virtual_memory,g_esp,16,FF_VM_READ)) return;
        words[3]=format_word(g_esp+4); words[4]=format_word(g_esp+8); words[5]=format_word(g_esp+12);
        for (unsigned i=0;i<sizeof format_observation.format;++i) {
            uint64_t byte=(uint64_t)words[4]+i;
            if (byte > UINT32_MAX || ff_vm_check(&virtual_memory,(uint32_t)byte,1,FF_VM_READ)) return;
            format_observation.format[i]=guest_ram[byte];
            if (!format_observation.format[i]) { words[7]|=1; return; }
        }
    } else {
        if (++words[1] != 1 || words[0] != 1) return;
        words[6]=g_eax; words[10]=g_esp; words[11]=g_fs_base;
        if (g_fs_base && !ff_vm_check(&virtual_memory,g_fs_base,4,FF_VM_READ)) words[12]=format_word(g_fs_base);
        else words[7]&=~4u;
        if (!(words[7]&1u) || g_eax >= sizeof format_observation.output || ff_vm_check(&virtual_memory,words[3],(uint64_t)g_eax+1,FF_VM_READ)) return;
        memcpy(format_observation.output,guest_ram+words[3],g_eax+1);
        if (!format_observation.output[g_eax]) words[7]|=2;
    }
}
uint32_t ff_probe_format_word(uint32_t index) { return index < 13 ? format_observation.words[index] : 0; }
uint32_t ff_probe_format_byte(uint32_t which,uint32_t index)
{
    if (!which && index < sizeof format_observation.format) return format_observation.format[index];
    if (which==1 && index < sizeof format_observation.output) return format_observation.output[index];
    return 0;
}
#endif
#ifdef FF_METADATA_FORMAT_PROOF
static int observe_wide_string(uint32_t address,uint8_t *output,uint32_t capacity,uint32_t *bytes)
{
    if (!address) return 0;
    for (uint32_t i=0;i+1<capacity;i+=2) {
        uint64_t at=(uint64_t)address+i;
        if (at > UINT32_MAX || ff_vm_check(&virtual_memory,(uint32_t)at,2,FF_VM_READ)) return 0;
        output[i]=guest_ram[at]; output[i+1]=guest_ram[at+1];
        if (!output[i] && !output[i+1]) { *bytes=i; return 1; }
    }
    return 0;
}
static void observe_metadata_format(uint32_t kind,uint32_t address)
{
    if (address != 0x001df4f0u) return;
    uint32_t *words=metadata_observation.words;
    if (kind==1) {
        if (++words[0] != 1) return;
        words[2]=g_esp; words[8]=g_fs_base;
        if (g_fs_base && !ff_vm_check(&virtual_memory,g_fs_base,4,FF_VM_READ)) { words[9]=format_word(g_fs_base); words[7]|=4; }
        if (ff_vm_check(&virtual_memory,g_esp,36,FF_VM_READ)) return;
        words[3]=format_word(g_esp+4); words[13]=format_word(g_esp+8);
        words[4]=format_word(g_esp+12); words[5]=format_word(g_esp+16); words[15]=format_word(g_esp+20);
        words[16]=format_word(g_esp+24); words[17]=format_word(g_esp+28); words[18]=format_word(g_esp+32);
        uint32_t format_bytes=0;
        if (observe_wide_string(words[15],metadata_observation.input[0],sizeof metadata_observation.input[0],&words[14])) words[7]|=8;
        if (observe_wide_string(words[17],metadata_observation.input[1],sizeof metadata_observation.input[1],&words[19])) words[7]|=16;
        if (observe_wide_string(words[18],metadata_observation.input[2],sizeof metadata_observation.input[2],&words[20])) words[7]|=32;
        if (observe_wide_string(words[4],metadata_observation.format,sizeof metadata_observation.format,&format_bytes)) words[7]|=1;
    } else {
        if (++words[1] != 1 || words[0] != 1) return;
        words[6]=g_eax; words[10]=g_esp; words[11]=g_fs_base;
        if (g_fs_base && !ff_vm_check(&virtual_memory,g_fs_base,4,FF_VM_READ)) words[12]=format_word(g_fs_base);
        else words[7]&=~4u;
        if (!(words[7]&1u) || !words[13] || words[13] > sizeof metadata_observation.output/2 || g_eax >= words[13] ||
            g_eax >= sizeof metadata_observation.output/2 || ff_vm_check(&virtual_memory,words[3],((uint64_t)g_eax+1)*2,FF_VM_READ)) return;
        memcpy(metadata_observation.output,guest_ram+words[3],(g_eax+1)*2);
        if (!metadata_observation.output[g_eax*2] && !metadata_observation.output[g_eax*2+1]) words[7]|=2;
    }
}
uint32_t ff_probe_metadata_format_word(uint32_t index) { return index < 21 ? metadata_observation.words[index] : 0; }
uint32_t ff_probe_metadata_format_byte(uint32_t which,uint32_t index)
{
    if (!which && index < sizeof metadata_observation.format) return metadata_observation.format[index];
    if (which==1 && index < sizeof metadata_observation.output) return metadata_observation.output[index];
    if (which>=2 && which<=4 && index < sizeof metadata_observation.input[0]) return metadata_observation.input[which-2][index];
    return 0;
}
#endif
static void trace(uint32_t kind, uint32_t address)
{
#ifdef FF_COPY_PROOF
    if (copy_checking) return;
#endif
#ifdef FF_FORMAT_PROOF
    observe_format(kind,address);
#endif
#ifdef FF_METADATA_FORMAT_PROOF
    observe_metadata_format(kind,address);
#endif
    if (probe_trace_count == FF_TRACE_CAPACITY) stop(5, address, 0);
    uint32_t i = probe_trace_count++ * 3;
    probe_trace[i] = kind; probe_trace[i + 1] = address; probe_trace[i + 2] = g_esp;
    probe_cpu_trace[i] = g_eax; probe_cpu_trace[i + 1] = g_fs_base;
    if (g_fs_base) {
        if ((g_fs_base & 3u) || g_fs_base > FF_GUEST_BYTES - 4u) stop(9, g_fs_base, 0);
#ifdef FF_VM_PROOF
        probe_cpu_trace[i + 2] = FF_MEMORY_U32(g_fs_base, FF_VM_READ);
#else
        probe_cpu_trace[i + 2] = MEM32(g_fs_base);
#endif
    } else probe_cpu_trace[i + 2] = UINT32_MAX;
}
void recomp_trace_enter(const char *name, uint32_t address) {
    (void)name;
#ifdef FF_VM_PROOF
    (void)ff_probe_guest_memory(address, 1, FF_VM_EXECUTE);
#endif
#ifdef FF_TIMER_PROOF
    ff_probe_poll();
#endif
    trace(1, address);
}
void recomp_trace_exit(const char *name, uint32_t address) {
    (void)name;
#ifdef FF_VM_PROOF
    (void)ff_probe_guest_memory(g_esp, 4, FF_VM_READ);
#endif
    trace(2, address);
}
void recomp_trace_esp(const char *name, const char *tag) { (void)name; (void)tag; }
void recomp_icall_fail_log(uint32_t address) { stop(3, address, 0); }
void recomp_icall_not_code_log(uint32_t address) { stop(3, address, 0); }
void recomp_unimpl(const char *text, uint32_t address) { (void)text; stop(7, address, 0); }
recomp_func_t recomp_lookup_manual(uint32_t address) { (void)address; return 0; }
recomp_func_t recomp_lookup(uint32_t address) { switch (address) { FF_TITLE_LOOKUP_CASES default: return 0; } }

#ifdef FF_TIMER_PROOF
static void sample_clock(void)
{
    if (!timers_ready) stop(9, 0, 149);
    ff_host_clock((uint32_t)(uintptr_t)clock_words);
    uint64_t interrupt = clock_words[0] | (uint64_t)clock_words[1] << 32;
    uint64_t system = clock_words[2] | (uint64_t)clock_words[3] << 32;
    uint32_t status = ff_timers_advance(&timers, interrupt, system);
    if (status != FF_BOOT_OK) stop(9, status, 149);
}
void ff_probe_dpc_recover(void)
{
    if (!dpc_frame.active) return;
    ff_guest_cpu cpu; ff_capture_cpu(&cpu);
    if (ff_dpc_abort(&dpc_frame, &cpu, guest_ram, FF_GUEST_BYTES) != FF_BOOT_OK) __builtin_trap();
    ff_restore_cpu(&cpu);
}
void ff_probe_poll(void)
{
#ifdef FF_COPY_PROOF
    if (copy_checking) return;
#endif
    if (!timers_ready || dpc_frame.active) return;
    sample_clock();
    for (unsigned i = 0; i < FF_DPC_CAPACITY; ++i) {
        if (!g_fs_base || g_fs_base > FF_GUEST_BYTES - 0x5cu) stop(9, g_fs_base, 0);
        ff_dpc_call call; uint32_t present;
        uint32_t status = ff_dpc_peek(&timers, guest_ram[g_fs_base + 0x24], &call, &present);
        if (status != FF_BOOT_OK) stop(9, status, 0);
        if (!present) return;
        recomp_func_t callback = recomp_lookup(call.routine);
        if (!callback) stop(2, call.routine, 0);
        ff_guest_cpu cpu; ff_capture_cpu(&cpu);
        status = ff_dpc_begin(&dpc_frame, &cpu, guest_ram, FF_GUEST_BYTES, 0x03e01000u, 0x03e11000u, &call);
        if (status != FF_BOOT_OK) stop(9, status, 0);
        /* This pop consumes the admission obtained before raising IRQL. No guest
         * execution or yield occurs between peek, frame admission and dequeue. */
        status = ff_dpc_pop(&timers, dpc_frame.irql, call.object);
        if (status != FF_BOOT_OK) stop(9, status, 0);
        ff_restore_cpu(&cpu); callback(); ff_capture_cpu(&cpu);
        status = ff_dpc_end(&dpc_frame, &cpu, guest_ram, FF_GUEST_BYTES); ff_restore_cpu(&cpu);
        if (status != FF_BOOT_OK) stop(9, status, 0);
        ++dpc_delivered;
    }
    /* Further callbacks stay queued until the next safe point; none is dropped. */
}
uint32_t ff_probe_timer_count(void) { return timers.timer_count; }
uint32_t ff_probe_dpc_count(void) { return timers.dpc_count; }
uint32_t ff_probe_dpc_delivered(void) { return dpc_delivered; }
uint32_t ff_probe_timer_word(uint32_t i, uint32_t n)
{
    if (i >= timers.timer_count) return 0;
    const ff_timer_entry *t = &timers.timers[i];
    switch (n) {
        case 0: return t->object; case 1: return t->dpc; case 2: return t->absolute;
        case 3: return (uint32_t)t->deadline; case 4: return (uint32_t)(t->deadline >> 32);
        case 5: return (uint32_t)t->system_due; case 6: return (uint32_t)(t->system_due >> 32); default: return 0;
    }
}
#endif

#ifdef FF_COPY_PROOF
void ff_probe_copy_recover(void)
{
    if (!copy_checking) return;
    ff_restore_cpu(&copy_saved); copy_checking = 0;
}
static uint8_t copy_pattern(uint32_t index, uint32_t tag)
{
    return (uint8_t)(index * 73u + tag * 29u + (index >> 8) * 19u);
}
/* Source-owned diagnostic buffers/oracle. Only the selected original helper
 * performs the copy. The source pattern is a snapshot oracle even on overlap.
 * The ordinary boot trace/clock/state are separate from these synthetic cases. */
uint32_t ff_probe_copy_case(uint32_t target, uint32_t destination, uint32_t source, uint32_t bytes, uint32_t tag)
{
    const uint32_t base = 0x03f00020u, count = 8192u, top = 0x03f08000u, frame = top - 16u;
    recomp_func_t function = recomp_lookup(target);
    if (!function || copy_checking || dpc_frame.active || destination > count || source > count ||
        bytes > 4096u || bytes > count - destination || bytes > count - source) return 0x80000000u;
    ff_capture_cpu(&copy_saved);
    uint32_t old_calls = kernel_call_count, result = 0;
    for (uint32_t i = 0; i < count; ++i) guest_ram[base + i] = copy_pattern(i, tag);
    memset(guest_ram + base - 32, 0xb7, 32); memset(guest_ram + base + count, 0xb7, 32);
    MEM32(frame) = 0xfffffff8u; MEM32(frame + 4) = base + destination;
    MEM32(frame + 8) = base + source; MEM32(frame + 12) = bytes;
    g_eax = 0xa1b2c3d4u; g_ecx = 0x31415926u; g_edx = 0x27182818u; g_esp = frame;
    g_ebx = 0x11223344u; g_esi = 0x55667788u; g_edi = 0x99aabbccu; g_ebp = g_seh_ebp = 0x13579bdfu;
    g_df = 0; g_eflags = 0x202u; g_force_return = 0;
    copy_target = target; copy_return_frame = 0; copy_return_seen = 0;
    copy_checking = 1; function();
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t expected_index = i >= destination && i - destination < bytes ? source + i - destination : i;
        if (guest_ram[base + i] != copy_pattern(expected_index, tag)) { result |= 1; break; }
    }
    for (uint32_t i = 0; i < 32; ++i) if (guest_ram[base - 32 + i] != 0xb7 || guest_ram[base + count + i] != 0xb7) result |= 2;
    if (g_esp != top - 12 || MEM32(frame) != 0xfffffff8u) result |= 4;
    if (g_eax != base + destination) result |= 8;
    if (g_ebx != 0x11223344u || g_esi != 0x55667788u || g_edi != 0x99aabbccu ||
        copy_return_seen != 1 || copy_return_frame != 0x13579bdfu) result |= 16;
    if (g_df) result |= 32;
    if (kernel_call_count != old_calls) result |= 64;
    ff_probe_copy_recover(); return result;
}
#endif

static uint32_t *kernel_record(uint32_t ordinal, unsigned argc)
{
#ifdef FF_VM_PROOF
    (void)ff_probe_guest_memory(g_esp, 4u * (argc + 1u), FF_VM_READ);
#endif
#ifdef FF_TIMER_PROOF
    ff_probe_poll();
#endif
    if (kernel_call_count == FF_KERNEL_CAPACITY || (g_esp & 3u) || g_esp > FF_GUEST_BYTES - 4u * (argc + 1u)) stop(4, 0, ordinal);
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
static void close_handle(void)
{
    uint32_t *row = kernel_record(187, 1);
#ifdef FF_VOLUME_PROOF
    if (ff_volume_owns_handle(row[2])) row[1] = ff_volume_close(&local_volume, row[2]);
    else
#endif
    row[1] = ff_boot_close(&boot, row[2]);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 187);
    g_eax = row[1]; g_esp += 8;
}
#ifdef FF_NAMESPACE_PROOF
uint32_t ff_probe_namespace_init(void)
{
    if (!vm_ready || !volume_ready || namespace_ready) return FF_BOOT_INVALID;
    uint32_t status=ff_namespace_init(&object_namespace,&virtual_memory),id;
    if (!status) status=ff_namespace_add_device(&object_namespace,"\\Device\\CdRom0",14,FF_NS_LOCAL_DISC,&id);
#ifdef FF_FILE_LINK_PROOF
    if (!status) status=ff_file_namespace_bind(&object_namespace,&local_volume);
#endif
    if (!status) namespace_ready=1;
    return status;
}
static void create_symbolic_link(void)
{
    uint32_t *row=kernel_record(67,2),id=0;
    if (!namespace_ready) stop(9,0,67);
    row[1]=ff_namespace_create_link(&object_namespace,row[2],row[3],&id);
    if (row[1] == FF_BOOT_INVALID || row[1] == FF_BOOT_UNSUPPORTED || row[1] == FF_VM_ACCESS_VIOLATION) stop(9,row[1],67);
    if (!row[1]) {
        const ff_namespace_object *link=NULL,*target=NULL;
        for (unsigned i=0;i<FF_NS_OBJECTS;++i) if (object_namespace.objects[i].id==id) link=&object_namespace.objects[i];
        if (!link) stop(9,id,67);
        for (unsigned i=0;i<FF_NS_OBJECTS;++i) if (object_namespace.objects[i].id==link->target) target=&object_namespace.objects[i];
        if (!target) stop(9,link->target,67);
        row[4]=link->id; row[5]=target->id; row[6]=target->references; row[7]=target->device_kind; row[8]=target->media_ready;
#ifdef FF_FILE_LINK_PROOF
        row[9]=target->kind; row[10]=target->provider_object; row[14]=object_namespace.count;
        row[15]=local_volume.file_count; row[16]=local_volume.opened; row[17]=local_volume.closed; row[18]=local_volume.active;
        if (target->kind==FF_NS_FILE) {
            const ff_volume_file *file=ff_volume_file_object(&local_volume,target->provider_object);
            if(!file) stop(9,target->provider_object,67);
            row[11]=file->directory; row[12]=file->references; row[13]=file->handle;
        }
#endif
    }
    g_eax=row[1]; g_esp+=12;
}
uint32_t ff_probe_namespace_count(void) { return object_namespace.count; }
uint32_t ff_probe_namespace_word(uint32_t index,uint32_t field)
{
    if (index >= FF_NS_OBJECTS) return 0;
    const ff_namespace_object *object=&object_namespace.objects[index];
    switch (field) {
        case 0:return object->id; case 1:return object->parent; case 2:return object->kind; case 3:return object->references;
        case 4:return object->target; case 5:return object->device_kind; case 6:return object->media_ready;
        case 7:return object->provider_object; default:return 0;
    }
}
uint32_t ff_probe_namespace_name(uint32_t index,uint32_t byte)
{
    return index < FF_NS_OBJECTS && byte < FF_NS_NAME ? (uint8_t)object_namespace.objects[index].name[byte] : 0;
}
uint32_t ff_probe_namespace_target_name(uint32_t index,uint32_t byte)
{
    return index < FF_NS_OBJECTS && byte < FF_NS_PATH ? (uint8_t)object_namespace.objects[index].target_name[byte] : 0;
}
uint32_t ff_probe_namespace_resolution(uint32_t field)
{
    ff_namespace_resolution result={0};
    uint32_t status=ff_namespace_resolve(&object_namespace,"\\??\\d:\\",7,&result);
    switch (field) { case 0:return status; case 1:return result.object; case 2:return result.remaining_offset; case 3:return result.remaining_bytes; default:return 0; }
}
#endif
#ifdef FF_FILE_LINK_PROOF
uint32_t ff_probe_file_word(uint32_t index,uint32_t field)
{
    if(index>=FF_VOLUME_FILES) return 0;
    const ff_volume_file *file=&local_volume.files[index];
    switch(field) { case 0:return file->id; case 1:return file->directory; case 2:return file->access; case 3:return file->share;
        case 4:return file->options; case 5:return file->references; case 6:return file->handle; case 7:return file->data_file; default:return 0; }
}
uint32_t ff_probe_file_resolution(uint32_t field)
{
    ff_namespace_resolution result={0};
    uint32_t status=ff_namespace_resolve(&object_namespace,"\\??\\t:\\",7,&result);
    switch(field) { case 0:return status; case 1:return result.object; case 2:return result.remaining_offset; case 3:return result.remaining_bytes; default:return 0; }
}
#endif
#ifdef FF_REGULAR_FILE_PROOF
uint32_t ff_probe_file_storage_capacity(uint32_t field)
{
    return !field?FF_VOLUME_DATA_FILES:field==1?FF_VOLUME_FILE_BYTES:0;
}
uint32_t ff_probe_regular_word(uint32_t index,uint32_t field)
{
    if(index>=FF_VOLUME_DATA_FILES) return 0;
    const ff_volume_data_file *file=&local_volume.data_files[index];
    switch(field) {case 0:return file->id;case 1:return file->parent;case 2:return file->attributes;case 3:return file->bytes;
        case 4:return (uint32_t)file->creation_time;case 5:return (uint32_t)(file->creation_time>>32);
        case 6:return (uint32_t)file->access_time;case 7:return (uint32_t)(file->access_time>>32);
        case 8:return (uint32_t)file->write_time;case 9:return (uint32_t)(file->write_time>>32);
        case 10:return (uint32_t)file->change_time;case 11:return (uint32_t)(file->change_time>>32);
        case 12:return (uint32_t)file->allocated_bytes;case 13:return (uint32_t)(file->allocated_bytes>>32);default:return 0;}
}
uint32_t ff_probe_regular_name(uint32_t index,uint32_t byte)
{
    return index<FF_VOLUME_DATA_FILES && byte<FF_VOLUME_DIRECTORY_NAME?(uint8_t)local_volume.data_files[index].name[byte]:0;
}
uint32_t ff_probe_regular_byte(uint32_t index,uint32_t byte)
{
    return index<FF_VOLUME_DATA_FILES && byte<local_volume.data_files[index].bytes && byte<FF_VOLUME_FILE_BYTES?local_volume.data_files[index].data[byte]:0;
}
uint32_t ff_probe_file_information_word(uint32_t index,uint32_t word)
{
    return index<kernel_call_count && word<14?file_information[index][word]:0;
}
uint32_t ff_probe_create_request_word(uint32_t index,uint32_t word)
{
    return index<kernel_call_count && word<6?create_request_words[index][word]:0;
}
uint32_t ff_probe_create_request_name(uint32_t index,uint32_t byte)
{
    return index<kernel_call_count && byte<256?create_request_names[index][byte]:0;
}
#endif
#ifdef FF_VOLUME_PROOF
static void open_volume_root(void)
{
    uint32_t *row = kernel_record(202, 6);
    if (!volume_ready) stop(9, 0, 202);
    row[1] = ff_volume_open_root(&local_volume, row + 2);
    if (row[1] == FF_BOOT_UNSUPPORTED || row[1] == FF_VM_ACCESS_VIOLATION || row[1] == FF_BOOT_INVALID) stop(9, row[1], 202);
    if (!row[1]) { row[8] = MEM32(row[2]); row[9] = MEM32(row[5]); row[10] = MEM32(row[5] + 4); }
    g_eax = row[1]; g_esp += 28;
}
#ifdef FF_DIRECTORY_PROOF
#ifdef FF_FILE_LINK_PROOF
static void equal_ansi_string(void)
{
    uint32_t *row=kernel_record(279,3),equal=0;
    row[1]=ff_equal_ansi_string(&virtual_memory,row[2],row[3],(uint8_t)row[4],&equal);
    if(row[1]) stop(9,row[1],279);
    row[5]=equal; row[6]=MEM16(row[2]); row[7]=MEM16(row[3]);
    g_eax=equal; g_esp+=16;
}
#endif
static void initialize_ansi_string(void)
{
    uint32_t *row = kernel_record(289, 2);
    row[1] = ff_init_ansi_string(&virtual_memory,row[2],row[3]);
    if (row[1]) stop(9,row[1],289);
    row[4] = MEM16(row[2]); row[5] = MEM16(row[2]+2); row[6] = MEM32(row[2]+4);
    g_esp += 12;
}
static void create_directory(void)
{
    uint32_t *row = kernel_record(190, 9);
    if (!volume_ready) stop(9, 0, 190);
#ifdef FF_REGULAR_FILE_PROOF
    row[1] = (row[10]&1u)?ff_volume_create_directory(&local_volume,row+2):ff_volume_create_file(&local_volume,row+2,timers.system_time);
#else
    row[1] = ff_volume_create_directory(&local_volume, row + 2);
#endif
    if (row[1] == FF_BOOT_UNSUPPORTED || row[1] == FF_VM_ACCESS_VIOLATION || row[1] == FF_BOOT_INVALID) stop(9, row[1], 190);
    if (!row[1]) { row[11] = MEM32(row[2]); row[12] = MEM32(row[5]); row[13] = MEM32(row[5] + 4); }
    row[14] = local_volume.directory_count; row[15] = local_volume.directories_created; row[16] = local_volume.active;
#ifdef FF_REGULAR_FILE_PROOF
    row[17]=local_volume.data_file_count; row[18]=local_volume.data_files_created;
    if(!row[1]) for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(local_volume.handles[i].handle==row[11]) row[19]=local_volume.handles[i].data_file;
    if(!row[1] && !(row[10]&1u)) {
        const uint32_t index=kernel_call_count-1,descriptor=MEM32(row[4]+4),buffer=MEM32(descriptor+4),bytes=MEM16(descriptor);
        if(bytes>=256) stop(9,bytes,190);
        uint32_t *captured=create_request_words[index];
        captured[0]=MEM32(row[4]); captured[1]=MEM32(row[4]+8); captured[2]=descriptor; captured[3]=buffer;
        captured[4]=bytes; captured[5]=MEM16(descriptor+2);
        for(unsigned i=0;i<bytes;++i) create_request_names[index][i]=MEM8(buffer+i);
    }
#endif
    g_eax = row[1]; g_esp += 40;
}
#ifdef FF_REGULAR_FILE_PROOF
static void query_file_information(void)
{
    uint32_t *row=kernel_record(211,5);
    row[1]=ff_volume_query_file(&local_volume,row+2);
    if(row[1]==FF_BOOT_UNSUPPORTED || row[1]==FF_VM_ACCESS_VIOLATION || row[1]==FF_BOOT_INVALID) stop(9,row[1],211);
    if(!row[1]) {
        row[7]=MEM32(row[3]); row[8]=MEM32(row[3]+4);
        for(unsigned i=0;i<14;++i) file_information[kernel_call_count-1][i]=MEM32(row[4]+i*4);
    }
    g_eax=row[1]; g_esp+=24;
}
#endif
#ifdef FF_FILE_WRITE_PROOF
static void write_regular_file(void)
{
    uint32_t *row=kernel_record(236,8);
    ff_volume_handle *handle=NULL;
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(local_volume.handles[i].handle==row[2]) handle=&local_volume.handles[i];
    if(handle) row[19]=handle->position;
    row[1]=ff_volume_write_file(&local_volume,row+2,timers.system_time);
    if(row[1]==FF_BOOT_UNSUPPORTED || row[1]==FF_VM_ACCESS_VIOLATION || row[1]==FF_BOOT_INVALID) stop(9,row[1],236);
    if(row[1]!=FF_BOOT_BAD_HANDLE) {row[10]=MEM32(row[6]); row[11]=MEM32(row[6]+4);}
    if(!row[1]) {
        if(!handle) stop(9,0,236);
        const ff_volume_data_file *file=NULL;
        for(unsigned i=0;i<FF_VOLUME_DATA_FILES;++i) if(local_volume.data_files[i].id==handle->data_file) file=&local_volume.data_files[i];
        if(!file) stop(9,handle->data_file,236);
        row[12]=file->id; row[13]=handle->position; row[14]=file->bytes;
        row[15]=(uint32_t)file->allocated_bytes; row[16]=(uint32_t)(file->allocated_bytes>>32);
        row[17]=(uint32_t)file->write_time; row[18]=(uint32_t)(file->write_time>>32);
    }
    g_eax=row[1]; g_esp+=36;
}
#endif
uint32_t ff_probe_directory_word(uint32_t index, uint32_t field)
{
    if (index >= FF_VOLUME_DIRECTORIES) return 0;
    const ff_volume_directory *directory = &local_volume.directories[index];
    return !field ? directory->id : field == 1 ? directory->parent : 0;
}
uint32_t ff_probe_directory_name(uint32_t index, uint32_t byte)
{
    return index < FF_VOLUME_DIRECTORIES && byte < FF_VOLUME_DIRECTORY_NAME ? (uint8_t)local_volume.directories[index].name[byte] : 0;
}
#endif
#ifdef FF_VOLUME_QUERY_PROOF
static void query_volume_size(void)
{
    uint32_t *row = kernel_record(218, 5);
    if (!volume_ready) stop(9, 0, 218);
    row[1] = ff_volume_query_size(&local_volume, row + 2);
    if (row[1] == FF_BOOT_UNSUPPORTED || row[1] == FF_VM_ACCESS_VIOLATION || row[1] == FF_BOOT_INVALID) stop(9, row[1], 218);
    if (!row[1]) {
        row[7] = MEM32(row[3]); row[8] = MEM32(row[3] + 4);
        for (unsigned i = 0; i < 6; ++i) row[9 + i] = MEM32(row[4] + i * 4);
    }
    g_eax = row[1]; g_esp += 24;
}
#endif
uint32_t ff_probe_volume_profile(uint32_t total_low, uint32_t total_high, uint32_t available_low,
                                 uint32_t available_high, uint32_t sectors, uint32_t bytes)
{
    if (!vm_ready || volume_ready) return FF_BOOT_INVALID;
    ff_volume_profile profile = {total_low | (uint64_t)total_high << 32,
        available_low | (uint64_t)available_high << 32, sectors, bytes};
    uint32_t status = ff_volume_init(&local_volume, &virtual_memory, &profile);
    if (!status) volume_ready = 1;
    return status;
}
uint32_t ff_probe_volume_word(uint32_t field)
{
    switch (field) {
        case 0: return volume_ready; case 1: return local_volume.active; case 2: return local_volume.opened; case 3: return local_volume.closed;
        case 4: return (uint32_t)local_volume.profile.total_units; case 5: return (uint32_t)(local_volume.profile.total_units >> 32);
        case 6: return (uint32_t)local_volume.profile.available_units; case 7: return (uint32_t)(local_volume.profile.available_units >> 32);
        case 8: return local_volume.profile.sectors_per_unit; case 9: return local_volume.profile.bytes_per_sector;
        case 10: return local_volume.directory_count; case 11: return local_volume.directories_created;
        case 12: return local_volume.file_count; case 13:return local_volume.data_file_count; case 14:return local_volume.data_files_created; default: return 0;
    }
}
uint32_t ff_probe_volume_handle_word(uint32_t index, uint32_t field)
{
    if (index >= FF_VOLUME_HANDLES) return 0;
    const ff_volume_handle *handle = &local_volume.handles[index];
    switch (field) { case 0: return handle->handle; case 1: return handle->access; case 2: return handle->share; case 3: return handle->options;
        case 4: return handle->directory; case 5:return handle->data_file; case 6:return handle->position; default: return 0; }
}
#endif
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
#ifdef FF_VM_PROOF
static void initialize_critical(void)
{
    uint32_t *row = kernel_record(291, 1);
    (void)ff_probe_guest_memory(row[2], 28, FF_VM_WRITE);
    row[1] = ff_initialize_critical(guest_ram, FF_GUEST_BYTES, row[2]);
    if (row[1]) stop(9, row[1], 291);
    g_esp += 8;
}
static void allocate_virtual_memory(void)
{
    uint32_t *row = kernel_record(184, 5);
    row[1] = ff_vm_allocate(&virtual_memory, row[2], row[3], row[4], row[5], row[6]);
    if (row[1] == FF_BOOT_UNSUPPORTED || row[1] == FF_VM_ACCESS_VIOLATION) stop(9, row[1], 184);
    if (!row[1]) { row[7] = MEM32(row[2]); row[8] = MEM32(row[4]); }
    g_eax = row[1]; g_esp += 24;
}
#endif
#ifdef FF_TIMER_PROOF
static void set_timer(void)
{
    uint32_t *row = kernel_record(149, 4);
    if (row[5]) {
        if ((row[5] & 3u) || row[5] > FF_GUEST_BYTES - 28u) stop(9, row[5], 149);
        uint32_t routine = MEM32(row[5] + 12);
        if (!recomp_lookup(routine)) stop(2, routine, 149);
    }
    sample_clock();
    uint64_t raw_due = row[3] | (uint64_t)row[4] << 32;
    int64_t due; memcpy(&due, &raw_due, sizeof due);
    uint32_t previous;
    row[1] = ff_timer_set(&timers, row[2], due, row[5], &previous);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 149);
    row[6] = previous; row[7] = clock_words[0]; row[8] = clock_words[1];
    row[9] = clock_words[2]; row[10] = clock_words[3];
    g_eax = previous; g_esp += 20;
}
#endif
recomp_func_t recomp_lookup_kernel(uint32_t address)
{
    uint32_t offset = address - FF_KERNEL_BASE;
    if (!(offset & 3u) && offset / 4u < FF_KERNEL_BINDING_COUNT) {
        uint32_t ordinal = bindings[offset / 4u].ordinal;
        if (ordinal == 255) return create_thread;
        if (ordinal == 187) return close_handle;
        if (ordinal == 277) return enter_critical;
        if (ordinal == 294) return leave_critical;
        if (ordinal == 107) return initialize_dpc;
        if (ordinal == 113) return initialize_timer;
        if (ordinal == 24) return query_nonvolatile;
        if (ordinal == 301) return status_to_dos_error;
#ifdef FF_VM_PROOF
        if (ordinal == 184) return allocate_virtual_memory;
        if (ordinal == 291) return initialize_critical;
#endif
#ifdef FF_TIMER_PROOF
        if (ordinal == 149) return set_timer;
#endif
#ifdef FF_VOLUME_PROOF
#ifdef FF_DIRECTORY_PROOF
        if (ordinal == 190) return create_directory;
        if (ordinal == 289) return initialize_ansi_string;
#ifdef FF_REGULAR_FILE_PROOF
        if (ordinal == 211) return query_file_information;
#endif
#ifdef FF_FILE_WRITE_PROOF
        if (ordinal == 236) return write_regular_file;
#endif
#ifdef FF_FILE_LINK_PROOF
        if (ordinal == 279) return equal_ansi_string;
#endif
#endif
        if (ordinal == 202) return open_volume_root;
#ifdef FF_VOLUME_QUERY_PROOF
        if (ordinal == 218) return query_volume_size;
#endif
#endif
#ifdef FF_NAMESPACE_PROOF
        if (ordinal == 67) return create_symbolic_link;
#endif
        stop(1, address, ordinal);
    }
    return 0;
}

uint32_t ff_probe_abi(void) {
#ifdef FF_FILE_WRITE_PROOF
    return 12;
#elif defined(FF_METADATA_FORMAT_PROOF)
    return 11;
#elif defined(FF_REGULAR_FILE_PROOF)
    return 10;
#elif defined(FF_FILE_LINK_PROOF)
    return 9;
#elif defined(FF_DIRECTORY_PROOF)
    return 8;
#elif defined(FF_NAMESPACE_PROOF)
    return 7;
#elif defined(FF_VOLUME_PROOF)
    return 6;
#elif defined(FF_VM_PROOF)
    return 5;
#elif defined(FF_TIMER_PROOF)
    return 4;
#else
    return 3;
#endif
}
void ff_probe_misc_flags(uint32_t value) { misc_flags = value; misc_flags_ready = 1; }
uint32_t ff_probe_ram(void) { return (uint32_t)(uintptr_t)guest_ram; }
uint32_t ff_probe_ram_bytes(void) { return FF_GUEST_BYTES; }
uint32_t ff_probe_stack_top(void) { return FF_STACK_TOP; }
uint32_t ff_probe_source_digest(void) { return (uint32_t)(uintptr_t)source_digest; }
uint32_t ff_probe_entry(void) { return FF_ENTRY_ADDRESS; }
uint32_t ff_probe_state(void) { return (uint32_t)(uintptr_t)probe_state; }
uint32_t ff_probe_trace(void) { return (uint32_t)(uintptr_t)probe_trace; }
uint32_t ff_probe_trace_count(void) { return probe_trace_count; }
uint32_t ff_probe_trace_capacity(void) { return FF_TRACE_CAPACITY; }
uint32_t ff_probe_trace_cpu(uint32_t index, uint32_t field) {
    return index < probe_trace_count && field < 3 ? probe_cpu_trace[index * 3 + field] : 0;
}
uint32_t ff_probe_binding_count(void) { return FF_KERNEL_BINDING_COUNT; }
uint32_t ff_probe_binding_slot(uint32_t i) { return i < FF_KERNEL_BINDING_COUNT ? bindings[i].slot : 0; }
uint32_t ff_probe_binding_ordinal(uint32_t i) { return i < FF_KERNEL_BINDING_COUNT ? bindings[i].ordinal : 0; }
uint32_t ff_probe_thread_count(void) { return boot.count; }
uint32_t ff_probe_parent_returned(void) { return parent_returned; }
uint32_t ff_probe_kernel_count(void) { return kernel_call_count; }
uint32_t ff_probe_kernel_capacity(void) { return FF_KERNEL_CAPACITY; }
uint32_t ff_probe_kernel_word(uint32_t i, uint32_t n) { return i < kernel_call_count && n < 20 ? kernel_calls[i][n] : 0; }
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
    memset(probe_cpu_trace, 0, sizeof probe_cpu_trace);
    memset(kernel_calls, 0, sizeof kernel_calls);
#ifdef FF_REGULAR_FILE_PROOF
    memset(file_information,0,sizeof file_information);
    memset(create_request_words,0,sizeof create_request_words); memset(create_request_names,0,sizeof create_request_names);
#endif
    probe_trace_count = kernel_call_count = parent_returned = 0;
    misc_flags = misc_flags_ready = 0;
#ifdef FF_FORMAT_PROOF
    memset(&format_observation,0,sizeof format_observation);
#endif
#ifdef FF_METADATA_FORMAT_PROOF
    memset(&metadata_observation,0,sizeof metadata_observation);
#endif
#ifdef FF_NAMESPACE_PROOF
    memset(&object_namespace,0,sizeof object_namespace); namespace_ready=0;
#endif
#ifdef FF_VOLUME_PROOF
    memset(&local_volume, 0, sizeof local_volume); volume_ready = 0;
#endif
#ifdef FF_VM_PROOF
    if (ff_vm_init(&virtual_memory, guest_ram, FF_GUEST_BYTES)) __builtin_trap();
    vm_ready = vm_access_count = 0; memset(vm_fault, 0, sizeof vm_fault);
#endif
#ifdef FF_TIMER_PROOF
    memset(&timers, 0, sizeof timers); memset(&dpc_frame, 0, sizeof dpc_frame);
    memset(clock_words, 0, sizeof clock_words); timers_ready = dpc_delivered = 0;
#endif
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
#ifdef FF_TIMER_PROOF
    if (ff_timers_init(&timers, guest_ram, FF_GUEST_BYTES, 0x03e00000u, thread->fs + 0x50u) != FF_BOOT_OK) stop(9, 0, 149);
    timers_ready = 1;
#endif
    recomp_func_t entry = recomp_lookup(thread->system);
    if (!entry) stop(2, thread->system, 0);
    entry(); stop(6, thread->system, 0);
}
/* The pinned emitter deliberately groups equality conditions twice. This
 * Clang style diagnostic does not change their meaning; retain all other
 * warnings/errors and scope the exception to generated title code only. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses-equality"
#include "title-functions.inc"
#pragma clang diagnostic pop
