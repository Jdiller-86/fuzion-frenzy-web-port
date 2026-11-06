#include "dpc_context.h"
#include "thread_bootstrap.h"
#include <string.h>

static int range(uint32_t bytes, uint32_t address, uint32_t size)
{
    return address && !(address & 3u) && size <= bytes && address <= bytes - size;
}
static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= (uint32_t)ram[address + i] << (i * 8);
    return value;
}
static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (i * 8));
}
static int overlap(uint32_t a, uint32_t an, uint32_t b, uint32_t bn)
{
    return (uint64_t)a < (uint64_t)b + bn && (uint64_t)b < (uint64_t)a + an;
}
uint32_t ff_dpc_begin(ff_dpc_frame *frame, ff_guest_cpu *cpu, uint8_t *ram, uint32_t bytes,
                      uint32_t low, uint32_t top, const ff_dpc_call *call)
{
    if (!frame || !cpu || !ram || !call || frame->active || !call->routine ||
        !range(bytes, cpu->r_fs_base, 0x5c) || !range(bytes, call->object, 28) ||
        !low || (low & 15u) || (top & 15u) || top <= low || top > bytes || top - low < 256 ||
        overlap(low, top - low, cpu->r_fs_base, 0x5c) || overlap(low, top - low, call->object, 28)) return FF_BOOT_INVALID;
    uint32_t fs = cpu->r_fs_base, base = read32(ram, fs + 4), limit = read32(ram, fs + 8);
    if (!range(bytes, limit, 4) || (base & 3u) || (cpu->r_esp & 3u) || limit >= base || base > bytes ||
        cpu->r_esp < limit || cpu->r_esp > base - 4 || overlap(fs, 0x5c, limit, base - limit) ||
        overlap(low, top - low, limit, base - limit) || read32(ram, fs + 0x18) != fs ||
        read32(ram, fs + 0x1c) != fs || read32(ram, fs + 0x20) != fs + 0x28) return FF_BOOT_INVALID;
    if (ram[fs + 0x24] >= 2 || read32(ram, fs + 0x58)) return FF_BOOT_UNSUPPORTED;
    if (ram[call->object] != 0x13 || ram[call->object + 1] || ram[call->object + 2] > 1 ||
        read32(ram, call->object + 12) != call->routine || read32(ram, call->object + 16) != call->context ||
        read32(ram, call->object + 20) != call->argument1 || read32(ram, call->object + 24) != call->argument2) return FF_BOOT_INVALID;
    frame->saved = *cpu; frame->fs = fs; frame->exception_head = read32(ram, fs);
    frame->stack_base = base; frame->stack_limit = limit; frame->irql = ram[fs + 0x24];
    frame->dpc_active = read32(ram, fs + 0x58); frame->stack_top = top; frame->frame_sp = top - 20;
    write32(ram, top - 20, FF_DPC_RETURN_SENTINEL); write32(ram, top - 16, call->object);
    write32(ram, top - 12, call->context); write32(ram, top - 8, call->argument1); write32(ram, top - 4, call->argument2);
    write32(ram, fs, UINT32_MAX); write32(ram, fs + 4, top); write32(ram, fs + 8, low);
    ram[fs + 0x24] = 2; write32(ram, fs + 0x58, 1);
    cpu->r_esp = top - 20; cpu->r_ebp = 0; cpu->r_seh_ebp = 0; cpu->r_df = 0; cpu->r_force_return = 0; cpu->r_eflags &= ~0x400u;
    frame->active = 1; return FF_BOOT_OK;
}
uint32_t ff_dpc_abort(ff_dpc_frame *frame, ff_guest_cpu *cpu, uint8_t *ram, uint32_t bytes)
{
    if (!frame || !frame->active || !cpu || !ram || !range(bytes, frame->fs, 0x5c)) return FF_BOOT_INVALID;
    write32(ram, frame->fs, frame->exception_head); write32(ram, frame->fs + 4, frame->stack_base);
    write32(ram, frame->fs + 8, frame->stack_limit); ram[frame->fs + 0x24] = (uint8_t)frame->irql;
    write32(ram, frame->fs + 0x58, frame->dpc_active);
    *cpu = frame->saved; frame->active = 0; return FF_BOOT_OK;
}
uint32_t ff_dpc_end(ff_dpc_frame *frame, ff_guest_cpu *cpu, uint8_t *ram, uint32_t bytes)
{
    if (!frame || !frame->active || !cpu || !ram || !range(bytes, frame->frame_sp, 20)) return FF_BOOT_INVALID;
    uint32_t result = cpu->r_esp == frame->stack_top && read32(ram, frame->frame_sp) == FF_DPC_RETURN_SENTINEL
        ? FF_BOOT_OK : FF_BOOT_INVALID;
    uint32_t restore = ff_dpc_abort(frame, cpu, ram, bytes);
    return restore == FF_BOOT_OK ? result : restore;
}
