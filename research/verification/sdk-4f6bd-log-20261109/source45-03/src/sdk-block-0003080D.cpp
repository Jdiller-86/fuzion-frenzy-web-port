#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0003080D) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0003080Du);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0003080D push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0003080F push [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00030813 call [0x0025339C]
	X86_PUSH32(0x30819u); x86_kernel(ctx, base, 221);
	// 00030819 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003081B jl 0x00030822
	if (((int32_t)(ca & cb)) < 0) { goto loc_00030822; }
	// 0003081D xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003081F inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00030820 jmp 0x0003082A
	goto loc_0003082A;
loc_00030822:
	// 00030822 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00030823 call 0x00031155
	X86_PUSH32(0x30828u); sub_00031155(ctx, base);
	// 00030828 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0003082A:
	// 0003082A ret 0x04
	ff_sdk_observe_exit(ctx, base, 0x0003080Du);
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_0003080D, __imp__sub_0003080D)