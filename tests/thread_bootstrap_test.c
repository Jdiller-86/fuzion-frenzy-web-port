/* Owned memory/API cases; no game content, vendor runtime or console firmware. */
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[1024 * 1024];
static ff_boot_runtime rt;
static uint32_t read_word(uint32_t address)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= (uint32_t)ram[address + i] << (i * 8);
    return value;
}
static void write_word(uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (i * 8));
}
static void reset(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_boot_init(&rt, ram, sizeof ram, 0x10000, sizeof ram) == FF_BOOT_OK);
}
int main(void)
{
    uint32_t args[10] = { 0x100, 0, 65536, 20, 0x104, 0x2000, 0x76543210, 0, 0, 0x3000 };
    reset();
    assert(ff_boot_create(&rt, args) == FF_BOOT_OK);
    const ff_boot_thread *thread = &rt.threads[0];
    assert(read_word(args[0]) == thread->handle && read_word(args[4]) == thread->id);
    assert(read_word(thread->fs + 0x28) == thread->object);
    assert(read_word(thread->object + 0x28) == thread->tls);
    assert(read_word(thread->fs + 4) - thread->tls_size == thread->tls);
    assert(!((thread->tls + 4) & 15u));
    for (unsigned i = 0; i < thread->tls_size; ++i) assert(ram[thread->tls + i] == 0);
    assert(read_word(thread->initial_sp) == 0 && read_word(thread->initial_sp + 4) == args[5]);
    assert(read_word(thread->initial_sp + 8) == args[6]);
    assert(thread->initial_sp >= thread->stack_low && thread->initial_sp + 12 <= thread->tls);
    assert(ff_boot_close(&rt, thread->handle) == FF_BOOT_OK);
    assert(ff_boot_close(&rt, thread->handle) == FF_BOOT_BAD_HANDLE);
    assert(ff_boot_next(&rt) == thread && thread->state == FF_BOOT_RUNNING);
    assert(ff_boot_next(&rt) == 0);
    write_word(0x210, UINT32_MAX); write_word(0x214, 0); write_word(0x218, 0);
    uint32_t event_prefix = read_word(0x200);
    assert(ff_boot_enter_critical(&rt, 0x200) == FF_BOOT_OK);
    assert(read_word(0x210) == 0 && read_word(0x214) == 1 && read_word(0x218) == thread->object);
    assert(ff_boot_enter_critical(&rt, 0x200) == FF_BOOT_OK);
    assert(read_word(0x210) == 1 && read_word(0x214) == 2);
    assert(ff_boot_leave_critical(&rt, 0x200) == FF_BOOT_OK);
    assert(read_word(0x210) == 0 && read_word(0x214) == 1);
    assert(ff_boot_leave_critical(&rt, 0x200) == FF_BOOT_OK);
    assert(read_word(0x210) == UINT32_MAX && read_word(0x214) == 0 && read_word(0x218) == 0);
    assert(read_word(0x200) == event_prefix);
    assert(ff_boot_leave_critical(&rt, 0x200) == FF_BOOT_INVALID);
    write_word(0x210, 0); write_word(0x214, 1); write_word(0x218, 0x1234);
    assert(ff_boot_enter_critical(&rt, 0x200) == FF_BOOT_UNSUPPORTED);
    assert(read_word(0x210) == 0 && read_word(0x214) == 1 && read_word(0x218) == 0x1234);

    reset();
    args[7] = 1;
    assert(ff_boot_create(&rt, args) == FF_BOOT_OK);
    assert(ff_boot_next(&rt) == 0); /* Suspension is honored. Resume is not implemented. */
    args[7] = 0; args[2] = 0; args[3] = 52; args[4] = 0;
    assert(ff_boot_create(&rt, args) == FF_BOOT_OK);
    assert(rt.threads[1].stack_top - rt.threads[1].stack_low == 65536);
    assert(rt.threads[1].object >= rt.threads[0].stack_top);
    assert(rt.threads[1].tls != rt.threads[0].tls && rt.threads[1].fs != rt.threads[0].fs);
    assert(ff_boot_next(&rt) == &rt.threads[1]);

    reset();
    uint32_t cursor = rt.cursor, before = read_word(args[0]);
    for (unsigned i = 0; i < 3; ++i) {
        uint32_t invalid[10]; memcpy(invalid, args, sizeof invalid);
        invalid[0] = i == 0 ? 0 : i == 1 ? 3 : sizeof ram;
        assert(ff_boot_create(&rt, invalid) == FF_BOOT_INVALID);
        assert(rt.count == 0 && rt.cursor == cursor && read_word(args[0]) == before);
    }
    args[8] = 1;
    assert(ff_boot_create(&rt, args) == FF_BOOT_UNSUPPORTED && rt.count == 0);
    args[8] = 0; args[3] = 8;
    assert(ff_boot_create(&rt, args) == FF_BOOT_UNSUPPORTED && rt.count == 0);
    args[3] = 20;
    for (unsigned i = 0; i < FF_BOOT_MAX_THREADS; ++i) assert(ff_boot_create(&rt, args) == FF_BOOT_OK);
    cursor = rt.cursor;
    assert(ff_boot_create(&rt, args) == FF_BOOT_NO_MEMORY && rt.cursor == cursor);
    assert(ff_boot_close(&rt, 0xdeadbeef) == FF_BOOT_BAD_HANDLE);

    reset(); rt.end = rt.cursor + 4096;
    assert(ff_boot_create(&rt, args) == FF_BOOT_NO_MEMORY && rt.count == 0);
    puts("Boot-thread contexts: allocation, TLS/FS, call frame, handles, suspension, isolation, failure atomicity and recursive uncontended critical sections passed");
    return 0;
}
