#ifndef FF_DEVICE_REGISTERS_H
#define FF_DEVICE_REGISTERS_H
#include <stdint.h>

/* Bounded synthetic device aperture for reached original register accesses.
 * Not an NV2A model: every address starts unknown unless the selected profile
 * supplies a value; unknown reads fault instead of returning a fabricated 0. */
#define FF_DEVICE_ENTRIES 8192u
#define FF_DEVICE_EVENTS 8192u
#define FF_DEVICE_INITIAL 64u
#define FF_DEVICE_READ 1u
#define FF_DEVICE_WRITE 2u
/* Access masks mirror the translated memory intent: read, write, read-write. */
#define FF_DEVICE_ACCESS_READ 1u
#define FF_DEVICE_ACCESS_WRITE 2u
#define FF_DEVICE_ACCESS_READWRITE 3u

typedef struct {
    uint32_t address, value, reads, writes, known, known_bytes;
} ff_device_entry;
typedef struct {
    uint32_t address, site, direction, value, before_known;
} ff_device_event;
typedef struct {
    uint32_t base, bytes, ready, count, event_count;
    /* A generated store writes through a returned lvalue after the host call.
     * Keep one staged write so the event can record the actual stored value. */
    uint32_t pending, pending_address, pending_site, pending_value, pending_bytes, pending_offset;
    ff_device_entry entries[FF_DEVICE_ENTRIES];
    ff_device_event events[FF_DEVICE_EVENTS];
} ff_device_registers;
typedef struct { uint32_t address, value; } ff_device_init_entry;

/* The window and the explicitly supplied initial values are profile inputs. */
uint32_t ff_device_registers_init(ff_device_registers *, uint32_t base, uint32_t bytes,
                                  uint32_t first_address, uint32_t first_value,
                                  uint32_t second_address, uint32_t second_value);
uint32_t ff_device_registers_init_many(ff_device_registers *, uint32_t base, uint32_t bytes,
                                       const ff_device_init_entry *, uint32_t count);
/* Seed one additional explicit initial value before any access is recorded. */
uint32_t ff_device_registers_add_initial(ff_device_registers *, uint32_t address, uint32_t value);
uint32_t ff_device_registers_contains(const ff_device_registers *, uint32_t address, uint32_t bytes);
/* Reads fail atomically when the address was never written or supplied. */
uint32_t *ff_device_registers_slot(ff_device_registers *, uint32_t address);
uint32_t ff_device_registers_touch(ff_device_registers *, uint32_t address, uint32_t direction,
                                   uint32_t site, uint32_t *value);
/* Stage one write (or the write half of a read-modify-write) and return the
 * lvalue the generated store fills. commit records it as one write event with
 * the stored value; touch, access and the event readers commit first. */
uint32_t *ff_device_registers_stage_write(ff_device_registers *, uint32_t address, uint32_t site);
uint32_t ff_device_registers_commit(ff_device_registers *);
/* Map one checked guest access to a readable lvalue. NULL means the aperture
 * must stop: unknown read, invalid width/address/site, bad access mask or full
 * storage. READWRITE records the read immediately and stages the write. */
uint32_t *ff_device_registers_access(ff_device_registers *, uint32_t address, uint32_t access,
                                     uint32_t site);
/* Width-aware access for reachable sub-dword register reads and writes.
 * Entries remain dword-valued profile registers; a 1/2-byte access maps into
 * the containing dword. Sub-dword writes may create or extend an entry, track
 * which bytes are known, and record the written byte/halfword value. Reads
 * require every accessed byte to be known. */
uint32_t *ff_device_registers_access_width(ff_device_registers *, uint32_t address, uint32_t bytes,
                                           uint32_t access, uint32_t site);
#endif
