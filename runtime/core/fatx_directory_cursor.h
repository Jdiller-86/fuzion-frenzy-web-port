#ifndef FF_FATX_DIRECTORY_CURSOR_H
#define FF_FATX_DIRECTORY_CURSOR_H
#include "fatx_volume.h"

/* Caller-owned state for the one consumed-root-entry continuation admitted by
 * the current startup profile. It is not a general directory enumerator. */
typedef struct {
    const ff_fatx_volume *owner;
    const ff_local_disk *backing;
    const ff_object_namespace *namespace_owner;
    uint32_t handle, file_id, mount, device, serial, index;
    uint64_t mount_offset, mount_bytes, mount_data_offset, mount_revision;
    uint32_t root_cluster, root_clusters, cluster_bytes, entry_bytes;
    uint32_t attributes, first_cluster, initial_size, name_bytes;
    char name[FF_FATX_NAME + 1u];
    uint8_t consumed, finished;
} ff_fatx_single_cursor;

uint32_t ff_fatx_single_cursor_begin(ff_fatx_volume *,uint32_t,const uint8_t *,uint32_t,
                                     ff_fatx_single_cursor *);
uint32_t ff_fatx_query_directory_next_single(ff_fatx_volume *,const uint32_t[10],
                                             ff_fatx_single_cursor *);
#endif
