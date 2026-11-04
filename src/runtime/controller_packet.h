#ifndef FF_CONTROLLER_PACKET_H
#define FF_CONTROLLER_PACKET_H
#include <stdint.h>

/* Bounded Xbox controller state and guest-facing packet encoder.
 *
 * Field order follows the pinned compatibility header (src/input/
 * xinput_xbox.h): DWORD dwPacketNumber, WORD wButtons, BYTE bAnalogButtons[8],
 * SHORT sThumbLX/LY/RX/RY. The guest-facing state is laid out explicitly at
 * 24 bytes: 4 + 2 + 8 + 8 + 2 trailing alignment bytes. This is a packet/state
 * component only: no host gamepad mapping, rumble, polling loop or boot-path
 * integration. */
#define FF_CONTROLLER_PACKET_BYTES 24u
#define FF_CONTROLLER_ANALOG_COUNT 8u

typedef struct {
    uint32_t packet;
    uint16_t buttons;
    uint8_t analog[FF_CONTROLLER_ANALOG_COUNT];
    int16_t lx, ly, rx, ry;
    uint8_t connected;
} ff_controller_state;

void ff_controller_init(ff_controller_state *);
/* Apply one explicit snapshot. The packet number advances only when a field
 * that reaches the guest changes. Returns 0 or FF_BOOT_INVALID on null state. */
uint32_t ff_controller_update(ff_controller_state *, uint8_t connected, uint16_t buttons,
                              const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT],
                              int16_t lx, int16_t ly, int16_t rx, int16_t ry);
/* Encode the current state into 24 checked packet bytes. Disconnected states
 * are rejected before writing anything. */
uint32_t ff_controller_encode(const ff_controller_state *, uint8_t *out, uint32_t bytes);
#endif
