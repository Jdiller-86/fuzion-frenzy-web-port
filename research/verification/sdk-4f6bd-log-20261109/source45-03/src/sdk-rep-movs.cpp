#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"

extern "C" void ff_sdk_shared_record_movs(ff_sdk_scalar *,uint32_t width,uint32_t complete);

namespace {
ff_sdk_scalar scalar_from(const X86Context& c) {
    return {c.eax,c.ecx,c.edx,c.ebx,c.esp,c.ebp,c.esi,c.edi,c.fs_base,c.df};
}
[[noreturn]] void reject(X86Context& c,uint32_t address,const char *reason) {
    ff_sdk_raise(c,address,reason);
    __builtin_unreachable();
}
}

/* The sole additional REP MOVS admission is the two source-derived operations
   in original helper 0x001E956F under profile 27/current root 0x0004F5E0.
   The shared C callback verifies the dynamic caller/stage and preflights the
   complete nonempty read/write spans before any scalar access. */
extern "C" void ff_sdk_record_rep_movs(X86Context& c,uint8_t *base,uint32_t width) {
    if(!ff_sdk_context_active(c,base)||c.df!=0u)
        reject(c,0x001E956Fu,"record REP MOVS requires active current thread and DF=0");

    const uint32_t source=c.esi,destination=c.edi,count=c.ecx;
    if(width==4u) {
        if(count!=3u||source!=0x03c10b28u||destination!=0x03c10cc8u)
            reject(c,0x001E95bdu,"record MOVSD shape outside exact profile-27 helper");
        const uint64_t source_end=uint64_t(source)+12u,destination_end=uint64_t(destination)+12u;
        if(uint64_t(source)<destination_end&&uint64_t(destination)<source_end)
            reject(c,source,"record MOVSD overlapping ranges are unsupported");
    } else if(width==1u) {
        if(count!=0u||source!=0x03c10b34u||destination!=0x03c10cd4u)
            reject(c,0x001E95c4u,"record MOVSB shape outside exact profile-27 helper");
    } else {
        reject(c,0x001E956Fu,"record REP MOVS width is unsupported");
    }

    ff_sdk_scalar s=scalar_from(c);
    ff_sdk_shared_record_movs(&s,width,0u);

    /* MOVS leaves flags unchanged. The nonempty DWORD operation performs each
       scalar read and write through the existing checked/accounted interface. */
    if(width==4u) {
        for(uint32_t i=0;i<3u;++i) {
            const uint32_t value=FFSDKGuestRef<uint32_t>(c,base,c.esi);
            FFSDKGuestRef<uint32_t>(c,base,c.edi)=value;
            c.esi+=4u;c.edi+=4u;--c.ecx;
        }
    }

    s=scalar_from(c);
    ff_sdk_shared_record_movs(&s,width,1u);
}
