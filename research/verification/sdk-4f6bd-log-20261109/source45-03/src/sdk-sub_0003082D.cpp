#include "sdk-shared-prototypes.hpp"

X86_FUNC(__imp__sub_0003082D) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0003082Du);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0003082D push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 0003082E mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 00030830 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 00030831 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 00030832 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00030833 push [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00030836 lea eax, [ebp-0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF8u));
	// 00030839 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0003083A call 0x000311F9
	X86_PUSH32(0x3083Fu); sub_000311F9(ctx, base);
	// 0003083F mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
loc_00030841:
	// 00030841 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00030842 push [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00030845 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 00030847 push [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 0003084A call [0x002533A0]
	X86_PUSH32(0x30850u); x86_kernel(ctx, base, 234);
	// 00030850 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00030852 jl 0x00030863
	if (((int32_t)(ca & cb)) < 0) { goto loc_00030863; }
	// 00030854 cmp dword ptr [ebp+0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00030858 jz 0x0003086C
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0003086C; }
	// 0003085A cmp eax, 0x101
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x101u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003085F jz 0x00030841
	if ((uint32_t)ca == (uint32_t)cb) { X86_BARRIER(); goto loc_00030841; }
	// 00030861 jmp 0x0003086C
	goto loc_0003086C;
loc_00030863:
	// 00030863 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00030864 call 0x00031155
	X86_PUSH32(0x30869u); sub_00031155(ctx, base);
	// 00030869 or eax, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0003086C:
	// 0003086C pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0003086D leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 0003086E ret 0x0C
	ff_sdk_observe_exit(ctx, base, 0x0003082Du);
	ctx.esp += 16; return;
}
X86_WEAK_FUNC(sub_0003082D, __imp__sub_0003082D)
