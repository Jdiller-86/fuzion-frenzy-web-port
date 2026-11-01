#include "object_namespace.h"
#include "thread_bootstrap.h"
#include <string.h>

static unsigned fold(unsigned n) { return n >= 'A' && n <= 'Z' ? n + ('a'-'A') : n; }
static int equal(const char *a, const char *b, uint32_t bytes)
{
    if (strlen(a) != bytes) return 0;
    for (uint32_t i = 0; i < bytes; ++i) if (fold((unsigned char)a[i]) != fold((unsigned char)b[i])) return 0;
    return 1;
}
static int path_valid(const char *path, uint32_t bytes)
{
    if (!path || !bytes || bytes >= FF_NS_PATH || path[0] != '\\') return 0;
    for (uint32_t i=0;i<bytes;++i) if ((unsigned char)path[i] < 32 || (unsigned char)path[i] > 126 || path[i] == '/' ||
        (path[i] == '\\' && i && path[i-1] == '\\')) return 0;
    return 1;
}
static ff_namespace_object *find(ff_object_namespace *space, uint32_t id)
{
    for (unsigned i=0;i<FF_NS_OBJECTS;++i) if (space->objects[i].id == id && id) return &space->objects[i];
    return NULL;
}
static const ff_namespace_object *child(const ff_object_namespace *space, uint32_t parent, const char *name, uint32_t bytes)
{
    for (unsigned i=0;i<FF_NS_OBJECTS;++i) if (space->objects[i].id && space->objects[i].parent == parent &&
        equal(space->objects[i].name,name,bytes)) return &space->objects[i];
    return NULL;
}
static uint32_t insert(ff_object_namespace *space, uint32_t parent, const char *name, uint32_t bytes, uint32_t kind, uint32_t *id)
{
    if (!bytes || bytes >= FF_NS_NAME || !id) return FF_BOOT_UNSUPPORTED;
    if (child(space,parent,name,bytes)) return FF_NS_COLLISION;
    ff_namespace_object *slot = NULL;
    for (unsigned i=0;i<FF_NS_OBJECTS;++i) if (!space->objects[i].id) { slot=&space->objects[i]; break; }
    if (!slot || space->next_id == UINT32_MAX) return FF_BOOT_NO_MEMORY;
    memset(slot,0,sizeof *slot); slot->id=space->next_id++; slot->parent=parent; slot->kind=kind; slot->references=1;
    memcpy(slot->name,name,bytes); ++space->count; *id=slot->id;
    return 0;
}
uint32_t ff_namespace_init(ff_object_namespace *space, ff_virtual_memory *memory)
{
    if (!space || !memory || !memory->ram) return FF_BOOT_INVALID;
    memset(space,0,sizeof *space); space->memory=memory; space->next_id=2; space->count=1;
    space->objects[0].id=1; space->objects[0].kind=FF_NS_DIRECTORY; space->objects[0].references=1;
    uint32_t id;
    if (insert(space,1,"??",2,FF_NS_DIRECTORY,&id) || insert(space,1,"Device",6,FF_NS_DIRECTORY,&id)) return FF_BOOT_INVALID;
    return 0;
}
uint32_t ff_namespace_resolve(const ff_object_namespace *space, const char *path, uint32_t bytes, ff_namespace_resolution *result)
{
    if (!space || !space->memory || !result || !path_valid(path,bytes)) return FF_BOOT_UNSUPPORTED;
    uint32_t parent=1, position=1;
    ff_namespace_resolution found={1,bytes,0};
    while (position < bytes) {
        uint32_t start=position;
        while (position < bytes && path[position] != '\\') ++position;
        if (position-start >= FF_NS_NAME) return FF_BOOT_UNSUPPORTED;
        const ff_namespace_object *object=child(space,parent,path+start,position-start);
        if (!object) return position == bytes ? FF_NS_NOT_FOUND : FF_NS_PATH_NOT_FOUND;
        if (position < bytes) ++position;
        if (object->kind == FF_NS_LINK) {
            /* This admitted profile holds a direct device reference, not a
             * host-path substitution or recursively interpreted target text. */
            const ff_namespace_object *target=NULL;
            for (unsigned i=0;i<FF_NS_OBJECTS;++i) if (space->objects[i].id == object->target) target=&space->objects[i];
            if (!target || target->kind != FF_NS_DEVICE) return FF_BOOT_UNSUPPORTED;
            object=target;
        }
        found=(ff_namespace_resolution){object->id,position,bytes-position};
        if (object->kind == FF_NS_DEVICE) { *result=found; return 0; }
        if (object->kind != FF_NS_DIRECTORY) return FF_BOOT_UNSUPPORTED;
        parent=object->id;
    }
    *result=found; return 0;
}
static uint32_t parent_for(ff_object_namespace *space, const char *path, uint32_t bytes, uint32_t *parent, uint32_t *start)
{
    if (!path_valid(path,bytes) || path[bytes-1] == '\\') return FF_BOOT_UNSUPPORTED;
    uint32_t last=bytes;
    while (last && path[last-1] != '\\') --last;
    ff_namespace_resolution resolution;
    uint32_t status=ff_namespace_resolve(space,path,last > 1 ? last-1 : 1,&resolution);
    if (status) return status;
    ff_namespace_object *directory=find(space,resolution.object);
    if (!directory || directory->kind != FF_NS_DIRECTORY || resolution.remaining_bytes) return FF_BOOT_UNSUPPORTED;
    *parent=directory->id; *start=last; return 0;
}
uint32_t ff_namespace_add_device(ff_object_namespace *space, const char *path, uint32_t bytes, uint32_t kind, uint32_t *id)
{
    if (!space || kind != FF_NS_LOCAL_DISC || !id) return FF_BOOT_UNSUPPORTED;
    uint32_t parent,start;
    uint32_t status=parent_for(space,path,bytes,&parent,&start);
    if (status) return status;
    status=insert(space,parent,path+start,bytes-start,FF_NS_DEVICE,id);
    if (!status) find(space,*id)->device_kind=kind;
    return status;
}
static uint32_t read_string(ff_object_namespace *space, uint32_t descriptor, char output[FF_NS_PATH], uint32_t *bytes)
{
    if (!descriptor || (descriptor & 3u) || ff_vm_check(space->memory,descriptor,8,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint8_t *ram=space->memory->ram;
    uint32_t length=ram[descriptor] | (uint32_t)ram[descriptor+1]<<8;
    uint32_t maximum=ram[descriptor+2] | (uint32_t)ram[descriptor+3]<<8;
    uint32_t buffer=ram[descriptor+4] | (uint32_t)ram[descriptor+5]<<8 | (uint32_t)ram[descriptor+6]<<16 | (uint32_t)ram[descriptor+7]<<24;
    if (!length || length > maximum) return FF_BOOT_INVALID;
    if (length >= FF_NS_PATH) return FF_BOOT_UNSUPPORTED;
    if (!buffer || ff_vm_check(space->memory,buffer,length,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    memcpy(output,ram+buffer,length); output[length]=0;
    if (!path_valid(output,length)) return FF_BOOT_UNSUPPORTED;
    *bytes=length; return 0;
}
uint32_t ff_namespace_create_link(ff_object_namespace *space, uint32_t link_string, uint32_t target_string, uint32_t *id)
{
    if (!space || !space->memory || !id) return FF_BOOT_INVALID;
    char name[FF_NS_PATH],target_name[FF_NS_PATH]; uint32_t name_bytes,target_bytes;
    uint32_t status=read_string(space,link_string,name,&name_bytes);
    if (status) return status;
    status=read_string(space,target_string,target_name,&target_bytes);
    if (status) return status;
    ff_namespace_resolution resolution;
    status=ff_namespace_resolve(space,target_name,target_bytes,&resolution);
    if (status) return status == FF_BOOT_UNSUPPORTED ? status : FF_BOOT_INVALID; /* Target resolution precedes creation. */
    ff_namespace_object *target=find(space,resolution.object);
    if (!target || target->kind != FF_NS_DEVICE || resolution.remaining_bytes) return FF_BOOT_UNSUPPORTED;
    uint32_t parent,start;
    status=parent_for(space,name,name_bytes,&parent,&start);
    if (status) return status;
    status=insert(space,parent,name+start,name_bytes-start,FF_NS_LINK,id);
    if (status) return status;
    ff_namespace_object *link=find(space,*id); link->target=target->id;
    memcpy(link->target_name,target_name,target_bytes+1); ++target->references;
    return 0;
}
uint32_t ff_namespace_delete_link(ff_object_namespace *space, const char *path, uint32_t bytes)
{
    if (!space || !space->memory) return FF_BOOT_INVALID;
    uint32_t parent,start; uint32_t status=parent_for(space,path,bytes,&parent,&start);
    if (status) return status;
    const ff_namespace_object *found=child(space,parent,path+start,bytes-start);
    if (!found) return FF_NS_NOT_FOUND;
    if (found->kind != FF_NS_LINK) return FF_BOOT_UNSUPPORTED;
    ff_namespace_object *target=find(space,found->target);
    if (!target || target->references < 2) return FF_BOOT_UNSUPPORTED;
    --target->references; memset(find(space,found->id),0,sizeof *found); --space->count;
    return 0;
}
uint32_t ff_namespace_remove_device(ff_object_namespace *space, uint32_t id)
{
    if (!space || !space->memory) return FF_BOOT_INVALID;
    ff_namespace_object *device=find(space,id);
    if (!device || device->kind != FF_NS_DEVICE) return FF_NS_NOT_FOUND;
    if (device->references != 1) return FF_BOOT_UNSUPPORTED;
    memset(device,0,sizeof *device); --space->count; return 0;
}
