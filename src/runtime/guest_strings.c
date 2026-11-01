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
