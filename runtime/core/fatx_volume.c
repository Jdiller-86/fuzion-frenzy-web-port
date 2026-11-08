#include "fatx_volume.h"
#include "fatx_directory_cursor.h"
#include "thread_bootstrap.h"
#include <string.h>

enum { FF_FATX_HANDLE_ROOT=0, FF_FATX_HANDLE_FILE=1 };
static uint32_t create_empty_internal(ff_fatx_volume *,uint32_t,const uint8_t *,uint32_t,uint32_t,
                                      const uint8_t *,uint32_t,ff_fatx_directory_entry *,uint64_t *);
static uint32_t reopen_root_file(ff_fatx_volume *,const ff_fatx_mount *,const uint8_t *,uint32_t,uint64_t *);

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
    if((flags&~0x40u) || (share&~7u) || (access&~0x001200a9u) ||
       (options&~0x00804061u))return FF_BOOT_UNSUPPORTED;
    /* NtOpenFile's reached cache-root request includes FILE_OPEN_FOR_BACKUP_INTENT
     * (0x4000). Admit only this observed DOS-root profile; the synthetic local
     * FATX provider does not model privilege, ACL, or general backup semantics. */
    const int dos_cache_root=relative==FF_FATX_DOS_DEVICE_ROOT && bytes==3 &&
        (options&0x00804061u)==0x00004021u && flags==0x40u && access==0x00100001u && share==3u;
    if(options&0x4000u) {
        if(!dos_cache_root)return FF_BOOT_UNSUPPORTED;
    } else if(options&~0x00800061u)return FF_BOOT_UNSUPPORTED;
    if((options&0x20u) && !(access&0x100000u))return FF_BOOT_INVALID;
    if(options&0x40u)return FF_BOOT_UNSUPPORTED;
    uint32_t partition=0;
    if(dos_cache_root) {
        const char *drive=(const char *)(fs->disk->memory->ram+buffer);
        if(!((drive[0]>='A'&&drive[0]<='Z')||(drive[0]>='a'&&drive[0]<='z')) ||
           drive[1]!=':' || drive[2]!='\\')return FF_BOOT_UNSUPPORTED;
        char device_path[16]="\\??\\X:";
        device_path[4]=(char)fold((uint8_t)drive[0]);
        ff_namespace_resolution resolution;
        if(ff_namespace_resolve(fs->space,device_path,6,&resolution) || resolution.remaining_bytes)
            return FF_BOOT_UNSUPPORTED;
        const ff_namespace_object *device=NULL;
        for(unsigned i=0;i<FF_NS_OBJECTS;++i)if(fs->space->objects[i].id==resolution.object)device=&fs->space->objects[i];
        if(!device || device->kind!=FF_NS_DEVICE || device->device_kind!=FF_NS_LOCAL_FATX || !device->media_ready)
            return FF_BOOT_UNSUPPORTED;
        ff_fatx_mount *owned=mount_id(fs,device->provider_object);
        if(!owned || owned->device!=device->id || !current(fs,owned))return FF_BOOT_UNSUPPORTED;
        partition=owned->partition;
    } else if(relative) {
        ff_fatx_file *parent=file_handle(fs,relative);
        if(!parent || parent->kind!=FF_FATX_HANDLE_ROOT)return FF_BOOT_UNSUPPORTED;
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
        if(old->id && old->kind==FF_FATX_HANDLE_ROOT && mounted && old->mount==mounted->id &&
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
    *slot=(ff_fatx_file){.id=fs->next_file++,.mount=mounted->id,.handle=fs->next_handle,
        .access=access,.share=share,.options=options,.references=1,.kind=FF_FATX_HANDLE_ROOT};
    fs->next_handle+=4;++fs->file_count;++fs->active;++fs->opened;++mounted->file_objects;
    put(fs,output,slot->handle,4);return result(fs,iosb,0,1);
}
uint32_t ff_fatx_create_file_guest(ff_fatx_volume *fs,const uint32_t args[9])
{
    if(!fs || !fs->disk || !fs->space || !args)return FF_BOOT_INVALID;
    const uint32_t output=args[0],access=args[1],attributes=args[2],iosb=args[3],allocation=args[4];
    if((output|attributes|iosb)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,output,4,FF_VM_WRITE) || !admitted(fs,iosb,8,FF_VM_WRITE) ||
       !admitted(fs,attributes,12,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    if(overlap(output,4,iosb,8) || overlap(output,4,attributes,12) || overlap(iosb,8,attributes,12))
        return FF_BOOT_UNSUPPORTED;
    const uint32_t root=get(fs,attributes,4),descriptor=get(fs,attributes+4,4),flags=get(fs,attributes+8,4);
    if(root!=FF_FATX_DOS_DEVICE_ROOT || (descriptor&3u) || !admitted(fs,descriptor,8,FF_VM_READ))
        return FF_BOOT_UNSUPPORTED;
    const uint32_t name_bytes=get(fs,descriptor,2),maximum=get(fs,descriptor+2,2),buffer=get(fs,descriptor+4,4);
    if(overlap(output,4,descriptor,8) || overlap(iosb,8,descriptor,8) ||
       overlap(output,4,buffer,name_bytes) || overlap(iosb,8,buffer,name_bytes))
        return FF_BOOT_UNSUPPORTED;
    if(name_bytes>maximum || name_bytes>FF_FATX_NAME+3u || !name_bytes ||
       (name_bytes && !admitted(fs,buffer,name_bytes,FF_VM_READ)))
        return FF_BOOT_UNSUPPORTED;
    /* This service slice admits only the observed X:\\name request shape. */
    const uint32_t disposition=args[7];
    if(access!=0x40100080u || allocation || args[5]!=0x80u || args[6]!=3u ||
       (disposition!=3u && disposition!=5u) || args[8]!=0x60u || flags!=0x40u || name_bytes<4u)
        return FF_BOOT_UNSUPPORTED;
    const uint8_t *guest=fs->disk->memory->ram+buffer;
    if(!((guest[0]>='A' && guest[0]<='Z') || (guest[0]>='a' && guest[0]<='z')) ||
       guest[1]!=':' || guest[2]!='\\' || guest[3]=='\\' || guest[3]=='/' || guest[3]==':' ||
       guest[3]=='.' || guest[3]==' ')
        return FF_BOOT_UNSUPPORTED;
    char dos_root[7]="\\??\\X:";
    dos_root[4]=(char)guest[0];
    ff_namespace_resolution resolution;
    if(ff_namespace_resolve(fs->space,dos_root,6,&resolution) || resolution.remaining_bytes)
        return FF_BOOT_UNSUPPORTED;
    const ff_namespace_object *device=NULL;
    for(unsigned i=0;i<FF_NS_OBJECTS;++i)
        if(fs->space->objects[i].id==resolution.object)device=&fs->space->objects[i];
    if(!device || device->kind!=FF_NS_DEVICE || device->device_kind!=FF_NS_LOCAL_FATX || !device->media_ready)
        return FF_BOOT_UNSUPPORTED;
    ff_fatx_mount *mount=mount_id(fs,device->provider_object);
    if(!mount || mount->device!=device->id || !current(fs,mount) || mount->root_clusters!=1u)
        return FF_BOOT_UNSUPPORTED;
    ff_fatx_file *slot=NULL;
    for(unsigned i=0;i<FF_FATX_FILES;++i)if(!fs->files[i].id){slot=&fs->files[i];break;}
    if(!slot || fs->next_handle>=FF_FATX_HANDLE_LIMIT || fs->next_file>=FF_FATX_FILE_LIMIT ||
       fs->active==UINT32_MAX || fs->opened==UINT32_MAX || mount->file_objects==UINT32_MAX)
        return result(fs,iosb,FF_BOOT_NO_MEMORY,0);
    const uint8_t *name=guest+3;
    const uint32_t file_name_bytes=name_bytes-3u;
    if(!file_name_bytes || file_name_bytes>FF_FATX_NAME || name[0]=='.' || name[0]==' ')
        return FF_BOOT_UNSUPPORTED;
    for(uint32_t i=0;i<file_name_bytes;++i) {
        const uint8_t c=name[i];
        if(!((c>='A'&&c<='Z') || (c>='a'&&c<='z') || (c>='0'&&c<='9') ||
             c=='.' || c=='_' || c=='-'))return FF_BOOT_UNSUPPORTED;
    }
    uint64_t entry_address;
    uint32_t information=2u; /* FILE_CREATED */
    if(disposition==3u) {
        /* File objects lack pathname identity, including after a handle closes
         * with a retained reference. Refuse ambiguous sharing on this mount. */
        for(unsigned i=0;i<FF_FATX_FILES;++i)
            if(fs->files[i].id && fs->files[i].kind==FF_FATX_HANDLE_FILE && fs->files[i].mount==mount->id)
                return FF_BOOT_UNSUPPORTED;
        const uint32_t status=reopen_root_file(fs,mount,name,file_name_bytes,&entry_address);
        if(status)return status;
        information=1u; /* FILE_OPENED */
    } else {
        ff_fatx_directory_entry created;
        const uint32_t status=create_empty_internal(fs,mount->id,NULL,0,0x40u,name,file_name_bytes,&created,&entry_address);
        if(status)return status==FF_BOOT_NO_MEMORY?result(fs,iosb,status,0):status;
    }
    *slot=(ff_fatx_file){.id=fs->next_file++,.mount=mount->id,.handle=fs->next_handle,
        .access=access,.share=args[6],.options=args[8],.references=1,.kind=FF_FATX_HANDLE_FILE,
        .entry_address=entry_address,.open_revision=mount->revision};
    fs->next_handle+=4u;++fs->file_count;++fs->active;++fs->opened;++mount->file_objects;
    put(fs,output,slot->handle,4);
    return result(fs,iosb,0,information);
}
uint32_t ff_fatx_query_size(ff_fatx_volume *fs,const uint32_t args[5])
{
    if(!fs || !fs->disk || !args)return FF_BOOT_INVALID;
    const ff_fatx_file *file=file_handle(fs,args[0]);if(!file)return FF_BOOT_BAD_HANDLE;
    if(file->kind!=FF_FATX_HANDLE_ROOT)return FF_BOOT_UNSUPPORTED;
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
static int single_cluster_file(ff_fatx_volume *fs,const ff_fatx_mount *mount,const ff_fatx_directory_entry *entry)
{
    if(entry->attributes!=0x20u)return 0;
    if(!entry->size)return !entry->first_cluster;
    if(entry->size>mount->cluster_bytes || entry->first_cluster<2u ||
       entry->first_cluster==mount->root_cluster)return 0;
    const uint64_t fat_address=mount->offset+4096+(uint64_t)entry->first_cluster*mount->entry_bytes;
    return ff_disk_has(fs->disk,fat_address,mount->entry_bytes) &&
        disk_word(fs,fat_address,mount->entry_bytes)==(mount->entry_bytes==2?0xffffu:UINT32_MAX);
}
static int checked_file_entry(ff_fatx_volume *fs,const ff_fatx_file *file,ff_fatx_directory_entry *entry)
{
    const ff_fatx_mount *mount=mount_id(fs,file->mount);
    if(file->kind!=FF_FATX_HANDLE_FILE || !current(fs,mount) || file->open_revision!=mount->revision)return 0;
    const uint64_t address=file->entry_address;
    return ff_disk_has(fs->disk,address,64u) && parse_entry(fs,mount,address,entry)==FF_FATX_PARSE_OK &&
        single_cluster_file(fs,mount,entry);
}
uint32_t ff_fatx_query_file(ff_fatx_volume *fs,const uint32_t args[5])
{
    if(!fs || !fs->disk || !args)return FF_BOOT_INVALID;
    const ff_fatx_file *file=file_handle(fs,args[0]);if(!file)return FF_BOOT_BAD_HANDLE;
    ff_fatx_directory_entry entry;
    if(!(file->access&0x80u) || args[4]!=34u || !checked_file_entry(fs,file,&entry))return FF_BOOT_UNSUPPORTED;
    /* Owned create leaves all three on-disk timestamp words unset (all ones).
     * This profile exposes zero guest times for that placeholder only. */
    for(uint32_t offset=52u;offset<64u;offset+=4u)
        if(disk_word(fs,file->entry_address+offset,4)!=UINT32_MAX)return FF_BOOT_UNSUPPORTED;
    const uint32_t iosb=args[1],output=args[2],bytes=args[3],info_bytes=56u;
    if((iosb|output)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,iosb,8,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,output,bytes<info_bytes?bytes:info_bytes))return FF_BOOT_UNSUPPORTED;
    if(bytes<info_bytes)return result(fs,iosb,0xc0000023u,0);
    if(!admitted(fs,output,info_bytes,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    memset(fs->disk->memory->ram+output,0,info_bytes);
    put(fs,output+32u,entry.size?mount_id(fs,file->mount)->cluster_bytes:0,8);
    put(fs,output+40u,entry.size,8);
    put(fs,output+48u,entry.attributes,4);
    return result(fs,iosb,0,info_bytes);
}
static void record_put(uint8_t *record,uint32_t offset,uint64_t value,unsigned bytes)
{
    for(unsigned i=0;i<bytes;++i)record[offset+i]=(uint8_t)(value>>(8*i));
}
uint32_t ff_fatx_query_directory_first(ff_fatx_volume *fs,const uint32_t args[10])
{
    if(!fs || !fs->disk || !fs->space || !args || !fs->disk->memory ||
       fs->space->memory!=fs->disk->memory)return FF_BOOT_INVALID;
    ff_fatx_file *file=file_handle(fs,args[0]);
    if(!file || !ff_fatx_owns_handle(args[0]))return FF_BOOT_BAD_HANDLE;
    if(file->kind!=FF_FATX_HANDLE_ROOT || file->handle!=args[0] || file->references!=1u ||
       file->access!=0x00100001u || args[1] || args[2] || args[3] ||
       args[6]!=0x148u || args[7]!=1u || args[9])return FF_BOOT_UNSUPPORTED;

    const uint32_t iosb=args[4],output=args[5],descriptor=args[8];
    if((iosb|output|descriptor)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,iosb,8,FF_VM_WRITE) || !admitted(fs,output,0x148u,FF_VM_WRITE) ||
       !admitted(fs,descriptor,8,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    const uint32_t mask_buffer=get(fs,descriptor+4u,4);
    if(!mask_buffer || !admitted(fs,mask_buffer,3,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,output,0x148u) || overlap(iosb,8,descriptor,8) ||
       overlap(iosb,8,mask_buffer,3) || overlap(output,0x148u,descriptor,8) ||
       overlap(output,0x148u,mask_buffer,3) || overlap(descriptor,8,mask_buffer,3))
        return FF_BOOT_UNSUPPORTED;
    if(get(fs,descriptor,2)!=0u || get(fs,descriptor+2u,2)!=3u ||
       memcmp(fs->disk->memory->ram+mask_buffer,"*.*",3))return FF_BOOT_UNSUPPORTED;

    ff_fatx_mount *mount=mount_id(fs,file->mount);
    if(!mount || mount->id!=file->mount || !current(fs,mount) || mount->root_cluster!=1u ||
       mount->root_clusters!=1u || mount->cluster_bytes!=16384u || mount->entry_bytes!=4u)
        return FF_BOOT_UNSUPPORTED;
    const ff_namespace_object *device=NULL;
    for(unsigned i=0;i<FF_NS_OBJECTS;++i)
        if(fs->space->objects[i].id==mount->device)device=&fs->space->objects[i];
    if(!device || device->kind!=FF_NS_DEVICE || device->device_kind!=FF_NS_LOCAL_FATX ||
       !device->media_ready || device->provider_object!=mount->id)return FF_BOOT_UNSUPPORTED;

    if(mount->offset>UINT64_MAX-mount->data_offset)return FF_BOOT_UNSUPPORTED;
    const uint64_t root_address=mount->offset+mount->data_offset;
    const uint64_t root_fat_relative=4096u+(uint64_t)mount->root_cluster*mount->entry_bytes;
    if(root_address<mount->offset || root_address-mount->offset>mount->bytes ||
       mount->cluster_bytes>mount->bytes-(root_address-mount->offset) ||
       root_fat_relative>mount->bytes || mount->entry_bytes>mount->bytes-root_fat_relative)
        return FF_BOOT_UNSUPPORTED;
    const uint64_t root_fat=mount->offset+root_fat_relative;
    if(root_fat<mount->offset || !ff_disk_has(fs->disk,root_address,mount->cluster_bytes) ||
       !ff_disk_has(fs->disk,root_fat,mount->entry_bytes) ||
       disk_word(fs,root_fat,mount->entry_bytes)!=UINT32_MAX)
        return FF_BOOT_UNSUPPORTED;

    uint32_t count=0;
    if(ff_fatx_directory_count(fs,mount->id,NULL,0,0,&count) || count!=1u)return FF_BOOT_UNSUPPORTED;
    ff_fatx_directory_entry entry;
    if(ff_fatx_directory_at(fs,mount->id,NULL,0,0,0,&entry))return FF_BOOT_UNSUPPORTED;
    const uint32_t name_bytes=ff_disk_byte(fs->disk,root_address);
    if(name_bytes!=12u || entry.attributes!=0x20u || entry.first_cluster!=2u || entry.size!=586u ||
       ff_disk_byte(fs->disk,root_address+1u)!=0x20u ||
       ff_disk_byte(fs->disk,root_address+64u)!=0xffu || !single_cluster_file(fs,mount,&entry))
        return FF_BOOT_UNSUPPORTED;
    for(uint32_t i=0;i<name_bytes;++i) {
        const uint8_t c=(uint8_t)entry.name[i];
        if(ff_disk_byte(fs->disk,root_address+2u+i)!=c ||
           !((c>='A'&&c<='Z') || (c>='a'&&c<='z') || (c>='0'&&c<='9') ||
             c=='.' || c=='_' || c=='-'))return FF_BOOT_UNSUPPORTED;
    }
    if(entry.name[name_bytes])return FF_BOOT_UNSUPPORTED;
    for(uint32_t offset=52u;offset<64u;offset+=4u)
        if(disk_word(fs,root_address+offset,4)!=UINT32_MAX)return FF_BOOT_UNSUPPORTED;

    uint8_t record[0x40u+12u]={0};
    record_put(record,0x28u,entry.size,8);
    record_put(record,0x30u,mount->cluster_bytes,8);
    record_put(record,0x38u,entry.attributes,4);
    record_put(record,0x3cu,name_bytes,4);
    memcpy(record+0x40u,entry.name,name_bytes);
    memcpy(fs->disk->memory->ram+output,record,sizeof record);
    return result(fs,iosb,0,(uint32_t)sizeof record);
}
uint32_t ff_fatx_set_position(ff_fatx_volume *fs,const uint32_t args[5])
{
    if(!fs || !fs->disk || !args)return FF_BOOT_INVALID;
    ff_fatx_file *file=file_handle(fs,args[0]);if(!file)return FF_BOOT_BAD_HANDLE;
    ff_fatx_directory_entry entry;
    if(file->options!=0x60u || args[3]!=8u || args[4]!=14u || !checked_file_entry(fs,file,&entry))
        return FF_BOOT_UNSUPPORTED;
    const uint32_t iosb=args[1],input=args[2];
    if((iosb|input)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,iosb,8,FF_VM_WRITE) || !admitted(fs,input,8,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,input,8))return FF_BOOT_UNSUPPORTED;
    const uint32_t position=get(fs,input,4);
    if(get(fs,input+4u,4) || position>entry.size)return FF_BOOT_UNSUPPORTED;
    file->position=position;
    return result(fs,iosb,0,0);
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
static uint32_t reopen_root_file(ff_fatx_volume *fs,const ff_fatx_mount *mount,
                                       const uint8_t *name,uint32_t bytes,uint64_t *address)
{
    const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(mount->root_cluster-1u)*mount->cluster_bytes;
    uint64_t match=0;
    /* The caller admits a current single-cluster root. Check every occupied
     * entry for ambiguity and require backing even for the end marker. */
    for(uint32_t offset=0;offset+64u<=mount->cluster_bytes;offset+=64u) {
        if(!ff_disk_has(fs->disk,base+offset,64u))return FF_BOOT_UNSUPPORTED;
        ff_fatx_directory_entry entry;
        const uint32_t parsed=parse_entry(fs,mount,base+offset,&entry);
        if(parsed==FF_FATX_PARSE_END)break;
        if(parsed==FF_FATX_PARSE_DELETED)continue;
        if(parsed!=FF_FATX_PARSE_OK)return FF_BOOT_UNSUPPORTED;
        if(!name_equal(entry.name,name,bytes,0x40u))continue;
        if(match || !single_cluster_file(fs,mount,&entry))return FF_BOOT_UNSUPPORTED;
        match=base+offset;
    }
    if(!match)return FF_BOOT_UNSUPPORTED;
    *address=match;
    return 0;
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
static int new_name_valid(const uint8_t *name,uint32_t bytes)
{
    if(!name || !bytes || bytes>FF_FATX_NAME)return 0;
    for(uint32_t i=0;i<bytes;++i)
        if(!name[i] || name[i]=='\\' || name[i]=='/')return 0;
    if((bytes==1 && name[0]=='.') || (bytes==2 && name[0]=='.' && name[1]=='.'))return 0;
    return 1;
}
static uint32_t directory_free_slot(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t start,
                                    const uint8_t *name,uint32_t name_bytes,uint32_t flags,
                                    uint64_t *address)
{
    uint32_t cluster=start,steps=0;
    int have_free=0,end=0;
    uint64_t found_address=0;
    for(;;) {
        if(!cluster || cluster>mount->cluster_count || steps++>=FF_FATX_DIRECTORY_CHAIN)return FF_BOOT_UNSUPPORTED;
        const uint64_t base=mount->offset+mount->data_offset+(uint64_t)(cluster-1)*mount->cluster_bytes;
        for(uint32_t offset=0;offset+64u<=mount->cluster_bytes;offset+=64u) {
            ff_fatx_directory_entry candidate;
            const uint32_t parsed=parse_entry(fs,mount,base+offset,&candidate);
            if(parsed==FF_FATX_PARSE_END) {
                if(!have_free){found_address=base+offset;have_free=1;}
                end=1;break;
            }
            if(parsed==FF_FATX_PARSE_DELETED) {
                if(!have_free){found_address=base+offset;have_free=1;}
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
    *address=found_address;
    return 0;
}
static uint32_t write_entry(ff_fatx_volume *fs,uint64_t address,const uint8_t *name,uint32_t name_bytes,
                            uint32_t first_cluster,uint32_t size,ff_fatx_directory_entry *created)
{
    if(!ff_disk_has(fs->disk,address,64u))return FF_BOOT_UNSUPPORTED;
    uint8_t entry[64];
    memset(entry,0xFF,sizeof entry);
    entry[0]=(uint8_t)name_bytes;
    entry[1]=0x20u; /* archive */
    memcpy(entry+2,name,name_bytes);
    for(unsigned i=0;i<4;++i) {
        entry[44+i]=(uint8_t)(first_cluster>>(8*i));
        entry[48+i]=(uint8_t)(size>>(8*i));
    }
    uint32_t status=ff_disk_store(fs->disk,address,entry,64u);
    if(status)return status;
    created->name[0]=0;
    memcpy(created->name,name,name_bytes);
    created->name[name_bytes]=0;
    created->attributes=0x20u;
    created->first_cluster=first_cluster;
    created->size=size;
    return 0;
}
static uint32_t fat_set(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t cluster,uint32_t value)
{
    const uint64_t address=mount->offset+4096+(uint64_t)cluster*mount->entry_bytes;
    if(!ff_disk_has(fs->disk,address,mount->entry_bytes))return FF_BOOT_UNSUPPORTED;
    uint8_t bytes[4];
    for(uint32_t i=0;i<mount->entry_bytes;++i)bytes[i]=(uint8_t)(value>>(8*i));
    return ff_disk_store(fs->disk,address,bytes,mount->entry_bytes);
}
static uint32_t find_free_cluster(ff_fatx_volume *fs,const ff_fatx_mount *mount,uint32_t *cluster)
{
    for(uint32_t c=2;c<=mount->cluster_count;++c)
        if(!disk_word(fs,mount->offset+4096+(uint64_t)c*mount->entry_bytes,mount->entry_bytes)) {
            *cluster=c;return 0;
        }
    return FF_BOOT_NO_MEMORY;
}
static uint32_t store_first_file_cluster(ff_fatx_volume *fs,const ff_fatx_mount *mount,
                                         uint64_t entry,const uint8_t *data,uint32_t bytes,uint32_t *cluster)
{
    uint32_t status=find_free_cluster(fs,mount,cluster);
    if(status)return status;
    const uint64_t fat_address=mount->offset+4096+(uint64_t)*cluster*mount->entry_bytes;
    /* Preflight both metadata stores and all three revision increments. The
     * allocation helper checks its page budget before touching disk state. */
    if(!ff_disk_has(fs->disk,entry,64u) || !ff_disk_has(fs->disk,fat_address,mount->entry_bytes) ||
       fs->disk->revision>UINT64_MAX-3u)return FF_BOOT_UNSUPPORTED;
    const uint64_t data_address=mount->offset+mount->data_offset+(uint64_t)(*cluster-1)*mount->cluster_bytes;
    status=ff_disk_store_allocate(fs->disk,data_address,data,bytes);
    if(status)return status;
    return fat_set(fs,mount,*cluster,mount->entry_bytes==2?0xffffu:UINT32_MAX);
}
uint32_t ff_fatx_write_file_guest(ff_fatx_volume *fs,const uint32_t args[8])
{
    if(!fs || !fs->disk || !args)return FF_BOOT_INVALID;
    ff_fatx_file *file=file_handle(fs,args[0]);if(!file)return FF_BOOT_BAD_HANDLE;
    ff_fatx_directory_entry entry;
    if(args[1] || args[2] || args[3] || args[7] || file->options!=0x60u ||
       !(file->access&0x40000000u) || !checked_file_entry(fs,file,&entry) || file->position!=entry.size)
        return FF_BOOT_UNSUPPORTED;
    ff_fatx_mount *mount=mount_id(fs,file->mount);
    const uint32_t iosb=args[4],input=args[5],bytes=args[6];
    if(!bytes || bytes>mount->cluster_bytes-entry.size)return FF_BOOT_UNSUPPORTED;
    if(iosb&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,iosb,8,FF_VM_WRITE) || !admitted(fs,input,bytes,FF_VM_READ))
        return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,input,bytes))return FF_BOOT_UNSUPPORTED;
    uint32_t cluster=entry.first_cluster,status;
    const uint8_t *data=fs->disk->memory->ram+input;
    if(!entry.size) {
        if(!mount->free_clusters)return result(fs,iosb,FF_DISK_FULL,0);
        status=store_first_file_cluster(fs,mount,file->entry_address,data,bytes,&cluster);
    } else {
        if(!ff_disk_has(fs->disk,file->entry_address+44u,8u) || fs->disk->revision>UINT64_MAX-2u)
            return FF_BOOT_UNSUPPORTED;
        const uint64_t data_address=mount->offset+mount->data_offset+(uint64_t)(cluster-1u)*mount->cluster_bytes;
        status=ff_disk_store_allocate(fs->disk,data_address+entry.size,data,bytes);
    }
    if(status==FF_DISK_FULL || status==FF_BOOT_NO_MEMORY)return result(fs,iosb,FF_DISK_FULL,0);
    if(status)return status;
    /* The existing entry's name, attributes and timestamp bytes are kept. */
    const uint32_t end=entry.size+bytes;
    uint8_t extent[8];
    for(unsigned i=0;i<4;++i) {
        extent[i]=(uint8_t)(cluster>>(8*i));
        extent[4+i]=(uint8_t)(end>>(8*i));
    }
    status=ff_disk_store(fs->disk,file->entry_address+44u,extent,sizeof extent);
    if(status)return status;
    if(!entry.size)--mount->free_clusters;
    file->open_revision=mount->revision=fs->disk->revision;
    file->position=end;
    return result(fs,iosb,0,bytes);
}
uint32_t ff_fatx_create_empty(ff_fatx_volume *fs,uint32_t id,
                              const uint8_t *directory_path,uint32_t path_bytes,uint32_t flags,
                              const uint8_t *name,uint32_t name_bytes,
                              ff_fatx_directory_entry *created)
{
    return create_empty_internal(fs,id,directory_path,path_bytes,flags,name,name_bytes,created,NULL);
}
static uint32_t create_empty_internal(ff_fatx_volume *fs,uint32_t id,
                                      const uint8_t *directory_path,uint32_t path_bytes,uint32_t flags,
                                      const uint8_t *name,uint32_t name_bytes,
                                      ff_fatx_directory_entry *created,uint64_t *guest_entry_address)
{
    if(!fs || !created || !new_name_valid(name,name_bytes))return FF_BOOT_INVALID;
    ff_fatx_mount *mount;
    uint32_t start;
    uint32_t status=directory_start(fs,id,&mount,directory_path,path_bytes,flags,&start);
    if(status)return status;
    uint64_t address;
    status=directory_free_slot(fs,mount,start,name,name_bytes,flags,&address);
    if(status)return status;
    if(guest_entry_address) {
        const uint8_t marker=ff_disk_byte(fs->disk,address);
        if(marker==0 || marker==0xff) {
            const uint64_t cluster_base=mount->offset+mount->data_offset+
                (uint64_t)(start-1u)*mount->cluster_bytes;
            const uint64_t cluster_end=cluster_base+mount->cluster_bytes;
            const uint64_t next=address+64u;
            if(next>=cluster_end || !ff_disk_has(fs->disk,next,64u))return FF_BOOT_UNSUPPORTED;
            const uint8_t next_marker=ff_disk_byte(fs->disk,next);
            if(next_marker!=0 && next_marker!=0xff)return FF_BOOT_UNSUPPORTED;
        }
    }
    status=write_entry(fs,address,name,name_bytes,0,0,created);
    if(status)return status;
    /* A metadata-only creation leaves the mount geometry valid. */
    mount->revision=fs->disk->revision;
    if(guest_entry_address)*guest_entry_address=address;
    return 0;
}
uint32_t ff_fatx_create_file(ff_fatx_volume *fs,uint32_t id,
                             const uint8_t *directory_path,uint32_t path_bytes,uint32_t flags,
                             const uint8_t *name,uint32_t name_bytes,
                             const uint8_t *data,uint32_t bytes,
                             ff_fatx_directory_entry *created)
{
    if(!fs || !created || !data || !bytes || !new_name_valid(name,name_bytes))return FF_BOOT_INVALID;
    ff_fatx_mount *mount;
    uint32_t start;
    uint32_t status=directory_start(fs,id,&mount,directory_path,path_bytes,flags,&start);
    if(status)return status;
    if(bytes>mount->cluster_bytes)return FF_BOOT_UNSUPPORTED;
    uint64_t address;
    status=directory_free_slot(fs,mount,start,name,name_bytes,flags,&address);
    if(status)return status;
    uint32_t cluster;
    status=store_first_file_cluster(fs,mount,address,data,bytes,&cluster);
    if(status)return status;
    status=write_entry(fs,address,name,name_bytes,cluster,bytes,created);
    if(status)return status;
    if(mount->free_clusters)--mount->free_clusters;
    /* The data store, FAT update and directory entry are complete. */
    mount->revision=fs->disk->revision;
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

static int cursor_device_valid(const ff_fatx_volume *fs,const ff_fatx_mount *mount)
{
    for(unsigned i=0;i<FF_NS_OBJECTS;++i) {
        const ff_namespace_object *device=&fs->space->objects[i];
        if(device->id==mount->device)
            return device->kind==FF_NS_DEVICE && device->device_kind==FF_NS_LOCAL_FATX &&
                device->media_ready && device->provider_object==mount->id;
    }
    return 0;
}
static int cursor_live_entry(ff_fatx_volume *fs,ff_fatx_single_cursor *cursor,
                             ff_fatx_file **file_out,ff_fatx_mount **mount_out)
{
    if(!fs || !fs->disk || !fs->space || fs->space->memory!=fs->disk->memory ||
       cursor->owner!=fs || cursor->backing!=fs->disk || cursor->namespace_owner!=fs->space ||
       !cursor->consumed || cursor->finished || cursor->index!=1u || cursor->name_bytes!=12u ||
       cursor->name[12])return 0;
    ff_fatx_file *file=file_handle(fs,cursor->handle);
    if(!file || !ff_fatx_owns_handle(cursor->handle) || file->kind!=FF_FATX_HANDLE_ROOT ||
       file->handle!=cursor->handle || file->id!=cursor->file_id || file->references!=1u ||
       file->access!=0x00100001u ||
       file->mount!=cursor->mount)return 0;
    ff_fatx_mount *mount=mount_id(fs,file->mount);
    if(!mount || mount->id!=cursor->mount || !current(fs,mount) || !cursor_device_valid(fs,mount) ||
       mount->device!=cursor->device || mount->serial!=cursor->serial || mount->offset!=cursor->mount_offset ||
       mount->bytes!=cursor->mount_bytes || mount->data_offset!=cursor->mount_data_offset ||
       mount->root_cluster!=cursor->root_cluster || mount->root_clusters!=cursor->root_clusters ||
       mount->cluster_bytes!=cursor->cluster_bytes || mount->entry_bytes!=cursor->entry_bytes ||
       mount->root_cluster!=1u || mount->root_clusters!=1u || mount->cluster_bytes!=16384u ||
       mount->entry_bytes!=4u)return 0;
    if(mount->offset>UINT64_MAX-mount->data_offset)return 0;
    const uint64_t root=mount->offset+mount->data_offset;
    const uint64_t fat_rel=4096u+(uint64_t)mount->root_cluster*mount->entry_bytes;
    if(root<mount->offset || root-mount->offset>mount->bytes ||
       mount->cluster_bytes>mount->bytes-(root-mount->offset) || fat_rel>mount->bytes ||
       mount->entry_bytes>mount->bytes-fat_rel || fat_rel>UINT64_MAX-mount->offset ||
       !ff_disk_has(fs->disk,root,mount->cluster_bytes) ||
       !ff_disk_has(fs->disk,mount->offset+fat_rel,mount->entry_bytes) ||
       disk_word(fs,mount->offset+fat_rel,mount->entry_bytes)!=UINT32_MAX)return 0;
    uint32_t count=0;ff_fatx_directory_entry entry;
    if(ff_fatx_directory_count(fs,mount->id,NULL,0,0,&count) || count!=1u ||
       ff_fatx_directory_at(fs,mount->id,NULL,0,0,0,&entry) ||
       entry.attributes!=cursor->attributes || entry.first_cluster!=cursor->first_cluster ||
       strlen(entry.name)!=cursor->name_bytes || memcmp(entry.name,cursor->name,cursor->name_bytes) ||
       !single_cluster_file(fs,mount,&entry))return 0;
    ff_fatx_directory_entry at_root;
    if(parse_entry(fs,mount,root,&at_root)!=FF_FATX_PARSE_OK ||
       strcmp(at_root.name,entry.name) || at_root.attributes!=entry.attributes ||
       at_root.first_cluster!=entry.first_cluster || at_root.size!=entry.size ||
       ff_disk_byte(fs->disk,root+64u)!=0xffu)return 0;
    if(file_out)*file_out=file;
    if(mount_out)*mount_out=mount;
    return 1;
}
uint32_t ff_fatx_single_cursor_begin(ff_fatx_volume *fs,uint32_t handle,const uint8_t *record,
                                     uint32_t record_bytes,ff_fatx_single_cursor *cursor)
{
    if(!fs || !fs->disk || !fs->space || !fs->disk->memory ||
       fs->space->memory!=fs->disk->memory || !record || !cursor)return FF_BOOT_INVALID;
    if(cursor->owner || cursor->backing || cursor->namespace_owner || cursor->handle ||
       cursor->file_id || cursor->mount || cursor->device || cursor->serial || cursor->index || cursor->mount_offset ||
       cursor->mount_bytes || cursor->mount_data_offset || cursor->mount_revision ||
       cursor->root_cluster || cursor->root_clusters || cursor->cluster_bytes || cursor->entry_bytes ||
       cursor->attributes || cursor->first_cluster || cursor->initial_size || cursor->name_bytes ||
       cursor->consumed || cursor->finished)return FF_BOOT_UNSUPPORTED;
    for(size_t i=0;i<sizeof cursor->name;++i)if(cursor->name[i])return FF_BOOT_UNSUPPORTED;
    if(record_bytes!=76u)return FF_BOOT_UNSUPPORTED;
    ff_fatx_file *file=file_handle(fs,handle);
    if(!file || !ff_fatx_owns_handle(handle))return FF_BOOT_BAD_HANDLE;
    ff_fatx_mount *mount=mount_id(fs,file->mount);
    if(!mount || file->kind!=FF_FATX_HANDLE_ROOT || file->handle!=handle ||
       file->references!=1u || file->access!=0x00100001u || !current(fs,mount) ||
       !cursor_device_valid(fs,mount) || mount->root_cluster!=1u || mount->root_clusters!=1u ||
       mount->cluster_bytes!=16384u || mount->entry_bytes!=4u)return FF_BOOT_UNSUPPORTED;
    uint32_t count=0;ff_fatx_directory_entry entry;
    if(ff_fatx_directory_count(fs,mount->id,NULL,0,0,&count) || count!=1u ||
       ff_fatx_directory_at(fs,mount->id,NULL,0,0,0,&entry) ||
       !single_cluster_file(fs,mount,&entry))return FF_BOOT_UNSUPPORTED;
    const uint32_t name_bytes=(uint32_t)strlen(entry.name);
    if(name_bytes!=12u)return FF_BOOT_UNSUPPORTED;
    uint8_t expected[76]={0};
    record_put(expected,0x28u,entry.size,8);record_put(expected,0x30u,mount->cluster_bytes,8);
    record_put(expected,0x38u,entry.attributes,4);record_put(expected,0x3cu,name_bytes,4);
    memcpy(expected+0x40u,entry.name,name_bytes);
    if(memcmp(record,expected,sizeof expected))return FF_BOOT_UNSUPPORTED;
    if(mount->offset>UINT64_MAX-mount->data_offset)return FF_BOOT_UNSUPPORTED;
    const uint64_t root=mount->offset+mount->data_offset;
    const uint64_t fat_rel=4096u+(uint64_t)mount->root_cluster*mount->entry_bytes;
    if(root<mount->offset || root-mount->offset>mount->bytes ||
       mount->cluster_bytes>mount->bytes-(root-mount->offset) || fat_rel>mount->bytes ||
       mount->entry_bytes>mount->bytes-fat_rel || fat_rel>UINT64_MAX-mount->offset ||
       !ff_disk_has(fs->disk,root,mount->cluster_bytes) ||
       !ff_disk_has(fs->disk,mount->offset+fat_rel,mount->entry_bytes) ||
       disk_word(fs,mount->offset+fat_rel,mount->entry_bytes)!=UINT32_MAX ||
       ff_disk_byte(fs->disk,root+64u)!=0xffu)return FF_BOOT_UNSUPPORTED;
    ff_fatx_directory_entry on_disk;
    if(parse_entry(fs,mount,root,&on_disk)!=FF_FATX_PARSE_OK ||
       strcmp(on_disk.name,entry.name) || on_disk.attributes!=entry.attributes ||
       on_disk.first_cluster!=entry.first_cluster || on_disk.size!=entry.size)return FF_BOOT_UNSUPPORTED;
    for(uint32_t i=52u;i<64u;i+=4u)if(disk_word(fs,root+i,4)!=UINT32_MAX)
        return FF_BOOT_UNSUPPORTED;
    ff_fatx_single_cursor next;memset(&next,0,sizeof next);
    next.owner=fs;next.backing=fs->disk;next.namespace_owner=fs->space;
    next.handle=handle;next.file_id=file->id;next.mount=mount->id;next.device=mount->device;next.serial=mount->serial;next.index=1;
    next.mount_offset=mount->offset;next.mount_bytes=mount->bytes;next.mount_data_offset=mount->data_offset;
    next.mount_revision=mount->revision;next.root_cluster=mount->root_cluster;
    next.root_clusters=mount->root_clusters;next.cluster_bytes=mount->cluster_bytes;
    next.entry_bytes=mount->entry_bytes;next.attributes=entry.attributes;
    next.first_cluster=entry.first_cluster;next.initial_size=entry.size;next.name_bytes=name_bytes;
    memcpy(next.name,entry.name,name_bytes+1u);next.consumed=1;
    *cursor=next;return 0;
}
uint32_t ff_fatx_query_directory_next_single(ff_fatx_volume *fs,const uint32_t args[10],
                                             ff_fatx_single_cursor *cursor)
{
    if(!fs || !fs->disk || !fs->space || !args || !cursor || !fs->disk->memory ||
       fs->space->memory!=fs->disk->memory)return FF_BOOT_INVALID;
    if(cursor->finished || !cursor_live_entry(fs,cursor,NULL,NULL))return FF_BOOT_UNSUPPORTED;
    if(args[0]!=cursor->handle || args[1] || args[2] || args[3] || args[6]!=0x148u ||
       args[7]!=1u || args[8] || args[9])return FF_BOOT_UNSUPPORTED;
    const uint32_t iosb=args[4],output=args[5];
    if((iosb|output)&3u)return FF_BOOT_INVALID;
    if(!admitted(fs,iosb,8,FF_VM_WRITE) || !admitted(fs,output,0x148u,FF_VM_WRITE))
        return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,output,0x148u))return FF_BOOT_UNSUPPORTED;
    result(fs,iosb,0x80000006u,0);cursor->finished=1;return 0x80000006u;
}
