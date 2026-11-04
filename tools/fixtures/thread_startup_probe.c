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
#include "../../src/runtime/port_io.h"
#ifdef FF_PFIFO_PROOF
#include "../../src/runtime/pfifo_channel.h"
#endif
#ifdef FF_VM_PROOF
#include "../../src/runtime/virtual_memory.h"
#include "../../src/runtime/guest_mirror.h"
#include "../../src/runtime/guest_write_map.h"
#include "../../src/runtime/interrupt_vector.h"
#include "../../src/runtime/gpu_instance.h"
#include "../../src/runtime/time_fields.h"
#include "../../src/runtime/av_query.h"
#include "../../src/runtime/av_query_adapter.h"
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
#ifdef FF_DISK_PROOF
#if !defined(FF_INITIALIZER_BATCH_PROOF) || !defined(FF_FILE_WRITE_PROOF)
#error Disk startup retains the checked initializer proof
#endif
#include "../../src/runtime/local_disk.h"
#define FF_DISK_DATA_ADDRESS 0x03f10000u
static ff_local_disk local_disk;
static uint32_t disk_ready;
#endif
#if defined(FF_DISK_PARTITION_PROOF) && !defined(FF_DISK_PROOF)
#error Partition views retain the checked sparse-disk contract
#endif
#if defined(FF_DISK_GEOMETRY_PROOF) && !defined(FF_DISK_PARTITION_PROOF)
#error Disk control retains the explicit partition contract
#endif
#if defined(FF_DISK_INFORMATION_PROOF) && !defined(FF_DISK_GEOMETRY_PROOF)
#error Partition information retains explicit disk geometry
#endif
#if defined(FF_DISK_DISMOUNT_PROOF) && !defined(FF_DISK_INFORMATION_PROOF)
#error Dismount retains explicit partition information and attachment state
#endif
#ifdef FF_FATX_PROOF
#if !defined(FF_DISK_DISMOUNT_PROOF) || !defined(FF_NAMESPACE_PROOF)
#error FATX retains explicit disk, RAW lifetime and namespace contracts
#endif
#include "../../src/runtime/fatx_volume.h"
static ff_fatx_volume fatx_volume;
static uint32_t fatx_ready;
#endif
#ifdef FF_MEMORY_STATISTICS_PROOF
#if !defined(FF_FATX_PROOF) || !defined(FF_SECTION_PROOF)
#error Memory statistics retain the current checked startup and filesystem ABI
#endif
#include "../../src/runtime/memory_statistics.h"
static ff_memory_statistics memory_statistics;
static uint32_t memory_statistics_ready;
#endif
#ifdef FF_MMIO_PROOF
#if !defined(FF_MEMORY_STATISTICS_PROOF)
#error Device register storage retains the modeled memory statistics
#endif
#include "../../src/runtime/device_registers.h"
#include "../../src/runtime/pcrtc_state.h"
#include "../../src/runtime/pfifo_dma.h"
#include "../../src/runtime/pfifo_status.h"
static ff_device_registers device_registers;
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
#ifdef FF_INITIALIZER_BATCH_PROOF
#ifndef FF_STARTUP_EXPANSION_PROOF
#error Initializer batches retain the expanded checked startup contract
#endif
#define FF_TRACE_CAPACITY 8192u
#elif defined(FF_METADATA_TRACE_PROOF) || defined(FF_METADATA_FORMAT_PROOF)
#ifndef FF_REGULAR_FILE_PROOF
#error Expanded metadata traces retain the explicit fresh regular-file contract
#endif
#define FF_TRACE_CAPACITY 1024u
#else
#define FF_TRACE_CAPACITY 128u
#endif
#ifdef FF_STARTUP_EXPANSION_PROOF
#ifndef FF_NV_QUERY_PROOF
#error Expanded startup retains the checked explicit settings contract
#endif
#endif
#ifdef FF_INITIALIZER_BATCH_PROOF
#define FF_KERNEL_CAPACITY 512u
#elif defined(FF_STARTUP_EXPANSION_PROOF)
#define FF_KERNEL_CAPACITY 256u
#else
#define FF_KERNEL_CAPACITY 128u
#endif

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
static ff_port_io port_io;
static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal);
/* PCRTC interrupt state, enabled only when the profile supplies events. The
 * register contract and PMC aggregation come from the retained NV2A reference;
 * no timing source exists here, so nothing is raised unless the profile says so. */
static ff_pcrtc_state pcrtc;
static uint32_t pcrtc_ready, pcrtc_shadow[3], pcrtc_site;
static int32_t pcrtc_dirty = -1;
static uint32_t pcrtc_offset(int32_t index)
{
    return index == 0 ? 0xFD600100u : index == 1 ? 0xFD600140u : 0xFD000100u;
}
static void pcrtc_flush(void)
{
    if (pcrtc_dirty < 0) return;
    int32_t index = pcrtc_dirty; pcrtc_dirty = -1;
    if (index == 0) (void)ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_0, pcrtc_shadow[0]);
    else if (index == 1) (void)ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_EN_0, pcrtc_shadow[1]);
    else if (pcrtc_shadow[2] & FF_PCRTC_PMC_IRQ)
        (void)ff_pcrtc_write(&pcrtc, FF_PCRTC_INTR_0, FF_PCRTC_INTR_VBLANK);
    /* Keep the report's device traffic complete for the modelled display path.
     * A refused log entry is evidence loss, so it stops the proof instead of
     * being dropped: the run reports its device capacity boundary explicitly. */
    if (device_registers.ready &&
        ff_device_registers_log(&device_registers, pcrtc_offset(index),
                                FF_DEVICE_WRITE, pcrtc_shadow[index], pcrtc_site))
        stop(15, pcrtc_offset(index), 2);
}
static uint32_t *pcrtc_access(uint32_t address, uint32_t access, uint32_t site)
{
    int32_t index = address == 0xFD600100u ? 0 : address == 0xFD600140u ? 1 :
                    address == 0xFD000100u ? 2 : -1;
    if (index < 0 || !pcrtc_ready) return NULL;
    pcrtc_flush();
    pcrtc_shadow[0] = pcrtc.pending;
    pcrtc_shadow[1] = pcrtc.enabled;
    pcrtc_shadow[2] = ff_pcrtc_pmc_irq(&pcrtc);
    if (device_registers.ready &&
        ff_device_registers_log(&device_registers, pcrtc_offset(index),
                                FF_DEVICE_READ, pcrtc_shadow[index], site))
        stop(15, pcrtc_offset(index), 1);
    if (access & 2u) { pcrtc_dirty = index; pcrtc_site = site; }
    return &pcrtc_shadow[index];
}
/* The profile supplies a bounded number of vertical-blank events up front. */
uint32_t ff_probe_pcrtc(uint32_t vblanks)
{
    if (vblanks > 64u) return FF_BOOT_INVALID;
    ff_pcrtc_init(&pcrtc);
    for (uint32_t i = 0; i < vblanks; ++i) ff_pcrtc_vblank(&pcrtc);
    pcrtc_ready = 1;
    return 0;
}
/* Display-interrupt state for the report: 0 ready, 1 pending, 2 enabled, 3 events. */
uint32_t ff_probe_pcrtc_word(uint32_t field)
{
    if (!pcrtc_ready) return UINT32_MAX;
    return field == 0 ? 1u : field == 1 ? pcrtc.pending : field == 2 ? pcrtc.enabled :
           field == 3 ? pcrtc.vblanks : UINT32_MAX;
}

