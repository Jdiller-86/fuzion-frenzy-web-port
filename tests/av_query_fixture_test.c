/* Native test for the unconnected numeric-6 query component.
 *
 * The test calls the real src/runtime/av_query.c component against a real
 * ff_virtual_memory window. It does not reimplement the writer. The synthetic
 * DWORD is supplied through a nullable config pointer; a null config means the
 * value is absent, not zero. Stores are bytewise little-endian and the guest
 * pointer may be unaligned. Every granted write must change exactly four bytes
 * and leave the surrounding bytes unchanged; every denial must leave them
 * unchanged.
 */
#include "../src/runtime/av_query.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_ONLY_BASE 0x00010000u
#define READ_WRITE_BASE 0x00020000u
#define ONE_PAGE_BASE 0x00030000u
#define TWO_PAGE_BASE 0x00032000u
#define UNMAPPED_BASE 0x00040000u
#define VM_END_PAGE 0x0004F000u
#define REGION_END 0x00050000u

static uint8_t ram[REGION_END];
static ff_virtual_memory vm;

static ff_av_query_arguments query(uint32_t pointer)
{
    ff_av_query_arguments arguments;
    arguments.register_base = 0u;
    arguments.option = FF_AV_QUERY_NUMERIC_OPTION;
    arguments.param = 0u;
    arguments.result_pointer = pointer;
    return arguments;
}

static ff_av_query_config config(uint32_t word)
{
    ff_av_query_config value;
    value.word = word;
    return value;
}

static void fill_window(uint32_t base, uint32_t bytes, uint8_t value)
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
    assert(pointer >= 4u && (uint64_t)pointer + 8u <= sizeof ram);
    fill_window(pointer - 4u, 12u, 0xAA);
    for (uint32_t i = 0; i < 4u; ++i) ram[pointer + i] = 0xCC;
    ff_av_query_arguments arguments = query(pointer);
    ff_av_query_config value = config(word);
    assert(ff_av_query_write_result(&vm, &arguments, &value) == FF_AV_QUERY_WROTE);
    assert(ram[pointer + 0] == b0 && ram[pointer + 1] == b1);
    assert(ram[pointer + 2] == b2 && ram[pointer + 3] == b3);
    assert_window(pointer - 4u, 4u, 0xAA);
    assert_window(pointer + 4u, 4u, 0xAA);
}

static void expect_denied(uint32_t pointer, uint32_t word)
{
    const int bounded = pointer >= 4u && (uint64_t)pointer + 8u <= sizeof ram;
    if (bounded) fill_window(pointer - 4u, 12u, 0xAA);
    ff_av_query_arguments arguments = query(pointer);
    ff_av_query_config value = config(word);
    assert(ff_av_query_write_result(&vm, &arguments, &value) == FF_AV_QUERY_DENIED);
    if (bounded) assert_window(pointer - 4u, 12u, 0xAA);
}

static void expect_denied_without_config(uint32_t pointer)
{
    assert(pointer >= 4u && (uint64_t)pointer + 8u <= sizeof ram);
    fill_window(pointer - 4u, 12u, 0xAA);
    ff_av_query_arguments arguments = query(pointer);
    assert(ff_av_query_write_result(&vm, &arguments, NULL) == FF_AV_QUERY_DENIED);
    assert_window(pointer - 4u, 12u, 0xAA);
}

