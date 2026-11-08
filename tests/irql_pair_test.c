/* Independently authored bounded IRQL-pair fixture; no title code or data. */
#include "../runtime/core/irql_pair.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[0x40000];
static ff_boot_runtime boot;
static ff_virtual_memory vm;
static ff_irql_pair pair;

static void put32(uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) ram[address + i] = (uint8_t)(value >> (i * 8));
}

static void setup(void)
{
    memset(ram, 0, sizeof ram);
    memset(&boot, 0, sizeof boot);
    assert(ff_vm_init(&vm, ram, sizeof ram) == FF_BOOT_OK);
    assert(ff_vm_claim_fixed(&vm, 0x10000, 0x10000) == FF_BOOT_OK);
    /* Existing boot PCR memory is RW under the component's access contract. */
    assert(ff_vm_map_fixed(&vm, 0x10000, 0x10000, 4) == FF_BOOT_OK);
    boot.ram = ram; boot.bytes = sizeof ram; boot.current = 0; boot.count = 1;
    boot.threads[0] = (ff_boot_thread){.id = 1, .state = FF_BOOT_RUNNING,
        .object = 0x10000, .fs = 0x10200, .stack_low = 0x11000, .stack_top = 0x12000};
    put32(boot.threads[0].fs + 4, 0x12000);
    put32(boot.threads[0].fs + 8, 0x11000);
    ram[boot.threads[0].fs + 0x24] = 0;
    ff_irql_pair_init(&pair);
}

static void assert_atomic(uint32_t status, const uint8_t *before,
                          const ff_irql_pair *state, uint8_t output)
{
    assert(status != FF_BOOT_OK);
    assert(!memcmp(before, ram, sizeof ram));
    assert(!memcmp(state, &pair, sizeof pair));
    assert(output == 0xa5);
}

static void valid_pair(void)
{
    setup();
    uint8_t old = 0xa5;
    assert(ff_irql_pair_raise_to_dpc(&pair, &boot, &vm, &old) == FF_BOOT_OK);
    assert(old == 0 && ram[boot.threads[0].fs + 0x24] == 2 && pair.active && pair.thread_id == 1);
    assert(ff_irql_pair_lower_saved(&pair, &boot, &vm, old) == FF_BOOT_OK);
    assert(ram[boot.threads[0].fs + 0x24] == 0 && !pair.active && pair.raise_count == 1 && pair.lower_count == 1);
}

static void refusals_are_atomic(void)
{
    uint8_t before[sizeof ram]; ff_irql_pair saved; uint8_t out;
    setup();
    memcpy(before, ram, sizeof ram); saved = pair; out = 0xa5;
    boot.threads[0].id++;
    assert_atomic(ff_irql_pair_raise_to_dpc(&pair, &boot, &vm, &out), before, &saved, out);
    boot.threads[0].id--;
    boot.current++;
    assert_atomic(ff_irql_pair_raise_to_dpc(&pair, &boot, &vm, &out), before, &saved, out);
    boot.current--;
    vm.permission[boot.threads[0].fs / FF_VM_PAGE] = FF_VM_READ;
    memcpy(before, ram, sizeof ram); saved = pair; out = 0xa5;
    assert_atomic(ff_irql_pair_raise_to_dpc(&pair, &boot, &vm, &out), before, &saved, out);

    setup();
    out = 0xa5;
    assert(ff_irql_pair_raise_to_dpc(&pair, &boot, &vm, &out) == FF_BOOT_OK);
    memcpy(before, ram, sizeof ram); saved = pair; out = 0xa5;
    boot.threads[0].id++;
    assert_atomic(ff_irql_pair_lower_saved(&pair, &boot, &vm, 0), before, &saved, out);
    boot.threads[0].id--;
    assert_atomic(ff_irql_pair_lower_saved(&pair, &boot, &vm, 1), before, &saved, out);
    assert_atomic(ff_irql_pair_raise_to_dpc(&pair, &boot, &vm, &out), before, &saved, out);
}

int main(void)
{
    valid_pair();
    refusals_are_atomic();
    puts("IRQL pair: bounded PCR 0->2->0 and atomic permission/thread/value/nested refusals passed");
    return 0;
}
