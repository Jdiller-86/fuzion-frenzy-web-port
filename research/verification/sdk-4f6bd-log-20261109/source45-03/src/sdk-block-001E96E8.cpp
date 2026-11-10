#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"
X86_EXTERN_FUNC(sub_00031155);
X86_EXTERN_FUNC(sub_001E956F);

X86_FUNC(__imp__sub_001E96E8) {
	if (!ff_sdk_context_active(ctx, base) || (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))))) ||
	    ff_probe_sdk_chain_word(17u) != 0x0004F5E0u || ff_probe_sdk_chain_word(2092u) != 0u ||
	    ctx.esp != 0x03C10C50u)
		ff_sdk_raise(ctx, 0x001E96E8u, "original helper outside exact profile-31 root/entry");
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x001E96E8u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E96E8 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001E96E9 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001E96EB sub esp, 0x150
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x150u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E96F1 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E96F2 xor esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a ^ b); ctx.esi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E96F4 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E96F5 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E96F6 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001E96F8 push 0x148
	{ uint32_t v = (uint32_t)(0x148u); X86_PUSH32(v); }
	// 001E96FD lea eax, [ebp-0x150]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFEB0u));
	// 001E9703 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E9704 lea eax, [ebp-0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF8u));
	// 001E9707 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E9708 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E9709 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E970A push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E970B push [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 001E970E call [0x002534F8]
	X86_PUSH32(0x1E9714u); x86_kernel(ctx, base, 207);
	// 001E9714 cmp eax, esi
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9716 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E9717 jnl 0x001E9723
	if ((int32_t)ca >= (int32_t)cb) { goto loc_001E9723; }
	// 001E9719 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E971A call 0x00031155
	X86_PUSH32(0x1E971Fu); sub_00031155(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x1E971Fu);
	// 001E971F xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9721 jmp 0x001E9735
	goto loc_001E9735;
loc_001E9723:
	// 001E9723 push [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 001E9726 lea eax, [ebp-0x150]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFEB0u));
	// 001E972C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E972D call 0x001E956F
	X86_PUSH32(0x1E9732u); sub_001E956F(ctx, base);
	// 001E9732 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9734 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001E9735:
	// 001E9735 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001E9736 ret 0x08
	ff_sdk_observe_exit(ctx, base, 0x001E96E8u); ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_001E96E8, __imp__sub_001E96E8)