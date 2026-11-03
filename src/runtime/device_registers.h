#ifndef FF_DEVICE_REGISTERS_H
#define FF_DEVICE_REGISTERS_H
#include <stdint.h>

/* Bounded synthetic device aperture for reached original register accesses.
 * Not an NV2A model: every address starts unknown unless the selected profile
 * supplies a value; unknown reads fault instead of returning a fabricated 0. */
#define FF_DEVICE_ENTRIES 8192u
/* The second reached bulk clear (0xDFB dwords) exhausted 8,192 events after the
 * first 5,120-store clear had already filled most of the log. Keep more ordered
 * events so original register traffic is retained rather than truncated. */
#define FF_DEVICE_EVENTS 32768u
/* A translated loop can poll one device register for a hardware transition the
 * synthetic aperture cannot provide. Stop after a small run of identical reads
 * at the same site instead of exhausting the event log with no new evidence. */
#define FF_DEVICE_POLL_LIMIT 16u
#define FF_DEVICE_INITIAL 64u
#define FF_DEVICE_ZERO_RANGES 4u
#define FF_DEVICE_SELF_CLEARING 4u
#define FF_DEVICE_ALIASES 4u
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
    uint32_t base, bytes;
} ff_device_zero_range;
/* One evidence-backed self-completing register bit: a write is recorded, then
 * the masked bits read back clear (for example NV_PFB_WBC_FLUSH). */
typedef struct {
    uint32_t address, mask;
} ff_device_self_clearing;
/* Two register addresses that share one storage entry (for example USER and
 * PFIFO CACHE1 views of the same DMA cursor). */
typedef struct {
    uint32_t first, second;
} ff_device_alias;
typedef struct {
    uint32_t base, bytes, ready, count, event_count, zero_count;
    /* A generated store writes through a returned lvalue after the host call.
     * Keep one staged write so the event can record the actual stored value. */
    uint32_t pending, pending_address, pending_site, pending_value, pending_bytes, pending_offset;
    /* Consecutive identical reads at one site: a device transition this model
     * cannot supply. poll_count above FF_DEVICE_POLL_LIMIT stops the proof. */
    uint32_t poll_address, poll_value, poll_site, poll_count;
    uint32_t self_clearing_count, alias_count;
    uint32_t zero_word;
    ff_device_alias aliases[FF_DEVICE_ALIASES];
    ff_device_self_clearing self_clearing[FF_DEVICE_SELF_CLEARING];
    ff_device_zero_range zero_ranges[FF_DEVICE_ZERO_RANGES];
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
/* Declare an explicit zero-filled device region. Reads in the region return
 * zero without creating entries; a later write creates a normal entry that
 * takes precedence over the range. */
uint32_t ff_device_registers_add_zero_range(ff_device_registers *, uint32_t base, uint32_t bytes);
/* Declare a register whose masked bits complete immediately: the write event is
 * retained, but the stored value reads back with the mask cleared. */
uint32_t ff_device_registers_add_self_clearing(ff_device_registers *, uint32_t address, uint32_t mask);
/* Declare two addresses that share one stored value; events keep the address
 * the translated block actually used. */
uint32_t ff_device_registers_add_alias(ff_device_registers *, uint32_t first, uint32_t second);
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
/* True once a read loop exceeded the bounded identical-read limit. */
uint32_t ff_device_registers_poll_limited(const ff_device_registers *);
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
