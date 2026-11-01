/* Source-owned namespace scenarios, with no original code or mounted media. */
#include "../src/runtime/object_namespace.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[256*1024],saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_object_namespace space,saved_space;
static uint32_t device;
enum { LINK=0x10000, TARGET=0x10010, LINK_TEXT=0x10100, TARGET_TEXT=0x10200 };
static void put(uint32_t p,uint32_t value,unsigned bytes) { for(unsigned i=0;i<bytes;++i) ram[p+i]=(uint8_t)(value>>(i*8)); }
static void string(uint32_t descriptor,uint32_t buffer,const char *text)
{
    uint32_t bytes=(uint32_t)strlen(text); assert(bytes<256);
    memcpy(ram+buffer,text,bytes+1); put(descriptor,bytes,2); put(descriptor+2,bytes+1,2); put(descriptor+4,buffer,4);
}
static ff_namespace_object *object(uint32_t id)
{
    for(unsigned i=0;i<FF_NS_OBJECTS;++i) if(space.objects[i].id==id) return &space.objects[i];
    assert(0); return NULL;
}
static uint32_t resolve(const char *path,ff_namespace_resolution *result)
{
    return ff_namespace_resolve(&space,path,(uint32_t)strlen(path),result);
}
static void reset(void)
{
    memset(ram,0xa5,sizeof ram);
    assert(ff_vm_init(&memory,ram,sizeof ram)==0);
    assert(ff_vm_claim_fixed(&memory,0x10000,0x10000)==0);
    assert(ff_vm_map_fixed(&memory,0x10000,0x10000,4)==0);
    assert(ff_namespace_init(&space,&memory)==0 && space.count==3);
    const char *path="\\Device\\OwnedDisc";
    assert(ff_namespace_add_device(&space,path,(uint32_t)strlen(path),FF_NS_LOCAL_DISC,&device)==0);
    assert(space.count==4 && object(device)->references==1 && object(device)->media_ready==0);
    string(LINK,LINK_TEXT,"\\??\\Q:"); string(TARGET,TARGET_TEXT,path);
}
static void save(void) { saved_space=space; memcpy(saved_ram,ram,sizeof ram); }
static void unchanged(void) { assert(!memcmp(&space,&saved_space,sizeof space)); assert(!memcmp(ram,saved_ram,sizeof ram)); }
static void create_resolve_and_lifetime(void)
{
    reset(); uint32_t id=0; assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==0);
    assert(space.count==5 && object(id)->kind==FF_NS_LINK && object(id)->references==1);
    assert(object(id)->target==device && object(device)->references==2);
    assert(!strcmp(object(id)->target_name,"\\Device\\OwnedDisc") && !object(device)->media_ready);
    ff_namespace_resolution result;
    const char *path="\\??\\q:\\Packages\\Owned.xbp";
    assert(resolve(path,&result)==0 && result.object==device);
    assert(!strcmp(path+result.remaining_offset,"Packages\\Owned.xbp") && result.remaining_bytes==18);
    assert(resolve("\\??\\Q:",&result)==0 && result.object==device && !result.remaining_bytes);
    assert(resolve("\\device\\owneddisc\\",&result)==0 && result.object==device && !result.remaining_bytes);
    save(); assert(ff_namespace_remove_device(&space,device)==FF_BOOT_UNSUPPORTED); unchanged();
    assert(ff_namespace_delete_link(&space,"\\??\\q:",6)==0 && space.count==4 && object(device)->references==1);
    assert(resolve("\\??\\Q:",&result)==FF_NS_NOT_FOUND);
    assert(ff_namespace_remove_device(&space,device)==0 && space.count==3);
    assert(resolve("\\Device\\OwnedDisc",&result)==FF_NS_NOT_FOUND);
}
static void collision_and_target_resolution(void)
{
    reset(); uint32_t id; assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==0);
    string(LINK,LINK_TEXT,"\\??\\q:"); uint32_t output=0x12345678; save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_NS_COLLISION && output==0x12345678); unchanged();
    string(TARGET,TARGET_TEXT,"\\Device\\MissingDisc"); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_BOOT_INVALID && output==0x12345678); unchanged();
    string(TARGET,TARGET_TEXT,"\\Device"); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_BOOT_UNSUPPORTED); unchanged();
    string(TARGET,TARGET_TEXT,"\\Device\\OwnedDisc\\Directory"); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_BOOT_UNSUPPORTED); unchanged();
    string(TARGET,TARGET_TEXT,"\\Device\\OwnedDisc"); string(LINK,LINK_TEXT,"\\MissingDirectory\\Q:"); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&output)==FF_NS_NOT_FOUND); unchanged();
}
static void guest_admission_and_read_only_inputs(void)
{
    reset(); uint32_t id=0; save();
    assert(ff_namespace_create_link(&space,0xfffffffcu,TARGET,&id)==FF_VM_ACCESS_VIOLATION); unchanged();
    assert(ff_namespace_create_link(&space,LINK+1,TARGET,&id)==FF_VM_ACCESS_VIOLATION); unchanged();
    put(TARGET+4,0x1fffcu,4); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); put(TARGET,256,2); put(TARGET+2,256,2); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); put(TARGET+2,1,2); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==FF_BOOT_INVALID); unchanged();
    reset(); ram[TARGET_TEXT+3]=0; save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); assert(ff_vm_init(&memory,ram,sizeof ram)==0);
    assert(ff_vm_claim_fixed(&memory,0x10000,0x10000)==0);
    assert(ff_vm_map_fixed(&memory,0x10000,0x10000,2)==0);
    memcpy(saved_ram,ram,sizeof ram); assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==0);
    assert(!memcmp(saved_ram,ram,sizeof ram));
}
static void path_boundaries_and_capacity(void)
{
    reset(); ff_namespace_resolution result={0xa5,0xb6,0xc7},saved=result;
    const char *invalid[]={"relative","\\??\\\\Q:","\\??\\Q:/Owned","\\Device\\OwnedDisc/../host"};
    for(unsigned i=0;i<sizeof invalid/sizeof invalid[0];++i) {
        assert(resolve(invalid[i],&result)==FF_BOOT_UNSUPPORTED && !memcmp(&result,&saved,sizeof result));
    }
    uint32_t id;
    for(unsigned i=0;i<FF_NS_OBJECTS-4;++i) {
        char path[32]; snprintf(path,sizeof path,"\\??\\Owned%u",i); string(LINK,LINK_TEXT,path);
        assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==0);
    }
    assert(space.count==FF_NS_OBJECTS && object(device)->references==FF_NS_OBJECTS-3);
    string(LINK,LINK_TEXT,"\\??\\OneMore"); save();
    assert(ff_namespace_create_link(&space,LINK,TARGET,&id)==FF_BOOT_NO_MEMORY); unchanged();
}
int main(void)
{
    create_resolve_and_lifetime(); collision_and_target_resolution(); guest_admission_and_read_only_inputs(); path_boundaries_and_capacity();
    puts("Owned namespace, device references, links, resolution and admission checks passed"); return 0;
}
