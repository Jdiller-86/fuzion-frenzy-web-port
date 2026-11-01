/* Owned volume/handle/memory scenarios only. No original code or disk image. */
#include "../src/runtime/local_volume.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[128 * 1024], saved_ram[sizeof ram];
static ff_virtual_memory memory;
static ff_local_volume volume, saved_volume;
static const ff_volume_profile profile = { 0x100000003ull, 0x100000001ull, 32, 512 };
enum { OUTPUT = 0x10100, IOSB = 0x10120, ATTR = 0x10140, NAME = 0x10180, TEXT = 0x10200, DATA = 0x10300 };
static uint32_t args[6];
static void put(uint32_t p, uint64_t n, unsigned bytes) { for (unsigned i = 0; i < bytes; ++i) ram[p+i] = (uint8_t)(n >> (i*8)); }
static uint64_t get(uint32_t p, unsigned bytes) { uint64_t n = 0; for (unsigned i = 0; i < bytes; ++i) n |= (uint64_t)ram[p+i] << (i*8); return n; }
static void name(const char *value)
{
    size_t count = strlen(value); assert(count < 1024);
    memcpy(ram + TEXT, value, count + 1); put(NAME, count, 2); put(NAME+2, count+1, 2); put(NAME+4, TEXT, 4);
}
static void reset(void)
{
    memset(ram, 0xa5, sizeof ram);
    assert(ff_vm_init(&memory, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&memory, 0x10000, 0x10000) == 0);
    assert(ff_vm_map_fixed(&memory, 0x10000, 0x10000, 4) == 0);
    assert(ff_volume_init(&volume, &memory, &profile) == 0);
    args[0]=OUTPUT; args[1]=0x100001; args[2]=ATTR; args[3]=IOSB; args[4]=3; args[5]=0x800021;
    put(ATTR, 0, 4); put(ATTR+4, NAME, 4); put(ATTR+8, 0x40, 4);
    name("\\Device\\Harddisk0\\partition1\\");
}
static void save(void) { saved_volume = volume; memcpy(saved_ram, ram, sizeof ram); }
static void unchanged(void) { assert(!memcmp(&volume, &saved_volume, sizeof volume)); assert(!memcmp(ram, saved_ram, sizeof ram)); }
static uint32_t open_root(void)
{
    assert(ff_volume_open_root(&volume, args) == 0);
    assert(get(IOSB,4) == 0 && get(IOSB+4,4) == 1);
    uint32_t handle = (uint32_t)get(OUTPUT,4); assert(ff_volume_owns_handle(handle)); return handle;
}
static void lifecycle_and_metadata(void)
{
    reset(); uint32_t first = open_root();
    assert(volume.active == 1 && volume.opened == 1 && volume.closed == 0);
    uint32_t query[5] = {first, IOSB, DATA, 24, 3};
    assert(ff_volume_query_size(&volume, query) == 0);
    assert(get(DATA,8) == profile.total_units && get(DATA+8,8) == profile.available_units);
    assert(get(DATA+16,4) == 32 && get(DATA+20,4) == 512 && get(IOSB+4,4) == 24);
    assert(ram[DATA-1] == 0xa5 && ram[DATA+24] == 0xa5);
    uint32_t second = open_root(); assert(second != first && volume.active == 2);
    assert(ff_volume_close(&volume, first) == 0 && volume.active == 1);
    save(); assert(ff_volume_query_size(&volume, query) == FF_BOOT_BAD_HANDLE); unchanged();
    assert(ff_volume_close(&volume, first) == FF_BOOT_BAD_HANDLE); unchanged();
    assert(ff_volume_close(&volume, second) == 0 && volume.active == 0 && volume.closed == 2);
    uint32_t third = open_root(); assert(third != first && third != second);
    assert(!ff_volume_owns_handle(0x100) && !ff_volume_owns_handle(FF_VOLUME_HANDLE_LIMIT));
}
static void counted_namespace_and_relative_handles(void)
{
    reset(); name("\\DEVICE\\HARDDISK0\\PARTITION1"); uint32_t first = open_root();
    put(ATTR+8,0,4); save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
    name("\\Device\\Harddisk0\\partition1"); open_root();
    name("\\Device\\Harddisk0\\partition10"); save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
    put(ATTR,first,4); put(NAME,0,2); put(NAME+2,0,2); put(NAME+4,0,4); open_root();
    put(ATTR,0x100,4); save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_BAD_HANDLE); unchanged();
    put(ATTR,first,4); name("..\\elsewhere"); save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
}
static void sharing_directory_errors_and_capacity(void)
{
    reset(); args[4]=0; uint32_t exclusive = open_root();
    args[4]=3; save();
    assert(ff_volume_open_root(&volume,args) == FF_VOLUME_SHARING_VIOLATION);
    assert(volume.active == 1 && volume.opened == 1 && get(OUTPUT,4) == exclusive);
    assert(get(IOSB,4) == FF_VOLUME_SHARING_VIOLATION && get(IOSB+4,4) == 0);
    assert(ff_volume_close(&volume,exclusive) == 0); open_root();
    reset(); args[5]=0x800060;
    assert(ff_volume_open_root(&volume,args) == FF_VOLUME_IS_DIRECTORY && volume.active == 0);
    reset(); uint32_t handles[FF_VOLUME_HANDLES];
    for (unsigned i=0;i<FF_VOLUME_HANDLES;++i) handles[i]=open_root();
    assert(ff_volume_open_root(&volume,args) == FF_VOLUME_TOO_MANY_HANDLES && volume.active == FF_VOLUME_HANDLES);
    assert(ff_volume_close(&volume,handles[4]) == 0); assert(open_root() != handles[4]);
}
static void pointer_permissions_and_failure_atomicity(void)
{
    reset(); args[0]=0x20000; save(); assert(ff_volume_open_root(&volume,args) == FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); args[2]=0xfffffffcu; save(); assert(ff_volume_open_root(&volume,args) == FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); put(ATTR+4,0x1fffcu,4); save(); assert(ff_volume_open_root(&volume,args) == FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); put(NAME+4,0x1fffcu,4); save(); assert(ff_volume_open_root(&volume,args) == FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); put(NAME,1025,2); put(NAME+2,1025,2); save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[0]=IOSB; save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[0]=OUTPUT+1; save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_INVALID); unchanged();
    reset(); assert(ff_vm_init(&memory,ram,sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&memory,0x10000,0x10000) == 0);
    assert(ff_vm_map_fixed(&memory,0x10000,0x10000,2) == 0); save();
    assert(ff_volume_open_root(&volume,args) == FF_VM_ACCESS_VIOLATION); unchanged();
    reset(); args[5]|=0x1000; save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
    reset(); args[1]=1; save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_INVALID); unchanged();
    reset(); args[1]|=2; save(); assert(ff_volume_open_root(&volume,args) == FF_BOOT_UNSUPPORTED); unchanged();
}
static void query_failures_and_profiles(void)
{
    reset(); uint32_t query[5]={open_root(),IOSB,DATA,23,3};
    assert(ff_volume_query_size(&volume,query) == FF_VOLUME_BUFFER_TOO_SMALL && get(IOSB+4,4) == 0);
    assert(ram[DATA] == 0xa5); query[3]=24; query[4]=7; save();
    assert(ff_volume_query_size(&volume,query) == FF_BOOT_UNSUPPORTED); unchanged();
    query[4]=3; query[2]=0x1fff0; save(); assert(ff_volume_query_size(&volume,query) == FF_VM_ACCESS_VIOLATION); unchanged();
    query[2]=IOSB; save(); assert(ff_volume_query_size(&volume,query) == FF_BOOT_UNSUPPORTED); unchanged();
    const ff_volume_profile invalid[]={{0,0,32,512},{1,2,32,512},{1,1,3,512},{1,1,256,512},
        {1,1,32,256},{1,1,32,513},{UINT64_MAX,0,32,512},{INT64_MAX,0,128,4096}};
    for (unsigned i=0;i<sizeof invalid/sizeof invalid[0];++i) { save(); assert(ff_volume_init(&volume,&memory,&invalid[i]) == FF_BOOT_INVALID); unchanged(); }
}
int main(void)
{
    lifecycle_and_metadata(); counted_namespace_and_relative_handles(); sharing_directory_errors_and_capacity();
    pointer_permissions_and_failure_atomicity(); query_failures_and_profiles();
    puts("Owned volume namespace, handles, sharing, metadata and memory checks passed"); return 0;
}
