#include "../../runtime/core/local_disc.h"
#include "../../runtime/core/thread_bootstrap.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[1024u * 1024u];
static ff_virtual_memory vm;
static ff_object_namespace ns;
static ff_local_disc disc;

static void put16(uint32_t at, uint16_t value)
{ ram[at] = (uint8_t)value; ram[at + 1] = (uint8_t)(value >> 8); }
static void put32(uint32_t at, uint32_t value)
{ for (unsigned i = 0; i < 4; ++i) ram[at + i] = (uint8_t)(value >> (8 * i)); }
static void counted_string(uint32_t descriptor, uint32_t buffer, const char *value, uint16_t length)
{ put16(descriptor, length); put16(descriptor + 2, length); put32(descriptor + 4, buffer); memcpy(ram + buffer, value, length); }

static int require(int ok, const char *label)
{ if (!ok) fprintf(stderr, "FAIL %s\n", label); return ok; }

int main(void)
{
    int ok = 1;
    ok &= require(!ff_vm_init(&vm, ram, sizeof ram), "vm init");
    ok &= require(!ff_vm_claim_fixed(&vm, 0x10000, 0x10000), "vm claim");
    ok &= require(!ff_vm_map_fixed(&vm, 0x10000, 0x10000, 0x40), "vm map");
    ok &= require(!ff_namespace_init(&ns, &vm), "namespace init");
    uint32_t device = 0;
    ok &= require(!ff_namespace_add_device(&ns, "\\Device\\CdRom0", 14, FF_NS_LOCAL_DISC, &device), "device init");
    uint32_t link_id = 0;
    counted_string(0x11200, 0x11300, "\\??\\D:", 6);
    counted_string(0x11210, 0x11310, "\\Device\\CdRom0", 14);
    ok &= require(!ff_namespace_create_link(&ns, 0x11200, 0x11210, &link_id), "D drive link");
    memset(&disc, 0, sizeof disc);
    ok &= require(!ff_disc_bind(&disc, &ns, 77, 34u * 2048u), "bind");
    ok &= require(ff_disc_bind(&disc, &ns, 78, 34u * 2048u) == FF_BOOT_UNSUPPORTED, "duplicate bind");
    ok &= require(ns.objects[device - 1].provider_object == 77, "provider retained");

    const uint32_t output = 0x11000, attributes = 0x11010, iosb = 0x11030;
    const uint32_t descriptor = 0x11020, name = 0x11100;
    memcpy(ram + name, "d:\\a", 4);
    put32(attributes, 0xfffffffdu); put32(attributes + 4, descriptor); put32(attributes + 8, 0x40);
    put16(descriptor, 4); put16(descriptor + 2, 4); put32(descriptor + 4, name);
    memset(ram + output, 0xa5, 4); memset(ram + iosb, 0x5a, 8);
    const uint32_t args[9] = {output, 0x40100080, attributes, iosb, 0, 0x80, 3, 5, 0x60};
    ok &= require(ff_disc_create(&disc, args) == 0xc0000022u, "read-only denial status");
    ok &= require(ram[output] == 0xa5 && ram[output + 3] == 0xa5, "handle output unchanged");
    ok &= require(ram[iosb] == 0x22 && ram[iosb + 1] == 0 && ram[iosb + 2] == 0 && ram[iosb + 3] == 0xc0 &&
                  ram[iosb + 4] == 0 && ram[iosb + 7] == 0 && disc.denied == 1, "iosb and denial count");

    memset(ram + iosb, 0x5a, 8);
    put32(descriptor + 4, 0x20000);
    ok &= require(ff_disc_create(&disc, args) == FF_VM_ACCESS_VIOLATION && ram[iosb] == 0x5a, "invalid name vm atomic");
    put32(descriptor + 4, name);
    put32(attributes, 0);
    ok &= require(ff_disc_create(&disc, args) == FF_BOOT_UNSUPPORTED && ram[iosb] == 0x5a, "unsupported shape atomic");
    put32(attributes, 0xfffffffdu);

    /* Output aliases the IOSB: reject before either output is touched. */
    uint32_t alias_args[9]; memcpy(alias_args, args, sizeof args); alias_args[3] = output;
    ok &= require(ff_disc_create(&disc, alias_args) == FF_BOOT_UNSUPPORTED && ram[output] == 0xa5,
                  "output alias atomic");

    /* Repoint D: at a different valid optical device; the physical binding
     * alone must not authorize access through the wrong namespace identity. */
    uint32_t foreign_id = 0;
    ok &= require(!ff_namespace_add_device(&ns, "\\Device\\Other", 13, FF_NS_LOCAL_DISC, &foreign_id), "foreign device");
    ok &= require(!ff_namespace_delete_link(&ns, "\\??\\D:", 6), "remove D link");
    memset(ram + iosb, 0x5a, 8);
    ok &= require(ff_disc_create(&disc, args) == FF_BOOT_UNSUPPORTED && ram[iosb] == 0x5a,
                  "missing namespace device atomic");
    counted_string(0x11210, 0x11310, "\\Device\\Other", 13);
    ok &= require(!ff_namespace_create_link(&ns, 0x11200, 0x11210, &link_id), "wrong D link");
    memset(ram + iosb, 0x5a, 8);
    ok &= require(ff_disc_create(&disc, args) == FF_BOOT_UNSUPPORTED && ram[iosb] == 0x5a,
                  "wrong namespace device atomic");

    ff_namespace_object unrelated = {0};
    unrelated.id = 99; unrelated.kind = FF_NS_DEVICE; unrelated.device_kind = FF_NS_LOCAL_DISC;
    unrelated.media_ready = 1; unrelated.provider_object = 88; ns.objects[FF_NS_OBJECTS - 1] = unrelated;
    ff_disc_reset(&disc);
    ok &= require(!ns.objects[device - 1].media_ready && !ns.objects[device - 1].provider_object,
                  "owned binding reset");
    ok &= require(ns.objects[FF_NS_OBJECTS - 1].media_ready == 1 && ns.objects[FF_NS_OBJECTS - 1].provider_object == 88,
                  "foreign device preserved");
    puts(ok ? "local disc checks passed" : "local disc checks failed");
    return ok ? 0 : 1;
}
