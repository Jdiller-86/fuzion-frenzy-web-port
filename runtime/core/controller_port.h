#ifndef FF_CONTROLLER_PORT_H
#define FF_CONTROLLER_PORT_H
#include <stdint.h>
#include "controller_packet.h"

/* One bounded Xbox controller port: connect/disconnect state, guest packet
 * encoding and a query result matching the pinned Win32 compatibility constant
 * ERROR_DEVICE_NOT_CONNECTED (1167). It does not poll a host device, enumerate
 * ports or implement rumble/async requests. */
#define FF_CONTROLLER_NOT_CONNECTED 1167u

typedef struct {
    ff_controller_state state;
} ff_controller_port;

void ff_controller_port_init(ff_controller_port *);
/* Apply an explicit snapshot; see ff_controller_update. */
uint32_t ff_controller_port_set(ff_controller_port *, uint8_t connected, uint16_t buttons,
                                const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT],
                                int16_t lx, int16_t ly, int16_t rx, int16_t ry);
/* Encode the current state. Returns 0, FF_CONTROLLER_NOT_CONNECTED for a
 * disconnected port, or FF_BOOT_INVALID for null/short output. Nothing is
 * written unless the query succeeds. */
uint32_t ff_controller_port_query(const ff_controller_port *, uint8_t *out, uint32_t bytes);
#endif
