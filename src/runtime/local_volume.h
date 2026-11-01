#ifndef FF_LOCAL_VOLUME_H
#define FF_LOCAL_VOLUME_H
#include "virtual_memory.h"

/* Explicit synthetic developer volume. No native filesystem, file payloads,
 * persistence or implicit original-console capacity/content defaults. */
#define FF_VOLUME_HANDLES 16u
#define FF_VOLUME_HANDLE_BASE 0x00010000u
#define FF_VOLUME_HANDLE_LIMIT 0x00020000u
#define FF_VOLUME_SHARING_VIOLATION 0xc0000043u
#define FF_VOLUME_TOO_MANY_HANDLES 0xc000009au
#define FF_VOLUME_IS_DIRECTORY 0xc00000bau
#define FF_VOLUME_BUFFER_TOO_SMALL 0xc0000023u

typedef struct {
    uint64_t total_units, available_units;
    uint32_t sectors_per_unit, bytes_per_sector;
} ff_volume_profile;
typedef struct { uint32_t handle, access, share, options; } ff_volume_handle;
typedef struct {
    ff_virtual_memory *memory;
    ff_volume_profile profile;
    ff_volume_handle handles[FF_VOLUME_HANDLES];
    uint32_t next_handle, active, opened, closed;
} ff_local_volume;

uint32_t ff_volume_init(ff_local_volume *, ff_virtual_memory *, const ff_volume_profile *);
uint32_t ff_volume_open_root(ff_local_volume *, const uint32_t args[6]);
uint32_t ff_volume_query_size(ff_local_volume *, const uint32_t args[5]);
uint32_t ff_volume_close(ff_local_volume *, uint32_t handle);
int ff_volume_owns_handle(uint32_t handle);
#endif
