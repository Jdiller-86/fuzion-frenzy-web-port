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
