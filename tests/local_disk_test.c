/* Source-owned block-device cases; no original executable or disk content. */
#include "../runtime/core/local_disk.h"
#include "../runtime/core/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[0x40000],saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_local_disk disk,saved_disk;
static const ff_disk_profile profile={0x200000000ull,512,3};
enum {OUT=0x10100,IOSB=0x10120,ATTR=0x10140,NAME=0x10180,TEXT=0x10200,OFFSET=0x10300,DATA=0x11000};
static uint32_t open_args[6];
static void put(uint32_t p,uint64_t value,unsigned bytes){for(unsigned i=0;i<bytes;++i)ram[p+i]=(uint8_t)(value>>(i*8));}
static uint64_t get(uint32_t p,unsigned bytes){uint64_t value=0;for(unsigned i=0;i<bytes;++i)value|=(uint64_t)ram[p+i]<<(i*8);return value;}
static void reset(void)
{
    memset(ram,0xa5,sizeof ram);
    assert(!ff_vm_init(&memory,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&memory,0x10000,0x30000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x30000,4));
    assert(!ff_disk_init(&disk,&memory,&profile));
    const char *name="\\Device\\Harddisk0\\partition0";
    memcpy(ram+TEXT,name,strlen(name)+1);
    put(NAME,strlen(name),2);put(NAME+2,strlen(name)+1,2);put(NAME+4,TEXT,4);
    put(ATTR,0,4);put(ATTR+4,NAME,4);put(ATTR+8,0x40,4);
    uint32_t args[6]={OUT,0xc0100000u,ATTR,IOSB,3,0x10};memcpy(open_args,args,sizeof args);
}
static void save(void){saved_disk=disk;memcpy(saved_ram,ram,sizeof ram);}
static void unchanged(void){assert(!memcmp(&disk,&saved_disk,sizeof disk));assert(!memcmp(ram,saved_ram,sizeof ram));}
static uint32_t open_disk(void)
{
    assert(!ff_disk_open(&disk,open_args));assert(get(IOSB,4)==0 && get(IOSB+4,4)==1);
    uint32_t file=(uint32_t)get(OUT,4);assert(ff_disk_owns_handle(file));return file;
}
static void sparse_roundtrip_and_lifetime(void)
{
    reset();uint32_t file=open_disk();
    uint32_t io[8]={file,0,0,0,IOSB,DATA,512,OFFSET};
    put(OFFSET,0x100000800ull,8);
    assert(!ff_disk_read(&disk,io) && !disk.allocated_pages);
    for(unsigned i=0;i<512;++i)assert(!ram[DATA+i]);
    for(unsigned i=0;i<512;++i)ram[DATA+i]=(uint8_t)(i*37+13);
    assert(!ff_disk_write(&disk,io));assert(disk.allocated_pages==1 && get(IOSB+4,4)==512);
    assert(!ff_disk_close(&disk,file));assert(!disk.active && disk.closed==1);
    uint32_t reopened=open_disk();assert(reopened!=file);io[0]=reopened;
    memset(ram+DATA,0,512);assert(!ff_disk_read(&disk,io));
    for(unsigned i=0;i<512;++i)assert(ram[DATA+i]==(uint8_t)(i*37+13));
    assert(ff_disk_byte(&disk,0x100000800ull)==13 && !ff_disk_byte(&disk,0x800));
    io[7]=0;assert(!ff_disk_read(&disk,io));
    for(unsigned i=0;i<512;++i)assert(!ram[DATA+i]);
    io[7]=OFFSET;assert(!ff_disk_write(&disk,io) && !disk.allocated_pages);
    put(OFFSET,profile.bytes-128,8);memset(ram+DATA,0xa5,512);
    assert(!ff_disk_read(&disk,io) && get(IOSB+4,4)==128 && ram[DATA+128]==0xa5);
    put(OFFSET,profile.bytes,8);assert(ff_disk_read(&disk,io)==FF_DISK_EOF);
    save();assert(ff_disk_close(&disk,file)==FF_BOOT_BAD_HANDLE);unchanged();
}
static void permissions_sharing_and_rejected_memory(void)
{
    reset();open_args[4]=0;uint32_t file=open_disk();
    open_args[4]=3;assert(ff_disk_open(&disk,open_args)==FF_DISK_SHARING && disk.active==1);
    assert(!ff_disk_close(&disk,file));open_args[1]=0x80100000u;file=open_disk();
    uint32_t io[8]={file,0,0,0,IOSB,DATA,512,OFFSET};put(OFFSET,2048,8);
    assert(ff_disk_write(&disk,io)==0xc0000022u && !disk.allocated_pages);
    io[2]=1;save();assert(ff_disk_read(&disk,io)==FF_BOOT_UNSUPPORTED);unchanged();io[2]=0;
    io[5]=0x3ff00;save();assert(ff_disk_read(&disk,io)==FF_VM_ACCESS_VIOLATION);unchanged();io[5]=DATA;
    io[4]=DATA;save();assert(ff_disk_read(&disk,io)==FF_BOOT_UNSUPPORTED);unchanged();io[4]=IOSB;
    put(OFFSET,UINT64_MAX,8);save();assert(ff_disk_read(&disk,io)==FF_BOOT_INVALID);unchanged();
    assert(!ff_vm_init(&memory,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&memory,0x10000,0x30000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x1000,4));
    assert(!ff_vm_map_fixed(&memory,DATA,4096,2));
    assert(!ff_vm_map_fixed(&memory,DATA+4096,0x40000-DATA-4096,4));put(OFFSET,2048,8);
    save();assert(ff_disk_read(&disk,io)==FF_VM_ACCESS_VIOLATION);unchanged();
    reset();open_args[0]=IOSB;save();assert(ff_disk_open(&disk,open_args)==FF_BOOT_UNSUPPORTED);unchanged();
    reset();ram[TEXT+27]='9';save();assert(ff_disk_open(&disk,open_args)==FF_BOOT_UNSUPPORTED);unchanged();
}
static void sparse_capacity_failure_preserves_disk_and_position(void)
{
    reset();uint32_t file=open_disk();
    memset(ram+DATA,0x5a,4096);
    uint32_t io[8]={file,0,0,0,IOSB,DATA,4096,OFFSET};
    for(unsigned i=0;i<FF_DISK_PAGES;++i){put(OFFSET,(uint64_t)i*4096,8);assert(!ff_disk_write(&disk,io));}
    assert(disk.allocated_pages==FF_DISK_PAGES);put(OFFSET,(uint64_t)FF_DISK_PAGES*4096,8);
    saved_disk=disk;assert(ff_disk_write(&disk,io)==FF_DISK_FULL);
    assert(!memcmp(&disk,&saved_disk,sizeof disk) && get(IOSB+4,4)==0);
    memset(ram+DATA,0,4096);put(OFFSET,0,8);assert(!ff_disk_write(&disk,io));assert(disk.allocated_pages==FF_DISK_PAGES-1);
    memset(ram+DATA,0x6b,4096);put(OFFSET,(uint64_t)FF_DISK_PAGES*4096,8);assert(!ff_disk_write(&disk,io));
    assert(disk.allocated_pages==FF_DISK_PAGES);
    ff_disk_profile bad=profile;bad.cache_partitions=0;saved_disk=disk;
    assert(ff_disk_init(&disk,&memory,&bad)==FF_BOOT_INVALID && !memcmp(&disk,&saved_disk,sizeof disk));
}
static void explicit_partition_views_share_backing_without_crossing_boundaries(void)
{
    reset();
    for(unsigned i=0;i<3;++i)assert(!ff_disk_add_partition(&disk,3+i,0x100000000ull+(uint64_t)i*0x40000000,0x40000000));
    save();assert(ff_disk_add_partition(&disk,5,0x1c0000000ull,512)==FF_BOOT_INVALID);unchanged();
    const char *part5="\\Device\\Harddisk0\\Partition5";memcpy(ram+TEXT,part5,strlen(part5)+1);
    open_args[4]=0;open_args[5]=0x18;uint32_t selected=open_disk();
    assert(ff_disk_open(&disk,open_args)==FF_DISK_SHARING);
    uint32_t io[8]={selected,0,0,0,IOSB,DATA,512,OFFSET};put(OFFSET,512,8);memset(ram+DATA,0x9a,512);
    assert(!ff_disk_write(&disk,io));
    assert(ff_disk_byte(&disk,0x180000200ull)==0x9a && !ff_disk_byte(&disk,512));
    ram[TEXT+27]='0';uint32_t whole=open_disk();io[0]=whole;put(OFFSET,0x180000200ull,8);
    memset(ram+DATA,0,512);assert(!ff_disk_read(&disk,io));for(unsigned i=0;i<512;++i)assert(ram[DATA+i]==0x9a);
    ram[TEXT+27]='3';uint32_t other=open_disk();io[0]=other;put(OFFSET,512,8);
    assert(!ff_disk_read(&disk,io));for(unsigned i=0;i<512;++i)assert(!ram[DATA+i]);
    io[0]=selected;io[6]=1024;put(OFFSET,0x40000000ull-512,8);memset(ram+DATA,0x6c,1024);
    saved_disk=disk;assert(ff_disk_write(&disk,io)==FF_DISK_FULL);assert(!memcmp(&disk,&saved_disk,sizeof disk));
    assert(!ff_disk_read(&disk,io) && get(IOSB+4,4)==512 && ram[DATA+512]==0x6c);
    save();assert(ff_disk_add_partition(&disk,3,0,512)==FF_BOOT_INVALID);unchanged();
    assert(!ff_disk_close(&disk,selected));assert(!ff_disk_close(&disk,whole));assert(!ff_disk_close(&disk,other));
    reset();assert(!ff_disk_add_partition(&disk,3,4096,4096));
    for(unsigned i=0;i<4;++i) {
        uint32_t number=i==0?3:4;uint64_t offset=i==1?4096:i==2?4097:profile.bytes-512,bytes=i==3?1024:4096;
        save();assert(ff_disk_add_partition(&disk,number,offset,bytes)==FF_BOOT_INVALID);unchanged();
    }
}
static void explicit_geometry_has_checked_output_and_no_disk_mutation(void)
{
    reset();save();assert(ff_disk_set_geometry(&disk,32767,16,32)==FF_BOOT_INVALID);unchanged();
    assert(!ff_disk_set_geometry(&disk,32768,16,32));
    uint32_t file=open_disk(),args[10]={file,0,0,0,IOSB,0x70000,0,0,DATA,32};
    saved_disk=disk;assert(!ff_disk_control(&disk,args));assert(!memcmp(&disk,&saved_disk,sizeof disk));
    assert(get(DATA,8)==32768 && get(DATA+8,4)==12 && get(DATA+12,4)==16 && get(DATA+16,4)==32 && get(DATA+20,4)==512);
    assert(get(IOSB+4,4)==24 && ram[DATA+24]==0xa5);
    args[9]=23;memcpy(saved_ram,ram,sizeof ram);assert(ff_disk_control(&disk,args)==0xc0000023u);
    assert(!memcmp(ram+DATA,saved_ram+DATA,32) && get(IOSB+4,4)==0);
    args[9]=24;args[8]=IOSB;save();assert(ff_disk_control(&disk,args)==FF_BOOT_UNSUPPORTED);unchanged();args[8]=DATA;
    args[8]=0x3fff0;save();assert(ff_disk_control(&disk,args)==FF_VM_ACCESS_VIOLATION);unchanged();args[8]=DATA;
    args[5]=0x74004;save();assert(ff_disk_control(&disk,args)==FF_BOOT_UNSUPPORTED);unchanged();args[5]=0x70000;
    args[2]=1;save();assert(ff_disk_control(&disk,args)==FF_BOOT_UNSUPPORTED);unchanged();args[2]=0;
    assert(!ff_disk_close(&disk,file));save();assert(ff_disk_control(&disk,args)==FF_BOOT_BAD_HANDLE);unchanged();
}
static void partition_information_preserves_explicit_descriptor_and_range(void)
{
    reset();assert(!ff_disk_add_partition(&disk,5,0x180000000ull,0x40000000));
    save();assert(ff_disk_set_partition_information(&disk,5,1,0x7f,5)==FF_BOOT_INVALID);unchanged();
    assert(!ff_disk_set_partition_information(&disk,5,0xc00000,0x7f,5));
    const char *name="\\Device\\Harddisk0\\partition5";memcpy(ram+TEXT,name,strlen(name)+1);
    uint32_t file=open_disk(),args[10]={file,0,0,0,IOSB,0x74004,0,0,DATA,40};
    saved_disk=disk;assert(!ff_disk_control(&disk,args));assert(!memcmp(&disk,&saved_disk,sizeof disk));
    assert(get(DATA,8)==0x180000000ull && get(DATA+8,8)==0x40000000 && get(DATA+16,4)==0xc00000 && get(DATA+20,4)==5);
    assert(ram[DATA+24]==0x7f && ram[DATA+25]==1 && !ram[DATA+26] && ram[DATA+27]==1);
    assert(!get(DATA+28,4) && ram[DATA+32]==0xa5 && get(IOSB+4,4)==32);
    args[9]=31;memcpy(saved_ram,ram,sizeof ram);assert(ff_disk_control(&disk,args)==0xc0000023u);
    assert(!memcmp(ram+DATA,saved_ram+DATA,40) && !get(IOSB+4,4));
}
static void raw_attachment_dismount_retires_after_close_and_preserves_bytes(void)
{
    reset();assert(!ff_disk_add_partition(&disk,5,0x180000000ull,0x40000000));
    save();assert(ff_disk_attach_raw_volume(&disk,4)==FF_BOOT_INVALID);unchanged();
    assert(!ff_disk_attach_raw_volume(&disk,5));
    save();assert(ff_disk_attach_raw_volume(&disk,5)==FF_BOOT_INVALID);unchanged();
    const char *name="\\Device\\Harddisk0\\partition5";memcpy(ram+TEXT,name,strlen(name)+1);
    open_args[4]=0;open_args[5]=0x18;uint32_t file=open_disk();
    uint32_t control[10]={file,0,0,0,IOSB,0x90020,0,0,0,0};
    uint32_t io[8]={file,0,0,0,IOSB,DATA,512,OFFSET};put(OFFSET,0,8);memset(ram+DATA,0x37,512);
    assert(!ff_disk_write(&disk,io));assert(disk.volumes[0].references==1);
    control[5]=0x90018;save();assert(ff_disk_fs_control(&disk,control)==FF_BOOT_UNSUPPORTED);unchanged();control[5]=0x90020;
    control[4]=0x40000;save();assert(ff_disk_fs_control(&disk,control)==FF_VM_ACCESS_VIOLATION);unchanged();control[4]=IOSB;
    control[2]=1;save();assert(ff_disk_fs_control(&disk,control)==FF_BOOT_UNSUPPORTED);unchanged();control[2]=0;
    assert(!ff_disk_fs_control(&disk,control));assert(!get(IOSB,4) && !get(IOSB+4,4));
    assert(disk.volumes[0].state==FF_DISK_VOLUME_DISMOUNTED && disk.volumes[0].references==1 && disk.volumes[0].dismounts==1);
    assert(disk.active==1 && disk.allocated_pages==1 && ff_disk_byte(&disk,0x180000000ull)==0x37);
    save();assert(ff_disk_open(&disk,open_args)==FF_BOOT_UNSUPPORTED);unchanged();
    save();assert(ff_disk_fs_control(&disk,control)==FF_BOOT_UNSUPPORTED);unchanged();
    memset(ram+DATA,0,512);assert(!ff_disk_read(&disk,io));for(unsigned i=0;i<512;++i)assert(ram[DATA+i]==0x37);
    assert(!ff_disk_close(&disk,file));assert(!disk.active && !disk.volumes[0].references && disk.volumes[0].state==FF_DISK_VOLUME_RETIRED);
    assert(ff_disk_byte(&disk,0x180000000ull)==0x37 && disk.allocated_pages==1);
    save();assert(ff_disk_fs_control(&disk,control)==FF_BOOT_BAD_HANDLE);unchanged();
    reset();assert(!ff_disk_add_partition(&disk,5,0x180000000ull,0x40000000));assert(!ff_disk_attach_raw_volume(&disk,5));
    memcpy(ram+TEXT,name,strlen(name)+1);file=open_disk();uint32_t other=open_disk();control[0]=file;
    assert(disk.volumes[0].references==2);save();assert(ff_disk_fs_control(&disk,control)==FF_BOOT_UNSUPPORTED);unchanged();
    assert(!ff_disk_close(&disk,file) && disk.volumes[0].references==1);assert(!ff_disk_close(&disk,other));
    assert(disk.volumes[0].state==FF_DISK_VOLUME_ATTACHED && !disk.volumes[0].references);
    reset();file=open_disk();control[0]=file;save();assert(ff_disk_fs_control(&disk,control)==FF_BOOT_UNSUPPORTED);unchanged();
}
int main(void)
{
    sparse_roundtrip_and_lifetime();permissions_sharing_and_rejected_memory();sparse_capacity_failure_preserves_disk_and_position();
    explicit_partition_views_share_backing_without_crossing_boundaries();
    explicit_geometry_has_checked_output_and_no_disk_mutation();
    partition_information_preserves_explicit_descriptor_and_range();
    raw_attachment_dismount_retires_after_close_and_preserves_bytes();
    puts("owned sparse disk and partition aliasing, range, lifetime and access checks passed");return 0;
}
