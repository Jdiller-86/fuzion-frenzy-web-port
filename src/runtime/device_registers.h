#ifndef FF_DEVICE_REGISTERS_H
#define FF_DEVICE_REGISTERS_H
#include <stdint.h>

/* Explicit synthetic register storage for the reached device helper.
 * This is not an NV2A model, not console state and not a hardware default:
 * every initial value is supplied by the selected profile and recorded. */
#define FF_DEVICE_REGISTERS 2u
#define FF_DEVICE_EVENTS 3u
#define FF_DEVICE_READ 1u
#define FF_DEVICE_WRITE 2u

typedef struct { uint32_t address, initial, value, reads, writes; } ff_device_register;
typedef struct { uint32_t address, site, direction, old_value, new_value; } ff_device_event;
typedef struct {
    ff_device_register registers[FF_DEVICE_REGISTERS];
    ff_device_event events[FF_DEVICE_EVENTS];
    uint32_t event_count, ready;
} ff_device_registers;

/* Both entries are required, page/word aligned, distinct and explicitly supplied. */
uint32_t ff_device_registers_init(ff_device_registers *, uint32_t first_address, uint32_t first_value,
                                  uint32_t second_address, uint32_t second_value);
uint32_t ff_device_registers_lookup(const ff_device_registers *, uint32_t address, uint32_t bytes);
uint32_t ff_device_registers_touch(ff_device_registers *, uint32_t address, uint32_t direction,
                                   uint32_t site, uint32_t *value);
uint32_t ff_device_registers_done(const ff_device_registers *);
#endif
