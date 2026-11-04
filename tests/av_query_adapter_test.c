/* Native test for the guest-call adapter around the numeric-6 component.
 *
 * The adapter must write exactly four bytes on success, keep EAX unchanged,
 * remove the return address and four stdcall arguments (ESP += 20), and leave
 * EAX, ESP and every output byte unchanged on denial.
 */
#include "../src/runtime/av_query_adapter.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_ONLY_BASE 0x00010000u
#define READ_WRITE_BASE 0x00020000u
#define ONE_PAGE_BASE 0x00030000u
#define UNMAPPED_BASE 0x00040000u
#define REGION_END 0x00050000u

static uint8_t ram[REGION_END];
static ff_virtual_memory vm;
static const uint32_t initial_esp = 0x00001000u;
static const uint32_t initial_eax = 0xdeadbeefu;

static void fill(uint32_t base, uint32_t bytes, uint8_t value)
{
    memset(ram + base, value, bytes);
}
static void assert_window(uint32_t base, uint32_t bytes, uint8_t value)
{
    for (uint32_t i = 0; i < bytes; ++i) assert(ram[base + i] == value);
}
static void expect_write(uint32_t pointer, uint32_t word,
                         uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    fill(pointer - 4u, 12u, 0xaa);
    for (uint32_t i = 0; i < 4u; ++i) ram[pointer + i] = 0xcc;
    const uint32_t args[4] = {0u, 6u, 0u, pointer};
    const ff_av_query_config config = {word};
    ff_av_query_cpu cpu = {initial_esp, initial_eax};
    assert(ff_av_query_adapter(&vm, args, &config, &cpu) == FF_AV_QUERY_WROTE);
    assert(ram[pointer + 0] == b0 && ram[pointer + 1] == b1);
    assert(ram[pointer + 2] == b2 && ram[pointer + 3] == b3);
    assert_window(pointer - 4u, 4u, 0xaa);
    assert_window(pointer + 4u, 4u, 0xaa);
    assert(cpu.esp == initial_esp + 20u);
    assert(cpu.eax == initial_eax);
}
static void expect_denied(uint32_t pointer, const uint32_t args[4], const ff_av_query_config *config)
{
    const int bounded = pointer >= 4u && (uint64_t)pointer + 8u <= sizeof ram;
    if (bounded) fill(pointer - 4u, 12u, 0xaa);
    ff_av_query_cpu cpu = {initial_esp, initial_eax};
    assert(ff_av_query_adapter(&vm, args, config, &cpu) == FF_AV_QUERY_DENIED);
    if (bounded) assert_window(pointer - 4u, 12u, 0xaa);
    assert(cpu.esp == initial_esp);
    assert(cpu.eax == initial_eax);
}

int main(void)
{
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, READ_ONLY_BASE, REGION_END - READ_ONLY_BASE) == 0);
    assert(ff_vm_map_fixed(&vm, READ_ONLY_BASE, READ_WRITE_BASE - READ_ONLY_BASE, 0x02u) == 0);
    assert(ff_vm_map_fixed(&vm, READ_WRITE_BASE, ONE_PAGE_BASE - READ_WRITE_BASE, 0x04u) == 0);
    assert(ff_vm_map_fixed(&vm, ONE_PAGE_BASE, FF_VM_PAGE, 0x04u) == 0);

    expect_write(READ_WRITE_BASE, 0x00800303u, 0x03, 0x03, 0x80, 0x00);
    expect_write(READ_WRITE_BASE + 1u, 0x00400104u, 0x04, 0x01, 0x40, 0x00);

    /* A null config is absent, not a zero default. */
    const uint32_t valid[4] = {0u, 6u, 0u, READ_WRITE_BASE + 16u};
    expect_denied(READ_WRITE_BASE + 16u, valid, NULL);
    const ff_av_query_config config = {0x00800303u};

    /* Read-only, null, unmapped, wrong-shape and stack-wrap denials leave the
     * output, EAX and ESP unchanged. */
    const uint32_t read_only[4] = {0u, 6u, 0u, READ_ONLY_BASE};
    expect_denied(READ_ONLY_BASE, read_only, &config);
    const uint32_t null_pointer[4] = {0u, 6u, 0u, 0u};
    expect_denied(0, null_pointer, &config);
    const uint32_t unmapped[4] = {0u, 6u, 0u, UNMAPPED_BASE};
    expect_denied(UNMAPPED_BASE, unmapped, &config);

    const uint32_t wrong_option[4] = {0u, 5u, 0u, READ_WRITE_BASE + 32u};
    expect_denied(READ_WRITE_BASE + 32u, wrong_option, &config);
    const uint32_t wrong_base[4] = {1u, 6u, 0u, READ_WRITE_BASE + 32u};
    expect_denied(READ_WRITE_BASE + 32u, wrong_base, &config);
    const uint32_t wrong_param[4] = {0u, 6u, 1u, READ_WRITE_BASE + 32u};
    expect_denied(READ_WRITE_BASE + 32u, wrong_param, &config);

    const uint32_t wrap[4] = {0u, 6u, 0u, READ_WRITE_BASE + 48u};
    fill(READ_WRITE_BASE + 44u, 12u, 0xaa);
    ff_av_query_cpu wrapped = {0xfffffff0u, initial_eax};
    assert(ff_av_query_adapter(&vm, wrap, &config, &wrapped) == FF_AV_QUERY_DENIED);
    assert_window(READ_WRITE_BASE + 44u, 12u, 0xaa);
    assert(wrapped.esp == 0xfffffff0u && wrapped.eax == initial_eax);

    ff_av_query_cpu cpu = {initial_esp, initial_eax};
    assert(ff_av_query_adapter(NULL, valid, &config, &cpu) == FF_AV_QUERY_DENIED);
    assert(ff_av_query_adapter(&vm, NULL, &config, &cpu) == FF_AV_QUERY_DENIED);
    assert(ff_av_query_adapter(&vm, valid, &config, NULL) == FF_AV_QUERY_DENIED);
    assert(cpu.esp == initial_esp && cpu.eax == initial_eax);

    puts("numeric-6 adapter: VOID/EAX/stack cleanup and unchanged denials passed");
    return 0;
}
