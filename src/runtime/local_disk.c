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
uint32_t ff_disk_open(ff_local_disk *disk,const uint32_t args[6])
{
    static const char device[]="\\Device\\Harddisk0\\partition0";
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
    if(get(disk,attributes,4) || bytes!=sizeof device-1)return FF_BOOT_UNSUPPORTED;
    for(unsigned i=0;i<sizeof device-1;++i) {
        unsigned a=disk->memory->ram[buffer+i],b=(uint8_t)device[i];
        if(flags&0x40u) {if(a>='A' && a<='Z')a+='a'-'A';if(b>='A' && b<='Z')b+='a'-'A';}
        if(a!=b)return FF_BOOT_UNSUPPORTED;
    }
    if((flags&~0x40u) || (share&~7u) || (options&~0x78u) || (options&0x30u)==0x30u)return FF_BOOT_UNSUPPORTED;
    if(overlap(output,4,iosb,8) || overlap(output,4,attributes,12) || overlap(iosb,8,attributes,12) ||
       overlap(output,4,descriptor,8) || overlap(iosb,8,descriptor,8) ||
       overlap(output,4,buffer,bytes) || overlap(iosb,8,buffer,bytes))return FF_BOOT_UNSUPPORTED;
    if(access&0x80000000u)access=(access&~0x80000000u)|0x120089u;
    if(access&0x40000000u)access=(access&~0x40000000u)|0x120116u;
    if(access&~0x0012019fu)return FF_BOOT_UNSUPPORTED;
    if(!(options&0x30u) || !(access&0x100000u))return FF_BOOT_UNSUPPORTED;
    for(unsigned i=0;i<FF_DISK_HANDLES;++i) {
        const ff_disk_handle *old=&disk->handles[i];
        if(old->handle && (((access&1u) && !(old->share&1u)) || ((old->access&1u) && !(share&1u)) ||
           ((access&6u) && !(old->share&2u)) || ((old->access&6u) && !(share&2u))))return result(disk,iosb,FF_DISK_SHARING,0);
    }
    ff_disk_handle *slot=NULL;
    for(unsigned i=0;i<FF_DISK_HANDLES;++i)if(!disk->handles[i].handle){slot=&disk->handles[i];break;}
    if(!slot || disk->next_handle>=FF_DISK_HANDLE_LIMIT)return result(disk,iosb,FF_BOOT_NO_MEMORY,0);
    *slot=(ff_disk_handle){disk->next_handle,access,share,options,0};
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
    if(offset>=disk->profile.bytes)return result(disk,iosb,writing?FF_DISK_FULL:FF_DISK_EOF,0);
    uint32_t bytes=requested;
    if(bytes>disk->profile.bytes-offset) {
        if(writing)return result(disk,iosb,FF_DISK_FULL,0);
        bytes=(uint32_t)(disk->profile.bytes-offset);
    }
    if(writing) {
        uint32_t needed=0;
        for(uint64_t cursor=offset;cursor<offset+bytes;) {
            uint64_t index=cursor/FF_DISK_PAGE_BYTES;
            uint32_t n=(uint32_t)(FF_DISK_PAGE_BYTES-cursor%FF_DISK_PAGE_BYTES);
            if(n>offset+bytes-cursor)n=(uint32_t)(offset+bytes-cursor);
            int nonzero=0;
            for(uint32_t i=0;i<n;++i)nonzero|=disk->memory->ram[buffer+(uint32_t)(cursor-offset)+i];
            if(nonzero && !page(disk,index))++needed;
            cursor+=n;
        }
        if(needed>FF_DISK_PAGES-disk->allocated_pages)return result(disk,iosb,FF_DISK_FULL,0);
    }
    for(uint64_t cursor=offset;cursor<offset+bytes;) {
        uint64_t index=cursor/FF_DISK_PAGE_BYTES;
        uint32_t within=(uint32_t)(cursor%FF_DISK_PAGE_BYTES),n=FF_DISK_PAGE_BYTES-within;
        if(n>offset+bytes-cursor)n=(uint32_t)(offset+bytes-cursor);
        ff_disk_page *entry=page(disk,index);
        uint8_t *guest=disk->memory->ram+buffer+(uint32_t)(cursor-offset);
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
    return result(disk,iosb,0,bytes);
}
uint32_t ff_disk_read(ff_local_disk *disk,const uint32_t args[8]) {return transfer(disk,args,0);}
uint32_t ff_disk_write(ff_local_disk *disk,const uint32_t args[8]) {return transfer(disk,args,1);}
uint32_t ff_disk_close(ff_local_disk *disk,uint32_t value)
{
    ff_disk_handle *file=handle(disk,value);
    if(!file)return FF_BOOT_BAD_HANDLE;
    memset(file,0,sizeof *file);--disk->active;++disk->closed;return 0;
}
