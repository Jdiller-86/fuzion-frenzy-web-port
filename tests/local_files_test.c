/* Owned fresh-volume file scenarios. No original saves, media or host files. */
#include "../src/runtime/local_volume.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[256*1024],saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_local_volume volume,saved_volume;
static const ff_volume_profile profile={100000,90000,32,512};
enum { OUTPUT=0x10100,IOSB=0x10120,ATTR=0x11000,NAME=0x11020,TEXT=0x12000 };
static uint32_t args[9];
static const uint64_t file_time=132537600000000000ULL;
static void put(uint32_t address,uint32_t value,unsigned bytes)
{
    for(unsigned i=0;i<bytes;++i) ram[address+i]=(uint8_t)(value>>(i*8));
}
static uint32_t get(uint32_t address)
{
    uint32_t value=0; for(unsigned i=0;i<4;++i) value|=(uint32_t)ram[address+i]<<(i*8); return value;
}
static void name(const char *text)
{
    uint32_t bytes=(uint32_t)strlen(text); assert(bytes<256);
    memcpy(ram+TEXT,text,bytes+1); put(NAME,bytes,2); put(NAME+2,bytes+1,2); put(NAME+4,TEXT,4);
}
static const ff_volume_handle *handle(uint32_t value)
{
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) if(volume.handles[i].handle==value) return &volume.handles[i];
    return NULL;
}
static void save(void) { saved_volume=volume; memcpy(saved_ram,ram,sizeof ram); }
static void unchanged(void)
{
    assert(!memcmp(&saved_volume,&volume,sizeof volume)); assert(!memcmp(saved_ram,ram,sizeof ram));
}
static void reset(void)
{
    memset(ram,0xa5,sizeof ram); assert(!ff_vm_init(&memory,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000)); assert(!ff_vm_map_fixed(&memory,0x10000,0x10000,4));
    assert(!ff_volume_init(&volume,&memory,&profile)); put(ATTR,0,4); put(ATTR+4,NAME,4); put(ATTR+8,0x40,4);
    name("\\Device\\Harddisk0\\partition1\\OwnedData");
    const uint32_t directory[9]={OUTPUT,0x120117,ATTR,IOSB,0,0x80,3,3,0x4021};
    assert(!ff_volume_create_directory(&volume,directory)); assert(!ff_volume_close(&volume,get(OUTPUT)));
    const uint32_t initial[9]={OUTPUT,0x40100000,ATTR,IOSB,0,4,1,3,0x22};
    memcpy(args,initial,sizeof args); name("\\Device\\Harddisk0\\partition1\\OwnedData\\Record.bin");
}
static uint32_t create(uint32_t information)
{
    assert(!ff_volume_create_file(&volume,args,file_time)); assert(get(IOSB)==0 && get(IOSB+4)==information);
    uint32_t value=get(OUTPUT); assert(handle(value) && handle(value)->data_file && !handle(value)->directory); return value;
}
static void file_identity_backing_and_reference_lifetime(void)
{
    reset(); uint32_t opened=create(2); const ff_volume_handle *slot=handle(opened);
    assert(slot->access==0x40100000 && slot->share==1 && slot->options==0x22 && !slot->position);
    ff_volume_data_file *file=&volume.data_files[slot->data_file-1];
    assert(file->id==1 && file->parent==2 && file->attributes==4 && !file->bytes && !strcmp(file->name,"Record.bin"));
    assert(file->creation_time==file_time && file->access_time==file_time && file->write_time==file_time && file->change_time==file_time && !file->allocated_bytes);
    for(unsigned i=0;i<FF_VOLUME_FILE_BYTES;++i) assert(!file->data[i]);
    uint32_t reference=0; assert(!ff_volume_reference_handle(&volume,opened,&reference));
    assert(ff_volume_file_object(&volume,reference)->data_file==file->id && !ff_volume_file_object(&volume,reference)->directory);
    assert(!ff_volume_close(&volume,opened)); assert(!ff_volume_file_object(&volume,reference)->handle);
    assert(ff_volume_file_object(&volume,reference)->references==1 && volume.data_file_count==1 && !volume.active);
    assert(!ff_volume_release_file(&volume,reference) && !volume.file_count);
    /* An owned existing-file fixture proves OPEN_IF does not truncate bytes. */
    memcpy(file->data,"owned",5); file->bytes=5; name("\\DEVICE\\HARDDISK0\\PARTITION1\\owneddata\\record.bin");
    args[5]=0x80; uint32_t reopened=create(1); assert(reopened!=opened && handle(reopened)->data_file==1);
    assert(file->bytes==5 && !memcmp(file->data,"owned",5) && file->attributes==4);
    assert(volume.data_file_count==1 && volume.data_files_created==1 && !handle(reopened)->position);
    assert(!ff_volume_close(&volume,reopened));
}
static void generic_access_sharing_and_independent_files(void)
{
    reset(); uint32_t writer=create(2);
    saved_volume=volume; assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_SHARING_VIOLATION);
    assert(!memcmp(&saved_volume,&volume,sizeof volume));
    args[1]=0x80100000; assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_SHARING_VIOLATION);
    args[6]=3; uint32_t reader=create(1); assert(handle(reader)->data_file==handle(writer)->data_file);
    args[1]=0x100004; args[6]=7; assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_SHARING_VIOLATION);
    args[1]=0x40100000; args[6]=1; name("\\Device\\Harddisk0\\partition1\\OwnedData\\Other.bin");
    uint32_t other=create(2); assert(handle(other)->data_file!=handle(writer)->data_file);
    assert(!ff_volume_close(&volume,writer)); assert(!ff_volume_close(&volume,reader)); assert(!ff_volume_close(&volume,other));
    name("\\Device\\Harddisk0\\partition1\\OwnedData\\Record.bin"); args[1]=0x100004; args[6]=7;
    uint32_t append=create(1); assert(!ff_volume_close(&volume,append));
}
static void paths_types_and_dispositions(void)
{
    reset(); args[7]=1; assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_NAME_NOT_FOUND && !volume.data_file_count);
    args[7]=2; uint32_t file=create(2); assert(!ff_volume_close(&volume,file));
    assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_NAME_COLLISION);
    args[7]=3; name("\\Device\\Harddisk0\\partition1\\Missing\\Child.bin");
    assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_PATH_NOT_FOUND);
    name("\\Device\\Harddisk0\\partition1\\OwnedData\\Record.bin\\Child");
    assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_NOT_DIRECTORY);
    name("\\Device\\Harddisk0\\partition1\\OwnedData"); assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_IS_DIRECTORY);
    const uint32_t directory[9]={OUTPUT,0x120117,ATTR,IOSB,0,0x80,3,3,0x4021};
    assert(!ff_volume_create_directory(&volume,directory)); uint32_t root=get(OUTPUT);
    put(ATTR,root,4); name("Relative.bin"); uint32_t relative=create(2);
    assert(volume.data_files[handle(relative)->data_file-1].parent==2);
    put(ATTR,relative,4); name("Child"); assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_NOT_DIRECTORY);
    assert(ff_volume_create_directory(&volume,directory)==FF_VOLUME_NOT_DIRECTORY);
    put(ATTR,root,4); name("Record.bin"); assert(ff_volume_create_directory(&volume,directory)==FF_VOLUME_NOT_DIRECTORY);
    assert(!ff_volume_close(&volume,relative)); assert(!ff_volume_close(&volume,root));
    put(ATTR,root,4); save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_BAD_HANDLE); unchanged();
}
static void capacities_do_not_leave_partial_files(void)
{
    reset(); uint32_t handles[FF_VOLUME_HANDLES]; handles[0]=create(2); args[1]=0x100000; args[6]=7;
    for(unsigned i=1;i<FF_VOLUME_HANDLES;++i) handles[i]=create(1);
    name("\\Device\\Harddisk0\\partition1\\OwnedData\\NoHandle.bin"); saved_volume=volume;
    assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_TOO_MANY_HANDLES && volume.data_file_count==1);
    assert(!memcmp(&saved_volume,&volume,sizeof volume));
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) assert(!ff_volume_close(&volume,handles[i]));
    reset();
    for(unsigned i=0;i<FF_VOLUME_DATA_FILES;++i) {
        char path[100]; snprintf(path,sizeof path,"\\Device\\Harddisk0\\partition1\\OwnedData\\File%u",i); name(path);
        uint32_t value=create(2); assert(!ff_volume_close(&volume,value));
    }
    name("\\Device\\Harddisk0\\partition1\\OwnedData\\OverCapacity"); saved_volume=volume;
    assert(ff_volume_create_file(&volume,args,file_time)==FF_VOLUME_TOO_MANY_HANDLES);
    assert(!memcmp(&saved_volume,&volume,sizeof volume));
}
static void admission_is_atomic_and_read_only_names_work(void)
{
    reset(); args[0]=OUTPUT+1; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_INVALID); unchanged();
    reset(); put(NAME+4,0x20000,4); save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); args[0]=TEXT; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[4]=0x13000; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[1]=0; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_INVALID); unchanged();
    reset(); args[8]=0x28; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[7]=5; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[5]=1; save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); name("\\Device\\Harddisk0\\partition1\\OwnedData\\..\\Host"); save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); name("\\Device\\Harddisk0\\partition10\\Host"); save(); assert(ff_volume_create_file(&volume,args,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); assert(!ff_vm_init(&memory,ram,sizeof ram)); assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x1000,4)); assert(!ff_vm_map_fixed(&memory,0x11000,0x2000,2));
    uint8_t inputs[0x2000]; memcpy(inputs,ram+0x11000,sizeof inputs); uint32_t value=create(2);
    assert(!memcmp(inputs,ram+0x11000,sizeof inputs)); assert(!ff_volume_close(&volume,value));
}
static void metadata_query_uses_actual_file_state(void)
{
    reset(); uint32_t opened=create(2); uint32_t query[5]={opened,IOSB,0x14000,56,34};
    assert(!ff_volume_query_file(&volume,query) && get(IOSB)==0 && get(IOSB+4)==56);
    for(unsigned i=0;i<4;++i) assert(get(0x14000+i*8)==(uint32_t)file_time && get(0x14004+i*8)==(uint32_t)(file_time>>32));
    assert(get(0x14020)==0 && get(0x14024)==0 && get(0x14028)==0 && get(0x1402c)==0 && get(0x14030)==4 && get(0x14034)==0);
    ff_volume_data_file *file=&volume.data_files[handle(opened)->data_file-1];
    file->bytes=5; file->allocated_bytes=512; file->access_time+=11; file->write_time+=22; file->change_time+=33;
    assert(!ff_volume_query_file(&volume,query));
    assert(get(0x14008)==(uint32_t)(file_time+11) && get(0x14010)==(uint32_t)(file_time+22) && get(0x14018)==(uint32_t)(file_time+33));
    assert(get(0x14020)==512 && get(0x14028)==5);
    args[6]=3; assert(!ff_volume_close(&volume,opened));
    assert(!ff_volume_create_file(&volume,args,file_time+1000)); uint32_t reopened=get(OUTPUT);
    assert(file->creation_time==file_time && file->write_time==file_time+22 && file->bytes==5 && file->allocated_bytes==512);
    query[0]=reopened; query[3]=55; memcpy(saved_ram,ram,sizeof ram); saved_volume=volume;
    assert(ff_volume_query_file(&volume,query)==FF_VOLUME_BUFFER_TOO_SMALL && get(IOSB+4)==0);
    assert(!memcmp(saved_ram+0x14000,ram+0x14000,56) && !memcmp(&saved_volume,&volume,sizeof volume));
    query[3]=56; query[4]=4; save(); assert(ff_volume_query_file(&volume,query)==FF_BOOT_UNSUPPORTED); unchanged();
    query[4]=34; query[2]=IOSB; save(); assert(ff_volume_query_file(&volume,query)==FF_BOOT_UNSUPPORTED); unchanged();
    query[2]=0x20000; save(); assert(ff_volume_query_file(&volume,query)==FF_VM_ACCESS_VIOLATION); unchanged();
    query[2]=0x14001; save(); assert(ff_volume_query_file(&volume,query)==FF_BOOT_INVALID); unchanged();
    assert(!ff_volume_close(&volume,reopened)); query[2]=0x14000; save(); assert(ff_volume_query_file(&volume,query)==FF_BOOT_BAD_HANDLE); unchanged();
}
static void sequential_seek_append_and_written_metadata(void)
{
    reset(); uint32_t opened=create(2); ff_volume_data_file *file=&volume.data_files[handle(opened)->data_file-1];
    uint32_t request[8]={opened,0,0,0,IOSB,0x18000,4,0};
    const uint8_t initial[]={0,255,7,5}; memcpy(ram+0x18000,initial,sizeof initial);
    assert(!ff_volume_write_file(&volume,request,file_time+10) && get(IOSB)==0 && get(IOSB+4)==4);
    assert(file->bytes==4 && !memcmp(file->data,initial,4) && handle(opened)->position==4);
    assert(file->allocated_bytes==16384 && volume.profile.available_units==89999);
    assert(file->creation_time==file_time && file->access_time==file_time && file->write_time==file_time+10 && file->change_time==file_time+10);
    request[7]=0x17000; put(0x17000,1,4); put(0x17004,0,4); request[6]=2; memcpy(ram+0x18000,"XY",2);
    assert(!ff_volume_write_file(&volume,request,file_time+20) && file->bytes==4 && handle(opened)->position==3);
    assert(file->data[0]==0 && file->data[1]=='X' && file->data[2]=='Y' && file->data[3]==5 && volume.profile.available_units==89999);
    put(0x17000,10,4); memcpy(ram+0x18000,"ab",2); assert(!ff_volume_write_file(&volume,request,file_time+30));
    assert(file->bytes==12 && handle(opened)->position==12 && file->data[10]=='a' && file->data[11]=='b');
    for(unsigned i=4;i<10;++i) assert(!file->data[i]);
    put(0x17000,0xfffffffeu,4); put(0x17004,0xffffffffu,4); request[6]=1; ram[0x18000]='z';
    assert(!ff_volume_write_file(&volume,request,file_time+40) && file->bytes==13 && file->data[12]=='z' && handle(opened)->position==13);
    put(0x17000,0,4); put(0x17004,0,4); ram[0x18000]='A'; assert(!ff_volume_write_file(&volume,request,file_time+50));
    assert(handle(opened)->position==1 && file->bytes==13);
    put(0x17000,0xffffffffu,4); put(0x17004,0xffffffffu,4); ram[0x18000]='B';
    assert(!ff_volume_write_file(&volume,request,file_time+60) && file->bytes==14 && file->data[13]=='B' && handle(opened)->position==14);
    assert(!ff_volume_close(&volume,opened)); args[1]=0x100004; args[6]=7; opened=create(1); request[0]=opened;
    put(0x17000,0,4); put(0x17004,0,4); ram[0x18000]='C';
    assert(!ff_volume_write_file(&volume,request,file_time+70) && file->data[0]=='A' && file->data[14]=='C' && file->bytes==15 && handle(opened)->position==15);
    const uint32_t query[5]={opened,IOSB,0x14000,56,34}; assert(!ff_volume_query_file(&volume,query));
    assert(get(0x14020)==16384 && get(0x14028)==15 && get(0x14030)==4);
    assert(get(0x14010)==(uint32_t)(file_time+70) && get(0x14018)==(uint32_t)(file_time+70));
    assert(!ff_volume_close(&volume,opened)); args[1]=0x40100000; opened=create(1); request[0]=opened;
    assert(!handle(opened)->position && file->bytes==15 && file->data[14]=='C');
    request[5]=0xffffffffu; request[6]=0; request[7]=0; saved_volume=volume;
    assert(!ff_volume_write_file(&volume,request,file_time+80) && get(IOSB+4)==0 && !memcmp(&saved_volume,&volume,sizeof volume));
    assert(!ff_volume_close(&volume,opened));
}
static void write_allocation_capacity_and_failure_atomicity(void)
{
    reset(); uint32_t opened=create(2); ff_volume_data_file *file=&volume.data_files[0];
    uint32_t request[8]={opened,0,0,0,IOSB,0x18000,1,0}; ram[0x18000]='q'; volume.profile.available_units=0; saved_volume=volume;
    assert(ff_volume_write_file(&volume,request,file_time+10)==FF_VOLUME_DISK_FULL && get(IOSB)==FF_VOLUME_DISK_FULL && get(IOSB+4)==0);
    assert(!memcmp(&saved_volume,&volume,sizeof volume));
    volume.profile.available_units=1; assert(!ff_volume_write_file(&volume,request,file_time+20));
    assert(!volume.profile.available_units && file->allocated_bytes==16384);
    request[7]=0x17000; put(0x17000,16383,4); put(0x17004,0,4); assert(!ff_volume_write_file(&volume,request,file_time+30));
    assert(file->bytes==16384 && handle(opened)->position==16384 && file->data[16383]=='q');
    put(0x17000,16384,4); saved_volume=volume;
    assert(ff_volume_write_file(&volume,request,file_time+40)==FF_VOLUME_DISK_FULL && get(IOSB+4)==0 && !memcmp(&saved_volume,&volume,sizeof volume));
    reset(); opened=create(2); request[0]=opened; request[6]=FF_VOLUME_FILE_BYTES; request[7]=0;
    assert(!ff_vm_claim_fixed(&memory,0x20000,0x10000)); assert(!ff_vm_map_fixed(&memory,0x20000,0x10000,4));
    for(unsigned i=0;i<FF_VOLUME_FILE_BYTES;++i) ram[0x18000+i]=(uint8_t)(i*17u+3u);
    assert(!ff_volume_write_file(&volume,request,file_time+50)); file=&volume.data_files[0];
    assert(file->bytes==FF_VOLUME_FILE_BYTES && file->allocated_bytes==FF_VOLUME_FILE_BYTES && handle(opened)->position==FF_VOLUME_FILE_BYTES);
    assert(!memcmp(file->data,ram+0x18000,FF_VOLUME_FILE_BYTES) && volume.profile.available_units==89996);
    request[6]=1; save(); assert(ff_volume_write_file(&volume,request,file_time+60)==FF_BOOT_UNSUPPORTED); unchanged();
    request[6]=FF_VOLUME_FILE_BYTES+1; save(); assert(ff_volume_write_file(&volume,request,file_time+60)==FF_BOOT_UNSUPPORTED); unchanged();
}
static void write_admission_permissions_and_independent_files(void)
{
    reset(); uint32_t opened=create(2); uint32_t request[8]={opened,0,0,0,IOSB,0x18000,4,0}; memcpy(ram+0x18000,"data",4);
    for(unsigned field=1;field<=3;++field) {request[field]=1;save();assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_UNSUPPORTED);unchanged();request[field]=0;}
    request[4]=IOSB+1; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_INVALID); unchanged(); request[4]=IOSB;
    request[5]=0x1fffe; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_VM_ACCESS_VIOLATION); unchanged();
    request[5]=IOSB; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_UNSUPPORTED); unchanged(); request[5]=0x18000;
    request[7]=0x17001; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_VM_ACCESS_VIOLATION); unchanged();
    request[7]=IOSB; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    request[7]=0x17000; put(0x17000,0xfffffffdu,4); put(0x17004,0xffffffffu,4); save();
    assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_INVALID); unchanged();
    put(0x17000,0,4); put(0x17004,1,4); save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_UNSUPPORTED); unchanged(); request[7]=0;
    save(); assert(ff_volume_write_file(&volume,request,UINT64_MAX)==FF_BOOT_INVALID); unchanged();
    assert(!ff_vm_init(&memory,ram,sizeof ram)); assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x8000,4)); assert(!ff_vm_map_fixed(&memory,0x18000,0x1000,2));
    uint8_t payload[4]; memcpy(payload,ram+0x18000,4); assert(!ff_volume_write_file(&volume,request,file_time+10)); assert(!memcmp(payload,ram+0x18000,4));
    assert(!ff_vm_init(&memory,ram,sizeof ram)); assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x1000,2)); assert(!ff_vm_map_fixed(&memory,0x18000,0x1000,4));
    save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); args[8]=2; opened=create(2); request[0]=opened; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[1]=0x80100000; opened=create(2); request[0]=opened; saved_volume=volume;
    assert(ff_volume_write_file(&volume,request,file_time)==FF_VOLUME_ACCESS_DENIED && get(IOSB)==FF_VOLUME_ACCESS_DENIED && !memcmp(&saved_volume,&volume,sizeof volume));
    assert(!ff_volume_close(&volume,opened)); save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_BAD_HANDLE); unchanged();
    request[4]=0xffffffffu; save(); assert(ff_volume_write_file(&volume,request,file_time)==FF_BOOT_BAD_HANDLE); unchanged(); request[4]=IOSB;
    reset(); opened=create(2); request[0]=opened; memcpy(ram+0x18000,"one!",4); assert(!ff_volume_write_file(&volume,request,file_time+10));
    name("\\Device\\Harddisk0\\partition1\\OwnedData\\Independent.bin"); uint32_t other=create(2); request[0]=other;
    memcpy(ram+0x18000,"two?",4); assert(!ff_volume_write_file(&volume,request,file_time+20));
    assert(!memcmp(volume.data_files[0].data,"one!",4) && !memcmp(volume.data_files[1].data,"two?",4));
    assert(volume.data_files[0].bytes==4 && volume.data_files[1].bytes==4 && volume.profile.available_units==89998);
}
int main(void)
{
    file_identity_backing_and_reference_lifetime(); generic_access_sharing_and_independent_files();
    paths_types_and_dispositions(); capacities_do_not_leave_partial_files(); admission_is_atomic_and_read_only_names_work(); metadata_query_uses_actual_file_state();
    sequential_seek_append_and_written_metadata(); write_allocation_capacity_and_failure_atomicity(); write_admission_permissions_and_independent_files();
    puts("Owned regular files, create/query/write, backing identity, allocation, offsets, permissions, lifetime and atomic admission passed");
    return 0;
}
