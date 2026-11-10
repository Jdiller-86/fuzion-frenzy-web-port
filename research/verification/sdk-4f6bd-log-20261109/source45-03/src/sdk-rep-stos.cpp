#include "sdk-shared-prototypes.hpp"
#include "shared-abi.h"

void ff_sdk_rep_stos(X86Context& c,uint8_t* base,uint32_t width)
{
    const uint32_t count=c.ecx;
    const uint32_t bytes=count*4u;
    const uint32_t destination=c.edi;
    const uint32_t profile=ff_probe_sdk_chain_word(0u);
    const bool startup_fill=profile>=23u && ff_probe_sdk_chain_word(17u)==0x4f5e0u &&
        ((count==0x232eu && destination==0x2d15e8u) || (count==11u && destination==0x2d15b8u));
    const bool parent_fill=profile>=15u && count==0x310u && c.edi==0x2cb570u;
    const bool prior_fill=(count==0x73u || count==0x24u) && c.edi==0x2cc1f0u;
    if (!ff_sdk_context_active(c,base) || width!=4u || c.df!=0u ||
        (!startup_fill && !parent_fill && !prior_fill) || c.eax!=0u)
        ff_sdk_raise(c,0x49d50u,"REP STOS shape outside the reached bounded profile");
    if(startup_fill){
        const ff_sdk_scalar live{c.eax,c.ecx,c.edx,c.ebx,c.esp,c.ebp,c.esi,c.edi,c.fs_base,c.df};
        ff_sdk_shared_write(&live);ff_sdk_shared_stos_event(0u);
    }else if (count==0x24u) {
        const ff_sdk_scalar live{c.eax,c.ecx,c.edx,c.ebx,c.esp,c.ebp,c.esi,c.edi,c.fs_base,c.df};
        ff_sdk_shared_write(&live);ff_sdk_shared_device_stos_event(0u);
    } else ff_sdk_shared_stos_event(0u);
    /* Conservative complete-span admission; every real store still uses the
       existing checked accessor and unchanged per-access work guard. */
    (void)ff_sdk_checked_span(c,base,destination,bytes,FF_VM_WRITE);
    for (uint32_t i=0;i<count;++i) {
        FFSDKGuestRef<uint32_t>(c,base,c.edi)=c.eax;
        c.edi+=4u;--c.ecx;
    }
    const ff_sdk_scalar live{c.eax,c.ecx,c.edx,c.ebx,c.esp,c.ebp,c.esi,c.edi,c.fs_base,c.df};
    ff_sdk_shared_write(&live);
    if (count==0x24u) ff_sdk_shared_device_stos_event(1u);
    else ff_sdk_shared_stos_event(1u);
}
