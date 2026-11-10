#include "sdk-shared-prototypes.hpp"

X86_FUNC(__imp__sub_000311F9) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x000311F9u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 000311F9 mov eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 000311FD cmp eax, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00031200 jnz 0x00031206
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_00031206; }
	// 00031202 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00031204 jmp 0x00031224
	goto loc_00031224;
loc_00031206:
	// 00031206 mov ecx, 0x2710
	{ ctx.ecx = (uint32_t)(0x2710u); }
	// 0003120B mul ecx
	{ uint64_t p = (uint64_t)ctx.eax * (uint32_t)ctx.ecx; ctx.eax = (uint32_t)p; ctx.edx = (uint32_t)(p >> 32); cf = of = ctx.edx != 0; }
	// 0003120D mov ecx, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00031211 mov [ecx], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00031213 neg eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(0 - a); ctx.eax = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00031215 mov [ecx+0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00031218 adc edx, 0x00
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x0u); uint32_t c0 = cf; uint32_t r = (uint32_t)(a + b + c0); ctx.edx = (uint32_t)(r); cf = ((uint64_t)a + b + c0) >> 32 & 1; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003121B neg edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(0 - a); ctx.edx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003121D mov [ecx], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0003121F mov [ecx+0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00031222 mov eax, ecx
	{ ctx.eax = (uint32_t)(ctx.ecx); }
loc_00031224:
	// 00031224 ret 0x08
	ff_sdk_observe_exit(ctx, base, 0x000311F9u);
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_000311F9, __imp__sub_000311F9)
