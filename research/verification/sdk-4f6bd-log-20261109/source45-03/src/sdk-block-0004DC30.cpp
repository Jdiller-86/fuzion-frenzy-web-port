#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0004DC30) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004DC30u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004DC30 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004DC31 mov esi, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0004DC35 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004DC37 mov dl, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 0004DC39 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004DC3B test dl, dl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(((uint8_t)ctx.edx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0004DC3D jz 0x0004DC61
	if (((uint8_t)(ca & cb)) == 0) { goto loc_0004DC61; }
	// 0004DC3F nop
loc_0004DC40:
	// 0004DC40 mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 0004DC42 sar ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x18u) & 31; if (c) { uint32_t r = (uint32_t)((int32_t)(int32_t)a >> c); cf = (uint32_t)((int32_t)(int32_t)a >> (c - 1)) & 1; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 0004DC45 xor ecx, edx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004DC47 mov dl, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 0004DC4A and ecx, 0xFF
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFFu); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004DC50 mov ecx, [ecx*4+0x2D0330]
	{ uint32_t ea = (uint32_t)(ctx.ecx * 4u + 0x2D0330u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004DC57 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004DC58 shl eax, 0x08
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x8u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 0004DC5B xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004DC5D test dl, dl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(((uint8_t)ctx.edx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0004DC5F jnz 0x0004DC40
	if (((uint8_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_0004DC40; }
loc_0004DC61:
	// 0004DC61 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004DC62 ret
	ff_sdk_observe_exit(ctx, base, 0x0004DC30u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004DC30, __imp__sub_0004DC30)