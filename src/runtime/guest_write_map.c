#include "guest_write_map.h"
#include "thread_bootstrap.h"
#include <string.h>

void ff_guest_write_map_init(ff_guest_write_map *map)
{
    if (map) memset(map, 0, sizeof *map);
}

uint32_t ff_guest_write_map_mark(ff_guest_write_map *map, uint32_t address, uint32_t bytes)
{
    if (!map) return FF_BOOT_INVALID;
    if (!bytes || address >= FF_GUEST_WRITE_MAP_BYTES) return 0;
    uint64_t end = (uint64_t)address + bytes;
    if (end > FF_GUEST_WRITE_MAP_BYTES) end = FF_GUEST_WRITE_MAP_BYTES;
    uint32_t first = address / FF_GUEST_WRITE_MAP_PAGE;
    uint32_t last = (uint32_t)((end - 1u) / FF_GUEST_WRITE_MAP_PAGE);
    for (uint32_t page = first; page <= last; ++page)
        map->words[page >> 5] |= 1u << (page & 31u);
    return 0;
}

uint32_t ff_guest_write_map_word(const ff_guest_write_map *map, uint32_t index)
{
    return map && index < FF_GUEST_WRITE_MAP_WORDS ? map->words[index] : 0;
}

uint32_t ff_guest_write_map_count(const ff_guest_write_map *map)
{
    if (!map) return 0;
    uint32_t count = 0;
    for (uint32_t i = 0; i < FF_GUEST_WRITE_MAP_WORDS; ++i) {
        uint32_t word = map->words[i];
        while (word) { word &= word - 1u; ++count; }
    }
    return count;
}
