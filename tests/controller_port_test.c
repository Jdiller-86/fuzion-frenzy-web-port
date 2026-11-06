/* Owned controller port query scenarios; no host device or original code. */
#include "../runtime/core/controller_port.h"
#include "../runtime/core/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    ff_controller_port port, second;
    uint8_t analog[FF_CONTROLLER_ANALOG_COUNT] = {0};
    uint8_t packet[FF_CONTROLLER_PACKET_BYTES], untouched[FF_CONTROLLER_PACKET_BYTES];
    memset(&port, 0xa5, sizeof port);
    ff_controller_port_init(&port);
    assert(!port.state.connected && port.state.packet == 0);

    memset(packet, 0x5a, sizeof packet);
    memcpy(untouched, packet, sizeof packet);
    assert(ff_controller_port_query(&port, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
    assert(memcmp(packet, untouched, sizeof packet) == 0);
    assert(ff_controller_port_query(NULL, packet, sizeof packet) == FF_BOOT_INVALID);
    assert(ff_controller_port_query(&port, NULL, sizeof packet) == FF_BOOT_INVALID);
    assert(ff_controller_port_query(&port, packet, FF_CONTROLLER_PACKET_BYTES - 1) == FF_BOOT_INVALID);

    assert(ff_controller_port_set(&port, 1, 0, analog, 0, 0, 0, 0) == 0);
    assert(port.state.connected && port.state.packet == 1);
    assert(ff_controller_port_query(&port, packet, sizeof packet) == 0);
    assert(packet[0] == 1 && packet[1] == 0);

    assert(ff_controller_port_set(&port, 1, 0, analog, 0, 0, 0, 0) == 0);
    assert(port.state.packet == 1);

    analog[0] = 255;
    assert(ff_controller_port_set(&port, 1, 0x0010, analog, 32767, -32768, 0, 0) == 0);
    assert(port.state.packet == 2);
    assert(ff_controller_port_query(&port, packet, sizeof packet) == 0);
    assert(packet[4] == 0x10 && packet[6] == 255);

    ff_controller_port_init(&second);
    assert(ff_controller_port_set(&second, 1, 0, analog, 0, 0, 0, 0) == 0);
    assert(second.state.packet == 1 && port.state.packet == 2);

    assert(ff_controller_port_set(&port, 0, 0, analog, 0, 0, 0, 0) == 0);
    assert(ff_controller_port_query(&port, packet, sizeof packet) == FF_CONTROLLER_NOT_CONNECTED);
    assert(ff_controller_port_set(NULL, 1, 0, analog, 0, 0, 0, 0) == FF_BOOT_INVALID);
    puts("owned controller port: disconnected query, connect, packet changes, independence and rejection passed");
    return 0;
}
