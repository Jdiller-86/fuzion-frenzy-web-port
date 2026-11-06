/* Owned guest write-page map; no original code or device model. */
#include "../src/runtime/guest_write_map.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    ff_guest_write_map map, saved;
    memset(&map, 0xa5, sizeof map);
    ff_guest_write_map_init(&map);
    assert(ff_guest_write_map_count(&map) == 0);
    assert(ff_guest_write_map_word(&map, 0) == 0);

    assert(ff_guest_write_map_mark(&map, 0x1000, 1) == 0);
    assert(ff_guest_write_map_word(&map, 0) == (1u << 1));
    assert(ff_guest_write_map_count(&map) == 1);

    assert(ff_guest_write_map_mark(&map, 0x1fff, 2) == 0);
    assert(ff_guest_write_map_count(&map) == 2);
    assert(ff_guest_write_map_word(&map, 0) == ((1u << 1) | (1u << 2)));

    assert(ff_guest_write_map_mark(&map, 0, 0x1000) == 0);
    assert(ff_guest_write_map_count(&map) == 3);
    assert(ff_guest_write_map_mark(&map, 0x1000, 0) == 0);
    assert(ff_guest_write_map_count(&map) == 3);
    assert(ff_guest_write_map_mark(&map, FF_GUEST_WRITE_MAP_BYTES, 4) == 0);
    assert(ff_guest_write_map_count(&map) == 3);
    assert(ff_guest_write_map_mark(&map, FF_GUEST_WRITE_MAP_BYTES - 1, 0x2000) == 0);
    assert(ff_guest_write_map_count(&map) == 4);

    assert(ff_guest_write_map_mark(NULL, 0, 4) == FF_BOOT_INVALID);
    assert(ff_guest_write_map_word(NULL, 0) == 0);
    assert(ff_guest_write_map_word(&map, FF_GUEST_WRITE_MAP_WORDS) == 0);
    assert(ff_guest_write_map_count(NULL) == 0);

    saved = map;
    ff_guest_write_map_init(&map);
    assert(ff_guest_write_map_count(&map) == 0 && memcmp(&map, &saved, sizeof map) != 0);
    puts("owned guest write page map: init, page marks, clipping, count and bounds passed");
    return 0;
}
