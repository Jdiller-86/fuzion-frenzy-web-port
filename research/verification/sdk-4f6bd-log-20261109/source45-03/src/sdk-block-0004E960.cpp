#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"
X86_EXTERN_FUNC(sub_0004B380);

X86_FUNC(__imp__sub_0004E960) {
	if (!ff_sdk_context_active(ctx, base) || (ff_probe_sdk_chain_word(0u) != 29u && (ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))))))) ||
	    ff_probe_sdk_chain_word(17u) != 0x0004F5E0u || ff_probe_sdk_chain_word(1898u) != 0u)
		ff_sdk_raise(ctx, 0x0004E960u, "original logger helper outside exact profile-29 root/invocation");
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004E960u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004E960 mov eax, [0x002DA2A0]
	{ uint32_t ea = (uint32_t)(0x2DA2A0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004E965 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004E967 jz 0x0004E97B
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004E97B; }
	// 0004E969 mov ecx, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004E96D lea eax, [esp+0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x8u));
	// 0004E971 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004E972 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004E973 call 0x0004B380
	X86_PUSH32(0x4E978u); sub_0004B380(ctx, base);
	// 0004E978 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0004E97B:
	// 0004E97B ret
	ff_sdk_observe_exit(ctx, base, 0x0004E960u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004E960, __imp__sub_0004E960)