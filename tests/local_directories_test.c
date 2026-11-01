/* Owned directory/handle/guest-memory cases only; no game or disk content. */
#include "../src/runtime/local_volume.h"
#include "../src/runtime/thread_bootstrap.h"
#include "../src/runtime/guest_strings.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[256 * 1024], saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_local_volume volume, saved_volume;
static const ff_volume_profile profile = {100000, 90000, 32, 512};
enum { OUTPUT=0x10100, IOSB=0x10120, ATTR=0x10140, NAME=0x10180, TEXT=0x10200, DATA=0x10400 };
static uint32_t args[9];
static void put(uint32_t address, uint64_t value, unsigned bytes)
{
    for (unsigned i=0;i<bytes;++i) ram[address+i]=(uint8_t)(value>>(i*8));
}
static uint32_t get(uint32_t address)
{
    uint32_t value=0; for(unsigned i=0;i<4;++i)value|=(uint32_t)ram[address+i]<<(i*8); return value;
}
static void name(const char *text)
{
    size_t length=strlen(text); assert(length<256);
    memcpy(ram+TEXT,text,length+1); put(NAME,length,2); put(NAME+2,length+1,2); put(NAME+4,TEXT,4);
}
static void reset(void)
{
    memset(ram,0xa5,sizeof ram);
    assert(!ff_vm_init(&memory,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x10000,4));
    assert(!ff_volume_init(&volume,&memory,&profile));
    const uint32_t initial[9]={OUTPUT,0x100001,ATTR,IOSB,0,0x80,3,3,0x4021};
    memcpy(args,initial,sizeof args); put(ATTR,0,4); put(ATTR+4,NAME,4); put(ATTR+8,0x40,4);
    name("\\Device\\Harddisk0\\partition1\\OwnedData");
}
static void save(void) { saved_volume=volume; memcpy(saved_ram,ram,sizeof ram); }
static void unchanged(void)
{
    assert(!memcmp(&volume,&saved_volume,sizeof volume)); assert(!memcmp(ram,saved_ram,sizeof ram));
}
static uint32_t create(uint32_t information)
{
    assert(!ff_volume_create_directory(&volume,args)); assert(!get(IOSB) && get(IOSB+4)==information);
    uint32_t value=get(OUTPUT); assert(ff_volume_owns_handle(value)); return value;
}
static void directory_lifecycle(void)
{
    reset(); uint32_t first=create(2);
    assert(volume.directory_count==2 && volume.directories_created==1 && volume.active==1);
    assert(volume.directories[1].parent==1 && !strcmp(volume.directories[1].name,"OwnedData"));
    name("\\DEVICE\\HARDDISK0\\PARTITION1\\owneddata\\");
    uint32_t second=create(1); assert(second!=first && volume.directory_count==2);
    assert(!ff_volume_close(&volume,second));
    put(ATTR,first,4); name("Child"); uint32_t child=create(2);
    assert(volume.directories[2].parent==2 && !strcmp(volume.directories[2].name,"Child"));
    assert(!ff_volume_close(&volume,first)); assert(!ff_volume_close(&volume,child));
    put(ATTR,0,4); name("\\Device\\Harddisk0\\partition1\\OwnedData\\Child");
    args[7]=1; uint32_t reopened=create(1); assert(reopened!=first && reopened!=child);
    assert(volume.directory_count==3 && volume.directories_created==2);
    uint32_t query[5]={reopened,IOSB,DATA,24,3}; assert(!ff_volume_query_size(&volume,query));
    assert(get(DATA)==100000 && get(DATA+8)==90000);
    put(ATTR,reopened,4); name(""); uint32_t same=create(1);
    assert(!ff_volume_close(&volume,same)); assert(!ff_volume_close(&volume,reopened));
    save(); assert(ff_volume_create_directory(&volume,args)==FF_BOOT_BAD_HANDLE); unchanged();
}
static void disposition_and_parent_errors(void)
{
    reset(); uint32_t first=create(2); args[7]=2;
    uint32_t old_output=get(OUTPUT); assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_NAME_COLLISION);
    assert(volume.directory_count==2 && volume.active==1 && get(OUTPUT)==old_output);
    args[7]=1; name("\\Device\\Harddisk0\\partition1\\Absent");
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_NAME_NOT_FOUND && volume.directory_count==2);
    args[7]=3; name("\\Device\\Harddisk0\\partition1\\Absent\\Child");
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_PATH_NOT_FOUND && volume.directory_count==2);
    name("\\Device\\Harddisk0\\partition1"); uint32_t root=create(1);
    assert(volume.directory_count==2); args[7]=2;
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_NAME_COLLISION);
    assert(!ff_volume_close(&volume,first)); assert(!ff_volume_close(&volume,root));
}
static void sharing_is_per_directory(void)
{
    reset(); args[6]=0; uint32_t exclusive=create(2); args[6]=3;
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_SHARING_VIOLATION && volume.active==1);
    name("\\Device\\Harddisk0\\partition1\\Other"); uint32_t other=create(2);
    const uint32_t root_args[6]={OUTPUT,0x100001,ATTR,IOSB,3,0x800021};
    name("\\Device\\Harddisk0\\partition1"); assert(!ff_volume_open_root(&volume,root_args));
    uint32_t root=get(OUTPUT);
    assert(!ff_volume_close(&volume,exclusive)); assert(!ff_volume_close(&volume,other)); assert(!ff_volume_close(&volume,root));
    name("\\Device\\Harddisk0\\partition1\\OwnedData"); create(1);
    reset(); args[1]=0x120117; args[6]=1; uint32_t writer=create(2);
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_SHARING_VIOLATION);
    args[1]=0x100001; args[6]=1;
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_SHARING_VIOLATION);
    args[6]=3; uint32_t reader=create(1);
    assert(!ff_volume_close(&volume,writer)); assert(!ff_volume_close(&volume,reader));
}
static void resource_failures_do_not_create_nodes(void)
{
    reset(); uint32_t handles[FF_VOLUME_HANDLES];
    for(unsigned i=0;i<FF_VOLUME_HANDLES;++i) handles[i]=create(i?1:2);
    name("\\Device\\Harddisk0\\partition1\\NoHandle"); saved_volume=volume;
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_TOO_MANY_HANDLES);
    assert(!memcmp(&volume,&saved_volume,sizeof volume));
    assert(!ff_volume_close(&volume,handles[3])); create(2);
    reset();
    for(unsigned i=0;i<FF_VOLUME_DIRECTORIES-1;++i) {
        char path[100]; snprintf(path,sizeof path,"\\Device\\Harddisk0\\partition1\\Owned%u",i); name(path);
        uint32_t opened=create(2); assert(!ff_volume_close(&volume,opened));
    }
    name("\\Device\\Harddisk0\\partition1\\OverCapacity"); saved_volume=volume;
    assert(ff_volume_create_directory(&volume,args)==FF_VOLUME_TOO_MANY_HANDLES);
    assert(!memcmp(&volume,&saved_volume,sizeof volume));
    assert(volume.directory_count==FF_VOLUME_DIRECTORIES);
}
static void input_and_memory_admission(void)
{
    const char *bad_names[]={"\\Device\\Harddisk0\\partition10\\Child","\\Device\\Harddisk0\\partition1\\..",
        "\\Device\\Harddisk0\\partition1\\.","\\Device\\Harddisk0\\partition1\\Bad/Name",
        "\\Device\\Harddisk0\\partition1\\Bad:Name","\\Device\\Harddisk0\\partition1\\Bad*Name"};
    for(unsigned i=0;i<sizeof bad_names/sizeof bad_names[0];++i) {
        reset(); name(bad_names[i]); save(); assert(ff_volume_create_directory(&volume,args)==FF_BOOT_UNSUPPORTED); unchanged();
    }
    for(unsigned i=0;i<9;++i) {
        if(i==3 || i==6) continue;
        reset(); const uint32_t bad[9]={OUTPUT+1,0x110001,0xfffffffc,0,DATA,0x100,0,5,0x5021}; args[i]=bad[i];
        save(); uint32_t status=ff_volume_create_directory(&volume,args);
        assert(status==FF_BOOT_INVALID || status==FF_BOOT_UNSUPPORTED || status==FF_VM_ACCESS_VIOLATION); unchanged();
    }
    reset(); args[0]=IOSB; save(); assert(ff_volume_create_directory(&volume,args)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); put(NAME+4,0xfffffffc,4); save(); assert(ff_volume_create_directory(&volume,args)==FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); put(NAME,256,2); put(NAME+2,256,2); save(); assert(ff_volume_create_directory(&volume,args)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); ram[TEXT+30]=0; save(); assert(ff_volume_create_directory(&volume,args)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); assert(!ff_vm_init(&memory,ram,sizeof ram)); assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x10000,2)); save();
    assert(ff_volume_create_directory(&volume,args)==FF_VM_ACCESS_VIOLATION); unchanged();
}
static void counted_string_initialization(void)
{
    reset(); memset(ram+TEXT,0x81,300); ram[TEXT+300]=0;
    assert(!ff_init_ansi_string(&memory,NAME,TEXT));
    assert((get(NAME)&0xffff)==300 && (get(NAME)>>16)==301 && get(NAME+4)==TEXT);
    assert(ram[TEXT]==0x81 && ram[TEXT+300]==0 && ram[NAME-1]==0xa5 && ram[NAME+8]==0xa5);
    assert(!ff_init_ansi_string(&memory,NAME,0)); assert(!get(NAME) && !get(NAME+4));
    ram[TEXT]=0; assert(!ff_init_ansi_string(&memory,NAME,TEXT));
    assert(get(NAME)==0x10000 && get(NAME+4)==TEXT);
    reset(); memset(ram+0x10000,0x81,65534); ram[0x1fffe]=0;
    assert(!ff_vm_claim_fixed(&memory,0x30000,4096)); assert(!ff_vm_map_fixed(&memory,0x30000,4096,4));
    assert(!ff_init_ansi_string(&memory,0x30000,0x10000)); assert(get(0x30000)==0xfffffffe);
    ram[0x1fffe]=0x81; ram[0x1ffff]=0; save();
    assert(ff_init_ansi_string(&memory,0x30000,0x10000)==FF_BOOT_UNSUPPORTED); unchanged();
    reset(); ram[TEXT]=0; save();
    assert(ff_init_ansi_string(&memory,NAME+1,TEXT)==FF_BOOT_INVALID); unchanged();
    assert(ff_init_ansi_string(&memory,0x1fffc,TEXT)==FF_VM_ACCESS_VIOLATION); unchanged();
    assert(ff_init_ansi_string(&memory,NAME,0xfffffffc)==FF_VM_ACCESS_VIOLATION); unchanged();
    assert(ff_init_ansi_string(&memory,TEXT,TEXT)==FF_BOOT_UNSUPPORTED); unchanged();
    ram[0x1ffff]=1; save();
    assert(ff_init_ansi_string(&memory,NAME,0x1ffff)==FF_VM_ACCESS_VIOLATION); unchanged();
}
static void counted_string_equality(void)
{
    reset(); memcpy(ram+TEXT,"OwNeD",6); memcpy(ram+TEXT+64,"owned",6);
    assert(!ff_init_ansi_string(&memory,NAME,TEXT)); assert(!ff_init_ansi_string(&memory,NAME+16,TEXT+64));
    uint32_t equal=99; save();
    assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,0,&equal) && !equal); unchanged();
    assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal) && equal==1); unchanged();
    const uint8_t pairs[][2]={{0xe0,0xc0},{0xef,0xcf},{0xfd,0xdd},{0xfe,0xde},{0xff,'?'},{'a','A'}};
    put(NAME,1,2); put(NAME+16,1,2);
    for(unsigned i=0;i<sizeof pairs/sizeof pairs[0];++i) {
        ram[TEXT]=pairs[i][0]; ram[TEXT+64]=pairs[i][1]; save();
        assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal) && equal==1); unchanged();
        assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,0,&equal) && !equal); unchanged();
    }
    ram[TEXT]=0xf7; ram[TEXT+64]=0xd7;
    assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal) && !equal);
    ram[TEXT]=ram[TEXT+64]=0; ram[TEXT+1]='x'; ram[TEXT+65]='y'; put(NAME,2,2); put(NAME+16,2,2);
    assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal) && !equal);
    ram[TEXT+65]='x'; assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,0,&equal) && equal==1);
    put(NAME+4,0xfffffffc,4); put(NAME+16+4,0xfffffffc,4); put(NAME,1,2); put(NAME+16,2,2);
    assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal) && !equal);
    put(NAME,0,2); put(NAME+16,0,2); assert(!ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal) && equal==1);
    put(NAME,1,2); put(NAME+16,1,2); equal=99; save();
    assert(ff_equal_ansi_string(&memory,NAME,NAME+16,1,&equal)==FF_VM_ACCESS_VIOLATION && equal==99); unchanged();
    assert(ff_equal_ansi_string(&memory,NAME+1,NAME+16,1,&equal)==FF_BOOT_INVALID && equal==99); unchanged();
}
int main(void)
{
    directory_lifecycle(); disposition_and_parent_errors(); sharing_is_per_directory();
    resource_failures_do_not_create_nodes(); input_and_memory_admission(); counted_string_initialization(); counted_string_equality();
    puts("Owned directory identity, create/open, handles, sharing, bounds and atomic-failure checks passed");
    return 0;
}
