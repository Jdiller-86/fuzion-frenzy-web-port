#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"

X86_FUNC(sub_0024CA33) {
    if (!ff_sdk_context_active(ctx, base))
        ff_sdk_raise(ctx, 0x0024CA33u, "inactive SDK XInitDevices context");
    if (ff_probe_sdk_chain_word(0u) < 9u)
        ff_sdk_raise(ctx, 0x0024CA33u, "untranslated SDK direct target");

    ff_sdk_scalar state{ctx.eax, ctx.ecx, ctx.edx, ctx.ebx,
                        ctx.esp, ctx.ebp, ctx.esi, ctx.edi,
                        ctx.fs_base, ctx.df};
    ff_sdk_shared_xinit(&state);

    ctx.eax = state.eax;
    ctx.ecx = state.ecx;
    ctx.edx = state.edx;
    ctx.ebx = state.ebx;
    ctx.esp = state.esp;
    ctx.ebp = state.ebp;
    ctx.esi = state.esi;
    ctx.edi = state.edi;
    ctx.fs_base = state.fs_base;
    ctx.df = state.df;
}