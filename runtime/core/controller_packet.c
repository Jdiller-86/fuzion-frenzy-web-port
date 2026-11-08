#include "controller_packet.h"
#include "thread_bootstrap.h"
#include <string.h>

void ff_controller_init(ff_controller_state *state)
{
    if (state) memset(state, 0, sizeof *state);
}

uint32_t ff_controller_update(ff_controller_state *state, uint8_t connected, uint16_t buttons,
                              const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT],
                              int16_t lx, int16_t ly, int16_t rx, int16_t ry)
{
    if (!state || !analog) return FF_BOOT_INVALID;
    uint8_t next[FF_CONTROLLER_ANALOG_COUNT] = {0};
    if (analog) memcpy(next, analog, sizeof next);
    int changed = state->connected != connected || state->buttons != buttons ||
                  state->lx != lx || state->ly != ly || state->rx != rx || state->ry != ry ||
                  memcmp(state->analog, next, sizeof next) != 0;
    state->connected = connected;
    state->buttons = buttons;
    memcpy(state->analog, next, sizeof next);
    state->lx = lx; state->ly = ly; state->rx = rx; state->ry = ry;
    if (changed) ++state->packet;
    return 0;
}

static void put16(uint8_t *p, uint16_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }
static void put32(uint8_t *p, uint32_t v) { put16(p, (uint16_t)v); put16(p + 2, (uint16_t)(v >> 16)); }

uint32_t ff_controller_encode(const ff_controller_state *state, uint8_t *out, uint32_t bytes)
{
    if (!state || !out || bytes < FF_CONTROLLER_PACKET_BYTES || !state->connected)
        return FF_BOOT_INVALID;
    memset(out, 0, FF_CONTROLLER_PACKET_BYTES);
    put32(out, state->packet);
    put16(out + 4, state->buttons);
    memcpy(out + 6, state->analog, FF_CONTROLLER_ANALOG_COUNT);
    put16(out + 14, (uint16_t)state->lx);
    put16(out + 16, (uint16_t)state->ly);
    put16(out + 18, (uint16_t)state->rx);
    put16(out + 20, (uint16_t)state->ry);
    return 0;
}
