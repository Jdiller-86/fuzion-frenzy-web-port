#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "cpu/x86_ops.h"
#include "sdk-shared-prototypes.hpp"

extern "C" uint32_t ff_sdk_shared_invoke_target(uint32_t target);
extern "C" void ff_sdk_shared_memory_fault(uint32_t address,uint32_t bytes,uint32_t access,
                                             const ff_sdk_scalar *scalar);
extern "C" void ff_sdk_shared_record_movs(ff_sdk_scalar *,uint32_t width,uint32_t complete);
extern "C" void ff_sdk_record_rep_movs(X86Context&,uint8_t*,uint32_t);

extern "C" uint32_t ff_probe_sdk_tls_getter_armed(void);
extern "C" uint32_t ff_probe_sdk_dosmap_child(uint32_t,const ff_sdk_scalar*);
extern "C" void ff_probe_sdk_dosmap_child_return(const ff_sdk_scalar*);
static FFSDKRuntime runtime;


extern "C" uint32_t ff_probe_sdk_scope43_active(void);
extern "C" uint32_t ff_probe_sdk_scope43_begin(uint32_t,ff_sdk_scalar*);
extern "C" uint32_t ff_probe_sdk_scope43_allocator_begin(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope43_allocator_post(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope43_return_post(uint32_t,ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope43_raw_pre_ret(uint32_t,ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope43_hash_post(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope43_pre_ret(X86Context&,uint8_t*,uint32_t);
X86_EXTERN_FUNC(sub_0004D620);
extern "C" void __imp__sub_0004D620(X86Context&,uint8_t*);
extern "C" void __imp__sub_0004BF10(X86Context&,uint8_t*);

extern "C" void __imp__sub_0004D4F0(X86Context&,uint8_t*);
X86_FUNC(__imp__sub_0004D620) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx,base,0x0004D620u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004D620 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004D624 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004D625 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004D626 call 0x0004DC30
	X86_PUSH32(0x4D62Bu); sub_0004DC30(ctx, base);
	// 0004D62B mov esi, [0x002D02FC]
	{ uint32_t ea = (uint32_t)(0x2D02FCu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0004D631 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D634 cmp esi, 0x2D02D8
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x2D02D8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D63A jz 0x0004D650
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004D650; }
	// 0004D63C lea esp, [esp]
	ctx.esp = (uint32_t)((uint32_t)(ctx.esp));
loc_0004D640:
	// 0004D640 cmp [esi+0x20], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x20u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D643 jz 0x0004D654
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004D654; }
	// 0004D645 mov esi, [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0004D648 cmp esi, 0x2D02D8
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x2D02D8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D64E jnz 0x0004D640
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_0004D640; }
loc_0004D650:
	// 0004D650 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D652 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004D653 ret
	ff_probe_sdk_scope43_pre_ret(ctx,base,0x0004d653u);
	ctx.esp += 4; return;
loc_0004D654:
	// 0004D654 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0004D655 mov edi, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0004D659 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004D65B push 0x25BFEC
	{ uint32_t v = (uint32_t)(0x25BFECu); X86_PUSH32(v); }
	// 0004D660 push 0x2001
	{ uint32_t v = (uint32_t)(0x2001u); X86_PUSH32(v); }
	// 0004D665 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 0004D667 push 0x9D
	{ uint32_t v = (uint32_t)(0x9Du); X86_PUSH32(v); }
	// 0004D66C shl ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x8u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 0004D66F add ecx, 0x20
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x20u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D672 push 0x25BFA8
	{ uint32_t v = (uint32_t)(0x25BFA8u); X86_PUSH32(v); }
	// 0004D677 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004D678 call 0x0004BF10
	X86_PUSH32(0x4D67Du); sub_0004BF10(ctx, base);
	// 0004D67D add esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D680 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D682 jnz 0x0004D687
	if (((uint32_t)(ca & cb)) != 0) { goto loc_0004D687; }
	// 0004D684 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004D685 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004D686 ret
	ff_probe_sdk_scope43_pre_ret(ctx,base,0x0004d686u);
	ctx.esp += 4; return;
loc_0004D687:
	// 0004D687 mov edx, [esi+0x30]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x30u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004D68A mov [eax+0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0004D68D lea ecx, [esi+0x2C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esi + 0x2Cu));
	// 0004D690 mov [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004D692 mov [edx], eax
	{ uint32_t ea = (uint32_t)(ctx.edx); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004D694 mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004D696 mov [ecx+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004D699 mov ecx, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004D69D lea edx, [eax+0x20]
	ctx.edx = (uint32_t)((uint32_t)(ctx.eax + 0x20u));
	// 0004D6A0 mov [eax+0x18], edi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0004D6A3 mov [eax+0x1C], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004D6A6 mov [eax+0x0C], esi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0004D6A9 mov dword ptr [eax+0x08], 0x01
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x1u); }
	// 0004D6B0 mov [eax+0x10], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0004D6B3 mov dword ptr [eax+0x14], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 0004D6BA mov ecx, [esi+0x4C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004D6BD inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D6BE pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004D6BF mov [esi+0x4C], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004D6C2 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004D6C3 ret
	ff_probe_sdk_scope43_pre_ret(ctx,base,0x0004d6c3u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004D620, __imp__sub_0004D620)
extern "C" void __imp__sub_0004DC30(X86Context&,uint8_t*);
extern "C" uint32_t ff_probe_sdk_scope41_begin(uint32_t,ff_sdk_scalar*);
extern "C" uint32_t ff_probe_sdk_scope41_active(void);
extern "C" void ff_probe_sdk_scope41_enter(uint32_t,ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope41_pre_ret(uint32_t,ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope41_post_ret(uint32_t,ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope41_allocator_attempt(ff_sdk_scalar*);


X86_EXTERN_FUNC(sub_00031155);
X86_EXTERN_FUNC(sub_00031127);
X86_EXTERN_FUNC(sub_000310FF);
X86_EXTERN_FUNC(sub_001E4F99);
X86_EXTERN_FUNC(sub_001E2038);
X86_EXTERN_FUNC(__imp__sub_001E2038);
static const FFSDKMapping mappings[] = {
    {0x00031155u, sub_00031155},
    {0x00031127u, sub_00031127},
    {0x000310FFu, sub_000310FF},
    {0x001E4F99u, sub_001E4F99},
    {0x001E2038u, sub_001E2038},
    {0x001e96e8u, sub_001E96E8},
    {0x001e056du, sub_001E056D},
    {0x0004e960u, sub_0004E960},
    {0x001e956fu, sub_001E956F},
    {0x001e95dbu, sub_001E95DB},
    {0x0004f5e0u, sub_0004F5E0},
    {0x0004eb50u, sub_0004EB50},
    {0x0003080du, sub_0003080D},
    {0x0004be40u, sub_0004BE40},
    {0x0004bf10u, sub_0004BF10},
    {0x0004d4f0u, sub_0004D4F0},
    {0x0004d620u, sub_0004D620},
    {0x0004dc30u, sub_0004DC30},
    {0x001e893cu, sub_001E893C},
    {0x0024d37eu, sub_0024D37E},
    {0x00049990u, sub_00049990},
    {0x0024d379u, sub_0024D379},
    {0x0003055au, sub_0003055A},
    {0x000A6FA0u, sub_000A6FA0},
    {0x0004F890u, sub_0004F890},
    {0x00048880u, sub_00048880},
    {0x00049D40u, sub_00049D40},
    {0x00030943u, sub_00030943},
    {0x0003082du, sub_0003082D},
    {0x000311f9u, sub_000311F9},
};

static FFSDKThreadState *state(X86Context& c){return static_cast<FFSDKThreadState*>(c.host);}
static ff_sdk_scalar scalar_from(const X86Context& c) {
    return {c.eax,c.ecx,c.edx,c.ebx,c.esp,c.ebp,c.esi,c.edi,c.fs_base,c.df};
}
static void scalar_to(X86Context& c,const ff_sdk_scalar& s) {
    c.eax=s.eax;c.ecx=s.ecx;c.edx=s.edx;c.ebx=s.ebx;c.esp=s.esp;
    c.ebp=s.ebp;c.esi=s.esi;c.edi=s.edi;c.fs_base=s.fs_base;c.df=s.df;
}
bool ff_sdk_context_active(X86Context& c,uint8_t* base){
    auto*t=state(c);if(!t||t->stopped||!t->runtime||t->runtime->current!=t||t->context!=&c||!t->boot_thread)return false;
    auto*b=t->runtime->boot;if(!b||b->count>FF_BOOT_MAX_THREADS||b->current>=b->count||b->current>=FF_BOOT_MAX_THREADS)return false;
    return t->runtime->base==base&&t->runtime->memory&&t->runtime->memory->ram==base&&
        &b->threads[b->current]==t->boot_thread&&t->boot_thread->id==b->current+1u&&t->boot_thread->state==FF_BOOT_RUNNING;
}
extern "C" uint8_t *ff_sdk_checked_span(X86Context& c,uint8_t* base,uint32_t va,uint32_t bytes,uint32_t access){
    auto*t=state(c);
    if(!ff_sdk_context_active(c,base)||!bytes)
        ff_sdk_raise(c,va,"checked SDK guest-memory context/shape rejected");
    if(va>t->runtime->memory->bytes||bytes>t->runtime->memory->bytes-va||
       ff_vm_check(t->runtime->memory,va,bytes,access)) {
        const ff_sdk_scalar s=scalar_from(c);
        ff_sdk_shared_memory_fault(va,bytes,access,&s);
        ff_sdk_raise(c,va,"C memory-fault stop unexpectedly returned");
    }
    ff_sdk_scalar s=scalar_from(c);
    uint8_t *checked=ff_sdk_shared_memory(va,bytes,access,&s);
    scalar_to(c,s);
    if(!checked||checked!=base+va||base!=t->runtime->memory->ram)
        ff_sdk_raise(c,va,"C guest-memory accounting returned a different RAM span");
    return checked;
}
extern "C" void ff_sdk_raise(X86Context& c,uint32_t va,const char* why){x86_trap(c,va,why);}
bool ff_sdk_bind_boot_thread(FFSDKRuntime& r,ff_boot_thread& b){
    if(!r.memory||!r.boot||!r.base||r.memory->ram!=r.base||!r.mapping||!r.mapping_count||r.mapping_count>512u||
       r.boot->count>FF_BOOT_MAX_THREADS||r.boot->current>=r.boot->count||r.boot->current>=FF_BOOT_MAX_THREADS||
       &r.boot->threads[r.boot->current]!=&b||b.id!=r.boot->current+1u||b.state!=FF_BOOT_RUNNING)return false;
    auto*s=&r.states[b.id-1u];*s={&r,&b,&r.contexts[b.id-1u],0};s->context->host=s;s->context->esp=b.initial_sp;s->context->fs_base=b.fs;r.current=s;return true;
}
X86Context *ff_sdk_current_context(FFSDKRuntime& r){return r.current&&r.current->context&&ff_sdk_context_active(*r.current->context,r.base)?r.current->context:nullptr;}

extern "C" void __imp__sub_0004BF10(X86Context&,uint8_t*);
extern "C" uint32_t ff_probe_sdk_scope42_begin_allocator(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope42_allocator_post(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope42_parent_post(ff_sdk_scalar*);

extern "C" void ff_probe_sdk_scope43_pre_ret(X86Context& c,uint8_t* base,uint32_t site) {
    if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,site,"inactive M43 pre-RET context");
    ff_sdk_scalar s=scalar_from(c);ff_probe_sdk_scope43_raw_pre_ret(site,&s);
}

bool ff_sdk_direct_entry(X86Context& c,uint8_t* base,X86Func* f){
    if((ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u)) && (f==sub_0004D620 || f==sub_0004D4F0)) {
        const uint32_t guest=f==sub_0004D620?0x4d620u:0x4d4f0u;
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,guest,"inactive M43 root context");
        auto*t=state(c);uint32_t matches=0,mappedGuest=0;
        if(!t||!t->runtime||!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)
            ff_sdk_raise(c,guest,"invalid M43 current context/map");
        for(uint32_t i=0;i<t->runtime->mapping_count;i++)
            if(t->runtime->mapping[i].host==f){mappedGuest=t->runtime->mapping[i].guest;++matches;}
        if(matches!=1u||mappedGuest!=guest||ff_vm_check(t->runtime->memory,guest,guest==0x4d620u?164u:1u,FF_VM_EXECUTE))
            ff_sdk_raise(c,guest,"M43 target lacks unique executable mapping");
        ff_sdk_scalar entered=scalar_from(c);
        if(ff_probe_sdk_scope43_begin(guest,&entered)) {
            if(guest==0x4d620u)__imp__sub_0004D620(c,base);else __imp__sub_0004D4F0(c,base);
            if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,guest,"M43 root lost live context");
            ff_sdk_scalar returned=scalar_from(c);ff_probe_sdk_scope43_return_post(guest,&returned);return false;
        }
        if(guest==0x4d620u)ff_sdk_raise(c,guest,"unproved M43 caller");
    }
    if(ff_probe_sdk_scope43_active() && (f==sub_0004DC30 || f==sub_0004BF10)) {
        const uint32_t guest=f==sub_0004DC30?0x4dc30u:0x4bf10u;
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,guest,"inactive M43 child context");
        auto*t=state(c);uint32_t matches=0,mappedGuest=0;
        if(!t||!t->runtime||!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)
            ff_sdk_raise(c,guest,"invalid M43 child context/map");
        for(uint32_t i=0;i<t->runtime->mapping_count;i++)
            if(t->runtime->mapping[i].host==f){mappedGuest=t->runtime->mapping[i].guest;++matches;}
        if(matches!=1u||mappedGuest!=guest||ff_vm_check(t->runtime->memory,guest,1u,FF_VM_EXECUTE))
            ff_sdk_raise(c,guest,"M43 child lacks unique executable mapping");
        ff_sdk_scalar entered=scalar_from(c);
        if(guest==0x4bf10u&&!ff_probe_sdk_scope43_allocator_begin(&entered))
            ff_sdk_raise(c,guest,"unproved M43 allocator tuple");
        if(guest==0x4dc30u)__imp__sub_0004DC30(c,base);else __imp__sub_0004BF10(c,base);
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,guest,"M43 child lost live context");
        ff_sdk_scalar returned=scalar_from(c);
        if(guest==0x4dc30u)ff_probe_sdk_scope43_hash_post(&returned);else ff_probe_sdk_scope43_allocator_post(&returned);
        return false;
    }

    if(!ff_sdk_context_active(c,base)||!f)ff_sdk_raise(c,0,"inactive SDK direct entry");

if(f==sub_0004D4F0 && (ff_probe_sdk_chain_word(0u)==41u || (ff_probe_sdk_chain_word(0u)==42u || (ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u))))) {
    auto*t=state(c);uint32_t guest=0,matches=0;
    if(!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)
        ff_sdk_raise(c,0x4d4f0u,"invalid M41 map");
    for(uint32_t i=0;i<t->runtime->mapping_count;++i)
        if(t->runtime->mapping[i].host==f){guest=t->runtime->mapping[i].guest;++matches;}
    if(matches!=1u||guest!=0x4d4f0u||ff_vm_check(t->runtime->memory,guest,1u,FF_VM_EXECUTE))
        ff_sdk_raise(c,guest,"M41 entry not uniquely mapped executable");
    ff_sdk_scalar entered=scalar_from(c);
    if(ff_probe_sdk_scope41_begin(0x4d4f0u,&entered)) {
        __imp__sub_0004D4F0(c,base);
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,0x4d4f0u,"M41 scoped parent lost active owner");
        ff_sdk_scalar returned=scalar_from(c);
        ff_probe_sdk_scope41_post_ret(0x4d4f0u,&returned);
        ff_probe_sdk_scope42_parent_post(&returned);
        return false;
    }
}
if(f==sub_0004DC30 && ff_probe_sdk_scope41_active()) {
    auto*t=state(c);uint32_t guest=0,matches=0;
    if(!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)
        ff_sdk_raise(c,0x4dc30u,"invalid M41 map");
    for(uint32_t i=0;i<t->runtime->mapping_count;++i)
        if(t->runtime->mapping[i].host==f){guest=t->runtime->mapping[i].guest;++matches;}
    if(matches!=1u||guest!=0x4dc30u||ff_vm_check(t->runtime->memory,guest,1u,FF_VM_EXECUTE))
        ff_sdk_raise(c,guest,"M41 entry not uniquely mapped executable");
    ff_sdk_scalar entered=scalar_from(c);
    if(ff_probe_sdk_scope41_begin(0x4dc30u,&entered)) {
        __imp__sub_0004DC30(c,base);
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,0x4dc30u,"M41 scoped hash child lost active owner");
        ff_sdk_scalar returned=scalar_from(c);
        ff_probe_sdk_scope41_post_ret(0x4dc30u,&returned);
        return false;
    }
}

if(f==sub_0004BF10 && (ff_probe_sdk_chain_word(0u)==42u || (ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u)))) {
    auto*t=state(c); uint32_t guest=0,matches=0;
    if(!t || !t->runtime || !t->runtime->mapping || !t->runtime->mapping_count || t->runtime->mapping_count>512u)
        ff_sdk_raise(c,0x4bf10u,"M42 invalid SDK mapping table");
    for(uint32_t i=0;i<t->runtime->mapping_count;i++)
        if(t->runtime->mapping[i].host==f){guest=t->runtime->mapping[i].guest;++matches;}
    if(matches!=1u || guest!=0x4bf10u || ff_vm_check(t->runtime->memory,guest,1u,FF_VM_EXECUTE))
        ff_sdk_raise(c,guest,"M42 allocator mapping/executable admission failed");
    ff_sdk_scalar entered=scalar_from(c);
    if(ff_probe_sdk_scope42_begin_allocator(&entered)) {
        __imp__sub_0004BF10(c,base); /* same context, one actual raw invocation */
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,0x4bf10u,"M42 allocator lost active owner");
        ff_sdk_scalar returned=scalar_from(c);
        ff_probe_sdk_scope42_allocator_post(&returned);
        if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,0x4bf10u,"M42 post-RET observer changed current owner");
        return false; /* handled exactly once; preserve true raw outputs */
    }
}
if(f==sub_0004BF10 && ff_probe_sdk_scope41_active()) {
    ff_sdk_scalar attempted=scalar_from(c);
    ff_probe_sdk_scope41_allocator_attempt(&attempted);
    /* Continue to the ordinary weak wrapper; shared_enter retains its exact
       denial for this unsupported optional allocation child. */
}

    if(f==sub_001E4F99 || f==sub_001E2038){
        const ff_sdk_scalar current=scalar_from(c);
        const uint32_t target=f==sub_001E4F99?0x001E4F99u:0x001E2038u;
        if((ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u ||
           !ff_probe_sdk_dosmap_child(target,&current))ff_sdk_raise(c,target,"unarmed direct M39 mapper entry");
        if(f==sub_001E2038){
            ff_sdk_scalar entered{};ff_sdk_shared_read(&entered);scalar_to(c,entered);
            __imp__sub_001E2038(c,base);
            if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,target,"PTD accessor context lost");
            const ff_sdk_scalar returned=scalar_from(c);ff_probe_sdk_dosmap_child_return(&returned);
            return false; /* Handled: X86_WEAK_FUNC must skip its ordinary impl call. */
        }
    }

    if(f==sub_0024D379&&ff_probe_sdk_chain_word(0u)<8u)ff_sdk_raise(c,0x0024D379u,"untranslated SDK direct target");
    if(f==sub_00049D40&&ff_probe_sdk_chain_word(0u)<7u)ff_sdk_raise(c,0x00049D40u,"untranslated SDK direct target");
    if(f==sub_001E893C&&ff_probe_sdk_chain_word(0u)<14u)ff_sdk_raise(c,0x001E893Cu,"untranslated SDK direct target");
    if(f==sub_0004D4F0&&ff_probe_sdk_chain_word(0u)<16u)ff_sdk_raise(c,0x0004D4F0u,"untranslated SDK direct target");
    if(f==sub_0004DC30&&ff_probe_sdk_chain_word(0u)<16u)ff_sdk_raise(c,0x0004DC30u,"untranslated SDK direct target");
    if(f==sub_0004BF10&&ff_probe_sdk_chain_word(0u)<17u)ff_sdk_raise(c,0x0004BF10u,"untranslated SDK direct target");
    if(f==sub_00030943&&ff_probe_sdk_chain_word(17u)==0x48880u&&(ff_probe_sdk_chain_word(0u)!=18u && ff_probe_sdk_chain_word(0u)!=19u && ff_probe_sdk_chain_word(0u)!=20u && ff_probe_sdk_chain_word(0u)!=21u && ff_probe_sdk_chain_word(0u)!=22u && (ff_probe_sdk_chain_word(0u)!=23u && ff_probe_sdk_chain_word(0u)!=24u && ff_probe_sdk_chain_word(0u)!=25u&&ff_probe_sdk_chain_word(0u)!=26u&&(ff_probe_sdk_chain_word(0u)!=27u&&(ff_probe_sdk_chain_word(0u)!=28u&&(ff_probe_sdk_chain_word(0u)!=29u&&(ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))))))))))))
        ff_sdk_raise(c,0x00030943u,"new original wait caller remains guarded");
    if(f==sub_0004BE40&&ff_probe_sdk_chain_word(0u)<19u)ff_sdk_raise(c,0x0004BE40u,"untranslated SDK direct target");
    if(f==sub_0003080D&&ff_probe_sdk_chain_word(0u)<20u)ff_sdk_raise(c,0x0003080Du,"untranslated SDK direct target");
    if((f==sub_0004F5E0 || f==sub_0004EB50) &&
       ((ff_probe_sdk_chain_word(0u)!=23u && ff_probe_sdk_chain_word(0u)!=24u && ff_probe_sdk_chain_word(0u)!=25u&&ff_probe_sdk_chain_word(0u)!=26u&&(ff_probe_sdk_chain_word(0u)!=27u&&(ff_probe_sdk_chain_word(0u)!=28u&&(ff_probe_sdk_chain_word(0u)!=29u&&(ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))))))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))
        ff_sdk_raise(c,f==sub_0004F5E0?0x4f5e0u:0x4eb50u,"original startup root outside exact profile/context");
    if(f==sub_001E95DB && ((ff_probe_sdk_chain_word(0u)!=24u && ff_probe_sdk_chain_word(0u)!=25u&&ff_probe_sdk_chain_word(0u)!=26u&&(ff_probe_sdk_chain_word(0u)!=27u&&(ff_probe_sdk_chain_word(0u)!=28u&&(ff_probe_sdk_chain_word(0u)!=29u&&(ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))))))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))
        ff_sdk_raise(c,0x1e95dbu,"directory candidate outside exact root/profile");
    if(f==sub_001E956F && ((ff_probe_sdk_chain_word(0u)!=27u&&(ff_probe_sdk_chain_word(0u)!=28u&&(ff_probe_sdk_chain_word(0u)!=29u&&(ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))
        ff_sdk_raise(c,0x1e956fu,"record helper outside exact root/profile");
    if(f==sub_0004E960 && ((ff_probe_sdk_chain_word(0u)!=29u&&(ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))
        ff_sdk_raise(c,0x4e960u,"conditional helper outside exact profile/root");
    if(f==sub_001E056D && ((ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))
        ff_sdk_raise(c,0x1e056du,"comparison outside exact profile/root");
    if(f==sub_001E96E8 && ((ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))
        ff_sdk_raise(c,0x1e96e8u,"next directory outside exact profile/root");
    if((f==sub_00031155 || f==sub_00031127) && ((ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u))ff_sdk_raise(c,0x31155u,"error pair outside profile/root");
    auto*t=state(c);if(!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)ff_sdk_raise(c,0,"invalid SDK map");
    uint32_t guest=0,matches=0;for(uint32_t i=0;i<t->runtime->mapping_count;i++)if(t->runtime->mapping[i].host==f){guest=t->runtime->mapping[i].guest;++matches;}
    if(matches!=1u||ff_vm_check(t->runtime->memory,guest,1,FF_VM_EXECUTE))ff_sdk_raise(c,guest,"SDK entry not mapped executable");return true;
}
void ff_sdk_dispatch(X86Context& c,uint8_t* base,uint32_t target){
    if(target==0x000310FFu && ((ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u || !ff_probe_sdk_tls_getter_armed()))ff_sdk_raise(c,target,"unarmed 0x310FF nested call");
    if((target==0x001E6AAFu || target==0x000343F3u || target==0x001E564Eu) && (ff_probe_sdk_chain_word(0u)==39u || (ff_probe_sdk_chain_word(0u)==40u || (ff_probe_sdk_chain_word(0u)==41u || (ff_probe_sdk_chain_word(0u)==42u || (ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u)))))) && ff_probe_sdk_chain_word(17u)==0x4f5e0u)ff_sdk_raise(c,target,"unobserved mapper allocation/failure path");
    if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,target,"inactive SDK call context");auto*t=state(c);
    if(!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)ff_sdk_raise(c,target,"invalid SDK call map");
    const FFSDKMapping*m=nullptr;for(uint32_t i=0;i<t->runtime->mapping_count;i++)if(t->runtime->mapping[i].guest==target){if(m||!t->runtime->mapping[i].host)ff_sdk_raise(c,target,"ambiguous SDK call target");m=&t->runtime->mapping[i];}
    if(!m||ff_vm_check(t->runtime->memory,target,1,FF_VM_EXECUTE))ff_sdk_raise(c,target,"SDK target outside executable admission");m->host(c,base);
}

void ff_sdk_observe_enter(X86Context& c,uint8_t* base,uint32_t guest) {
    if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,guest,"SDK trace entry without active current thread");
    ff_sdk_scalar s=scalar_from(c);ff_sdk_shared_enter(guest,&s);scalar_to(c,s);
}
void ff_sdk_observe_exit(X86Context& c,uint8_t* base,uint32_t guest) {
    if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,guest,"SDK trace exit without active current thread");
    ff_sdk_scalar s=scalar_from(c);ff_sdk_shared_exit(guest,&s);scalar_to(c,s);
}

extern "C" uint32_t ff_sdk_shared_invoke(void) {
    return ff_sdk_shared_invoke_target(0x00030943u);
}
extern "C" uint32_t ff_sdk_shared_invoke_target(uint32_t target) {
    if(target!=0x0003055au&&target!=0x00030943u&&target!=0x000A6FA0u&&target!=0x0004F890u&&target!=0x00048880u&&target!=0x0004F5E0u)return 6u;
    auto *memory=ff_sdk_shared_vm();auto *boot=ff_sdk_shared_boot();auto *base=ff_sdk_shared_ram();
    if(!memory||!boot||!base||memory->ram!=base||!boot->count||boot->count>FF_BOOT_MAX_THREADS||
       boot->current>=boot->count||boot->current>=FF_BOOT_MAX_THREADS)return 1u;
    ff_boot_thread *thread=&boot->threads[boot->current];
    if(thread->id!=boot->current+1u||thread->state!=FF_BOOT_RUNNING)return 2u;
    runtime={};runtime.memory=memory;runtime.boot=boot;runtime.mapping=mappings;
    runtime.mapping_count=sizeof(mappings)/sizeof(mappings[0]);runtime.base=base;
    if(!ff_sdk_bind_boot_thread(runtime,*thread))return 3u;
    X86Context *ctx=ff_sdk_current_context(runtime);if(!ctx)return 4u;
    ff_sdk_scalar s{};ff_sdk_shared_read(&s);scalar_to(*ctx,s);
    x86_icall(*ctx,base,target);
    if(!ff_sdk_context_active(*ctx,base))return 5u;
    s=scalar_from(*ctx);ff_sdk_shared_write(&s);
    return 0u;
}

/* Exact C-parent to existing SDK child on the SAME active context; no rebuild,
 * no cloned CPU rollback and no synthetic guest result or flags. */
extern "C" uint32_t ff_probe_sdk_scope44_active(void);
extern "C" uint32_t ff_probe_sdk_scope44_allocator_begin(ff_sdk_scalar*);
extern "C" void ff_probe_sdk_scope44_allocator_post(ff_sdk_scalar*);
extern "C" void ff_sdk_allocator_c44(void) {
    X86Context *ctx=ff_sdk_current_context(runtime);
    if(!ctx||!ff_sdk_context_active(*ctx,runtime.base)||(ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)||
       ff_probe_sdk_chain_word(17u)!=0x4f5e0u||!ff_probe_sdk_scope44_active())
        ff_sdk_shared_stop(0x4bf10u);
    FFSDKThreadState *owner=runtime.current;
    auto*t=state(*ctx);uint32_t matches=0,mappedGuest=0;
    if(!t||!t->runtime||!t->runtime->mapping||!t->runtime->mapping_count||t->runtime->mapping_count>512u)
        ff_sdk_raise(*ctx,0x4bf10u,"invalid M44 allocator mapping");
    for(uint32_t i=0;i<t->runtime->mapping_count;i++)
        if(t->runtime->mapping[i].host==sub_0004BF10){mappedGuest=t->runtime->mapping[i].guest;++matches;}
    if(matches!=1u||mappedGuest!=0x4bf10u||ff_vm_check(t->runtime->memory,0x4bf10u,1u,FF_VM_EXECUTE))
        ff_sdk_raise(*ctx,0x4bf10u,"M44 allocator lacks unique executable mapping");
    ff_sdk_scalar incoming{};ff_sdk_shared_read(&incoming);
    if(!ff_probe_sdk_scope44_allocator_begin(&incoming))ff_sdk_raise(*ctx,0x4bf10u,"unproved M44 C-to-SDK tuple");
    scalar_to(*ctx,incoming); /* Actual C callee input image, including real ESP. */
    __imp__sub_0004BF10(*ctx,runtime.base);
    if(runtime.current!=owner||ff_sdk_current_context(runtime)!=ctx||!ff_sdk_context_active(*ctx,runtime.base))
        ff_sdk_raise(*ctx,0x4bf10u,"M44 allocator changed active context owner");
    ff_sdk_scalar returned=scalar_from(*ctx);
    ff_probe_sdk_scope44_allocator_post(&returned);
    ff_sdk_shared_write(&returned); /* Actual raw output consumed by original C caller. */
}

/* Nested exact C-to-SDK provider. Keep the active runtime and outer host context. */
extern "C" uint32_t ff_probe_sdk_tls_getter_begin(void);
extern "C" uint32_t ff_probe_sdk_tls_getter_fs_byte(void);
extern "C" uint32_t ff_probe_sdk_tls_getter_eflags(void);
extern "C" uint32_t ff_probe_sdk_tls_getter_armed(void);
extern "C" void ff_probe_sdk_tls_getter_complete(uint32_t,uint32_t,uint32_t,uint32_t);
extern "C" void ff_sdk_shared_write_eflags(uint32_t);
extern "C" void ff_sdk_tls_getter_c(void) {
    X86Context *outer=ff_sdk_current_context(runtime);
    if(!outer){ff_sdk_shared_stop(0x000310FFu);return;}
    if(!ff_sdk_context_active(*outer,runtime.base) ||
       (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u ||
       !ff_probe_sdk_tls_getter_begin())ff_sdk_raise(*outer,0x000310FFu,"TLS getter outside exact nested M38 call");
    FFSDKThreadState *outer_state=state(*outer);
    X86Context saved=*outer,nested=saved;
    FFSDKThreadState nested_state=*outer_state;
    nested_state.context=&nested;nested.host=&nested_state;
    ff_sdk_scalar incoming{};ff_sdk_shared_read(&incoming);scalar_to(nested,incoming);
    const uint32_t entry_flags=ff_probe_sdk_tls_getter_eflags();
    nested.cf=entry_flags&1u;nested.pf=(entry_flags>>2)&1u;
    nested.zf=(entry_flags>>6)&1u;nested.sf=(entry_flags>>7)&1u;nested.of=(entry_flags>>11)&1u;
    nested.df=incoming.df;nested.xflags=entry_flags&0x00240000u;
    auto *previous=runtime.current;runtime.current=&nested_state;
    ff_sdk_dispatch(nested,runtime.base,0x000310FFu);
    if(!ff_sdk_context_active(nested,runtime.base))ff_sdk_raise(nested,0x000310FFu,"nested getter context lost");
    runtime.current=previous;

    /* The generated body keeps lazy flags local; publish the exact byte CMP
       flags at the adapter boundary while retaining unrelated EFLAGS bits. */
    const uint32_t a=ff_probe_sdk_tls_getter_fs_byte()&0xffu,b=2u,r=(a-b)&0xffu;
    const uint32_t cf=a<b,pf=(uint32_t)(__builtin_parity(r)^1u),af=((a^b^r)>>4)&1u;
    const uint32_t zf=r==0u,sf=(r>>7)&1u,of=((a^b)&(a^r)&0x80u)?1u:0u;
    const uint32_t mask=0x8d5u;
    const uint32_t flags=(entry_flags&~mask)|cf|(pf<<2)|(af<<4)|(zf<<6)|(sf<<7)|(of<<11);
    nested.cf=cf;nested.pf=pf;nested.zf=zf;nested.sf=sf;nested.of=of;
    nested.xflags=flags&0x00240000u;
    const ff_sdk_scalar returned=scalar_from(nested);
    ff_sdk_shared_write(&returned);ff_sdk_shared_write_eflags(flags);
    /* The suspended parent is not the callee's C-visible CPU image. Leave
       its complete context untouched; the enclosing bridge synchronizes on
       its ordinary return path and retains its existing return guard. */
    ff_probe_sdk_tls_getter_complete(returned.eax,returned.ecx,returned.esp,flags);
}

/* M39 private nested DOS mapper invocation. Outer context is left untouched. */
extern "C" uint32_t ff_probe_sdk_dosmap_begin(void);
extern "C" uint32_t ff_probe_sdk_dosmap_child(uint32_t,const ff_sdk_scalar*);
extern "C" void ff_probe_sdk_dosmap_complete(const ff_sdk_scalar*);
extern "C" uint32_t ff_sdk_shared_read_eflags(void);
extern "C" void ff_sdk_shared_write_eflags(uint32_t);
extern "C" void ff_sdk_dosmaperr_c(void) {
    X86Context *outer=ff_sdk_current_context(runtime);
    if(!outer || !ff_sdk_context_active(*outer,runtime.base) ||
       (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))) || ff_probe_sdk_chain_word(17u)!=0x4f5e0u ||
       !ff_probe_sdk_dosmap_begin())ff_sdk_shared_stop(0x001E4F99u);
    FFSDKThreadState *previous=runtime.current;
    X86Context nested=*outer;FFSDKThreadState nested_state=*previous;
    nested_state.context=&nested;nested.host=&nested_state;
    ff_sdk_scalar incoming{};ff_sdk_shared_read(&incoming);scalar_to(nested,incoming);
    const uint32_t entry_flags=ff_sdk_shared_read_eflags();
    nested.cf=entry_flags&1u;nested.pf=(entry_flags>>2)&1u;
    nested.zf=(entry_flags>>6)&1u;nested.sf=(entry_flags>>7)&1u;nested.of=(entry_flags>>11)&1u;
    nested.df=incoming.df;nested.xflags=entry_flags&0x00240000u;
    runtime.current=&nested_state;
    ff_sdk_dispatch(nested,runtime.base,0x001E4F99u);
    if(!ff_sdk_context_active(nested,runtime.base))ff_sdk_raise(nested,0x001E4F99u,"nested DOS mapper context lost");
    runtime.current=previous;
    const ff_sdk_scalar returned=scalar_from(nested);
    ff_sdk_shared_write(&returned);
    /* Caller POP/MOV/LEAVE/RET paths do not consume these lazy flags; the
       original ADD ESP,14 defines flags before the next original read. */
    ff_sdk_shared_write_eflags(entry_flags);
    ff_probe_sdk_dosmap_complete(&returned);
}

/* The C fixture owns all service state and retains its exact binding checks. */
void ff_sdk_kernel_dispatch(X86Context& c,uint8_t* base,uint32_t ordinal) {
    if(ordinal==95u && (ff_probe_sdk_chain_word(0u)==39u || (ff_probe_sdk_chain_word(0u)==40u || (ff_probe_sdk_chain_word(0u)==41u || (ff_probe_sdk_chain_word(0u)==42u || (ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u)))))) && ff_probe_sdk_chain_word(17u)==0x4f5e0u)ff_sdk_raise(c,ordinal,"unbound high-IRQL accessor edge");
    if(!ff_sdk_context_active(c,base))ff_sdk_raise(c,ordinal,"kernel call without active current thread");
    ff_sdk_scalar s=scalar_from(c);ff_sdk_shared_kernel(ordinal,&s);scalar_to(c,s);
}
void ff_sdk_rep_movs(X86Context& c,uint8_t* base,uint32_t width) {
    if((ff_probe_sdk_chain_word(0u)==27u||(ff_probe_sdk_chain_word(0u)==28u||(ff_probe_sdk_chain_word(0u)==29u||(ff_probe_sdk_chain_word(0u)==30u || (ff_probe_sdk_chain_word(0u)==31u || (ff_probe_sdk_chain_word(0u)==32u || (ff_probe_sdk_chain_word(0u)==33u || (ff_probe_sdk_chain_word(0u)==34u || (ff_probe_sdk_chain_word(0u)==35u || (ff_probe_sdk_chain_word(0u)==36u || (ff_probe_sdk_chain_word(0u)==37u || (ff_probe_sdk_chain_word(0u)==38u || (ff_probe_sdk_chain_word(0u)==39u || (ff_probe_sdk_chain_word(0u)==40u || (ff_probe_sdk_chain_word(0u)==41u || (ff_probe_sdk_chain_word(0u)==42u || (ff_probe_sdk_chain_word(0u)==43u || (ff_probe_sdk_chain_word(0u)==44u || ff_probe_sdk_chain_word(0u)==45u))))))))))))))))))&&ff_probe_sdk_chain_word(17u)==0x4f5e0u){
        ff_sdk_record_rep_movs(c,base,width);
        return;
    }
    constexpr uint32_t admitted_count=0x300u;
    constexpr uint32_t admitted_width=4u;
    constexpr uint32_t span_bytes=0xC00u;
    if(!ff_sdk_context_active(c,base)||width!=admitted_width||c.df!=0u||c.ecx!=admitted_count)
        ff_sdk_raise(c,0x0003059du,"REP MOVS shape outside the bounded 0x3055A profile");
    auto*t=state(c);const uint32_t source=c.esi,destination=c.edi;
    if(source>UINT32_MAX-span_bytes||source>t->runtime->memory->bytes||
       span_bytes>t->runtime->memory->bytes-source||
       ff_vm_check(t->runtime->memory,source,span_bytes,FF_VM_READ)) {
        const ff_sdk_scalar s=scalar_from(c);
        ff_sdk_shared_memory_fault(source,span_bytes,FF_VM_READ,&s);
        ff_sdk_raise(c,0x0003059du,"REP MOVS source memory-fault stop unexpectedly returned");
    }
    if(destination>UINT32_MAX-span_bytes||destination>t->runtime->memory->bytes||
       span_bytes>t->runtime->memory->bytes-destination||
       ff_vm_check(t->runtime->memory,destination,span_bytes,FF_VM_WRITE)) {
        const ff_sdk_scalar s=scalar_from(c);
        ff_sdk_shared_memory_fault(destination,span_bytes,FF_VM_WRITE,&s);
        ff_sdk_raise(c,0x0003059du,"REP MOVS destination memory-fault stop unexpectedly returned");
    }
    /* Forward DWORD-by-DWORD iteration preserves REP overlap behavior. Each
       scalar read/write uses the checked guest reference and shared C meter. */
    for(uint32_t i=0;i<admitted_count;++i){
        const uint32_t value=FFSDKGuestRef<uint32_t>(c,base,c.esi);
        FFSDKGuestRef<uint32_t>(c,base,c.edi)=value;
        c.esi+=admitted_width;c.edi+=admitted_width;--c.ecx;
    }
}
void x86_icall(X86Context& c,uint8_t* base,uint32_t target){ff_sdk_dispatch(c,base,target);}
void x86_kernel(X86Context& c,uint8_t* base,uint32_t ordinal){ff_sdk_kernel_dispatch(c,base,ordinal);}
[[noreturn]] void x86_trap(X86Context& c,uint32_t va,const char* what){
    (void)what;auto*t=state(c);
    /* Preserve the last valid SDK scalar state in the authoritative C runner
       before stop() traps; never try to synchronize an invalid frame/context. */
    if(t&&t->runtime&&t->runtime->current==t&&t->context==&c&&t->boot_thread&&t->runtime->boot&&
       t->runtime->boot->count<=FF_BOOT_MAX_THREADS&&t->runtime->boot->current<t->runtime->boot->count&&
       t->runtime->boot->current<FF_BOOT_MAX_THREADS &&
       &t->runtime->boot->threads[t->runtime->boot->current]==t->boot_thread&&
       t->boot_thread->state==FF_BOOT_RUNNING&&t->boot_thread->id==t->runtime->boot->current+1u&&
       c.fs_base==t->boot_thread->fs&&c.df<=1u&&c.esp>=t->boot_thread->stack_low&&c.esp<=t->boot_thread->stack_top){
        const ff_sdk_scalar s=scalar_from(c);ff_sdk_shared_write(&s);
    }
    if(t)t->stopped=1u;
    ff_sdk_shared_stop(va);__builtin_trap();
}
void x86_priv(X86Context& c,uint32_t va,const char* what){ff_sdk_raise(c,va,what);}
void x86_resume(X86Context& c,uint8_t*,const X86Resume&){ff_sdk_raise(c,0,"SDK continuation unsupported");}
void x86_debug_service(X86Context& c,uint8_t*,uint32_t va){ff_sdk_raise(c,va,"SDK debug service unsupported");}
uint32_t x86_trace(X86Context& c,uint32_t va,uint32_t){ff_sdk_raise(c,va,"SDK tracing unsupported");return 0;}
uint64_t x86_rdtsc(){__builtin_trap();}
void x86_cpuid(X86Context& c){ff_sdk_raise(c,0,"SDK CPUID unsupported");}
