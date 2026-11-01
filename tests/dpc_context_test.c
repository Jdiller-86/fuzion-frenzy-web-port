/* Owned CPU globals and callback; no vendor or original game code. */
#include "../src/runtime/dpc_context.h"
#include "../src/runtime/kernel_objects.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define FF_GLOBAL_U32(name) uint32_t g_##name;
FF_CPU_U32_FIELDS(FF_GLOBAL_U32)
#define FF_GLOBAL_INT(name) int g_##name;
FF_CPU_INT_FIELDS(FF_GLOBAL_INT)
#define FF_GLOBAL_U16(name) uint16_t g_##name;
FF_CPU_U16_FIELDS(FF_GLOBAL_U16)
double g_fp_stack[8];
#define FF_VECTOR(n) uint8_t g_xmm##n[16], g_mm##n[8];
FF_VECTOR(0) FF_VECTOR(1) FF_VECTOR(2) FF_VECTOR(3) FF_VECTOR(4) FF_VECTOR(5) FF_VECTOR(6) FF_VECTOR(7)
/* Exercise composition with the recompiler's register-alias interface. The
 * context type above must retain its own member names under these aliases. */
#define eax g_eax
#define ecx g_ecx
#define edx g_edx
#define esp g_esp
#define ebx g_ebx
#define esi g_esi
#define edi g_edi
#include "../src/runtime/recomp_cpu_snapshot.h"

static uint8_t ram[16384];
static ff_timer_queue queue;
static ff_dpc_frame frame;
static uint32_t word(unsigned a)
{
    return (uint32_t)ram[a] | (uint32_t)ram[a + 1] << 8 | (uint32_t)ram[a + 2] << 16 | (uint32_t)ram[a + 3] << 24;
}
static void put(unsigned a, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[a + i] = (uint8_t)(value >> (i * 8));
}
static ff_guest_cpu prepare(void)
{
    memset(ram, 0xa5, sizeof ram); memset(&frame, 0, sizeof frame);
    ff_guest_cpu cpu;
    for (unsigned i = 0; i < sizeof cpu; ++i) ((uint8_t *)&cpu)[i] = (uint8_t)(i * 37 + 5);
    cpu.r_fs_base = 128; cpu.r_esp = 1800; cpu.r_df = 1; cpu.r_eflags = 0x602;
    ff_restore_cpu(&cpu); ff_capture_cpu(&cpu);
    put(128, 1776); put(132, 2048); put(136, 1024);
    put(128 + 0x18, 128); put(128 + 0x1c, 128); put(128 + 0x20, 128 + 0x28);
    ram[128 + 0x24] = 1; put(128 + 0x28, 512); put(512 + 0x28, 1900);
    put(128 + 0x58, 0); put(1900, 1904);
    assert(ff_timers_init(&queue, ram, sizeof ram, 32, 128 + 0x50) == FF_BOOT_OK);
    assert(ff_timers_advance(&queue, 10, UINT64_C(0x123456789abc)) == FF_BOOT_OK);
    assert(ff_initialize_timer(ram, sizeof ram, 3000, 0) == FF_BOOT_OK);
    assert(ff_initialize_dpc(ram, sizeof ram, 4096, 0x1234, 0x87654321) == FF_BOOT_OK);
    uint32_t previous;
    assert(ff_timer_set(&queue, 3000, 0, 4096, &previous) == FF_BOOT_OK && !previous);
    return cpu;
}
static void callback(int bad_cleanup)
{
    assert(g_fs_base == 128 && ram[128 + 0x24] == 2 && word(128 + 0x58) == 1);
    assert(word(128) == UINT32_MAX && word(132) == 12288 && word(136) == 8192);
    assert(g_esp == 12268 && !g_df && !(g_eflags & 0x400) && !g_force_return);
    assert(word(g_esp) == FF_DPC_RETURN_SENTINEL && word(g_esp + 4) == 4096);
    assert(word(g_esp + 8) == 0x87654321 && word(g_esp + 12) == 0x56789abc && word(g_esp + 16) == 0x1234);
    assert(!ram[4096 + 2] && word(128 + 0x28) == 512 && word(512 + 0x28) == 1900 && word(1900) == 1904);
    ram[5000] = 0x7b; /* Legitimate guest side effect survives context restoration. */
    ff_guest_cpu clobbered;
    memset(&clobbered, 0x3c, sizeof clobbered); ff_restore_cpu(&clobbered);
    g_esp = bad_cleanup ? 12272 : 12288;
    put(128, 0x12345678); put(132, 4000); put(136, 3000); ram[128 + 0x24] = 7; put(128 + 0x58, 99);
}
static void complete_or_abort(int mode)
{
    ff_guest_cpu original = prepare(), cpu = original; ff_dpc_call call; uint32_t present;
    assert(ff_dpc_peek(&queue, 1, &call, &present) == FF_BOOT_OK && present);
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 8192, 12288, &call) == FF_BOOT_OK);
    assert(ff_dpc_pop(&queue, frame.irql, call.object) == FF_BOOT_OK);
    ff_restore_cpu(&cpu); callback(mode == 1); ff_capture_cpu(&cpu);
    uint32_t status = mode == 2 ? ff_dpc_abort(&frame, &cpu, ram, sizeof ram) : ff_dpc_end(&frame, &cpu, ram, sizeof ram);
    assert(status == (mode == 1 ? FF_BOOT_INVALID : FF_BOOT_OK));
    ff_restore_cpu(&cpu); ff_capture_cpu(&cpu);
    assert(!memcmp(&original, &cpu, sizeof cpu) && !frame.active && !queue.dpc_count);
    assert(word(128) == 1776 && word(132) == 2048 && word(136) == 1024 && ram[128 + 0x24] == 1 && word(128 + 0x58) == 0);
    assert(word(1900) == 1904 && ram[5000] == 0x7b && ram[8191] == 0xa5 && ram[12288] == 0xa5);
}
static void admission(void)
{
    ff_guest_cpu cpu = prepare(); ff_dpc_call call; uint32_t present;
    assert(ff_dpc_peek(&queue, 1, &call, &present) == FF_BOOT_OK && present);
    ram[128 + 0x24] = 2;
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 8192, 12288, &call) == FF_BOOT_UNSUPPORTED && queue.dpc_count == 1);
    ram[128 + 0x24] = 1; put(128 + 0x58, 1);
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 8192, 12288, &call) == FF_BOOT_UNSUPPORTED);
    put(128 + 0x58, 0);
    uint8_t before[sizeof ram]; memcpy(before, ram, sizeof ram); ff_guest_cpu saved = cpu;
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 1024, 2048, &call) == FF_BOOT_INVALID);
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 8193, 12288, &call) == FF_BOOT_INVALID);
    call.argument1++;
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 8192, 12288, &call) == FF_BOOT_INVALID);
    call.argument1--;
    frame.active = 1;
    assert(ff_dpc_begin(&frame, &cpu, ram, sizeof ram, 8192, 12288, &call) == FF_BOOT_INVALID);
    frame.active = 0;
    assert(!memcmp(before, ram, sizeof ram) && !memcmp(&saved, &cpu, sizeof cpu) && queue.dpc_count == 1);
}
int main(void)
{
    complete_or_abort(0); complete_or_abort(1); complete_or_abort(2); admission();
    puts("DPC bridge: four-argument frame, cleanup, all CPU/FP/SIMD globals, PCR/TLS/SEH/IRQL restore, fault abort, nested/high-IRQL rejection and retained guest effects passed");
    return 0;
}
