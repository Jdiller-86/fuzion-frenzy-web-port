#ifndef FF_DEVICE_REGISTERS_H
#define FF_DEVICE_REGISTERS_H
#include <stdint.h>

/* Bounded synthetic device aperture for reached original register accesses.
 * Not an NV2A model: every address starts unknown unless the selected profile
 * supplies a value; unknown reads fault instead of returning a fabricated 0. */
#define FF_DEVICE_ENTRIES 1024u
#define FF_DEVICE_EVENTS 4096u
#define FF_DEVICE_READ 1u
#define FF_DEVICE_WRITE 2u

typedef struct {
    uint32_t address, value, reads, writes, known;
} ff_device_entry;
typedef struct {
    uint32_t address, site, direction, value, before_known;
} ff_device_event;
typedef struct {
    uint32_t base, bytes, ready, count, event_count;
    ff_device_entry entries[FF_DEVICE_ENTRIES];
    ff_device_event events[FF_DEVICE_EVENTS];
} ff_device_registers;

/* The window and the two explicitly supplied initial values are profile inputs. */
uint32_t ff_device_registers_init(ff_device_registers *, uint32_t base, uint32_t bytes,
                                  uint32_t first_address, uint32_t first_value,
                                  uint32_t second_address, uint32_t second_value);
uint32_t ff_device_registers_contains(const ff_device_registers *, uint32_t address, uint32_t bytes);
/* Reads fail atomically when the address was never written or supplied. */
uint32_t *ff_device_registers_slot(ff_device_registers *, uint32_t address);
uint32_t ff_device_registers_touch(ff_device_registers *, uint32_t address, uint32_t direction,
                                   uint32_t site, uint32_t *value);
#endif
