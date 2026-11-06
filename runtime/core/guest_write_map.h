#ifndef FF_GUEST_WRITE_MAP_H
#define FF_GUEST_WRITE_MAP_H
#include <stdint.h>

/* Bounded 4 KiB page map of guest physical RAM that the translated code has
 * written. It records pages, never bytes, and is a diagnostic aid only. */
#define FF_GUEST_WRITE_MAP_BYTES (64u * 1024u * 1024u)
#define FF_GUEST_WRITE_MAP_PAGE 4096u
#define FF_GUEST_WRITE_MAP_PAGES (FF_GUEST_WRITE_MAP_BYTES / FF_GUEST_WRITE_MAP_PAGE)
#define FF_GUEST_WRITE_MAP_WORDS ((FF_GUEST_WRITE_MAP_PAGES + 31u) / 32u)

typedef struct {
    uint32_t words[FF_GUEST_WRITE_MAP_WORDS];
} ff_guest_write_map;

void ff_guest_write_map_init(ff_guest_write_map *);
/* Marks every 4 KiB page touched by [address, address+bytes); clipped to the
 * modeled 64 MiB window. Returns 0, or FF_BOOT_INVALID on a null map. */
uint32_t ff_guest_write_map_mark(ff_guest_write_map *, uint32_t address, uint32_t bytes);
uint32_t ff_guest_write_map_word(const ff_guest_write_map *, uint32_t index);
uint32_t ff_guest_write_map_count(const ff_guest_write_map *);
#endif
