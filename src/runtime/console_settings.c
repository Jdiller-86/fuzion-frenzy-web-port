#include "console_settings.h"
#include "thread_bootstrap.h"

static void put(uint8_t *ram,uint32_t address,uint32_t value)
{ for(unsigned i=0;i<4;++i)ram[address+i]=(uint8_t)(value>>(i*8u)); }
static int writable(ff_virtual_memory *memory,uint32_t address,uint32_t alignment)
{ return address && !(address%alignment) && !ff_vm_check(memory,address,4,FF_VM_WRITE); }
static int overlaps(uint32_t a,uint32_t b)
{ return (uint64_t)a+4>b && (uint64_t)b+4>a; }

uint32_t ff_query_console_dword(ff_virtual_memory *memory,const uint32_t args[5],uint32_t value)
{
    if(!memory || !memory->ram || !args)return FF_BOOT_INVALID;
    if(args[0]!=10 && args[0]!=17)return FF_BOOT_UNSUPPORTED;
    uint32_t type=args[1],buffer=args[2],capacity=args[3],required=args[4];
    if(required && !writable(memory,required,4))return FF_VM_ACCESS_VIOLATION;
    if(capacity<4) {
        if(required)put(memory->ram,required,4);
        return 0xc0000023u;
    }
    if(!writable(memory,type,4) || !writable(memory,buffer,1))return FF_VM_ACCESS_VIOLATION;
    /* Unsupported aliases stop before mutation; they are not fabricated Xbox
     * failure ordering. Capacity beyond the returned four bytes is untouched. */
    if(overlaps(type,buffer) || (required && (overlaps(required,type) || overlaps(required,buffer))))return FF_BOOT_UNSUPPORTED;
    put(memory->ram,type,4);put(memory->ram,buffer,value);
    if(required)put(memory->ram,required,4);
    return 0;
}

static void put_at(uint8_t *bytes,uint32_t at,uint32_t value)
{ for(unsigned i=0;i<4;++i)bytes[at+i]=(uint8_t)(value>>(i*8u)); }
static int ranges_overlap(uint32_t first,uint32_t first_bytes,uint32_t second,uint32_t second_bytes)
{ return (uint64_t)first+first_bytes>second && (uint64_t)second+second_bytes>first; }

uint32_t ff_query_console_block(ff_virtual_memory *memory,const uint32_t args[5],const ff_console_settings_block *settings)
{
    if(!memory || !memory->ram || !args || !settings)return FF_BOOT_INVALID;
    if(args[0]!=0xffu)return FF_BOOT_UNSUPPORTED;
    uint32_t type=args[1],buffer=args[2],capacity=args[3],required=args[4];
    if(!writable(memory,type,4))return FF_VM_ACCESS_VIOLATION;
    if(required && !writable(memory,required,4))return FF_VM_ACCESS_VIOLATION;
    if(required && ranges_overlap(type,4,required,4))return FF_BOOT_UNSUPPORTED;
    /* The pinned model reports the fixed width and type before it refuses a
     * short buffer; the value bytes stay untouched in that case. */
    if(capacity<FF_CONSOLE_SETTINGS_BLOCK_BYTES) {
        put(memory->ram,type,4);
        if(required)put(memory->ram,required,4);
        return 0xc0000023u;
    }
    if(!writable(memory,buffer,1) || ff_vm_check(memory,buffer,capacity,FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    if(ranges_overlap(type,4,buffer,capacity) || (required && (ranges_overlap(required,4,buffer,capacity) || ranges_overlap(required,4,type,4))))return FF_BOOT_UNSUPPORTED;
    uint8_t block[FF_CONSOLE_SETTINGS_BLOCK_BYTES]={0};
    put_at(block,0x04,(uint32_t)settings->bias);
    for(unsigned i=0;i<4;++i) {
        block[0x08+i]=settings->standard_name[i];
        block[0x0c+i]=settings->daylight_name[i];
    }
    put_at(block,0x18,settings->standard_date);put_at(block,0x1c,settings->daylight_date);
    put_at(block,0x28,(uint32_t)settings->standard_bias);put_at(block,0x2c,(uint32_t)settings->daylight_bias);
    put_at(block,0x30,settings->language);put_at(block,0x34,settings->video);
    put_at(block,0x38,settings->audio);put_at(block,0x3c,settings->parental);
    for(unsigned i=0;i<4;++i)put_at(block,0x48+4u*i,settings->online[i]);
    put_at(block,0x58,settings->misc);put_at(block,0x5c,settings->dvd_region);
    for(unsigned i=0;i<FF_CONSOLE_SETTINGS_BLOCK_BYTES;++i)memory->ram[buffer+i]=block[i];
    for(uint32_t i=FF_CONSOLE_SETTINGS_BLOCK_BYTES;i<capacity;++i)memory->ram[buffer+i]=0;
    put(memory->ram,type,3u);
    if(required)put(memory->ram,required,FF_CONSOLE_SETTINGS_BLOCK_BYTES);
    return 0;
}
