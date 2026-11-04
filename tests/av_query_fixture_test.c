/* Native test for the unconnected numeric-6 query component.
 *
 * The test calls the real src/runtime/av_query.c component against a real
 * ff_virtual_memory window. It does not reimplement the writer. Two distinct
 * synthetic DWORDs must be written verbatim, and every denial must leave the
 * entire checked span unchanged.
 */
#include "../src/runtime/av_query.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_ONLY_BASE 0x00010000u
#define READ_WRITE_BASE 0x00020000u
#define ONE_PAGE_BASE 0x00030000u
#define UNMAPPED_BASE 0x00040000u
#define VM_END_PAGE 0x0004F000u
#define REGION_END 0x00050000u

static uint8_t ram[REGION_END];
static ff_virtual_memory vm;

static ff_av_query_arguments query(uint32_t pointer, uint32_t word)
{
    ff_av_query_arguments arguments;
    arguments.register_base = 0u;
    arguments.option = FF_AV_QUERY_NUMERIC_OPTION;
    arguments.param = 0u;
    arguments.result_pointer = pointer;
    arguments.word = word;
    return arguments;
}

static void fill(uint32_t base, uint8_t value)
{
    memset(ram + base, value, 8);
}

static void assert_span(uint32_t base, uint8_t value)
{
    for (uint32_t i = 0; i < 8; ++i) assert(ram[base + i] == value);
}

static void assert_denied(uint32_t pointer, uint32_t word)
{
    ff_av_query_arguments arguments = query(pointer, word);
    assert(ff_av_query_write_result(&vm, &arguments) == FF_AV_QUERY_DENIED);
}

