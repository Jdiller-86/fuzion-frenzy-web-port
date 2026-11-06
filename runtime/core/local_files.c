#include "local_volume.h"
#include "thread_bootstrap.h"
#include <string.h>

/* Independently authored in-memory files in the explicitly fresh guest tree.
 * No native paths, original save defaults, disk sectors or persistence. */
static uint32_t word(const ff_local_volume *volume,uint32_t address,unsigned bytes)
{
    uint32_t value=0;
    for(unsigned i=0;i<bytes;++i) value|=(uint32_t)volume->memory->ram[address+i]<<(i*8);
    return value;
}
static void put(ff_local_volume *volume,uint32_t address,uint32_t value)
{
    for(unsigned i=0;i<4;++i) volume->memory->ram[address+i]=(uint8_t)(value>>(i*8));
}
static int admitted(const ff_local_volume *volume,uint32_t address,uint32_t bytes,uint32_t access)
{
    return address && !ff_vm_check(volume->memory,address,bytes,access);
}
static int overlaps(uint32_t a,uint32_t n,uint32_t b,uint32_t m)
{
    return n && m && (uint64_t)a<(uint64_t)b+m && (uint64_t)b<(uint64_t)a+n;
}
static uint32_t result(ff_local_volume *volume,uint32_t iosb,uint32_t status,uint32_t information)
{
    put(volume,iosb,status); put(volume,iosb+4,information); return status;
}
static uint32_t mapped_access(uint32_t access)
{
    uint32_t mapped=access&0x3fffffffu;
    if(access&0x80000000u) mapped|=0x120089u;
    if(access&0x40000000u) mapped|=0x120116u;
    return mapped;
}
uint32_t ff_volume_create_file(ff_local_volume *volume,const uint32_t args[9],uint64_t system_time)
{
    if(!volume || !volume->memory || !volume->memory->ram || !args) return FF_BOOT_INVALID;
    if(system_time>INT64_MAX) return FF_BOOT_INVALID;
    const uint32_t output=args[0],access=args[1],attributes=args[2],iosb=args[3],share=args[6],disposition=args[7],options=args[8];
    if((output|attributes|iosb)&3u) return FF_BOOT_INVALID;
    if(!admitted(volume,output,4,FF_VM_WRITE) || !admitted(volume,iosb,8,FF_VM_WRITE) ||
        !admitted(volume,attributes,12,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint32_t relative=word(volume,attributes,4),descriptor=word(volume,attributes+4,4),flags=word(volume,attributes+8,4);
    if((descriptor&3u) || !admitted(volume,descriptor,8,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint32_t bytes=word(volume,descriptor,2),maximum=word(volume,descriptor+2,2),buffer=word(volume,descriptor+4,4);
    if(bytes>maximum || bytes>=256) return FF_BOOT_UNSUPPORTED;
    if(bytes && !admitted(volume,buffer,bytes,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    if(overlaps(output,4,iosb,8) || overlaps(output,4,attributes,12) || overlaps(iosb,8,attributes,12) ||
        overlaps(output,4,descriptor,8) || overlaps(iosb,8,descriptor,8) || overlaps(output,4,buffer,bytes) || overlaps(iosb,8,buffer,bytes)) return FF_BOOT_UNSUPPORTED;
    const uint32_t rights=mapped_access(access);
    if(args[4] || (access&~0xc012019fu) || (share&~7u) || (options&~0x62u) || disposition<1 || disposition>3 ||
        (args[5]&~0xa6u) || ((args[5]&0x80u) && args[5]!=0x80u)) return FF_BOOT_UNSUPPORTED;
    if((options&0x20u) && !(rights&0x100000u)) return FF_BOOT_INVALID;
    char path[256];
    for(uint32_t i=0;i<bytes;++i) path[i]=(char)word(volume,buffer+i,1);
    path[bytes]=0;
    if(bytes && path[bytes-1]=='\\') return FF_BOOT_UNSUPPORTED;
    const ff_volume_handle *base=NULL;
    if(relative) {
        for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(volume->handles[i].handle==relative) base=&volume->handles[i];
        if(!base) return FF_BOOT_BAD_HANDLE;
        if(!base->directory) return result(volume,iosb,FF_VOLUME_NOT_DIRECTORY,0);
    }
    ff_volume_path resolved;
    uint32_t status=ff_volume_resolve_path(volume,base?base->directory:0,path,bytes,flags,&resolved);
    if(status==FF_BOOT_UNSUPPORTED || status==FF_BOOT_INVALID) return status;
    if(status) return result(volume,iosb,status,0);
    if((resolved.directory || resolved.data_file) && disposition==2) return result(volume,iosb,FF_VOLUME_NAME_COLLISION,0);
    if(resolved.directory) return result(volume,iosb,FF_VOLUME_IS_DIRECTORY,0);
    ff_volume_data_file *file=NULL;
    if(resolved.data_file) {
        for(unsigned i=0;i<FF_VOLUME_DATA_FILES;++i) if(volume->data_files[i].id==resolved.data_file) file=&volume->data_files[i];
        if(!file) return FF_BOOT_UNSUPPORTED;
    } else {
        if(disposition==1) return result(volume,iosb,FF_VOLUME_NAME_NOT_FOUND,0);
        for(unsigned i=0;i<FF_VOLUME_DATA_FILES;++i) if(!volume->data_files[i].id) {file=&volume->data_files[i];break;}
        if(!file) return result(volume,iosb,FF_VOLUME_TOO_MANY_HANDLES,0);
    }
    const uint32_t id=resolved.data_file?resolved.data_file:(uint32_t)(file-volume->data_files)+1;
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) {
        const ff_volume_handle *old=&volume->handles[i];
        const uint32_t old_rights=mapped_access(old->access);
        if(old->handle && old->data_file==id && (((rights&1u) && !(old->share&1u)) || ((old_rights&1u) && !(share&1u)) ||
            ((rights&6u) && !(old->share&2u)) || ((old_rights&6u) && !(share&2u)))) return result(volume,iosb,FF_VOLUME_SHARING_VIOLATION,0);
    }
    ff_volume_handle *slot=NULL;
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(!volume->handles[i].handle) {slot=&volume->handles[i];break;}
    if(!slot || volume->next_handle>=FF_VOLUME_HANDLE_LIMIT) return result(volume,iosb,FF_VOLUME_TOO_MANY_HANDLES,0);
    if(!resolved.data_file) {
        memset(file,0,sizeof *file); file->id=id; file->parent=resolved.parent; file->attributes=args[5];
        file->creation_time=file->access_time=file->write_time=file->change_time=system_time;
        memcpy(file->name,resolved.name,sizeof file->name); ++volume->data_file_count; ++volume->data_files_created;
    }
    *slot=(ff_volume_handle){volume->next_handle,access,share,options,0,0,id,0};
    volume->next_handle+=4; ++volume->active; ++volume->opened;
    put(volume,output,slot->handle);
    return result(volume,iosb,0,resolved.data_file?1:2);
}
uint32_t ff_volume_query_file(ff_local_volume *volume,const uint32_t args[5])
{
    if(!volume || !volume->memory || !volume->memory->ram || !args) return FF_BOOT_INVALID;
    const ff_volume_handle *handle=NULL;
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(args[0] && volume->handles[i].handle==args[0]) handle=&volume->handles[i];
    if(!handle) return FF_BOOT_BAD_HANDLE;
    if(!handle->data_file || args[4]!=34) return FF_BOOT_UNSUPPORTED;
    const ff_volume_data_file *file=NULL;
    for(unsigned i=0;i<FF_VOLUME_DATA_FILES;++i) if(volume->data_files[i].id==handle->data_file) file=&volume->data_files[i];
    if(!file || file->bytes>FF_VOLUME_FILE_BYTES || file->allocated_bytes<file->bytes) return FF_BOOT_UNSUPPORTED;
    const uint32_t iosb=args[1],output=args[2],bytes=args[3];
    if((iosb|output)&3u) return FF_BOOT_INVALID;
    if(!admitted(volume,iosb,8,FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    if(overlaps(iosb,8,output,bytes<56?bytes:56)) return FF_BOOT_UNSUPPORTED;
    if(bytes<56) return result(volume,iosb,FF_VOLUME_BUFFER_TOO_SMALL,0);
    if(!admitted(volume,output,56,FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    const uint64_t values[6]={file->creation_time,file->access_time,file->write_time,file->change_time,file->allocated_bytes,file->bytes};
    for(unsigned i=0;i<6;++i) for(unsigned byte=0;byte<8;++byte) volume->memory->ram[output+i*8+byte]=(uint8_t)(values[i]>>(byte*8));
    put(volume,output+48,file->attributes); put(volume,output+52,0);
    return result(volume,iosb,0,56);
}

uint32_t ff_volume_write_file(ff_local_volume *volume,const uint32_t args[8],uint64_t system_time)
{
    if(!volume || !volume->memory || !volume->memory->ram || !args || system_time>INT64_MAX) return FF_BOOT_INVALID;
    ff_volume_handle *handle=NULL;
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(args[0] && volume->handles[i].handle==args[0]) handle=&volume->handles[i];
    if(!handle) return FF_BOOT_BAD_HANDLE;
    const uint32_t iosb=args[4],buffer=args[5],bytes=args[6],offset_pointer=args[7];
    if(iosb&3u) return FF_BOOT_INVALID;
    if(!admitted(volume,iosb,8,FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    if(args[1] || args[2] || args[3]) return FF_BOOT_UNSUPPORTED;
    if(!handle->data_file) return result(volume,iosb,FF_VOLUME_IS_DIRECTORY,0);
    /* Only synchronous, buffered regular files are admitted. Write-through
     * reaches this explicit RAM volume's backing; it is not persistent storage. */
    if(!(handle->options&0x20u) || (handle->options&~0x62u)) return FF_BOOT_UNSUPPORTED;
    ff_volume_data_file *file=NULL;
    for(unsigned i=0;i<FF_VOLUME_DATA_FILES;++i) if(volume->data_files[i].id==handle->data_file) file=&volume->data_files[i];
    if(!file) return FF_BOOT_UNSUPPORTED;
    const uint32_t rights=mapped_access(handle->access);
    if(!(rights&6u) || (file->attributes&1u)) return result(volume,iosb,FF_VOLUME_ACCESS_DENIED,0);
    if(bytes>FF_VOLUME_FILE_BYTES) return FF_BOOT_UNSUPPORTED;
    if(bytes && !admitted(volume,buffer,bytes,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    if(offset_pointer && ((offset_pointer&3u) || !admitted(volume,offset_pointer,8,FF_VM_READ))) return FF_VM_ACCESS_VIOLATION;
    if(overlaps(iosb,8,buffer,bytes) || overlaps(iosb,8,offset_pointer,offset_pointer?8:0)) return FF_BOOT_UNSUPPORTED;
    const uint64_t unit_bytes=(uint64_t)volume->profile.sectors_per_unit*volume->profile.bytes_per_sector;
    if(!unit_bytes || unit_bytes>524288u || !volume->profile.total_units || volume->profile.available_units>volume->profile.total_units ||
        file->bytes>FF_VOLUME_FILE_BYTES || file->allocated_bytes<file->bytes || file->allocated_bytes%unit_bytes ||
        file->allocated_bytes/unit_bytes>volume->profile.total_units) return FF_BOOT_UNSUPPORTED;
    uint64_t offset=handle->position;
    if(offset_pointer) {
        offset=word(volume,offset_pointer,4)|(uint64_t)word(volume,offset_pointer+4,4)<<32;
        if(offset==UINT64_MAX) offset=file->bytes;
        else if(offset==UINT64_MAX-1) offset=handle->position;
        else if(offset>INT64_MAX) return FF_BOOT_INVALID;
    }
    if(!(rights&2u)) offset=file->bytes; /* Append-only access never overwrites. */
    if(offset>FF_VOLUME_FILE_BYTES || bytes>FF_VOLUME_FILE_BYTES-offset) return FF_BOOT_UNSUPPORTED;
    /* A null write has no data, extent, position, capacity or timestamp effect. */
    if(!bytes) return result(volume,iosb,0,0);
    const uint32_t end=(uint32_t)offset+bytes,new_bytes=end>file->bytes?end:file->bytes;
    const uint64_t required=((uint64_t)new_bytes+unit_bytes-1)/unit_bytes*unit_bytes;
    const uint64_t allocated=required>file->allocated_bytes?required:file->allocated_bytes;
    const uint64_t extra_units=(allocated-file->allocated_bytes)/unit_bytes;
    if(extra_units>volume->profile.available_units) return result(volume,iosb,FF_VOLUME_DISK_FULL,0);
    /* All admission and capacity checks precede the first backing mutation. */
    if(offset>file->bytes) memset(file->data+file->bytes,0,(size_t)offset-file->bytes);
    memcpy(file->data+(uint32_t)offset,volume->memory->ram+buffer,bytes);
    file->bytes=new_bytes; file->allocated_bytes=allocated; handle->position=end;
    volume->profile.available_units-=extra_units;
    /* Explicit synthetic-volume times; FATX update/close granularity is not
     * inferred from these coherent owned values. Creation/access stay intact. */
    file->write_time=file->change_time=system_time;
    return result(volume,iosb,0,bytes);
}
