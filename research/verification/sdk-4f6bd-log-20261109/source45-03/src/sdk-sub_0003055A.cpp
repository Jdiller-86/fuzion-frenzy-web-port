#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0003055A) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0003055Au);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0003055A mov eax, [0x00253368]
	{ uint32_t ea = (uint32_t)(0x253368u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0003055F mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00030561 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00030563 jz 0x000305B6
	if (((uint32_t)(ca & cb)) == 0) { goto loc_000305B6; }
	// 00030565 mov eax, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00030567 cmp eax, 0x02
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003056A jz 0x0003057F
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0003057F; }
	// 0003056C cmp eax, 0x03
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003056F jz 0x0003057F
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0003057F; }
	// 00030571 mov edx, [0x00010118]
	{ uint32_t ea = (uint32_t)(0x10118u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00030577 mov ecx, [ecx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0003057A cmp ecx, [edx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x8u); uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0003057D jnz 0x000305B6
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_000305B6; }
loc_0003057F:
	// 0003057F mov ecx, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00030583 mov [ecx], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00030585 mov eax, [0x00253368]
	{ uint32_t ea = (uint32_t)(0x253368u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0003058A push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0003058B mov esi, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0003058D push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0003058E mov edi, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00030592 add esi, 0x400
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x400u); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00030598 mov ecx, 0x300
	{ ctx.ecx = (uint32_t)(0x300u); }
	// 0003059D rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 0003059F mov eax, [0x00253368]
	{ uint32_t ea = (uint32_t)(0x253368u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 000305A4 mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 000305A6 and dword ptr [eax], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000305A9 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 000305AA call [0x0025336C]
	X86_PUSH32(0x305B0u); x86_kernel(ctx, base, 171);
	// 000305B0 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 000305B1 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000305B3 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 000305B4 jmp 0x000305BB
	goto loc_000305BB;
loc_000305B6:
	// 000305B6 mov eax, 0x490
	{ ctx.eax = (uint32_t)(0x490u); }
loc_000305BB:
	// 000305BB ret 0x08
	ff_sdk_observe_exit(ctx, base, 0x0003055Au);
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_0003055A, __imp__sub_0003055A)