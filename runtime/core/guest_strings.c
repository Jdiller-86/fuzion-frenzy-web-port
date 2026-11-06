#include "guest_strings.h"
#include "thread_bootstrap.h"

uint32_t ff_init_ansi_string(ff_virtual_memory *memory, uint32_t destination, uint32_t source)
{
    if (!memory || !memory->ram || !destination || (destination & 3u)) return FF_BOOT_INVALID;
    if (ff_vm_check(memory,destination,8,FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    uint32_t length=0, maximum=0;
    if (source) {
        for (;;) {
            uint64_t address=(uint64_t)source+length;
            if (address>UINT32_MAX || ff_vm_check(memory,(uint32_t)address,1,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
            if (!memory->ram[address]) break;
            if (length==65534u) return FF_BOOT_UNSUPPORTED;
            ++length;
        }
        maximum=length+1;
        if ((uint64_t)destination<(uint64_t)source+maximum && (uint64_t)source<(uint64_t)destination+8) return FF_BOOT_UNSUPPORTED;
    }
    memory->ram[destination]=(uint8_t)length; memory->ram[destination+1]=(uint8_t)(length>>8);
    memory->ram[destination+2]=(uint8_t)maximum; memory->ram[destination+3]=(uint8_t)(maximum>>8);
    for(unsigned i=0;i<4;++i) memory->ram[destination+4+i]=(uint8_t)(source>>(i*8));
    return 0;
}
static uint32_t word(const uint8_t *ram,uint32_t address,unsigned bytes)
{
    uint32_t value=0; for(unsigned i=0;i<bytes;++i)value|=(uint32_t)ram[address+i]<<(i*8); return value;
}
static uint8_t upper_byte(uint8_t byte)
{
    if ((byte>='a' && byte<='z') || (byte>=0xe0 && byte<=0xfe && byte!=0xf7)) return (uint8_t)(byte-32);
    return byte==255?'?':byte;
}
uint32_t ff_equal_ansi_string(ff_virtual_memory *memory,uint32_t first,uint32_t second,uint8_t insensitive,uint32_t *equal)
{
    if(!memory || !memory->ram || !equal || !first || !second || ((first|second)&3u)) return FF_BOOT_INVALID;
    if(ff_vm_check(memory,first,8,FF_VM_READ) || ff_vm_check(memory,second,8,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint8_t *ram=memory->ram;
    uint32_t bytes=word(ram,first,2), other=word(ram,second,2);
    if(bytes!=other) { *equal=0; return 0; }
    if(!bytes) { *equal=1; return 0; }
    uint32_t a=word(ram,first+4,4), b=word(ram,second+4,4);
    if(ff_vm_check(memory,a,bytes,FF_VM_READ) || ff_vm_check(memory,b,bytes,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    for(uint32_t i=0;i<bytes;++i) {
        uint8_t left=ram[a+i], right=ram[b+i];
        if(insensitive && left!=right) { left=upper_byte(left); right=upper_byte(right); }
        if(left!=right) { *equal=0; return 0; }
    }
    *equal=1; return 0;
}
