#include "fatx_volume.h"
#include "thread_bootstrap.h"
#include <string.h>

static uint32_t get(const ff_fatx_volume *fs,uint32_t address,unsigned bytes)
{
    uint32_t value=0;
    for(unsigned i=0;i<bytes;++i)value|=(uint32_t)fs->disk->memory->ram[address+i]<<(8*i);
    return value;
}
static void put(ff_fatx_volume *fs,uint32_t address,uint64_t value,unsigned bytes)
{
    for(unsigned i=0;i<bytes;++i)fs->disk->memory->ram[address+i]=(uint8_t)(value>>(8*i));
}
static int admitted(const ff_fatx_volume *fs,uint32_t address,uint32_t bytes,uint32_t access)
{
    return fs && fs->disk && fs->disk->memory && address && !ff_vm_check(fs->disk->memory,address,bytes,access);
}
static int overlap(uint32_t a,uint32_t n,uint32_t b,uint32_t m)
{
    return n && m && (uint64_t)a<(uint64_t)b+m && (uint64_t)b<(uint64_t)a+n;
}
static uint32_t result(ff_fatx_volume *fs,uint32_t iosb,uint32_t status,uint32_t bytes)
{
    put(fs,iosb,status,4);put(fs,iosb+4,bytes,4);return status;
}
static uint32_t disk_word(const ff_fatx_volume *fs,uint64_t address,unsigned bytes)
{
    uint32_t value=0;
    for(unsigned i=0;i<bytes;++i)value|=(uint32_t)ff_disk_byte(fs->disk,address+i)<<(8*i);
    return value;
}
static ff_fatx_file *file_handle(ff_fatx_volume *fs,uint32_t handle)
{
    if(fs && handle)for(unsigned i=0;i<FF_FATX_FILES;++i)if(fs->files[i].handle==handle)return &fs->files[i];
    return NULL;
}
static ff_fatx_mount *mount_id(ff_fatx_volume *fs,uint32_t id)
{
    return id && id<=fs->mount_count?&fs->mounts[id-1]:NULL;
}
static int current(const ff_fatx_volume *fs,const ff_fatx_mount *mount)
{
    return mount && mount->revision==fs->disk->revision;
}
int ff_fatx_owns_handle(uint32_t handle){return handle>=FF_FATX_HANDLE_BASE && handle<FF_FATX_HANDLE_LIMIT;}
uint32_t ff_fatx_init(ff_fatx_volume *fs,ff_local_disk *disk,ff_object_namespace *space)
{
    if(!fs || !disk || !disk->memory || !space || space->memory!=disk->memory)return FF_BOOT_INVALID;
    memset(fs,0,sizeof *fs);fs->disk=disk;fs->space=space;
    fs->next_handle=FF_FATX_HANDLE_BASE;fs->next_file=FF_FATX_FILE_BASE;return 0;
}
static uint32_t layout(ff_fatx_volume *fs,uint32_t partition,ff_fatx_mount *mount)
{
    const ff_disk_partition *part=NULL;
    for(unsigned i=0;i<fs->disk->partition_count;++i)if(fs->disk->partitions[i].number==partition)part=&fs->disk->partitions[i];
    if(!part || fs->disk->profile.sector_bytes!=512)return FF_BOOT_UNSUPPORTED;
    int retired=0;
    for(unsigned i=0;i<fs->disk->volume_count;++i)if(fs->disk->volumes[i].partition==partition) {
        const ff_disk_volume *raw=&fs->disk->volumes[i];
        retired=raw->state==FF_DISK_VOLUME_RETIRED && !raw->references;
    }
    if(!retired || part->bytes<4096 || disk_word(fs,part->offset,4)!=0x58544146u)return FF_BOOT_UNSUPPORTED;
    const uint32_t sectors=disk_word(fs,part->offset+8,4),root=disk_word(fs,part->offset+12,4);
    if(!sectors || (sectors&(sectors-1)) || sectors>128)return FF_BOOT_UNSUPPORTED;
    const uint32_t cluster_bytes=sectors*512;
    const uint64_t entries=part->bytes/cluster_bytes+1;
    if(entries>FF_FATX_SCAN_CLUSTERS)return FF_BOOT_UNSUPPORTED;
    const uint32_t width=entries<0xfff0?2:4;
    const uint64_t fat=(entries*width+4095)&~4095ull,data=4096+fat;
    if(data>=part->bytes || part->bytes-data<cluster_bytes)return FF_BOOT_UNSUPPORTED;
    const uint32_t count=(uint32_t)((part->bytes-data)/cluster_bytes),last=width==2?0xffffu:UINT32_MAX;
    if(!root || root>count || disk_word(fs,part->offset+4096,width)!=last-7)return FF_BOOT_UNSUPPORTED;
    uint32_t free_clusters=0;
    for(uint32_t c=1;c<=count;++c) {
        const uint32_t next=disk_word(fs,part->offset+4096+(uint64_t)c*width,width);
        if(!next)++free_clusters;
        else if(next>count && next<last-15)return FF_BOOT_UNSUPPORTED;
    }
    uint32_t chain[FF_FATX_ROOT_CHAIN],length=0,cursor=root;
    for(;;) {
        if(!cursor || cursor>count || length==FF_FATX_ROOT_CHAIN)return FF_BOOT_UNSUPPORTED;
        for(unsigned i=0;i<length;++i)if(chain[i]==cursor)return FF_BOOT_UNSUPPORTED;
        chain[length++]=cursor;
        cursor=disk_word(fs,part->offset+4096+(uint64_t)cursor*width,width);
        if(cursor==last)break;
    }
    *mount=(ff_fatx_mount){.partition=partition,.serial=disk_word(fs,part->offset+4,4),.sectors_per_cluster=sectors,
        .cluster_bytes=cluster_bytes,.entry_bytes=width,.root_cluster=root,.cluster_count=count,.free_clusters=free_clusters,
        .root_clusters=length,.offset=part->offset,.bytes=part->bytes,.fat_bytes=fat,.data_offset=data,.revision=fs->disk->revision};
    return 0;
}
static unsigned fold(unsigned value){return value>='A' && value<='Z'?value+'a'-'A':value;}
static uint32_t resolve_root(ff_fatx_volume *fs,const char *path,uint32_t bytes,uint32_t flags,uint32_t *partition)
{
    static const char prefix[]="\\Device\\Harddisk0\\Partition";
    if(bytes && path[bytes-1]=='\\')--bytes;
    if(bytes>sizeof prefix-1 && bytes<=sizeof prefix+1) {
        unsigned i=0;
        for(;i<sizeof prefix-1;++i)if((flags&0x40u?fold((uint8_t)path[i]):(uint8_t)path[i])!=
            (flags&0x40u?fold((uint8_t)prefix[i]):(uint8_t)prefix[i]))break;
        if(i==sizeof prefix-1) {
            uint32_t number=0;
            if(path[i]=='0' && i+1<bytes)return FF_BOOT_UNSUPPORTED;
            for(;i<bytes;++i){if(path[i]<'0' || path[i]>'9')return FF_BOOT_UNSUPPORTED;number=number*10+path[i]-'0';}
            *partition=number;return 0;
        }
    }
    ff_namespace_resolution resolution;
    uint32_t status=ff_namespace_resolve(fs->space,path,bytes,&resolution);
    if(status)return FF_BOOT_UNSUPPORTED;
    for(unsigned i=0;i<FF_NS_OBJECTS;++i) {
        const ff_namespace_object *object=&fs->space->objects[i];
        if(object->id==resolution.object && object->kind==FF_NS_DEVICE && object->device_kind==FF_NS_LOCAL_FATX &&
           object->media_ready && !resolution.remaining_bytes) {
            const ff_fatx_mount *mount=mount_id(fs,object->provider_object);
            if(mount && mount->device==object->id){*partition=mount->partition;return 0;}
        }
    }
    return FF_BOOT_UNSUPPORTED;
}
uint32_t ff_fatx_open_root(ff_fatx_volume *fs,const uint32_t args[6])
{
    if(!fs || !fs->disk || !fs->space || !args)return FF_BOOT_INVALID;
    const uint32_t output=args[0],access=args[1],attributes=args[2],iosb=args[3],share=args[4],options=args[5];
    if((output|attributes|iosb)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,output,4,FF_VM_WRITE) || !admitted(fs,iosb,8,FF_VM_WRITE) || !admitted(fs,attributes,12,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    const uint32_t relative=get(fs,attributes,4),descriptor=get(fs,attributes+4,4),flags=get(fs,attributes+8,4);
    if((descriptor&3u) || !admitted(fs,descriptor,8,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    const uint32_t bytes=get(fs,descriptor,2),maximum=get(fs,descriptor+2,2),buffer=get(fs,descriptor+4,4);
    if(bytes>maximum || bytes>=FF_NS_PATH)return FF_BOOT_UNSUPPORTED;
    if(bytes && !admitted(fs,buffer,bytes,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    if(overlap(output,4,iosb,8) || overlap(output,4,attributes,12) || overlap(iosb,8,attributes,12) ||
       overlap(output,4,descriptor,8) || overlap(iosb,8,descriptor,8) || overlap(output,4,buffer,bytes) || overlap(iosb,8,buffer,bytes))return FF_BOOT_UNSUPPORTED;
    if((flags&~0x40u) || (share&~7u) || (access&~0x001200a9u) || (options&~0x00800061u))return FF_BOOT_UNSUPPORTED;
    if((options&0x20u) && !(access&0x100000u))return FF_BOOT_INVALID;
    if(options&0x40u)return FF_BOOT_UNSUPPORTED;
    uint32_t partition=0;
    if(relative) {
        ff_fatx_file *parent=file_handle(fs,relative);
        if(!parent)return FF_BOOT_UNSUPPORTED;
        if(bytes)return FF_BOOT_UNSUPPORTED;
        partition=mount_id(fs,parent->mount)->partition;
    } else {
        char path[FF_NS_PATH];if(bytes)memcpy(path,fs->disk->memory->ram+buffer,bytes);path[bytes]=0;
        uint32_t status=resolve_root(fs,path,bytes,flags,&partition);if(status)return status;
    }
    ff_fatx_mount *mounted=NULL,candidate;
    for(unsigned i=0;i<fs->mount_count;++i)if(fs->mounts[i].partition==partition)mounted=&fs->mounts[i];
    if(mounted && !current(fs,mounted))return FF_BOOT_UNSUPPORTED;
    for(unsigned i=0;i<FF_FATX_FILES;++i) {
        const ff_fatx_file *old=&fs->files[i];
        if(old->id && mounted && old->mount==mounted->id &&
           (((access&0x21u) && !(old->share&1u)) || ((old->access&0x21u) && !(share&1u))))return result(fs,iosb,FF_DISK_SHARING,0);
    }
    ff_fatx_file *slot=NULL;
    for(unsigned i=0;i<FF_FATX_FILES;++i)if(!fs->files[i].id){slot=&fs->files[i];break;}
    if(!slot || fs->next_handle>=FF_FATX_HANDLE_LIMIT || fs->next_file>=FF_FATX_FILE_LIMIT)return result(fs,iosb,FF_BOOT_NO_MEMORY,0);
    if(!mounted) {
        if(fs->mount_count==FF_FATX_MOUNTS)return FF_BOOT_UNSUPPORTED;
        uint32_t status=layout(fs,partition,&candidate);if(status)return status;
        candidate.id=fs->mount_count+1;
        status=ff_namespace_add_fatx_device(fs->space,partition,candidate.id,&candidate.device);
        if(status)return status==FF_BOOT_NO_MEMORY?result(fs,iosb,status,0):FF_BOOT_UNSUPPORTED;
        mounted=&fs->mounts[fs->mount_count++];*mounted=candidate;
    }
    *slot=(ff_fatx_file){fs->next_file++,mounted->id,fs->next_handle,access,share,options,1};
    fs->next_handle+=4;++fs->file_count;++fs->active;++fs->opened;++mounted->file_objects;
    put(fs,output,slot->handle,4);return result(fs,iosb,0,1);
}
uint32_t ff_fatx_query_size(ff_fatx_volume *fs,const uint32_t args[5])
{
    if(!fs || !fs->disk || !args)return FF_BOOT_INVALID;
    const ff_fatx_file *file=file_handle(fs,args[0]);if(!file)return FF_BOOT_BAD_HANDLE;
    const ff_fatx_mount *mount=mount_id(fs,file->mount);
    if(!current(fs,mount) || args[4]!=3)return FF_BOOT_UNSUPPORTED;
    const uint32_t iosb=args[1],output=args[2],bytes=args[3];
    if((iosb|output)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,iosb,8,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,output,bytes<24?bytes:24))return FF_BOOT_UNSUPPORTED;
    if(bytes<24)return result(fs,iosb,0xc0000023u,0);
    if(!admitted(fs,output,24,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    put(fs,output,mount->cluster_count,8);put(fs,output+8,mount->free_clusters,8);
    put(fs,output+16,mount->sectors_per_cluster,4);put(fs,output+20,fs->disk->profile.sector_bytes,4);
    return result(fs,iosb,0,24);
}
static void release_instance(ff_fatx_volume *fs,ff_fatx_file *file)
{
    --mount_id(fs,file->mount)->file_objects;--fs->file_count;memset(file,0,sizeof *file);
}
uint32_t ff_fatx_close(ff_fatx_volume *fs,uint32_t handle)
{
    ff_fatx_file *file=file_handle(fs,handle);if(!file)return FF_BOOT_BAD_HANDLE;
    file->handle=0;if(!--file->references)release_instance(fs,file);
    --fs->active;++fs->closed;return 0;
}
uint32_t ff_fatx_reference_handle(ff_fatx_volume *fs,uint32_t handle,uint32_t *token)
{
    if(!token)return FF_BOOT_INVALID;
    ff_fatx_file *file=file_handle(fs,handle);if(!file)return FF_BOOT_BAD_HANDLE;
    if(!current(fs,mount_id(fs,file->mount)) || file->references==UINT32_MAX)return FF_BOOT_UNSUPPORTED;
    ++file->references;*token=file->id;return 0;
}
uint32_t ff_fatx_release_file(ff_fatx_volume *fs,uint32_t token)
{
    if(!fs)return FF_BOOT_INVALID;
    for(unsigned i=0;i<FF_FATX_FILES;++i)if(fs->files[i].id==token && token) {
        ff_fatx_file *file=&fs->files[i];
        if(file->references<=(file->handle?1u:0u))return FF_BOOT_UNSUPPORTED;
        if(!--file->references)release_instance(fs,file);
        return 0;
    }
    return FF_BOOT_BAD_HANDLE;
}
static uint32_t current_mount(ff_fatx_volume *fs,uint32_t id,ff_fatx_mount **mount)
{
    ff_fatx_mount *m=mount_id(fs,id);
    if(!m || !current(fs,m))return FF_BOOT_UNSUPPORTED;
    *mount=m;return 0;
}
static uint32_t fat_next(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t cluster,uint32_t *next)
{
    const uint32_t last=mount->entry_bytes==2?0xffffu:UINT32_MAX;
    if(!cluster || cluster>mount->cluster_count)return FF_BOOT_UNSUPPORTED;
    const uint32_t value=disk_word(fs,mount->offset+4096+(uint64_t)cluster*mount->entry_bytes,mount->entry_bytes);
    if(value!=last && (!value || value>mount->cluster_count))return FF_BOOT_UNSUPPORTED;
    *next=value;return 0;
}
enum { FF_FATX_PARSE_OK=0,FF_FATX_PARSE_END=1,FF_FATX_PARSE_DELETED=2,FF_FATX_PARSE_INVALID=3 };
static uint32_t parse_entry(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint64_t address,
                            ff_fatx_directory_entry *entry)
{
    const uint32_t length=ff_disk_byte(fs->disk,address);
    if(length==0u || length==0xffu)return FF_FATX_PARSE_END;
    if(length==0xe5u)return FF_FATX_PARSE_DELETED;
    if(length>FF_FATX_NAME)return FF_FATX_PARSE_INVALID;
    const uint32_t attributes=ff_disk_byte(fs->disk,address+1);
    const uint32_t first=disk_word(fs,address+44,4),size=disk_word(fs,address+48,4);
    if(first>mount->cluster_count || ((attributes&FF_FATX_ATTR_DIRECTORY)&&!first) || (size&&!first))
        return FF_FATX_PARSE_INVALID;
    for(uint32_t i=0;i<length;++i) {
        const uint32_t value=ff_disk_byte(fs->disk,address+2+i);
        if(!value)return FF_FATX_PARSE_INVALID;
        entry->name[i]=(char)value;
    }
    entry->name[length]=0;entry->attributes=attributes;entry->first_cluster=first;entry->size=size;
    return FF_FATX_PARSE_OK;
}
static uint32_t directory_scan(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t start,
                               uint32_t max_clusters,uint32_t target,
                               ff_fatx_directory_entry *entry,uint32_t *count)
{
    uint32_t cluster=start,steps=0,seen=0;
    if(count)*count=0;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || steps++>=max_clusters)return FF_BOOT_UNSUPPORTED;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cluster-1)*mount->cluster_bytes;
        for(uint32_t offset=0;offset+64u<=mount->cluster_bytes;offset+=64u) {
            ff_fatx_directory_entry candidate;
            const uint32_t parsed=parse_entry(fs,mount,base+offset,&candidate);
            if(parsed==FF_FATX_PARSE_END) { if(count)*count=seen;return entry?FF_BOOT_UNSUPPORTED:0; }
            if(parsed==FF_FATX_PARSE_DELETED)continue;
            if(parsed!=FF_FATX_PARSE_OK)return FF_BOOT_UNSUPPORTED;
            if(entry && seen==target) {
                *entry=candidate;
                if(count)*count=seen+1u;
                return 0;
            }
            ++seen;
        }
        uint32_t next;
        if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;
        if(next==(mount->entry_bytes==2?0xffffu:UINT32_MAX)) { if(count)*count=seen;return entry?FF_BOOT_UNSUPPORTED:0; }
        cluster=next;
    }
}
static uint32_t root_scan(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t target,
                          ff_fatx_directory_entry *entry,uint32_t *count)
{
    return directory_scan(fs,mount,mount->root_cluster,FF_FATX_ROOT_CHAIN,target,entry,count);
}
uint32_t ff_fatx_root_count(ff_fatx_volume *fs,uint32_t id,uint32_t *count)
{
    ff_fatx_mount *mount;
    if(!fs || !count)return FF_BOOT_INVALID;
    uint32_t status=current_mount(fs,id,&mount);if(status)return status;
    return root_scan(fs,mount,0,NULL,count);
}
uint32_t ff_fatx_root_entry(ff_fatx_volume *fs,uint32_t id,uint32_t index,ff_fatx_directory_entry *entry)
{
    ff_fatx_mount *mount;
    if(!fs || !entry)return FF_BOOT_INVALID;
    uint32_t status=current_mount(fs,id,&mount);if(status)return status;
    return root_scan(fs,mount,index,entry,NULL);
}
uint32_t ff_fatx_read_entry(ff_fatx_volume *fs,uint32_t id,const ff_fatx_directory_entry *entry,
                            uint32_t offset,uint8_t *output,uint32_t capacity,uint32_t *bytes)
{
    ff_fatx_mount *mount;
    if(!fs || !entry || !bytes)return FF_BOOT_INVALID;
    *bytes=0;
    uint32_t status=current_mount(fs,id,&mount);if(status)return status;
    if(entry->attributes&FF_FATX_ATTR_DIRECTORY)return FF_BOOT_UNSUPPORTED;
    if(offset>entry->size)return FF_BOOT_INVALID;
    uint32_t remaining=entry->size-offset,size=remaining<capacity?remaining:capacity;
    if(!size)return 0;
    if(!output || !entry->first_cluster || entry->first_cluster>mount->cluster_count)return FF_BOOT_UNSUPPORTED;
    const uint32_t skip=offset/mount->cluster_bytes,intra=offset%mount->cluster_bytes;
    const uint64_t span=(uint64_t)intra+size;
    const uint32_t need=skip+(uint32_t)((span+mount->cluster_bytes-1u)/mount->cluster_bytes);
    uint32_t cluster=entry->first_cluster,length=0,next;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || ++length>mount->cluster_count)return FF_BOOT_UNSUPPORTED;
        if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;
        if(next==(mount->entry_bytes==2?0xffffu:UINT32_MAX))break;
        cluster=next;
    }
    if(length<need)return FF_BOOT_UNSUPPORTED;
    cluster=entry->first_cluster;
    for(uint32_t i=0;i<skip;++i) { if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;cluster=next; }
    uint32_t written=0,fragment=intra;
    while(written<size) {
        uint32_t chunk=size-written;
        const uint32_t available=mount->cluster_bytes-fragment;
        if(chunk>available)chunk=available;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cluster-1)*mount->cluster_bytes+fragment;
        for(uint32_t i=0;i<chunk;++i)output[written+i]=ff_disk_byte(fs->disk,base+i);
        written+=chunk;fragment=0;
        if(written<size) { if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;cluster=next; }
    }
    *bytes=written;return 0;
}
uint32_t ff_fatx_write_entry(ff_fatx_volume *fs,uint32_t id,const ff_fatx_directory_entry *entry,
                             uint32_t offset,const uint8_t *input,uint32_t bytes,uint32_t *written)
{
    ff_fatx_mount *mount;
    if(!fs || !entry || !written)return FF_BOOT_INVALID;
    *written=0;
    uint32_t status=current_mount(fs,id,&mount);if(status)return status;
    if(entry->attributes&FF_FATX_ATTR_DIRECTORY)return FF_BOOT_UNSUPPORTED;
    if(!bytes)return 0;
    if(!input || offset>entry->size || bytes>entry->size-offset)return FF_BOOT_INVALID;
    if(!entry->first_cluster || entry->first_cluster>mount->cluster_count)return FF_BOOT_UNSUPPORTED;
    const uint32_t skip=offset/mount->cluster_bytes,intra=offset%mount->cluster_bytes;
    const uint64_t span=(uint64_t)intra+bytes;
    const uint32_t need=skip+(uint32_t)((span+mount->cluster_bytes-1u)/mount->cluster_bytes);
    uint32_t cluster=entry->first_cluster,length=0,next;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || ++length>mount->cluster_count)return FF_BOOT_UNSUPPORTED;
        if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;
        if(next==(mount->entry_bytes==2?0xffffu:UINT32_MAX))break;
        cluster=next;
    }
    if(length<need)return FF_BOOT_UNSUPPORTED;
    cluster=entry->first_cluster;
    for(uint32_t i=0;i<skip;++i) { if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;cluster=next; }
    /* Preflight the whole destination before writing. The chunk count bounds
     * the revision increments so a multi-cluster write cannot overflow it
     * after the first chunk. */
    uint32_t cursor_cluster=cluster,fragment=intra,remaining=bytes;
    uint64_t chunks=0;
    while(remaining) {
        uint32_t chunk=mount->cluster_bytes-fragment;
        if(chunk>remaining)chunk=remaining;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cursor_cluster-1)*mount->cluster_bytes+fragment;
        if(!ff_disk_has(fs->disk,base,chunk))return FF_BOOT_UNSUPPORTED;
        ++chunks;remaining-=chunk;
        if(remaining) {
            if(fat_next(fs,mount,cursor_cluster,&next))return FF_BOOT_UNSUPPORTED;
            cursor_cluster=next;fragment=0;
        }
    }
    if(fs->disk->revision>UINT64_MAX-chunks)return FF_BOOT_UNSUPPORTED;
    cursor_cluster=cluster;fragment=intra;
    uint32_t done=0;
    while(done<bytes) {
        uint32_t chunk=mount->cluster_bytes-fragment;
        if(chunk>bytes-done)chunk=bytes-done;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cursor_cluster-1)*mount->cluster_bytes+fragment;
        status=ff_disk_store(fs->disk,base,input+done,chunk);
        if(status)return status;
        done+=chunk;fragment=0;
        if(done<bytes) {
            if(fat_next(fs,mount,cursor_cluster,&next))return FF_BOOT_UNSUPPORTED;
            cursor_cluster=next;
        }
    }
    /* A data-only overwrite leaves the mount geometry valid. */
    mount->revision=fs->disk->revision;
    *written=done;
    return 0;
}
static int name_equal(const char *entry_name,const uint8_t *path,uint32_t bytes,uint32_t flags)
{
    if(strlen(entry_name)!=bytes)return 0;
    for(uint32_t i=0;i<bytes;++i) {
        unsigned a=(uint8_t)entry_name[i],b=path[i];
        if(flags&0x40u){a=fold(a);b=fold(b);}
        if(a!=b)return 0;
    }
    return 1;
}
static uint32_t directory_find(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t start,
                               const uint8_t *path,uint32_t bytes,uint32_t flags,
                               ff_fatx_directory_entry *entry)
{
    uint32_t cluster=start,steps=0;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || steps++>=FF_FATX_DIRECTORY_CHAIN)return FF_BOOT_UNSUPPORTED;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cluster-1)*mount->cluster_bytes;
        for(uint32_t offset=0;offset+64u<=mount->cluster_bytes;offset+=64u) {
            ff_fatx_directory_entry candidate;
            const uint32_t parsed=parse_entry(fs,mount,base+offset,&candidate);
            if(parsed==FF_FATX_PARSE_END)return FF_BOOT_UNSUPPORTED;
            if(parsed==FF_FATX_PARSE_DELETED)continue;
            if(parsed!=FF_FATX_PARSE_OK)return FF_BOOT_UNSUPPORTED;
            if(name_equal(candidate.name,path,bytes,flags)){*entry=candidate;return 0;}
        }
        uint32_t next;
        if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;
        if(next==(mount->entry_bytes==2?0xffffu:UINT32_MAX))return FF_BOOT_UNSUPPORTED;
        cluster=next;
    }
}
uint32_t ff_fatx_lookup(ff_fatx_volume *fs,uint32_t id,const uint8_t *path,uint32_t path_bytes,
                        uint32_t flags,ff_fatx_directory_entry *entry)
{
    ff_fatx_mount *mount;
    if(!fs || !path || !entry || !path_bytes || path_bytes>FF_FATX_PATH_BYTES)return FF_BOOT_INVALID;
    if(flags&~0x40u)return FF_BOOT_UNSUPPORTED;
    uint32_t status=current_mount(fs,id,&mount);if(status)return status;
    uint32_t position=0,cluster=mount->root_cluster,depth=0;
    while(position<path_bytes) {
        uint32_t end=position;
        while(end<path_bytes && path[end]!='\\' && path[end]!='/')++end;
        const uint32_t length=end-position;
        if(!length || length>FF_FATX_NAME || depth++>=FF_FATX_PATH_DEPTH)return FF_BOOT_UNSUPPORTED;
        for(uint32_t i=0;i<length;++i)if(!path[position+i])return FF_BOOT_UNSUPPORTED;
        if((length==1 && path[position]=='.') ||
           (length==2 && path[position]=='.' && path[position+1]=='.'))return FF_BOOT_UNSUPPORTED;
        ff_fatx_directory_entry found;
        status=directory_find(fs,mount,cluster,path+position,length,flags,&found);
        if(status)return status;
        if(end==path_bytes){*entry=found;return 0;}
        if(!(found.attributes&FF_FATX_ATTR_DIRECTORY) || !found.first_cluster)return FF_BOOT_UNSUPPORTED;
        cluster=found.first_cluster;position=end+1;
        if(position==path_bytes)return FF_BOOT_UNSUPPORTED;
    }
    return FF_BOOT_UNSUPPORTED;
}
uint32_t ff_fatx_read_path(ff_fatx_volume *fs,uint32_t id,const uint8_t *path,uint32_t path_bytes,
                           uint32_t flags,uint32_t offset,uint8_t *output,uint32_t capacity,uint32_t *bytes)
{
    ff_fatx_directory_entry entry;
    uint32_t status=ff_fatx_lookup(fs,id,path,path_bytes,flags,&entry);
    if(status)return status;
    return ff_fatx_read_entry(fs,id,&entry,offset,output,capacity,bytes);
}
static uint32_t directory_start(ff_fatx_volume *fs,uint32_t id,ff_fatx_mount **mount,
                                const uint8_t *path,uint32_t path_bytes,uint32_t flags,uint32_t *start)
{
    if(!fs || !mount || !start || (flags&~0x40u) || path_bytes>FF_FATX_PATH_BYTES)return FF_BOOT_INVALID;
    uint32_t status=current_mount(fs,id,mount);if(status)return status;
    if(!path_bytes) { *start=(*mount)->root_cluster;return 0; }
    if(!path)return FF_BOOT_INVALID;
    ff_fatx_directory_entry entry;
    status=ff_fatx_lookup(fs,id,path,path_bytes,flags,&entry);
    if(status)return status;
    if(!(entry.attributes&FF_FATX_ATTR_DIRECTORY) || !entry.first_cluster)return FF_BOOT_UNSUPPORTED;
    *start=entry.first_cluster;return 0;
}
uint32_t ff_fatx_directory_count(ff_fatx_volume *fs,uint32_t id,const uint8_t *path,uint32_t path_bytes,
                                 uint32_t flags,uint32_t *count)
{
    ff_fatx_mount *mount;
    if(!count)return FF_BOOT_INVALID;
    uint32_t start;
    uint32_t status=directory_start(fs,id,&mount,path,path_bytes,flags,&start);
    if(status)return status;
    return directory_scan(fs,mount,start,FF_FATX_DIRECTORY_CHAIN,0,NULL,count);
}
uint32_t ff_fatx_directory_at(ff_fatx_volume *fs,uint32_t id,const uint8_t *path,uint32_t path_bytes,
                              uint32_t flags,uint32_t index,ff_fatx_directory_entry *entry)
{
    ff_fatx_mount *mount;
    if(!entry)return FF_BOOT_INVALID;
    uint32_t start;
    uint32_t status=directory_start(fs,id,&mount,path,path_bytes,flags,&start);
    if(status)return status;
    return directory_scan(fs,mount,start,FF_FATX_DIRECTORY_CHAIN,index,entry,NULL);
}
uint32_t ff_fatx_create_empty(ff_fatx_volume *fs,uint32_t id,
                              const uint8_t *directory_path,uint32_t path_bytes,uint32_t flags,
                              const uint8_t *name,uint32_t name_bytes,
                              ff_fatx_directory_entry *created)
{
    if(!fs || !name || !name_bytes || name_bytes>FF_FATX_NAME || !created)return FF_BOOT_INVALID;
    for(uint32_t i=0;i<name_bytes;++i)
        if(!name[i] || name[i]=='\\' || name[i]=='/')return FF_BOOT_INVALID;
    if((name_bytes==1 && name[0]=='.') ||
       (name_bytes==2 && name[0]=='.' && name[1]=='.'))return FF_BOOT_INVALID;
    ff_fatx_mount *mount;
    uint32_t start;
    uint32_t status=directory_start(fs,id,&mount,directory_path,path_bytes,flags,&start);
    if(status)return status;
    uint32_t cluster=start,steps=0;
    int have_free=0,end=0;
    uint64_t free_address=0;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || steps++>=FF_FATX_DIRECTORY_CHAIN)return FF_BOOT_UNSUPPORTED;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cluster-1)*mount->cluster_bytes;
        for(uint32_t offset=0;offset+64u<=mount->cluster_bytes;offset+=64u) {
            ff_fatx_directory_entry candidate;
            const uint32_t parsed=parse_entry(fs,mount,base+offset,&candidate);
            if(parsed==FF_FATX_PARSE_END) {
                if(!have_free){free_address=base+offset;have_free=1;}
                end=1;break;
            }
            if(parsed==FF_FATX_PARSE_DELETED) {
                if(!have_free){free_address=base+offset;have_free=1;}
                continue;
            }
            if(parsed!=FF_FATX_PARSE_OK)return FF_BOOT_UNSUPPORTED;
            if(name_equal(candidate.name,name,name_bytes,flags))return FF_BOOT_UNSUPPORTED;
        }
        if(end)break;
        uint32_t next;
        if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;
        if(next==(mount->entry_bytes==2?0xffffu:UINT32_MAX))break;
        cluster=next;
    }
    if(!have_free)return FF_BOOT_NO_MEMORY;
    if(!ff_disk_has(fs->disk,free_address,64u))return FF_BOOT_UNSUPPORTED;
    uint8_t entry[64];
    memset(entry,0xFF,sizeof entry);
    entry[0]=(uint8_t)name_bytes;
    entry[1]=0x20u; /* archive */
    memcpy(entry+2,name,name_bytes);
    memset(entry+44,0,4);
    memset(entry+48,0,4);
    status=ff_disk_store(fs->disk,free_address,entry,64u);
    if(status)return status;
    /* A metadata-only creation leaves the mount geometry valid. */
    mount->revision=fs->disk->revision;
    created->name[0]=0;
    memcpy(created->name,name,name_bytes);
    created->name[name_bytes]=0;
    created->attributes=0x20u;
    created->first_cluster=0;
    created->size=0;
    return 0;
}
uint32_t ff_fatx_delete_empty(ff_fatx_volume *fs,uint32_t id,
                              const uint8_t *path,uint32_t path_bytes,uint32_t flags)
{
    if(!fs || !path || !path_bytes || path_bytes>FF_FATX_PATH_BYTES)return FF_BOOT_INVALID;
    if(path[0]=='\\' || path[0]=='/')return FF_BOOT_INVALID;
    uint32_t parent_bytes=0,name_offset=0;
    for(uint32_t i=path_bytes;i>0;--i)
        if(path[i-1]=='\\' || path[i-1]=='/'){parent_bytes=i-1;name_offset=i;break;}
    const uint8_t *name=path+name_offset;
    const uint32_t name_bytes=path_bytes-name_offset;
    if(!name_bytes || name_bytes>FF_FATX_NAME)return FF_BOOT_INVALID;
    for(uint32_t i=0;i<name_bytes;++i)
        if(!name[i] || name[i]=='\\' || name[i]=='/')return FF_BOOT_INVALID;
    if((name_bytes==1 && name[0]=='.') ||
       (name_bytes==2 && name[0]=='.' && name[1]=='.'))return FF_BOOT_INVALID;
    ff_fatx_mount *mount;
    uint32_t start;
    uint32_t status=directory_start(fs,id,&mount,parent_bytes?path:NULL,parent_bytes,flags,&start);
    if(status)return status;
    uint32_t cluster=start,steps=0;
    int found=0;
    uint64_t address=0;
    ff_fatx_directory_entry target;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || steps++>=FF_FATX_DIRECTORY_CHAIN)return FF_BOOT_UNSUPPORTED;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cluster-1)*mount->cluster_bytes;
        for(uint32_t offset=0;offset+64u<=mount->cluster_bytes;offset+=64u) {
            ff_fatx_directory_entry candidate;
            const uint32_t parsed=parse_entry(fs,mount,base+offset,&candidate);
            if(parsed==FF_FATX_PARSE_END)return FF_BOOT_UNSUPPORTED;
            if(parsed==FF_FATX_PARSE_DELETED)continue;
            if(parsed!=FF_FATX_PARSE_OK)return FF_BOOT_UNSUPPORTED;
            if(name_equal(candidate.name,name,name_bytes,flags)) {
                address=base+offset;target=candidate;found=1;break;
            }
        }
        if(found)break;
        uint32_t next;
        if(fat_next(fs,mount,cluster,&next))return FF_BOOT_UNSUPPORTED;
        if(next==(mount->entry_bytes==2?0xffffu:UINT32_MAX))return FF_BOOT_UNSUPPORTED;
        cluster=next;
    }
    if(!found)return FF_BOOT_UNSUPPORTED;
    if((target.attributes&FF_FATX_ATTR_DIRECTORY) || target.size || target.first_cluster)
        return FF_BOOT_UNSUPPORTED;
    if(!ff_disk_has(fs->disk,address,1u))return FF_BOOT_UNSUPPORTED;
    const uint8_t deleted=0xe5u;
    status=ff_disk_store(fs->disk,address,&deleted,1u);
    if(status)return status;
    /* A metadata-only deletion leaves the mount geometry valid. */
    mount->revision=fs->disk->revision;
    return 0;
}
