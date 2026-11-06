#include "file_namespace.h"
#include "thread_bootstrap.h"
#include <stddef.h>

static const ff_namespace_file_provider provider;

uint32_t ff_file_namespace_open(const ff_object_namespace *space, const char *path, uint32_t bytes,
                               uint32_t access, uint32_t share, uint32_t options, uint32_t *handle)
{
    if(!space || space->file_provider!=&provider || !space->file_context || !handle) return FF_BOOT_INVALID;
    ff_local_volume *volume=space->file_context;
    ff_namespace_resolution resolution;
    uint32_t status=ff_namespace_resolve(space,path,bytes,&resolution);
    if(status==FF_NS_NOT_FOUND || status==FF_NS_PATH_NOT_FOUND) {
        return ff_volume_open_directory_path(volume,0,path,bytes,access,share,options,handle);
    }
    if(status) return status;
    const ff_namespace_object *object=NULL;
    for(unsigned i=0;i<FF_NS_OBJECTS;++i) if(space->objects[i].id==resolution.object) object=&space->objects[i];
    if(!object || object->kind!=FF_NS_FILE || resolution.remaining_offset>bytes || resolution.remaining_bytes!=bytes-resolution.remaining_offset) return FF_BOOT_UNSUPPORTED;
    const ff_volume_file *file=ff_volume_file_object(volume,object->provider_object);
    if(!file || !file->references) return FF_BOOT_UNSUPPORTED;
    return ff_volume_open_directory_path(volume,file->directory,path+resolution.remaining_offset,resolution.remaining_bytes,access,share,options,handle);
}
static uint32_t open_target(void *context, const ff_object_namespace *space, const char *path, uint32_t bytes, uint32_t *file)
{
    ff_local_volume *volume=context;
    uint32_t handle=0;
    uint32_t status=ff_file_namespace_open(space,path,bytes,0,7,1,&handle);
    if(status) return status;
    status=ff_volume_reference_handle(volume,handle,file);
    uint32_t closed=ff_volume_close(volume,handle);
    return closed?FF_BOOT_UNSUPPORTED:status;
}
static uint32_t release_target(void *context, uint32_t file)
{
    return ff_volume_release_file(context,file);
}
static const ff_namespace_file_provider provider={open_target,release_target};
uint32_t ff_file_namespace_bind(ff_object_namespace *space, ff_local_volume *volume)
{
    if(!space || !volume || !volume->memory || volume->memory!=space->memory || !volume->directory_count) return FF_BOOT_INVALID;
    return ff_namespace_bind_files(space,&provider,volume);
}
