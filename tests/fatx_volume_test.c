/* Owned FATX structures only; no original executable, files or disk image. */
#include "../src/runtime/fatx_volume.h"
#include "../src/runtime/file_namespace.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[0x40000],saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_local_disk disk,saved_disk;
static ff_object_namespace space,saved_space;
static ff_fatx_volume fs,saved_fs;
static ff_local_volume primary;
enum {OUT=0x10100,IOSB=0x10120,ATTR=0x10140,NAME=0x10180,TEXT=0x10200,OFFSET=0x10300,
      LINK=0x10400,TARGET=0x10420,LINK_TEXT=0x10500,TARGET_TEXT=0x10600,DATA=0x11000};
static uint32_t args[6];
static const uint64_t part_base=0x180000000ull;
static void put(uint32_t p,uint64_t value,unsigned bytes){for(unsigned i=0;i<bytes;++i)ram[p+i]=(uint8_t)(value>>(8*i));}
static uint64_t get(uint32_t p,unsigned bytes){uint64_t value=0;for(unsigned i=0;i<bytes;++i)value|=(uint64_t)ram[p+i]<<(8*i);return value;}
static void name(const char *text){uint32_t bytes=(uint32_t)strlen(text);memcpy(ram+TEXT,text,bytes+1);put(NAME,bytes,2);put(NAME+2,bytes+1,2);put(NAME+4,TEXT,4);}
static void string(uint32_t descriptor,uint32_t buffer,const char *text)
{
    uint32_t bytes=(uint32_t)strlen(text);memcpy(ram+buffer,text,bytes+1);
    put(descriptor,bytes,2);put(descriptor+2,bytes+1,2);put(descriptor+4,buffer,4);
}
static void save(void){saved_fs=fs;saved_disk=disk;saved_space=space;memcpy(saved_ram,ram,sizeof ram);}
static void unchanged(void)
{
    assert(!memcmp(&fs,&saved_fs,sizeof fs) && !memcmp(&disk,&saved_disk,sizeof disk) &&
           !memcmp(&space,&saved_space,sizeof space) && !memcmp(ram,saved_ram,sizeof ram));
}
static void reset(uint64_t bytes)
{
    memset(ram,0xa5,sizeof ram);assert(!ff_vm_init(&memory,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&memory,0x10000,0x30000) && !ff_vm_map_fixed(&memory,0x10000,0x30000,4));
    ff_disk_profile profile={0x200000000ull,512,3};assert(!ff_disk_init(&disk,&memory,&profile));
    assert(!ff_disk_add_partition(&disk,5,part_base,bytes) && !ff_disk_attach_raw_volume(&disk,5));
    assert(!ff_namespace_init(&space,&memory));
    ff_volume_profile volume={1000,1000,32,512};assert(!ff_volume_init(&primary,&memory,&volume));
    assert(!ff_file_namespace_bind(&space,&primary));assert(!ff_fatx_init(&fs,&disk,&space));
    put(ATTR,0,4);put(ATTR+4,NAME,4);put(ATTR+8,0x40,4);
    uint32_t open[6]={OUT,0x100003,ATTR,IOSB,0,0x18};memcpy(args,open,sizeof args);
    name("\\Device\\Harddisk0\\Partition5");
}
static void write_page(uint32_t handle,uint64_t offset)
{
    uint32_t io[8]={handle,0,0,0,IOSB,DATA,4096,OFFSET};put(OFFSET,offset,8);assert(!ff_disk_write(&disk,io));
}
static void format_owned(uint32_t width,uint32_t root_offset)
{
    assert(!ff_disk_open(&disk,args));uint32_t handle=(uint32_t)get(OUT,4);
    memset(ram+DATA,0,4096);put(DATA,0x58544146,4);put(DATA+4,0x12345678,4);put(DATA+8,32,4);put(DATA+12,1,4);write_page(handle,0);
    memset(ram+DATA,0,4096);put(DATA,width==2?0xfff8:0xfffffff8u,width);put(DATA+width,width==2?0xffff:UINT32_MAX,width);write_page(handle,4096);
    memset(ram+DATA,0xff,4096);for(unsigned i=0;i<4;++i)write_page(handle,root_offset+i*4096);
    uint32_t control[10]={handle,0,0,0,IOSB,0x90020,0,0,0,0};assert(!ff_disk_fs_control(&disk,control));assert(!ff_disk_close(&disk,handle));
    name("\\Device\\Harddisk0\\Partition5\\");args[1]=0x100001;args[4]=3;args[5]=0x800021;
}
static uint32_t open_root(void){assert(!ff_fatx_open_root(&fs,args));assert(get(IOSB+4,4)==1);return (uint32_t)get(OUT,4);}
static void rewrite_fat(uint32_t first,uint32_t second)
{
    name("\\Device\\Harddisk0\\Partition0");uint32_t raw[6]={OUT,0x100003,ATTR,IOSB,3,0x18};
    assert(!ff_disk_open(&disk,raw));uint32_t handle=(uint32_t)get(OUT,4);
    memset(ram+DATA,0,4096);put(DATA,0xfffffff8u,4);put(DATA+4,first,4);put(DATA+8,second,4);
    write_page(handle,part_base+4096);assert(!ff_disk_close(&disk,handle));name("\\Device\\Harddisk0\\Partition5\\");
}
static void fat32_root_query_namespace_and_references(void)
{
    reset(0x40000000);format_owned(4,270336);
    string(LINK,LINK_TEXT,"\\??\\Q:");string(TARGET,TARGET_TEXT,"\\Device\\Harddisk0\\partition1");
    uint32_t link;assert(!ff_namespace_create_link(&space,LINK,TARGET,&link));
    uint32_t handle=open_root();const ff_fatx_mount *mount=&fs.mounts[0];
    assert(ff_fatx_owns_handle(handle) && !ff_volume_owns_handle(handle) && !ff_disk_owns_handle(handle));
    assert(mount->partition==5 && mount->entry_bytes==4 && mount->fat_bytes==266240 && mount->data_offset==270336);
    assert(mount->offset==part_base && mount->cluster_count==65519 && mount->free_clusters==65518 && mount->root_clusters==1);
    uint32_t query[5]={handle,IOSB,DATA,24,3};assert(!ff_fatx_query_size(&fs,query));
    assert(get(DATA,8)==65519 && get(DATA+8,8)==65518 && get(DATA+16,4)==32 && get(DATA+20,4)==512 && get(IOSB+4,4)==24);
    query[2]=0x3fff0;save();assert(ff_fatx_query_size(&fs,query)==FF_VM_ACCESS_VIOLATION);unchanged();query[2]=DATA;
    query[2]=IOSB;save();assert(ff_fatx_query_size(&fs,query)==FF_BOOT_UNSUPPORTED);unchanged();query[2]=DATA;
    query[3]=23;memset(ram+DATA,0x91,24);assert(ff_fatx_query_size(&fs,query)==0xc0000023u && ram[DATA]==0x91 && !get(IOSB+4,4));query[3]=24;
    uint32_t token;assert(!ff_fatx_reference_handle(&fs,handle,&token));assert(!ff_fatx_close(&fs,handle));
    assert(!fs.active && fs.file_count==1 && mount->file_objects==1 && fs.files[0].references==1 && !fs.files[0].handle);
    assert(!ff_fatx_release_file(&fs,token) && !fs.file_count && !mount->file_objects);
    string(LINK,LINK_TEXT,"\\??\\Z:");string(TARGET,TARGET_TEXT,"\\Device\\Harddisk0\\Partition5");
    assert(!ff_namespace_create_link(&space,LINK,TARGET,&link));
    ff_namespace_resolution found;assert(!ff_namespace_resolve(&space,"\\??\\z:\\",7,&found) && found.object==mount->device && !found.remaining_bytes);
    name("\\??\\z:\\");handle=open_root();assert(fs.mount_count==1);
    args[4]=0;assert(ff_fatx_open_root(&fs,args)==FF_DISK_SHARING);args[4]=3;
    assert(!ff_fatx_close(&fs,handle));save();assert(ff_fatx_query_size(&fs,query)==FF_BOOT_BAD_HANDLE);unchanged();
    uint32_t primary_handle;assert(!ff_file_namespace_open(&space,"\\??\\Q:\\",7,0x100001,3,0x21,&primary_handle));
    assert(ff_volume_owns_handle(primary_handle) && !ff_volume_close(&primary,primary_handle));
    name("\\Device\\Harddisk0\\Partition1\\");save();assert(ff_fatx_open_root(&fs,args)==FF_BOOT_UNSUPPORTED);unchanged();
}
static void fat16_bounds_chain_and_mutation(void)
{
    reset(0x800000);format_owned(2,8192);uint32_t handle=open_root();
    assert(fs.mounts[0].entry_bytes==2 && fs.mounts[0].fat_bytes==4096 && fs.mounts[0].cluster_count==511 && fs.mounts[0].free_clusters==510);
    assert(!ff_fatx_close(&fs,handle));
    reset(0x40000000);format_owned(4,270336);
    rewrite_fat(1,0);save();assert(ff_fatx_open_root(&fs,args)==FF_BOOT_UNSUPPORTED);unchanged();
    rewrite_fat(65520,0);save();assert(ff_fatx_open_root(&fs,args)==FF_BOOT_UNSUPPORTED);unchanged();
    rewrite_fat(0,0);save();assert(ff_fatx_open_root(&fs,args)==FF_BOOT_UNSUPPORTED);unchanged();
    rewrite_fat(2,UINT32_MAX);handle=open_root();assert(fs.mounts[0].root_clusters==2 && fs.mounts[0].free_clusters==65517);
    rewrite_fat(UINT32_MAX,0);uint32_t query[5]={handle,IOSB,DATA,24,3};
    save();assert(ff_fatx_query_size(&fs,query)==FF_BOOT_UNSUPPORTED);unchanged();
    save();assert(ff_fatx_open_root(&fs,args)==FF_BOOT_UNSUPPORTED);unchanged();assert(!ff_fatx_close(&fs,handle));
}
static void capacity_failures_do_not_partially_mount(void)
{
    reset(0x40000000);format_owned(4,270336);uint32_t handle[FF_FATX_FILES];
    for(unsigned i=0;i<FF_FATX_FILES;++i)handle[i]=open_root();
    saved_fs=fs;saved_space=space;saved_disk=disk;
    assert(ff_fatx_open_root(&fs,args)==FF_BOOT_NO_MEMORY && !memcmp(&fs,&saved_fs,sizeof fs) && !memcmp(&space,&saved_space,sizeof space) && !memcmp(&disk,&saved_disk,sizeof disk));
    for(unsigned i=0;i<FF_FATX_FILES;++i)assert(!ff_fatx_close(&fs,handle[i]));
    reset(0x40000000);format_owned(4,270336);
    for(unsigned i=0;i<FF_NS_OBJECTS-4;++i){char path[40];snprintf(path,sizeof path,"\\Device\\Owned%u",i);uint32_t id;assert(!ff_namespace_add_device(&space,path,(uint32_t)strlen(path),FF_NS_LOCAL_DISC,&id));}
    saved_fs=fs;saved_space=space;saved_disk=disk;
    assert(ff_fatx_open_root(&fs,args)==FF_BOOT_NO_MEMORY && get(IOSB,4)==FF_BOOT_NO_MEMORY && !memcmp(&fs,&saved_fs,sizeof fs) && !memcmp(&space,&saved_space,sizeof space) && !memcmp(&disk,&saved_disk,sizeof disk));
}
int main(void)
{
    fat32_root_query_namespace_and_references();fat16_bounds_chain_and_mutation();capacity_failures_do_not_partially_mount();
    puts("owned FATX mount, allocation query, namespace identity, references, bounds and mutation checks passed");return 0;
}
