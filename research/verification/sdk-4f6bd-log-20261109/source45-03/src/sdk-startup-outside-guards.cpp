#include <cstring>
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"
X86_FUNC(sub_00030B2D) { (void)base; ff_sdk_raise(ctx, 0x00030B2Du, "unsupported original direct dependency"); }
extern "C" void ff_sdk_shared_c_close(ff_sdk_scalar*);
X86_FUNC(sub_0003417A) {
    if(!ff_sdk_context_active(ctx,base) || (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))))))) ||
       ff_probe_sdk_chain_word(17u)!=0x4f5e0u || ff_probe_sdk_chain_word(3124u) || ctx.esp!=0x03c10c54u)
        ff_sdk_raise(ctx,0x3417au,"original C close provider outside exact current profile/frame");
    const uint32_t cf=ctx.cf,zf=ctx.zf,sf=ctx.sf,of=ctx.of,pf=ctx.pf,xflags=ctx.xflags;
    ff_sdk_scalar s{ctx.eax,ctx.ecx,ctx.edx,ctx.ebx,ctx.esp,ctx.ebp,ctx.esi,ctx.edi,ctx.fs_base,ctx.df};
    ff_sdk_shared_c_close(&s);
    ctx.eax=s.eax;ctx.ecx=s.ecx;ctx.edx=s.edx;ctx.ebx=s.ebx;ctx.esp=s.esp;
    ctx.ebp=s.ebp;ctx.esi=s.esi;ctx.edi=s.edi;ctx.fs_base=s.fs_base;ctx.df=s.df;
    if(ctx.cf!=cf || ctx.zf!=zf || ctx.sf!=sf || ctx.of!=of || ctx.pf!=pf || ctx.xflags!=xflags ||
       ctx.esp!=0x03c10c5cu || ctx.eax!=1u || ff_probe_sdk_chain_word(3125u)!=1u)
        ff_sdk_raise(ctx,0x3417au,"C close return scalar/flag transport rejected");
}
extern "C" void ff_sdk_shared_c_4c480(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope44_cpp_complete(void);
X86_FUNC(sub_0004C480) {
    if(!ff_sdk_context_active(ctx,base)||(ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)||
       ff_probe_sdk_chain_word(17u)!=0x4f5e0u||ctx.esp!=0x03c10e50u)
        ff_sdk_raise(ctx,0x4c480u,"C wrapper outside exact M44 caller");
    const X86Context before=ctx;
    ff_sdk_scalar s{ctx.eax,ctx.ecx,ctx.edx,ctx.ebx,ctx.esp,ctx.ebp,ctx.esi,ctx.edi,ctx.fs_base,ctx.df};
    ff_sdk_shared_c_4c480(&s);
    ctx.eax=s.eax;ctx.ecx=s.ecx;ctx.edx=s.edx;ctx.ebx=s.ebx;ctx.esp=s.esp;
    ctx.ebp=s.ebp;ctx.esi=s.esi;ctx.edi=s.edi;ctx.fs_base=s.fs_base;ctx.df=s.df;
    if(!ff_sdk_context_active(ctx,base)||ctx.cf!=before.cf||ctx.zf!=before.zf||ctx.sf!=before.sf||
       ctx.of!=before.of||ctx.pf!=before.pf||ctx.xflags!=before.xflags||
       ctx.top!=before.top||ctx.fcw!=before.fcw||ctx.fsw!=before.fsw||ctx.mxcsr!=before.mxcsr||
       memcmp(ctx.st,before.st,sizeof ctx.st)||memcmp(ctx.mm,before.mm,sizeof ctx.mm)||memcmp(ctx.xmm,before.xmm,sizeof ctx.xmm)||
       ctx.esp!=0x03c10e54u||ctx.ebp!=before.ebp||ctx.ebx!=before.ebx||ctx.esi!=before.esi||ctx.edi!=before.edi||
       ctx.fs_base!=before.fs_base||ctx.df!=before.df)
        ff_sdk_raise(ctx,0x4c480u,"M44 C wrapper context/FP/flag transport rejected");
    /* Arithmetic return flags are not generalized: original4F68E ADD ESP,10
       and4F691 TEST EAX define them before the next original conditional. */
    ff_probe_sdk_scope44_cpp_complete();
}
X86_FUNC(sub_0004D620) { if((ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u)) && !ff_sdk_direct_entry(ctx,base,sub_0004D620))return; (void)base; ff_sdk_raise(ctx, 0x0004D620u, "unsupported original direct dependency"); }
X86_FUNC(sub_0004EA30) { (void)base; ff_sdk_raise(ctx, 0x0004EA30u, "unsupported original direct dependency"); }
X86_FUNC(sub_0004EAF0) { (void)base; ff_sdk_raise(ctx, 0x0004EAF0u, "unsupported original direct dependency"); }
X86_FUNC(sub_001DEE6E) { (void)base; ff_sdk_raise(ctx, 0x001DEE6Eu, "unsupported original direct dependency"); }
X86_FUNC(sub_001DFE85) { (void)base; ff_sdk_raise(ctx, 0x001DFE85u, "unsupported original direct dependency"); }
extern "C" void ff_sdk_shared_c_1e01ad(ff_sdk_scalar*);
X86_FUNC(sub_001E01AD) {
    if(!ff_sdk_context_active(ctx,base) || (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u ||
       ff_probe_sdk_chain_word(3316u) || ctx.esp!=0x03c10e54u)
        ff_sdk_raise(ctx,0x1e01adu,"C provider outside exact M35 caller/context");
    const X86Context before=ctx;
    ff_sdk_scalar s{ctx.eax,ctx.ecx,ctx.edx,ctx.ebx,ctx.esp,ctx.ebp,ctx.esi,ctx.edi,ctx.fs_base,ctx.df};
    ff_sdk_shared_c_1e01ad(&s);
    ctx.eax=s.eax;ctx.ecx=s.ecx;ctx.edx=s.edx;ctx.ebx=s.ebx;ctx.esp=s.esp;ctx.ebp=s.ebp;ctx.esi=s.esi;ctx.edi=s.edi;ctx.fs_base=s.fs_base;ctx.df=s.df;
    if(ctx.cf!=before.cf || ctx.zf!=before.zf || ctx.sf!=before.sf || ctx.of!=before.of || ctx.pf!=before.pf || ctx.xflags!=before.xflags ||
       ctx.top!=before.top || ctx.fcw!=before.fcw || ctx.fsw!=before.fsw || ctx.mxcsr!=before.mxcsr ||
       memcmp(ctx.st,before.st,sizeof ctx.st) || memcmp(ctx.mm,before.mm,sizeof ctx.mm) || memcmp(ctx.xmm,before.xmm,sizeof ctx.xmm) ||
       ctx.esp!=0x03c10e58u || ctx.ebp!=before.ebp || ff_probe_sdk_chain_word(3317u)!=1u)
        ff_sdk_raise(ctx,0x1e01adu,"C normal-return context/FP/flag transport rejected");
}
X86_FUNC(sub_001E0EA0) { (void)base; ff_sdk_raise(ctx, 0x001E0EA0u, "unsupported original direct dependency"); }
X86_FUNC(sub_001E91CA) { (void)base; ff_sdk_raise(ctx, 0x001E91CAu, "unsupported original direct dependency"); }
X86_FUNC(sub_001E93E4) { (void)base; ff_sdk_raise(ctx, 0x001E93E4u, "unsupported original direct dependency"); }
X86_FUNC(sub_001E97CF) { (void)base; ff_sdk_raise(ctx, 0x001E97CFu, "unsupported original direct dependency"); }
X86_FUNC(sub_0004B380) { (void)base; ff_sdk_raise(ctx, 0x0004B380u, "unsupported original logger terminal"); }
X86_FUNC(sub_001E6AAF) { (void)base; ff_sdk_raise(ctx,0x001E6AAFu,"unobserved mapper allocation path"); }
X86_FUNC(sub_000343F3) { (void)base; ff_sdk_raise(ctx,0x000343F3u,"unobserved mapper initialization path"); }
X86_FUNC(sub_001E564E) { (void)base; ff_sdk_raise(ctx,0x001E564Eu,"unobserved mapper allocation failure path"); }
X86_FUNC(sub_001E566A) { (void)base; ff_sdk_raise(ctx, 0x001E566Au, "unsupported original direct dependency"); }
