/* Owned volume/file-object/namespace scenarios. No game or disk content. */
#include "../src/runtime/file_namespace.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[256*1024], saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_local_volume volume;
static ff_object_namespace space;
static const ff_volume_profile profile={100000,90000,32,512};
enum { LINK=0x10000, TARGET=0x10010, LINK_TEXT=0x10100, TARGET_TEXT=0x10200,
       ATTR=0x10300, NAME=0x10310, OUTPUT=0x10320, IOSB=0x10330, PATH=0x10400 };
static const char owned[]="\\Device\\Harddisk0\\partition1\\OwnedData";
static void put(uint32_t p,uint32_t value,unsigned bytes)
{
    for(unsigned i=0;i<bytes;++i) ram[p+i]=(uint8_t)(value>>(i*8));
}
static uint32_t get(uint32_t p)
{
    uint32_t value=0; for(unsigned i=0;i<4;++i)value|=(uint32_t)ram[p+i]<<(i*8); return value;
}
static void string(uint32_t descriptor,uint32_t buffer,const char *text)
{
    uint32_t bytes=(uint32_t)strlen(text); assert(bytes<256);
    memcpy(ram+buffer,text,bytes+1); put(descriptor,bytes,2); put(descriptor+2,bytes+1,2); put(descriptor+4,buffer,4);
}
static const ff_namespace_object *object(uint32_t id)
{
    for(unsigned i=0;i<FF_NS_OBJECTS;++i) if(space.objects[i].id==id) return &space.objects[i];
    return NULL;
}
static const ff_volume_handle *handle(uint32_t id)
{
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(volume.handles[i].handle==id) return &volume.handles[i];
    return NULL;
}
static void create_directory(const char *path)
{
    string(NAME,PATH,path); put(ATTR,0,4); put(ATTR+4,NAME,4); put(ATTR+8,0x40,4);
    const uint32_t args[9]={OUTPUT,0x120117,ATTR,IOSB,0,0x80,3,3,0x4021};
    assert(!ff_volume_create_directory(&volume,args)); assert(get(IOSB+4)==2);
    assert(!ff_volume_close(&volume,get(OUTPUT)));
}
static void reset(void)
{
    memset(ram,0xa5,sizeof ram);
    assert(!ff_vm_init(&memory,ram,sizeof ram)); assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x10000,4)); assert(!ff_volume_init(&volume,&memory,&profile));
    create_directory(owned); create_directory("\\Device\\Harddisk0\\partition1\\OwnedData\\Child");
    assert(!ff_namespace_init(&space,&memory)); uint32_t disc;
    assert(!ff_namespace_add_device(&space,"\\Device\\OwnedDisc",17,FF_NS_LOCAL_DISC,&disc));
    assert(!ff_file_namespace_bind(&space,&volume));
    string(LINK,LINK_TEXT,"\\??\\Q:"); string(TARGET,TARGET_TEXT,owned);
}
static uint32_t link(void)
{
    uint32_t id=0; assert(!ff_namespace_create_link(&space,LINK,TARGET,&id)); return id;
}
static uint32_t open(const char *path)
{
    uint32_t value=0; assert(!ff_file_namespace_open(&space,path,(uint32_t)strlen(path),0x100001,3,0x21,&value));
    assert(handle(value)); return value;
}
static void retained_object_and_relative_paths(void)
{
    reset(); uint32_t id=link(); const ff_namespace_object *target=object(object(id)->target);
    assert(target && target->kind==FF_NS_FILE && target->references==1);
    const ff_volume_file *file=ff_volume_file_object(&volume,target->provider_object);
    assert(file && file->directory==2 && file->references==1 && !file->handle);
    assert(volume.file_count==1 && !volume.active && volume.opened==3 && volume.closed==3);
    memcpy(saved_ram,ram,sizeof ram);
    ff_namespace_resolution resolved;
    const char *path="\\??\\q:\\Child";
    assert(!ff_namespace_resolve(&space,path,(uint32_t)strlen(path),&resolved));
    assert(resolved.object==target->id && resolved.remaining_bytes==5 && !strcmp(path+resolved.remaining_offset,"Child"));
    uint32_t child=open(path); assert(handle(child)->directory==3);
    uint32_t retained=0; assert(!ff_volume_reference_handle(&volume,child,&retained));
    assert(ff_volume_file_object(&volume,retained)->references==2);
    assert(!ff_volume_close(&volume,child)); assert(ff_volume_file_object(&volume,retained)->references==1);
    assert(!ff_volume_release_file(&volume,retained)); assert(!ff_volume_file_object(&volume,retained));
    assert(!memcmp(saved_ram,ram,sizeof ram));
    string(LINK,LINK_TEXT,"\\??\\M:"); string(TARGET,TARGET_TEXT,"\\??\\Q:"); uint32_t opened=volume.opened;
    uint32_t alias=link(); assert(object(alias)->target==target->id && object(target->id)->references==2 && volume.opened==opened);
    assert(!ff_namespace_delete_link(&space,"\\??\\Q:",6)); assert(volume.file_count==1 && object(target->id)->references==1);
    uint32_t root=open("\\??\\m:"); assert(handle(root)->directory==2);
    assert(!ff_namespace_delete_link(&space,"\\??\\M:",6)); assert(!volume.file_count && space.count==4);
    assert(!ff_volume_close(&volume,root)); assert(!volume.active);
    assert(ff_volume_release_file(&volume,retained)==FF_BOOT_BAD_HANDLE);
}
static void alias_to_existing_subdirectory(void)
{
    reset(); link(); string(LINK,LINK_TEXT,"\\??\\R:"); string(TARGET,TARGET_TEXT,"\\??\\Q:\\Child");
    uint32_t id=link(); const ff_namespace_object *target=object(object(id)->target);
    assert(ff_volume_file_object(&volume,target->provider_object)->directory==3 && volume.file_count==2);
    uint32_t opened=open("\\??\\r:"); assert(handle(opened)->directory==3); assert(!ff_volume_close(&volume,opened));
    uint32_t output=0xa5a5a5a5;
    const char *missing="\\??\\R:\\Missing";
    assert(ff_file_namespace_open(&space,missing,(uint32_t)strlen(missing),0,7,1,&output)==FF_VOLUME_NAME_NOT_FOUND);
    assert(output==0xa5a5a5a5 && !volume.active && volume.file_count==2);
    assert(!ff_namespace_delete_link(&space,"\\??\\Q:",6)); assert(volume.file_count==1);
    opened=open("\\??\\R:"); assert(handle(opened)->directory==3); assert(!ff_volume_close(&volume,opened));
    assert(!ff_namespace_delete_link(&space,"\\??\\R:",6)); assert(!volume.file_count);
}
static void failed_creation_releases_temporary_objects(void)
{
    reset(); link(); uint32_t count=space.count, files=volume.file_count, output=0xa5a5a5a5;
    memcpy(saved_ram,ram,sizeof ram);
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_NS_COLLISION);
    assert(space.count==count && volume.file_count==files && !volume.active && output==0xa5a5a5a5);
    assert(!memcmp(saved_ram,ram,sizeof ram));
    string(TARGET,TARGET_TEXT,"\\Device\\Harddisk0\\partition1\\Missing");
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_VOLUME_NAME_NOT_FOUND);
    assert(space.count==count && volume.file_count==files && !volume.active);
    string(TARGET,TARGET_TEXT,owned); put(TARGET+2,1,2); memcpy(saved_ram,ram,sizeof ram);
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_BOOT_INVALID);
    assert(!memcmp(saved_ram,ram,sizeof ram) && volume.file_count==files);
    assert(ff_file_namespace_bind(&space,&volume)==FF_BOOT_INVALID);
}
static void capacity_and_reference_boundaries(void)
{
    reset(); uint32_t handles[FF_VOLUME_HANDLES];
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) assert(!ff_volume_open_directory_path(&volume,0,owned,sizeof owned-1,0,7,1,&handles[i]));
    uint32_t output=0xa5a5a5a5;
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_VOLUME_TOO_MANY_HANDLES && space.count==4 && !volume.file_count);
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) assert(!ff_volume_close(&volume,handles[i]));
    uint32_t references[FF_VOLUME_FILES];
    for(unsigned i=0;i<FF_VOLUME_FILES;++i) {
        uint32_t value=0; assert(!ff_volume_open_directory_path(&volume,0,owned,sizeof owned-1,0,7,1,&value));
        assert(!ff_volume_reference_handle(&volume,value,&references[i])); assert(!ff_volume_close(&volume,value));
    }
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_BOOT_NO_MEMORY);
    assert(space.count==4 && volume.file_count==FF_VOLUME_FILES && !volume.active && output==0xa5a5a5a5);
    for(unsigned i=0;i<FF_VOLUME_FILES;++i) assert(!ff_volume_release_file(&volume,references[i]));
    assert(!volume.file_count); uint32_t id=link();
    string(TARGET,TARGET_TEXT,"\\??\\Q:");
    while(space.count<FF_NS_OBJECTS) {
        char path[32]; snprintf(path,sizeof path,"\\??\\Alias%u",space.count); string(LINK,LINK_TEXT,path); link();
    }
    string(TARGET,TARGET_TEXT,owned); string(LINK,LINK_TEXT,"\\??\\OneMore");
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_BOOT_NO_MEMORY);
    assert(volume.file_count==1 && !volume.active && space.count==FF_NS_OBJECTS);
    assert(object(id)->kind==FF_NS_LINK);
}
static void read_only_names_and_unimplemented_paths(void)
{
    reset(); assert(!ff_vm_init(&memory,ram,sizeof ram)); assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x10000,2)); memcpy(saved_ram,ram,sizeof ram); link();
    assert(!memcmp(saved_ram,ram,sizeof ram));
    uint32_t output=0xa5a5a5a5;
    const char *invalid[]={"\\??\\Q:\\..","\\??\\Q:\\Child/Host","\\Device\\OwnedDisc\\Child","\\Device\\Harddisk0\\partition10\\Child"};
    for(unsigned i=0;i<sizeof invalid/sizeof invalid[0];++i) {
        assert(ff_file_namespace_open(&space,invalid[i],(uint32_t)strlen(invalid[i]),0,7,1,&output)==FF_BOOT_UNSUPPORTED);
        assert(!volume.active && volume.file_count==1 && output==0xa5a5a5a5);
    }
}
int main(void)
{
    retained_object_and_relative_paths(); alias_to_existing_subdirectory(); failed_creation_releases_temporary_objects();
    capacity_and_reference_boundaries(); read_only_names_and_unimplemented_paths();
    puts("Owned file-target links, retained references, relative opens, cleanup and bounded failures passed");
    return 0;
}
