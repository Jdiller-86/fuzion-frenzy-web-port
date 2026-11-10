#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "cpu/x86_ops.h"
extern "C" void ff_sdk_shared_log_bridge_record(ff_sdk_scalar*,uint32_t,uint32_t,uint32_t,uint32_t);
extern "C" void ff_sdk_shared_log_bridge45(ff_sdk_scalar*);
X86_FUNC(sub_0004B230) {
    const uint32_t profile=ff_probe_sdk_chain_word(0u);
    if(profile==45u && ff_probe_sdk_chain_word(17u)==0x4f5e0u && ctx.esp==0x03c10e5cu) {
        if(!ff_sdk_context_active(ctx,base)||ctx.df)ff_sdk_raise(ctx,ctx.esp,"inactive M45 logger context");
        auto*t=static_cast<FFSDKThreadState*>(ctx.host);
        const X86Context before=ctx;
        ff_sdk_scalar s{ctx.eax,ctx.ecx,ctx.edx,ctx.ebx,ctx.esp,ctx.ebp,ctx.esi,ctx.edi,ctx.fs_base,ctx.df};
        ff_sdk_shared_log_bridge45(&s);
        if(!ff_sdk_context_active(ctx,base)||ctx.host!=t||ctx.thread_handle!=before.thread_handle||
           ctx.last_icall!=before.last_icall||ctx.cf!=before.cf||ctx.zf!=before.zf||ctx.sf!=before.sf||
           ctx.of!=before.of||ctx.pf!=before.pf||ctx.xflags!=before.xflags||
           ctx.top!=before.top||ctx.fcw!=before.fcw||ctx.fsw!=before.fsw||ctx.mxcsr!=before.mxcsr)
            ff_sdk_raise(ctx,ctx.esp,"M45 logger changed live SDK context");
        for(uint32_t i=0;i<8u;i++) {
            if(ctx.mm[i]!=before.mm[i]||__builtin_memcmp(&ctx.st[i],&before.st[i],sizeof(double))||
               __builtin_memcmp(&ctx.xmm[i],&before.xmm[i],sizeof(X86Xmm)))
                ff_sdk_raise(ctx,ctx.esp,"M45 logger changed FP state");
        }
        if(s.esp!=before.esp+4u||s.ebp!=before.ebp||s.ebx!=before.ebx||s.esi!=before.esi||
           s.edi!=before.edi||s.fs_base!=before.fs_base||s.df)
            ff_sdk_raise(ctx,s.esp,"M45 logger did not restore original caller");
        ctx.eax=s.eax;ctx.ecx=s.ecx;ctx.edx=s.edx;ctx.ebx=s.ebx;ctx.esp=s.esp;
        ctx.ebp=s.ebp;ctx.esi=s.esi;ctx.edi=s.edi;ctx.fs_base=s.fs_base;ctx.df=s.df;
        return;
    }

    if((profile==28u||(profile==29u||(profile==30u || (profile==31u || (profile==32u || (profile==33u || (profile==34u || (profile==35u || (profile==36u || (profile==37u || (profile==38u || (profile==39u || (profile==40u || (profile==41u || (profile==42u || (profile==43u || (profile==44u || profile==45u)))))))))))))))))&&ff_probe_sdk_chain_word(17u)==0x0004F5E0u) {
        if(!ff_sdk_context_active(ctx,base)||ctx.df!=0u)
            ff_sdk_raise(ctx,0x0004B230u,"profile-28 record logger requires active RUNNING thread and DF=0");
        auto*t=static_cast<FFSDKThreadState*>(ctx.host)->boot_thread;
        const uint32_t entry_esp=ctx.esp;
        if((entry_esp&3u)||entry_esp<t->stack_low||entry_esp>t->stack_top||
           12u>t->stack_top-entry_esp)
            ff_sdk_raise(ctx,entry_esp,"profile-28 logger frame outside current thread stack");
        const uint32_t ret=FFSDKGuestRef<uint32_t>(ctx,base,entry_esp);
        const uint32_t literal=FFSDKGuestRef<uint32_t>(ctx,base,entry_esp+4u);
        const uint32_t ambient=FFSDKGuestRef<uint32_t>(ctx,base,entry_esp+8u);
        if(ret!=0x0004EB7Bu||literal!=0x0025C470u||ambient!=0x03C10E58u||
           ctx.eax!=0x00070038u||ctx.ebp!=0x00070038u||
           ff_probe_sdk_chain_word(1329u)!=1u||
           ctx.ebx!=ff_probe_sdk_chain_word(1395u)||
           ctx.esi!=ff_probe_sdk_chain_word(1396u)||
           ctx.edi!=ff_probe_sdk_chain_word(1397u)||
           ctx.fs_base!=ff_probe_sdk_chain_word(1400u))
            ff_sdk_raise(ctx,entry_esp,"profile-28 logger caller frame/registers differ from the returned copy path");

        /* This is an additional checked preflight observation. The retained C
           logger still performs its formatter and original routine before its
           own late read of the same zero gate. */
        const uint32_t gate=FFSDKGuestRef<uint32_t>(ctx,base,0x002CCCF8u);
        if(gate!=0u)ff_sdk_raise(ctx,0x002CCCF8u,"profile-28 logger gate preflight is nonzero");

        const uint32_t cf=ctx.cf,zf=ctx.zf,sf=ctx.sf,of=ctx.of,pf=ctx.pf,xflags=ctx.xflags;
        ff_sdk_scalar s{ctx.eax,ctx.ecx,ctx.edx,ctx.ebx,ctx.esp,ctx.ebp,ctx.esi,ctx.edi,ctx.fs_base,ctx.df};
        ff_sdk_shared_log_bridge_record(&s,ret,literal,ambient,gate);
        ctx.eax=s.eax;ctx.ecx=s.ecx;ctx.edx=s.edx;ctx.ebx=s.ebx;ctx.esp=s.esp;
        ctx.ebp=s.ebp;ctx.esi=s.esi;ctx.edi=s.edi;ctx.fs_base=s.fs_base;ctx.df=s.df;
        if(ctx.esp!=entry_esp+4u||ctx.eax!=ff_probe_sdk_chain_word(1769u)||ctx.ebp!=0x00070038u||
           ctx.ebx!=ff_probe_sdk_chain_word(1395u)||ctx.esi!=ff_probe_sdk_chain_word(1396u)||
           ctx.edi!=ff_probe_sdk_chain_word(1397u)||ctx.fs_base!=ff_probe_sdk_chain_word(1400u)||ctx.df!=0u||
           ctx.cf!=cf||ctx.zf!=zf||ctx.sf!=sf||ctx.of!=of||ctx.pf!=pf||ctx.xflags!=xflags)
            ff_sdk_raise(ctx,0x0004B230u,"profile-28 logger changed required registers, stack, or caller flags");
        return;
    }
    if(!ff_sdk_context_active(ctx,base)||ctx.df!=0u||(profile!=12u&&profile!=13u&&profile!=14u&&(profile!=15u && profile!=16u && (profile!=17u && (profile!=18u && (profile!=19u && (profile!=20u && (profile!=21u && (profile!=22u && (profile!=23u && profile!=24u && profile!=25u&&profile!=26u&&profile!=27u&&profile!=28u&&profile!=29u&&(profile!=30u && (profile!=31u && (profile!=32u && (profile!=33u && (profile!=34u && (profile!=35u && (profile!=36u && (profile!=37u && (profile!=38u && (profile!=39u && (profile!=40u && (profile!=41u && (profile!=42u && (profile!=43u && (profile!=44u && profile!=45u))))))))))))))))))))))))||
       ff_probe_sdk_chain_word(17u)!=0x00048880u)
        ff_sdk_raise(ctx,0x0004B230u,"C log bridge outside admitted startup profile/context");
    auto*t=static_cast<FFSDKThreadState*>(ctx.host)->boot_thread;
    const uint32_t esp=ctx.esp;
    if((esp&3u)||esp<t->stack_low||esp>t->stack_top||12u>t->stack_top-esp)
        ff_sdk_raise(ctx,esp,"C log bridge frame outside current RUNNING guest stack");
    const uint32_t ret=FFSDKGuestRef<uint32_t>(ctx,base,esp);
    const uint32_t arg0=FFSDKGuestRef<uint32_t>(ctx,base,esp+4u);
    const uint32_t arg1=FFSDKGuestRef<uint32_t>(ctx,base,esp+8u);
    const bool first=ret==0x000499D4u&&arg0==0x0025B55Cu;
    const bool second=profile>=13u&&ret==0x00049A68u&&arg0==0x0025B4F8u;
    const bool third=profile>=15u&&ret==0x00049D9Bu&&arg0==0x0025B628u;
    if((!first&&!second&&!third)||arg1!=0u)
        ff_sdk_raise(ctx,0x0004B230u,"C log bridge callsite or argument frame mismatch");
    uint32_t log_gate=0u;
    if(!third) {
        log_gate=FFSDKGuestRef<uint32_t>(ctx,base,0x002CCCF8u);
        if(log_gate!=0u)ff_sdk_raise(ctx,0x002CCCF8u,"C log bridge only admits the statically zero-log branch");
    }
    const uint32_t cf=ctx.cf,zf=ctx.zf,sf=ctx.sf,of=ctx.of,pf=ctx.pf;
    ff_sdk_scalar s{ctx.eax,ctx.ecx,ctx.edx,ctx.ebx,ctx.esp,ctx.ebp,ctx.esi,ctx.edi,ctx.fs_base,ctx.df};
    if(first)ff_sdk_shared_log_bridge(&s,ret,arg0,arg1,log_gate);
    else if(second)ff_sdk_shared_log_bridge_second(&s,ret,arg0,arg1,log_gate);
    else ff_sdk_shared_log_bridge_third(&s,ret,arg0,arg1);
    ctx.eax=s.eax;ctx.ecx=s.ecx;ctx.edx=s.edx;ctx.ebx=s.ebx;ctx.esp=s.esp;
    ctx.ebp=s.ebp;ctx.esi=s.esi;ctx.edi=s.edi;ctx.fs_base=s.fs_base;ctx.df=s.df;
    if(ctx.cf!=cf||ctx.zf!=zf||ctx.sf!=sf||ctx.of!=of||ctx.pf!=pf)
        ff_sdk_raise(ctx,0x0004B230u,"C log bridge changed SDK caller-local flags");
}
