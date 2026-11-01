#ifndef FF_LOCAL_VOLUME_H
#define FF_LOCAL_VOLUME_H
#include "virtual_memory.h"

/* Explicit synthetic developer volume. No native filesystem, file payloads,
 * persistence or implicit original-console capacity/content defaults. */
#define FF_VOLUME_HANDLES 16u
#define FF_VOLUME_DIRECTORIES 32u
#define FF_VOLUME_DIRECTORY_NAME 64u
#define FF_VOLUME_HANDLE_BASE 0x00010000u
#define FF_VOLUME_HANDLE_LIMIT 0x00020000u
#define FF_VOLUME_SHARING_VIOLATION 0xc0000043u
#define FF_VOLUME_TOO_MANY_HANDLES 0xc000009au
#define FF_VOLUME_IS_DIRECTORY 0xc00000bau
#define FF_VOLUME_BUFFER_TOO_SMALL 0xc0000023u
#define FF_VOLUME_NAME_NOT_FOUND 0xc0000034u
#define FF_VOLUME_NAME_COLLISION 0xc0000035u
#define FF_VOLUME_PATH_NOT_FOUND 0xc000003au

typedef struct {
    uint64_t total_units, available_units;
    uint32_t sectors_per_unit, bytes_per_sector;
} ff_volume_profile;
typedef struct { uint32_t handle, access, share, options, directory; } ff_volume_handle;
typedef struct { uint32_t id, parent; char name[FF_VOLUME_DIRECTORY_NAME]; } ff_volume_directory;
typedef struct {
    ff_virtual_memory *memory;
    ff_volume_profile profile;
    ff_volume_handle handles[FF_VOLUME_HANDLES];
    ff_volume_directory directories[FF_VOLUME_DIRECTORIES];
    uint32_t next_handle, active, opened, closed;
    uint32_t directory_count, directories_created;
} ff_local_volume;

uint32_t ff_volume_init(ff_local_volume *, ff_virtual_memory *, const ff_volume_profile *);
uint32_t ff_volume_open_root(ff_local_volume *, const uint32_t args[6]);
uint32_t ff_volume_create_directory(ff_local_volume *, const uint32_t args[9]);
uint32_t ff_volume_query_size(ff_local_volume *, const uint32_t args[5]);
uint32_t ff_volume_close(ff_local_volume *, uint32_t handle);
int ff_volume_owns_handle(uint32_t handle);
#endif