static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal);
void xbox_PortOut(uint32_t port, uint32_t value, int width)
{
    (void)ff_port_io_write(&port_io, port, value, (uint32_t)width);
}
uint32_t xbox_PortIn(uint32_t port, int width)
{
    uint32_t value = 0;
    if (ff_port_io_read(&port_io, port, (uint32_t)width, &value)) stop(14, port & 0xFFFFu, (uint32_t)width);
    return value;
}
/* Alignment-requiring vector move: a misaligned address faults on the original
 * CPU, so it stops the proof instead of being served as an unaligned copy. */
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site);
void *ff_probe_guest_vector(uint32_t address, uint32_t access, uint32_t site)
{
    if (address & 15u) stop(10, address, access);
    return ff_probe_guest_memory(address, 16u, access, site);
}
#ifdef FF_DISK_GEOMETRY_PROOF
static uint32_t disk_control_information[FF_KERNEL_CAPACITY][8];
#endif
static ff_boot_runtime boot;
static uint32_t misc_flags, misc_flags_ready;
#ifdef FF_NV_QUERY_PROOF
#ifndef FF_SECTION_PROOF
#error Expanded settings retain checked image and guest memory ownership
#endif
#include "../../src/runtime/console_settings.h"
static uint32_t parental_games,parental_games_ready;
#endif
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
static ff_guest_mirror guest_mirror;
static ff_guest_write_map guest_write_map;
static uint32_t vm_ready, vm_access_count, vm_fault[4];
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site);
#endif
#ifdef FF_SECTION_PROOF
#if !defined(FF_VM_PROOF) || !defined(FF_FILE_WRITE_PROOF)
#error Section loading requires checked memory and real file writes
#endif
#include "../../src/runtime/image_sections.h"
static uint8_t image_source[FF_IMAGE_SOURCE_BYTES] __attribute__((aligned(16)));
static ff_image_sections image_sections;
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
static uint32_t timers_ready, dpc_delivered, clock_words[4], timer_check;
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

#ifdef FF_MMIO_PROOF
static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal);
#endif
static void ff_probe_return_frame(uint32_t address, uint32_t frame)
{
#ifdef FF_COPY_PROOF
    if (copy_checking && address == copy_target) { copy_return_frame = frame; ++copy_return_seen; }
#else
    (void)address; (void)frame;
#endif
#ifdef FF_MMIO_PROOF
    /* Stop at the reached device helper's return, before its caller continues. */
    if (device_registers.ready && address == 0x001FE05Au) {
        (void)ff_device_registers_commit(&device_registers);
        stop(11, address, device_registers.event_count);
    }
#endif
}

