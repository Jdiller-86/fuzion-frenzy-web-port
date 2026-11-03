/* Owned Xbox controller packet/state scenarios; no original code or device. */
#include "../src/runtime/controller_packet.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint16_t get16(const uint8_t *p) { return (uint16_t)(p[0] | (p[1] << 8)); }
static uint32_t get32(const uint8_t *p) { return (uint32_t)get16(p) | ((uint32_t)get16(p + 2) << 16); }

int main(void)
{
    ff_controller_state state, saved;
    uint8_t analog[FF_CONTROLLER_ANALOG_COUNT] = {0};
    uint8_t packet[FF_CONTROLLER_PACKET_BYTES], saved_packet[FF_CONTROLLER_PACKET_BYTES];
    memset(&state, 0xa5, sizeof state);
    ff_controller_init(&state);
    assert(state.packet == 0 && !state.connected);

    assert(ff_controller_update(&state, 1, 0, analog, 0, 0, 0, 0) == 0);
    assert(state.connected && state.packet == 1);
    assert(ff_controller_encode(&state, packet, sizeof packet) == 0);
    assert(get32(packet) == 1 && get16(packet + 4) == 0);
    for (unsigned i = 6; i < 14; ++i) assert(packet[i] == 0);
    assert(get16(packet + 14) == 0 && get16(packet + 22) == 0 && get16(packet + 22) == 0);
    assert(packet[22] == 0 && packet[23] == 0);

    analog[0] = 255; analog[7] = 30;
    assert(ff_controller_update(&state, 1, 0x0001 | 0x0010, analog, -32768, 32767, -1, 12345) == 0);
    assert(state.packet == 2);
    assert(ff_controller_encode(&state, packet, sizeof packet) == 0);
    assert(get16(packet + 4) == 0x0011);
    assert(packet[6] == 255 && packet[13] == 30);
    assert(get16(packet + 14) == 0x8000);
    assert(get16(packet + 16) == 0x7FFF);
    assert(get16(packet + 18) == 0xFFFF);
    assert(get16(packet + 20) == 12345);

    saved = state; memcpy(saved_packet, packet, sizeof packet);
    assert(ff_controller_update(&state, 1, 0x0001 | 0x0010, analog, -32768, 32767, -1, 12345) == 0);
    assert(state.packet == 2 && memcmp(&state, &saved, sizeof state) == 0);

    assert(ff_controller_update(&state, 0, 0, analog, 0, 0, 0, 0) == 0);
    assert(!state.connected && state.packet == 3);
    assert(ff_controller_encode(&state, packet, sizeof packet) == FF_BOOT_INVALID);
    assert(memcmp(packet, saved_packet, sizeof packet) == 0);

    assert(ff_controller_update(NULL, 1, 0, analog, 0, 0, 0, 0) == FF_BOOT_INVALID);
    assert(ff_controller_encode(NULL, packet, sizeof packet) == FF_BOOT_INVALID);
    assert(ff_controller_update(&state, 1, 0, NULL, 0, 0, 0, 0) == FF_BOOT_INVALID);
    assert(ff_controller_encode(&saved, NULL, sizeof packet) == FF_BOOT_INVALID);
    assert(ff_controller_encode(&saved, packet, FF_CONTROLLER_PACKET_BYTES - 1) == FF_BOOT_INVALID);
    puts("owned controller packet: neutral, buttons, axis extremes, packet changes, disconnect and rejection passed");
    return 0;
}
