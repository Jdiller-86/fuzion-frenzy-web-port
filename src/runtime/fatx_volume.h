#ifndef FF_FATX_VOLUME_H
#define FF_FATX_VOLUME_H
#include "local_disk.h"
#include "object_namespace.h"

/* Read-only FATX mounts over owned block backing. No prefilled tree, host disk
 * or original default geometry. Guest file operations beyond root/size stop;
 * the root directory/file reader below is an owned component, not a guest
 * service. */
#define FF_FATX_MOUNTS 4u
#define FF_FATX_FILES 16u
#define FF_FATX_SCAN_CLUSTERS 262144u
#define FF_FATX_ROOT_CHAIN 64u
#define FF_FATX_HANDLE_BASE 0x00070000u
#define FF_FATX_HANDLE_LIMIT 0x00080000u
#define FF_FATX_FILE_BASE 0x50000000u
#define FF_FATX_FILE_LIMIT 0x50010000u
#define FF_FATX_NAME 42u
#define FF_FATX_ATTR_DIRECTORY 0x10u
#define FF_FATX_PATH_BYTES 256u
#define FF_FATX_PATH_DEPTH 8u
#define FF_FATX_DIRECTORY_CHAIN 256u

typedef struct {
    char name[FF_FATX_NAME + 1];
    uint32_t attributes, first_cluster, size;
} ff_fatx_directory_entry;

typedef struct {
    uint32_t id,partition,device,serial,sectors_per_cluster,cluster_bytes,entry_bytes;
    uint32_t root_cluster,cluster_count,free_clusters,root_clusters,file_objects;
    uint64_t offset,bytes,fat_bytes,data_offset,revision;
} ff_fatx_mount;
typedef struct {uint32_t id,mount,handle,access,share,options,references;} ff_fatx_file;
typedef struct {
    ff_local_disk *disk;
    ff_object_namespace *space;
    ff_fatx_mount mounts[FF_FATX_MOUNTS];
    ff_fatx_file files[FF_FATX_FILES];
    uint32_t mount_count,file_count,next_handle,next_file,active,opened,closed;
} ff_fatx_volume;

uint32_t ff_fatx_init(ff_fatx_volume *,ff_local_disk *,ff_object_namespace *);
uint32_t ff_fatx_open_root(ff_fatx_volume *,const uint32_t args[6]);
uint32_t ff_fatx_query_size(ff_fatx_volume *,const uint32_t args[5]);
uint32_t ff_fatx_close(ff_fatx_volume *,uint32_t handle);
uint32_t ff_fatx_reference_handle(ff_fatx_volume *,uint32_t handle,uint32_t *file);
uint32_t ff_fatx_release_file(ff_fatx_volume *,uint32_t file);
/* Read-only directory/file access over the validated mount. These are
 * source-owned components, not guest services; unsupported layouts stop.
 * The directory entry passed to ff_fatx_read_entry must come from
 * ff_fatx_root_entry, not from untrusted guest bytes. */
uint32_t ff_fatx_root_count(ff_fatx_volume *,uint32_t mount,uint32_t *count);
uint32_t ff_fatx_root_entry(ff_fatx_volume *,uint32_t mount,uint32_t index,ff_fatx_directory_entry *entry);
uint32_t ff_fatx_read_entry(ff_fatx_volume *,uint32_t mount,const ff_fatx_directory_entry *entry,
                            uint32_t offset,uint8_t *output,uint32_t capacity,uint32_t *bytes);
/* Resolve a relative path of up to eight components and read a file. The path
 * uses '\\' or '/' separators; flags 0x40 selects case-insensitive matching.
 * These remain source-owned helpers, not guest services. */
uint32_t ff_fatx_lookup(ff_fatx_volume *,uint32_t mount,const uint8_t *path,uint32_t path_bytes,
                        uint32_t flags,ff_fatx_directory_entry *entry);
uint32_t ff_fatx_read_path(ff_fatx_volume *,uint32_t mount,const uint8_t *path,uint32_t path_bytes,
                           uint32_t flags,uint32_t offset,uint8_t *output,uint32_t capacity,uint32_t *bytes);
int ff_fatx_owns_handle(uint32_t);
#endif