int main(void)
{
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, READ_ONLY_BASE, REGION_END - READ_ONLY_BASE) == 0);
    assert(ff_vm_map_fixed(&vm, READ_ONLY_BASE, READ_WRITE_BASE - READ_ONLY_BASE, 0x02u) == 0);
    assert(ff_vm_map_fixed(&vm, READ_WRITE_BASE, ONE_PAGE_BASE - READ_WRITE_BASE, 0x04u) == 0);
    assert(ff_vm_map_fixed(&vm, ONE_PAGE_BASE, FF_VM_PAGE, 0x04u) == 0);
    assert(ff_vm_map_fixed(&vm, TWO_PAGE_BASE, 2u * FF_VM_PAGE, 0x04u) == 0);
    assert(ff_vm_map_fixed(&vm, VM_END_PAGE, FF_VM_PAGE, 0x04u) == 0);

    /* Two distinct DWORDs, including one unaligned pointer. Each store is
     * bytewise little-endian and the four surrounding bytes on both sides stay
     * unchanged. */
    expect_write(READ_WRITE_BASE, 0x00400104u, 0x04, 0x01, 0x40, 0x00);
    expect_write(READ_WRITE_BASE + 1u, 0x00800301u, 0x01, 0x03, 0x80, 0x00);
    expect_write(READ_WRITE_BASE + 16u, 0xFFFFFFFFu, 0xFF, 0xFF, 0xFF, 0xFF);
    expect_write(READ_WRITE_BASE + 32u, 0x00000000u, 0x00, 0x00, 0x00, 0x00);

    /* A page-crossing four-byte span succeeds when both pages are committed;
     * the unaligned span is checked as a whole, not one page at a time. */
    expect_write(TWO_PAGE_BASE + FF_VM_PAGE - 2u, 0x11223344u, 0x44, 0x33, 0x22, 0x11);

    /* A null config means the synthetic DWORD is absent; it is not a zero
     * default. An explicit zero config writes zero bytes. */
    expect_denied_without_config(READ_WRITE_BASE + 48u);

    /* Arguments outside the observed numeric-6 shape are denied unchanged. */
    fill_window(READ_WRITE_BASE, 12u, 0xAA);
    ff_av_query_arguments bad = query(READ_WRITE_BASE + 4u);
    ff_av_query_config value = config(0x11223344u);
    bad.option = FF_AV_QUERY_NUMERIC_OPTION - 1u;
    assert(ff_av_query_write_result(&vm, &bad, &value) == FF_AV_QUERY_DENIED);
    assert_window(READ_WRITE_BASE, 12u, 0xAA);

    bad = query(READ_WRITE_BASE + 4u);
    bad.register_base = 1u;
    assert(ff_av_query_write_result(&vm, &bad, &value) == FF_AV_QUERY_DENIED);
    assert_window(READ_WRITE_BASE, 12u, 0xAA);

    bad = query(READ_WRITE_BASE + 4u);
    bad.param = 1u;
    assert(ff_av_query_write_result(&vm, &bad, &value) == FF_AV_QUERY_DENIED);
    assert_window(READ_WRITE_BASE, 12u, 0xAA);

    /* Read-only, unmapped, one-page-crossing and VM-end-crossing spans are
     * denied without a partial write. */
    expect_denied(READ_ONLY_BASE, 0x00400104u);
    expect_denied(UNMAPPED_BASE, 0x00400104u);
    expect_denied(ONE_PAGE_BASE + FF_VM_PAGE - 2u, 0x00400104u);

    fill_window(VM_END_PAGE + FF_VM_PAGE - 4u, 4u, 0xAA);
    expect_denied(VM_END_PAGE + FF_VM_PAGE - 2u, 0x00400104u);
    assert_window(VM_END_PAGE + FF_VM_PAGE - 4u, 4u, 0xAA);

    /* A 32-bit wrap would cross the end of the VM; the checked span is denied
     * and the component never touches memory. */
    expect_denied(0xFFFFFFFEu, 0x00400104u);
    expect_denied(0xFFFFFFFFu, 0x00400104u);

    /* Null VM, arguments and config all fail closed with no write. */
    ff_av_query_arguments valid = query(READ_WRITE_BASE + 64u);
    expect_denied_without_config(READ_WRITE_BASE + 64u);
    assert(ff_av_query_write_result(NULL, &valid, &value) == FF_AV_QUERY_DENIED);
    assert(ff_av_query_write_result(&vm, NULL, &value) == FF_AV_QUERY_DENIED);

    puts("unconnected numeric-6 query component: nullable config, bytewise LE, unaligned/whole-span/wrap and neighbor checks passed");
    return 0;
}
