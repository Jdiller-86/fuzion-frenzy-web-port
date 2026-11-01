#ifndef FF_LOCAL_VOLUME_H
#define FF_LOCAL_VOLUME_H
#include "virtual_memory.h"

/* Explicit synthetic developer volume. No native filesystem, persistence or
 * implicit original-console capacity/content defaults. */
#define FF_VOLUME_HANDLES 16u
#define FF_VOLUME_FILES 32u
#define FF_VOLUME_DIRECTORIES 32u
#define FF_VOLUME_DIRECTORY_NAME 64u
#define FF_VOLUME_DATA_FILES 16u
#define FF_VOLUME_FILE_BYTES 65536u
#define FF_VOLUME_HANDLE_BASE 0x00010000u
#define FF_VOLUME_HANDLE_LIMIT 0x00020000u
#define FF_VOLUME_SHARING_VIOLATION 0xc0000043u
#define FF_VOLUME_TOO_MANY_HANDLES 0xc000009au
#define FF_VOLUME_IS_DIRECTORY 0xc00000bau
#define FF_VOLUME_BUFFER_TOO_SMALL 0xc0000023u
#define FF_VOLUME_NAME_NOT_FOUND 0xc0000034u
#define FF_VOLUME_NAME_COLLISION 0xc0000035u
#define FF_VOLUME_PATH_NOT_FOUND 0xc000003au
#define FF_VOLUME_NOT_DIRECTORY 0xc0000103u

typedef struct {
    uint64_t total_units, available_units;
    uint32_t sectors_per_unit, bytes_per_sector;
} ff_volume_profile;
typedef struct { uint32_t handle, access, share, options, directory, file, data_file, position; } ff_volume_handle;
/* A referenced open instance can outlive its last handle. Directory identity
 * and file-object identity are separate; these are opaque owned tokens. */
typedef struct { uint32_t id, directory, access, share, options, references, handle, data_file; } ff_volume_file;
typedef struct { uint32_t id, parent; char name[FF_VOLUME_DIRECTORY_NAME]; } ff_volume_directory;
typedef struct {
    uint32_t id, parent, attributes, bytes;
    uint64_t creation_time, access_time, write_time, change_time, allocated_bytes;
    char name[FF_VOLUME_DIRECTORY_NAME];
    uint8_t data[FF_VOLUME_FILE_BYTES];
} ff_volume_data_file;
/* Pure guest-tree lookup: a missing final component has both identities zero.
 * Missing intermediate directories are errors. No handles or nodes are added. */
typedef struct { uint32_t parent, directory, data_file; char name[FF_VOLUME_DIRECTORY_NAME]; } ff_volume_path;
typedef struct {
    ff_virtual_memory *memory;
    ff_volume_profile profile;
    ff_volume_handle handles[FF_VOLUME_HANDLES];
    ff_volume_file files[FF_VOLUME_FILES];
    ff_volume_directory directories[FF_VOLUME_DIRECTORIES];
    ff_volume_data_file data_files[FF_VOLUME_DATA_FILES];
    uint32_t next_handle, active, opened, closed;
    uint32_t directory_count, directories_created;
    uint32_t next_file, file_count;
    uint32_t data_file_count, data_files_created;
} ff_local_volume;

uint32_t ff_volume_init(ff_local_volume *, ff_virtual_memory *, const ff_volume_profile *);
uint32_t ff_volume_open_root(ff_local_volume *, const uint32_t args[6]);
uint32_t ff_volume_create_directory(ff_local_volume *, const uint32_t args[9]);
uint32_t ff_volume_create_file(ff_local_volume *, const uint32_t args[9], uint64_t system_time);
uint32_t ff_volume_query_file(ff_local_volume *, const uint32_t args[5]);
uint32_t ff_volume_resolve_path(const ff_local_volume *, uint32_t directory, const char *, uint32_t bytes,
                              uint32_t attributes, ff_volume_path *);
uint32_t ff_volume_open_directory_path(ff_local_volume *, uint32_t directory, const char *, uint32_t bytes,
                                       uint32_t access, uint32_t share, uint32_t options, uint32_t *handle);
uint32_t ff_volume_query_size(ff_local_volume *, const uint32_t args[5]);
uint32_t ff_volume_close(ff_local_volume *, uint32_t handle);
uint32_t ff_volume_reference_handle(ff_local_volume *, uint32_t handle, uint32_t *file);
uint32_t ff_volume_release_file(ff_local_volume *, uint32_t file);
const ff_volume_file *ff_volume_file_object(const ff_local_volume *, uint32_t file);
int ff_volume_owns_handle(uint32_t handle);
#endif
