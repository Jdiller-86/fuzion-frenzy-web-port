#include "thread_bootstrap.h"
#include <string.h>

static int output_range(const ff_boot_runtime *rt, uint32_t address)
{
    return address && !(address & 3u) && rt->bytes >= 4 && address <= rt->bytes - 4;
}

static void word(ff_boot_runtime *rt, uint32_t address, uint32_t value)
{
    /* Guest little-endian layout is independent of the compiler host. */
    for (unsigned i = 0; i < 4; ++i) rt->ram[address + i] = (uint8_t)(value >> (i * 8));
}

static uint32_t read_word(const ff_boot_runtime *rt, uint32_t address)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= (uint32_t)rt->ram[address + i] << (i * 8);
    return value;
}

uint32_t ff_boot_init(ff_boot_runtime *rt, uint8_t *ram, uint32_t bytes, uint32_t low, uint32_t high)
{
    if (!rt || !ram || !low || (low & 4095u) || (high & 4095u) || low >= high || high > bytes) return FF_BOOT_INVALID;
    memset(rt, 0, sizeof(*rt));
    rt->ram = ram; rt->bytes = bytes; rt->cursor = low; rt->end = high;
    rt->current = UINT32_MAX;
    return FF_BOOT_OK;
}

uint32_t ff_boot_create(ff_boot_runtime *rt, const uint32_t args[10])
{
    if (!rt || !rt->ram || !args || !output_range(rt, args[0]) || (args[4] && !output_range(rt, args[4])) ||
        !args[5] || !args[9]) return FF_BOOT_INVALID;
    /* Fail explicitly for unsupported kernel options, never pretend to honor them. */
    if (args[1] || args[7] > 1 || args[8] || (args[3] && (args[3] & 15u) != 4)) return FF_BOOT_UNSUPPORTED;
    uint32_t stack = args[2] ? args[2] : 65536u, tls_size = args[3];
    if (stack > 1024u * 1024u || tls_size > stack || stack - tls_size < 128) return FF_BOOT_UNSUPPORTED;
    stack = (stack + 4095u) & ~4095u;
    /* One page contains the small, currently supported thread/PCR prefixes.
     * The following pages contain the thread's private guest stack and TLS. */
    uint32_t needed = 4096u + stack;
    if (rt->count == FF_BOOT_MAX_THREADS || rt->cursor > rt->end || needed > rt->end - rt->cursor) return FF_BOOT_NO_MEMORY;
    uint32_t object = rt->cursor, fs = object + 0x200u, low = object + 4096u, top = low + stack;
    if ((args[0] >= object && args[0] < top) || (args[4] >= object && args[4] < top)) return FF_BOOT_INVALID;
    ff_boot_thread thread = { .id = rt->count + 1, .handle = 0x100u + rt->count * 4,
        .handle_open = 1, .state = args[7] ? FF_BOOT_SUSPENDED : FF_BOOT_READY,
        .object = object, .fs = fs, .stack_low = low, .stack_top = top,
        .tls = tls_size ? top - tls_size : 0, .tls_size = tls_size,
        .routine = args[5], .context = args[6], .system = args[9],
        .initial_sp = ((top - tls_size) & ~15u) - 16u - 12u };
    memset(rt->ram + object, 0, needed);
    rt->ram[object] = 6; /* Thread dispatcher-object type; prefix only. */
    word(rt, object + 0x1c, top); word(rt, object + 0x20, low);
    word(rt, object + 0x24, thread.initial_sp); word(rt, object + 0x28, thread.tls);
    word(rt, fs, UINT32_MAX); /* Empty SEH chain. Original startup builds it. */
    word(rt, fs + 4, top); word(rt, fs + 8, low); word(rt, fs + 0x18, fs);
    word(rt, fs + 0x1c, fs); word(rt, fs + 0x20, fs + 0x28); word(rt, fs + 0x28, object);
    /* NTAPI system routine receives (game start routine, game context). */
    word(rt, thread.initial_sp, 0); word(rt, thread.initial_sp + 4, thread.routine);
    word(rt, thread.initial_sp + 8, thread.context);
    rt->threads[rt->count++] = thread; rt->cursor = top;
    word(rt, args[0], thread.handle);
    if (args[4]) word(rt, args[4], thread.id);
    return FF_BOOT_OK;
}

uint32_t ff_boot_close(ff_boot_runtime *rt, uint32_t handle)
{
    if (!rt || !rt->ram) return FF_BOOT_BAD_HANDLE;
    for (uint32_t i = 0; i < rt->count; ++i) {
        ff_boot_thread *thread = &rt->threads[i];
        if (thread->handle == handle && thread->handle_open) {
            thread->handle_open = 0;
            return FF_BOOT_OK; /* Runnable-thread ownership survives handle close. */
        }
    }
    return FF_BOOT_BAD_HANDLE;
}

ff_boot_thread *ff_boot_next(ff_boot_runtime *rt)
{
    if (!rt || !rt->ram || rt->current != UINT32_MAX) return 0;
    for (uint32_t i = 0; i < rt->count; ++i) if (rt->threads[i].state == FF_BOOT_READY) {
        rt->current = i; rt->threads[i].state = FF_BOOT_RUNNING;
        return &rt->threads[i];
    }
    return 0;
}

static uint32_t critical(ff_boot_runtime *rt, uint32_t address, int enter)
{
    if (!rt || !rt->ram || rt->bytes < 28 || !address || (address & 3u) || address > rt->bytes - 28 ||
        rt->current >= rt->count) return FF_BOOT_INVALID;
    /* Public Xbox RTL layout: 16-byte event prefix, lock count, recursion,
     * owning KTHREAD pointer. The event prefix is untouched without waiters. */
    uint32_t lock = read_word(rt, address + 16), depth = read_word(rt, address + 20);
    uint32_t owner = read_word(rt, address + 24), self = rt->threads[rt->current].object;
    if (enter && !owner && !depth && lock == UINT32_MAX) {
        word(rt, address + 16, 0); word(rt, address + 20, 1); word(rt, address + 24, self);
        return FF_BOOT_OK;
    }
    if (owner != self) return enter ? FF_BOOT_UNSUPPORTED : FF_BOOT_INVALID;
    if (!depth || depth >= INT32_MAX || lock != depth - 1) return FF_BOOT_UNSUPPORTED;
    if (enter) {
        word(rt, address + 16, lock + 1); word(rt, address + 20, depth + 1);
    } else {
        word(rt, address + 16, lock - 1); word(rt, address + 20, depth - 1);
        if (depth == 1) word(rt, address + 24, 0);
    }
    return FF_BOOT_OK;
}

uint32_t ff_boot_enter_critical(ff_boot_runtime *rt, uint32_t address) { return critical(rt, address, 1); }
uint32_t ff_boot_leave_critical(ff_boot_runtime *rt, uint32_t address) { return critical(rt, address, 0); }