static _Noreturn void stop(uint32_t status, uint32_t target, uint32_t ordinal)
{
#ifdef FF_MMIO_PROOF
    (void)ff_device_registers_commit(&device_registers);
#endif
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
#ifdef FF_PFIFO_PROOF
static ff_pfifo_channel pfifo_profile_channel;
static uint32_t pfifo_profile_ready;
uint32_t ff_probe_pfifo_profile(uint32_t base, uint32_t end, uint32_t put)
{
    if (ff_pfifo_channel_configure(&pfifo_profile_channel, base, end)) return FF_BOOT_INVALID;
    if (ff_pfifo_channel_set_get(&pfifo_profile_channel, 0)) return FF_BOOT_INVALID;
    if (ff_pfifo_channel_set_put(&pfifo_profile_channel, put)) return FF_BOOT_INVALID;
    if (ff_pfifo_channel_enable(&pfifo_profile_channel)) return FF_BOOT_INVALID;
    if (!ff_pfifo_channel_empty(&pfifo_profile_channel)) return FF_BOOT_INVALID;
    pfifo_profile_ready = 1;
    return 0;
}
#endif
#ifdef FF_PFIFO_EMPTY_RING_PROBE
static ff_pfifo_channel pfifo_ring;
static uint32_t pfifo_ring_word(uint32_t address)
{
    if (address > FF_GUEST_BYTES - 4u) return 0;
    return guest_ram[address] | (uint32_t)guest_ram[address + 1] << 8 |
           (uint32_t)guest_ram[address + 2] << 16 | (uint32_t)guest_ram[address + 3] << 24;
}
static void pfifo_ring_sync(void)
{
    uint32_t context = pfifo_ring_word(0x00206720u);
    if (!context || context > FF_GUEST_BYTES - 0x20u) return;
    uint32_t base = pfifo_ring_word(context + 0x10u);
    uint32_t end = pfifo_ring_word(context + 0x14u);
    uint32_t put = pfifo_ring_word(context + 0x18u);
    if (!pfifo_ring.ready && ff_pfifo_channel_configure(&pfifo_ring, base, end)) return;
    if (ff_pfifo_channel_set_get(&pfifo_ring, 0)) return;
    if (ff_pfifo_channel_set_put(&pfifo_ring, put)) return;
    (void)ff_pfifo_channel_enable(&pfifo_ring);
}
#endif
#ifdef FF_VM_PROOF
void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site)
{
    (void)site;
    /* The PCRTC path is served from its own explicit state when the profile
     * supplies display events; without that it falls through to the aperture and
     * ultimately to the fail-closed stop. */
    if (pcrtc_ready && bytes == 4u) {
        uint32_t *slot = pcrtc_access(address, access, site);
        if (slot) { ++vm_access_count; return slot; }
    }
#ifdef FF_MMIO_PROOF
    /* A staged device write is committed before any later access, so event
     * order and the stored value are both final. */
    if (device_registers.ready && ff_device_registers_commit(&device_registers))
        stop(10, address, access);
    if (device_registers.ready && !ff_device_registers_contains(&device_registers, address, (uint32_t)bytes)) {
        /* The reached PFIFO submission writes DMA_PUT and waits for the device
         * GET to catch up. Without a FIFO consumer model that match cannot
         * occur, so report the boundary instead of spinning to the poll limit.
         * The opt-in FF_ALLOW_DMA_GET_READ diagnostic serves the observed GET
         * instead, letting the original comparison decide; it is not the
         * default fail-closed contract. */
        int pfifo_served = 0;
#ifdef FF_PFIFO_PROOF
        if (pfifo_profile_ready && access == FF_VM_READ && ff_pfifo_dma_classify(address) == FF_PFIFO_DMA_GET) {
            uint32_t *put = ff_device_registers_slot(&device_registers, FF_PFIFO_USER_BASE + 0x40u);
            if (put && ff_pfifo_channel_should_serve_empty_get(&pfifo_profile_channel, *put)) {
                uint32_t *get = ff_device_registers_slot(&device_registers, address);
                if (get) { *get = ff_pfifo_channel_read_get(&pfifo_profile_channel); pfifo_served = 1; }
            }
        }
#endif
#ifdef FF_PFIFO_EMPTY_RING_PROBE
        if (access == FF_VM_READ && ff_pfifo_dma_classify(address) == FF_PFIFO_DMA_GET) {
            pfifo_ring_sync();
            if (pfifo_ring.enabled && ff_pfifo_channel_empty(&pfifo_ring)) {
                uint32_t *get = ff_device_registers_slot(&device_registers, address);
                if (get) { *get = ff_pfifo_channel_read_get(&pfifo_ring); pfifo_served = 1; }
            }
        }
#endif
#ifndef FF_ALLOW_DMA_GET_READ
        if (!pfifo_served && access == FF_VM_READ && ff_pfifo_dma_classify(address) == FF_PFIFO_DMA_GET) {
            uint32_t *put = ff_device_registers_slot(&device_registers, FF_PFIFO_USER_BASE + 0x40u);
            uint32_t *get = ff_device_registers_slot(&device_registers, address);
            uint32_t get_value = get ? *get : 0;
            if (put && get_value != *put) stop(13, address, access);
        }
#endif
        uint32_t device_access = access == FF_VM_READ ? FF_DEVICE_ACCESS_READ
                               : access == FF_VM_WRITE ? FF_DEVICE_ACCESS_WRITE
                               : access == (FF_VM_READ | FF_VM_WRITE) ? FF_DEVICE_ACCESS_READWRITE : 0;
        uint32_t *slot = device_access ? ff_device_registers_access_width(&device_registers, address, (uint32_t)bytes, device_access, site) : NULL;
        if (!slot) stop(ff_device_registers_poll_limited(&device_registers) ? 12 :
                        ff_device_registers_capacity_reached(&device_registers) ? 15 : 10, address, access);
        ++vm_access_count;
        return slot;
    }
#endif
#ifdef FF_VM_PROOF
    /* The reached cache-flush write targets the physical-mirror base. Only a
     * bounded data page is modeled; outside it the identity window still
     * decides, and execute access is never served from this storage. */
    if (access && (access & ~(FF_VM_READ | FF_VM_WRITE)) == 0 && bytes <= FF_GUEST_MIRROR_BYTES &&
        ff_guest_mirror_contains(&guest_mirror, address, (uint32_t)bytes)) {
        ++vm_access_count;
        uint32_t offset = ff_guest_mirror_offset(address);
        if ((access & FF_VM_WRITE) && bytes)
            (void)ff_guest_write_map_mark(&guest_write_map, offset, (uint32_t)bytes);
        return guest_ram + offset;
    }
#endif
    if (!vm_ready || ff_vm_check(&virtual_memory, address, bytes, access)) {
        vm_fault[0] = address; vm_fault[1] = (uint32_t)bytes; vm_fault[2] = (uint32_t)(bytes >> 32); vm_fault[3] = access;
        stop(10, address, access);
    }
    ++vm_access_count;
    if ((access & FF_VM_WRITE) && bytes) (void)ff_guest_write_map_mark(&guest_write_map, address, (uint32_t)bytes);
    /* Zero-count string operations access no guest address. Return a harmless
     * in-array pointer even if their unused register is outside this window. */
    return guest_ram + (bytes ? address : 0);
}
uint32_t ff_probe_vm_image(uint32_t base, uint32_t bytes)
{
    if (vm_ready || ff_vm_claim_fixed(&virtual_memory, base, bytes) ||
        ff_vm_claim_fixed(&virtual_memory, 0x03c00000u, 0x00400000u)) return FF_BOOT_INVALID;
#ifdef FF_DISK_PROOF
    if(ff_vm_map_fixed(&virtual_memory,0x03c00000u,FF_DISK_DATA_ADDRESS-0x03c00000u,4) ||
       ff_vm_map_fixed(&virtual_memory,FF_DISK_DATA_ADDRESS,FF_VM_PAGE,2) ||
       ff_vm_map_fixed(&virtual_memory,FF_DISK_DATA_ADDRESS+FF_VM_PAGE,FF_GUEST_BYTES-FF_DISK_DATA_ADDRESS-FF_VM_PAGE,4))return FF_BOOT_INVALID;
#else
    if(ff_vm_map_fixed(&virtual_memory,0x03c00000u,0x00400000u,4))return FF_BOOT_INVALID;
#endif
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
#ifdef FF_CONTEXT_PROBE
static uint32_t probe_word(uint32_t address)
{
    if (address > FF_GUEST_BYTES - 4u) return 0;
    return guest_ram[address] | (uint32_t)guest_ram[address + 1] << 8 |
           (uint32_t)guest_ram[address + 2] << 16 | (uint32_t)guest_ram[address + 3] << 24;
}
uint32_t ff_probe_context_word(uint32_t index)
{
    if (index >= 32u) return 0;
    if (index == 0u) return probe_word(0x00206720u);
    uint32_t base = probe_word(0x00206720u);
    if (!base || base > FF_GUEST_BYTES - 0x80u) return 0;
    return probe_word(base + (index - 1u) * 4u);
}
#endif
uint32_t ff_probe_write_map_size(void) { return FF_GUEST_WRITE_MAP_WORDS; }
uint32_t ff_probe_write_map_word(uint32_t index) { return ff_guest_write_map_word(&guest_write_map, index); }
uint32_t ff_probe_write_map_count(void) { return ff_guest_write_map_count(&guest_write_map); }
uint32_t ff_probe_vm_fault_word(uint32_t index) { return index < 4 ? vm_fault[index] : 0; }
uint32_t ff_probe_vm_access_probe(uint32_t address, uint32_t access)
{
    if (access == FF_VM_READ) return FF_MEMORY_U8(address, FF_VM_READ);
    if (access == FF_VM_WRITE) { FF_MEMORY_U8(address, FF_VM_WRITE) = 0xb3; return 0; }
    stop(9, address, access);
}
#endif
#ifdef FF_SECTION_PROOF
uint32_t ff_probe_image_source(void) { return (uint32_t)(uintptr_t)image_source; }
uint32_t ff_probe_image_source_capacity(void) { return sizeof image_source; }
uint32_t ff_probe_image_init(uint32_t bytes)
{
    return vm_ready?ff_image_init(&image_sections,&virtual_memory,image_source,bytes):FF_BOOT_INVALID;
}
uint32_t ff_probe_image_count(void) { return image_sections.ready?image_sections.count:0; }
uint32_t ff_probe_image_word(uint32_t index,uint32_t field)
{
    if(!image_sections.ready || index>=image_sections.count)return 0;
    const ff_image_section *s=&image_sections.sections[index];
    switch(field) {
        case 0:return s->descriptor;case 1:return s->flags;case 2:return s->address;case 3:return s->bytes;
        case 4:return s->offset;case 5:return s->file_bytes;case 6:return s->references;
        case 7:return image_sections.shared[s->head].references;case 8:return image_sections.shared[s->tail].references;
        default:return 0;
    }
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
    (void)ff_probe_guest_memory(address, 1, FF_VM_EXECUTE, 0u);
#endif
#ifdef FF_TIMER_PROOF
    ff_probe_poll();
#endif
    trace(1, address);
}
void recomp_trace_exit(const char *name, uint32_t address) {
    (void)name;
#ifdef FF_VM_PROOF
    (void)ff_probe_guest_memory(g_esp, 4, FF_VM_READ, 0u);
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
    if (status != FF_BOOT_OK) { timer_check = ff_timers_check(&timers); stop(9, status, 149); }
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
    (void)ff_probe_guest_memory(g_esp, 4u * (argc + 1u), FF_VM_READ, 0u);
#endif
#ifdef FF_TIMER_PROOF
    ff_probe_poll();
#endif
    if (kernel_call_count == FF_KERNEL_CAPACITY || (g_esp & 3u) || g_esp > FF_GUEST_BYTES - 4u * (argc + 1u)) stop(4, 0, ordinal);
    uint32_t *row = kernel_calls[kernel_call_count++]; row[0] = ordinal;
    for (unsigned i = 0; i < argc; ++i) row[i + 2] = MEM32(g_esp + 4 + i * 4);
    return row;
}
#ifdef FF_MEMORY_STATISTICS_PROOF
uint32_t ff_probe_memory_profile(uint32_t physical_pages,uint32_t baseline_pool_pages)
{
    /* The modeled physical capacity must equal the bounded guest RAM. A smaller
     * explicit value would understate the pages this runtime actually owns. */
    if(!vm_ready || !image_sections.ready || memory_statistics_ready ||
       physical_pages!=FF_GUEST_BYTES/FF_VM_PAGE)return FF_BOOT_INVALID;
    const ff_memory_profile profile={physical_pages,baseline_pool_pages};
    uint32_t status=ff_memory_statistics_init(&memory_statistics,&virtual_memory,&profile);
    if(!status) {
        const ff_vm_region *image=&virtual_memory.regions[image_sections.owner-1];
        status=ff_memory_statistics_classify(&memory_statistics,image->base,image->bytes,FF_MEMORY_IMAGE);
    }
    if(!status)status=ff_memory_statistics_classify(&memory_statistics,0x03c00000u,0x00400000u,FF_MEMORY_POOL);
    if(!status)memory_statistics_ready=1;
    return status;
}
#ifdef FF_MMIO_PROOF
static uint32_t mmio_write_clearing, mmio_pmc_zero, mmio_engine_wait, mmio_pfifo_reset, mmio_pfifo_reset_count;
static ff_pfifo_status mmio_pfifo_status;
static uint32_t av_query_word, av_query_ready;
/* Explicit, default-off synthetic raw DWORD for the observed ordinal-2 query.
 * The worker must call this before the run; without it the service stays
 * unsupported. No composer, default or console inference exists. */
uint32_t ff_probe_av_query_word(uint32_t word)
{
    if (av_query_ready) return FF_BOOT_INVALID;
    av_query_word = word;
    av_query_ready = 1;
    return 0;
}
/* Explicitly experimental: the selected profile must opt in to modelling an
 * acknowledging store, and the same profile without it is the control run. */
uint32_t ff_probe_mmio_write_clearing(uint32_t enable)
{
    if(enable > 1u || device_registers.ready) return FF_BOOT_INVALID;
    mmio_write_clearing = enable;
    return 0;
}
/* Explicitly experimental: serve NV_PMC_INTR_0 as a zero status register. The
 * reached code never acknowledges this register (its only store is zero), so
 * this models "no device event has been delivered" for diagnosis; it is not
 * evidence of interrupt delivery or a completion rule. Without it the read stays
 * fail-closed. */
uint32_t ff_probe_mmio_pmc_zero(uint32_t enable)
{
    if(enable > 1u || device_registers.ready) return FF_BOOT_INVALID;
    mmio_pmc_zero = enable;
    return 0;
}
/* Explicitly experimental: declare the three PFIFO status registers the reached
 * loop tests as status inputs, so the report names the predicate that blocks the
 * exit instead of ending at the bounded event log. Nothing is raised here. */
uint32_t ff_probe_mmio_engine_wait(uint32_t enable)
{
    if(enable > 1u || device_registers.ready) return FF_BOOT_INVALID;
    mmio_engine_wait = enable;
    return 0;
}
/* Explicitly experimental and default-off: apply the pinned reference reset
 * value (LOW_MARK on both PFIFO status registers) through the existing seam
 * exactly once at initialization. No consumer update is made, no read or poll
 * raises anything, and the pushbuffer pointers are untouched. */
uint32_t ff_probe_mmio_pfifo_reset(uint32_t enable)
{
    if(enable > 1u || device_registers.ready) return FF_BOOT_INVALID;
    mmio_pfifo_reset = enable;
    return 0;
}
/* Report the seam state after the reset adapter ran. Field 0 is the ready flag
 * for the experiment; the others are the two status values, DMA_PUSH, the
 * consumer-update counter and the number of reset applications. */
uint32_t ff_probe_pfifo_status_word(uint32_t field)
{
    switch(field) {
        case 0: return mmio_pfifo_reset ? 1u : 0u;
        case 1: return mmio_pfifo_status.cache1_status;
        case 2: return mmio_pfifo_status.runout_status;
        case 3: return mmio_pfifo_status.dma_push;
        case 4: return mmio_pfifo_status.consumer_updates;
        case 5: return mmio_pfifo_reset_count;
        default: return 0;
    }
}
uint32_t ff_probe_mmio_profile(uint32_t base,uint32_t bytes,uint32_t first_value,uint32_t second_value)
{
    if(!vm_ready || device_registers.ready)return FF_BOOT_INVALID;
    uint32_t status = ff_device_registers_init(&device_registers,base,bytes,0xFD001804u,first_value,0xFD600140u,second_value);
    if(status)return status;
    /* NV_PFB_WBC at NV2A offset 0x410: the reached wait sets NV_PFB_WBC_FLUSH
     * (1 << 16) and polls until it clears. xemu's PFB model reads 0 because a
     * flush is not pending, so model only that self-clearing bit here. */
    status = ff_device_registers_add_self_clearing(&device_registers,0xFD100410u,0x10000u);
    if(status)return status;
    /* xemu user.c mirrors USER DMA_PUT/GET/REF into the PFIFO CACHE1 copies. */
    if((status=ff_device_registers_add_alias(&device_registers,0xFD800040u,0xFD003240u)))return status;
    if((status=ff_device_registers_add_alias(&device_registers,0xFD800044u,0xFD003244u)))return status;
    if((status=ff_device_registers_add_alias(&device_registers,0xFD800048u,0xFD003248u)))return status;
    /* Reference reset adapter: one-time initialization through the existing
     * PFIFO status seam. It sets LOW_MARK on both status registers without a
     * consumer update and leaves DMA_PUSH and the pushbuffer untouched. */
    if(mmio_pfifo_reset) {
        ff_pfifo_status_init(&mmio_pfifo_status);
        ff_pfifo_status_reference_reset(&mmio_pfifo_status);
        ++mmio_pfifo_reset_count;
        if((status=ff_device_registers_add_initial(&device_registers,0xFD003214u,
              ff_pfifo_status_read(&mmio_pfifo_status,FF_PFIFO_CACHE1_STATUS_OFFSET))) ||
           (status=ff_device_registers_add_initial(&device_registers,0xFD002400u,
              ff_pfifo_status_read(&mmio_pfifo_status,FF_PFIFO_RUNOUT_STATUS_OFFSET)))) return status;
    }
    /* Experimental PMC zero status: the reached wait at 0x1FE50D polls bit 24 of
     * NV_PMC_INTR_0, and the only store the original code makes to it is zero
     * (0x1FDFC3), so this register is a status input rather than an acknowledge
     * register. Serving it as zero is a diagnostic for "no device event has been
     * delivered"; it is not evidence of interrupt delivery. Without this flag the
     * read stays fail-closed. */
    if(mmio_pmc_zero && (status=ff_device_registers_add_zero_range(&device_registers,0xFD000100u,4u)))return status;
    /* The reached wait at 0x1FE50D polls bit 24 of NV_PMC_INTR_0. Declaring it as
     * a status input means the polling loop is reported as a device wait instead
     * of silently consuming the bounded event log until it fills. */
    if(mmio_pmc_zero && (status=ff_device_registers_add_status_wait(&device_registers,0xFD000100u,0x01000000u)))return status;
    /* The reached loop's exit also needs PFIFO engine status: CACHE1_STATUS and
     * RUNOUT_STATUS bit 0x10 set, CACHE1_DMA_PUSH bit 0x10 clear. Declaring them
     * names the blocking predicate; no bit is raised. */
    if(mmio_engine_wait &&
       ((status=ff_device_registers_add_status_wait(&device_registers,0xFD003214u,0x10u)) ||
        (status=ff_device_registers_add_status_wait(&device_registers,0xFD002400u,0x10u)) ||
        (status=ff_device_registers_add_status_wait(&device_registers,0xFD003220u,0x10u)))) return status;
    if(!mmio_write_clearing) return 0;
    /* NV_PGRAPH_INTR at NV2A offset 0x100. The reached code reads the status,
     * reads NSOURCE (0x108) and TRAPPED_ADDR (0x704), stores exactly the bit it
     * observed back to 0x100 and then re-reads it from another routine that
     * re-enters the handler while the value is nonzero. Storing the written
     * value therefore latches a phantom interrupt that only that store could
     * have raised, and the drain loop cannot terminate. Acknowledge the stored
     * bits instead: the guest's store clears what it carries, so with no pending
     * status in this synthetic profile the register reads 0 and the loop exits.
     * The predicted hardware clear is not independently verified here; the local
     * xemu-derived PGRAPH reference stores writes as a stub, so it cannot confirm
     * it. Sub-dword stores to this register are rejected rather than half-applied. */
    if((status=ff_device_registers_add_write_clearing(&device_registers,0xFD400100u)))return status;
    /* NV_PFIFO_INTR_0 at NV2A offset 0x2100 shows the same guest shape: 0x1FDC30
     * reads it, stores exactly the observed bits 0x100000 and 0x1000000 back,
     * re-reads it and acts on the remaining low byte, and its caller repeats the
     * whole block while the phantom bits stay set. Same acknowledge rule, same
     * limits: only this declared register and only 4-byte stores. */
    return ff_device_registers_add_write_clearing(&device_registers,0xFD002100u);
}
uint32_t ff_probe_mmio_add(uint32_t address,uint32_t value)
{
    return ff_device_registers_add_initial(&device_registers,address,value);
}
uint32_t ff_probe_mmio_zero(uint32_t base,uint32_t bytes)
{
    return ff_device_registers_add_zero_range(&device_registers,base,bytes);
}
uint32_t ff_probe_mmio_alias_count(void) { return device_registers.alias_count; }uint32_t ff_probe_mmio_alias_word(uint32_t index, uint32_t field)
{
    if(index >= device_registers.alias_count)return UINT32_MAX;
    return field ? device_registers.aliases[index].second : device_registers.aliases[index].first;
}
/* Declared status inputs the guest polls: address, mask, last masked value and
 * the bounded read count, so the report shows which interrupt bit is waited on. */
uint32_t ff_probe_device_status_wait_count(void) { return device_registers.status_wait_count; }
uint32_t ff_probe_device_status_wait_word(uint32_t index, uint32_t field)
{
    if(ff_device_registers_commit(&device_registers))return UINT32_MAX;
    if(index >= device_registers.status_wait_count)return UINT32_MAX;
    const ff_device_status_wait *wait = &device_registers.status_waits[index];
    return field==0?wait->address:field==1?wait->mask:field==2?wait->value:
           field==3?wait->count:UINT32_MAX;
}
uint32_t ff_probe_mmio_word(uint32_t field)
{
    if(ff_device_registers_commit(&device_registers))return UINT32_MAX;
    if(field==0)return device_registers.ready;
    if(field==1)return device_registers.base;
    if(field==2)return device_registers.bytes;
    if(field==3)return device_registers.count;
    if(field==4)return device_registers.event_count;
    if(field>=5 && field<5+FF_DEVICE_ENTRIES*4){
        uint32_t e=(field-5)/4u,k=(field-5)%4u;
        if(e>=device_registers.count)return UINT32_MAX;
        return k==0?device_registers.entries[e].address:k==1?device_registers.entries[e].value:
               k==2?device_registers.entries[e].reads:device_registers.entries[e].writes;
    }
    return UINT32_MAX;
}
uint32_t ff_probe_mmio_event(uint32_t index,uint32_t field)
{
    if(ff_device_registers_commit(&device_registers))return UINT32_MAX;
    if(index>=device_registers.event_count)return UINT32_MAX;
    const ff_device_event *e=&device_registers.events[index];
    return field==0?e->address:field==1?e->direction:field==2?e->value:field==3?e->before_known:field==4?e->site:UINT32_MAX;
}
#endif
uint32_t ff_probe_memory_word(uint32_t field)
{    if(field==0)return memory_statistics_ready;
    if(field==1)return memory_statistics.profile.physical_pages;
    if(field==2)return memory_statistics.profile.baseline_pool_pages;
    uint32_t words[FF_MEMORY_STATISTICS_WORDS];
    uint32_t status=memory_statistics_ready?ff_memory_statistics_snapshot(&memory_statistics,words):FF_BOOT_INVALID;
    if(field==3)return status;
    return !status && field>=4 && field<13?words[field-4]:UINT32_MAX;
}
static void query_memory_statistics(void)
{
    uint32_t *row=kernel_record(181,1);row[1]=FF_BOOT_UNSUPPORTED;
    if(!memory_statistics_ready)stop(9,row[1],181);
    row[1]=ff_memory_statistics_query(&memory_statistics,row[2]);
    if(row[1])stop(9,row[1],181);
    for(unsigned i=0;i<FF_MEMORY_STATISTICS_WORDS;++i)row[3+i]=MEM32(row[2]+i*4);
    g_eax=0;g_esp+=8;
}
#ifdef FF_VM_PROOF
static void hal_get_interrupt_vector(void)
{
    uint32_t *row=kernel_record(44,2),vector=0;
    row[1]=ff_hal_get_interrupt_vector(&virtual_memory,guest_ram,row[2],row[3],&vector);
    if(row[1])stop(9,row[1],44);
    row[4]=vector;g_eax=vector;g_esp+=12;
}
static void ke_initialize_interrupt(void)
{
    uint32_t *row=kernel_record(109,7);
    row[1]=ff_ke_initialize_interrupt(&virtual_memory,guest_ram,row[2],row[3],row[4],row[5]);
    if(row[1])stop(9,row[1],109);
    row[9]=row[6];row[10]=row[7];row[11]=row[8];
    g_eax=0;g_esp+=32;
}
static void ke_connect_interrupt(void)
{
    uint32_t *row=kernel_record(98,1);
    row[1]=ff_ke_connect_interrupt(&virtual_memory,guest_ram,row[2]);
    if(row[1]==FF_BOOT_INVALID || row[1]==FF_VM_ACCESS_VIOLATION)stop(9,row[1],98);
    g_eax=row[1];g_esp+=8;
}
static void hal_register_shutdown_notification(void)
{
    uint32_t *row=kernel_record(47,2);
    row[1]=ff_hal_register_shutdown_notification(&virtual_memory,guest_ram,row[2],row[3]);
    if(row[1])stop(9,row[1],47);
    g_eax=0;g_esp+=12;
}
#endif
#endif
#ifdef FF_FATX_PROOF
static int fatx_parent_handle(uint32_t attributes)
{
    return attributes && !(attributes&3u) && !ff_vm_check(&virtual_memory,attributes,12,FF_VM_READ) &&
        ff_fatx_owns_handle(MEM32(attributes));
}
#endif
#if defined(FF_TIMER_PROOF) && defined(FF_VM_PROOF)
static void query_system_time(void)
{
    uint32_t *row=kernel_record(128,1);
    row[1]=FF_BOOT_UNSUPPORTED;
    if(!timers_ready)stop(9,row[1],128);
    row[1]=(!row[2] || (row[2]&3u))?FF_BOOT_INVALID:ff_vm_check(&virtual_memory,row[2],8,FF_VM_WRITE);
    if(row[1])stop(9,row[1],128);
    const uint64_t ticks=timers.system_time;
    MEM32(row[2])=(uint32_t)ticks;MEM32(row[2]+4)=(uint32_t)(ticks>>32);
    row[1]=0;row[3]=(uint32_t)ticks;row[4]=(uint32_t)(ticks>>32);
    g_esp+=8; /* VOID NTAPI: preserve registers; pop return address and pointer. */
}
#endif
#ifdef FF_SECTION_PROOF
static void section_operation(uint32_t ordinal)
{
    uint32_t *row=kernel_record(ordinal,1),index=FF_IMAGE_SECTIONS;
    for(uint32_t i=0;i<image_sections.count;++i)if(image_sections.sections[i].descriptor==row[2])index=i;
    if(index<image_sections.count)row[3]=image_sections.sections[index].references;
    row[1]=ordinal==327?ff_image_load(&image_sections,row[2]):ff_image_unload(&image_sections,row[2]);
    if(row[1]==FF_BOOT_UNSUPPORTED)stop(9,row[1],ordinal);
    if(!row[1] && index<image_sections.count) {
        const ff_image_section *s=&image_sections.sections[index];
        row[4]=s->references;row[5]=image_sections.shared[s->head].references;row[6]=image_sections.shared[s->tail].references;
        row[7]=s->address;row[8]=s->bytes;row[9]=s->file_bytes;row[10]=s->offset;
    }
    g_eax=row[1];g_esp+=8;
}
static void load_image_section(void) { section_operation(327); }
static void unload_image_section(void) { section_operation(328); }
#endif
static void create_thread(void)
{
    uint32_t *row = kernel_record(255, 10);
    if (!recomp_lookup(row[7]) || !recomp_lookup(row[11])) stop(9, row[11], 255);
    row[1] = ff_boot_create(&boot, row + 2);
    if (row[1] != FF_BOOT_OK) stop(9, row[1], 255);
#ifdef FF_MEMORY_STATISTICS_PROOF
    const ff_boot_thread *created=&boot.threads[boot.count-1];
    if(!memory_statistics_ready || ff_memory_statistics_classify(&memory_statistics,created->stack_low,
        created->stack_top-created->stack_low,FF_MEMORY_STACK))stop(9,FF_BOOT_INVALID,255);
#endif
    g_eax = row[1]; g_esp += 44; /* NTAPI return address and ten arguments. */
}
static void close_handle(void)
{
    uint32_t *row = kernel_record(187, 1);
#ifdef FF_FATX_PROOF
    if (ff_fatx_owns_handle(row[2])) row[1] = ff_fatx_close(&fatx_volume, row[2]);
    else
#endif
#ifdef FF_DISK_PROOF
    if (ff_disk_owns_handle(row[2])) row[1] = ff_disk_close(&local_disk, row[2]);
    else
#endif
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
#ifdef FF_FATX_PROOF
    if(!status)status=disk_ready?ff_fatx_init(&fatx_volume,&local_disk,&object_namespace):FF_BOOT_INVALID;
    if(!status)fatx_ready=1;
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
    row[1] = FF_BOOT_UNSUPPORTED;
#ifdef FF_DISK_PROOF
    if (disk_ready) row[1] = ff_disk_open(&local_disk, row + 2);
#endif
#ifdef FF_FATX_PROOF
    if(row[1]==FF_BOOT_UNSUPPORTED && fatx_ready)row[1]=ff_fatx_open_root(&fatx_volume,row+2);
    if(row[1]==FF_BOOT_UNSUPPORTED && fatx_parent_handle(row[4]))stop(9,row[1],202);
#endif
    if (row[1] == FF_BOOT_UNSUPPORTED) row[1] = ff_volume_open_root(&local_volume, row + 2);
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
#ifdef FF_FATX_PROOF
    if(fatx_parent_handle(row[4])){row[1]=FF_BOOT_UNSUPPORTED;stop(9,row[1],190);}
#endif
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
#ifdef FF_FATX_PROOF
    if(ff_fatx_owns_handle(row[2])){row[1]=FF_BOOT_UNSUPPORTED;stop(9,row[1],211);}
#endif
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
#ifdef FF_FATX_PROOF
    if(ff_fatx_owns_handle(row[2])){row[1]=FF_BOOT_UNSUPPORTED;stop(9,row[1],236);}
#endif
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
#ifdef FF_DISK_PROOF
static void transfer_disk(int writing)
{
    const uint32_t ordinal=writing?236:219;
    uint32_t *row=kernel_record(ordinal,8);
    row[1]=FF_BOOT_UNSUPPORTED;
    if(!disk_ready || !ff_disk_owns_handle(row[2]))stop(9,row[1],ordinal);
    row[1]=writing?ff_disk_write(&local_disk,row+2):ff_disk_read(&local_disk,row+2);
    if(row[1]==FF_BOOT_UNSUPPORTED || row[1]==FF_VM_ACCESS_VIOLATION || row[1]==FF_BOOT_INVALID)stop(9,row[1],ordinal);
    if(row[1]!=FF_BOOT_BAD_HANDLE){row[10]=MEM32(row[6]);row[11]=MEM32(row[6]+4);}
    if(!row[1] && row[11])for(unsigned i=0;i<FF_DISK_HANDLES;++i)if(local_disk.handles[i].handle==row[2]) {
        const ff_disk_handle *file=&local_disk.handles[i];
        const uint64_t start=file->position-row[11],physical=file->offset+start;
        row[12]=file->partition;row[13]=(uint32_t)file->position;row[14]=(uint32_t)(file->position>>32);
        row[15]=(uint32_t)start;row[16]=(uint32_t)(start>>32);row[17]=(uint32_t)physical;row[18]=(uint32_t)(physical>>32);
    }
    g_eax=row[1];g_esp+=36;
}
static void read_disk(void){transfer_disk(0);}
static void write_file(void)
{
    if(ff_disk_owns_handle(MEM32(g_esp+4)))transfer_disk(1);
    else write_regular_file();
}
uint32_t ff_probe_disk_profile(uint32_t low,uint32_t high,uint32_t sector_bytes,uint32_t cache_partitions)
{
    if(!vm_ready || disk_ready)return FF_BOOT_INVALID;
    ff_disk_profile profile={low|((uint64_t)high<<32),sector_bytes,cache_partitions};
    uint32_t status=ff_disk_init(&local_disk,&virtual_memory,&profile);
    if(!status){disk_ready=1;MEM32(FF_DISK_DATA_ADDRESS)=cache_partitions;}
    return status;
}
uint32_t ff_probe_disk_word(uint32_t field)
{
    switch(field){
    case 0:return disk_ready;case 1:return local_disk.active;case 2:return local_disk.opened;case 3:return local_disk.closed;
    case 4:return local_disk.allocated_pages;case 5:return (uint32_t)local_disk.profile.bytes;
    case 6:return (uint32_t)(local_disk.profile.bytes>>32);case 7:return local_disk.profile.sector_bytes;
    case 8:return local_disk.profile.cache_partitions;
#ifdef FF_DISK_PARTITION_PROOF
    case 9:return local_disk.partition_count;
#endif
#ifdef FF_DISK_GEOMETRY_PROOF
    case 10:return local_disk.geometry.ready;case 11:return (uint32_t)local_disk.geometry.cylinders;
    case 12:return (uint32_t)(local_disk.geometry.cylinders>>32);case 13:return local_disk.geometry.tracks_per_cylinder;
    case 14:return local_disk.geometry.sectors_per_track;
#endif
#ifdef FF_DISK_DISMOUNT_PROOF
    case 15:return local_disk.volume_count;
#endif
    default:return UINT32_MAX;
    }
}
uint32_t ff_probe_disk_byte(uint32_t low,uint32_t high){return ff_disk_byte(&local_disk,low|((uint64_t)high<<32));}
uint32_t ff_probe_disk_transfer_details(void){return 1;}
#ifdef FF_DISK_PARTITION_PROOF
uint32_t ff_probe_disk_partition_profile(uint32_t number,uint32_t offset_low,uint32_t offset_high,uint32_t bytes_low,uint32_t bytes_high)
{
    return disk_ready?ff_disk_add_partition(&local_disk,number,offset_low|((uint64_t)offset_high<<32),bytes_low|((uint64_t)bytes_high<<32)):FF_BOOT_INVALID;
}
uint32_t ff_probe_disk_partition_word(uint32_t index,uint32_t field)
{
    if(index>=local_disk.partition_count)return UINT32_MAX;
    const ff_disk_partition *part=&local_disk.partitions[index];
    switch(field){case 0:return part->number;case 1:return (uint32_t)part->offset;case 2:return (uint32_t)(part->offset>>32);
        case 3:return (uint32_t)part->bytes;case 4:return (uint32_t)(part->bytes>>32);default:return UINT32_MAX;}
}
#endif
#ifdef FF_DISK_GEOMETRY_PROOF
uint32_t ff_probe_disk_geometry_profile(uint32_t low,uint32_t high,uint32_t tracks,uint32_t sectors)
{
    return disk_ready?ff_disk_set_geometry(&local_disk,low|((uint64_t)high<<32),tracks,sectors):FF_BOOT_INVALID;
}
static void control_disk(void)
{
    uint32_t *row=kernel_record(196,10);
    row[1]=FF_BOOT_UNSUPPORTED;
#ifdef FF_FATX_PROOF
    if(ff_fatx_owns_handle(row[2]))stop(9,row[1],196);
#endif
    if(!disk_ready)stop(9,row[1],196);
    row[1]=ff_disk_control(&local_disk,row+2);
    if(row[1]==FF_BOOT_UNSUPPORTED || row[1]==FF_BOOT_INVALID || row[1]==FF_VM_ACCESS_VIOLATION)stop(9,row[1],196);
    if(row[1]!=FF_BOOT_BAD_HANDLE){row[12]=MEM32(row[6]);row[13]=MEM32(row[6]+4);}
    if(!row[1]) {
        if(row[13]>32 || (row[13]&3u))stop(9,row[13],196);
        for(unsigned i=0;i<row[13]/4;++i)disk_control_information[kernel_call_count-1][i]=MEM32(row[10]+i*4);
    }
    g_eax=row[1];g_esp+=44;
}
uint32_t ff_probe_disk_control_word(uint32_t index,uint32_t word)
{
    return index<kernel_call_count && word<8?disk_control_information[index][word]:0;
}
#ifdef FF_DISK_INFORMATION_PROOF
uint32_t ff_probe_disk_partition_information(uint32_t number,uint32_t hidden,uint32_t type,uint32_t flags)
{
    return disk_ready?ff_disk_set_partition_information(&local_disk,number,hidden,type,flags):FF_BOOT_INVALID;
}
uint32_t ff_probe_disk_information_word(uint32_t index,uint32_t field)
{
    if(index>=local_disk.partition_count)return UINT32_MAX;
    const ff_disk_partition *part=&local_disk.partitions[index];
    switch(field){case 0:return part->information_ready;case 1:return part->hidden_sectors;case 2:return part->type;
        case 3:return part->flags;default:return UINT32_MAX;}
}
#ifdef FF_DISK_DISMOUNT_PROOF
uint32_t ff_probe_disk_raw_volume(uint32_t partition)
{
    return disk_ready?ff_disk_attach_raw_volume(&local_disk,partition):FF_BOOT_INVALID;
}
uint32_t ff_probe_disk_volume_word(uint32_t index,uint32_t field)
{
    if(index>=local_disk.volume_count)return UINT32_MAX;
    const ff_disk_volume *volume=&local_disk.volumes[index];
    switch(field){case 0:return volume->partition;case 1:return volume->state;case 2:return volume->references;
        case 3:return volume->dismounts;default:return UINT32_MAX;}
}
static void filesystem_control_disk(void)
{
    uint32_t *row=kernel_record(200,10);
    row[1]=FF_BOOT_UNSUPPORTED;
#ifdef FF_FATX_PROOF
    if(ff_fatx_owns_handle(row[2]))stop(9,row[1],200);
#endif
    if(!disk_ready)stop(9,row[1],200);
    row[1]=ff_disk_fs_control(&local_disk,row+2);
    if(row[1]==FF_BOOT_UNSUPPORTED || row[1]==FF_BOOT_INVALID || row[1]==FF_VM_ACCESS_VIOLATION)stop(9,row[1],200);
    if(row[1]!=FF_BOOT_BAD_HANDLE){row[12]=MEM32(row[6]);row[13]=MEM32(row[6]+4);}
    if(!row[1])for(unsigned i=0;i<FF_DISK_HANDLES;++i)if(local_disk.handles[i].handle==row[2]) {
        const uint32_t id=local_disk.handles[i].volume;
        const ff_disk_volume *volume=&local_disk.volumes[id-1];
        row[14]=id;row[15]=volume->state;row[16]=volume->references;row[17]=volume->dismounts;
    }
    g_eax=row[1];g_esp+=44;
}
#endif
#endif
#endif
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
#ifdef FF_FATX_PROOF
    if(ff_fatx_owns_handle(row[2]))row[1]=fatx_ready?ff_fatx_query_size(&fatx_volume,row+2):FF_BOOT_UNSUPPORTED;
    else
#endif
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
#ifdef FF_FATX_PROOF
uint32_t ff_probe_fatx_word(uint32_t field)
{
    switch(field){case 0:return fatx_ready;case 1:return fatx_volume.mount_count;case 2:return fatx_volume.file_count;
        case 3:return fatx_volume.active;case 4:return fatx_volume.opened;case 5:return fatx_volume.closed;default:return UINT32_MAX;}
}
uint32_t ff_probe_fatx_mount_word(uint32_t index,uint32_t field)
{
    if(index>=fatx_volume.mount_count)return UINT32_MAX;
    const ff_fatx_mount *mount=&fatx_volume.mounts[index];
    switch(field){case 0:return mount->id;case 1:return mount->partition;case 2:return mount->device;case 3:return mount->serial;
        case 4:return mount->sectors_per_cluster;case 5:return mount->cluster_bytes;case 6:return mount->entry_bytes;
        case 7:return mount->root_cluster;case 8:return mount->cluster_count;case 9:return mount->free_clusters;
        case 10:return mount->root_clusters;case 11:return mount->file_objects;
        case 12:return (uint32_t)mount->offset;case 13:return (uint32_t)(mount->offset>>32);
        case 14:return (uint32_t)mount->bytes;case 15:return (uint32_t)(mount->bytes>>32);
        case 16:return (uint32_t)mount->fat_bytes;case 17:return (uint32_t)(mount->fat_bytes>>32);
        case 18:return (uint32_t)mount->data_offset;case 19:return (uint32_t)(mount->data_offset>>32);
        case 20:return (uint32_t)mount->revision;case 21:return (uint32_t)(mount->revision>>32);default:return UINT32_MAX;}
}
uint32_t ff_probe_fatx_file_word(uint32_t index,uint32_t field)
{
    if(index>=FF_FATX_FILES)return UINT32_MAX;
    const ff_fatx_file *file=&fatx_volume.files[index];
    switch(field){case 0:return file->id;case 1:return file->mount;case 2:return file->handle;case 3:return file->access;
        case 4:return file->share;case 5:return file->options;case 6:return file->references;default:return UINT32_MAX;}
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
#ifdef FF_NV_QUERY_PROOF
    if(!misc_flags_ready || (row[2]!=17 && row[2]!=10) || (row[2]==10 && !parental_games_ready)) {
        row[1]=FF_BOOT_UNSUPPORTED;stop(9,row[2],24);
    }
    row[1]=ff_query_console_dword(&virtual_memory,row+2,row[2]==17?misc_flags:parental_games);
#else
    if (!misc_flags_ready || row[2] != 17) { row[1]=FF_BOOT_UNSUPPORTED; stop(9, row[2], 24); }
    row[1] = ff_query_misc_flags(guest_ram, FF_GUEST_BYTES, row + 2, misc_flags);
#endif
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
    (void)ff_probe_guest_memory(row[2], 28, FF_VM_WRITE, 0u);
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
static void allocate_contiguous_memory(void)
{
    uint32_t *row = kernel_record(166, 5), base = 0;
    row[1] = ff_vm_allocate_contiguous(&virtual_memory, row[2], row[3], row[4], row[5], row[6], &base);
    if (row[1] != 0 && row[1] != FF_BOOT_NO_MEMORY) stop(9, row[1], 166);
    row[7] = base;
    g_eax = base; g_esp += 24;
}
static void claim_gpu_instance_memory(void)
{
    uint32_t *row = kernel_record(168, 2), base = 0;
    row[1] = ff_claim_gpu_instance_memory(&virtual_memory, row[2], row[3], 0, 0x03e00000u, &base);
    if (row[1]) stop(9, row[1], 168);
    row[4] = base;
    g_eax = base; g_esp += 12;
}
static void rtl_time_to_fields(void)
{
    uint32_t *row = kernel_record(305, 2);
    if (!row[2] || (row[2] & 3u) || ff_vm_check(&virtual_memory, row[2], 8, FF_VM_READ))
        stop(9, FF_VM_ACCESS_VIOLATION, 305);
    if (!row[3] || (row[3] & 1u) || ff_vm_check(&virtual_memory, row[3], 16, FF_VM_WRITE))
        stop(9, FF_VM_ACCESS_VIOLATION, 305);
    uint64_t time = (uint64_t)MEM32(row[2]) | ((uint64_t)MEM32(row[2] + 4) << 32);
    uint16_t fields[8] = {0};
    row[1] = ff_time_to_fields(time, fields);
    if (row[1]) stop(9, row[1], 305);
    for (unsigned i = 0; i < 8; ++i) { MEM16(row[3] + i * 2u) = fields[i]; row[4 + i] = fields[i]; }
    g_eax = 0; g_esp += 12;
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
#if defined(FF_MMIO_PROOF) && defined(FF_VM_PROOF)
static void av_send_tv_encoder_option(void)
{
    uint32_t *row = kernel_record(2, 4);
    if (!av_query_ready) {
        row[1] = FF_BOOT_UNSUPPORTED;
        stop(1, 0xFE000094u, 2);
    }
    const uint32_t eax_before = g_eax, esp_before = g_esp;
    const uint32_t args[4] = {row[2], row[3], row[4], row[5]};
    const ff_av_query_config config = {av_query_word};
    ff_av_query_cpu cpu = {esp_before, eax_before};
    const ff_av_query_result result = ff_av_query_adapter(&virtual_memory, args, &config, &cpu);
    if (result != FF_AV_QUERY_WROTE) {
        row[1] = FF_BOOT_UNSUPPORTED;
        stop(1, 0xFE000094u, 2);
    }
    /* The guest-run effect must match the adapter contract: VOID leaves EAX
     * untouched and the stdcall return pops the return address plus four
     * arguments. A mismatch is an internal proof failure, not a guest result. */
    if (cpu.eax != eax_before || cpu.esp != esp_before + 20u) {
        row[1] = 4;
        stop(4, 0xFE000094u, 2);
    }
    g_esp = cpu.esp;
    /* VOID-style: g_eax is intentionally unchanged. */
    row[1] = 0;
    row[6] = av_query_word;
    row[7] = eax_before;
    row[8] = esp_before;
    row[9] = cpu.eax;
    row[10] = cpu.esp;
    row[11] = args[3]; /* output address the adapter wrote through */
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
#ifdef FF_SECTION_PROOF
        if (ordinal == 327) return load_image_section;
        if (ordinal == 328) return unload_image_section;
#endif
#ifdef FF_VM_PROOF
        if (ordinal == 184) return allocate_virtual_memory;
        if (ordinal == 291) return initialize_critical;
        if (ordinal == 44) return hal_get_interrupt_vector;
        if (ordinal == 109) return ke_initialize_interrupt;
        if (ordinal == 98) return ke_connect_interrupt;
        if (ordinal == 47) return hal_register_shutdown_notification;
#ifdef FF_MEMORY_STATISTICS_PROOF
        if (ordinal == 181) return query_memory_statistics;
        if (ordinal == 166) return allocate_contiguous_memory;
        if (ordinal == 168) return claim_gpu_instance_memory;
        if (ordinal == 305) return rtl_time_to_fields;
#endif
#ifdef FF_TIMER_PROOF
        if (ordinal == 128) return query_system_time;
#endif
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
#ifdef FF_DISK_PROOF
        if (ordinal == 219) return read_disk;
        if (ordinal == 236) return write_file;
#ifdef FF_DISK_GEOMETRY_PROOF
        if (ordinal == 196) return control_disk;
#ifdef FF_DISK_DISMOUNT_PROOF
        if (ordinal == 200) return filesystem_control_disk;
#endif
#endif
#else
        if (ordinal == 236) return write_regular_file;
#endif
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
#if defined(FF_MMIO_PROOF) && defined(FF_VM_PROOF)
        if (ordinal == 2 && av_query_ready) return av_send_tv_encoder_option;
#endif
        /* Record the attempted unsupported service with its observed call-frame
         * arguments, then stop. No return value is supplied and nothing is
         * modeled; the row makes the boundary inspectable in the report. */
        {
            uint32_t *row = kernel_record(ordinal, 4);
            row[1] = 1;
            for (uint32_t i = 0; i < 4; ++i) row[2 + i] = MEM32(g_esp + 4u + i * 4u);
        }
        stop(1, address, ordinal);
    }
    return 0;
}

uint32_t ff_probe_abi(void) {
#ifdef FF_MMIO_PROOF
    return 24;
#elif defined(FF_MEMORY_STATISTICS_PROOF)
    return 23;
#elif defined(FF_FATX_PROOF)
    return 22;
#elif defined(FF_DISK_DISMOUNT_PROOF)
    return 21;
#elif defined(FF_DISK_INFORMATION_PROOF)
    return 20;
#elif defined(FF_DISK_GEOMETRY_PROOF)
    return 19;
#elif defined(FF_DISK_PARTITION_PROOF)
    return 18;
#elif defined(FF_DISK_PROOF)
    return 17;
#elif defined(FF_INITIALIZER_BATCH_PROOF)
    return 16;
#elif defined(FF_STARTUP_EXPANSION_PROOF)
    return 15;
#elif defined(FF_NV_QUERY_PROOF)
    return 14;
#elif defined(FF_SECTION_PROOF)
    return 13;
#elif defined(FF_FILE_WRITE_PROOF)
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
#ifdef FF_NV_QUERY_PROOF
void ff_probe_parental_games(uint32_t value) { parental_games=value;parental_games_ready=1; }
#endif
uint32_t ff_probe_ram(void) { return (uint32_t)(uintptr_t)guest_ram; }
uint32_t ff_probe_ram_bytes(void) { return FF_GUEST_BYTES; }
uint32_t ff_probe_stack_top(void) { return FF_STACK_TOP; }
uint32_t ff_probe_source_digest(void) { return (uint32_t)(uintptr_t)source_digest; }
uint32_t ff_probe_entry(void) { return FF_ENTRY_ADDRESS; }
uint32_t ff_probe_state(void) { return (uint32_t)(uintptr_t)probe_state; }
uint32_t ff_probe_trace(void) { return (uint32_t)(uintptr_t)probe_trace; }
uint32_t ff_probe_trace_count(void) { return probe_trace_count; }
uint32_t ff_probe_trace_capacity(void) { return FF_TRACE_CAPACITY; }
uint32_t ff_probe_timer_check(void) { return timer_check; }
uint32_t ff_probe_dpc_head(void) { return timers.dpc_head; }
uint32_t ff_probe_dpc_head_value(void) { return guest_ram[timers.dpc_head] | (uint32_t)guest_ram[timers.dpc_head + 1] << 8 | (uint32_t)guest_ram[timers.dpc_head + 2] << 16 | (uint32_t)guest_ram[timers.dpc_head + 3] << 24; }
uint32_t ff_probe_timer_head(void) { return timers.timer_head; }
uint32_t ff_probe_timer_head_value(void) { return guest_ram[timers.timer_head] | (uint32_t)guest_ram[timers.timer_head + 1] << 8 | (uint32_t)guest_ram[timers.timer_head + 2] << 16 | (uint32_t)guest_ram[timers.timer_head + 3] << 24; }
uint32_t ff_probe_guest_word(uint32_t address)
{
    if (address > FF_GUEST_BYTES - 4u) return 0;
    return guest_ram[address] | (uint32_t)guest_ram[address + 1] << 8 |
           (uint32_t)guest_ram[address + 2] << 16 | (uint32_t)guest_ram[address + 3] << 24;
}
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
#ifdef FF_STARTUP_EXPANSION_PROOF
uint32_t ff_probe_fp_word(uint32_t field)
{
    switch(field) {
    case 0:return g_fp_control_word;
    case 1:return g_mxcsr;
    case 2:return g_fp_cc;
    case 3:return (uint32_t)g_fp_top;
    default:return UINT32_MAX;
    }
}
#endif
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
    ff_port_io_init(&port_io);
#ifdef FF_DISK_GEOMETRY_PROOF
    memset(disk_control_information,0,sizeof disk_control_information);
#endif
#ifdef FF_REGULAR_FILE_PROOF
    memset(file_information,0,sizeof file_information);
    memset(create_request_words,0,sizeof create_request_words); memset(create_request_names,0,sizeof create_request_names);
#endif
    probe_trace_count = kernel_call_count = parent_returned = 0;
    misc_flags = misc_flags_ready = 0;
#ifdef FF_NV_QUERY_PROOF
    parental_games=parental_games_ready=0;
#endif
#ifdef FF_SECTION_PROOF
    memset(&image_sections,0,sizeof image_sections);
#endif
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
#ifdef FF_DISK_PROOF
    memset(&local_disk,0,sizeof local_disk);disk_ready=0;
#endif
#ifdef FF_FATX_PROOF
    memset(&fatx_volume,0,sizeof fatx_volume);fatx_ready=0;
#endif
#ifdef FF_MEMORY_STATISTICS_PROOF
    memset(&memory_statistics,0,sizeof memory_statistics);memory_statistics_ready=0;
#endif
#ifdef FF_MMIO_PROOF
    memset(&device_registers,0,sizeof device_registers);
#endif
#ifdef FF_VM_PROOF
    if (ff_vm_init(&virtual_memory, guest_ram, FF_GUEST_BYTES)) __builtin_trap();
    ff_guest_mirror_init(&guest_mirror);
    ff_guest_write_map_init(&guest_write_map);
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
#ifdef FF_DISK_PROOF
    if(ff_vm_map_fixed(&virtual_memory,FF_DISK_DATA_ADDRESS,FF_VM_PAGE,2))stop(9,FF_DISK_DATA_ADDRESS,40);
#endif
    for (unsigned i = 0; i < FF_KERNEL_BINDING_COUNT; ++i) {
        if ((bindings[i].slot & 3u) || bindings[i].slot > FF_GUEST_BYTES - 4u) stop(3, bindings[i].slot, 0);
#ifdef FF_DISK_PROOF
        if(bindings[i].ordinal==40){MEM32(bindings[i].slot)=FF_DISK_DATA_ADDRESS;continue;}
#endif
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
    /* Model-owned list heads live in the reserved synthetic queue area. The
     * reached code writes TLS+0x50, so that address must not double as a head. */
    if (ff_timers_init(&timers, guest_ram, FF_GUEST_BYTES, 0x03e00000u, 0x03e00008u) != FF_BOOT_OK) stop(9, 0, 149);
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