int main(void)
{
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, READ_ONLY_BASE, REGION_END - READ_ONLY_BASE) == 0);
    assert(ff_vm_map_fixed(&vm, READ_ONLY_BASE, READ_WRITE_BASE - READ_ONLY_BASE, 0x02u) == 0);
    assert(ff_vm_map_fixed(&vm, READ_WRITE_BASE, ONE_PAGE_BASE - READ_WRITE_BASE, 0x04u) == 0);
    assert(ff_vm_map_fixed(&vm, ONE_PAGE_BASE, FF_VM_PAGE, 0x04u) == 0);
    assert(ff_vm_map_fixed(&vm, VM_END_PAGE, FF_VM_PAGE, 0x04u) == 0);

    /* Two distinct synthetic DWORDs are written exactly in little-endian order,
     * and each granted write leaves its neighbours untouched. */
    ff_av_query_arguments first = query(READ_WRITE_BASE, 0x00400104u);
    fill(READ_WRITE_BASE, 0xAA);
    assert(ff_av_query_write_result(&vm, &first) == FF_AV_QUERY_WROTE);
    assert(ram[READ_WRITE_BASE + 0] == 0x04);
    assert(ram[READ_WRITE_BASE + 1] == 0x01);
    assert(ram[READ_WRITE_BASE + 2] == 0x40);
    assert(ram[READ_WRITE_BASE + 3] == 0x00);
    for (uint32_t i = 4; i < 8; ++i) assert(ram[READ_WRITE_BASE + i] == 0xAA);

    ff_av_query_arguments second = query(READ_WRITE_BASE + 8u, 0x00800301u);
    fill(READ_WRITE_BASE + 8u, 0xAA);
    assert(ff_av_query_write_result(&vm, &second) == FF_AV_QUERY_WROTE);
    assert(ram[READ_WRITE_BASE + 8] == 0x01);
    assert(ram[READ_WRITE_BASE + 9] == 0x03);
    assert(ram[READ_WRITE_BASE + 10] == 0x80);
    assert(ram[READ_WRITE_BASE + 11] == 0x00);
    for (uint32_t i = 12; i < 16; ++i) assert(ram[READ_WRITE_BASE + i] == 0xAA);
    assert(ram[READ_WRITE_BASE + 0] == 0x04 && ram[READ_WRITE_BASE + 3] == 0x00);

    /* The component rejects arguments outside the observed numeric-6 shape and
     * keeps the backing span unchanged. */
    fill(READ_WRITE_BASE, 0xAA);
    ff_av_query_arguments bad = query(READ_WRITE_BASE, 0x11223344u);
    bad.option = FF_AV_QUERY_NUMERIC_OPTION - 1u;
    assert(ff_av_query_write_result(&vm, &bad) == FF_AV_QUERY_DENIED);
    assert_span(READ_WRITE_BASE, 0xAA);

    bad = query(READ_WRITE_BASE, 0x11223344u);
    bad.register_base = 1u;
    assert(ff_av_query_write_result(&vm, &bad) == FF_AV_QUERY_DENIED);
    assert_span(READ_WRITE_BASE, 0xAA);

    bad = query(READ_WRITE_BASE, 0x11223344u);
    bad.param = 1u;
    assert(ff_av_query_write_result(&vm, &bad) == FF_AV_QUERY_DENIED);
    assert_span(READ_WRITE_BASE, 0xAA);

    /* Read-only, unaligned, null, unmapped and whole-span-crossing pointers are
     * denied with no partial write. */
    fill(READ_ONLY_BASE, 0xAA);
    assert_denied(READ_ONLY_BASE, 0x00400104u);
    assert_span(READ_ONLY_BASE, 0xAA);

    fill(READ_WRITE_BASE, 0xAA);
    assert_denied(READ_WRITE_BASE + 1u, 0x00400104u);
    assert_span(READ_WRITE_BASE, 0xAA);

    assert_denied(0u, 0x00400104u);

    fill(UNMAPPED_BASE, 0xAA);
    assert_denied(UNMAPPED_BASE, 0x00400104u);
    assert_span(UNMAPPED_BASE, 0xAA);

    /* Two bytes fall in the one committed page, two in the next uncommitted
     * page: the whole four-byte span is checked before writing. */
    fill(ONE_PAGE_BASE + FF_VM_PAGE - 4u, 0xAA);
    assert_denied(ONE_PAGE_BASE + FF_VM_PAGE - 2u, 0x00400104u);
    assert_span(ONE_PAGE_BASE + FF_VM_PAGE - 4u, 0xAA);

    /* Two bytes fall inside the last committed page and two beyond the VM's
     * declared size: the bounds check denies the whole span. */
    fill(VM_END_PAGE + FF_VM_PAGE - 8u, 0xAA);
    assert_denied(VM_END_PAGE + FF_VM_PAGE - 2u, 0x00400104u);
    assert_span(VM_END_PAGE + FF_VM_PAGE - 8u, 0xAA);

    /* The same page still accepts a query wholly inside its committed span. */
    ff_av_query_arguments inside = query(ONE_PAGE_BASE + FF_VM_PAGE - 4u, 0x00800301u);
    fill(ONE_PAGE_BASE + FF_VM_PAGE - 8u, 0xAA);
    assert(ff_av_query_write_result(&vm, &inside) == FF_AV_QUERY_WROTE);
    assert(ram[ONE_PAGE_BASE + FF_VM_PAGE - 4u + 0] == 0x01);
    assert(ram[ONE_PAGE_BASE + FF_VM_PAGE - 4u + 1] == 0x03);
    assert(ram[ONE_PAGE_BASE + FF_VM_PAGE - 4u + 2] == 0x80);
    assert(ram[ONE_PAGE_BASE + FF_VM_PAGE - 4u + 3] == 0x00);

    assert(ff_av_query_write_result(NULL, &first) == FF_AV_QUERY_DENIED);
    assert(ff_av_query_write_result(&vm, NULL) == FF_AV_QUERY_DENIED);

    puts("unconnected numeric-6 query component: two-value exact write and unchanged denial passed");
    return 0;
}
