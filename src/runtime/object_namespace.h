#ifndef FF_OBJECT_NAMESPACE_H
#define FF_OBJECT_NAMESPACE_H
#include "virtual_memory.h"

/* Owned bounded object names/references, not native paths or a file provider.
 * Device identity and mounted content are separate. These internal opaque
 * identifiers do not implement guest object layouts or handle APIs. */
#define FF_NS_OBJECTS 32u
#define FF_NS_NAME 64u
#define FF_NS_PATH 256u
#define FF_NS_COLLISION 0xc0000035u
#define FF_NS_NOT_FOUND 0xc0000034u
#define FF_NS_PATH_NOT_FOUND 0xc000003au
enum { FF_NS_DIRECTORY = 1, FF_NS_DEVICE, FF_NS_LINK };
enum { FF_NS_LOCAL_DISC = 1 };
typedef struct {
    uint32_t id, parent, kind, references, target, device_kind, media_ready;
    char name[FF_NS_NAME], target_name[FF_NS_PATH];
} ff_namespace_object;
typedef struct {
    ff_virtual_memory *memory;
    ff_namespace_object objects[FF_NS_OBJECTS];
    uint32_t next_id, count;
} ff_object_namespace;
typedef struct { uint32_t object, remaining_offset, remaining_bytes; } ff_namespace_resolution;

uint32_t ff_namespace_init(ff_object_namespace *, ff_virtual_memory *);
uint32_t ff_namespace_add_device(ff_object_namespace *, const char *, uint32_t bytes, uint32_t kind, uint32_t *id);
uint32_t ff_namespace_resolve(const ff_object_namespace *, const char *, uint32_t bytes, ff_namespace_resolution *);
uint32_t ff_namespace_create_link(ff_object_namespace *, uint32_t link_string, uint32_t target_string, uint32_t *id);
uint32_t ff_namespace_delete_link(ff_object_namespace *, const char *, uint32_t bytes);
uint32_t ff_namespace_remove_device(ff_object_namespace *, uint32_t id);
#endif
