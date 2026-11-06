#ifndef FF_CONTROLLER_DEVICE_H
#define FF_CONTROLLER_DEVICE_H
#include <stdint.h>
#include "controller_port.h"

/* Four bounded controller seats matching the pinned XBOX_MAX_CONTROLLERS.
 * Each seat composes one controller port; there is no host polling, port
 * enumeration, rumble or asynchronous request handling. An out-of-range port
 * returns FF_CONTROLLER_NOT_CONNECTED, matching the pinned layer's treatment
 * of a port beyond its four controllers. */
#define FF_CONTROLLER_MAX_PORTS 4u

typedef struct {
    ff_controller_port ports[FF_CONTROLLER_MAX_PORTS];
} ff_controller_device;

void ff_controller_device_init(ff_controller_device *);
/* Apply an explicit snapshot to one seat. Returns 0, FF_CONTROLLER_NOT_CONNECTED
 * for an out-of-range port, or FF_BOOT_INVALID for a null device. */
uint32_t ff_controller_device_set_port(ff_controller_device *, uint32_t port, uint8_t connected,
                                       uint16_t buttons, const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT],
                                       int16_t lx, int16_t ly, int16_t rx, int16_t ry);
/* Query one seat. Returns 0 and the checked 24-byte packet, FF_CONTROLLER_NOT_CONNECTED
 * for a disconnected or out-of-range port, or FF_BOOT_INVALID for null/short output.
 * Nothing is written unless the query succeeds. */
uint32_t ff_controller_device_query(const ff_controller_device *, uint32_t port, uint8_t *out, uint32_t bytes);
#endif
