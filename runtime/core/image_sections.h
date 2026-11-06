#ifndef FF_IMAGE_SECTIONS_H
#define FF_IMAGE_SECTIONS_H
#include "virtual_memory.h"

#define FF_IMAGE_SECTIONS 128u
#define FF_IMAGE_SHARED (FF_IMAGE_SECTIONS * 2u)
#define FF_IMAGE_SOURCE_BYTES (16u * 1024u * 1024u)

/* Source is a caller-owned immutable snapshot, disjoint from guest RAM.
 * Only validated image pages and explicit header reference fields are owned.
 * This does not implement physical Xbox memory, signature checks or concurrency. */
typedef struct {
    uint32_t descriptor, flags, address, bytes, offset, file_bytes, name, name_bytes;
    uint32_t references, head, tail;
} ff_image_section;
typedef struct { uint32_t address, page, references; } ff_image_shared;
typedef struct {
    ff_virtual_memory *memory;
    const uint8_t *source;
    uint32_t source_bytes, base, header_bytes, image_bytes, table, count, shared_count, owner, ready;
    ff_image_section sections[FF_IMAGE_SECTIONS];
    ff_image_shared shared[FF_IMAGE_SHARED];
} ff_image_sections;

uint32_t ff_image_init(ff_image_sections *, ff_virtual_memory *, const uint8_t *source, uint32_t bytes);
uint32_t ff_image_load(ff_image_sections *, uint32_t descriptor);
uint32_t ff_image_unload(ff_image_sections *, uint32_t descriptor);
#endif
