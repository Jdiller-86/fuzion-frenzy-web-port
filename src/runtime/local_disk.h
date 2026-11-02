#ifndef FF_LOCAL_DISK_H
#define FF_LOCAL_DISK_H
#include "virtual_memory.h"

/* Explicit fresh synthetic block device. Sparse pages stay inside Wasm memory;
 * no host disk, firmware, original save image or persistent backing is opened. */
#define FF_DISK_HANDLES 8u
#define FF_DISK_PAGES 128u
#define FF_DISK_PARTITIONS 41u
#define FF_DISK_PAGE_BYTES 4096u
#define FF_DISK_HANDLE_BASE 0x00060000u
#define FF_DISK_HANDLE_LIMIT 0x00070000u
#define FF_DISK_EOF 0xc0000011u
#define FF_DISK_FULL 0xc000007fu
#define FF_DISK_SHARING 0xc0000043u

typedef struct { uint64_t bytes; uint32_t sector_bytes, cache_partitions; } ff_disk_profile;
typedef struct { uint32_t number; uint64_t offset, bytes; uint32_t hidden_sectors, type, flags, information_ready; } ff_disk_partition;
typedef struct { uint64_t cylinders; uint32_t tracks_per_cylinder, sectors_per_track, ready; } ff_disk_geometry;
typedef struct { uint32_t handle, access, share, options, partition; uint64_t position, offset, bytes; } ff_disk_handle;
typedef struct { uint64_t index; uint32_t used; uint8_t data[FF_DISK_PAGE_BYTES]; } ff_disk_page;
typedef struct {
    ff_virtual_memory *memory;
    ff_disk_profile profile;
    ff_disk_geometry geometry;
    ff_disk_handle handles[FF_DISK_HANDLES];
    ff_disk_page pages[FF_DISK_PAGES];
    ff_disk_partition partitions[FF_DISK_PARTITIONS];
    uint32_t partition_count;
    uint32_t next_handle, active, opened, closed, allocated_pages;
} ff_local_disk;

uint32_t ff_disk_init(ff_local_disk *, ff_virtual_memory *, const ff_disk_profile *);
uint32_t ff_disk_add_partition(ff_local_disk *, uint32_t number, uint64_t offset, uint64_t bytes);
uint32_t ff_disk_set_geometry(ff_local_disk *, uint64_t cylinders, uint32_t tracks, uint32_t sectors);
uint32_t ff_disk_set_partition_information(ff_local_disk *, uint32_t number, uint32_t hidden_sectors, uint32_t type, uint32_t flags);
uint32_t ff_disk_control(ff_local_disk *, const uint32_t args[10]);
uint32_t ff_disk_open(ff_local_disk *, const uint32_t args[6]);
uint32_t ff_disk_read(ff_local_disk *, const uint32_t args[8]);
uint32_t ff_disk_write(ff_local_disk *, const uint32_t args[8]);
uint32_t ff_disk_close(ff_local_disk *, uint32_t);
int ff_disk_owns_handle(uint32_t);
uint8_t ff_disk_byte(const ff_local_disk *, uint64_t);
#endif
