/* Owned four-port controller device scenarios; no host device or original code. */
#include "../runtime/core/controller_device.h"
#include "../runtime/core/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint16_t get16(const uint8_t *p) { return (uint16_t)(p[0] | (p[1] << 8)); }
static uint32_t get32(const uint8_t *p) { return (uint32_t)get16(p) | ((uint32_t)get16(p + 2) << 16); }

static void expect_untouched(const uint8_t *packet)
{
    for (unsigned i = 0; i < FF_CONTROLLER_PACKET_BYTES; ++i) assert(packet[i] == 0x5a);
}

int main(void)
{
    ff_controller_device device;
    uint8_t analog[FF_CONTROLLER_ANALOG_COUNT] = {0};
    uint8_t packet[FF_CONTROLLER_PACKET_BYTES];

    memset(&device, 0xa5, sizeof device);
    ff_controller_device_init(&device);
    for (uint32_t port = 0; port < FF_CONTROLLER_MAX_PORTS; ++port) {
        memset(packet, 0x5a, sizeof packet);
        assert(ff_controller_device_query(&device, port, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
        expect_untouched(packet);
    }

    assert(ff_controller_device_set_port(&device, 0, 1, 0, analog, 0, 0, 0, 0) == 0);
    assert(ff_controller_device_query(&device, 0, packet, sizeof packet) == 0);
    assert(get32(packet) == 1 && get16(packet + 4) == 0);
    for (uint32_t port = 1; port < FF_CONTROLLER_MAX_PORTS; ++port) {
        memset(packet, 0x5a, sizeof packet);
        assert(ff_controller_device_query(&device, port, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
        expect_untouched(packet);
    }

    analog[0] = 255;
    assert(ff_controller_device_set_port(&device, 3, 1, 0x0010, analog, 32767, -32768, 0, 0) == 0);
    assert(ff_controller_device_query(&device, 3, packet, sizeof packet) == 0);
    assert(get32(packet) == 1 && get16(packet + 4) == 0x0010 && packet[6] == 255);

    assert(ff_controller_device_set_port(&device, 0, 1, 0x0001, analog, 0, 0, 0, 0) == 0);
    assert(ff_controller_device_query(&device, 0, packet, sizeof packet) == 0);
    assert(get32(packet) == 2 && get16(packet + 4) == 0x0001);
    assert(ff_controller_device_query(&device, 3, packet, sizeof packet) == 0);
    assert(get32(packet) == 1 && get16(packet + 4) == 0x0010);

    memset(packet, 0x5a, sizeof packet);
    assert(ff_controller_device_query(&device, FF_CONTROLLER_MAX_PORTS, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
    expect_untouched(packet);
    assert(ff_controller_device_query(&device, 0xffffffffu, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
    expect_untouched(packet);
    assert(ff_controller_device_set_port(&device, FF_CONTROLLER_MAX_PORTS, 1, 0, analog, 0, 0, 0, 0) == FF_CONTROLLER_NOT_CONNECTED);

    assert(ff_controller_device_set_port(&device, 0, 0, 0, analog, 0, 0, 0, 0) == 0);
    memset(packet, 0x5a, sizeof packet);
    assert(ff_controller_device_query(&device, 0, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
    expect_untouched(packet);
    assert(ff_controller_device_query(&device, 3, packet, sizeof packet) == 0);

    ff_controller_device_init(NULL);
    assert(ff_controller_device_set_port(NULL, 0, 1, 0, analog, 0, 0, 0, 0) == FF_BOOT_INVALID);
    assert(ff_controller_device_query(NULL, 0, packet, sizeof packet) == FF_BOOT_INVALID);
    assert(ff_controller_device_set_port(&device, 0, 1, 0, NULL, 0, 0, 0, 0) == FF_BOOT_INVALID);
    assert(ff_controller_device_query(&device, 0, NULL, sizeof packet) == FF_BOOT_INVALID);
    assert(ff_controller_device_query(&device, 0, packet, FF_CONTROLLER_PACKET_BYTES - 1) == FF_BOOT_INVALID);
    puts("owned controller device: four seats, independence, out-of-range, disconnect and rejection passed");
    return 0;
}
