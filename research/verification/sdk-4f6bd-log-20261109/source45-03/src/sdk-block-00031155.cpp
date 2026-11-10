#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_00031155) {
	if (!ff_sdk_context_active(ctx, base) || (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))) ||
	    ff_probe_sdk_chain_word(17u) != 0x0004F5E0u || ff_probe_sdk_chain_word(2892u) != 0u ||
	    ctx.esp != 0x03C10AF4u)
		ff_sdk_raise(ctx, 0x00031155u, "original error helper outside exact profile-33 root/invocation");
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x00031155u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00031155 push [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00031159 call [0x00253370]
	X86_PUSH32(0x3115Fu); x86_kernel(ctx, base, 301);
	ff_sdk_observe_exit(ctx, base, 0x3115Fu);
	// 0003115F mov edx, eax
	{ ctx.edx = (uint32_t)(ctx.eax); }
	// 00031161 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 00031162 call 0x00031127
	X86_PUSH32(0x31167u); sub_00031127(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x31167u);
	// 00031167 mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 00031169 ret 0x04
	ff_sdk_observe_exit(ctx, base, 0x00031155u); ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00031155, __imp__sub_00031155)