#include "local_disk.h"
#include "thread_bootstrap.h"
#include <string.h>

static uint64_t get(const ff_local_disk *disk, uint32_t address, unsigned bytes)
{
    uint64_t value=0;
    for(unsigned i=0;i<bytes;++i) value|=(uint64_t)disk->memory->ram[address+i]<<(8*i);
    return value;
}
static void put(ff_local_disk *disk, uint32_t address, uint64_t value, unsigned bytes)
{
    for(unsigned i=0;i<bytes;++i) disk->memory->ram[address+i]=(uint8_t)(value>>(8*i));
}
static int admitted(const ff_local_disk *disk,uint32_t address,uint32_t bytes,uint32_t access)
{
    return disk && disk->memory && address && !ff_vm_check(disk->memory,address,bytes,access);
}
static int overlap(uint32_t a,uint32_t n,uint32_t b,uint32_t m)
{
    return n && m && (uint64_t)a<(uint64_t)b+m && (uint64_t)b<(uint64_t)a+n;
}
static uint32_t result(ff_local_disk *disk,uint32_t iosb,uint32_t status,uint32_t bytes)
{
    put(disk,iosb,status,4); put(disk,iosb+4,bytes,4); return status;
}
static ff_disk_handle *handle(ff_local_disk *disk,uint32_t value)
{
    if(disk && value)for(unsigned i=0;i<FF_DISK_HANDLES;++i)if(disk->handles[i].handle==value)return &disk->handles[i];
    return NULL;
}
static ff_disk_page *page(ff_local_disk *disk,uint64_t index)
{
    for(unsigned i=0;i<FF_DISK_PAGES;++i)if(disk->pages[i].used && disk->pages[i].index==index)return &disk->pages[i];
    return NULL;
}
int ff_disk_owns_handle(uint32_t value)
{
    return value>=FF_DISK_HANDLE_BASE && value<FF_DISK_HANDLE_LIMIT;
}
uint32_t ff_disk_init(ff_local_disk *disk,ff_virtual_memory *memory,const ff_disk_profile *profile)
{
    if(!disk || !memory || !memory->ram || !profile || !profile->bytes || profile->bytes>INT64_MAX ||
       (profile->sector_bytes!=512 && profile->sector_bytes!=4096) || profile->bytes%profile->sector_bytes ||
       !profile->cache_partitions || profile->cache_partitions>41)return FF_BOOT_INVALID;
    memset(disk,0,sizeof *disk); disk->memory=memory; disk->profile=*profile;
    disk->next_handle=FF_DISK_HANDLE_BASE; return 0;
}
uint32_t ff_disk_add_partition(ff_local_disk *disk,uint32_t number,uint64_t offset,uint64_t bytes)
{
    if(!disk || !disk->memory || !disk->profile.sector_bytes || disk->opened || disk->allocated_pages ||
       number<3 || number-3>=disk->profile.cache_partitions || !bytes || offset>disk->profile.bytes ||
       bytes>disk->profile.bytes-offset || (offset|bytes)%disk->profile.sector_bytes ||
       disk->partition_count>=FF_DISK_PARTITIONS)return FF_BOOT_INVALID;
    for(unsigned i=0;i<disk->partition_count;++i) {
        const ff_disk_partition *old=&disk->partitions[i];
        if(old->number==number || (offset<old->offset+old->bytes && old->offset<offset+bytes))return FF_BOOT_INVALID;
    }
    disk->partitions[disk->partition_count++]=(ff_disk_partition){number,offset,bytes,0,0,0,0};return 0;
}
uint32_t ff_disk_set_geometry(ff_local_disk *disk,uint64_t cylinders,uint32_t tracks,uint32_t sectors)
{
    if(!disk || !disk->memory || !disk->profile.sector_bytes || disk->opened || disk->allocated_pages || disk->geometry.ready ||
       !cylinders || !tracks || !sectors)return FF_BOOT_INVALID;
    const uint64_t count=disk->profile.bytes/disk->profile.sector_bytes;
    if(count%tracks || count/tracks%sectors || count/tracks/sectors!=cylinders)return FF_BOOT_INVALID;
    disk->geometry=(ff_disk_geometry){cylinders,tracks,sectors,1};return 0;
}
uint32_t ff_disk_set_partition_information(ff_local_disk *disk,uint32_t number,uint32_t hidden_sectors,uint32_t type,uint32_t flags)
{
    if(!disk || !disk->memory || !disk->profile.sector_bytes || disk->opened || disk->allocated_pages || type>255 || flags>7)return FF_BOOT_INVALID;
    for(unsigned i=0;i<disk->partition_count;++i)if(disk->partitions[i].number==number) {
        ff_disk_partition *part=&disk->partitions[i];
        if(part->information_ready || part->offset/disk->profile.sector_bytes!=hidden_sectors)return FF_BOOT_INVALID;
        part->hidden_sectors=hidden_sectors;part->type=type;part->flags=flags;part->information_ready=1;return 0;
    }
    return FF_BOOT_INVALID;
}
uint32_t ff_disk_control(ff_local_disk *disk,const uint32_t args[10])
{
    if(!disk || !disk->memory || !args)return FF_BOOT_INVALID;
    const ff_disk_handle *file=handle(disk,args[0]);
    if(!file)return FF_BOOT_BAD_HANDLE;
    const uint32_t iosb=args[4],code=args[5],output=args[8],bytes=args[9];
    if((iosb|output)&3u)return FF_BOOT_INVALID;
    if(!admitted(disk,iosb,8,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(args[1] || args[2] || args[3] || args[6] || args[7])return FF_BOOT_UNSUPPORTED;
    const ff_disk_partition *part=NULL;
    uint32_t required;
    if(code==0x70000u && disk->geometry.ready)required=24;
    else if(code==0x74004u) {
        for(unsigned i=0;i<disk->partition_count;++i)if(disk->partitions[i].number==file->partition)part=&disk->partitions[i];
        if(!part || !part->information_ready)return FF_BOOT_UNSUPPORTED;
        if(!(file->access&1u))return result(disk,iosb,0xc0000022u,0);
        required=32;
    } else return FF_BOOT_UNSUPPORTED;
    if(overlap(iosb,8,output,bytes<required?bytes:required))return FF_BOOT_UNSUPPORTED;
    if(bytes<required)return result(disk,iosb,0xc0000023u,0);
    if(!admitted(disk,output,required,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(code==0x70000u) {
        put(disk,output,disk->geometry.cylinders,8);put(disk,output+8,12,4); /* FixedMedia */
        put(disk,output+12,disk->geometry.tracks_per_cylinder,4);put(disk,output+16,disk->geometry.sectors_per_track,4);
        put(disk,output+20,disk->profile.sector_bytes,4);
    } else {
        memset(disk->memory->ram+output,0,32);
        put(disk,output,part->offset,8);put(disk,output+8,part->bytes,8);
        put(disk,output+16,part->hidden_sectors,4);put(disk,output+20,part->number,4);
        put(disk,output+24,part->type,1);put(disk,output+25,!!(part->flags&1u),1);
        put(disk,output+26,!!(part->flags&2u),1);put(disk,output+27,!!(part->flags&4u),1);
    }
    return result(disk,iosb,0,required);
}
uint32_t ff_disk_attach_raw_volume(ff_local_disk *disk,uint32_t partition)
{
    if(!disk || !disk->memory || disk->opened || disk->allocated_pages || disk->volume_count>=FF_DISK_PARTITIONS)return FF_BOOT_INVALID;
    int found=0;
    for(unsigned i=0;i<disk->partition_count;++i)found|=disk->partitions[i].number==partition;
    if(!found)return FF_BOOT_INVALID;
    for(unsigned i=0;i<disk->volume_count;++i)if(disk->volumes[i].partition==partition)return FF_BOOT_INVALID;
    disk->volumes[disk->volume_count++]=(ff_disk_volume){partition,FF_DISK_VOLUME_ATTACHED,0,0};
    return 0;
}
uint32_t ff_disk_fs_control(ff_local_disk *disk,const uint32_t args[10])
{
    if(!disk || !disk->memory || !args)return FF_BOOT_INVALID;
    const ff_disk_handle *file=handle(disk,args[0]);
    if(!file)return FF_BOOT_BAD_HANDLE;
    const uint32_t iosb=args[4];
    if(iosb&3u)return FF_BOOT_INVALID;
    if(!admitted(disk,iosb,8,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(args[1] || args[2] || args[3] || args[5]!=0x90020u || args[6] || args[7] || args[8] || args[9] ||
       !file->volume || file->volume>disk->volume_count)return FF_BOOT_UNSUPPORTED;
    ff_disk_volume *volume=&disk->volumes[file->volume-1];
    /* Only the established synchronous exclusive RAW-volume path is admitted.
     * Shared opens, retained filesystem references and other providers need
     * their actual contracts before receiving a guest result. */
    if(volume->partition!=file->partition || volume->state!=FF_DISK_VOLUME_ATTACHED || volume->references!=1 ||
       file->share || (file->access&3u)!=3u)return FF_BOOT_UNSUPPORTED;
    volume->state=FF_DISK_VOLUME_DISMOUNTED;++volume->dismounts;
    return result(disk,iosb,0,0);
}
uint32_t ff_disk_open(ff_local_disk *disk,const uint32_t args[6])
{
    static const char device[]="\\Device\\Harddisk0\\partition";
    if(!disk || !disk->memory || !args)return FF_BOOT_INVALID;
    uint32_t output=args[0],access=args[1],attributes=args[2],iosb=args[3],share=args[4],options=args[5];
    if((output|attributes|iosb)&3u)return FF_BOOT_INVALID;
    if(!admitted(disk,output,4,FF_VM_WRITE) || !admitted(disk,iosb,8,FF_VM_WRITE) ||
       !admitted(disk,attributes,12,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    uint32_t descriptor=(uint32_t)get(disk,attributes+4,4),flags=(uint32_t)get(disk,attributes+8,4);
    if((descriptor&3u) || !admitted(disk,descriptor,8,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    uint32_t bytes=(uint32_t)get(disk,descriptor,2),maximum=(uint32_t)get(disk,descriptor+2,2),buffer=(uint32_t)get(disk,descriptor+4,4);
    if(bytes>maximum || bytes>1024)return FF_BOOT_UNSUPPORTED;
    if(bytes && !admitted(disk,buffer,bytes,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
    if(get(disk,attributes,4) || bytes<sizeof device || bytes>sizeof device+1)return FF_BOOT_UNSUPPORTED;
    for(unsigned i=0;i<sizeof device-1;++i) {
        unsigned a=disk->memory->ram[buffer+i],b=(uint8_t)device[i];
        if(flags&0x40u) {if(a>='A' && a<='Z')a+='a'-'A';if(b>='A' && b<='Z')b+='a'-'A';}
        if(a!=b)return FF_BOOT_UNSUPPORTED;
    }
    uint32_t partition=0;
    for(unsigned i=sizeof device-1;i<bytes;++i) {
        unsigned digit=disk->memory->ram[buffer+i];
        if(digit<'0' || digit>'9' || (i==sizeof device-1 && digit=='0' && i+1<bytes))return FF_BOOT_UNSUPPORTED;
        partition=partition*10+digit-'0';
    }
    uint64_t partition_offset=0,partition_bytes=disk->profile.bytes;
    if(partition) {
        const ff_disk_partition *view=NULL;
        for(unsigned i=0;i<disk->partition_count;++i)if(disk->partitions[i].number==partition)view=&disk->partitions[i];
        if(!view)return FF_BOOT_UNSUPPORTED;
        partition_offset=view->offset;partition_bytes=view->bytes;
    }
    if((flags&~0x40u) || (share&~7u) || (options&~0x78u) || (options&0x30u)==0x30u)return FF_BOOT_UNSUPPORTED;
    if(overlap(output,4,iosb,8) || overlap(output,4,attributes,12) || overlap(iosb,8,attributes,12) ||
       overlap(output,4,descriptor,8) || overlap(iosb,8,descriptor,8) ||
       overlap(output,4,buffer,bytes) || overlap(iosb,8,buffer,bytes))return FF_BOOT_UNSUPPORTED;
    if(access&0x80000000u)access=(access&~0x80000000u)|0x120089u;
    if(access&0x40000000u)access=(access&~0x40000000u)|0x120116u;
    if(access&~0x0012019fu)return FF_BOOT_UNSUPPORTED;
    if(!(options&0x30u) || !(access&0x100000u))return FF_BOOT_UNSUPPORTED;
    uint32_t volume=0;
    for(unsigned i=0;i<disk->volume_count;++i)if(disk->volumes[i].partition==partition) {
        if(disk->volumes[i].state!=FF_DISK_VOLUME_ATTACHED)return FF_BOOT_UNSUPPORTED;
        volume=i+1;
    }
    for(unsigned i=0;i<FF_DISK_HANDLES;++i) {
        const ff_disk_handle *old=&disk->handles[i];
        if(old->handle && old->partition==partition && (((access&1u) && !(old->share&1u)) || ((old->access&1u) && !(share&1u)) ||
           ((access&6u) && !(old->share&2u)) || ((old->access&6u) && !(share&2u))))return result(disk,iosb,FF_DISK_SHARING,0);
    }
    ff_disk_handle *slot=NULL;
    for(unsigned i=0;i<FF_DISK_HANDLES;++i)if(!disk->handles[i].handle){slot=&disk->handles[i];break;}
    if(!slot || disk->next_handle>=FF_DISK_HANDLE_LIMIT)return result(disk,iosb,FF_BOOT_NO_MEMORY,0);
    *slot=(ff_disk_handle){disk->next_handle,access,share,options,partition,volume,0,partition_offset,partition_bytes};
    if(volume)++disk->volumes[volume-1].references;
    disk->next_handle+=4;++disk->active;++disk->opened;put(disk,output,slot->handle,4);
    return result(disk,iosb,0,1);
}
uint8_t ff_disk_byte(const ff_local_disk *disk,uint64_t offset)
{
    if(!disk || offset>=disk->profile.bytes)return 0;
    for(unsigned i=0;i<FF_DISK_PAGES;++i)if(disk->pages[i].used && disk->pages[i].index==offset/FF_DISK_PAGE_BYTES)
        return disk->pages[i].data[offset%FF_DISK_PAGE_BYTES];
    return 0;
}
uint32_t ff_disk_has(const ff_local_disk *disk,uint64_t offset,uint32_t bytes)
{
    if(!disk || offset>disk->profile.bytes || bytes>disk->profile.bytes-offset)return 0;
    if(!bytes)return 1;
    const uint64_t end=offset+bytes;
    for(uint64_t cursor=offset;cursor<end;) {
        const uint64_t index=cursor/FF_DISK_PAGE_BYTES;
        if(!page((ff_local_disk*)disk,index))return 0;
        cursor=(index+1)*FF_DISK_PAGE_BYTES;
    }
    return 1;
}
uint32_t ff_disk_store(ff_local_disk *disk,uint64_t offset,const uint8_t *data,uint32_t bytes)
{
    if(!disk || !data || !bytes)return FF_BOOT_INVALID;
    if(!ff_disk_has(disk,offset,bytes))return FF_BOOT_UNSUPPORTED;
    if(disk->revision==UINT64_MAX)return FF_BOOT_UNSUPPORTED;
    const uint64_t end=offset+bytes;
    uint32_t copied=0;
    for(uint64_t cursor=offset;cursor<end;) {
        const uint64_t index=cursor/FF_DISK_PAGE_BYTES;
        const uint32_t within=(uint32_t)(cursor-index*FF_DISK_PAGE_BYTES);
        uint32_t chunk=FF_DISK_PAGE_BYTES-within;
        if(chunk>end-cursor)chunk=(uint32_t)(end-cursor);
        ff_disk_page *entry=page(disk,index);
        if(!entry)return FF_BOOT_UNSUPPORTED;
        memcpy(entry->data+within,data+copied,chunk);
        copied+=chunk;cursor+=chunk;
    }
    ++disk->revision;
    return 0;
}
uint32_t ff_disk_store_allocate(ff_local_disk *disk,uint64_t offset,const uint8_t *data,uint32_t bytes)
{
    if(!disk || !data || !bytes)return FF_BOOT_INVALID;
    if(offset>disk->profile.bytes || bytes>disk->profile.bytes-offset)return FF_BOOT_INVALID;
    if(disk->revision==UINT64_MAX)return FF_BOOT_UNSUPPORTED;
    const uint64_t end=offset+bytes;
    uint32_t needed=0;
    for(uint64_t cursor=offset;cursor<end;) {
        const uint64_t index=cursor/FF_DISK_PAGE_BYTES;
        uint32_t chunk=FF_DISK_PAGE_BYTES-(uint32_t)(cursor-index*FF_DISK_PAGE_BYTES);
        if(chunk>end-cursor)chunk=(uint32_t)(end-cursor);
        int nonzero=0;
        for(uint32_t i=0;i<chunk;++i)nonzero|=(uint32_t)data[(uint32_t)(cursor-offset)+i];
        if(nonzero && !page(disk,index))++needed;
        cursor+=chunk;
    }
    if(needed>FF_DISK_PAGES-disk->allocated_pages)return FF_DISK_FULL;
    uint32_t copied=0;
    for(uint64_t cursor=offset;cursor<end;) {
        const uint64_t index=cursor/FF_DISK_PAGE_BYTES;
        const uint32_t within=(uint32_t)(cursor-index*FF_DISK_PAGE_BYTES);
        uint32_t chunk=FF_DISK_PAGE_BYTES-within;
        if(chunk>end-cursor)chunk=(uint32_t)(end-cursor);
        ff_disk_page *entry=page(disk,index);
        int nonzero=0;
        for(uint32_t i=0;i<chunk;++i)nonzero|=(uint32_t)data[copied+i];
        if(!entry && nonzero) {
            for(unsigned i=0;i<FF_DISK_PAGES;++i)if(!disk->pages[i].used) {
                entry=&disk->pages[i];
                memset(entry,0,sizeof *entry);
                entry->used=1;entry->index=index;++disk->allocated_pages;
                break;
            }
        }
        if(entry)memcpy(entry->data+within,data+copied,chunk);
        copied+=chunk;cursor+=chunk;
    }
    ++disk->revision;
    return 0;
}
static uint32_t transfer(ff_local_disk *disk,const uint32_t args[8],int writing)
{
    if(!disk || !disk->memory || !args)return FF_BOOT_INVALID;
    ff_disk_handle *file=handle(disk,args[0]);
    if(!file)return FF_BOOT_BAD_HANDLE;
    const uint32_t iosb=args[4],buffer=args[5],requested=args[6],offset_pointer=args[7];
    if((iosb|offset_pointer)&3u)return FF_BOOT_INVALID;
    if(!admitted(disk,iosb,8,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(args[1] || args[2] || args[3])return FF_BOOT_UNSUPPORTED;
    if(requested>1024u*1024u)return FF_BOOT_UNSUPPORTED;
    if(requested && !admitted(disk,buffer,requested,writing?FF_VM_READ:FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(overlap(iosb,8,buffer,requested))return FF_BOOT_UNSUPPORTED;
    uint64_t offset=file->position;
    if(offset_pointer) {
        if(!admitted(disk,offset_pointer,8,FF_VM_READ))return FF_VM_ACCESS_VIOLATION;
        if(overlap(iosb,8,offset_pointer,8) || overlap(buffer,requested,offset_pointer,8))return FF_BOOT_UNSUPPORTED;
        uint64_t value=get(disk,offset_pointer,8);
        if(value!=UINT64_MAX-1u) {
            if(value>INT64_MAX)return FF_BOOT_INVALID;
            offset=value;
        }
    }
    if(!(file->access&(writing?2u:1u)))return result(disk,iosb,0xc0000022u,0);
    if((file->options&8u) && ((offset|requested)%disk->profile.sector_bytes))return FF_BOOT_INVALID;
    if(!requested)return result(disk,iosb,0,0);
    if(offset>=file->bytes)return result(disk,iosb,writing?FF_DISK_FULL:FF_DISK_EOF,0);
    uint32_t bytes=requested;
    if(bytes>file->bytes-offset) {
        if(writing)return result(disk,iosb,FF_DISK_FULL,0);
        bytes=(uint32_t)(file->bytes-offset);
    }
    const uint64_t begin=file->offset+offset,end=begin+bytes;
    if(writing && disk->revision==UINT64_MAX)return FF_BOOT_UNSUPPORTED;
    if(writing) {
        uint32_t needed=0;
        for(uint64_t cursor=begin;cursor<end;) {
            uint64_t index=cursor/FF_DISK_PAGE_BYTES;
            uint32_t n=(uint32_t)(FF_DISK_PAGE_BYTES-cursor%FF_DISK_PAGE_BYTES);
            if(n>end-cursor)n=(uint32_t)(end-cursor);
            int nonzero=0;
            for(uint32_t i=0;i<n;++i)nonzero|=disk->memory->ram[buffer+(uint32_t)(cursor-begin)+i];
            if(nonzero && !page(disk,index))++needed;
            cursor+=n;
        }
        if(needed>FF_DISK_PAGES-disk->allocated_pages)return result(disk,iosb,FF_DISK_FULL,0);
    }
    for(uint64_t cursor=begin;cursor<end;) {
        uint64_t index=cursor/FF_DISK_PAGE_BYTES;
        uint32_t within=(uint32_t)(cursor%FF_DISK_PAGE_BYTES),n=FF_DISK_PAGE_BYTES-within;
        if(n>end-cursor)n=(uint32_t)(end-cursor);
        ff_disk_page *entry=page(disk,index);
        uint8_t *guest=disk->memory->ram+buffer+(uint32_t)(cursor-begin);
        if(writing) {
            int nonzero=0;
            for(uint32_t i=0;i<n;++i)nonzero|=guest[i];
            if(!entry && nonzero)for(unsigned i=0;i<FF_DISK_PAGES;++i)if(!disk->pages[i].used) {
                entry=&disk->pages[i];memset(entry,0,sizeof *entry);entry->used=1;entry->index=index;++disk->allocated_pages;break;
            }
            if(entry) {
                memcpy(entry->data+within,guest,n);
                nonzero=0;for(unsigned i=0;i<FF_DISK_PAGE_BYTES;++i)nonzero|=entry->data[i];
                if(!nonzero){memset(entry,0,sizeof *entry);--disk->allocated_pages;}
            }
        } else if(entry)memcpy(guest,entry->data+within,n);
        else memset(guest,0,n);
        cursor+=n;
    }
    file->position=offset+bytes;
    if(writing)++disk->revision;
    return result(disk,iosb,0,bytes);
}
uint32_t ff_disk_read(ff_local_disk *disk,const uint32_t args[8]) {return transfer(disk,args,0);}
uint32_t ff_disk_write(ff_local_disk *disk,const uint32_t args[8]) {return transfer(disk,args,1);}
uint32_t ff_disk_close(ff_local_disk *disk,uint32_t value)
{
    ff_disk_handle *file=handle(disk,value);
    if(!file)return FF_BOOT_BAD_HANDLE;
    if(file->volume) {
        ff_disk_volume *volume=&disk->volumes[file->volume-1];
        --volume->references;
        if(!volume->references && volume->state==FF_DISK_VOLUME_DISMOUNTED)volume->state=FF_DISK_VOLUME_RETIRED;
    }
    memset(file,0,sizeof *file);--disk->active;++disk->closed;return 0;
}
