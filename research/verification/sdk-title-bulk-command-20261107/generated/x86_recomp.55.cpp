#include "x86_recomp_shared.h"

X86_FUNC(__imp__sub_001EB3E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB3E0 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB3E1 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EB3E3 call 0x001EAE00
	X86_PUSH32(0x1EB3E8u); sub_001EAE00(ctx, base);
	// 001EB3E8 test byte ptr [esp+0x08], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB3ED jz 0x001EB3F8
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB3F8; }
	// 001EB3EF push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB3F0 call 0x0004C110
	X86_PUSH32(0x1EB3F5u); sub_0004C110(ctx, base);
	// 001EB3F5 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EB3F8:
	// 001EB3F8 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EB3FA pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB3FB ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EB3E0, __imp__sub_001EB3E0)

X86_FUNC(__imp__sub_001EB400) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB400 mov eax, ecx
	{ ctx.eax = (uint32_t)(ctx.ecx); }
	// 001EB402 mov dword ptr [eax], 0x27D2B8
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(0x27D2B8u); }
	// 001EB408 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB400, __imp__sub_001EB400)

X86_FUNC(__imp__sub_001EB410) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB410 mov dword ptr [ecx], 0x27D2B8
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(0x27D2B8u); }
	// 001EB416 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB410, __imp__sub_001EB410)

X86_FUNC(__imp__sub_001EB420) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB420 add ecx, 0x04
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB423 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB424 call 0x0003BF10
	X86_PUSH32(0x1EB429u); sub_0003BF10(ctx, base);
	// 001EB429 pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001EB42A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB420, __imp__sub_001EB420)

X86_FUNC(__imp__sub_001EB430) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB430 test byte ptr [esp+0x04], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB435 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB436 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EB438 mov dword ptr [esi], 0x27D2B8
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(0x27D2B8u); }
	// 001EB43E jz 0x001EB449
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB449; }
	// 001EB440 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB441 call 0x0004C110
	X86_PUSH32(0x1EB446u); sub_0004C110(ctx, base);
	// 001EB446 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EB449:
	// 001EB449 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EB44B pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB44C ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EB430, __imp__sub_001EB430)

X86_FUNC(__imp__sub_001EB450) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB450 mov dword ptr [ecx], 0x27D2C4
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(0x27D2C4u); }
	// 001EB456 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB450, __imp__sub_001EB450)

X86_FUNC(__imp__sub_001EB460) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB460 sub esp, 0x100
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x100u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB466 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EB467 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001EB469 fld [ebx+0xEC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xECu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB46F mov eax, [ebx+0xE0]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB475 fsub [ebx+0xE8]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xE8u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d - s)); }
	// 001EB47B mov edx, [ebx+0x130]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x130u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB481 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB482 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB483 fdivr [ebx+0xEC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xECu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (s / d)); }
	// 001EB489 mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EB48E mov esi, 0x2DA2C8
	{ ctx.esi = (uint32_t)(0x2DA2C8u); }
	// 001EB493 lea edi, [esp+0x0C]
	ctx.edi = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 001EB497 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EB499 mov ecx, [ebx+0xE4]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xE4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB49F push 0x3F800000
	{ uint32_t v = (uint32_t)(0x3F800000u); X86_PUSH32(v); }
	// 001EB4A4 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB4A5 mov [esp+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB4A9 lea eax, [esp+0x94]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x94u));
	// 001EB4B0 mov [esp+0x28], ecx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EB4B4 mov dword ptr [esp+0x40], 0x3F800000
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x40u); X86_MEM32(ea) = (uint32_t)(0x3F800000u); }
	// 001EB4BC fstp [esp+0x3C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x3Cu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB4C0 fld [ebx+0x134]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x134u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB4C6 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB4C8 fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB4CB push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EB4CC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB4CD call 0x000502A0
	X86_PUSH32(0x1EB4D2u); sub_000502A0(ctx, base);
	// 001EB4D2 mov ecx, [ebx+0x134]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x134u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB4D8 mov edx, [ebx+0x130]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x130u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB4DE push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EB4E0 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB4E1 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EB4E2 lea eax, [esp+0xE8]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0xE8u));
	// 001EB4E9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB4EA call 0x00050250
	X86_PUSH32(0x1EB4EFu); sub_00050250(ctx, base);
	// 001EB4EF lea edx, [esp+0xAC]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0xACu));
	// 001EB4F6 lea ecx, [esp+0x6C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 001EB4FA or ecx, edx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB4FC lea eax, [esp+0x2C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x2Cu));
	// 001EB500 add esp, 0x20
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x20u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB503 or ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB505 test cl, 0x0F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0xFu); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB508 lea ecx, [esp+0x0C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 001EB50C push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB50D lea edx, [esp+0x90]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x90u));
	// 001EB514 lea eax, [esp+0x50]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x50u));
	// 001EB518 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EB519 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB51A jz 0x001EB523
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB523; }
	// 001EB51C call 0x0004FF30
	X86_PUSH32(0x1EB521u); sub_0004FF30(ctx, base);
	// 001EB521 jmp 0x001EB528
	goto loc_001EB528;
loc_001EB523:
	// 001EB523 call 0x00050150
	X86_PUSH32(0x1EB528u); sub_00050150(ctx, base);
loc_001EB528:
	// 001EB528 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001EB52A lea edx, [esp+0x58]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x58u));
	// 001EB52E or ecx, edx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB530 lea eax, [esp+0xD8]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0xD8u));
	// 001EB537 or ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB539 add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB53C add ebx, 0xF0
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(0xF0u); uint32_t r = (uint32_t)(a + b); ctx.ebx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB542 test cl, 0x0F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0xFu); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB545 jz 0x001EB567
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB567; }
	// 001EB547 lea ecx, [esp+0x4C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x4Cu));
	// 001EB54B push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB54C lea edx, [esp+0xD0]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0xD0u));
	// 001EB553 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EB554 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EB555 call 0x0004FF30
	X86_PUSH32(0x1EB55Au); sub_0004FF30(ctx, base);
	// 001EB55A add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB55D pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EB55E pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB55F pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EB560 add esp, 0x100
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x100u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB566 ret
	ctx.esp += 4; return;
loc_001EB567:
	// 001EB567 lea eax, [esp+0x4C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x4Cu));
	// 001EB56B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB56C lea ecx, [esp+0xD0]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0xD0u));
	// 001EB573 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB574 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EB575 call 0x00050150
	X86_PUSH32(0x1EB57Au); sub_00050150(ctx, base);
	// 001EB57A add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB57D pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EB57E pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB57F pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EB580 add esp, 0x100
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x100u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB586 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB460, __imp__sub_001EB460)

X86_FUNC(__imp__sub_001EB590) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB590 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB594 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EB595 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001EB597 mov dword ptr [ebx], 0x27D2C4
	{ uint32_t ea = (uint32_t)(ctx.ebx); X86_MEM32(ea) = (uint32_t)(0x27D2C4u); }
	// 001EB59D push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EB59E xor ebp, ebp
	{ uint32_t a = (uint32_t)(ctx.ebp); uint32_t b = (uint32_t)(ctx.ebp); uint32_t r = (uint32_t)(a ^ b); ctx.ebp = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB5A0 cmp eax, ebp
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.ebp); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB5A2 mov ecx, 0x3F800000
	{ ctx.ecx = (uint32_t)(0x3F800000u); }
	// 001EB5A7 mov [ebx+0x08], ebp
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 001EB5AA mov [ebx+0x0C], ebp
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 001EB5AD mov [ebx+0x10], ebp
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 001EB5B0 mov [ebx+0x14], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EB5B3 mov [ebx+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB5B6 jnz 0x001EB5CA
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_001EB5CA; }
	// 001EB5B8 mov [ebx+0x18], ebp
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 001EB5BB mov [ebx+0x1C], ebp
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 001EB5BE mov dword ptr [ebx+0x20], 0xC2C80000
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x20u); X86_MEM32(ea) = (uint32_t)(0xC2C80000u); }
	// 001EB5C5 mov [ebx+0x24], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EB5C8 jmp 0x001EB5E7
	goto loc_001EB5E7;
loc_001EB5CA:
	// 001EB5CA push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB5CB push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB5CC lea edi, [ebx+0x28]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ebx + 0x28u));
	// 001EB5CF mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EB5D4 mov esi, 0x2855F0
	{ ctx.esi = (uint32_t)(0x2855F0u); }
	// 001EB5D9 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EB5DB pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EB5DC mov dword ptr [ebx+0xA8], 0x42C80000
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xA8u); X86_MEM32(ea) = (uint32_t)(0x42C80000u); }
	// 001EB5E6 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
loc_001EB5E7:
	// 001EB5E7 fld [0x00275880]
	{ uint32_t ea = (uint32_t)(0x275880u); x87_push(ctx, X86_MEMF64(ea)); }
	// 001EB5ED mov eax, 0x3FC90FDA
	{ ctx.eax = (uint32_t)(0x3FC90FDAu); }
	// 001EB5F2 fptan
	{ X87_ST(0) = std::tan(X87_ST(0)); x87_push(ctx, 1.0); ctx.fsw &= ~0x400; }
	// 001EB5F4 mov [ebx+0xD8], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xD8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB5FA mov [ebx+0xDC], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xDCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB600 mov eax, 0x3F000000
	{ ctx.eax = (uint32_t)(0x3F000000u); }
	// 001EB605 mov [ebx+0x134], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x134u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB60B mov [ebx+0x130], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x130u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB611 mov eax, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB615 and eax, 0xFFFF0001
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFF0001u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB61A mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001EB61C mov dword ptr [ebx+0xE8], 0x40A00000
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xE8u); X86_MEM32(ea) = (uint32_t)(0x40A00000u); }
	// 001EB626 mov dword ptr [ebx+0xEC], 0x459C4000
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xECu); X86_MEM32(ea) = (uint32_t)(0x459C4000u); }
	// 001EB630 mov [ebx+0xAC], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xACu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB636 fstp st0
	{ X87_ST(0) = X87_ST(0); x87_pop(ctx); }
	// 001EB638 fdivr [0x00253850]
	{ uint32_t ea = (uint32_t)(0x253850u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (s / d)); }
	// 001EB63E fst [ebx+0xE0]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xE0u); X86_MEMF32(ea) = (float)X87_ST(0); }
	// 001EB644 fstp [ebx+0xE4]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xE4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB64A call 0x001EB460
	X86_PUSH32(0x1EB64Fu); sub_001EB460(ctx, base);
	// 001EB64F mov [ebx+0x13C], ebp
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x13Cu); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 001EB655 pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EB656 mov eax, ebx
	{ ctx.eax = (uint32_t)(ctx.ebx); }
	// 001EB658 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EB659 ret 0x08
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_001EB590, __imp__sub_001EB590)

X86_FUNC(__imp__sub_001EB660) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB660 test byte ptr [esp+0x04], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB665 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB666 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EB668 mov dword ptr [esi], 0x27D2C4
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(0x27D2C4u); }
	// 001EB66E jz 0x001EB679
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB679; }
	// 001EB670 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB671 call 0x0004C110
	X86_PUSH32(0x1EB676u); sub_0004C110(ctx, base);
	// 001EB676 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EB679:
	// 001EB679 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EB67B pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB67C ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EB660, __imp__sub_001EB660)

X86_FUNC(__imp__sub_001EB680) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB680 sub esp, 0x28
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x28u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB683 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB684 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EB686 cmp dword ptr [esi+0x04], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x1u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB68A jnz 0x001EB6E9
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_001EB6E9; }
	// 001EB68C fld [esi+0xA8]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xA8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB692 lea eax, [esp+0x0C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 001EB696 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB697 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB699 lea ecx, [esi+0x68]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esi + 0x68u));
	// 001EB69C push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB69D lea edx, [esp+0x24]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x24u));
	// 001EB6A1 fstp [esp+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB6A5 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EB6A6 mov dword ptr [esp+0x18], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EB6AE mov dword ptr [esp+0x1C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EB6B6 mov dword ptr [esp+0x24], 0x3F800000
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x24u); X86_MEM32(ea) = (uint32_t)(0x3F800000u); }
	// 001EB6BE call 0x000505C0
	X86_PUSH32(0x1EB6C3u); sub_000505C0(ctx, base);
	// 001EB6C3 fld [esp+0x28]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x28u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB6C7 fadd [esi+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB6CA add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB6CD fstp [esi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB6D0 fld [esp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB6D4 fadd [esi+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB6D7 fstp [esi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB6DA fld [esp+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x24u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB6DE fadd [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB6E1 fstp [esi+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x20u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB6E4 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB6E5 add esp, 0x28
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x28u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB6E8 ret
	ctx.esp += 4; return;
loc_001EB6E9:
	// 001EB6E9 fld [esi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB6EC fsub [esi+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d - s)); }
	// 001EB6EF fld [esi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB6F2 fsub [esi+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d - s)); }
	// 001EB6F5 fstp [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB6F9 fld [esi+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x20u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB6FC fsub [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d - s)); }
	// 001EB6FF fld st1
	{ double v = X87_ST(1); x87_push(ctx, v); }
	// 001EB701 fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EB707 fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EB709 test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB70C jnp 0x001EB716
	if (!pf) { goto loc_001EB716; }
	// 001EB70E fld st0
	{ double v = X87_ST(0); x87_push(ctx, v); }
	// 001EB710 fld st2
	{ double v = X87_ST(2); x87_push(ctx, v); }
	// 001EB712 fpatan
	{ double v = std::atan2(X87_ST(1), X87_ST(0)); x87_pop(ctx); X87_ST(0) = v; }
	// 001EB714 jmp 0x001EB731
	goto loc_001EB731;
loc_001EB716:
	// 001EB716 fcom [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); }
	// 001EB71C fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EB71E test ah, 0x01
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB721 jnz 0x001EB72B
	if (((uint8_t)(ca & cb)) != 0) { goto loc_001EB72B; }
	// 001EB723 fld [0x00253898]
	{ uint32_t ea = (uint32_t)(0x253898u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB729 jmp 0x001EB731
	goto loc_001EB731;
loc_001EB72B:
	// 001EB72B fld [0x00259078]
	{ uint32_t ea = (uint32_t)(0x259078u); x87_push(ctx, (double)X86_MEMF32(ea)); }
loc_001EB731:
	// 001EB731 fadd [0x00253898]
	{ uint32_t ea = (uint32_t)(0x253898u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB737 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB738 fstp [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB73C fld st0
	{ double v = X87_ST(0); x87_push(ctx, v); }
	// 001EB73E fmul st0, st1
	{ double s = X87_ST(1); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EB740 fld st2
	{ double v = X87_ST(2); x87_push(ctx, v); }
	// 001EB742 fmul st0, st3
	{ double s = X87_ST(3); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EB744 faddp st1, st0
	{ double s = X87_ST(0); double d = X87_ST(1); X87_ST(1) = x87_prec(ctx, (d + s)); x87_pop(ctx); }
	// 001EB746 fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB749 fstp st0
	{ X87_ST(0) = X87_ST(0); x87_pop(ctx); }
	// 001EB74B fstp st0
	{ X87_ST(0) = X87_ST(0); x87_pop(ctx); }
	// 001EB74D call 0x00050F80
	X86_PUSH32(0x1EB752u); sub_00050F80(ctx, base);
	// 001EB752 fcom [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); }
	// 001EB758 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB75B fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EB75D test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB760 jnp 0x001EB76C
	if (!pf) { goto loc_001EB76C; }
	// 001EB762 fdivr [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (s / d)); }
	// 001EB766 fld1
	{ x87_push(ctx, 1.0); }
	// 001EB768 fpatan
	{ double v = std::atan2(X87_ST(1), X87_ST(0)); x87_pop(ctx); X87_ST(0) = v; }
	// 001EB76A jmp 0x001EB78D
	goto loc_001EB78D;
loc_001EB76C:
	// 001EB76C fstp st0
	{ X87_ST(0) = X87_ST(0); x87_pop(ctx); }
	// 001EB76E fld [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB772 fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EB778 fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EB77A test ah, 0x01
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB77D jnz 0x001EB787
	if (((uint8_t)(ca & cb)) != 0) { goto loc_001EB787; }
	// 001EB77F fld [0x00253898]
	{ uint32_t ea = (uint32_t)(0x253898u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB785 jmp 0x001EB78D
	goto loc_001EB78D;
loc_001EB787:
	// 001EB787 fld [0x00259078]
	{ uint32_t ea = (uint32_t)(0x259078u); x87_push(ctx, (double)X86_MEMF32(ea)); }
loc_001EB78D:
	// 001EB78D mov eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB791 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB793 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB794 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB795 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EB796 lea edi, [esi+0x28]
	ctx.edi = (uint32_t)((uint32_t)(ctx.esi + 0x28u));
	// 001EB799 fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB79C push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB79D call 0x00050840
	X86_PUSH32(0x1EB7A2u); sub_00050840(ctx, base);
	// 001EB7A2 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB7A3 add esi, 0x68
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x68u); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB7A6 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB7A7 call 0x00050650
	X86_PUSH32(0x1EB7ACu); sub_00050650(ctx, base);
	// 001EB7AC add esp, 0x14
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x14u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB7AF pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EB7B0 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB7B1 add esp, 0x28
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x28u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB7B4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB680, __imp__sub_001EB680)

X86_FUNC(__imp__sub_001EB7C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB7C0 mov edx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7C4 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB7C6 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7CA mov [ecx+0xB0], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EB7D0 jz 0x001EB7F9
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EB7F9; }
	// 001EB7D2 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB7D3 mov edi, [edx]
	{ uint32_t ea = (uint32_t)(ctx.edx); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7D5 mov [ecx+0xB4], edi
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB4u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 001EB7DB mov edi, [edx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x4u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7DE mov [ecx+0xB8], edi
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB8u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 001EB7E4 mov edi, [edx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7E7 mov [ecx+0xBC], edi
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xBCu); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 001EB7ED mov edx, [edx+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0xCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7F0 mov [ecx+0xC0], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xC0u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EB7F6 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EB7F7 jmp 0x001EB82F
	goto loc_001EB82F;
loc_001EB7F9:
	// 001EB7F9 mov edx, [eax+0x130]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x130u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB7FF fld [edx+0x6C]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x6Cu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB802 fadd [edx+0x68]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x68u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB805 mov dword ptr [ecx+0xB4], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB4u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EB80F mov dword ptr [ecx+0xBC], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xBCu); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EB819 mov dword ptr [ecx+0xC0], 0x3F800000
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xC0u); X86_MEM32(ea) = (uint32_t)(0x3F800000u); }
	// 001EB823 fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EB829 fstp [ecx+0xB8]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
loc_001EB82F:
	// 001EB82F mov edx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB833 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB835 jz 0x001EB841
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EB841; }
	// 001EB837 or dword ptr [ecx+0xAC], 0x02
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xACu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a | b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB83E ret 0x0C
	ctx.esp += 16; return;
loc_001EB841:
	// 001EB841 mov edx, [ecx+0xAC]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xACu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB847 and edx, 0xFFFFFFFD
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0xFFFFFFFDu); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB84A mov [ecx+0xAC], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xACu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EB850 fld [eax+0xC0]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xC0u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB856 fadd [ecx+0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB4u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB85C fstp [ecx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB85F fld [ecx+0xB8]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xB8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB865 fadd [eax+0xC4]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xC4u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB86B fstp [ecx+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xCu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB86E fld [ecx+0xBC]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xBCu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB874 fadd [eax+0xC8]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xC8u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB87A fstp [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB87D call 0x001EB680
	X86_PUSH32(0x1EB882u); sub_001EB680(ctx, base);
	// 001EB882 ret 0x0C
	ctx.esp += 16; return;
}
X86_WEAK_FUNC(sub_001EB7C0, __imp__sub_001EB7C0)

X86_FUNC(__imp__sub_001EB890) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB890 fld [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB894 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB895 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB897 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB898 sub esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB89B fstp [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB89F mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EB8A1 fld [esp+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB8A5 mov edx, [esi+0xAC]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xACu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB8AB fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB8AD and edx, 0xFFFFFFE3
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0xFFFFFFE3u); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB8B0 fstp [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB8B4 lea edi, [esi+0x28]
	ctx.edi = (uint32_t)((uint32_t)(ctx.esi + 0x28u));
	// 001EB8B7 fld [esp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB8BB mov [esi+0xAC], edx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xACu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EB8C1 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB8C3 fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB8C6 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB8C7 call 0x00050D20
	X86_PUSH32(0x1EB8CCu); sub_00050D20(ctx, base);
	// 001EB8CC lea eax, [esi+0x68]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esi + 0x68u));
	// 001EB8CF push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EB8D0 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB8D1 call 0x00050650
	X86_PUSH32(0x1EB8D6u); sub_00050650(ctx, base);
	// 001EB8D6 fld [esp+0x30]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x30u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB8DA fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EB8E0 add esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB8E3 fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EB8E5 test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB8E8 jnp 0x001EB8F4
	if (!pf) { goto loc_001EB8F4; }
	// 001EB8EA mov ecx, [esp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB8EE mov [esi+0xA8], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xA8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
loc_001EB8F4:
	// 001EB8F4 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EB8F6 call 0x001EB680
	X86_PUSH32(0x1EB8FBu); sub_001EB680(ctx, base);
	// 001EB8FB pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EB8FC pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB8FD ret 0x10
	ctx.esp += 20; return;
}
X86_WEAK_FUNC(sub_001EB890, __imp__sub_001EB890)

X86_FUNC(__imp__sub_001EB900) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EB900 sub esp, 0x40
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB903 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EB904 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001EB906 test byte ptr [ebx+0xAC], 0x02
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xACu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB90D jz 0x001EB978
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB978; }
	// 001EB90F mov eax, [ebx+0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xB0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB915 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB917 jz 0x001EB94F
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EB94F; }
	// 001EB919 fld [eax+0xC0]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xC0u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB91F fadd [ebx+0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xB4u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB925 fstp [ebx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB928 fld [eax+0xC4]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xC4u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB92E fadd [ebx+0xB8]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xB8u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB934 fstp [ebx+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB937 fld [eax+0xC8]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xC8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB93D fadd [ebx+0xBC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xBCu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB943 fstp [ebx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x10u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB946 mov dword ptr [ebx+0x14], 0x3F800000
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x14u); X86_MEM32(ea) = (uint32_t)(0x3F800000u); }
	// 001EB94D jmp 0x001EB978
	goto loc_001EB978;
loc_001EB94F:
	// 001EB94F mov eax, [ebx+0xC4]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xC4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB955 fld [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB957 fadd [ebx+0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xB4u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB95D fstp [ebx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB960 fld [eax+0x04]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB963 fadd [ebx+0xB8]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xB8u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB969 fstp [ebx+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB96C fld [eax+0x08]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB96F fadd [ebx+0xBC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xBCu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d + s)); }
	// 001EB975 fstp [ebx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x10u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
loc_001EB978:
	// 001EB978 mov eax, [ebx+0xAC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB97E test al, 0x20
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x20u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB980 jz 0x001EB9AB
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB9AB; }
	// 001EB982 mov eax, [ebx+0xC8]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xC8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB988 mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB98A mov [ebx+0x18], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EB98D mov edx, [eax+0x04]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB990 mov [ebx+0x1C], edx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EB993 mov ecx, [eax+0x08]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB996 mov [ebx+0x20], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EB999 mov edx, [eax+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB99C mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001EB99E mov [ebx+0x24], edx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EB9A1 call 0x001EB680
	X86_PUSH32(0x1EB9A6u); sub_001EB680(ctx, base);
	// 001EB9A6 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EB9A7 add esp, 0x40
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB9AA ret
	ctx.esp += 4; return;
loc_001EB9AB:
	// 001EB9AB test al, 0x04
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EB9AD push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB9AE jz 0x001EB9FF
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EB9FF; }
	// 001EB9B0 mov eax, [ebx+0xD4]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xD4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EB9B6 fld [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB9B8 mov ecx, [ebx+0xD0]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xD0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB9BE mov edx, [ebx+0xCC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EB9C4 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB9C6 sub esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB9C9 fstp [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB9CD lea esi, [ebx+0x28]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebx + 0x28u));
	// 001EB9D0 fld [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB9D2 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB9D4 fstp [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB9D8 fld [edx]
	{ uint32_t ea = (uint32_t)(ctx.edx); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EB9DA fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EB9DC fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EB9DF push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB9E0 call 0x00050D20
	X86_PUSH32(0x1EB9E5u); sub_00050D20(ctx, base);
	// 001EB9E5 lea eax, [ebx+0x68]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebx + 0x68u));
	// 001EB9E8 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EB9E9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EB9EA call 0x00050650
	X86_PUSH32(0x1EB9EFu); sub_00050650(ctx, base);
	// 001EB9EF add esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB9F2 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001EB9F4 call 0x001EB680
	X86_PUSH32(0x1EB9F9u); sub_001EB680(ctx, base);
	// 001EB9F9 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EB9FA pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EB9FB add esp, 0x40
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EB9FE ret
	ctx.esp += 4; return;
loc_001EB9FF:
	// 001EB9FF test al, 0x08
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x8u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBA01 jz 0x001EBA2D
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EBA2D; }
	// 001EBA03 mov ecx, [ebx+0xCC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBA09 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBA0A lea esi, [ebx+0x68]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebx + 0x68u));
	// 001EBA0D push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBA0E call 0x000515C0
	X86_PUSH32(0x1EBA13u); sub_000515C0(ctx, base);
	// 001EBA13 lea edx, [ebx+0x28]
	ctx.edx = (uint32_t)((uint32_t)(ctx.ebx + 0x28u));
	// 001EBA16 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBA17 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EBA18 call 0x00050650
	X86_PUSH32(0x1EBA1Du); sub_00050650(ctx, base);
	// 001EBA1D add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBA20 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001EBA22 call 0x001EB680
	X86_PUSH32(0x1EBA27u); sub_001EB680(ctx, base);
	// 001EBA27 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EBA28 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EBA29 add esp, 0x40
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBA2C ret
	ctx.esp += 4; return;
loc_001EBA2D:
	// 001EBA2D test al, 0x10
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x10u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBA2F jz 0x001EBA63
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EBA63; }
	// 001EBA31 mov eax, [ebx+0xCC]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBA37 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EBA38 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 001EBA3A lea edi, [ebx+0x68]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ebx + 0x68u));
	// 001EBA3D mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EBA42 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EBA44 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 001EBA46 mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EBA4B lea edi, [esp+0x0C]
	ctx.edi = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 001EBA4F lea eax, [esp+0x0C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 001EBA53 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EBA55 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBA56 lea ecx, [ebx+0x28]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebx + 0x28u));
	// 001EBA59 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBA5A call 0x00050650
	X86_PUSH32(0x1EBA5Fu); sub_00050650(ctx, base);
	// 001EBA5F add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBA62 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
loc_001EBA63:
	// 001EBA63 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001EBA65 call 0x001EB680
	X86_PUSH32(0x1EBA6Au); sub_001EB680(ctx, base);
	// 001EBA6A pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EBA6B pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EBA6C add esp, 0x40
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBA6F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EB900, __imp__sub_001EB900)

X86_FUNC(__imp__sub_001EBA70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EBA70 fld [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBA74 fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EBA7A fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EBA7C test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBA7F jnp 0x001EBAA5
	if (!pf) { goto loc_001EBAA5; }
	// 001EBA81 fld [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBA85 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBA89 fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBA8F mov [ecx+0xD8], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xD8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBA95 fptan
	{ X87_ST(0) = std::tan(X87_ST(0)); x87_push(ctx, 1.0); ctx.fsw &= ~0x400; }
	// 001EBA97 fstp st0
	{ X87_ST(0) = X87_ST(0); x87_pop(ctx); }
	// 001EBA99 fdivr [0x00253850]
	{ uint32_t ea = (uint32_t)(0x253850u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (s / d)); }
	// 001EBA9F fstp [ecx+0xE0]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xE0u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
loc_001EBAA5:
	// 001EBAA5 fld [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBAA9 fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EBAAF fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EBAB1 test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBAB4 jnp 0x001EBADA
	if (!pf) { goto loc_001EBADA; }
	// 001EBAB6 fld [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBABA mov edx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBABE fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBAC4 mov [ecx+0xDC], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xDCu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EBACA fptan
	{ X87_ST(0) = std::tan(X87_ST(0)); x87_push(ctx, 1.0); ctx.fsw &= ~0x400; }
	// 001EBACC fstp st0
	{ X87_ST(0) = X87_ST(0); x87_pop(ctx); }
	// 001EBACE fdivr [0x00253850]
	{ uint32_t ea = (uint32_t)(0x253850u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (s / d)); }
	// 001EBAD4 fstp [ecx+0xE4]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xE4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
loc_001EBADA:
	// 001EBADA fld [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBADE fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EBAE4 fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EBAE6 test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBAE9 jnp 0x001EBAF5
	if (!pf) { goto loc_001EBAF5; }
	// 001EBAEB mov eax, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBAEF mov [ecx+0xE8], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xE8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EBAF5:
	// 001EBAF5 fld [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBAF9 fcomp [0x002536E0]
	{ uint32_t ea = (uint32_t)(0x2536E0u); x87_compare(ctx, X87_ST(0), (double)X86_MEMF32(ea)); x87_pop(ctx); }
	// 001EBAFF fnstsw ax
	{ ctx.eax = (ctx.eax & 0xFFFF0000u) | x87_status(ctx); }
	// 001EBB01 test ah, 0x44
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(0x44u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBB04 jnp 0x001EBB10
	if (!pf) { goto loc_001EBB10; }
	// 001EBB06 mov edx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBB0A mov [ecx+0xEC], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0xECu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_001EBB10:
	// 001EBB10 call 0x001EB460
	X86_PUSH32(0x1EBB15u); sub_001EB460(ctx, base);
	// 001EBB15 ret 0x10
	ctx.esp += 20; return;
}
X86_WEAK_FUNC(sub_001EBA70, __imp__sub_001EBA70)

X86_FUNC(__imp__sub_001EBB20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EBB20 sub esp, 0x11C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x11Cu); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBB26 mov eax, [esp+0x120]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x120u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBB2D push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBB2E push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EBB2F mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EBB31 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBB32 lea ecx, [esp+0xE8]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0xE8u));
	// 001EBB39 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBB3A call 0x000502F0
	X86_PUSH32(0x1EBB3Fu); sub_000502F0(ctx, base);
	// 001EBB3F lea edx, [esp+0xEC]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0xECu));
	// 001EBB46 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EBB47 call 0x00043930
	X86_PUSH32(0x1EBB4Cu); sub_00043930(ctx, base);
	// 001EBB4C add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBB4F push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBB51 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBB53 call 0x001F5F90
	X86_PUSH32(0x1EBB58u); sub_001F5F90(ctx, base);
	// 001EBB58 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBB5A mov ecx, 0x40358
	{ ctx.ecx = (uint32_t)(0x40358u); }
	// 001EBB5F call 0x001F6650
	X86_PUSH32(0x1EBB64u); sub_001F6650(ctx, base);
	// 001EBB64 mov edx, 0x207
	{ ctx.edx = (uint32_t)(0x207u); }
	// 001EBB69 mov ecx, 0x40364
	{ ctx.ecx = (uint32_t)(0x40364u); }
	// 001EBB6E mov dword ptr [0x00203A5C], 0x00
	{ uint32_t ea = (uint32_t)(0x203A5Cu); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBB78 call 0x001F6650
	X86_PUSH32(0x1EBB7Du); sub_001F6650(ctx, base);
	// 001EBB7D mov edx, 0x1E01
	{ ctx.edx = (uint32_t)(0x1E01u); }
	// 001EBB82 mov ecx, 0x40378
	{ ctx.ecx = (uint32_t)(0x40378u); }
	// 001EBB87 mov dword ptr [0x00203A68], 0x207
	{ uint32_t ea = (uint32_t)(0x203A68u); X86_MEM32(ea) = (uint32_t)(0x207u); }
	// 001EBB91 call 0x001F6650
	X86_PUSH32(0x1EBB96u); sub_001F6650(ctx, base);
	// 001EBB96 mov edx, 0x80
	{ ctx.edx = (uint32_t)(0x80u); }
	// 001EBB9B mov ecx, 0x40360
	{ ctx.ecx = (uint32_t)(0x40360u); }
	// 001EBBA0 mov dword ptr [0x00203A64], 0x1E01
	{ uint32_t ea = (uint32_t)(0x203A64u); X86_MEM32(ea) = (uint32_t)(0x1E01u); }
	// 001EBBAA call 0x001F6650
	X86_PUSH32(0x1EBBAFu); sub_001F6650(ctx, base);
	// 001EBBAF mov edx, 0x80
	{ ctx.edx = (uint32_t)(0x80u); }
	// 001EBBB4 mov ecx, 0x40368
	{ ctx.ecx = (uint32_t)(0x40368u); }
	// 001EBBB9 mov dword ptr [0x00203A74], 0x80
	{ uint32_t ea = (uint32_t)(0x203A74u); X86_MEM32(ea) = (uint32_t)(0x80u); }
	// 001EBBC3 call 0x001F6650
	X86_PUSH32(0x1EBBC8u); sub_001F6650(ctx, base);
	// 001EBBC8 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EBBCA mov dword ptr [0x00203A6C], 0x80
	{ uint32_t ea = (uint32_t)(0x203A6Cu); X86_MEM32(ea) = (uint32_t)(0x80u); }
	// 001EBBD4 call 0x001F7370
	X86_PUSH32(0x1EBBD9u); sub_001F7370(ctx, base);
	// 001EBBD9 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBBDB mov ecx, 0x4035C
	{ ctx.ecx = (uint32_t)(0x4035Cu); }
	// 001EBBE0 call 0x001F6650
	X86_PUSH32(0x1EBBE5u); sub_001F6650(ctx, base);
	// 001EBBE5 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBBE7 mov ecx, 0x40300
	{ ctx.ecx = (uint32_t)(0x40300u); }
	// 001EBBEC mov dword ptr [0x00203A50], 0x00
	{ uint32_t ea = (uint32_t)(0x203A50u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBBF6 call 0x001F6650
	X86_PUSH32(0x1EBBFBu); sub_001F6650(ctx, base);
	// 001EBBFB push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBBFD push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBBFF push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EBC01 mov dword ptr [0x00203A40], 0x00
	{ uint32_t ea = (uint32_t)(0x203A40u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBC0B call 0x00046CB0
	X86_PUSH32(0x1EBC10u); sub_00046CB0(ctx, base);
	// 001EBC10 fld [esi+0xD8]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xD8u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBC16 fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBC1C mov ecx, [0x002BDA3C]
	{ uint32_t ea = (uint32_t)(0x2BDA3Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBC22 mov eax, [0x002BCC94]
	{ uint32_t ea = (uint32_t)(0x2BCC94u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBC27 mov edi, [0x002BC88C]
	{ uint32_t ea = (uint32_t)(0x2BC88Cu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EBC2D fst [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); X86_MEMF32(ea) = (float)X87_ST(0); }
	// 001EBC31 or eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBC33 fsin
	{ X87_ST(0) = std::sin(X87_ST(0)); ctx.fsw &= ~0x400; }
	// 001EBC35 or eax, edi
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBC37 mov dword ptr [esp+0x18], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBC3F mov dword ptr [esp+0x1C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBC47 mov dword ptr [esp+0x20], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBC4F fmul [esi+0xEC]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xECu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBC55 fld [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBC59 fcos
	{ X87_ST(0) = std::cos(X87_ST(0)); ctx.fsw &= ~0x400; }
	// 001EBC5B fmul [esi+0xEC]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xECu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBC61 fld [esi+0xDC]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xDCu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBC67 fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBC6D fsin
	{ X87_ST(0) = std::sin(X87_ST(0)); ctx.fsw &= ~0x400; }
	// 001EBC6F fmul [esi+0xEC]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xECu); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBC75 or eax, [0x002BD214]
	{ uint32_t ea = (uint32_t)(0x2BD214u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBC7B mov [esp+0x30], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x30u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBC7F fstp [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBC83 mov [esp+0x54], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x54u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBC87 mov ecx, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBC8B fld st1
	{ double v = X87_ST(1); x87_push(ctx, v); }
	// 001EBC8D fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EBC8F mov [esp+0x40], ecx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x40u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBC93 fst [esp+0x3C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x3Cu); X86_MEMF32(ea) = (float)X87_ST(0); }
	// 001EBC97 fld st1
	{ double v = X87_ST(1); x87_push(ctx, v); }
	// 001EBC99 fstp [esp+0x44]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x44u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBC9D mov [esp+0x78], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x78u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBCA1 fld st2
	{ double v = X87_ST(2); x87_push(ctx, v); }
	// 001EBCA3 mov [esp+0x9C], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x9Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBCAA fstp [esp+0x60]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x60u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBCAE mov [esp+0xC0], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xC0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBCB5 fld st1
	{ double v = X87_ST(1); x87_push(ctx, v); }
	// 001EBCB7 mov edx, ecx
	{ ctx.edx = (uint32_t)(ctx.ecx); }
	// 001EBCB9 fstp [esp+0x68]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x68u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBCBD push 0x06
	{ uint32_t v = (uint32_t)(0x6u); X86_PUSH32(v); }
	// 001EBCBF fxch st2
	{ double t = X87_ST(0); X87_ST(0) = X87_ST(2); X87_ST(2) = t; }
	// 001EBCC1 lea eax, [esp+0x1C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x1Cu));
	// 001EBCC5 fstp [esp+0x88]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x88u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBCCC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBCCD fld [esp+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EBCD1 mov [esp+0x6C], edx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x6Cu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EBCD5 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EBCD7 mov ecx, 0x09
	{ ctx.ecx = (uint32_t)(0x9u); }
	// 001EBCDC fst [esp+0x90]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x90u); X86_MEMF32(ea) = (float)X87_ST(0); }
	// 001EBCE3 lea esi, [esp+0x44]
	ctx.esi = (uint32_t)((uint32_t)(ctx.esp + 0x44u));
	// 001EBCE7 fld st1
	{ double v = X87_ST(1); x87_push(ctx, v); }
	// 001EBCE9 lea edi, [esp+0xD4]
	ctx.edi = (uint32_t)((uint32_t)(ctx.esp + 0xD4u));
	// 001EBCF0 fstp [esp+0x94]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x94u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBCF7 push 0x05
	{ uint32_t v = (uint32_t)(0x5u); X86_PUSH32(v); }
	// 001EBCF9 fxch st2
	{ double t = X87_ST(0); X87_ST(0) = X87_ST(2); X87_ST(2) = t; }
	// 001EBCFB fstp [esp+0xB4]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xB4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBD02 fxch st1
	{ double t = X87_ST(0); X87_ST(0) = X87_ST(1); X87_ST(1) = t; }
	// 001EBD04 fstp [esp+0xB8]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xB8u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBD0B fstp [esp+0xBC]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xBCu); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBD12 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EBD14 call 0x0003F2D0
	X86_PUSH32(0x1EBD19u); sub_0003F2D0(ctx, base);
	// 001EBD19 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EBD1B call 0x00046D00
	X86_PUSH32(0x1EBD20u); sub_00046D00(ctx, base);
	// 001EBD20 add esp, 0x1C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x1Cu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBD23 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBD25 call 0x001F7370
	X86_PUSH32(0x1EBD2Au); sub_001F7370(ctx, base);
	// 001EBD2A mov esi, 0x01
	{ ctx.esi = (uint32_t)(0x1u); }
	// 001EBD2F mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 001EBD31 mov ecx, 0x4035C
	{ ctx.ecx = (uint32_t)(0x4035Cu); }
	// 001EBD36 call 0x001F6650
	X86_PUSH32(0x1EBD3Bu); sub_001F6650(ctx, base);
	// 001EBD3B mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 001EBD3D mov ecx, 0x40300
	{ ctx.ecx = (uint32_t)(0x40300u); }
	// 001EBD42 mov [0x00203A50], esi
	{ uint32_t ea = (uint32_t)(0x203A50u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 001EBD48 call 0x001F6650
	X86_PUSH32(0x1EBD4Du); sub_001F6650(ctx, base);
	// 001EBD4D mov edx, 0x1010101
	{ ctx.edx = (uint32_t)(0x1010101u); }
	// 001EBD52 mov ecx, 0x40358
	{ ctx.ecx = (uint32_t)(0x40358u); }
	// 001EBD57 mov [0x00203A40], esi
	{ uint32_t ea = (uint32_t)(0x203A40u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 001EBD5D call 0x001F6650
	X86_PUSH32(0x1EBD62u); sub_001F6650(ctx, base);
	// 001EBD62 mov edx, 0x202
	{ ctx.edx = (uint32_t)(0x202u); }
	// 001EBD67 mov ecx, 0x40364
	{ ctx.ecx = (uint32_t)(0x40364u); }
	// 001EBD6C mov dword ptr [0x00203A5C], 0x1010101
	{ uint32_t ea = (uint32_t)(0x203A5Cu); X86_MEM32(ea) = (uint32_t)(0x1010101u); }
	// 001EBD76 call 0x001F6650
	X86_PUSH32(0x1EBD7Bu); sub_001F6650(ctx, base);
	// 001EBD7B push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBD7C mov dword ptr [0x00203A68], 0x202
	{ uint32_t ea = (uint32_t)(0x203A68u); X86_MEM32(ea) = (uint32_t)(0x202u); }
	// 001EBD86 call 0x001F7370
	X86_PUSH32(0x1EBD8Bu); sub_001F7370(ctx, base);
	// 001EBD8B mov edx, 0x80
	{ ctx.edx = (uint32_t)(0x80u); }
	// 001EBD90 mov ecx, 0x4036C
	{ ctx.ecx = (uint32_t)(0x4036Cu); }
	// 001EBD95 call 0x001F6650
	X86_PUSH32(0x1EBD9Au); sub_001F6650(ctx, base);
	// 001EBD9A pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EBD9B mov dword ptr [0x00203A70], 0x80
	{ uint32_t ea = (uint32_t)(0x203A70u); X86_MEM32(ea) = (uint32_t)(0x80u); }
	// 001EBDA5 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EBDA6 add esp, 0x11C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x11Cu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBDAC ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EBB20, __imp__sub_001EBB20)

X86_FUNC(__imp__sub_001EBDB0) {
	const uint32_t x86_frame = ctx.esp;
	try {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EBDB0 push 0xFFFFFFFF
	{ uint32_t v = (uint32_t)(0xFFFFFFFFu); X86_PUSH32(v); }
	// 001EBDB2 push 0x1F29D8
	{ uint32_t v = (uint32_t)(0x1F29D8u); X86_PUSH32(v); }
	// 001EBDB7 mov eax, fs:[0x00000000]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBDBD push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBDBE mov fs:[0x00000000], esp
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0u); X86_MEM32(ea) = (uint32_t)(ctx.esp); }
	// 001EBDC5 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBDC6 mov eax, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBDCA push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBDCB push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EBDCC mov edi, [esp+0x28]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x28u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EBDD0 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EBDD1 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EBDD3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBDD4 mov [esp+0x10], esi
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 001EBDD8 call 0x001EB590
	X86_PUSH32(0x1EBDDDu); sub_001EB590(ctx, base);
	// 001EBDDD mov eax, [esp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBDE1 add eax, 0x1F
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x1Fu); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBDE4 and eax, 0xFFFFFFE0
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFFFFE0u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBDE7 mov [esp+0x20], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBDEB fild [esp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); x87_push(ctx, (double)(int32_t)X86_MEM32(ea)); }
	// 001EBDEF and edi, 0x10000
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x10000u); uint32_t r = (uint32_t)(a & b); ctx.edi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBDF5 or edi, 0x2000
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x2000u); uint32_t r = (uint32_t)(a | b); ctx.edi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBDFB mov ecx, 0x40
	{ ctx.ecx = (uint32_t)(0x40u); }
	// 001EBE00 fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBE06 shr edi, 0x0D
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t c = (uint32_t)(0xDu) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edi = (uint32_t)(r); } }
	// 001EBE09 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EBE0A push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBE0C fstp [esi+0x130]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x130u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBE12 mov [esi+0x14C], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBE18 fild [esp+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x2Cu); x87_push(ctx, (double)(int32_t)X86_MEM32(ea)); }
	// 001EBE1C mov [esi+0x148], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x148u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBE22 mov [esi+0x144], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x144u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBE28 mov ecx, [esp+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x2Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBE2C fmul [0x002536D0]
	{ uint32_t ea = (uint32_t)(0x2536D0u); double s = (double)X86_MEMF32(ea); double d = X87_ST(0); X87_ST(0) = x87_prec(ctx, (d * s)); }
	// 001EBE32 push 0x10
	{ uint32_t v = (uint32_t)(0x10u); X86_PUSH32(v); }
	// 001EBE34 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBE35 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBE36 fstp [esi+0x134]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x134u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EBE3C mov dword ptr [esp+0x28], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x28u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBE44 mov dword ptr [esi], 0x27D2D0
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(0x27D2D0u); }
	// 001EBE4A call 0x00044E90
	X86_PUSH32(0x1EBE4Fu); sub_00044E90(ctx, base);
	// 001EBE4F add esp, 0x14
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x14u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBE52 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EBE54 mov [esi+0x140], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x140u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBE5A call 0x001EB460
	X86_PUSH32(0x1EBE5Fu); sub_001EB460(ctx, base);
	// 001EBE5F mov ecx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBE63 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EBE64 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EBE66 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EBE67 mov fs:[0x00000000], ecx
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBE6E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBE71 ret 0x10
	ctx.esp += 20; return;
	} catch (const X86Resume& r) {
		if (r.esp >= x86_frame) throw;
		x86_resume(ctx, base, r);
	}
}
X86_WEAK_FUNC(sub_001EBDB0, __imp__sub_001EBDB0)

X86_FUNC(__imp__sub_001EBE80) {
	const uint32_t x86_frame = ctx.esp;
	try {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EBE80 push 0xFFFFFFFF
	{ uint32_t v = (uint32_t)(0xFFFFFFFFu); X86_PUSH32(v); }
	// 001EBE82 push 0x1F29D8
	{ uint32_t v = (uint32_t)(0x1F29D8u); X86_PUSH32(v); }
	// 001EBE87 mov eax, fs:[0x00000000]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBE8D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBE8E mov fs:[0x00000000], esp
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0u); X86_MEM32(ea) = (uint32_t)(ctx.esp); }
	// 001EBE95 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBE96 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBE97 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EBE99 mov [esp+0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 001EBE9D mov dword ptr [esi], 0x27D2D0
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(0x27D2D0u); }
	// 001EBEA3 mov eax, [esi+0x140]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x140u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBEA9 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBEAB mov dword ptr [esp+0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 001EBEB3 jz 0x001EBEBE
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EBEBE; }
	// 001EBEB5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBEB6 call 0x000458A0
	X86_PUSH32(0x1EBEBBu); sub_000458A0(ctx, base);
	// 001EBEBB add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EBEBE:
	// 001EBEBE mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EBEC0 mov dword ptr [esp+0x10], 0xFFFFFFFF
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); X86_MEM32(ea) = (uint32_t)(0xFFFFFFFFu); }
	// 001EBEC8 call 0x001EB450
	X86_PUSH32(0x1EBECDu); sub_001EB450(ctx, base);
	// 001EBECD mov ecx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBED1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EBED2 mov fs:[0x00000000], ecx
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBED9 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBEDC ret
	ctx.esp += 4; return;
	} catch (const X86Resume& r) {
		if (r.esp >= x86_frame) throw;
		x86_resume(ctx, base, r);
	}
}
X86_WEAK_FUNC(sub_001EBE80, __imp__sub_001EBE80)

X86_FUNC(__imp__sub_001EBEE0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EBEE0 sub esp, 0x78
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x78u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBEE3 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EBEE4 mov ebx, [esp+0x80]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x80u); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBEEB test ebx, ebx
	{ uint32_t a = (uint32_t)(ctx.ebx), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBEED push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EBEEE push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EBEEF push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EBEF0 mov ebp, ecx
	{ ctx.ebp = (uint32_t)(ctx.ecx); }
	// 001EBEF2 jnz 0x001EBEFA
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001EBEFA; }
	// 001EBEF4 mov ebx, [ebp+0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xB0u); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
loc_001EBEFA:
	// 001EBEFA test byte ptr [esp+0x94], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x94u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EBF02 jz 0x001EBF28
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EBF28; }
	// 001EBF04 mov eax, [ebp+0x140]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x140u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF0A push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EBF0C push 0x3F800000
	{ uint32_t v = (uint32_t)(0x3F800000u); X86_PUSH32(v); }
	// 001EBF11 push 0xFF
	{ uint32_t v = (uint32_t)(0xFFu); X86_PUSH32(v); }
	// 001EBF16 push 0xFF
	{ uint32_t v = (uint32_t)(0xFFu); X86_PUSH32(v); }
	// 001EBF1B push 0xFF
	{ uint32_t v = (uint32_t)(0xFFu); X86_PUSH32(v); }
	// 001EBF20 push 0xFF
	{ uint32_t v = (uint32_t)(0xFFu); X86_PUSH32(v); }
	// 001EBF25 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBF26 jmp 0x001EBF3E
	goto loc_001EBF3E;
loc_001EBF28:
	// 001EBF28 mov ecx, [ebp+0x140]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x140u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF2E push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBF30 push 0x3F800000
	{ uint32_t v = (uint32_t)(0x3F800000u); X86_PUSH32(v); }
	// 001EBF35 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBF37 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBF39 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBF3B push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBF3D push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
loc_001EBF3E:
	// 001EBF3E call 0x00045330
	X86_PUSH32(0x1EBF43u); sub_00045330(ctx, base);
	// 001EBF43 add esp, 0x1C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x1Cu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBF46 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBF48 call 0x0003BF10
	X86_PUSH32(0x1EBF4Du); sub_0003BF10(ctx, base);
	// 001EBF4D add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBF50 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBF52 mov [esp+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBF56 jnz 0x001EBFAC
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001EBFAC; }
	// 001EBF58 mov edx, [0x002BE570]
	{ uint32_t ea = (uint32_t)(0x2BE570u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF5E mov ecx, [0x002BE578]
	{ uint32_t ea = (uint32_t)(0x2BE578u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF64 mov eax, [0x002BE574]
	{ uint32_t ea = (uint32_t)(0x2BE574u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF69 mov [esp+0x1C], ecx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBF6D mov [esp+0x14], edx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EBF71 mov edx, [0x002BE588]
	{ uint32_t ea = (uint32_t)(0x2BE588u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF77 mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EBF7C mov esi, 0x2BE330
	{ ctx.esi = (uint32_t)(0x2BE330u); }
	// 001EBF81 lea edi, [esp+0x48]
	ctx.edi = (uint32_t)((uint32_t)(ctx.esp + 0x48u));
	// 001EBF85 mov [esp+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBF89 mov eax, [0x002BE58C]
	{ uint32_t ea = (uint32_t)(0x2BE58Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF8E rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EBF90 mov ecx, [0x002BE580]
	{ uint32_t ea = (uint32_t)(0x2BE580u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBF96 mov [esp+0x20], edx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EBF9A mov edx, [0x002BE584]
	{ uint32_t ea = (uint32_t)(0x2BE584u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFA0 mov [esp+0x24], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EBFA4 mov [esp+0x28], ecx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EBFA8 mov [esp+0x2C], edx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x2Cu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_001EBFAC:
	// 001EBFAC mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFAF test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBFB1 jnz 0x001EBFC4
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001EBFC4; }
	// 001EBFB3 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EBFB5 lea eax, [ebp+0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0x8u));
	// 001EBFB8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBFB9 lea ecx, [ebp+0x18]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0x18u));
	// 001EBFBC push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBFBD call 0x000435F0
	X86_PUSH32(0x1EBFC2u); sub_000435F0(ctx, base);
	// 001EBFC2 jmp 0x001EBFE4
	goto loc_001EBFE4;
loc_001EBFC4:
	// 001EBFC4 mov edx, [ebp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x20u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFC7 mov eax, [ebp+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFCA mov ecx, [ebp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFCD push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EBFCE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBFCF push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EBFD0 call 0x00043490
	X86_PUSH32(0x1EBFD5u); sub_00043490(ctx, base);
	// 001EBFD5 lea esi, [ebp+0x28]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebp + 0x28u));
	// 001EBFD8 mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EBFDD mov edi, 0x2BE330
	{ ctx.edi = (uint32_t)(0x2BE330u); }
	// 001EBFE2 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
loc_001EBFE4:
	// 001EBFE4 mov edx, [ebp+0xDC]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xDCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFEA mov eax, [ebp+0xD8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xD8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EBFF0 add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EBFF3 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EBFF4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EBFF5 call 0x00044A00
	X86_PUSH32(0x1EBFFAu); sub_00044A00(ctx, base);
	// 001EBFFA mov ecx, [ebp+0xEC]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xECu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC000 mov edx, [ebp+0xE8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xE8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC006 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC007 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC008 call 0x00044810
	X86_PUSH32(0x1EC00Du); sub_00044810(ctx, base);
	// 001EC00D call 0x00043790
	X86_PUSH32(0x1EC012u); sub_00043790(ctx, base);
	// 001EC012 lea esi, [ebx+0x140]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebx + 0x140u));
	// 001EC018 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EC01A mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC01C mov edx, [eax+0x04]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC01F mov eax, [eax+0x08]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC022 mov [esp+0x40], ecx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x40u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EC026 lea edi, [ebx+0x150]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ebx + 0x150u));
	// 001EC02C add ebp, 0x144
	{ uint32_t a = (uint32_t)(ctx.ebp); uint32_t b = (uint32_t)(0x144u); uint32_t r = (uint32_t)(a + b); ctx.ebp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC032 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EC033 mov [esp+0x48], edx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x48u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC037 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 001EC039 mov edx, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC03B mov [esp+0x4C], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC03F mov eax, [ecx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC042 mov ecx, [ecx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC045 push 0x08
	{ uint32_t v = (uint32_t)(0x8u); X86_PUSH32(v); }
	// 001EC047 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC049 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EC04A mov [esp+0x5C], edx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x5Cu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC04E mov [esp+0x60], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x60u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC052 mov [esp+0x64], ecx
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x64u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EC056 call 0x00035CC0
	X86_PUSH32(0x1EC05Bu); sub_00035CC0(ctx, base);
	// 001EC05B add esp, 0x20
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x20u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC05E mov ebp, 0x01
	{ ctx.ebp = (uint32_t)(0x1u); }
loc_001EC063:
	// 001EC063 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC065 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC067 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EC068 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EC069 call 0x00035CC0
	X86_PUSH32(0x1EC06Eu); sub_00035CC0(ctx, base);
	// 001EC06E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC071 inc ebp
	{ uint32_t a = (uint32_t)(ctx.ebp);  uint32_t r = (uint32_t)(a + 1); ctx.ebp = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC072 cmp ebp, 0x03
	{ uint32_t a = (uint32_t)(ctx.ebp), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC075 jl 0x001EC063
	if ((int32_t)ca < (int32_t)cb) { X86_BARRIER(); goto loc_001EC063; }
	// 001EC077 mov edx, [esp+0x90]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x90u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC07E push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC07F push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EC080 call 0x00034D50
	X86_PUSH32(0x1EC085u); sub_00034D50(ctx, base);
	// 001EC085 mov eax, [esp+0x38]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x38u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC089 mov ecx, [esp+0x3C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x3Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC08D mov edx, [esp+0x40]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x40u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC091 mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC093 mov eax, [esp+0x44]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x44u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC097 mov [edi], eax
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC099 mov al, [esp+0x9C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x9Cu); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC0A0 mov [esi+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EC0A3 mov ecx, [esp+0x48]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x48u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC0A7 mov [esi+0x08], edx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC0AA mov edx, [esp+0x4C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC0AE add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC0B1 test al, 0x04
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC0B3 mov [edi+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EC0B6 mov [edi+0x08], edx
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC0B9 jz 0x001EC0D4
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC0D4; }
	// 001EC0BB push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC0BD push 0x3F800000
	{ uint32_t v = (uint32_t)(0x3F800000u); X86_PUSH32(v); }
	// 001EC0C2 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC0C4 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC0C6 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC0C8 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC0CA push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC0CC call 0x00045330
	X86_PUSH32(0x1EC0D1u); sub_00045330(ctx, base);
	// 001EC0D1 add esp, 0x1C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x1Cu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC0D4:
	// 001EC0D4 mov eax, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC0D8 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC0DA jz 0x001EC0F4
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC0F4; }
	// 001EC0DC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC0DD call 0x0003BF10
	X86_PUSH32(0x1EC0E2u); sub_0003BF10(ctx, base);
	// 001EC0E2 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC0E5 call 0x00043790
	X86_PUSH32(0x1EC0EAu); sub_00043790(ctx, base);
	// 001EC0EA pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC0EB pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC0EC pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EC0ED pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC0EE add esp, 0x78
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x78u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC0F1 ret 0x0C
	ctx.esp += 16; return;
loc_001EC0F4:
	// 001EC0F4 mov ecx, [esp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC0F8 mov eax, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC0FC mov edx, [esp+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x1Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC100 mov [0x002BE574], ecx
	{ uint32_t ea = (uint32_t)(0x2BE574u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EC106 mov [0x002BE570], eax
	{ uint32_t ea = (uint32_t)(0x2BE570u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC10B mov eax, [esp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC10F mov [0x002BE578], edx
	{ uint32_t ea = (uint32_t)(0x2BE578u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC115 mov edx, [esp+0x28]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x28u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC119 mov ecx, 0x10
	{ ctx.ecx = (uint32_t)(0x10u); }
	// 001EC11E lea esi, [esp+0x48]
	ctx.esi = (uint32_t)((uint32_t)(ctx.esp + 0x48u));
	// 001EC122 mov edi, 0x2BE330
	{ ctx.edi = (uint32_t)(0x2BE330u); }
	// 001EC127 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EC129 mov ecx, [esp+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x24u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC12D mov [0x002BE588], eax
	{ uint32_t ea = (uint32_t)(0x2BE588u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC132 mov eax, [esp+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x2Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC136 mov [0x002BE58C], ecx
	{ uint32_t ea = (uint32_t)(0x2BE58Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001EC13C mov [0x002BE580], edx
	{ uint32_t ea = (uint32_t)(0x2BE580u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC142 mov [0x002BE584], eax
	{ uint32_t ea = (uint32_t)(0x2BE584u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC147 call 0x00043790
	X86_PUSH32(0x1EC14Cu); sub_00043790(ctx, base);
	// 001EC14C pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC14D pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC14E pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EC14F pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC150 add esp, 0x78
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x78u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC153 ret 0x0C
	ctx.esp += 16; return;
}
X86_WEAK_FUNC(sub_001EBEE0, __imp__sub_001EBEE0)

X86_FUNC(__imp__sub_001EC160) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC160 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC164 mov [ecx+0x14C], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x14Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC16A mov [ecx+0x148], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x148u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC170 mov [ecx+0x144], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x144u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC176 ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EC160, __imp__sub_001EC160)

X86_FUNC(__imp__sub_001EC180) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC180 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC181 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 001EC183 call 0x001EBE80
	X86_PUSH32(0x1EC188u); sub_001EBE80(ctx, base);
	// 001EC188 test byte ptr [esp+0x08], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC18D jz 0x001EC198
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC198; }
	// 001EC18F push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC190 call 0x0004C110
	X86_PUSH32(0x1EC195u); sub_0004C110(ctx, base);
	// 001EC195 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC198:
	// 001EC198 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EC19A pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC19B ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EC180, __imp__sub_001EC180)

X86_FUNC(__imp__sub_001EC1A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC1A0 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC1A1 mov esi, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC1A5 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC1A6 mov edi, ecx
	{ ctx.edi = (uint32_t)(ctx.ecx); }
	// 001EC1A8 mov eax, [edi+esi*4+0x150]
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.esi * 4u + 0x150u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC1AF test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC1B1 jl 0x001EC1DD
	if (((int32_t)(ca & cb)) < 0) { goto loc_001EC1DD; }
	// 001EC1B3 cmp eax, 0x01
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x1u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC1B6 jnle 0x001EC1DD
	if ((int32_t)ca > (int32_t)cb) { goto loc_001EC1DD; }
	// 001EC1B8 test esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC1BA jnz 0x001EC1DD
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001EC1DD; }
	// 001EC1BC push 0x06
	{ uint32_t v = (uint32_t)(0x6u); X86_PUSH32(v); }
	// 001EC1BE call 0x00044AE0
	X86_PUSH32(0x1EC1C3u); sub_00044AE0(ctx, base);
	// 001EC1C3 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC1C4 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC1C5 push 0x0A
	{ uint32_t v = (uint32_t)(0xAu); X86_PUSH32(v); }
	// 001EC1C7 call 0x00044B00
	X86_PUSH32(0x1EC1CCu); sub_00044B00(ctx, base);
	// 001EC1CC push 0x06
	{ uint32_t v = (uint32_t)(0x6u); X86_PUSH32(v); }
	// 001EC1CE call 0x00046D00
	X86_PUSH32(0x1EC1D3u); sub_00046D00(ctx, base);
	// 001EC1D3 push 0x07
	{ uint32_t v = (uint32_t)(0x7u); X86_PUSH32(v); }
	// 001EC1D5 call 0x00046D00
	X86_PUSH32(0x1EC1DAu); sub_00046D00(ctx, base);
	// 001EC1DA add esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC1DD:
	// 001EC1DD push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC1DF push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC1E0 push 0x0D
	{ uint32_t v = (uint32_t)(0xDu); X86_PUSH32(v); }
	// 001EC1E2 call 0x00044B00
	X86_PUSH32(0x1EC1E7u); sub_00044B00(ctx, base);
	// 001EC1E7 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC1E9 call 0x00044AE0
	X86_PUSH32(0x1EC1EEu); sub_00044AE0(ctx, base);
	// 001EC1EE push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC1F0 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC1F1 call 0x000416C0
	X86_PUSH32(0x1EC1F6u); sub_000416C0(ctx, base);
	// 001EC1F6 add esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC1F9 test esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC1FB jz 0x001EC208
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC208; }
	// 001EC1FD push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC1FF xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC201 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC203 call 0x001F6700
	X86_PUSH32(0x1EC208u); sub_001F6700(ctx, base);
loc_001EC208:
	// 001EC208 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC20A mov edx, 0x0A
	{ ctx.edx = (uint32_t)(0xAu); }
	// 001EC20F mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC211 call 0x001F6700
	X86_PUSH32(0x1EC216u); sub_001F6700(ctx, base);
	// 001EC216 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC218 mov edx, 0x0B
	{ ctx.edx = (uint32_t)(0xBu); }
	// 001EC21D mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC21F call 0x001F6700
	X86_PUSH32(0x1EC224u); sub_001F6700(ctx, base);
	// 001EC224 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC225 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC226 call 0x001F6BF0
	X86_PUSH32(0x1EC22Bu); sub_001F6BF0(ctx, base);
	// 001EC22B push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC22D mov edx, 0x09
	{ ctx.edx = (uint32_t)(0x9u); }
	// 001EC232 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC234 call 0x001F6700
	X86_PUSH32(0x1EC239u); sub_001F6700(ctx, base);
	// 001EC239 test byte ptr [edi+0xAC], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edi + 0xACu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC240 jz 0x001EC249
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC249; }
	// 001EC242 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC244 call 0x001F7370
	X86_PUSH32(0x1EC249u); sub_001F7370(ctx, base);
loc_001EC249:
	// 001EC249 mov eax, [esi*4+0x2BEE80]
	{ uint32_t ea = (uint32_t)(ctx.esi * 4u + 0x2BEE80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC250 dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC251 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC252 mov [esi*4+0x2BEE80], eax
	{ uint32_t ea = (uint32_t)(ctx.esi * 4u + 0x2BEE80u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC259 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC25A ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EC1A0, __imp__sub_001EC1A0)

X86_FUNC(__imp__sub_001EC260) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC260 ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EC260, __imp__sub_001EC260)

X86_FUNC(__imp__sub_001EC270) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC270 sub esp, 0x100
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x100u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC276 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC277 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC278 lea eax, [esp+0x88]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x88u));
	// 001EC27F push 0x2BE270
	{ uint32_t v = (uint32_t)(0x2BE270u); X86_PUSH32(v); }
	// 001EC284 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC285 mov edi, ecx
	{ ctx.edi = (uint32_t)(ctx.ecx); }
	// 001EC287 call 0x000502F0
	X86_PUSH32(0x1EC28Cu); sub_000502F0(ctx, base);
	// 001EC28C fld [edi+0x20]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x20u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EC28F fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EC291 fstp [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EC295 fld [edi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Cu); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EC298 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EC29A fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EC29D push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC29E fld [edi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x18u); x87_push(ctx, (double)X86_MEMF32(ea)); }
	// 001EC2A1 lea ecx, [esp+0x14]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x14u));
	// 001EC2A5 fchs
	{ X87_ST(0) = -X87_ST(0); }
	// 001EC2A7 fstp [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); X86_MEMF32(ea) = (float)X87_ST(0); x87_pop(ctx); }
	// 001EC2AA push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC2AB call 0x00050250
	X86_PUSH32(0x1EC2B0u); sub_00050250(ctx, base);
	// 001EC2B0 lea eax, [esp+0x58]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x58u));
	// 001EC2B4 lea edx, [edi-0x08]
	ctx.edx = (uint32_t)((uint32_t)(ctx.edi + 0xFFFFFFF8u));
	// 001EC2B7 or edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC2B9 lea ecx, [esp+0x18]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x18u));
	// 001EC2BD add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC2C0 or edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC2C2 test dl, 0x0F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(0xFu); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC2C5 lea edx, [esp+0x08]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x8u));
	// 001EC2C9 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC2CA lea eax, [edi+0x28]
	ctx.eax = (uint32_t)((uint32_t)(ctx.edi + 0x28u));
	// 001EC2CD lea ecx, [esp+0x4C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x4Cu));
	// 001EC2D1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC2D2 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC2D3 jz 0x001EC2DC
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC2DC; }
	// 001EC2D5 call 0x0004FF30
	X86_PUSH32(0x1EC2DAu); sub_0004FF30(ctx, base);
	// 001EC2DA jmp 0x001EC2E1
	goto loc_001EC2E1;
loc_001EC2DC:
	// 001EC2DC call 0x00050150
	X86_PUSH32(0x1EC2E1u); sub_00050150(ctx, base);
loc_001EC2E1:
	// 001EC2E1 lea eax, [esp+0x14]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x14u));
	// 001EC2E5 lea edx, [esp+0x54]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x54u));
	// 001EC2E9 or edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC2EB lea ecx, [esp+0x94]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x94u));
	// 001EC2F2 add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC2F5 or edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC2F7 test dl, 0x0F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(0xFu); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC2FA lea edx, [esp+0x88]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x88u));
	// 001EC301 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC302 lea eax, [esp+0x4C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x4Cu));
	// 001EC306 lea ecx, [esp+0x0C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 001EC30A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC30B push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC30C jz 0x001EC315
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC315; }
	// 001EC30E call 0x0004FF30
	X86_PUSH32(0x1EC313u); sub_0004FF30(ctx, base);
	// 001EC313 jmp 0x001EC31A
	goto loc_001EC31A;
loc_001EC315:
	// 001EC315 call 0x00050150
	X86_PUSH32(0x1EC31Au); sub_00050150(ctx, base);
loc_001EC31A:
	// 001EC31A lea eax, [esp+0xD4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0xD4u));
	// 001EC321 mov edx, edi
	{ ctx.edx = (uint32_t)(ctx.edi); }
	// 001EC323 or edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC325 lea ecx, [esp+0x14]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x14u));
	// 001EC329 add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC32C or edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC32E test dl, 0x0F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(0xFu); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC331 lea edx, [esp+0x08]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x8u));
	// 001EC335 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC336 lea eax, [edi+0xF0]
	ctx.eax = (uint32_t)((uint32_t)(ctx.edi + 0xF0u));
	// 001EC33C lea ecx, [esp+0xCC]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0xCCu));
	// 001EC343 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC344 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC345 jz 0x001EC34E
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC34E; }
	// 001EC347 call 0x0004FF30
	X86_PUSH32(0x1EC34Cu); sub_0004FF30(ctx, base);
	// 001EC34C jmp 0x001EC353
	goto loc_001EC353;
loc_001EC34E:
	// 001EC34E call 0x00050150
	X86_PUSH32(0x1EC353u); sub_00050150(ctx, base);
loc_001EC353:
	// 001EC353 mov al, [edi+0xAC]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0xACu); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC359 add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC35C test al, 0x01
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC35E jz 0x001EC36C
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC36C; }
	// 001EC360 lea edx, [esp+0x48]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x48u));
	// 001EC364 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC365 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 001EC367 call 0x001EBB20
	X86_PUSH32(0x1EC36Cu); sub_001EBB20(ctx, base);
loc_001EC36C:
	// 001EC36C mov esi, [esp+0x10C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10Cu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC373 mov eax, [edi+0x140]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x140u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC379 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC37A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC37B call 0x000455A0
	X86_PUSH32(0x1EC380u); sub_000455A0(ctx, base);
	// 001EC380 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC383 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EC385 mov edx, 0x0A
	{ ctx.edx = (uint32_t)(0xAu); }
	// 001EC38A mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC38C call 0x001F6700
	X86_PUSH32(0x1EC391u); sub_001F6700(ctx, base);
	// 001EC391 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EC393 mov edx, 0x0B
	{ ctx.edx = (uint32_t)(0xBu); }
	// 001EC398 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC39A call 0x001F6700
	X86_PUSH32(0x1EC39Fu); sub_001F6700(ctx, base);
	// 001EC39F push 0xFFFFFFFF
	{ uint32_t v = (uint32_t)(0xFFFFFFFFu); X86_PUSH32(v); }
	// 001EC3A1 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC3A2 call 0x001F6D30
	X86_PUSH32(0x1EC3A7u); sub_001F6D30(ctx, base);
	// 001EC3A7 mov eax, [esp+0x110]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x110u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC3AE mov [edi+esi*4+0x150], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.esi * 4u + 0x150u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC3B5 xor edi, edi
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a ^ b); ctx.edi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC3B7 sub eax, edi
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC3B9 jz 0x001EC440
	if (zf) { goto loc_001EC440; }
	// 001EC3BF dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC3C0 jz 0x001EC411
	if (zf) { goto loc_001EC411; }
	// 001EC3C2 dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC3C3 jnz 0x001EC4D7
	if (!zf) { goto loc_001EC4D7; }
	// 001EC3C9 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC3CB:
	// 001EC3CB push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EC3CD mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC3CF call 0x001F6700
	X86_PUSH32(0x1EC3D4u); sub_001F6700(ctx, base);
	// 001EC3D4 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC3D6 mov edx, 0x02
	{ ctx.edx = (uint32_t)(0x2u); }
	// 001EC3DB mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC3DD call 0x001F6700
	X86_PUSH32(0x1EC3E2u); sub_001F6700(ctx, base);
	// 001EC3E2 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC3E4 mov edx, 0x03
	{ ctx.edx = (uint32_t)(0x3u); }
	// 001EC3E9 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC3EB call 0x001F6700
	X86_PUSH32(0x1EC3F0u); sub_001F6700(ctx, base);
	// 001EC3F0 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC3F2 mov edx, 0x04
	{ ctx.edx = (uint32_t)(0x4u); }
	// 001EC3F7 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC3F9 call 0x001F6700
	X86_PUSH32(0x1EC3FEu); sub_001F6700(ctx, base);
	// 001EC3FE push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC400 mov edx, 0x06
	{ ctx.edx = (uint32_t)(0x6u); }
	// 001EC405 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC407 call 0x001F6700
	X86_PUSH32(0x1EC40Cu); sub_001F6700(ctx, base);
	// 001EC40C jmp 0x001EC4D7
	goto loc_001EC4D7;
loc_001EC411:
	// 001EC411 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC413 cmp esi, edi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC415 jnz 0x001EC3CB
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_001EC3CB; }
	// 001EC417 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC419 xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC41B call 0x001F6700
	X86_PUSH32(0x1EC420u); sub_001F6700(ctx, base);
	// 001EC420 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC422 mov edx, 0x02
	{ ctx.edx = (uint32_t)(0x2u); }
	// 001EC427 xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC429 call 0x001F6700
	X86_PUSH32(0x1EC42Eu); sub_001F6700(ctx, base);
	// 001EC42E push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC430 mov edx, 0x04
	{ ctx.edx = (uint32_t)(0x4u); }
	// 001EC435 xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC437 call 0x001F6700
	X86_PUSH32(0x1EC43Cu); sub_001F6700(ctx, base);
	// 001EC43C xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC43E jmp 0x001EC469
	goto loc_001EC469;
loc_001EC440:
	// 001EC440 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC442 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC444 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC446 call 0x001F6700
	X86_PUSH32(0x1EC44Bu); sub_001F6700(ctx, base);
	// 001EC44B push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC44D mov edx, 0x02
	{ ctx.edx = (uint32_t)(0x2u); }
	// 001EC452 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC454 call 0x001F6700
	X86_PUSH32(0x1EC459u); sub_001F6700(ctx, base);
	// 001EC459 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC45B mov edx, 0x04
	{ ctx.edx = (uint32_t)(0x4u); }
	// 001EC460 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC462 call 0x001F6700
	X86_PUSH32(0x1EC467u); sub_001F6700(ctx, base);
	// 001EC467 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
loc_001EC469:
	// 001EC469 mov edx, 0x06
	{ ctx.edx = (uint32_t)(0x6u); }
	// 001EC46E push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC470 call 0x001F6700
	X86_PUSH32(0x1EC475u); sub_001F6700(ctx, base);
	// 001EC475 mov ecx, 0x40344
	{ ctx.ecx = (uint32_t)(0x40344u); }
	// 001EC47A mov edx, 0x306
	{ ctx.edx = (uint32_t)(0x306u); }
	// 001EC47F call 0x001F6650
	X86_PUSH32(0x1EC484u); sub_001F6650(ctx, base);
	// 001EC484 mov ecx, 0x40348
	{ ctx.ecx = (uint32_t)(0x40348u); }
	// 001EC489 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC48B mov dword ptr [0x00203A48], 0x306
	{ uint32_t ea = (uint32_t)(0x203A48u); X86_MEM32(ea) = (uint32_t)(0x306u); }
	// 001EC495 call 0x001F6650
	X86_PUSH32(0x1EC49Au); sub_001F6650(ctx, base);
	// 001EC49A mov ecx, 0x40350
	{ ctx.ecx = (uint32_t)(0x40350u); }
	// 001EC49F mov edx, 0x8006
	{ ctx.edx = (uint32_t)(0x8006u); }
	// 001EC4A4 mov [0x00203A4C], edi
	{ uint32_t ea = (uint32_t)(0x203A4Cu); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 001EC4AA call 0x001F6650
	X86_PUSH32(0x1EC4AFu); sub_001F6650(ctx, base);
	// 001EC4AF push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC4B0 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC4B2 push 0x07
	{ uint32_t v = (uint32_t)(0x7u); X86_PUSH32(v); }
	// 001EC4B4 mov dword ptr [0x00203A78], 0x8006
	{ uint32_t ea = (uint32_t)(0x203A78u); X86_MEM32(ea) = (uint32_t)(0x8006u); }
	// 001EC4BE call 0x00046CB0
	X86_PUSH32(0x1EC4C3u); sub_00046CB0(ctx, base);
	// 001EC4C3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC4C4 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC4C6 push 0x06
	{ uint32_t v = (uint32_t)(0x6u); X86_PUSH32(v); }
	// 001EC4C8 call 0x00046CB0
	X86_PUSH32(0x1EC4CDu); sub_00046CB0(ctx, base);
	// 001EC4CD push 0x06
	{ uint32_t v = (uint32_t)(0x6u); X86_PUSH32(v); }
	// 001EC4CF call 0x00044AC0
	X86_PUSH32(0x1EC4D4u); sub_00044AC0(ctx, base);
	// 001EC4D4 add esp, 0x1C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x1Cu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC4D7:
	// 001EC4D7 lea ecx, [esp+0xC8]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0xC8u));
	// 001EC4DE push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 001EC4DF lea edx, [esi+0x02]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esi + 0x2u));
	// 001EC4E2 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 001EC4E3 call 0x001F5360
	X86_PUSH32(0x1EC4E8u); sub_001F5360(ctx, base);
	// 001EC4E8 push 0x103
	{ uint32_t v = (uint32_t)(0x103u); X86_PUSH32(v); }
	// 001EC4ED mov edx, 0x09
	{ ctx.edx = (uint32_t)(0x9u); }
	// 001EC4F2 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 001EC4F4 call 0x001F6700
	X86_PUSH32(0x1EC4F9u); sub_001F6700(ctx, base);
	// 001EC4F9 push 0x20000
	{ uint32_t v = (uint32_t)(0x20000u); X86_PUSH32(v); }
	// 001EC4FE push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC4FF call 0x001F6BF0
	X86_PUSH32(0x1EC504u); sub_001F6BF0(ctx, base);
	// 001EC504 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC505 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC506 push 0x0D
	{ uint32_t v = (uint32_t)(0xDu); X86_PUSH32(v); }
	// 001EC508 call 0x00044B00
	X86_PUSH32(0x1EC50Du); sub_00044B00(ctx, base);
	// 001EC50D push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 001EC50F call 0x00044AC0
	X86_PUSH32(0x1EC514u); sub_00044AC0(ctx, base);
	// 001EC514 mov eax, [esi*4+0x2BEE80]
	{ uint32_t ea = (uint32_t)(ctx.esi * 4u + 0x2BEE80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC51B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC51E inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC51F pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC520 mov [esi*4+0x2BEE80], eax
	{ uint32_t ea = (uint32_t)(ctx.esi * 4u + 0x2BEE80u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC527 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC528 add esp, 0x100
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x100u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC52E ret 0x08
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_001EC270, __imp__sub_001EC270)

X86_FUNC(__imp__sub_001EC540) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC540 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC544 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC546 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC547 call 0x001EC270
	X86_PUSH32(0x1EC54Cu); sub_001EC270(ctx, base);
	// 001EC54C ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_001EC540, __imp__sub_001EC540)

X86_FUNC(__imp__sub_001EC550) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC550 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EC551 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001EC553 cmp dword ptr [ebp+0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC557 jz 0x001EC5A1
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_001EC5A1; }
	// 001EC559 cmp dword ptr [0x00315AC4], 0x00
	{ uint32_t ea = (uint32_t)(0x315AC4u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC560 jnz 0x001EC568
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_001EC568; }
	// 001EC562 pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EC563 jmp 0x001ECBC0
	goto loc_001ECBC0;
loc_001EC568:
	// 001EC568 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EC569 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC56A mov esi, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC56D push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC56E mov edi, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
loc_001EC571:
	// 001EC571 movzx eax, byte ptr [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 001EC574 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC575 call 0x001E566A
	X86_PUSH32(0x1EC57Au); sub_001E566A(ctx, base);
	// 001EC57A mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 001EC57C movzx eax, byte ptr [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 001EC57F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EC580 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC581 call 0x001E566A
	X86_PUSH32(0x1EC586u); sub_001E566A(ctx, base);
	// 001EC586 pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001EC587 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC588 dec [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); uint32_t a = (uint32_t)(X86_MEM32(ea));  uint32_t r = (uint32_t)(a - 1); X86_MEM32(ea) = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC58B pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001EC58C mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 001EC58E jz 0x001EC598
	if (zf) { goto loc_001EC598; }
	// 001EC590 test ebx, ebx
	{ uint32_t a = (uint32_t)(ctx.ebx), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC592 jz 0x001EC598
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC598; }
	// 001EC594 cmp ebx, ecx
	{ uint32_t a = (uint32_t)(ctx.ebx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC596 jz 0x001EC571
	if ((uint32_t)ca == (uint32_t)cb) { X86_BARRIER(); goto loc_001EC571; }
loc_001EC598:
	// 001EC598 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC599 mov eax, ebx
	{ ctx.eax = (uint32_t)(ctx.ebx); }
	// 001EC59B pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC59C sub eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC59E pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC59F pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EC5A0 ret
	ctx.esp += 4; return;
loc_001EC5A1:
	// 001EC5A1 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5A3 pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EC5A4 ret
	ctx.esp += 4; return;
loc_001ECBC0:
	// 001ECBC0 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001ECBC1 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001ECBC3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001ECBC4 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001ECBC5 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001ECBC6 mov ecx, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBC9 or ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECBCB jz 0x001ECC14
	if (zf) { goto loc_001ECC14; }
	// 001ECBCD mov esi, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBD0 mov edi, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBD3 mov bh, 0x41
	{ ctx.ebx = (ctx.ebx & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(0x41u) << 8); }
	// 001ECBD5 mov bl, 0x5A
	{ ctx.ebx = (ctx.ebx & 0xFFFFFF00u) | (uint8_t)(0x5Au); }
	// 001ECBD7 mov dh, 0x20
	{ ctx.edx = (ctx.edx & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(0x20u) << 8); }
	// 001ECBD9 lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
loc_001ECBDC:
	// 001ECBDC mov ah, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(X86_MEM8(ea)) << 8); }
	// 001ECBDE or ah, ah
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))); uint8_t b = (uint8_t)(((uint8_t)(ctx.eax >> 8))); uint8_t r = (uint8_t)(a | b); ctx.eax = (ctx.eax & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(r) << 8); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECBE0 mov al, [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBE2 jz 0x001ECC05
	if (zf) { goto loc_001ECC05; }
	// 001ECBE4 or al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a | b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECBE6 jz 0x001ECC05
	if (zf) { goto loc_001ECC05; }
	// 001ECBE8 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECBE9 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECBEA cmp ah, bh
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(((uint8_t)(ctx.ebx >> 8))); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECBEC jb 0x001ECBF4
	if ((uint8_t)ca < (uint8_t)cb) { goto loc_001ECBF4; }
	// 001ECBEE cmp ah, bl
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(((uint8_t)ctx.ebx)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECBF0 jnbe 0x001ECBF4
	if ((uint8_t)ca > (uint8_t)cb) { goto loc_001ECBF4; }
	// 001ECBF2 add ah, dh
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))); uint8_t b = (uint8_t)(((uint8_t)(ctx.edx >> 8))); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(r) << 8); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_001ECBF4:
	// 001ECBF4 cmp al, bh
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)(ctx.ebx >> 8))); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECBF6 jb 0x001ECBFE
	if ((uint8_t)ca < (uint8_t)cb) { goto loc_001ECBFE; }
	// 001ECBF8 cmp al, bl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.ebx)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECBFA jnbe 0x001ECBFE
	if ((uint8_t)ca > (uint8_t)cb) { goto loc_001ECBFE; }
	// 001ECBFC add al, dh
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)(ctx.edx >> 8))); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_001ECBFE:
	// 001ECBFE cmp ah, al
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECC00 jnz 0x001ECC0B
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_001ECC0B; }
	// 001ECC02 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECC03 jnz 0x001ECBDC
	if (!zf) { X86_BARRIER(); goto loc_001ECBDC; }
loc_001ECC05:
	// 001ECC05 xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECC07 cmp ah, al
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001ECC09 jz 0x001ECC14
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_001ECC14; }
loc_001ECC0B:
	// 001ECC0B mov ecx, 0xFFFFFFFF
	{ ctx.ecx = (uint32_t)(0xFFFFFFFFu); }
	// 001ECC10 jb 0x001ECC14
	if (cf) { goto loc_001ECC14; }
	// 001ECC12 neg ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(0 - a); ctx.ecx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001ECC14:
	// 001ECC14 mov eax, ecx
	{ ctx.eax = (uint32_t)(ctx.ecx); }
	// 001ECC16 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001ECC17 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECC18 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECC19 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECC1A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC550, __imp__sub_001EC550)

X86_FUNC(__imp__sub_001EC5A5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC5A5 cmp dword ptr [esp+0x08], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5AA jz 0x001EC5B2
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_001EC5B2; }
	// 001EC5AC mov byte ptr [ecx], 0x2D
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM8(ea) = (uint8_t)(0x2Du); }
	// 001EC5AF inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5B0 neg eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(0 - a); ctx.eax = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC5B2:
	// 001EC5B2 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC5B3 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
loc_001EC5B5:
	// 001EC5B5 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5B7 div [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint64_t n = ((uint64_t)ctx.edx << 32) | ctx.eax; uint32_t d = (uint32_t)X86_MEM32(ea); if (!d) x86_trap(ctx, 0x1EC5B7, "divide error"); uint64_t q = n / d; if (q > 0xFFFFFFFFull) x86_trap(ctx, 0x1EC5B7, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 001EC5BB cmp edx, 0x09
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(0x9u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5BE jbe 0x001EC5C5
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_001EC5C5; }
	// 001EC5C0 add dl, 0x57
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)); uint8_t b = (uint8_t)(0x57u); uint8_t r = (uint8_t)(a + b); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC5C3 jmp 0x001EC5C8
	goto loc_001EC5C8;
loc_001EC5C5:
	// 001EC5C5 add dl, 0x30
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)); uint8_t b = (uint8_t)(0x30u); uint8_t r = (uint8_t)(a + b); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_001EC5C8:
	// 001EC5C8 mov [ecx], dl
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 001EC5CA inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5CB test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5CD jnbe 0x001EC5B5
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_001EC5B5; }
	// 001EC5CF and byte ptr [ecx], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ecx); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC5D2 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC5D3:
	// 001EC5D3 mov dl, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC5D5 mov al, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC5D7 mov [ecx], dl
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 001EC5D9 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5DA mov [esi], al
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC5DC inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5DD cmp esi, ecx
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5DF jb 0x001EC5D3
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001EC5D3; }
	// 001EC5E1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC5E2 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC5A5, __imp__sub_001EC5A5)

X86_FUNC(__imp__sub_001EC5E3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC5E3 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EC5E4 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001EC5E6 cmp dword ptr [ebp+0x10], 0x0A
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0xAu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5EA mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC5ED jnz 0x001EC5F9
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_001EC5F9; }
	// 001EC5EF test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC5F1 jnl 0x001EC5F9
	if (((int32_t)(ca & cb)) >= 0) { goto loc_001EC5F9; }
	// 001EC5F3 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001EC5F5 push 0x0A
	{ uint32_t v = (uint32_t)(0xAu); X86_PUSH32(v); }
	// 001EC5F7 jmp 0x001EC5FE
	goto loc_001EC5FE;
loc_001EC5F9:
	// 001EC5F9 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 001EC5FB push [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
loc_001EC5FE:
	// 001EC5FE mov ecx, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC601 call 0x001EC5A5
	X86_PUSH32(0x1EC606u); sub_001EC5A5(ctx, base);
	// 001EC606 mov eax, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC609 pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001EC60A pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001EC60B pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 001EC60C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC5E3, __imp__sub_001EC5E3)

X86_FUNC(__imp__sub_001EC60D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC60D mov ecx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC611 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC613 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC614 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC615 mov edi, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC619 mov esi, edi
	{ ctx.esi = (uint32_t)(ctx.edi); }
	// 001EC61B jz 0x001EC645
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC645; }
	// 001EC61D mov edx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
loc_001EC621:
	// 001EC621 mov ax, [edx]
	{ uint32_t ea = (uint32_t)(ctx.edx); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 001EC624 mov [edi], ax
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 001EC627 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC628 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC629 inc edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(a + 1); ctx.edx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC62A inc edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(a + 1); ctx.edx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC62B test ax, ax
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.eax)), b = (uint16_t)(((uint16_t)ctx.eax)); uint16_t r = (uint16_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 001EC62E jz 0x001EC633
	if (((uint16_t)(ca & cb)) == 0) { goto loc_001EC633; }
	// 001EC630 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC631 jnz 0x001EC621
	if (!zf) { X86_BARRIER(); goto loc_001EC621; }
loc_001EC633:
	// 001EC633 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC635 jz 0x001EC645
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC645; }
	// 001EC637 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC638 jz 0x001EC645
	if (zf) { goto loc_001EC645; }
	// 001EC63A xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC63C shr ecx, 0x01
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x1u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC63E rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 001EC640 adc ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t c0 = cf; uint32_t r = (uint32_t)(a + b + c0); ctx.ecx = (uint32_t)(r); cf = ((uint64_t)a + b + c0) >> 32 & 1; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC642 rep stosw
	{ x86_rep_stos(ctx, base, 2); }
loc_001EC645:
	// 001EC645 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC646 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001EC648 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC649 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC60D, __imp__sub_001EC60D)

X86_FUNC(__imp__sub_001EC650) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC650 cmp cl, 0x40
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0x40u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC653 jnb 0x001EC66A
	if ((uint8_t)ca >= (uint8_t)cb) { goto loc_001EC66A; }
	// 001EC655 cmp cl, 0x20
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0x20u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC658 jnb 0x001EC660
	if ((uint8_t)ca >= (uint8_t)cb) { goto loc_001EC660; }
	// 001EC65A shrd eax, edx, cl
	{ uint32_t a = ctx.eax, b = ctx.edx; uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((((uint64_t)b << 32) | a) >> c); cf = (a >> (c - 1)) & 1; of = ((r ^ a) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 001EC65D shr edx, cl
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edx = (uint32_t)(r); } }
	// 001EC65F ret
	ctx.esp += 4; return;
loc_001EC660:
	// 001EC660 mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 001EC662 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC664 and cl, 0x1F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(0x1Fu); uint8_t r = (uint8_t)(a & b); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC667 shr eax, cl
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 001EC669 ret
	ctx.esp += 4; return;
loc_001EC66A:
	// 001EC66A xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC66C xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC66E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC650, __imp__sub_001EC650)

X86_FUNC(__imp__sub_001EC670) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC670 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EC671 mov eax, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC675 or eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC677 jnz 0x001EC691
	if (!zf) { goto loc_001EC691; }
	// 001EC679 mov ecx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC67D mov eax, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC681 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC683 div ecx
	{ uint64_t n = ((uint64_t)ctx.edx << 32) | ctx.eax; uint32_t d = (uint32_t)ctx.ecx; if (!d) x86_trap(ctx, 0x1EC683, "divide error"); uint64_t q = n / d; if (q > 0xFFFFFFFFull) x86_trap(ctx, 0x1EC683, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 001EC685 mov eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC689 div ecx
	{ uint64_t n = ((uint64_t)ctx.edx << 32) | ctx.eax; uint32_t d = (uint32_t)ctx.ecx; if (!d) x86_trap(ctx, 0x1EC689, "divide error"); uint64_t q = n / d; if (q > 0xFFFFFFFFull) x86_trap(ctx, 0x1EC689, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 001EC68B mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 001EC68D xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC68F jmp 0x001EC6E1
	goto loc_001EC6E1;
loc_001EC691:
	// 001EC691 mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 001EC693 mov ebx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC697 mov edx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC69B mov eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
loc_001EC69F:
	// 001EC69F shr ecx, 0x01
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x1u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC6A1 rcr ebx, 0x01
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t n = ((uint32_t)(0x1u) & 31) % 33; for (; n; n--) { uint32_t lo = a & 1; a = (uint32_t)(((uint32_t)a >> 1) | (cf << 31)); cf = lo; } ctx.ebx = (uint32_t)(a); }
	// 001EC6A3 shr edx, 0x01
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t c = (uint32_t)(0x1u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edx = (uint32_t)(r); } }
	// 001EC6A5 rcr eax, 0x01
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t n = ((uint32_t)(0x1u) & 31) % 33; for (; n; n--) { uint32_t lo = a & 1; a = (uint32_t)(((uint32_t)a >> 1) | (cf << 31)); cf = lo; } ctx.eax = (uint32_t)(a); }
	// 001EC6A7 or ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6A9 jnz 0x001EC69F
	if (!zf) { X86_BARRIER(); goto loc_001EC69F; }
	// 001EC6AB div ebx
	{ uint64_t n = ((uint64_t)ctx.edx << 32) | ctx.eax; uint32_t d = (uint32_t)ctx.ebx; if (!d) x86_trap(ctx, 0x1EC6AB, "divide error"); uint64_t q = n / d; if (q > 0xFFFFFFFFull) x86_trap(ctx, 0x1EC6AB, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 001EC6AD mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 001EC6AF mul [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); uint64_t p = (uint64_t)ctx.eax * (uint32_t)X86_MEM32(ea); ctx.eax = (uint32_t)p; ctx.edx = (uint32_t)(p >> 32); cf = of = ctx.edx != 0; }
	// 001EC6B3 xchg ecx, eax
	{ uint32_t t = (uint32_t)(ctx.ecx); ctx.ecx = (uint32_t)(ctx.eax); ctx.eax = (uint32_t)(t); }
	// 001EC6B4 mul [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); uint64_t p = (uint64_t)ctx.eax * (uint32_t)X86_MEM32(ea); ctx.eax = (uint32_t)p; ctx.edx = (uint32_t)(p >> 32); cf = of = ctx.edx != 0; }
	// 001EC6B8 add edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); ctx.edx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6BA jb 0x001EC6CA
	if (cf) { goto loc_001EC6CA; }
	// 001EC6BC cmp edx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6C0 jnbe 0x001EC6CA
	if ((uint32_t)ca > (uint32_t)cb) { goto loc_001EC6CA; }
	// 001EC6C2 jb 0x001EC6D2
	if (cf) { goto loc_001EC6D2; }
	// 001EC6C4 cmp eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6C8 jbe 0x001EC6D2
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_001EC6D2; }
loc_001EC6CA:
	// 001EC6CA sub eax, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6CE sbb edx, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t c0 = cf; uint32_t r = (uint32_t)(a - b - c0); ctx.edx = (uint32_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC6D2:
	// 001EC6D2 sub eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6D6 sbb edx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t c0 = cf; uint32_t r = (uint32_t)(a - b - c0); ctx.edx = (uint32_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6DA neg edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(0 - a); ctx.edx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6DC neg eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(0 - a); ctx.eax = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC6DE sbb edx, 0x00
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x0u); uint32_t c0 = cf; uint32_t r = (uint32_t)(a - b - c0); ctx.edx = (uint32_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC6E1:
	// 001EC6E1 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC6E2 ret 0x10
	ctx.esp += 20; return;
}
X86_WEAK_FUNC(sub_001EC670, __imp__sub_001EC670)

X86_FUNC(__imp__sub_001EC6F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC6F0 cmp cl, 0x40
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0x40u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC6F3 jnb 0x001EC70B
	if ((uint8_t)ca >= (uint8_t)cb) { goto loc_001EC70B; }
	// 001EC6F5 cmp cl, 0x20
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0x20u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC6F8 jnb 0x001EC700
	if ((uint8_t)ca >= (uint8_t)cb) { goto loc_001EC700; }
	// 001EC6FA shrd eax, edx, cl
	{ uint32_t a = ctx.eax, b = ctx.edx; uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((((uint64_t)b << 32) | a) >> c); cf = (a >> (c - 1)) & 1; of = ((r ^ a) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 001EC6FD sar edx, cl
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((int32_t)(int32_t)a >> c); cf = (uint32_t)((int32_t)(int32_t)a >> (c - 1)) & 1; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edx = (uint32_t)(r); } }
	// 001EC6FF ret
	ctx.esp += 4; return;
loc_001EC700:
	// 001EC700 mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 001EC702 sar edx, 0x1F
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t c = (uint32_t)(0x1Fu) & 31; if (c) { uint32_t r = (uint32_t)((int32_t)(int32_t)a >> c); cf = (uint32_t)((int32_t)(int32_t)a >> (c - 1)) & 1; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edx = (uint32_t)(r); } }
	// 001EC705 and cl, 0x1F
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(0x1Fu); uint8_t r = (uint8_t)(a & b); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC708 sar eax, cl
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((int32_t)(int32_t)a >> c); cf = (uint32_t)((int32_t)(int32_t)a >> (c - 1)) & 1; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 001EC70A ret
	ctx.esp += 4; return;
loc_001EC70B:
	// 001EC70B sar edx, 0x1F
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t c = (uint32_t)(0x1Fu) & 31; if (c) { uint32_t r = (uint32_t)((int32_t)(int32_t)a >> c); cf = (uint32_t)((int32_t)(int32_t)a >> (c - 1)) & 1; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edx = (uint32_t)(r); } }
	// 001EC70E mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 001EC710 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC6F0, __imp__sub_001EC6F0)

X86_FUNC(__imp__sub_001EC720) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC720 mov ecx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC724 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC725 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC727 jz 0x001EC7A3
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC7A3; }
	// 001EC729 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC72A push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001EC72B mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001EC72D mov esi, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC731 test esi, 0x03
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC737 mov edi, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC73B jnz 0x001EC744
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001EC744; }
	// 001EC73D shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC740 jnz 0x001EC7B1
	if (!zf) { goto loc_001EC7B1; }
	// 001EC742 jmp 0x001EC765
	goto loc_001EC765;
loc_001EC744:
	// 001EC744 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC746 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC747 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC749 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC74A dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC74B jz 0x001EC772
	if (zf) { goto loc_001EC772; }
	// 001EC74D test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC74F jz 0x001EC77A
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC77A; }
	// 001EC751 test esi, 0x03
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC757 jnz 0x001EC744
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_001EC744; }
	// 001EC759 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001EC75B shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC75E jnz 0x001EC7B1
	if (!zf) { goto loc_001EC7B1; }
loc_001EC760:
	// 001EC760 and ebx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ebx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC763 jz 0x001EC772
	if (zf) { goto loc_001EC772; }
loc_001EC765:
	// 001EC765 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC767 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC768 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC76A inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC76B test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC76D jz 0x001EC79E
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC79E; }
	// 001EC76F dec ebx
	{ uint32_t a = (uint32_t)(ctx.ebx);  uint32_t r = (uint32_t)(a - 1); ctx.ebx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC770 jnz 0x001EC765
	if (!zf) { X86_BARRIER(); goto loc_001EC765; }
loc_001EC772:
	// 001EC772 mov eax, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC776 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC777 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC778 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC779 ret
	ctx.esp += 4; return;
loc_001EC77A:
	// 001EC77A test edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC780 jz 0x001EC794
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC794; }
loc_001EC782:
	// 001EC782 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC784 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC785 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC786 jz 0x001EC816
	if (zf) { goto loc_001EC816; }
	// 001EC78C test edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC792 jnz 0x001EC782
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_001EC782; }
loc_001EC794:
	// 001EC794 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001EC796 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC799 jnz 0x001EC807
	if (!zf) { goto loc_001EC807; }
loc_001EC79B:
	// 001EC79B mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC79D inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC79E:
	// 001EC79E dec ebx
	{ uint32_t a = (uint32_t)(ctx.ebx);  uint32_t r = (uint32_t)(a - 1); ctx.ebx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC79F jnz 0x001EC79B
	if (!zf) { X86_BARRIER(); goto loc_001EC79B; }
	// 001EC7A1 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC7A2 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
loc_001EC7A3:
	// 001EC7A3 mov eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC7A7 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC7A8 ret
	ctx.esp += 4; return;
loc_001EC7A9:
	// 001EC7A9 mov [edi], edx
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC7AB add edi, 0x04
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7AE dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7AF jz 0x001EC760
	if (zf) { X86_BARRIER(); goto loc_001EC760; }
loc_001EC7B1:
	// 001EC7B1 mov edx, 0x7EFEFEFF
	{ ctx.edx = (uint32_t)(0x7EFEFEFFu); }
	// 001EC7B6 mov eax, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC7B8 add edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7BA xor eax, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7BD xor eax, edx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7BF mov edx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC7C1 add esi, 0x04
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7C4 test eax, 0x81010100
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x81010100u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7C9 jz 0x001EC7A9
	if (((uint32_t)(ca & cb)) == 0) { X86_BARRIER(); goto loc_001EC7A9; }
	// 001EC7CB test dl, dl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(((uint8_t)ctx.edx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC7CD jz 0x001EC7FB
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC7FB; }
	// 001EC7CF test dh, dh
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.edx >> 8))), b = (uint8_t)(((uint8_t)(ctx.edx >> 8))); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001EC7D1 jz 0x001EC7F1
	if (((uint8_t)(ca & cb)) == 0) { goto loc_001EC7F1; }
	// 001EC7D3 test edx, 0xFF0000
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(0xFF0000u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7D9 jz 0x001EC7E7
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC7E7; }
	// 001EC7DB test edx, 0xFF000000
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(0xFF000000u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7E1 jnz 0x001EC7A9
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_001EC7A9; }
	// 001EC7E3 mov [edi], edx
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC7E5 jmp 0x001EC7FF
	goto loc_001EC7FF;
loc_001EC7E7:
	// 001EC7E7 and edx, 0xFFFF
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0xFFFFu); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7ED mov [edi], edx
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC7EF jmp 0x001EC7FF
	goto loc_001EC7FF;
loc_001EC7F1:
	// 001EC7F1 and edx, 0xFF
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0xFFu); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7F7 mov [edi], edx
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001EC7F9 jmp 0x001EC7FF
	goto loc_001EC7FF;
loc_001EC7FB:
	// 001EC7FB xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC7FD mov [edi], edx
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_001EC7FF:
	// 001EC7FF add edi, 0x04
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC802 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC804 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC805 jz 0x001EC811
	if (zf) { goto loc_001EC811; }
loc_001EC807:
	// 001EC807 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC809:
	// 001EC809 mov [edi], eax
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC80B add edi, 0x04
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC80E dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC80F jnz 0x001EC809
	if (!zf) { X86_BARRIER(); goto loc_001EC809; }
loc_001EC811:
	// 001EC811 and ebx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ebx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC814 jnz 0x001EC79B
	if (!zf) { X86_BARRIER(); goto loc_001EC79B; }
loc_001EC816:
	// 001EC816 mov eax, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC81A pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001EC81B pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC81C pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC81D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC720, __imp__sub_001EC720)

X86_FUNC(__imp__sub_001EC820) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC820 mov edx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC824 mov ecx, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC828 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC82A jz 0x001EC873
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC873; }
	// 001EC82C xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC82E mov al, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC832 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC833 mov edi, ecx
	{ ctx.edi = (uint32_t)(ctx.ecx); }
	// 001EC835 cmp edx, 0x04
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC838 jb 0x001EC867
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001EC867; }
	// 001EC83A neg ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(0 - a); ctx.ecx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC83C and ecx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC83F jz 0x001EC849
	if (zf) { goto loc_001EC849; }
	// 001EC841 sub edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC843:
	// 001EC843 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC845 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC846 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC847 jnz 0x001EC843
	if (!zf) { X86_BARRIER(); goto loc_001EC843; }
loc_001EC849:
	// 001EC849 mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 001EC84B shl eax, 0x08
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x8u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 001EC84E add eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC850 mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 001EC852 shl eax, 0x10
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x10u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 001EC855 add eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC857 mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
	// 001EC859 and edx, 0x03
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC85C shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC85F jz 0x001EC867
	if (zf) { goto loc_001EC867; }
	// 001EC861 rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 001EC863 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC865 jz 0x001EC86D
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001EC86D; }
loc_001EC867:
	// 001EC867 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC869 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC86A dec edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(a - 1); ctx.edx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC86B jnz 0x001EC867
	if (!zf) { X86_BARRIER(); goto loc_001EC867; }
loc_001EC86D:
	// 001EC86D mov eax, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC871 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC872 ret
	ctx.esp += 4; return;
loc_001EC873:
	// 001EC873 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC877 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC820, __imp__sub_001EC820)

X86_FUNC(__imp__sub_001EC880) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC880 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001EC881 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001EC883 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001EC884 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001EC885 mov esi, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC888 mov ecx, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EC88B mov edi, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001EC88E mov eax, ecx
	{ ctx.eax = (uint32_t)(ctx.ecx); }
	// 001EC890 mov edx, ecx
	{ ctx.edx = (uint32_t)(ctx.ecx); }
	// 001EC892 add eax, esi
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC894 cmp edi, esi
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC896 jbe 0x001EC8A0
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_001EC8A0; }
	// 001EC898 cmp edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC89A jb 0x001ECA18
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001ECA18; }
loc_001EC8A0:
	// 001EC8A0 test edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8A6 jnz 0x001EC8BC
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001EC8BC; }
	// 001EC8A8 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC8AB and edx, 0x03
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8AE cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8B1 jb 0x001EC8DC
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001EC8DC; }
	// 001EC8B3 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EC8B5 jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC8BC:
	// 001EC8BC mov eax, edi
	{ ctx.eax = (uint32_t)(ctx.edi); }
	// 001EC8BE mov edx, 0x03
	{ ctx.edx = (uint32_t)(0x3u); }
	// 001EC8C3 sub ecx, 0x04
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8C6 jb 0x001EC8D4
	if (cf) { goto loc_001EC8D4; }
	// 001EC8C8 and eax, 0x03
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8CB add ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8CD jmp [eax*4+0x1EC8E0]
	switch (X86_MEM32((uint32_t)(ctx.eax * 4u + 0x1EC8E0u))) {
	case 0x1EC8F0u: goto loc_001EC8F0;
	case 0x1EC91Cu: goto loc_001EC91C;
	case 0x1EC940u: goto loc_001EC940;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.eax * 4u + 0x1EC8E0u))); return; }
	}
loc_001EC8D4:
	// 001EC8D4 jmp [ecx*4+0x1EC9D8]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1EC9D8u))) {
	case 0x1ECA00u: goto loc_001ECA00;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9D8u: goto loc_001EC9D8;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1EC9D8u))); return; }
	}
loc_001EC8DC:
	// 001EC8DC jmp [ecx*4+0x1EC95C]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1EC95Cu))) {
	case 0x1EC9BFu: goto loc_001EC9BF;
	case 0x1EC9ACu: goto loc_001EC9AC;
	case 0x1EC9A4u: goto loc_001EC9A4;
	case 0x1EC99Cu: goto loc_001EC99C;
	case 0x1EC994u: goto loc_001EC994;
	case 0x1EC98Cu: goto loc_001EC98C;
	case 0x1EC984u: goto loc_001EC984;
	case 0x1EC97Cu: goto loc_001EC97C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1EC95Cu))); return; }
	}
loc_001EC8F0:
	// 001EC8F0 and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC8F2 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC8F4 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC8F6 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC8F9 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC8FC mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC8FF shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC902 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC905 add esi, 0x03
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC908 add edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC90B cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC90E jb 0x001EC8DC
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001EC8DC; }
	// 001EC910 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EC912 jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC91C:
	// 001EC91C and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC91E mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC920 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC922 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC925 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC928 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC92B add esi, 0x02
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC92E add edi, 0x02
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC931 cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC934 jb 0x001EC8DC
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001EC8DC; }
	// 001EC936 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EC938 jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC940:
	// 001EC940 and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC942 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC944 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC946 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC947 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC94A inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC94B cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC94E jb 0x001EC8DC
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001EC8DC; }
	// 001EC950 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EC952 jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC97C:
	// 001EC97C mov eax, [esi+ecx*4-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC980 mov [edi+ecx*4-0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC984:
	// 001EC984 mov eax, [esi+ecx*4-0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC988 mov [edi+ecx*4-0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFE8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC98C:
	// 001EC98C mov eax, [esi+ecx*4-0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFECu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC990 mov [edi+ecx*4-0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC994:
	// 001EC994 mov eax, [esi+ecx*4-0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC998 mov [edi+ecx*4-0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFF0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC99C:
	// 001EC99C mov eax, [esi+ecx*4-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFF4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9A0 mov [edi+ecx*4-0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC9A4:
	// 001EC9A4 mov eax, [esi+ecx*4-0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9A8 mov [edi+ecx*4-0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC9AC:
	// 001EC9AC mov eax, [esi+ecx*4-0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9B0 mov [edi+ecx*4-0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC9B4 lea eax, [ecx*4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx * 4u));
	// 001EC9BB add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC9BD add edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC9BF:
	// 001EC9BF jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC9D8:
	// 001EC9D8 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9DB pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC9DC pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC9DD leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001EC9DE ret
	ctx.esp += 4; return;
loc_001EC9E0:
	// 001EC9E0 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC9E2 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC9E4 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9E7 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC9E8 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC9E9 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001EC9EA ret
	ctx.esp += 4; return;
loc_001EC9EC:
	// 001EC9EC mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC9EE mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC9F0 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC9F3 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC9F6 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9F9 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC9FA pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC9FB leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001EC9FC ret
	ctx.esp += 4; return;
loc_001ECA00:
	// 001ECA00 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA02 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA04 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA07 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA0A mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA0D mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA10 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECA13 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECA14 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECA15 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECA16 ret
	ctx.esp += 4; return;
loc_001ECA18:
	// 001ECA18 lea esi, [ecx+esi*1-0x04]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ecx + ctx.esi + 0xFFFFFFFCu));
	// 001ECA1C lea edi, [ecx+edi*1-0x04]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ecx + ctx.edi + 0xFFFFFFFCu));
	// 001ECA20 test edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA26 jnz 0x001ECA4C
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001ECA4C; }
	// 001ECA28 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECA2B and edx, 0x03
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA2E cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA31 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001ECA40; }
	// 001ECA33 std
	ctx.df = 1;
	// 001ECA34 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECA36 cld
	ctx.df = 0;
	// 001ECA37 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECA40:
	// 001ECA40 neg ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(0 - a); ctx.ecx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA42 jmp [ecx*4+0x1ECB10]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))) {
	case 0x1ECB57u: goto loc_001ECB57;
	case 0x1ECB44u: goto loc_001ECB44;
	case 0x1ECB3Cu: goto loc_001ECB3C;
	case 0x1ECB34u: goto loc_001ECB34;
	case 0x1ECB2Cu: goto loc_001ECB2C;
	case 0x1ECB24u: goto loc_001ECB24;
	case 0x1ECB1Cu: goto loc_001ECB1C;
	case 0x1ECB14u: goto loc_001ECB14;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))); return; }
	}
loc_001ECA4C:
	// 001ECA4C mov eax, edi
	{ ctx.eax = (uint32_t)(ctx.edi); }
	// 001ECA4E mov edx, 0x03
	{ ctx.edx = (uint32_t)(0x3u); }
	// 001ECA53 cmp ecx, 0x04
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA56 jb 0x001ECA64
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001ECA64; }
	// 001ECA58 and eax, 0x03
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA5B sub ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA5D jmp [eax*4+0x1ECA68]
	switch (X86_MEM32((uint32_t)(ctx.eax * 4u + 0x1ECA68u))) {
	case 0x1ECA78u: goto loc_001ECA78;
	case 0x1ECA98u: goto loc_001ECA98;
	case 0x1ECAC0u: goto loc_001ECAC0;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.eax * 4u + 0x1ECA68u))); return; }
	}
loc_001ECA64:
	// 001ECA64 jmp [ecx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECA78:
	// 001ECA78 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA7B and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA7D mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA80 dec esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a - 1); ctx.esi = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA81 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECA84 dec edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a - 1); ctx.edi = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA85 cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA88 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECA8A std
	ctx.df = 1;
	// 001ECA8B rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECA8D cld
	ctx.df = 0;
	// 001ECA8E jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECA98:
	// 001ECA98 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA9B and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA9D mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAA0 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAA3 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECAA6 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAA9 sub esi, 0x02
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAAC sub edi, 0x02
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ctx.edi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAAF cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAB2 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECAB4 std
	ctx.df = 1;
	// 001ECAB5 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECAB7 cld
	ctx.df = 0;
	// 001ECAB8 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECAC0:
	// 001ECAC0 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAC3 and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAC5 mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAC8 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECACB mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECACE mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAD1 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECAD4 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAD7 sub esi, 0x03
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECADA sub edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ctx.edi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECADD cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAE0 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECAE6 std
	ctx.df = 1;
	// 001ECAE7 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECAE9 cld
	ctx.df = 0;
	// 001ECAEA jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB14:
	// 001ECB14 mov eax, [esi+ecx*4+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB18 mov [edi+ecx*4+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB1C:
	// 001ECB1C mov eax, [esi+ecx*4+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB20 mov [edi+ecx*4+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB24:
	// 001ECB24 mov eax, [esi+ecx*4+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB28 mov [edi+ecx*4+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB2C:
	// 001ECB2C mov eax, [esi+ecx*4+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB30 mov [edi+ecx*4+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB34:
	// 001ECB34 mov eax, [esi+ecx*4+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB38 mov [edi+ecx*4+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB3C:
	// 001ECB3C mov eax, [esi+ecx*4+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB40 mov [edi+ecx*4+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB44:
	// 001ECB44 mov eax, [esi+ecx*4+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB48 mov [edi+ecx*4+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001ECB4C lea eax, [ecx*4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx * 4u));
	// 001ECB53 add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECB55 add edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001ECB57:
	// 001ECB57 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB70:
	// 001ECB70 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB73 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB74 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB75 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB76 ret
	ctx.esp += 4; return;
loc_001ECB78:
	// 001ECB78 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB7B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB7E mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB81 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB82 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB83 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB84 ret
	ctx.esp += 4; return;
loc_001ECB88:
	// 001ECB88 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB8B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB8E mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB91 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB94 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB97 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB98 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB99 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB9A ret
	ctx.esp += 4; return;
loc_001ECB9C:
	// 001ECB9C mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB9F mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA2 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBA5 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA8 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBAB mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBAE mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBB1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECBB2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECBB3 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECBB4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC880, __imp__sub_001EC880)

X86_FUNC(__imp__sub_001EC919) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_001EC919;
loc_001EC8DC:
	// 001EC8DC jmp [ecx*4+0x1EC95C]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1EC95Cu))) {
	case 0x1EC9BFu: goto loc_001EC9BF;
	case 0x1EC9ACu: goto loc_001EC9AC;
	case 0x1EC9A4u: goto loc_001EC9A4;
	case 0x1EC99Cu: goto loc_001EC99C;
	case 0x1EC994u: goto loc_001EC994;
	case 0x1EC98Cu: goto loc_001EC98C;
	case 0x1EC984u: goto loc_001EC984;
	case 0x1EC97Cu: goto loc_001EC97C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1EC95Cu))); return; }
	}
loc_001EC919:
	// 001EC919 lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
	// 001EC91C and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC91E mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC920 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC922 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC925 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001EC928 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC92B add esi, 0x02
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC92E add edi, 0x02
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC931 cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC934 jb 0x001EC8DC
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001EC8DC; }
	// 001EC936 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001EC938 jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC97C:
	// 001EC97C mov eax, [esi+ecx*4-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC980 mov [edi+ecx*4-0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC984:
	// 001EC984 mov eax, [esi+ecx*4-0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC988 mov [edi+ecx*4-0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFE8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC98C:
	// 001EC98C mov eax, [esi+ecx*4-0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFECu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC990 mov [edi+ecx*4-0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC994:
	// 001EC994 mov eax, [esi+ecx*4-0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC998 mov [edi+ecx*4-0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFF0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC99C:
	// 001EC99C mov eax, [esi+ecx*4-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFF4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9A0 mov [edi+ecx*4-0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC9A4:
	// 001EC9A4 mov eax, [esi+ecx*4-0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9A8 mov [edi+ecx*4-0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001EC9AC:
	// 001EC9AC mov eax, [esi+ecx*4-0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9B0 mov [edi+ecx*4-0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001EC9B4 lea eax, [ecx*4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx * 4u));
	// 001EC9BB add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EC9BD add edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001EC9BF:
	// 001EC9BF jmp [edx*4+0x1EC9C8]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))) {
	case 0x1EC9D8u: goto loc_001EC9D8;
	case 0x1EC9E0u: goto loc_001EC9E0;
	case 0x1EC9ECu: goto loc_001EC9EC;
	case 0x1ECA00u: goto loc_001ECA00;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1EC9C8u))); return; }
	}
loc_001EC9D8:
	// 001EC9D8 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9DB pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC9DC pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC9DD leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001EC9DE ret
	ctx.esp += 4; return;
loc_001EC9E0:
	// 001EC9E0 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC9E2 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC9E4 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9E7 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC9E8 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC9E9 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001EC9EA ret
	ctx.esp += 4; return;
loc_001EC9EC:
	// 001EC9EC mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC9EE mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC9F0 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001EC9F3 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001EC9F6 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EC9F9 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001EC9FA pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001EC9FB leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001EC9FC ret
	ctx.esp += 4; return;
loc_001ECA00:
	// 001ECA00 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA02 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA04 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA07 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA0A mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA0D mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA10 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECA13 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECA14 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECA15 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECA16 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC919, __imp__sub_001EC919)

X86_FUNC(__imp__sub_001EC9FD) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EC9FD lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
	// 001ECA00 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA02 mov [edi], al
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA04 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA07 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA0A mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA0D mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA10 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECA13 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECA14 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECA15 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECA16 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EC9FD, __imp__sub_001EC9FD)

X86_FUNC(__imp__sub_001ECA3E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECA3E mov edi, edi
	{ ctx.edi = (uint32_t)(ctx.edi); }
	// 001ECA40 neg ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(0 - a); ctx.ecx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA42 jmp [ecx*4+0x1ECB10]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))) {
	case 0x1ECB57u: goto loc_001ECB57;
	case 0x1ECB44u: goto loc_001ECB44;
	case 0x1ECB3Cu: goto loc_001ECB3C;
	case 0x1ECB34u: goto loc_001ECB34;
	case 0x1ECB2Cu: goto loc_001ECB2C;
	case 0x1ECB24u: goto loc_001ECB24;
	case 0x1ECB1Cu: goto loc_001ECB1C;
	case 0x1ECB14u: goto loc_001ECB14;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))); return; }
	}
loc_001ECB14:
	// 001ECB14 mov eax, [esi+ecx*4+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB18 mov [edi+ecx*4+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB1C:
	// 001ECB1C mov eax, [esi+ecx*4+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB20 mov [edi+ecx*4+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB24:
	// 001ECB24 mov eax, [esi+ecx*4+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB28 mov [edi+ecx*4+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB2C:
	// 001ECB2C mov eax, [esi+ecx*4+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB30 mov [edi+ecx*4+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB34:
	// 001ECB34 mov eax, [esi+ecx*4+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB38 mov [edi+ecx*4+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB3C:
	// 001ECB3C mov eax, [esi+ecx*4+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB40 mov [edi+ecx*4+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB44:
	// 001ECB44 mov eax, [esi+ecx*4+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB48 mov [edi+ecx*4+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001ECB4C lea eax, [ecx*4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx * 4u));
	// 001ECB53 add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECB55 add edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001ECB57:
	// 001ECB57 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB70:
	// 001ECB70 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB73 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB74 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB75 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB76 ret
	ctx.esp += 4; return;
loc_001ECB78:
	// 001ECB78 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB7B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB7E mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB81 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB82 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB83 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB84 ret
	ctx.esp += 4; return;
loc_001ECB88:
	// 001ECB88 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB8B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB8E mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB91 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB94 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB97 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB98 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB99 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB9A ret
	ctx.esp += 4; return;
loc_001ECB9C:
	// 001ECB9C mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB9F mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA2 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBA5 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA8 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBAB mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBAE mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBB1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECBB2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECBB3 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECBB4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECA3E, __imp__sub_001ECA3E)

X86_FUNC(__imp__sub_001ECA49) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_001ECA49;
loc_001ECA40:
	// 001ECA40 neg ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(0 - a); ctx.ecx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA42 jmp [ecx*4+0x1ECB10]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))) {
	case 0x1ECB57u: goto loc_001ECB57;
	case 0x1ECB44u: goto loc_001ECB44;
	case 0x1ECB3Cu: goto loc_001ECB3C;
	case 0x1ECB34u: goto loc_001ECB34;
	case 0x1ECB2Cu: goto loc_001ECB2C;
	case 0x1ECB24u: goto loc_001ECB24;
	case 0x1ECB1Cu: goto loc_001ECB1C;
	case 0x1ECB14u: goto loc_001ECB14;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))); return; }
	}
loc_001ECA49:
	// 001ECA49 lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
	// 001ECA4C mov eax, edi
	{ ctx.eax = (uint32_t)(ctx.edi); }
	// 001ECA4E mov edx, 0x03
	{ ctx.edx = (uint32_t)(0x3u); }
	// 001ECA53 cmp ecx, 0x04
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA56 jb 0x001ECA64
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001ECA64; }
	// 001ECA58 and eax, 0x03
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA5B sub ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA5D jmp [eax*4+0x1ECA68]
	switch (X86_MEM32((uint32_t)(ctx.eax * 4u + 0x1ECA68u))) {
	case 0x1ECA78u: goto loc_001ECA78;
	case 0x1ECA98u: goto loc_001ECA98;
	case 0x1ECAC0u: goto loc_001ECAC0;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.eax * 4u + 0x1ECA68u))); return; }
	}
loc_001ECA64:
	// 001ECA64 jmp [ecx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECA78:
	// 001ECA78 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA7B and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA7D mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECA80 dec esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a - 1); ctx.esi = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA81 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECA84 dec edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a - 1); ctx.edi = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA85 cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA88 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECA8A std
	ctx.df = 1;
	// 001ECA8B rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECA8D cld
	ctx.df = 0;
	// 001ECA8E jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECA98:
	// 001ECA98 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA9B and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA9D mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAA0 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAA3 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECAA6 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAA9 sub esi, 0x02
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAAC sub edi, 0x02
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ctx.edi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAAF cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAB2 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECAB4 std
	ctx.df = 1;
	// 001ECAB5 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECAB7 cld
	ctx.df = 0;
	// 001ECAB8 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECAC0:
	// 001ECAC0 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAC3 and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAC5 mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAC8 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECACB mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECACE mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAD1 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECAD4 mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAD7 sub esi, 0x03
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECADA sub edi, 0x03
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ctx.edi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECADD cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAE0 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECAE6 std
	ctx.df = 1;
	// 001ECAE7 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECAE9 cld
	ctx.df = 0;
	// 001ECAEA jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB14:
	// 001ECB14 mov eax, [esi+ecx*4+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB18 mov [edi+ecx*4+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB1C:
	// 001ECB1C mov eax, [esi+ecx*4+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB20 mov [edi+ecx*4+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB24:
	// 001ECB24 mov eax, [esi+ecx*4+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB28 mov [edi+ecx*4+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB2C:
	// 001ECB2C mov eax, [esi+ecx*4+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB30 mov [edi+ecx*4+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB34:
	// 001ECB34 mov eax, [esi+ecx*4+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB38 mov [edi+ecx*4+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB3C:
	// 001ECB3C mov eax, [esi+ecx*4+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB40 mov [edi+ecx*4+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB44:
	// 001ECB44 mov eax, [esi+ecx*4+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB48 mov [edi+ecx*4+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001ECB4C lea eax, [ecx*4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx * 4u));
	// 001ECB53 add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECB55 add edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001ECB57:
	// 001ECB57 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB70:
	// 001ECB70 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB73 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB74 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB75 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB76 ret
	ctx.esp += 4; return;
loc_001ECB78:
	// 001ECB78 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB7B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB7E mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB81 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB82 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB83 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB84 ret
	ctx.esp += 4; return;
loc_001ECB88:
	// 001ECB88 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB8B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB8E mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB91 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB94 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB97 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB98 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB99 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB9A ret
	ctx.esp += 4; return;
loc_001ECB9C:
	// 001ECB9C mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB9F mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA2 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBA5 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA8 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBAB mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBAE mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBB1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECBB2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECBB3 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECBB4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECA49, __imp__sub_001ECA49)

X86_FUNC(__imp__sub_001ECA95) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_001ECA95;
loc_001ECA40:
	// 001ECA40 neg ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(0 - a); ctx.ecx = (uint32_t)(r); cf = a != 0; of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA42 jmp [ecx*4+0x1ECB10]
	switch (X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))) {
	case 0x1ECB57u: goto loc_001ECB57;
	case 0x1ECB44u: goto loc_001ECB44;
	case 0x1ECB3Cu: goto loc_001ECB3C;
	case 0x1ECB34u: goto loc_001ECB34;
	case 0x1ECB2Cu: goto loc_001ECB2C;
	case 0x1ECB24u: goto loc_001ECB24;
	case 0x1ECB1Cu: goto loc_001ECB1C;
	case 0x1ECB14u: goto loc_001ECB14;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.ecx * 4u + 0x1ECB10u))); return; }
	}
loc_001ECA95:
	// 001ECA95 lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
	// 001ECA98 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECA9B and edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECA9D mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAA0 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECAA3 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001ECAA6 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECAA9 sub esi, 0x02
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAAC sub edi, 0x02
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a - b); ctx.edi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAAF cmp ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECAB2 jb 0x001ECA40
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001ECA40; }
	// 001ECAB4 std
	ctx.df = 1;
	// 001ECAB5 rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001ECAB7 cld
	ctx.df = 0;
	// 001ECAB8 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB14:
	// 001ECB14 mov eax, [esi+ecx*4+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB18 mov [edi+ecx*4+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB1C:
	// 001ECB1C mov eax, [esi+ecx*4+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB20 mov [edi+ecx*4+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB24:
	// 001ECB24 mov eax, [esi+ecx*4+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB28 mov [edi+ecx*4+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB2C:
	// 001ECB2C mov eax, [esi+ecx*4+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB30 mov [edi+ecx*4+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB34:
	// 001ECB34 mov eax, [esi+ecx*4+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB38 mov [edi+ecx*4+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB3C:
	// 001ECB3C mov eax, [esi+ecx*4+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB40 mov [edi+ecx*4+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_001ECB44:
	// 001ECB44 mov eax, [esi+ecx*4+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx * 4u + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB48 mov [edi+ecx*4+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.ecx * 4u + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001ECB4C lea eax, [ecx*4]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx * 4u));
	// 001ECB53 add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECB55 add edi, eax
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001ECB57:
	// 001ECB57 jmp [edx*4+0x1ECB60]
	switch (X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))) {
	case 0x1ECB70u: goto loc_001ECB70;
	case 0x1ECB78u: goto loc_001ECB78;
	case 0x1ECB88u: goto loc_001ECB88;
	case 0x1ECB9Cu: goto loc_001ECB9C;
	default: { X86_SAVE_FLAGS(); x86_icall(ctx, base, X86_MEM32((uint32_t)(ctx.edx * 4u + 0x1ECB60u))); return; }
	}
loc_001ECB70:
	// 001ECB70 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB73 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB74 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB75 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB76 ret
	ctx.esp += 4; return;
loc_001ECB78:
	// 001ECB78 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB7B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB7E mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB81 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB82 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB83 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB84 ret
	ctx.esp += 4; return;
loc_001ECB88:
	// 001ECB88 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB8B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB8E mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB91 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB94 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB97 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB98 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB99 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB9A ret
	ctx.esp += 4; return;
loc_001ECB9C:
	// 001ECB9C mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB9F mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA2 mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBA5 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBA8 mov al, [esi+0x01]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECBAB mov [edi+0x01], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECBAE mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECBB1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECBB2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECBB3 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECBB4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECA95, __imp__sub_001ECA95)

X86_FUNC(__imp__sub_001ECB85) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECB85 lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
	// 001ECB88 mov al, [esi+0x03]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB8B mov [edi+0x03], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x3u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB8E mov al, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001ECB91 mov [edi+0x02], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 001ECB94 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECB97 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001ECB98 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001ECB99 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001ECB9A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECB85, __imp__sub_001ECB85)

X86_FUNC(__imp__sub_001ECC20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC20 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECC23 jmp 0x00011A90
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00011A90(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC20, __imp__sub_001ECC20)

X86_FUNC(__imp__sub_001ECC28) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC28 mov eax, 0x27D2E4
	{ ctx.eax = (uint32_t)(0x27D2E4u); }
	// 001ECC2D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC28, __imp__sub_001ECC28)

X86_FUNC(__imp__sub_001ECC40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC40 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECC43 jmp 0x00011B70
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00011B70(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC40, __imp__sub_001ECC40)

X86_FUNC(__imp__sub_001ECC48) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC48 mov eax, 0x27D398
	{ ctx.eax = (uint32_t)(0x27D398u); }
	// 001ECC4D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC48, __imp__sub_001ECC48)

X86_FUNC(__imp__sub_001ECC60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC60 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECC63 add ecx, 0x1C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECC66 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC60, __imp__sub_001ECC60)

X86_FUNC(__imp__sub_001ECC6B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC6B mov eax, 0x27D3BC
	{ ctx.eax = (uint32_t)(0x27D3BCu); }
	// 001ECC70 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC6B, __imp__sub_001ECC6B)

X86_FUNC(__imp__sub_001ECC80) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC80 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECC83 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC80, __imp__sub_001ECC80)

X86_FUNC(__imp__sub_001ECC88) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC88 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECC8B add ecx, 0xA4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xA4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECC91 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC88, __imp__sub_001ECC88)

X86_FUNC(__imp__sub_001ECC96) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECC96 mov eax, 0x27D40C
	{ ctx.eax = (uint32_t)(0x27D40Cu); }
	// 001ECC9B jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECC96, __imp__sub_001ECC96)

X86_FUNC(__imp__sub_001ECCA0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECCA0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECCA3 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECCA0, __imp__sub_001ECCA0)

X86_FUNC(__imp__sub_001ECCA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECCA8 mov eax, 0x27D430
	{ ctx.eax = (uint32_t)(0x27D430u); }
	// 001ECCAD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECCA8, __imp__sub_001ECCA8)

X86_FUNC(__imp__sub_001ECCC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECCC0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECCC5 push 0x5B
	{ uint32_t v = (uint32_t)(0x5Bu); X86_PUSH32(v); }
	// 001ECCC7 push 0x25397C
	{ uint32_t v = (uint32_t)(0x25397Cu); X86_PUSH32(v); }
	// 001ECCCC mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECCCF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECCD0 call 0x000127D0
	X86_PUSH32(0x1ECCD5u); sub_000127D0(ctx, base);
	// 001ECCD5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECCD8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECCC0, __imp__sub_001ECCC0)

X86_FUNC(__imp__sub_001ECCD9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECCD9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECCDE push 0x64
	{ uint32_t v = (uint32_t)(0x64u); X86_PUSH32(v); }
	// 001ECCE0 push 0x25397C
	{ uint32_t v = (uint32_t)(0x25397Cu); X86_PUSH32(v); }
	// 001ECCE5 mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECCE8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECCE9 call 0x000127D0
	X86_PUSH32(0x1ECCEEu); sub_000127D0(ctx, base);
	// 001ECCEE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECCF1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECCD9, __imp__sub_001ECCD9)

X86_FUNC(__imp__sub_001ECCF2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECCF2 mov eax, 0x27D45C
	{ ctx.eax = (uint32_t)(0x27D45Cu); }
	// 001ECCF7 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECCF2, __imp__sub_001ECCF2)

X86_FUNC(__imp__sub_001ECD00) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD00 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD03 jmp 0x00011A90
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00011A90(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD00, __imp__sub_001ECD00)

X86_FUNC(__imp__sub_001ECD08) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD08 mov eax, 0x27D480
	{ ctx.eax = (uint32_t)(0x27D480u); }
	// 001ECD0D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD08, __imp__sub_001ECD08)

X86_FUNC(__imp__sub_001ECD20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD20 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD23 jmp 0x00011A90
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00011A90(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD20, __imp__sub_001ECD20)

X86_FUNC(__imp__sub_001ECD28) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD28 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD2B add ecx, 0x10
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECD2E jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD28, __imp__sub_001ECD28)

X86_FUNC(__imp__sub_001ECD33) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD33 mov eax, 0x27D4AC
	{ ctx.eax = (uint32_t)(0x27D4ACu); }
	// 001ECD38 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD33, __imp__sub_001ECD33)

X86_FUNC(__imp__sub_001ECD40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD40 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD43 jmp 0x00012860
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00012860(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD40, __imp__sub_001ECD40)

X86_FUNC(__imp__sub_001ECD48) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD48 mov eax, 0x27D4D0
	{ ctx.eax = (uint32_t)(0x27D4D0u); }
	// 001ECD4D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD48, __imp__sub_001ECD48)

X86_FUNC(__imp__sub_001ECD60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD60 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD63 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD60, __imp__sub_001ECD60)

X86_FUNC(__imp__sub_001ECD68) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD68 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD6B add ecx, 0x1A0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1A0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECD71 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD68, __imp__sub_001ECD68)

X86_FUNC(__imp__sub_001ECD76) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD76 mov eax, 0x27D54C
	{ ctx.eax = (uint32_t)(0x27D54Cu); }
	// 001ECD7B jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECD76, __imp__sub_001ECD76)

X86_FUNC(__imp__sub_001ECD80) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD80 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECD85 push 0x1E1
	{ uint32_t v = (uint32_t)(0x1E1u); X86_PUSH32(v); }
	// 001ECD8A push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECD8F mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECD95 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECD96 call 0x000127D0
	X86_PUSH32(0x1ECD9Bu); sub_000127D0(ctx, base);
	// 001ECD9B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECD9E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECD80, __imp__sub_001ECD80)

X86_FUNC(__imp__sub_001ECD9F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECD9F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECDA4 push 0x20A
	{ uint32_t v = (uint32_t)(0x20Au); X86_PUSH32(v); }
	// 001ECDA9 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECDAE mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECDB4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECDB5 call 0x000127D0
	X86_PUSH32(0x1ECDBAu); sub_000127D0(ctx, base);
	// 001ECDBA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECDBD ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECD9F, __imp__sub_001ECD9F)

X86_FUNC(__imp__sub_001ECDBE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECDBE push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECDC3 push 0x21E
	{ uint32_t v = (uint32_t)(0x21Eu); X86_PUSH32(v); }
	// 001ECDC8 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECDCD mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECDD3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECDD4 call 0x000127D0
	X86_PUSH32(0x1ECDD9u); sub_000127D0(ctx, base);
	// 001ECDD9 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECDDC ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECDBE, __imp__sub_001ECDBE)

X86_FUNC(__imp__sub_001ECDDD) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECDDD push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECDE2 push 0x227
	{ uint32_t v = (uint32_t)(0x227u); X86_PUSH32(v); }
	// 001ECDE7 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECDEC mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECDF2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECDF3 call 0x000127D0
	X86_PUSH32(0x1ECDF8u); sub_000127D0(ctx, base);
	// 001ECDF8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECDFB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECDDD, __imp__sub_001ECDDD)

X86_FUNC(__imp__sub_001ECDFC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECDFC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECE01 push 0x231
	{ uint32_t v = (uint32_t)(0x231u); X86_PUSH32(v); }
	// 001ECE06 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECE0B mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECE11 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECE12 call 0x000127D0
	X86_PUSH32(0x1ECE17u); sub_000127D0(ctx, base);
	// 001ECE17 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECE1A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECDFC, __imp__sub_001ECDFC)

X86_FUNC(__imp__sub_001ECE1B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECE1B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECE20 push 0x232
	{ uint32_t v = (uint32_t)(0x232u); X86_PUSH32(v); }
	// 001ECE25 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECE2A mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECE30 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECE31 call 0x000127D0
	X86_PUSH32(0x1ECE36u); sub_000127D0(ctx, base);
	// 001ECE36 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECE39 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECE1B, __imp__sub_001ECE1B)

X86_FUNC(__imp__sub_001ECE3A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECE3A push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECE3F push 0x237
	{ uint32_t v = (uint32_t)(0x237u); X86_PUSH32(v); }
	// 001ECE44 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECE49 mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECE4F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECE50 call 0x000127D0
	X86_PUSH32(0x1ECE55u); sub_000127D0(ctx, base);
	// 001ECE55 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECE58 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECE3A, __imp__sub_001ECE3A)

X86_FUNC(__imp__sub_001ECE59) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECE59 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECE5E push 0x24E
	{ uint32_t v = (uint32_t)(0x24Eu); X86_PUSH32(v); }
	// 001ECE63 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECE68 mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECE6E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECE6F call 0x000127D0
	X86_PUSH32(0x1ECE74u); sub_000127D0(ctx, base);
	// 001ECE74 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECE77 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECE59, __imp__sub_001ECE59)

X86_FUNC(__imp__sub_001ECE78) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECE78 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECE7D push 0x267
	{ uint32_t v = (uint32_t)(0x267u); X86_PUSH32(v); }
	// 001ECE82 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECE87 mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECE8D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECE8E call 0x000127D0
	X86_PUSH32(0x1ECE93u); sub_000127D0(ctx, base);
	// 001ECE93 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECE96 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECE78, __imp__sub_001ECE78)

X86_FUNC(__imp__sub_001ECE97) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECE97 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECE9C push 0x287
	{ uint32_t v = (uint32_t)(0x287u); X86_PUSH32(v); }
	// 001ECEA1 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECEA6 mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECEAC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECEAD call 0x000127D0
	X86_PUSH32(0x1ECEB2u); sub_000127D0(ctx, base);
	// 001ECEB2 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECEB5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECE97, __imp__sub_001ECE97)

X86_FUNC(__imp__sub_001ECEB6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECEB6 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECEBB push 0x292
	{ uint32_t v = (uint32_t)(0x292u); X86_PUSH32(v); }
	// 001ECEC0 push 0x253EF4
	{ uint32_t v = (uint32_t)(0x253EF4u); X86_PUSH32(v); }
	// 001ECEC5 mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECECB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECECC call 0x000127D0
	X86_PUSH32(0x1ECED1u); sub_000127D0(ctx, base);
	// 001ECED1 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECED4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECEB6, __imp__sub_001ECEB6)

X86_FUNC(__imp__sub_001ECED5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECED5 mov eax, 0x27D5C0
	{ ctx.eax = (uint32_t)(0x27D5C0u); }
	// 001ECEDA jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECED5, __imp__sub_001ECED5)

X86_FUNC(__imp__sub_001ECEE0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECEE0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECEE5 push 0x117
	{ uint32_t v = (uint32_t)(0x117u); X86_PUSH32(v); }
	// 001ECEEA push 0x254E9C
	{ uint32_t v = (uint32_t)(0x254E9Cu); X86_PUSH32(v); }
	// 001ECEEF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECEF2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECEF3 call 0x000127D0
	X86_PUSH32(0x1ECEF8u); sub_000127D0(ctx, base);
	// 001ECEF8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECEFB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECEE0, __imp__sub_001ECEE0)

X86_FUNC(__imp__sub_001ECEFC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECEFC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECF01 push 0x124
	{ uint32_t v = (uint32_t)(0x124u); X86_PUSH32(v); }
	// 001ECF06 push 0x254E9C
	{ uint32_t v = (uint32_t)(0x254E9Cu); X86_PUSH32(v); }
	// 001ECF0B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECF0E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECF0F call 0x000127D0
	X86_PUSH32(0x1ECF14u); sub_000127D0(ctx, base);
	// 001ECF14 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECF17 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECEFC, __imp__sub_001ECEFC)

X86_FUNC(__imp__sub_001ECF18) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF18 mov eax, 0x27D5EC
	{ ctx.eax = (uint32_t)(0x27D5ECu); }
	// 001ECF1D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECF18, __imp__sub_001ECF18)

X86_FUNC(__imp__sub_001ECF30) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF30 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ECF33 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECF30, __imp__sub_001ECF30)

X86_FUNC(__imp__sub_001ECF38) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF38 mov eax, 0x27D610
	{ ctx.eax = (uint32_t)(0x27D610u); }
	// 001ECF3D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECF38, __imp__sub_001ECF38)

X86_FUNC(__imp__sub_001ECF50) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF50 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECF55 push 0x2C
	{ uint32_t v = (uint32_t)(0x2Cu); X86_PUSH32(v); }
	// 001ECF57 push 0x254F7C
	{ uint32_t v = (uint32_t)(0x254F7Cu); X86_PUSH32(v); }
	// 001ECF5C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECF5F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECF60 call 0x000127D0
	X86_PUSH32(0x1ECF65u); sub_000127D0(ctx, base);
	// 001ECF65 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECF68 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECF50, __imp__sub_001ECF50)

X86_FUNC(__imp__sub_001ECF69) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF69 mov eax, 0x27D658
	{ ctx.eax = (uint32_t)(0x27D658u); }
	// 001ECF6E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ECF69, __imp__sub_001ECF69)

X86_FUNC(__imp__sub_001ECF80) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF80 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECF85 push 0x46
	{ uint32_t v = (uint32_t)(0x46u); X86_PUSH32(v); }
	// 001ECF87 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ECF8C mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECF8F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECF90 call 0x000127D0
	X86_PUSH32(0x1ECF95u); sub_000127D0(ctx, base);
	// 001ECF95 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECF98 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECF80, __imp__sub_001ECF80)

X86_FUNC(__imp__sub_001ECF99) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECF99 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECF9E push 0x51
	{ uint32_t v = (uint32_t)(0x51u); X86_PUSH32(v); }
	// 001ECFA0 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ECFA5 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECFA8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECFA9 call 0x000127D0
	X86_PUSH32(0x1ECFAEu); sub_000127D0(ctx, base);
	// 001ECFAE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECFB1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECF99, __imp__sub_001ECF99)

X86_FUNC(__imp__sub_001ECFB2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECFB2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECFB7 push 0x6E
	{ uint32_t v = (uint32_t)(0x6Eu); X86_PUSH32(v); }
	// 001ECFB9 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ECFBE mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECFC1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECFC2 call 0x000127D0
	X86_PUSH32(0x1ECFC7u); sub_000127D0(ctx, base);
	// 001ECFC7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECFCA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECFB2, __imp__sub_001ECFB2)

X86_FUNC(__imp__sub_001ECFCB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECFCB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECFD0 push 0x82
	{ uint32_t v = (uint32_t)(0x82u); X86_PUSH32(v); }
	// 001ECFD5 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ECFDA mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECFDD push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECFDE call 0x000127D0
	X86_PUSH32(0x1ECFE3u); sub_000127D0(ctx, base);
	// 001ECFE3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ECFE6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECFCB, __imp__sub_001ECFCB)

X86_FUNC(__imp__sub_001ECFE7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ECFE7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ECFEC push 0xAE
	{ uint32_t v = (uint32_t)(0xAEu); X86_PUSH32(v); }
	// 001ECFF1 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ECFF6 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ECFF9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ECFFA call 0x000127D0
	X86_PUSH32(0x1ECFFFu); sub_000127D0(ctx, base);
	// 001ECFFF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED002 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ECFE7, __imp__sub_001ECFE7)

X86_FUNC(__imp__sub_001ED003) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED003 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED008 push 0xB5
	{ uint32_t v = (uint32_t)(0xB5u); X86_PUSH32(v); }
	// 001ED00D push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ED012 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED015 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED016 call 0x000127D0
	X86_PUSH32(0x1ED01Bu); sub_000127D0(ctx, base);
	// 001ED01B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED01E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED003, __imp__sub_001ED003)

X86_FUNC(__imp__sub_001ED01F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED01F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED024 push 0xC2
	{ uint32_t v = (uint32_t)(0xC2u); X86_PUSH32(v); }
	// 001ED029 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ED02E mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED031 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED032 call 0x000127D0
	X86_PUSH32(0x1ED037u); sub_000127D0(ctx, base);
	// 001ED037 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED03A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED01F, __imp__sub_001ED01F)

X86_FUNC(__imp__sub_001ED03B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED03B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED040 push 0xCD
	{ uint32_t v = (uint32_t)(0xCDu); X86_PUSH32(v); }
	// 001ED045 push 0x25501C
	{ uint32_t v = (uint32_t)(0x25501Cu); X86_PUSH32(v); }
	// 001ED04A mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED04D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED04E call 0x000127D0
	X86_PUSH32(0x1ED053u); sub_000127D0(ctx, base);
	// 001ED053 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED056 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED03B, __imp__sub_001ED03B)

X86_FUNC(__imp__sub_001ED057) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED057 mov eax, 0x27D6B4
	{ ctx.eax = (uint32_t)(0x27D6B4u); }
	// 001ED05C jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED057, __imp__sub_001ED057)

X86_FUNC(__imp__sub_001ED070) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED070 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED073 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED070, __imp__sub_001ED070)

X86_FUNC(__imp__sub_001ED078) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED078 mov eax, 0x27D6D8
	{ ctx.eax = (uint32_t)(0x27D6D8u); }
	// 001ED07D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED078, __imp__sub_001ED078)

X86_FUNC(__imp__sub_001ED090) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED090 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED095 push 0x54
	{ uint32_t v = (uint32_t)(0x54u); X86_PUSH32(v); }
	// 001ED097 push 0x2550F4
	{ uint32_t v = (uint32_t)(0x2550F4u); X86_PUSH32(v); }
	// 001ED09C mov eax, [ebp-0x34]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED09F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED0A0 call 0x000127D0
	X86_PUSH32(0x1ED0A5u); sub_000127D0(ctx, base);
	// 001ED0A5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED0A8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED090, __imp__sub_001ED090)

X86_FUNC(__imp__sub_001ED0A9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED0A9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED0AE push 0x6A
	{ uint32_t v = (uint32_t)(0x6Au); X86_PUSH32(v); }
	// 001ED0B0 push 0x2550F4
	{ uint32_t v = (uint32_t)(0x2550F4u); X86_PUSH32(v); }
	// 001ED0B5 mov eax, [ebp-0x34]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED0B8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED0B9 call 0x000127D0
	X86_PUSH32(0x1ED0BEu); sub_000127D0(ctx, base);
	// 001ED0BE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED0C1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED0A9, __imp__sub_001ED0A9)

X86_FUNC(__imp__sub_001ED0C2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED0C2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED0C7 push 0x75
	{ uint32_t v = (uint32_t)(0x75u); X86_PUSH32(v); }
	// 001ED0C9 push 0x2550F4
	{ uint32_t v = (uint32_t)(0x2550F4u); X86_PUSH32(v); }
	// 001ED0CE mov eax, [ebp-0x34]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED0D1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED0D2 call 0x000127D0
	X86_PUSH32(0x1ED0D7u); sub_000127D0(ctx, base);
	// 001ED0D7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED0DA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED0C2, __imp__sub_001ED0C2)

X86_FUNC(__imp__sub_001ED0DB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED0DB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED0E0 push 0x7B
	{ uint32_t v = (uint32_t)(0x7Bu); X86_PUSH32(v); }
	// 001ED0E2 push 0x2550F4
	{ uint32_t v = (uint32_t)(0x2550F4u); X86_PUSH32(v); }
	// 001ED0E7 mov eax, [ebp-0x34]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED0EA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED0EB call 0x000127D0
	X86_PUSH32(0x1ED0F0u); sub_000127D0(ctx, base);
	// 001ED0F0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED0F3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED0DB, __imp__sub_001ED0DB)

X86_FUNC(__imp__sub_001ED0F4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED0F4 mov eax, 0x27D738
	{ ctx.eax = (uint32_t)(0x27D738u); }
	// 001ED0F9 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED0F4, __imp__sub_001ED0F4)

X86_FUNC(__imp__sub_001ED100) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED100 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED103 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED100, __imp__sub_001ED100)

X86_FUNC(__imp__sub_001ED108) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED108 mov eax, 0x27D75C
	{ ctx.eax = (uint32_t)(0x27D75Cu); }
	// 001ED10D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED108, __imp__sub_001ED108)

X86_FUNC(__imp__sub_001ED120) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED120 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED125 push 0x173
	{ uint32_t v = (uint32_t)(0x173u); X86_PUSH32(v); }
	// 001ED12A push 0x25534C
	{ uint32_t v = (uint32_t)(0x25534Cu); X86_PUSH32(v); }
	// 001ED12F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED132 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED133 call 0x000127D0
	X86_PUSH32(0x1ED138u); sub_000127D0(ctx, base);
	// 001ED138 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED13B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED120, __imp__sub_001ED120)

X86_FUNC(__imp__sub_001ED13C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED13C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED141 push 0x1C3
	{ uint32_t v = (uint32_t)(0x1C3u); X86_PUSH32(v); }
	// 001ED146 push 0x25534C
	{ uint32_t v = (uint32_t)(0x25534Cu); X86_PUSH32(v); }
	// 001ED14B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED14E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED14F call 0x000127D0
	X86_PUSH32(0x1ED154u); sub_000127D0(ctx, base);
	// 001ED154 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED157 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED13C, __imp__sub_001ED13C)

X86_FUNC(__imp__sub_001ED158) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED158 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED15D push 0x1D2
	{ uint32_t v = (uint32_t)(0x1D2u); X86_PUSH32(v); }
	// 001ED162 push 0x25534C
	{ uint32_t v = (uint32_t)(0x25534Cu); X86_PUSH32(v); }
	// 001ED167 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED16A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED16B call 0x000127D0
	X86_PUSH32(0x1ED170u); sub_000127D0(ctx, base);
	// 001ED170 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED173 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED158, __imp__sub_001ED158)

X86_FUNC(__imp__sub_001ED174) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED174 mov eax, 0x27D7D8
	{ ctx.eax = (uint32_t)(0x27D7D8u); }
	// 001ED179 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED174, __imp__sub_001ED174)

X86_FUNC(__imp__sub_001ED180) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED180 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED185 push 0x291
	{ uint32_t v = (uint32_t)(0x291u); X86_PUSH32(v); }
	// 001ED18A push 0x25534C
	{ uint32_t v = (uint32_t)(0x25534Cu); X86_PUSH32(v); }
	// 001ED18F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED192 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED193 call 0x000127D0
	X86_PUSH32(0x1ED198u); sub_000127D0(ctx, base);
	// 001ED198 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED19B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED180, __imp__sub_001ED180)

X86_FUNC(__imp__sub_001ED19C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED19C mov eax, 0x27D7FC
	{ ctx.eax = (uint32_t)(0x27D7FCu); }
	// 001ED1A1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED19C, __imp__sub_001ED19C)

X86_FUNC(__imp__sub_001ED1B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED1B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED1B5 push 0x1CB
	{ uint32_t v = (uint32_t)(0x1CBu); X86_PUSH32(v); }
	// 001ED1BA push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED1BF mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED1C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED1C3 call 0x000127D0
	X86_PUSH32(0x1ED1C8u); sub_000127D0(ctx, base);
	// 001ED1C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED1CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED1B0, __imp__sub_001ED1B0)

X86_FUNC(__imp__sub_001ED1CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED1CC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED1D1 push 0x1D7
	{ uint32_t v = (uint32_t)(0x1D7u); X86_PUSH32(v); }
	// 001ED1D6 push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED1DB mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED1DE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED1DF call 0x000127D0
	X86_PUSH32(0x1ED1E4u); sub_000127D0(ctx, base);
	// 001ED1E4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED1E7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED1CC, __imp__sub_001ED1CC)

X86_FUNC(__imp__sub_001ED1E8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED1E8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED1ED push 0x1DC
	{ uint32_t v = (uint32_t)(0x1DCu); X86_PUSH32(v); }
	// 001ED1F2 push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED1F7 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED1FA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED1FB call 0x000127D0
	X86_PUSH32(0x1ED200u); sub_000127D0(ctx, base);
	// 001ED200 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED203 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED1E8, __imp__sub_001ED1E8)

X86_FUNC(__imp__sub_001ED204) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED204 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED209 push 0x1E1
	{ uint32_t v = (uint32_t)(0x1E1u); X86_PUSH32(v); }
	// 001ED20E push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED213 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED216 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED217 call 0x000127D0
	X86_PUSH32(0x1ED21Cu); sub_000127D0(ctx, base);
	// 001ED21C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED21F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED204, __imp__sub_001ED204)

X86_FUNC(__imp__sub_001ED220) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED220 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED225 push 0x1E5
	{ uint32_t v = (uint32_t)(0x1E5u); X86_PUSH32(v); }
	// 001ED22A push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED22F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED232 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED233 call 0x000127D0
	X86_PUSH32(0x1ED238u); sub_000127D0(ctx, base);
	// 001ED238 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED23B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED220, __imp__sub_001ED220)

X86_FUNC(__imp__sub_001ED23C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED23C mov eax, 0x27D840
	{ ctx.eax = (uint32_t)(0x27D840u); }
	// 001ED241 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED23C, __imp__sub_001ED23C)

X86_FUNC(__imp__sub_001ED250) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED250 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED255 push 0x94
	{ uint32_t v = (uint32_t)(0x94u); X86_PUSH32(v); }
	// 001ED25A push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED25F mov eax, [ebp-0x24]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED262 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED263 call 0x000127D0
	X86_PUSH32(0x1ED268u); sub_000127D0(ctx, base);
	// 001ED268 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED26B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED250, __imp__sub_001ED250)

X86_FUNC(__imp__sub_001ED26C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED26C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED271 push 0x97
	{ uint32_t v = (uint32_t)(0x97u); X86_PUSH32(v); }
	// 001ED276 push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED27B mov eax, [ebp-0x24]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED27E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED27F call 0x000127D0
	X86_PUSH32(0x1ED284u); sub_000127D0(ctx, base);
	// 001ED284 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED287 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED26C, __imp__sub_001ED26C)

X86_FUNC(__imp__sub_001ED288) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED288 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED28D push 0xA4
	{ uint32_t v = (uint32_t)(0xA4u); X86_PUSH32(v); }
	// 001ED292 push 0x255464
	{ uint32_t v = (uint32_t)(0x255464u); X86_PUSH32(v); }
	// 001ED297 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED29A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED29B call 0x000127D0
	X86_PUSH32(0x1ED2A0u); sub_000127D0(ctx, base);
	// 001ED2A0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED2A3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED288, __imp__sub_001ED288)

X86_FUNC(__imp__sub_001ED2A4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED2A4 mov eax, 0x27D874
	{ ctx.eax = (uint32_t)(0x27D874u); }
	// 001ED2A9 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED2A4, __imp__sub_001ED2A4)

X86_FUNC(__imp__sub_001ED2B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED2B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED2B5 push 0x5A
	{ uint32_t v = (uint32_t)(0x5Au); X86_PUSH32(v); }
	// 001ED2B7 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED2BC mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED2BF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED2C0 call 0x000127D0
	X86_PUSH32(0x1ED2C5u); sub_000127D0(ctx, base);
	// 001ED2C5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED2C8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED2B0, __imp__sub_001ED2B0)

X86_FUNC(__imp__sub_001ED2C9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED2C9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED2CE push 0x5C
	{ uint32_t v = (uint32_t)(0x5Cu); X86_PUSH32(v); }
	// 001ED2D0 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED2D5 mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED2D8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED2D9 call 0x000127D0
	X86_PUSH32(0x1ED2DEu); sub_000127D0(ctx, base);
	// 001ED2DE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED2E1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED2C9, __imp__sub_001ED2C9)

X86_FUNC(__imp__sub_001ED2E2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED2E2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED2E7 push 0x5D
	{ uint32_t v = (uint32_t)(0x5Du); X86_PUSH32(v); }
	// 001ED2E9 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED2EE mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED2F1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED2F2 call 0x000127D0
	X86_PUSH32(0x1ED2F7u); sub_000127D0(ctx, base);
	// 001ED2F7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED2FA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED2E2, __imp__sub_001ED2E2)

X86_FUNC(__imp__sub_001ED2FB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED2FB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED300 push 0x5E
	{ uint32_t v = (uint32_t)(0x5Eu); X86_PUSH32(v); }
	// 001ED302 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED307 mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED30A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED30B call 0x000127D0
	X86_PUSH32(0x1ED310u); sub_000127D0(ctx, base);
	// 001ED310 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED313 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED2FB, __imp__sub_001ED2FB)

X86_FUNC(__imp__sub_001ED314) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED314 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED319 push 0x5F
	{ uint32_t v = (uint32_t)(0x5Fu); X86_PUSH32(v); }
	// 001ED31B push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED320 mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED323 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED324 call 0x000127D0
	X86_PUSH32(0x1ED329u); sub_000127D0(ctx, base);
	// 001ED329 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED32C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED314, __imp__sub_001ED314)

X86_FUNC(__imp__sub_001ED32D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED32D push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED332 push 0x60
	{ uint32_t v = (uint32_t)(0x60u); X86_PUSH32(v); }
	// 001ED334 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED339 mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED33C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED33D call 0x000127D0
	X86_PUSH32(0x1ED342u); sub_000127D0(ctx, base);
	// 001ED342 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED345 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED32D, __imp__sub_001ED32D)

X86_FUNC(__imp__sub_001ED346) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED346 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED34B push 0x61
	{ uint32_t v = (uint32_t)(0x61u); X86_PUSH32(v); }
	// 001ED34D push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED352 mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED355 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED356 call 0x000127D0
	X86_PUSH32(0x1ED35Bu); sub_000127D0(ctx, base);
	// 001ED35B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED35E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED346, __imp__sub_001ED346)

X86_FUNC(__imp__sub_001ED35F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED35F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED364 push 0x92
	{ uint32_t v = (uint32_t)(0x92u); X86_PUSH32(v); }
	// 001ED369 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED36E mov eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED371 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED372 call 0x000127D0
	X86_PUSH32(0x1ED377u); sub_000127D0(ctx, base);
	// 001ED377 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED37A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED35F, __imp__sub_001ED35F)

X86_FUNC(__imp__sub_001ED37B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED37B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED380 push 0x96
	{ uint32_t v = (uint32_t)(0x96u); X86_PUSH32(v); }
	// 001ED385 push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED38A mov eax, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED38D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED38E call 0x000127D0
	X86_PUSH32(0x1ED393u); sub_000127D0(ctx, base);
	// 001ED393 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED396 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED37B, __imp__sub_001ED37B)

X86_FUNC(__imp__sub_001ED397) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED397 mov eax, 0x27D8D8
	{ ctx.eax = (uint32_t)(0x27D8D8u); }
	// 001ED39C jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED397, __imp__sub_001ED397)

X86_FUNC(__imp__sub_001ED3B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED3B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED3B5 push 0x16E
	{ uint32_t v = (uint32_t)(0x16Eu); X86_PUSH32(v); }
	// 001ED3BA push 0x25554C
	{ uint32_t v = (uint32_t)(0x25554Cu); X86_PUSH32(v); }
	// 001ED3BF mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED3C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED3C3 call 0x000127D0
	X86_PUSH32(0x1ED3C8u); sub_000127D0(ctx, base);
	// 001ED3C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED3CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED3B0, __imp__sub_001ED3B0)

X86_FUNC(__imp__sub_001ED3CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED3CC mov eax, 0x27D8FC
	{ ctx.eax = (uint32_t)(0x27D8FCu); }
	// 001ED3D1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED3CC, __imp__sub_001ED3CC)

X86_FUNC(__imp__sub_001ED3E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED3E0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED3E3 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED3E0, __imp__sub_001ED3E0)

X86_FUNC(__imp__sub_001ED3E8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED3E8 push 0x1DDC0
	{ uint32_t v = (uint32_t)(0x1DDC0u); X86_PUSH32(v); }
	// 001ED3ED push 0x06
	{ uint32_t v = (uint32_t)(0x6u); X86_PUSH32(v); }
	// 001ED3EF push 0x64
	{ uint32_t v = (uint32_t)(0x64u); X86_PUSH32(v); }
	// 001ED3F1 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED3F4 add eax, 0xA4
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xA4u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED3F9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED3FA call 0x001DEF24
	X86_PUSH32(0x1ED3FFu); sub_001DEF24(ctx, base);
	// 001ED3FF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED3E8, __imp__sub_001ED3E8)

X86_FUNC(__imp__sub_001ED400) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED400 mov eax, 0x27D954
	{ ctx.eax = (uint32_t)(0x27D954u); }
	// 001ED405 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED400, __imp__sub_001ED400)

X86_FUNC(__imp__sub_001ED410) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED410 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED415 push 0x3CA
	{ uint32_t v = (uint32_t)(0x3CAu); X86_PUSH32(v); }
	// 001ED41A push 0x2558E4
	{ uint32_t v = (uint32_t)(0x2558E4u); X86_PUSH32(v); }
	// 001ED41F mov eax, [ebp-0x3D4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC2Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED425 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED426 call 0x000127D0
	X86_PUSH32(0x1ED42Bu); sub_000127D0(ctx, base);
	// 001ED42B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED42E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED410, __imp__sub_001ED410)

X86_FUNC(__imp__sub_001ED42F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED42F mov eax, 0x27D978
	{ ctx.eax = (uint32_t)(0x27D978u); }
	// 001ED434 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED42F, __imp__sub_001ED42F)

X86_FUNC(__imp__sub_001ED440) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED440 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED443 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED440, __imp__sub_001ED440)

X86_FUNC(__imp__sub_001ED448) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED448 mov eax, 0x27D99C
	{ ctx.eax = (uint32_t)(0x27D99Cu); }
	// 001ED44D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED448, __imp__sub_001ED448)

X86_FUNC(__imp__sub_001ED460) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED460 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED465 push 0x131
	{ uint32_t v = (uint32_t)(0x131u); X86_PUSH32(v); }
	// 001ED46A push 0x255AC8
	{ uint32_t v = (uint32_t)(0x255AC8u); X86_PUSH32(v); }
	// 001ED46F mov eax, [ebp-0x3F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED475 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED476 call 0x000127D0
	X86_PUSH32(0x1ED47Bu); sub_000127D0(ctx, base);
	// 001ED47B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED47E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED460, __imp__sub_001ED460)

X86_FUNC(__imp__sub_001ED47F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED47F mov eax, 0x27D9E4
	{ ctx.eax = (uint32_t)(0x27D9E4u); }
	// 001ED484 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED47F, __imp__sub_001ED47F)

X86_FUNC(__imp__sub_001ED490) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED490 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED493 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED490, __imp__sub_001ED490)

X86_FUNC(__imp__sub_001ED498) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED498 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED49B add ecx, 0x98
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x98u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED4A1 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED498, __imp__sub_001ED498)

X86_FUNC(__imp__sub_001ED4A6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED4A6 mov eax, 0x27DA10
	{ ctx.eax = (uint32_t)(0x27DA10u); }
	// 001ED4AB jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED4A6, __imp__sub_001ED4A6)

X86_FUNC(__imp__sub_001ED4B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED4B0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED4B3 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED4B0, __imp__sub_001ED4B0)

X86_FUNC(__imp__sub_001ED4B8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED4B8 mov eax, 0x27DA34
	{ ctx.eax = (uint32_t)(0x27DA34u); }
	// 001ED4BD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED4B8, __imp__sub_001ED4B8)

X86_FUNC(__imp__sub_001ED4D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED4D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED4D5 push 0x5C
	{ uint32_t v = (uint32_t)(0x5Cu); X86_PUSH32(v); }
	// 001ED4D7 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED4DC mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED4E2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED4E3 call 0x000127D0
	X86_PUSH32(0x1ED4E8u); sub_000127D0(ctx, base);
	// 001ED4E8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED4EB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED4D0, __imp__sub_001ED4D0)

X86_FUNC(__imp__sub_001ED4EC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED4EC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED4F1 push 0x68
	{ uint32_t v = (uint32_t)(0x68u); X86_PUSH32(v); }
	// 001ED4F3 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED4F8 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED4FE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED4FF call 0x000127D0
	X86_PUSH32(0x1ED504u); sub_000127D0(ctx, base);
	// 001ED504 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED507 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED4EC, __imp__sub_001ED4EC)

X86_FUNC(__imp__sub_001ED508) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED508 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED50D push 0x6D
	{ uint32_t v = (uint32_t)(0x6Du); X86_PUSH32(v); }
	// 001ED50F push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED514 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED51A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED51B call 0x000127D0
	X86_PUSH32(0x1ED520u); sub_000127D0(ctx, base);
	// 001ED520 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED523 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED508, __imp__sub_001ED508)

X86_FUNC(__imp__sub_001ED524) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED524 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED529 push 0x7B
	{ uint32_t v = (uint32_t)(0x7Bu); X86_PUSH32(v); }
	// 001ED52B push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED530 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED536 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED537 call 0x000127D0
	X86_PUSH32(0x1ED53Cu); sub_000127D0(ctx, base);
	// 001ED53C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED53F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED524, __imp__sub_001ED524)

X86_FUNC(__imp__sub_001ED540) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED540 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED545 push 0x7D
	{ uint32_t v = (uint32_t)(0x7Du); X86_PUSH32(v); }
	// 001ED547 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED54C mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED552 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED553 call 0x000127D0
	X86_PUSH32(0x1ED558u); sub_000127D0(ctx, base);
	// 001ED558 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED55B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED540, __imp__sub_001ED540)

X86_FUNC(__imp__sub_001ED55C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED55C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED561 push 0x86
	{ uint32_t v = (uint32_t)(0x86u); X86_PUSH32(v); }
	// 001ED566 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED56B mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED571 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED572 call 0x000127D0
	X86_PUSH32(0x1ED577u); sub_000127D0(ctx, base);
	// 001ED577 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED57A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED55C, __imp__sub_001ED55C)

X86_FUNC(__imp__sub_001ED57B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED57B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED580 push 0x88
	{ uint32_t v = (uint32_t)(0x88u); X86_PUSH32(v); }
	// 001ED585 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED58A mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED590 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED591 call 0x000127D0
	X86_PUSH32(0x1ED596u); sub_000127D0(ctx, base);
	// 001ED596 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED599 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED57B, __imp__sub_001ED57B)

X86_FUNC(__imp__sub_001ED59A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED59A push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED59F push 0x90
	{ uint32_t v = (uint32_t)(0x90u); X86_PUSH32(v); }
	// 001ED5A4 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED5A9 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED5AF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED5B0 call 0x000127D0
	X86_PUSH32(0x1ED5B5u); sub_000127D0(ctx, base);
	// 001ED5B5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED5B8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED59A, __imp__sub_001ED59A)

X86_FUNC(__imp__sub_001ED5B9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED5B9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED5BE push 0x98
	{ uint32_t v = (uint32_t)(0x98u); X86_PUSH32(v); }
	// 001ED5C3 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED5C8 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED5CE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED5CF call 0x000127D0
	X86_PUSH32(0x1ED5D4u); sub_000127D0(ctx, base);
	// 001ED5D4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED5D7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED5B9, __imp__sub_001ED5B9)

X86_FUNC(__imp__sub_001ED5D8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED5D8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED5DD push 0x9E
	{ uint32_t v = (uint32_t)(0x9Eu); X86_PUSH32(v); }
	// 001ED5E2 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED5E7 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED5ED push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED5EE call 0x000127D0
	X86_PUSH32(0x1ED5F3u); sub_000127D0(ctx, base);
	// 001ED5F3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED5F6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED5D8, __imp__sub_001ED5D8)

X86_FUNC(__imp__sub_001ED5F7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED5F7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED5FC push 0xA4
	{ uint32_t v = (uint32_t)(0xA4u); X86_PUSH32(v); }
	// 001ED601 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED606 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED60C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED60D call 0x000127D0
	X86_PUSH32(0x1ED612u); sub_000127D0(ctx, base);
	// 001ED612 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED615 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED5F7, __imp__sub_001ED5F7)

X86_FUNC(__imp__sub_001ED616) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED616 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED61B push 0xA8
	{ uint32_t v = (uint32_t)(0xA8u); X86_PUSH32(v); }
	// 001ED620 push 0x255C1C
	{ uint32_t v = (uint32_t)(0x255C1Cu); X86_PUSH32(v); }
	// 001ED625 mov eax, [ebp-0x3C4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED62B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED62C call 0x000127D0
	X86_PUSH32(0x1ED631u); sub_000127D0(ctx, base);
	// 001ED631 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED634 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED616, __imp__sub_001ED616)

X86_FUNC(__imp__sub_001ED635) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED635 mov eax, 0x27DAB0
	{ ctx.eax = (uint32_t)(0x27DAB0u); }
	// 001ED63A jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED635, __imp__sub_001ED635)

X86_FUNC(__imp__sub_001ED640) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED640 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED643 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED640, __imp__sub_001ED640)

X86_FUNC(__imp__sub_001ED648) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED648 mov eax, 0x27DAD4
	{ ctx.eax = (uint32_t)(0x27DAD4u); }
	// 001ED64D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED648, __imp__sub_001ED648)

X86_FUNC(__imp__sub_001ED660) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED660 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED665 push 0x11D
	{ uint32_t v = (uint32_t)(0x11Du); X86_PUSH32(v); }
	// 001ED66A push 0x255D64
	{ uint32_t v = (uint32_t)(0x255D64u); X86_PUSH32(v); }
	// 001ED66F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED672 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED673 call 0x000127D0
	X86_PUSH32(0x1ED678u); sub_000127D0(ctx, base);
	// 001ED678 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED67B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED660, __imp__sub_001ED660)

X86_FUNC(__imp__sub_001ED67C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED67C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED681 push 0x124
	{ uint32_t v = (uint32_t)(0x124u); X86_PUSH32(v); }
	// 001ED686 push 0x255D64
	{ uint32_t v = (uint32_t)(0x255D64u); X86_PUSH32(v); }
	// 001ED68B mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED68E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED68F call 0x000127D0
	X86_PUSH32(0x1ED694u); sub_000127D0(ctx, base);
	// 001ED694 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED697 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED67C, __imp__sub_001ED67C)

X86_FUNC(__imp__sub_001ED698) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED698 mov eax, 0x27DB24
	{ ctx.eax = (uint32_t)(0x27DB24u); }
	// 001ED69D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED698, __imp__sub_001ED698)

X86_FUNC(__imp__sub_001ED6B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED6B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED6B5 push 0x18F
	{ uint32_t v = (uint32_t)(0x18Fu); X86_PUSH32(v); }
	// 001ED6BA push 0x255D64
	{ uint32_t v = (uint32_t)(0x255D64u); X86_PUSH32(v); }
	// 001ED6BF mov eax, [ebp-0x410]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED6C5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED6C6 call 0x000127D0
	X86_PUSH32(0x1ED6CBu); sub_000127D0(ctx, base);
	// 001ED6CB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED6CE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED6B0, __imp__sub_001ED6B0)

X86_FUNC(__imp__sub_001ED6CF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED6CF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED6D4 push 0x1C2
	{ uint32_t v = (uint32_t)(0x1C2u); X86_PUSH32(v); }
	// 001ED6D9 push 0x255D64
	{ uint32_t v = (uint32_t)(0x255D64u); X86_PUSH32(v); }
	// 001ED6DE mov eax, [ebp-0x410]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED6E4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED6E5 call 0x000127D0
	X86_PUSH32(0x1ED6EAu); sub_000127D0(ctx, base);
	// 001ED6EA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED6ED ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED6CF, __imp__sub_001ED6CF)

X86_FUNC(__imp__sub_001ED6EE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED6EE mov eax, 0x27DB50
	{ ctx.eax = (uint32_t)(0x27DB50u); }
	// 001ED6F3 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED6EE, __imp__sub_001ED6EE)

X86_FUNC(__imp__sub_001ED700) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED700 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED703 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED700, __imp__sub_001ED700)

X86_FUNC(__imp__sub_001ED708) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED708 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED70B add ecx, 0x0C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED70E jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED708, __imp__sub_001ED708)

X86_FUNC(__imp__sub_001ED713) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED713 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED716 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED719 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED713, __imp__sub_001ED713)

X86_FUNC(__imp__sub_001ED71E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED71E mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED721 add ecx, 0x24
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED724 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED71E, __imp__sub_001ED71E)

X86_FUNC(__imp__sub_001ED729) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED729 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED72C add ecx, 0x30
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x30u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED72F jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED729, __imp__sub_001ED729)

X86_FUNC(__imp__sub_001ED734) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED734 mov eax, 0x27DBD8
	{ ctx.eax = (uint32_t)(0x27DBD8u); }
	// 001ED739 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED734, __imp__sub_001ED734)

X86_FUNC(__imp__sub_001ED740) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED740 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED743 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED740, __imp__sub_001ED740)

X86_FUNC(__imp__sub_001ED748) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED748 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED74B add ecx, 0xB4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xB4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED751 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED748, __imp__sub_001ED748)

X86_FUNC(__imp__sub_001ED756) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED756 mov eax, 0x27DC04
	{ ctx.eax = (uint32_t)(0x27DC04u); }
	// 001ED75B jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED756, __imp__sub_001ED756)

X86_FUNC(__imp__sub_001ED760) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED760 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED765 push 0x52
	{ uint32_t v = (uint32_t)(0x52u); X86_PUSH32(v); }
	// 001ED767 push 0x255F58
	{ uint32_t v = (uint32_t)(0x255F58u); X86_PUSH32(v); }
	// 001ED76C mov eax, [ebp-0x3C8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC38u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED772 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED773 call 0x000127D0
	X86_PUSH32(0x1ED778u); sub_000127D0(ctx, base);
	// 001ED778 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED77B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED760, __imp__sub_001ED760)

X86_FUNC(__imp__sub_001ED77C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED77C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED781 push 0x56
	{ uint32_t v = (uint32_t)(0x56u); X86_PUSH32(v); }
	// 001ED783 push 0x255F58
	{ uint32_t v = (uint32_t)(0x255F58u); X86_PUSH32(v); }
	// 001ED788 mov eax, [ebp-0x3C8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC38u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED78E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED78F call 0x000127D0
	X86_PUSH32(0x1ED794u); sub_000127D0(ctx, base);
	// 001ED794 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED797 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED77C, __imp__sub_001ED77C)

X86_FUNC(__imp__sub_001ED798) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED798 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED79D push 0x5E
	{ uint32_t v = (uint32_t)(0x5Eu); X86_PUSH32(v); }
	// 001ED79F push 0x255F58
	{ uint32_t v = (uint32_t)(0x255F58u); X86_PUSH32(v); }
	// 001ED7A4 mov eax, [ebp-0x3C8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC38u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED7AA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED7AB call 0x000127D0
	X86_PUSH32(0x1ED7B0u); sub_000127D0(ctx, base);
	// 001ED7B0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED7B3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED798, __imp__sub_001ED798)

X86_FUNC(__imp__sub_001ED7B4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED7B4 mov eax, 0x27DC64
	{ ctx.eax = (uint32_t)(0x27DC64u); }
	// 001ED7B9 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED7B4, __imp__sub_001ED7B4)

X86_FUNC(__imp__sub_001ED7C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED7C0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED7C5 push 0xF7
	{ uint32_t v = (uint32_t)(0xF7u); X86_PUSH32(v); }
	// 001ED7CA push 0x255F58
	{ uint32_t v = (uint32_t)(0x255F58u); X86_PUSH32(v); }
	// 001ED7CF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED7D2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED7D3 call 0x000127D0
	X86_PUSH32(0x1ED7D8u); sub_000127D0(ctx, base);
	// 001ED7D8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED7DB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED7C0, __imp__sub_001ED7C0)

X86_FUNC(__imp__sub_001ED7DC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED7DC mov eax, 0x27DC88
	{ ctx.eax = (uint32_t)(0x27DC88u); }
	// 001ED7E1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED7DC, __imp__sub_001ED7DC)

X86_FUNC(__imp__sub_001ED7F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED7F0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001ED7F3 jmp 0x00012860
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00012860(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED7F0, __imp__sub_001ED7F0)

X86_FUNC(__imp__sub_001ED7F8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED7F8 mov eax, 0x27DCAC
	{ ctx.eax = (uint32_t)(0x27DCACu); }
	// 001ED7FD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED7F8, __imp__sub_001ED7F8)

X86_FUNC(__imp__sub_001ED810) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED810 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED815 push 0x12F
	{ uint32_t v = (uint32_t)(0x12Fu); X86_PUSH32(v); }
	// 001ED81A push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED81F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED822 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED823 call 0x000127D0
	X86_PUSH32(0x1ED828u); sub_000127D0(ctx, base);
	// 001ED828 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED82B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED810, __imp__sub_001ED810)

X86_FUNC(__imp__sub_001ED82C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED82C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED831 push 0x13F
	{ uint32_t v = (uint32_t)(0x13Fu); X86_PUSH32(v); }
	// 001ED836 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED83B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED83E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED83F call 0x000127D0
	X86_PUSH32(0x1ED844u); sub_000127D0(ctx, base);
	// 001ED844 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED847 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED82C, __imp__sub_001ED82C)

X86_FUNC(__imp__sub_001ED848) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED848 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED84D push 0x16F
	{ uint32_t v = (uint32_t)(0x16Fu); X86_PUSH32(v); }
	// 001ED852 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED857 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED85A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED85B call 0x000127D0
	X86_PUSH32(0x1ED860u); sub_000127D0(ctx, base);
	// 001ED860 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED863 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED848, __imp__sub_001ED848)

X86_FUNC(__imp__sub_001ED864) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED864 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED869 push 0x173
	{ uint32_t v = (uint32_t)(0x173u); X86_PUSH32(v); }
	// 001ED86E push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED873 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED876 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED877 call 0x000127D0
	X86_PUSH32(0x1ED87Cu); sub_000127D0(ctx, base);
	// 001ED87C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED87F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED864, __imp__sub_001ED864)

X86_FUNC(__imp__sub_001ED880) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED880 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED885 push 0x177
	{ uint32_t v = (uint32_t)(0x177u); X86_PUSH32(v); }
	// 001ED88A push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED88F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED892 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED893 call 0x000127D0
	X86_PUSH32(0x1ED898u); sub_000127D0(ctx, base);
	// 001ED898 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED89B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED880, __imp__sub_001ED880)

X86_FUNC(__imp__sub_001ED89C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED89C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED8A1 push 0x17B
	{ uint32_t v = (uint32_t)(0x17Bu); X86_PUSH32(v); }
	// 001ED8A6 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED8AB mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED8AE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED8AF call 0x000127D0
	X86_PUSH32(0x1ED8B4u); sub_000127D0(ctx, base);
	// 001ED8B4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED8B7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED89C, __imp__sub_001ED89C)

X86_FUNC(__imp__sub_001ED8B8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED8B8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED8BD push 0x180
	{ uint32_t v = (uint32_t)(0x180u); X86_PUSH32(v); }
	// 001ED8C2 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED8C7 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED8CA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED8CB call 0x000127D0
	X86_PUSH32(0x1ED8D0u); sub_000127D0(ctx, base);
	// 001ED8D0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED8D3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED8B8, __imp__sub_001ED8B8)

X86_FUNC(__imp__sub_001ED8D4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED8D4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED8D9 push 0x184
	{ uint32_t v = (uint32_t)(0x184u); X86_PUSH32(v); }
	// 001ED8DE push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED8E3 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED8E6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED8E7 call 0x000127D0
	X86_PUSH32(0x1ED8ECu); sub_000127D0(ctx, base);
	// 001ED8EC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED8EF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED8D4, __imp__sub_001ED8D4)

X86_FUNC(__imp__sub_001ED8F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED8F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED8F5 push 0x190
	{ uint32_t v = (uint32_t)(0x190u); X86_PUSH32(v); }
	// 001ED8FA push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED8FF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED902 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED903 call 0x000127D0
	X86_PUSH32(0x1ED908u); sub_000127D0(ctx, base);
	// 001ED908 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED90B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED8F0, __imp__sub_001ED8F0)

X86_FUNC(__imp__sub_001ED90C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED90C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED911 push 0x195
	{ uint32_t v = (uint32_t)(0x195u); X86_PUSH32(v); }
	// 001ED916 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED91B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED91E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED91F call 0x000127D0
	X86_PUSH32(0x1ED924u); sub_000127D0(ctx, base);
	// 001ED924 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED927 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED90C, __imp__sub_001ED90C)

X86_FUNC(__imp__sub_001ED928) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED928 mov eax, 0x27DD3C
	{ ctx.eax = (uint32_t)(0x27DD3Cu); }
	// 001ED92D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED928, __imp__sub_001ED928)

X86_FUNC(__imp__sub_001ED940) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED940 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED945 push 0x21D
	{ uint32_t v = (uint32_t)(0x21Du); X86_PUSH32(v); }
	// 001ED94A push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED94F mov eax, [ebp-0x68]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF98u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED952 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED953 call 0x000127D0
	X86_PUSH32(0x1ED958u); sub_000127D0(ctx, base);
	// 001ED958 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED95B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED940, __imp__sub_001ED940)

X86_FUNC(__imp__sub_001ED95C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED95C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED961 push 0x249
	{ uint32_t v = (uint32_t)(0x249u); X86_PUSH32(v); }
	// 001ED966 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED96B mov eax, [ebp-0x64]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF9Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED96E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED96F call 0x000127D0
	X86_PUSH32(0x1ED974u); sub_000127D0(ctx, base);
	// 001ED974 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED977 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED95C, __imp__sub_001ED95C)

X86_FUNC(__imp__sub_001ED978) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED978 mov eax, 0x27DD68
	{ ctx.eax = (uint32_t)(0x27DD68u); }
	// 001ED97D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001ED978, __imp__sub_001ED978)

X86_FUNC(__imp__sub_001ED990) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED990 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED995 push 0x2DC
	{ uint32_t v = (uint32_t)(0x2DCu); X86_PUSH32(v); }
	// 001ED99A push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED99F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED9A2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED9A3 call 0x000127D0
	X86_PUSH32(0x1ED9A8u); sub_000127D0(ctx, base);
	// 001ED9A8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED9AB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED990, __imp__sub_001ED990)

X86_FUNC(__imp__sub_001ED9AC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED9AC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED9B1 push 0x2DD
	{ uint32_t v = (uint32_t)(0x2DDu); X86_PUSH32(v); }
	// 001ED9B6 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED9BB mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED9BE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED9BF call 0x000127D0
	X86_PUSH32(0x1ED9C4u); sub_000127D0(ctx, base);
	// 001ED9C4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED9C7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED9AC, __imp__sub_001ED9AC)

X86_FUNC(__imp__sub_001ED9C8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED9C8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED9CD push 0x2DE
	{ uint32_t v = (uint32_t)(0x2DEu); X86_PUSH32(v); }
	// 001ED9D2 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED9D7 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED9DA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED9DB call 0x000127D0
	X86_PUSH32(0x1ED9E0u); sub_000127D0(ctx, base);
	// 001ED9E0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED9E3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED9C8, __imp__sub_001ED9C8)

X86_FUNC(__imp__sub_001ED9E4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001ED9E4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001ED9E9 push 0x2DF
	{ uint32_t v = (uint32_t)(0x2DFu); X86_PUSH32(v); }
	// 001ED9EE push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001ED9F3 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001ED9F6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001ED9F7 call 0x000127D0
	X86_PUSH32(0x1ED9FCu); sub_000127D0(ctx, base);
	// 001ED9FC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001ED9FF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001ED9E4, __imp__sub_001ED9E4)

X86_FUNC(__imp__sub_001EDA00) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDA00 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDA05 push 0x2E0
	{ uint32_t v = (uint32_t)(0x2E0u); X86_PUSH32(v); }
	// 001EDA0A push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001EDA0F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDA12 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDA13 call 0x000127D0
	X86_PUSH32(0x1EDA18u); sub_000127D0(ctx, base);
	// 001EDA18 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDA1B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDA00, __imp__sub_001EDA00)

X86_FUNC(__imp__sub_001EDA1C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDA1C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDA21 push 0x2E1
	{ uint32_t v = (uint32_t)(0x2E1u); X86_PUSH32(v); }
	// 001EDA26 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001EDA2B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDA2E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDA2F call 0x000127D0
	X86_PUSH32(0x1EDA34u); sub_000127D0(ctx, base);
	// 001EDA34 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDA37 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDA1C, __imp__sub_001EDA1C)

X86_FUNC(__imp__sub_001EDA38) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDA38 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDA3D push 0x2E2
	{ uint32_t v = (uint32_t)(0x2E2u); X86_PUSH32(v); }
	// 001EDA42 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001EDA47 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDA4A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDA4B call 0x000127D0
	X86_PUSH32(0x1EDA50u); sub_000127D0(ctx, base);
	// 001EDA50 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDA53 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDA38, __imp__sub_001EDA38)

X86_FUNC(__imp__sub_001EDA54) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDA54 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDA59 push 0x2E3
	{ uint32_t v = (uint32_t)(0x2E3u); X86_PUSH32(v); }
	// 001EDA5E push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001EDA63 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDA66 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDA67 call 0x000127D0
	X86_PUSH32(0x1EDA6Cu); sub_000127D0(ctx, base);
	// 001EDA6C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDA6F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDA54, __imp__sub_001EDA54)

X86_FUNC(__imp__sub_001EDA70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDA70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDA75 push 0x308
	{ uint32_t v = (uint32_t)(0x308u); X86_PUSH32(v); }
	// 001EDA7A push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001EDA7F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDA82 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDA83 call 0x000127D0
	X86_PUSH32(0x1EDA88u); sub_000127D0(ctx, base);
	// 001EDA88 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDA8B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDA70, __imp__sub_001EDA70)

X86_FUNC(__imp__sub_001EDA8C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDA8C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDA91 push 0x309
	{ uint32_t v = (uint32_t)(0x309u); X86_PUSH32(v); }
	// 001EDA96 push 0x256088
	{ uint32_t v = (uint32_t)(0x256088u); X86_PUSH32(v); }
	// 001EDA9B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDA9E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDA9F call 0x000127D0
	X86_PUSH32(0x1EDAA4u); sub_000127D0(ctx, base);
	// 001EDAA4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDAA7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDA8C, __imp__sub_001EDA8C)

X86_FUNC(__imp__sub_001EDAA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDAA8 mov eax, 0x27DDD4
	{ ctx.eax = (uint32_t)(0x27DDD4u); }
	// 001EDAAD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDAA8, __imp__sub_001EDAA8)

X86_FUNC(__imp__sub_001EDAC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDAC0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDAC3 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDAC0, __imp__sub_001EDAC0)

X86_FUNC(__imp__sub_001EDAC8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDAC8 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDACB add ecx, 0x148
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x148u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDAD1 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDAC8, __imp__sub_001EDAC8)

X86_FUNC(__imp__sub_001EDAD6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDAD6 mov eax, 0x27DE00
	{ ctx.eax = (uint32_t)(0x27DE00u); }
	// 001EDADB jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDAD6, __imp__sub_001EDAD6)

X86_FUNC(__imp__sub_001EDAE0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDAE0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDAE3 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDAE0, __imp__sub_001EDAE0)

X86_FUNC(__imp__sub_001EDAE8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDAE8 mov eax, 0x27DE24
	{ ctx.eax = (uint32_t)(0x27DE24u); }
	// 001EDAED jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDAE8, __imp__sub_001EDAE8)

X86_FUNC(__imp__sub_001EDB00) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB00 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDB05 push 0x3C6
	{ uint32_t v = (uint32_t)(0x3C6u); X86_PUSH32(v); }
	// 001EDB0A push 0x25662C
	{ uint32_t v = (uint32_t)(0x25662Cu); X86_PUSH32(v); }
	// 001EDB0F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDB12 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDB13 call 0x000127D0
	X86_PUSH32(0x1EDB18u); sub_000127D0(ctx, base);
	// 001EDB18 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDB1B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDB00, __imp__sub_001EDB00)

X86_FUNC(__imp__sub_001EDB1C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB1C mov eax, 0x27DE48
	{ ctx.eax = (uint32_t)(0x27DE48u); }
	// 001EDB21 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDB1C, __imp__sub_001EDB1C)

X86_FUNC(__imp__sub_001EDB30) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB30 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDB33 jmp 0x00030190
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00030190(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDB30, __imp__sub_001EDB30)

X86_FUNC(__imp__sub_001EDB38) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB38 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDB3B add ecx, 0x98
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x98u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDB41 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDB38, __imp__sub_001EDB38)

X86_FUNC(__imp__sub_001EDB46) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB46 mov eax, 0x27DE74
	{ ctx.eax = (uint32_t)(0x27DE74u); }
	// 001EDB4B jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDB46, __imp__sub_001EDB46)

X86_FUNC(__imp__sub_001EDB50) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB50 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDB55 push 0x53
	{ uint32_t v = (uint32_t)(0x53u); X86_PUSH32(v); }
	// 001EDB57 push 0x2566C4
	{ uint32_t v = (uint32_t)(0x2566C4u); X86_PUSH32(v); }
	// 001EDB5C mov eax, [ebp-0xA8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF58u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDB62 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDB63 call 0x000127D0
	X86_PUSH32(0x1EDB68u); sub_000127D0(ctx, base);
	// 001EDB68 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDB6B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDB50, __imp__sub_001EDB50)

X86_FUNC(__imp__sub_001EDB6C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB6C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDB71 push 0x5B
	{ uint32_t v = (uint32_t)(0x5Bu); X86_PUSH32(v); }
	// 001EDB73 push 0x2566C4
	{ uint32_t v = (uint32_t)(0x2566C4u); X86_PUSH32(v); }
	// 001EDB78 mov eax, [ebp-0xA8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF58u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDB7E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDB7F call 0x000127D0
	X86_PUSH32(0x1EDB84u); sub_000127D0(ctx, base);
	// 001EDB84 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDB87 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDB6C, __imp__sub_001EDB6C)

X86_FUNC(__imp__sub_001EDB88) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDB88 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDB8D push 0x65
	{ uint32_t v = (uint32_t)(0x65u); X86_PUSH32(v); }
	// 001EDB8F push 0x2566C4
	{ uint32_t v = (uint32_t)(0x2566C4u); X86_PUSH32(v); }
	// 001EDB94 mov eax, [ebp-0xA0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF60u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDB9A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDB9B call 0x000127D0
	X86_PUSH32(0x1EDBA0u); sub_000127D0(ctx, base);
	// 001EDBA0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDBA3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDB88, __imp__sub_001EDB88)

X86_FUNC(__imp__sub_001EDBA4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDBA4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDBA9 push 0x69
	{ uint32_t v = (uint32_t)(0x69u); X86_PUSH32(v); }
	// 001EDBAB push 0x2566C4
	{ uint32_t v = (uint32_t)(0x2566C4u); X86_PUSH32(v); }
	// 001EDBB0 mov eax, [ebp-0xA0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF60u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDBB6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDBB7 call 0x000127D0
	X86_PUSH32(0x1EDBBCu); sub_000127D0(ctx, base);
	// 001EDBBC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDBBF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDBA4, __imp__sub_001EDBA4)

X86_FUNC(__imp__sub_001EDBC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDBC0 mov eax, 0x27DEDC
	{ ctx.eax = (uint32_t)(0x27DEDCu); }
	// 001EDBC5 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDBC0, __imp__sub_001EDBC0)

X86_FUNC(__imp__sub_001EDBD0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDBD0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDBD5 push 0x139
	{ uint32_t v = (uint32_t)(0x139u); X86_PUSH32(v); }
	// 001EDBDA push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDBDF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDBE2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDBE3 call 0x000127D0
	X86_PUSH32(0x1EDBE8u); sub_000127D0(ctx, base);
	// 001EDBE8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDBEB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDBD0, __imp__sub_001EDBD0)

X86_FUNC(__imp__sub_001EDBEC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDBEC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDBF1 push 0x13A
	{ uint32_t v = (uint32_t)(0x13Au); X86_PUSH32(v); }
	// 001EDBF6 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDBFB mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDBFE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDBFF call 0x000127D0
	X86_PUSH32(0x1EDC04u); sub_000127D0(ctx, base);
	// 001EDC04 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDC07 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDBEC, __imp__sub_001EDBEC)

X86_FUNC(__imp__sub_001EDC08) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDC08 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDC0D push 0x13B
	{ uint32_t v = (uint32_t)(0x13Bu); X86_PUSH32(v); }
	// 001EDC12 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDC17 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDC1A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDC1B call 0x000127D0
	X86_PUSH32(0x1EDC20u); sub_000127D0(ctx, base);
	// 001EDC20 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDC23 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDC08, __imp__sub_001EDC08)

X86_FUNC(__imp__sub_001EDC24) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDC24 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDC29 push 0x13C
	{ uint32_t v = (uint32_t)(0x13Cu); X86_PUSH32(v); }
	// 001EDC2E push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDC33 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDC36 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDC37 call 0x000127D0
	X86_PUSH32(0x1EDC3Cu); sub_000127D0(ctx, base);
	// 001EDC3C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDC3F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDC24, __imp__sub_001EDC24)

X86_FUNC(__imp__sub_001EDC40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDC40 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDC45 push 0x13D
	{ uint32_t v = (uint32_t)(0x13Du); X86_PUSH32(v); }
	// 001EDC4A push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDC4F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDC52 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDC53 call 0x000127D0
	X86_PUSH32(0x1EDC58u); sub_000127D0(ctx, base);
	// 001EDC58 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDC5B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDC40, __imp__sub_001EDC40)

X86_FUNC(__imp__sub_001EDC5C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDC5C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDC61 push 0x13E
	{ uint32_t v = (uint32_t)(0x13Eu); X86_PUSH32(v); }
	// 001EDC66 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDC6B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDC6E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDC6F call 0x000127D0
	X86_PUSH32(0x1EDC74u); sub_000127D0(ctx, base);
	// 001EDC74 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDC77 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDC5C, __imp__sub_001EDC5C)

X86_FUNC(__imp__sub_001EDC78) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDC78 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDC7D push 0x13F
	{ uint32_t v = (uint32_t)(0x13Fu); X86_PUSH32(v); }
	// 001EDC82 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDC87 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDC8A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDC8B call 0x000127D0
	X86_PUSH32(0x1EDC90u); sub_000127D0(ctx, base);
	// 001EDC90 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDC93 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDC78, __imp__sub_001EDC78)

X86_FUNC(__imp__sub_001EDC94) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDC94 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDC99 push 0x140
	{ uint32_t v = (uint32_t)(0x140u); X86_PUSH32(v); }
	// 001EDC9E push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDCA3 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDCA6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDCA7 call 0x000127D0
	X86_PUSH32(0x1EDCACu); sub_000127D0(ctx, base);
	// 001EDCAC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDCAF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDC94, __imp__sub_001EDC94)

X86_FUNC(__imp__sub_001EDCB0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDCB0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDCB5 push 0x141
	{ uint32_t v = (uint32_t)(0x141u); X86_PUSH32(v); }
	// 001EDCBA push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDCBF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDCC2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDCC3 call 0x000127D0
	X86_PUSH32(0x1EDCC8u); sub_000127D0(ctx, base);
	// 001EDCC8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDCCB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDCB0, __imp__sub_001EDCB0)

X86_FUNC(__imp__sub_001EDCCC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDCCC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDCD1 push 0x143
	{ uint32_t v = (uint32_t)(0x143u); X86_PUSH32(v); }
	// 001EDCD6 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDCDB mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDCDE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDCDF call 0x000127D0
	X86_PUSH32(0x1EDCE4u); sub_000127D0(ctx, base);
	// 001EDCE4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDCE7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDCCC, __imp__sub_001EDCCC)

X86_FUNC(__imp__sub_001EDCE8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDCE8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDCED push 0x144
	{ uint32_t v = (uint32_t)(0x144u); X86_PUSH32(v); }
	// 001EDCF2 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDCF7 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDCFA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDCFB call 0x000127D0
	X86_PUSH32(0x1EDD00u); sub_000127D0(ctx, base);
	// 001EDD00 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDD03 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDCE8, __imp__sub_001EDCE8)

X86_FUNC(__imp__sub_001EDD04) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDD04 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDD09 push 0x145
	{ uint32_t v = (uint32_t)(0x145u); X86_PUSH32(v); }
	// 001EDD0E push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDD13 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDD16 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDD17 call 0x000127D0
	X86_PUSH32(0x1EDD1Cu); sub_000127D0(ctx, base);
	// 001EDD1C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDD1F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDD04, __imp__sub_001EDD04)

X86_FUNC(__imp__sub_001EDD20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDD20 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDD25 push 0x146
	{ uint32_t v = (uint32_t)(0x146u); X86_PUSH32(v); }
	// 001EDD2A push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDD2F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDD32 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDD33 call 0x000127D0
	X86_PUSH32(0x1EDD38u); sub_000127D0(ctx, base);
	// 001EDD38 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDD3B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDD20, __imp__sub_001EDD20)

X86_FUNC(__imp__sub_001EDD3C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDD3C mov eax, 0x27DF60
	{ ctx.eax = (uint32_t)(0x27DF60u); }
	// 001EDD41 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDD3C, __imp__sub_001EDD3C)

X86_FUNC(__imp__sub_001EDD50) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDD50 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDD55 push 0x2AB
	{ uint32_t v = (uint32_t)(0x2ABu); X86_PUSH32(v); }
	// 001EDD5A push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDD5F mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDD65 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDD66 call 0x000127D0
	X86_PUSH32(0x1EDD6Bu); sub_000127D0(ctx, base);
	// 001EDD6B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDD6E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDD50, __imp__sub_001EDD50)

X86_FUNC(__imp__sub_001EDD6F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDD6F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDD74 push 0x2F6
	{ uint32_t v = (uint32_t)(0x2F6u); X86_PUSH32(v); }
	// 001EDD79 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDD7E mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDD84 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDD85 call 0x000127D0
	X86_PUSH32(0x1EDD8Au); sub_000127D0(ctx, base);
	// 001EDD8A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDD8D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDD6F, __imp__sub_001EDD6F)

X86_FUNC(__imp__sub_001EDD8E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDD8E push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDD93 push 0x32B
	{ uint32_t v = (uint32_t)(0x32Bu); X86_PUSH32(v); }
	// 001EDD98 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDD9D mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDDA3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDDA4 call 0x000127D0
	X86_PUSH32(0x1EDDA9u); sub_000127D0(ctx, base);
	// 001EDDA9 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDDAC ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDD8E, __imp__sub_001EDD8E)

X86_FUNC(__imp__sub_001EDDAD) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDDAD push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDDB2 push 0x35B
	{ uint32_t v = (uint32_t)(0x35Bu); X86_PUSH32(v); }
	// 001EDDB7 push 0x2574E0
	{ uint32_t v = (uint32_t)(0x2574E0u); X86_PUSH32(v); }
	// 001EDDBC mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDDC2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDDC3 call 0x000127D0
	X86_PUSH32(0x1EDDC8u); sub_000127D0(ctx, base);
	// 001EDDC8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDDCB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDDAD, __imp__sub_001EDDAD)

X86_FUNC(__imp__sub_001EDDCC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDDCC mov eax, 0x27DF9C
	{ ctx.eax = (uint32_t)(0x27DF9Cu); }
	// 001EDDD1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDDCC, __imp__sub_001EDDCC)

X86_FUNC(__imp__sub_001EDDE0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_001EDDE0;
loc_0002DC00:
	// 0002DC00 mov dword ptr [ecx], 0x257604
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(0x257604u); }
	// 0002DC06 ret
	ctx.esp += 4; return;
loc_001EDDE0:
	// 001EDDE0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDDE3 jmp 0x0002DC00
	X86_BARRIER();
	goto loc_0002DC00;
}
X86_WEAK_FUNC(sub_001EDDE0, __imp__sub_001EDDE0)

X86_FUNC(__imp__sub_001EDDE8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDDE8 mov eax, 0x27DFC0
	{ ctx.eax = (uint32_t)(0x27DFC0u); }
	// 001EDDED jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDDE8, __imp__sub_001EDDE8)

X86_FUNC(__imp__sub_001EDE00) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_001EDE00;
loc_0002DC00:
	// 0002DC00 mov dword ptr [ecx], 0x257604
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(0x257604u); }
	// 0002DC06 ret
	ctx.esp += 4; return;
loc_001EDE00:
	// 001EDE00 mov ecx, [ebp-0x30]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDE03 jmp 0x0002DC00
	X86_BARRIER();
	goto loc_0002DC00;
}
X86_WEAK_FUNC(sub_001EDE00, __imp__sub_001EDE00)

X86_FUNC(__imp__sub_001EDE08) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE08 mov eax, 0x27E02C
	{ ctx.eax = (uint32_t)(0x27E02Cu); }
	// 001EDE0D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDE08, __imp__sub_001EDE08)

X86_FUNC(__imp__sub_001EDE20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE20 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDE23 add ecx, 0xDC
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xDCu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDE29 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDE20, __imp__sub_001EDE20)

X86_FUNC(__imp__sub_001EDE2E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE2E mov eax, 0x27E074
	{ ctx.eax = (uint32_t)(0x27E074u); }
	// 001EDE33 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDE2E, __imp__sub_001EDE2E)

X86_FUNC(__imp__sub_001EDE40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE40 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDE45 push 0x168
	{ uint32_t v = (uint32_t)(0x168u); X86_PUSH32(v); }
	// 001EDE4A push 0x25769C
	{ uint32_t v = (uint32_t)(0x25769Cu); X86_PUSH32(v); }
	// 001EDE4F mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDE52 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDE53 call 0x000127D0
	X86_PUSH32(0x1EDE58u); sub_000127D0(ctx, base);
	// 001EDE58 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDE5B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDE40, __imp__sub_001EDE40)

X86_FUNC(__imp__sub_001EDE5C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE5C mov eax, 0x27E098
	{ ctx.eax = (uint32_t)(0x27E098u); }
	// 001EDE61 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDE5C, __imp__sub_001EDE5C)

X86_FUNC(__imp__sub_001EDE70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDE75 push 0x184
	{ uint32_t v = (uint32_t)(0x184u); X86_PUSH32(v); }
	// 001EDE7A push 0x25769C
	{ uint32_t v = (uint32_t)(0x25769Cu); X86_PUSH32(v); }
	// 001EDE7F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDE82 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDE83 call 0x000127D0
	X86_PUSH32(0x1EDE88u); sub_000127D0(ctx, base);
	// 001EDE88 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDE8B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDE70, __imp__sub_001EDE70)

X86_FUNC(__imp__sub_001EDE8C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDE8C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDE91 push 0x188
	{ uint32_t v = (uint32_t)(0x188u); X86_PUSH32(v); }
	// 001EDE96 push 0x25769C
	{ uint32_t v = (uint32_t)(0x25769Cu); X86_PUSH32(v); }
	// 001EDE9B mov eax, [ebp+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDE9E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDE9F call 0x000127D0
	X86_PUSH32(0x1EDEA4u); sub_000127D0(ctx, base);
	// 001EDEA4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDEA7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDE8C, __imp__sub_001EDE8C)

X86_FUNC(__imp__sub_001EDEA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDEA8 mov eax, 0x27E0C4
	{ ctx.eax = (uint32_t)(0x27E0C4u); }
	// 001EDEAD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDEA8, __imp__sub_001EDEA8)

X86_FUNC(__imp__sub_001EDEC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDEC0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDEC5 push 0x19C
	{ uint32_t v = (uint32_t)(0x19Cu); X86_PUSH32(v); }
	// 001EDECA push 0x25769C
	{ uint32_t v = (uint32_t)(0x25769Cu); X86_PUSH32(v); }
	// 001EDECF mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDED2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDED3 call 0x000127D0
	X86_PUSH32(0x1EDED8u); sub_000127D0(ctx, base);
	// 001EDED8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDEDB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDEC0, __imp__sub_001EDEC0)

X86_FUNC(__imp__sub_001EDEDC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDEDC mov eax, 0x27E0E8
	{ ctx.eax = (uint32_t)(0x27E0E8u); }
	// 001EDEE1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDEDC, __imp__sub_001EDEDC)

X86_FUNC(__imp__sub_001EDEF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDEF0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDEF3 jmp 0x00012860
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00012860(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDEF0, __imp__sub_001EDEF0)

X86_FUNC(__imp__sub_001EDEF8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDEF8 mov eax, 0x27E154
	{ ctx.eax = (uint32_t)(0x27E154u); }
	// 001EDEFD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDEF8, __imp__sub_001EDEF8)

X86_FUNC(__imp__sub_001EDF10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDF10 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDF15 push 0x134
	{ uint32_t v = (uint32_t)(0x134u); X86_PUSH32(v); }
	// 001EDF1A push 0x262D04
	{ uint32_t v = (uint32_t)(0x262D04u); X86_PUSH32(v); }
	// 001EDF1F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDF22 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDF23 call 0x000127D0
	X86_PUSH32(0x1EDF28u); sub_000127D0(ctx, base);
	// 001EDF28 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDF2B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDF10, __imp__sub_001EDF10)

X86_FUNC(__imp__sub_001EDF2C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDF2C mov eax, 0x27E178
	{ ctx.eax = (uint32_t)(0x27E178u); }
	// 001EDF31 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDF2C, __imp__sub_001EDF2C)

X86_FUNC(__imp__sub_001EDF40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDF40 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDF45 push 0x32D
	{ uint32_t v = (uint32_t)(0x32Du); X86_PUSH32(v); }
	// 001EDF4A push 0x262D04
	{ uint32_t v = (uint32_t)(0x262D04u); X86_PUSH32(v); }
	// 001EDF4F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDF52 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDF53 call 0x000127D0
	X86_PUSH32(0x1EDF58u); sub_000127D0(ctx, base);
	// 001EDF58 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDF5B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDF40, __imp__sub_001EDF40)

X86_FUNC(__imp__sub_001EDF5C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDF5C mov eax, 0x27E19C
	{ ctx.eax = (uint32_t)(0x27E19Cu); }
	// 001EDF61 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDF5C, __imp__sub_001EDF5C)

X86_FUNC(__imp__sub_001EDF70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDF70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDF75 push 0x5C
	{ uint32_t v = (uint32_t)(0x5Cu); X86_PUSH32(v); }
	// 001EDF77 push 0x262D04
	{ uint32_t v = (uint32_t)(0x262D04u); X86_PUSH32(v); }
	// 001EDF7C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDF7F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDF80 call 0x000127D0
	X86_PUSH32(0x1EDF85u); sub_000127D0(ctx, base);
	// 001EDF85 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDF88 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDF70, __imp__sub_001EDF70)

X86_FUNC(__imp__sub_001EDF89) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDF89 mov eax, 0x27E1C0
	{ ctx.eax = (uint32_t)(0x27E1C0u); }
	// 001EDF8E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDF89, __imp__sub_001EDF89)

X86_FUNC(__imp__sub_001EDFA0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDFA0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EDFA3 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDFA0, __imp__sub_001EDFA0)

X86_FUNC(__imp__sub_001EDFA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDFA8 mov eax, 0x27E1E4
	{ ctx.eax = (uint32_t)(0x27E1E4u); }
	// 001EDFAD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDFA8, __imp__sub_001EDFA8)

X86_FUNC(__imp__sub_001EDFC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDFC0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDFC5 push 0x3F
	{ uint32_t v = (uint32_t)(0x3Fu); X86_PUSH32(v); }
	// 001EDFC7 push 0x262ECC
	{ uint32_t v = (uint32_t)(0x262ECCu); X86_PUSH32(v); }
	// 001EDFCC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EDFCF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EDFD0 call 0x000127D0
	X86_PUSH32(0x1EDFD5u); sub_000127D0(ctx, base);
	// 001EDFD5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EDFD8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDFC0, __imp__sub_001EDFC0)

X86_FUNC(__imp__sub_001EDFD9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDFD9 mov eax, 0x27E208
	{ ctx.eax = (uint32_t)(0x27E208u); }
	// 001EDFDE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EDFD9, __imp__sub_001EDFD9)

X86_FUNC(__imp__sub_001EDFF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EDFF0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EDFF5 push 0x301
	{ uint32_t v = (uint32_t)(0x301u); X86_PUSH32(v); }
	// 001EDFFA push 0x26324C
	{ uint32_t v = (uint32_t)(0x26324Cu); X86_PUSH32(v); }
	// 001EDFFF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE002 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE003 call 0x000127D0
	X86_PUSH32(0x1EE008u); sub_000127D0(ctx, base);
	// 001EE008 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE00B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EDFF0, __imp__sub_001EDFF0)

X86_FUNC(__imp__sub_001EE00C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE00C mov eax, 0x27E22C
	{ ctx.eax = (uint32_t)(0x27E22Cu); }
	// 001EE011 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE00C, __imp__sub_001EE00C)

X86_FUNC(__imp__sub_001EE020) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE020 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE025 push 0x13A
	{ uint32_t v = (uint32_t)(0x13Au); X86_PUSH32(v); }
	// 001EE02A push 0x26324C
	{ uint32_t v = (uint32_t)(0x26324Cu); X86_PUSH32(v); }
	// 001EE02F mov eax, [ebp-0x430]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBD0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE035 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE036 call 0x000127D0
	X86_PUSH32(0x1EE03Bu); sub_000127D0(ctx, base);
	// 001EE03B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE03E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE020, __imp__sub_001EE020)

X86_FUNC(__imp__sub_001EE03F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE03F mov eax, 0x27E250
	{ ctx.eax = (uint32_t)(0x27E250u); }
	// 001EE044 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE03F, __imp__sub_001EE03F)

X86_FUNC(__imp__sub_001EE050) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE050 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE055 push 0x3E4
	{ uint32_t v = (uint32_t)(0x3E4u); X86_PUSH32(v); }
	// 001EE05A push 0x26324C
	{ uint32_t v = (uint32_t)(0x26324Cu); X86_PUSH32(v); }
	// 001EE05F mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE062 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE063 call 0x000127D0
	X86_PUSH32(0x1EE068u); sub_000127D0(ctx, base);
	// 001EE068 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE06B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE050, __imp__sub_001EE050)

X86_FUNC(__imp__sub_001EE06C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE06C mov eax, 0x27E274
	{ ctx.eax = (uint32_t)(0x27E274u); }
	// 001EE071 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE06C, __imp__sub_001EE06C)

X86_FUNC(__imp__sub_001EE080) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE080 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE085 push 0x2D
	{ uint32_t v = (uint32_t)(0x2Du); X86_PUSH32(v); }
	// 001EE087 push 0x2632A0
	{ uint32_t v = (uint32_t)(0x2632A0u); X86_PUSH32(v); }
	// 001EE08C mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE08F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE090 call 0x000127D0
	X86_PUSH32(0x1EE095u); sub_000127D0(ctx, base);
	// 001EE095 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE098 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE080, __imp__sub_001EE080)

X86_FUNC(__imp__sub_001EE099) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE099 mov eax, 0x27E298
	{ ctx.eax = (uint32_t)(0x27E298u); }
	// 001EE09E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE099, __imp__sub_001EE099)

X86_FUNC(__imp__sub_001EE0B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE0B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE0B5 push 0xDF
	{ uint32_t v = (uint32_t)(0xDFu); X86_PUSH32(v); }
	// 001EE0BA push 0x2632A0
	{ uint32_t v = (uint32_t)(0x2632A0u); X86_PUSH32(v); }
	// 001EE0BF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE0C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE0C3 call 0x000127D0
	X86_PUSH32(0x1EE0C8u); sub_000127D0(ctx, base);
	// 001EE0C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE0CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE0B0, __imp__sub_001EE0B0)

X86_FUNC(__imp__sub_001EE0CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE0CC mov eax, 0x27E2BC
	{ ctx.eax = (uint32_t)(0x27E2BCu); }
	// 001EE0D1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE0CC, __imp__sub_001EE0CC)

X86_FUNC(__imp__sub_001EE0E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE0E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE0E5 push 0x17B
	{ uint32_t v = (uint32_t)(0x17Bu); X86_PUSH32(v); }
	// 001EE0EA push 0x2632A0
	{ uint32_t v = (uint32_t)(0x2632A0u); X86_PUSH32(v); }
	// 001EE0EF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE0F2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE0F3 call 0x000127D0
	X86_PUSH32(0x1EE0F8u); sub_000127D0(ctx, base);
	// 001EE0F8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE0FB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE0E0, __imp__sub_001EE0E0)

X86_FUNC(__imp__sub_001EE0FC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE0FC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE101 push 0x17C
	{ uint32_t v = (uint32_t)(0x17Cu); X86_PUSH32(v); }
	// 001EE106 push 0x2632A0
	{ uint32_t v = (uint32_t)(0x2632A0u); X86_PUSH32(v); }
	// 001EE10B mov eax, [ebp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE10E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE10F call 0x000127D0
	X86_PUSH32(0x1EE114u); sub_000127D0(ctx, base);
	// 001EE114 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE117 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE0FC, __imp__sub_001EE0FC)

X86_FUNC(__imp__sub_001EE118) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE118 mov eax, 0x27E2E8
	{ ctx.eax = (uint32_t)(0x27E2E8u); }
	// 001EE11D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE118, __imp__sub_001EE118)

X86_FUNC(__imp__sub_001EE130) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE130 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE135 push 0xA3
	{ uint32_t v = (uint32_t)(0xA3u); X86_PUSH32(v); }
	// 001EE13A push 0x263444
	{ uint32_t v = (uint32_t)(0x263444u); X86_PUSH32(v); }
	// 001EE13F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE142 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE143 call 0x000127D0
	X86_PUSH32(0x1EE148u); sub_000127D0(ctx, base);
	// 001EE148 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE14B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE130, __imp__sub_001EE130)

X86_FUNC(__imp__sub_001EE14C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE14C mov eax, 0x27E30C
	{ ctx.eax = (uint32_t)(0x27E30Cu); }
	// 001EE151 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE14C, __imp__sub_001EE14C)

X86_FUNC(__imp__sub_001EE160) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE160 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE165 push 0xEF
	{ uint32_t v = (uint32_t)(0xEFu); X86_PUSH32(v); }
	// 001EE16A push 0x263444
	{ uint32_t v = (uint32_t)(0x263444u); X86_PUSH32(v); }
	// 001EE16F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE172 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE173 call 0x000127D0
	X86_PUSH32(0x1EE178u); sub_000127D0(ctx, base);
	// 001EE178 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE17B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE160, __imp__sub_001EE160)

X86_FUNC(__imp__sub_001EE17C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE17C mov eax, 0x27E330
	{ ctx.eax = (uint32_t)(0x27E330u); }
	// 001EE181 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE17C, __imp__sub_001EE17C)

X86_FUNC(__imp__sub_001EE190) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE190 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE195 push 0x5C
	{ uint32_t v = (uint32_t)(0x5Cu); X86_PUSH32(v); }
	// 001EE197 push 0x263570
	{ uint32_t v = (uint32_t)(0x263570u); X86_PUSH32(v); }
	// 001EE19C mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE19F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE1A0 call 0x000127D0
	X86_PUSH32(0x1EE1A5u); sub_000127D0(ctx, base);
	// 001EE1A5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE1A8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE190, __imp__sub_001EE190)

X86_FUNC(__imp__sub_001EE1A9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE1A9 mov eax, 0x27E354
	{ ctx.eax = (uint32_t)(0x27E354u); }
	// 001EE1AE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE1A9, __imp__sub_001EE1A9)

X86_FUNC(__imp__sub_001EE1C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE1C0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE1C5 push 0x500
	{ uint32_t v = (uint32_t)(0x500u); X86_PUSH32(v); }
	// 001EE1CA push 0x263680
	{ uint32_t v = (uint32_t)(0x263680u); X86_PUSH32(v); }
	// 001EE1CF mov eax, [ebp-0x68]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF98u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE1D2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE1D3 call 0x000127D0
	X86_PUSH32(0x1EE1D8u); sub_000127D0(ctx, base);
	// 001EE1D8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE1DB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE1C0, __imp__sub_001EE1C0)

X86_FUNC(__imp__sub_001EE1DC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE1DC mov eax, 0x27E378
	{ ctx.eax = (uint32_t)(0x27E378u); }
	// 001EE1E1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE1DC, __imp__sub_001EE1DC)

X86_FUNC(__imp__sub_001EE1F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE1F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE1F5 push 0x1A4
	{ uint32_t v = (uint32_t)(0x1A4u); X86_PUSH32(v); }
	// 001EE1FA push 0x263680
	{ uint32_t v = (uint32_t)(0x263680u); X86_PUSH32(v); }
	// 001EE1FF mov eax, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE202 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE203 call 0x000127D0
	X86_PUSH32(0x1EE208u); sub_000127D0(ctx, base);
	// 001EE208 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE20B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE1F0, __imp__sub_001EE1F0)

X86_FUNC(__imp__sub_001EE20C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE20C mov eax, 0x27E39C
	{ ctx.eax = (uint32_t)(0x27E39Cu); }
	// 001EE211 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE20C, __imp__sub_001EE20C)

X86_FUNC(__imp__sub_001EE220) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE220 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE225 push 0x154
	{ uint32_t v = (uint32_t)(0x154u); X86_PUSH32(v); }
	// 001EE22A push 0x263680
	{ uint32_t v = (uint32_t)(0x263680u); X86_PUSH32(v); }
	// 001EE22F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE232 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE233 call 0x000127D0
	X86_PUSH32(0x1EE238u); sub_000127D0(ctx, base);
	// 001EE238 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE23B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE220, __imp__sub_001EE220)

X86_FUNC(__imp__sub_001EE23C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE23C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE241 push 0x158
	{ uint32_t v = (uint32_t)(0x158u); X86_PUSH32(v); }
	// 001EE246 push 0x263680
	{ uint32_t v = (uint32_t)(0x263680u); X86_PUSH32(v); }
	// 001EE24B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE24E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE24F call 0x000127D0
	X86_PUSH32(0x1EE254u); sub_000127D0(ctx, base);
	// 001EE254 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE257 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE23C, __imp__sub_001EE23C)

X86_FUNC(__imp__sub_001EE258) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE258 mov eax, 0x27E3C8
	{ ctx.eax = (uint32_t)(0x27E3C8u); }
	// 001EE25D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE258, __imp__sub_001EE258)

X86_FUNC(__imp__sub_001EE270) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE270 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE275 push 0x41
	{ uint32_t v = (uint32_t)(0x41u); X86_PUSH32(v); }
	// 001EE277 push 0x2637A0
	{ uint32_t v = (uint32_t)(0x2637A0u); X86_PUSH32(v); }
	// 001EE27C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE27F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE280 call 0x000127D0
	X86_PUSH32(0x1EE285u); sub_000127D0(ctx, base);
	// 001EE285 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE288 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE270, __imp__sub_001EE270)

X86_FUNC(__imp__sub_001EE289) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE289 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE28E push 0x68
	{ uint32_t v = (uint32_t)(0x68u); X86_PUSH32(v); }
	// 001EE290 push 0x2637A0
	{ uint32_t v = (uint32_t)(0x2637A0u); X86_PUSH32(v); }
	// 001EE295 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE298 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE299 call 0x000127D0
	X86_PUSH32(0x1EE29Eu); sub_000127D0(ctx, base);
	// 001EE29E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE2A1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE289, __imp__sub_001EE289)

X86_FUNC(__imp__sub_001EE2A2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE2A2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE2A7 push 0x69
	{ uint32_t v = (uint32_t)(0x69u); X86_PUSH32(v); }
	// 001EE2A9 push 0x2637A0
	{ uint32_t v = (uint32_t)(0x2637A0u); X86_PUSH32(v); }
	// 001EE2AE mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE2B1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE2B2 call 0x000127D0
	X86_PUSH32(0x1EE2B7u); sub_000127D0(ctx, base);
	// 001EE2B7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE2BA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE2A2, __imp__sub_001EE2A2)

X86_FUNC(__imp__sub_001EE2BB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE2BB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE2C0 push 0x6A
	{ uint32_t v = (uint32_t)(0x6Au); X86_PUSH32(v); }
	// 001EE2C2 push 0x2637A0
	{ uint32_t v = (uint32_t)(0x2637A0u); X86_PUSH32(v); }
	// 001EE2C7 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE2CA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE2CB call 0x000127D0
	X86_PUSH32(0x1EE2D0u); sub_000127D0(ctx, base);
	// 001EE2D0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE2D3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE2BB, __imp__sub_001EE2BB)

X86_FUNC(__imp__sub_001EE2D4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE2D4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE2D9 push 0x6B
	{ uint32_t v = (uint32_t)(0x6Bu); X86_PUSH32(v); }
	// 001EE2DB push 0x2637A0
	{ uint32_t v = (uint32_t)(0x2637A0u); X86_PUSH32(v); }
	// 001EE2E0 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE2E3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE2E4 call 0x000127D0
	X86_PUSH32(0x1EE2E9u); sub_000127D0(ctx, base);
	// 001EE2E9 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE2EC ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE2D4, __imp__sub_001EE2D4)

X86_FUNC(__imp__sub_001EE2ED) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE2ED mov eax, 0x27E40C
	{ ctx.eax = (uint32_t)(0x27E40Cu); }
	// 001EE2F2 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE2ED, __imp__sub_001EE2ED)

X86_FUNC(__imp__sub_001EE300) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE300 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE303 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE306 jmp 0x0008BF90
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0008BF90(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE300, __imp__sub_001EE300)

X86_FUNC(__imp__sub_001EE30B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE30B mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE30E add ecx, 0x5C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x5Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE311 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE30B, __imp__sub_001EE30B)

X86_FUNC(__imp__sub_001EE316) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE316 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE319 add ecx, 0x2640
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2640u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE31F jmp 0x0008D1EE
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0008D1EE(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE316, __imp__sub_001EE316)

X86_FUNC(__imp__sub_001EE324) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE324 push 0x86B90
	{ uint32_t v = (uint32_t)(0x86B90u); X86_PUSH32(v); }
	// 001EE329 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EE32B push 0x2E0
	{ uint32_t v = (uint32_t)(0x2E0u); X86_PUSH32(v); }
	// 001EE330 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE333 add eax, 0x2750
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x2750u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE338 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE339 call 0x001DEF24
	X86_PUSH32(0x1EE33Eu); sub_001DEF24(ctx, base);
	// 001EE33E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE324, __imp__sub_001EE324)

X86_FUNC(__imp__sub_001EE33F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE33F mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE342 add ecx, 0x3310
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3310u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE348 jmp 0x000919F0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000919F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE33F, __imp__sub_001EE33F)

X86_FUNC(__imp__sub_001EE34D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE34D mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE350 add ecx, 0x3320
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3320u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE356 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE34D, __imp__sub_001EE34D)

X86_FUNC(__imp__sub_001EE35B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE35B mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE35E add ecx, 0x3604
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3604u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE364 jmp 0x000924F0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000924F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE35B, __imp__sub_001EE35B)

X86_FUNC(__imp__sub_001EE369) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE369 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE36C add ecx, 0x3F28
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3F28u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE372 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE369, __imp__sub_001EE369)

X86_FUNC(__imp__sub_001EE377) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE377 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE37A add ecx, 0x4B50
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x4B50u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE380 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE377, __imp__sub_001EE377)

X86_FUNC(__imp__sub_001EE385) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE385 mov eax, 0x27E4D4
	{ ctx.eax = (uint32_t)(0x27E4D4u); }
	// 001EE38A jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE385, __imp__sub_001EE385)

X86_FUNC(__imp__sub_001EE390) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE390 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE395 push 0x6C
	{ uint32_t v = (uint32_t)(0x6Cu); X86_PUSH32(v); }
	// 001EE397 push 0x263BA8
	{ uint32_t v = (uint32_t)(0x263BA8u); X86_PUSH32(v); }
	// 001EE39C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE39F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE3A0 call 0x000127D0
	X86_PUSH32(0x1EE3A5u); sub_000127D0(ctx, base);
	// 001EE3A5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE3A8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE390, __imp__sub_001EE390)

X86_FUNC(__imp__sub_001EE3A9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE3A9 mov eax, 0x27E4F8
	{ ctx.eax = (uint32_t)(0x27E4F8u); }
	// 001EE3AE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE3A9, __imp__sub_001EE3A9)

X86_FUNC(__imp__sub_001EE3C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE3C0 lea ecx, [ebp-0x220]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDE0u));
	// 001EE3C6 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE3C0, __imp__sub_001EE3C0)

X86_FUNC(__imp__sub_001EE3CB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE3CB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE3D0 push 0x1CC
	{ uint32_t v = (uint32_t)(0x1CCu); X86_PUSH32(v); }
	// 001EE3D5 push 0x263BA8
	{ uint32_t v = (uint32_t)(0x263BA8u); X86_PUSH32(v); }
	// 001EE3DA mov eax, [ebp-0x228]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDD8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE3E0 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE3E1 call 0x000127D0
	X86_PUSH32(0x1EE3E6u); sub_000127D0(ctx, base);
	// 001EE3E6 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE3E9 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE3CB, __imp__sub_001EE3CB)

X86_FUNC(__imp__sub_001EE3EA) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE3EA push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE3EF push 0x1CF
	{ uint32_t v = (uint32_t)(0x1CFu); X86_PUSH32(v); }
	// 001EE3F4 push 0x263BA8
	{ uint32_t v = (uint32_t)(0x263BA8u); X86_PUSH32(v); }
	// 001EE3F9 mov eax, [ebp-0x228]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDD8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE3FF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE400 call 0x000127D0
	X86_PUSH32(0x1EE405u); sub_000127D0(ctx, base);
	// 001EE405 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE408 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE3EA, __imp__sub_001EE3EA)

X86_FUNC(__imp__sub_001EE409) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE409 mov eax, 0x27E52C
	{ ctx.eax = (uint32_t)(0x27E52Cu); }
	// 001EE40E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE409, __imp__sub_001EE409)

X86_FUNC(__imp__sub_001EE420) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE420 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE423 add ecx, 0x40
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE426 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE420, __imp__sub_001EE420)

X86_FUNC(__imp__sub_001EE42B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE42B mov eax, 0x27E550
	{ ctx.eax = (uint32_t)(0x27E550u); }
	// 001EE430 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE42B, __imp__sub_001EE42B)

X86_FUNC(__imp__sub_001EE440) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE440 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE445 push 0x70
	{ uint32_t v = (uint32_t)(0x70u); X86_PUSH32(v); }
	// 001EE447 push 0x2645F8
	{ uint32_t v = (uint32_t)(0x2645F8u); X86_PUSH32(v); }
	// 001EE44C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE44F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE450 call 0x000127D0
	X86_PUSH32(0x1EE455u); sub_000127D0(ctx, base);
	// 001EE455 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE458 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE440, __imp__sub_001EE440)

X86_FUNC(__imp__sub_001EE459) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE459 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE45E push 0x8B
	{ uint32_t v = (uint32_t)(0x8Bu); X86_PUSH32(v); }
	// 001EE463 push 0x2645F8
	{ uint32_t v = (uint32_t)(0x2645F8u); X86_PUSH32(v); }
	// 001EE468 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE46B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE46C call 0x000127D0
	X86_PUSH32(0x1EE471u); sub_000127D0(ctx, base);
	// 001EE471 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE474 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE459, __imp__sub_001EE459)

X86_FUNC(__imp__sub_001EE475) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE475 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE47A push 0xA4
	{ uint32_t v = (uint32_t)(0xA4u); X86_PUSH32(v); }
	// 001EE47F push 0x2645F8
	{ uint32_t v = (uint32_t)(0x2645F8u); X86_PUSH32(v); }
	// 001EE484 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE487 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE488 call 0x000127D0
	X86_PUSH32(0x1EE48Du); sub_000127D0(ctx, base);
	// 001EE48D add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE490 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE475, __imp__sub_001EE475)

X86_FUNC(__imp__sub_001EE491) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE491 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE496 push 0xBE
	{ uint32_t v = (uint32_t)(0xBEu); X86_PUSH32(v); }
	// 001EE49B push 0x2645F8
	{ uint32_t v = (uint32_t)(0x2645F8u); X86_PUSH32(v); }
	// 001EE4A0 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE4A3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE4A4 call 0x000127D0
	X86_PUSH32(0x1EE4A9u); sub_000127D0(ctx, base);
	// 001EE4A9 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE4AC ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE491, __imp__sub_001EE491)

X86_FUNC(__imp__sub_001EE4AD) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE4AD mov eax, 0x27E5B0
	{ ctx.eax = (uint32_t)(0x27E5B0u); }
	// 001EE4B2 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE4AD, __imp__sub_001EE4AD)

X86_FUNC(__imp__sub_001EE4C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE4C0 lea ecx, [ebp-0x23C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDC4u));
	// 001EE4C6 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE4C0, __imp__sub_001EE4C0)

X86_FUNC(__imp__sub_001EE4CB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE4CB mov eax, 0x27E5D4
	{ ctx.eax = (uint32_t)(0x27E5D4u); }
	// 001EE4D0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE4CB, __imp__sub_001EE4CB)

X86_FUNC(__imp__sub_001EE4D5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE4D5 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE4DA push 0x3D8
	{ uint32_t v = (uint32_t)(0x3D8u); X86_PUSH32(v); }
	// 001EE4DF push 0x292864
	{ uint32_t v = (uint32_t)(0x292864u); X86_PUSH32(v); }
	// 001EE4E4 mov eax, [ebp-0x64]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF9Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE4E7 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE4E8 call 0x000127D0
	X86_PUSH32(0x1EE4EDu); sub_000127D0(ctx, base);
	// 001EE4ED add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE4F0 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE4D5, __imp__sub_001EE4D5)

X86_FUNC(__imp__sub_001EE4F1) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE4F1 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE4F6 push 0x3ED
	{ uint32_t v = (uint32_t)(0x3EDu); X86_PUSH32(v); }
	// 001EE4FB push 0x2928CC
	{ uint32_t v = (uint32_t)(0x2928CCu); X86_PUSH32(v); }
	// 001EE500 mov eax, [ebp-0x74]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF8Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE503 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE504 call 0x000127D0
	X86_PUSH32(0x1EE509u); sub_000127D0(ctx, base);
	// 001EE509 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE50C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE4F1, __imp__sub_001EE4F1)

X86_FUNC(__imp__sub_001EE50D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE50D mov eax, 0x27E600
	{ ctx.eax = (uint32_t)(0x27E600u); }
	// 001EE512 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE50D, __imp__sub_001EE50D)

X86_FUNC(__imp__sub_001EE520) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE520 lea ecx, [ebp-0x220]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDE0u));
	// 001EE526 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE520, __imp__sub_001EE520)

X86_FUNC(__imp__sub_001EE52B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE52B mov eax, 0x27E624
	{ ctx.eax = (uint32_t)(0x27E624u); }
	// 001EE530 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE52B, __imp__sub_001EE52B)

X86_FUNC(__imp__sub_001EE540) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE540 lea ecx, [ebp-0x220]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDE0u));
	// 001EE546 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE540, __imp__sub_001EE540)

X86_FUNC(__imp__sub_001EE54B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE54B mov eax, 0x27E648
	{ ctx.eax = (uint32_t)(0x27E648u); }
	// 001EE550 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE54B, __imp__sub_001EE54B)

X86_FUNC(__imp__sub_001EE560) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE560 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE563 add ecx, 0xA0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xA0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE569 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE560, __imp__sub_001EE560)

X86_FUNC(__imp__sub_001EE56E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE56E mov eax, 0x27E66C
	{ ctx.eax = (uint32_t)(0x27E66Cu); }
	// 001EE573 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE56E, __imp__sub_001EE56E)

X86_FUNC(__imp__sub_001EE580) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE580 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE585 push 0x6E
	{ uint32_t v = (uint32_t)(0x6Eu); X86_PUSH32(v); }
	// 001EE587 push 0x26512C
	{ uint32_t v = (uint32_t)(0x26512Cu); X86_PUSH32(v); }
	// 001EE58C mov eax, [ebp-0x121C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFEDE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE592 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE593 call 0x000127D0
	X86_PUSH32(0x1EE598u); sub_000127D0(ctx, base);
	// 001EE598 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE59B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE580, __imp__sub_001EE580)

X86_FUNC(__imp__sub_001EE59C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE59C mov eax, 0x27E6B4
	{ ctx.eax = (uint32_t)(0x27E6B4u); }
	// 001EE5A1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE59C, __imp__sub_001EE59C)

X86_FUNC(__imp__sub_001EE5B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE5B0 lea ecx, [ebp-0x220]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDE0u));
	// 001EE5B6 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE5B0, __imp__sub_001EE5B0)

X86_FUNC(__imp__sub_001EE5BB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE5BB mov eax, 0x27E6D8
	{ ctx.eax = (uint32_t)(0x27E6D8u); }
	// 001EE5C0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE5BB, __imp__sub_001EE5BB)

X86_FUNC(__imp__sub_001EE5D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE5D0 lea ecx, [ebp-0x320]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFCE0u));
	// 001EE5D6 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE5D0, __imp__sub_001EE5D0)

X86_FUNC(__imp__sub_001EE5DB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE5DB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE5E0 push 0xF2
	{ uint32_t v = (uint32_t)(0xF2u); X86_PUSH32(v); }
	// 001EE5E5 push 0x26550C
	{ uint32_t v = (uint32_t)(0x26550Cu); X86_PUSH32(v); }
	// 001EE5EA mov eax, [ebp-0x32C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFCD4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE5F0 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE5F1 call 0x000127D0
	X86_PUSH32(0x1EE5F6u); sub_000127D0(ctx, base);
	// 001EE5F6 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE5F9 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE5DB, __imp__sub_001EE5DB)

X86_FUNC(__imp__sub_001EE5FA) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE5FA push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE5FF push 0x100
	{ uint32_t v = (uint32_t)(0x100u); X86_PUSH32(v); }
	// 001EE604 push 0x26550C
	{ uint32_t v = (uint32_t)(0x26550Cu); X86_PUSH32(v); }
	// 001EE609 mov eax, [ebp-0x32C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFCD4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE60F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE610 call 0x000127D0
	X86_PUSH32(0x1EE615u); sub_000127D0(ctx, base);
	// 001EE615 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE618 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE5FA, __imp__sub_001EE5FA)

X86_FUNC(__imp__sub_001EE619) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE619 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE61E push 0x103
	{ uint32_t v = (uint32_t)(0x103u); X86_PUSH32(v); }
	// 001EE623 push 0x26550C
	{ uint32_t v = (uint32_t)(0x26550Cu); X86_PUSH32(v); }
	// 001EE628 mov eax, [ebp-0x32C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFCD4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE62E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE62F call 0x000127D0
	X86_PUSH32(0x1EE634u); sub_000127D0(ctx, base);
	// 001EE634 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE637 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE619, __imp__sub_001EE619)

X86_FUNC(__imp__sub_001EE638) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE638 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE63D push 0x106
	{ uint32_t v = (uint32_t)(0x106u); X86_PUSH32(v); }
	// 001EE642 push 0x26550C
	{ uint32_t v = (uint32_t)(0x26550Cu); X86_PUSH32(v); }
	// 001EE647 mov eax, [ebp-0x32C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFCD4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE64D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE64E call 0x000127D0
	X86_PUSH32(0x1EE653u); sub_000127D0(ctx, base);
	// 001EE653 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE656 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE638, __imp__sub_001EE638)

X86_FUNC(__imp__sub_001EE657) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE657 mov eax, 0x27E71C
	{ ctx.eax = (uint32_t)(0x27E71Cu); }
	// 001EE65C jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE657, __imp__sub_001EE657)

X86_FUNC(__imp__sub_001EE670) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE670 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE673 add ecx, 0x160
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x160u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE679 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE670, __imp__sub_001EE670)

X86_FUNC(__imp__sub_001EE67E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE67E mov eax, 0x27E740
	{ ctx.eax = (uint32_t)(0x27E740u); }
	// 001EE683 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE67E, __imp__sub_001EE67E)

X86_FUNC(__imp__sub_001EE690) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE690 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE695 push 0x17
	{ uint32_t v = (uint32_t)(0x17u); X86_PUSH32(v); }
	// 001EE697 push 0x2657A4
	{ uint32_t v = (uint32_t)(0x2657A4u); X86_PUSH32(v); }
	// 001EE69C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE69F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE6A0 call 0x000127D0
	X86_PUSH32(0x1EE6A5u); sub_000127D0(ctx, base);
	// 001EE6A5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE6A8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE690, __imp__sub_001EE690)

X86_FUNC(__imp__sub_001EE6A9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE6A9 mov eax, 0x27E788
	{ ctx.eax = (uint32_t)(0x27E788u); }
	// 001EE6AE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE6A9, __imp__sub_001EE6A9)

X86_FUNC(__imp__sub_001EE6C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE6C0 lea ecx, [ebp-0x220]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDE0u));
	// 001EE6C6 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE6C0, __imp__sub_001EE6C0)

X86_FUNC(__imp__sub_001EE6CB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE6CB mov eax, 0x27E7F4
	{ ctx.eax = (uint32_t)(0x27E7F4u); }
	// 001EE6D0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE6CB, __imp__sub_001EE6CB)

X86_FUNC(__imp__sub_001EE6E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE6E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE6E5 push 0x15A
	{ uint32_t v = (uint32_t)(0x15Au); X86_PUSH32(v); }
	// 001EE6EA push 0x266614
	{ uint32_t v = (uint32_t)(0x266614u); X86_PUSH32(v); }
	// 001EE6EF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE6F2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE6F3 call 0x000127D0
	X86_PUSH32(0x1EE6F8u); sub_000127D0(ctx, base);
	// 001EE6F8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE6FB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE6E0, __imp__sub_001EE6E0)

X86_FUNC(__imp__sub_001EE6FC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE6FC mov eax, 0x27E818
	{ ctx.eax = (uint32_t)(0x27E818u); }
	// 001EE701 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE6FC, __imp__sub_001EE6FC)

X86_FUNC(__imp__sub_001EE710) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE710 lea ecx, [ebp-0x220]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFDE0u));
	// 001EE716 jmp 0x000A5BF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A5BF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE710, __imp__sub_001EE710)

X86_FUNC(__imp__sub_001EE71B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE71B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE720 push 0x1D
	{ uint32_t v = (uint32_t)(0x1Du); X86_PUSH32(v); }
	// 001EE722 push 0x2666EC
	{ uint32_t v = (uint32_t)(0x2666ECu); X86_PUSH32(v); }
	// 001EE727 mov eax, [ebp-0x228]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDD8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE72D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE72E call 0x000127D0
	X86_PUSH32(0x1EE733u); sub_000127D0(ctx, base);
	// 001EE733 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE736 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE71B, __imp__sub_001EE71B)

X86_FUNC(__imp__sub_001EE737) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE737 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE73C push 0x4F
	{ uint32_t v = (uint32_t)(0x4Fu); X86_PUSH32(v); }
	// 001EE73E push 0x2666EC
	{ uint32_t v = (uint32_t)(0x2666ECu); X86_PUSH32(v); }
	// 001EE743 mov eax, [ebp-0x228]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDD8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE749 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE74A call 0x000127D0
	X86_PUSH32(0x1EE74Fu); sub_000127D0(ctx, base);
	// 001EE74F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE752 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE737, __imp__sub_001EE737)

X86_FUNC(__imp__sub_001EE753) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE753 mov eax, 0x27E84C
	{ ctx.eax = (uint32_t)(0x27E84Cu); }
	// 001EE758 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE753, __imp__sub_001EE753)

X86_FUNC(__imp__sub_001EE760) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE760 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE765 push 0x173
	{ uint32_t v = (uint32_t)(0x173u); X86_PUSH32(v); }
	// 001EE76A push 0x266A10
	{ uint32_t v = (uint32_t)(0x266A10u); X86_PUSH32(v); }
	// 001EE76F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE772 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE773 call 0x000127D0
	X86_PUSH32(0x1EE778u); sub_000127D0(ctx, base);
	// 001EE778 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE77B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE760, __imp__sub_001EE760)

X86_FUNC(__imp__sub_001EE77C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE77C mov eax, 0x27E894
	{ ctx.eax = (uint32_t)(0x27E894u); }
	// 001EE781 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE77C, __imp__sub_001EE77C)

X86_FUNC(__imp__sub_001EE790) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE790 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE793 add ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE796 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE790, __imp__sub_001EE790)

X86_FUNC(__imp__sub_001EE79B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE79B mov eax, 0x27E8B8
	{ ctx.eax = (uint32_t)(0x27E8B8u); }
	// 001EE7A0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE79B, __imp__sub_001EE79B)

X86_FUNC(__imp__sub_001EE7B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE7B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE7B5 push 0xB8
	{ uint32_t v = (uint32_t)(0xB8u); X86_PUSH32(v); }
	// 001EE7BA push 0x266B94
	{ uint32_t v = (uint32_t)(0x266B94u); X86_PUSH32(v); }
	// 001EE7BF mov eax, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE7C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE7C3 call 0x000127D0
	X86_PUSH32(0x1EE7C8u); sub_000127D0(ctx, base);
	// 001EE7C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE7CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE7B0, __imp__sub_001EE7B0)

X86_FUNC(__imp__sub_001EE7CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE7CC mov eax, 0x27E8DC
	{ ctx.eax = (uint32_t)(0x27E8DCu); }
	// 001EE7D1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE7CC, __imp__sub_001EE7CC)

X86_FUNC(__imp__sub_001EE7E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE7E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE7E5 push 0x67
	{ uint32_t v = (uint32_t)(0x67u); X86_PUSH32(v); }
	// 001EE7E7 push 0x267060
	{ uint32_t v = (uint32_t)(0x267060u); X86_PUSH32(v); }
	// 001EE7EC mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE7EF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE7F0 call 0x000127D0
	X86_PUSH32(0x1EE7F5u); sub_000127D0(ctx, base);
	// 001EE7F5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE7F8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE7E0, __imp__sub_001EE7E0)

X86_FUNC(__imp__sub_001EE7F9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE7F9 mov eax, 0x27E900
	{ ctx.eax = (uint32_t)(0x27E900u); }
	// 001EE7FE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE7F9, __imp__sub_001EE7F9)

X86_FUNC(__imp__sub_001EE810) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE810 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE815 push 0x2B7
	{ uint32_t v = (uint32_t)(0x2B7u); X86_PUSH32(v); }
	// 001EE81A push 0x267218
	{ uint32_t v = (uint32_t)(0x267218u); X86_PUSH32(v); }
	// 001EE81F mov eax, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE822 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE823 call 0x000127D0
	X86_PUSH32(0x1EE828u); sub_000127D0(ctx, base);
	// 001EE828 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE82B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE810, __imp__sub_001EE810)

X86_FUNC(__imp__sub_001EE82C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE82C mov eax, 0x27E924
	{ ctx.eax = (uint32_t)(0x27E924u); }
	// 001EE831 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE82C, __imp__sub_001EE82C)

X86_FUNC(__imp__sub_001EE840) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE840 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE845 push 0x2D9
	{ uint32_t v = (uint32_t)(0x2D9u); X86_PUSH32(v); }
	// 001EE84A push 0x267218
	{ uint32_t v = (uint32_t)(0x267218u); X86_PUSH32(v); }
	// 001EE84F mov eax, [ebp-0x28]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE852 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE853 call 0x000127D0
	X86_PUSH32(0x1EE858u); sub_000127D0(ctx, base);
	// 001EE858 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE85B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE840, __imp__sub_001EE840)

X86_FUNC(__imp__sub_001EE85C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE85C mov eax, 0x27E948
	{ ctx.eax = (uint32_t)(0x27E948u); }
	// 001EE861 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE85C, __imp__sub_001EE85C)

X86_FUNC(__imp__sub_001EE870) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE870 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE875 push 0x2FA
	{ uint32_t v = (uint32_t)(0x2FAu); X86_PUSH32(v); }
	// 001EE87A push 0x267218
	{ uint32_t v = (uint32_t)(0x267218u); X86_PUSH32(v); }
	// 001EE87F mov eax, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE882 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE883 call 0x000127D0
	X86_PUSH32(0x1EE888u); sub_000127D0(ctx, base);
	// 001EE888 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE88B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE870, __imp__sub_001EE870)

X86_FUNC(__imp__sub_001EE88C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE88C mov eax, 0x27E96C
	{ ctx.eax = (uint32_t)(0x27E96Cu); }
	// 001EE891 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE88C, __imp__sub_001EE88C)

X86_FUNC(__imp__sub_001EE8A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE8A0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE8A5 push 0x313
	{ uint32_t v = (uint32_t)(0x313u); X86_PUSH32(v); }
	// 001EE8AA push 0x267218
	{ uint32_t v = (uint32_t)(0x267218u); X86_PUSH32(v); }
	// 001EE8AF mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE8B2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE8B3 call 0x000127D0
	X86_PUSH32(0x1EE8B8u); sub_000127D0(ctx, base);
	// 001EE8B8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE8BB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE8A0, __imp__sub_001EE8A0)

X86_FUNC(__imp__sub_001EE8BC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE8BC mov eax, 0x27E990
	{ ctx.eax = (uint32_t)(0x27E990u); }
	// 001EE8C1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE8BC, __imp__sub_001EE8BC)

X86_FUNC(__imp__sub_001EE8D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_001EE8D0;
loc_000BEFB0:
	// 000BEFB0 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 000BEFB1 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 000BEFB3 mov eax, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 000BEFB5 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000BEFB7 jz 0x000BEFC8
	if (((uint32_t)(ca & cb)) == 0) { goto loc_000BEFC8; }
	// 000BEFB9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 000BEFBA call 0x0004C110
	X86_PUSH32(0xBEFBFu); sub_0004C110(ctx, base);
	// 000BEFBF add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000BEFC2 mov dword ptr [esi], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(0x0u); }
loc_000BEFC8:
	// 000BEFC8 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 000BEFC9 ret
	ctx.esp += 4; return;
loc_001EE8D0:
	// 001EE8D0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EE8D3 add ecx, 0x1B8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1B8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE8D9 jmp 0x000BEFB0
	X86_BARRIER();
	goto loc_000BEFB0;
}
X86_WEAK_FUNC(sub_001EE8D0, __imp__sub_001EE8D0)

X86_FUNC(__imp__sub_001EE8DE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE8DE mov eax, 0x27E9B4
	{ ctx.eax = (uint32_t)(0x27E9B4u); }
	// 001EE8E3 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE8DE, __imp__sub_001EE8DE)

X86_FUNC(__imp__sub_001EE8F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE8F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE8F5 push 0x22A
	{ uint32_t v = (uint32_t)(0x22Au); X86_PUSH32(v); }
	// 001EE8FA push 0x267320
	{ uint32_t v = (uint32_t)(0x267320u); X86_PUSH32(v); }
	// 001EE8FF mov eax, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE902 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE903 call 0x000127D0
	X86_PUSH32(0x1EE908u); sub_000127D0(ctx, base);
	// 001EE908 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE90B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE8F0, __imp__sub_001EE8F0)

X86_FUNC(__imp__sub_001EE90C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE90C mov eax, 0x27E9D8
	{ ctx.eax = (uint32_t)(0x27E9D8u); }
	// 001EE911 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE90C, __imp__sub_001EE90C)

X86_FUNC(__imp__sub_001EE920) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE920 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE925 push 0x245
	{ uint32_t v = (uint32_t)(0x245u); X86_PUSH32(v); }
	// 001EE92A push 0x267320
	{ uint32_t v = (uint32_t)(0x267320u); X86_PUSH32(v); }
	// 001EE92F mov eax, [ebp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE932 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE933 call 0x000127D0
	X86_PUSH32(0x1EE938u); sub_000127D0(ctx, base);
	// 001EE938 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE93B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE920, __imp__sub_001EE920)

X86_FUNC(__imp__sub_001EE93C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE93C mov eax, 0x27E9FC
	{ ctx.eax = (uint32_t)(0x27E9FCu); }
	// 001EE941 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE93C, __imp__sub_001EE93C)

X86_FUNC(__imp__sub_001EE950) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE950 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE955 push 0x22D
	{ uint32_t v = (uint32_t)(0x22Du); X86_PUSH32(v); }
	// 001EE95A push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EE95F mov eax, [ebp-0x90]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF70u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE965 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE966 call 0x000127D0
	X86_PUSH32(0x1EE96Bu); sub_000127D0(ctx, base);
	// 001EE96B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE96E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE950, __imp__sub_001EE950)

X86_FUNC(__imp__sub_001EE96F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE96F mov eax, 0x27EA20
	{ ctx.eax = (uint32_t)(0x27EA20u); }
	// 001EE974 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE96F, __imp__sub_001EE96F)

X86_FUNC(__imp__sub_001EE980) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE980 lea ecx, [ebp-0xC0]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFF40u));
	// 001EE986 jmp 0x000C3A30
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000C3A30(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE980, __imp__sub_001EE980)

X86_FUNC(__imp__sub_001EE98B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE98B lea ecx, [ebp-0xCC]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFF34u));
	// 001EE991 jmp 0x000C3AD0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000C3AD0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EE98B, __imp__sub_001EE98B)

X86_FUNC(__imp__sub_001EE996) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE996 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE99B push 0x277
	{ uint32_t v = (uint32_t)(0x277u); X86_PUSH32(v); }
	// 001EE9A0 push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EE9A5 mov eax, [ebp-0xD0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF30u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE9AB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE9AC call 0x000127D0
	X86_PUSH32(0x1EE9B1u); sub_000127D0(ctx, base);
	// 001EE9B1 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE9B4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE996, __imp__sub_001EE996)

X86_FUNC(__imp__sub_001EE9B5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE9B5 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE9BA push 0x27E
	{ uint32_t v = (uint32_t)(0x27Eu); X86_PUSH32(v); }
	// 001EE9BF push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EE9C4 mov eax, [ebp-0xD0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF30u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE9CA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE9CB call 0x000127D0
	X86_PUSH32(0x1EE9D0u); sub_000127D0(ctx, base);
	// 001EE9D0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE9D3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE9B5, __imp__sub_001EE9B5)

X86_FUNC(__imp__sub_001EE9D4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE9D4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE9D9 push 0x2CD
	{ uint32_t v = (uint32_t)(0x2CDu); X86_PUSH32(v); }
	// 001EE9DE push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EE9E3 mov eax, [ebp-0xD0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF30u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EE9E9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EE9EA call 0x000127D0
	X86_PUSH32(0x1EE9EFu); sub_000127D0(ctx, base);
	// 001EE9EF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EE9F2 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE9D4, __imp__sub_001EE9D4)

X86_FUNC(__imp__sub_001EE9F3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EE9F3 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EE9F8 push 0x2FC
	{ uint32_t v = (uint32_t)(0x2FCu); X86_PUSH32(v); }
	// 001EE9FD push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEA02 mov eax, [ebp-0xC4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEA08 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEA09 call 0x000127D0
	X86_PUSH32(0x1EEA0Eu); sub_000127D0(ctx, base);
	// 001EEA0E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEA11 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EE9F3, __imp__sub_001EE9F3)

X86_FUNC(__imp__sub_001EEA12) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEA12 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEA17 push 0x306
	{ uint32_t v = (uint32_t)(0x306u); X86_PUSH32(v); }
	// 001EEA1C push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEA21 mov eax, [ebp-0xC4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEA27 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEA28 call 0x000127D0
	X86_PUSH32(0x1EEA2Du); sub_000127D0(ctx, base);
	// 001EEA2D add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEA30 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEA12, __imp__sub_001EEA12)

X86_FUNC(__imp__sub_001EEA31) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEA31 mov eax, 0x27EA74
	{ ctx.eax = (uint32_t)(0x27EA74u); }
	// 001EEA36 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEA31, __imp__sub_001EEA31)

X86_FUNC(__imp__sub_001EEA40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEA40 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEA45 push 0x383
	{ uint32_t v = (uint32_t)(0x383u); X86_PUSH32(v); }
	// 001EEA4A push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEA4F mov eax, [ebp-0x54]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEA52 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEA53 call 0x000127D0
	X86_PUSH32(0x1EEA58u); sub_000127D0(ctx, base);
	// 001EEA58 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEA5B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEA40, __imp__sub_001EEA40)

X86_FUNC(__imp__sub_001EEA5C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEA5C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEA61 push 0x3B1
	{ uint32_t v = (uint32_t)(0x3B1u); X86_PUSH32(v); }
	// 001EEA66 push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEA6B mov eax, [ebp-0x54]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEA6E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEA6F call 0x000127D0
	X86_PUSH32(0x1EEA74u); sub_000127D0(ctx, base);
	// 001EEA74 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEA77 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEA5C, __imp__sub_001EEA5C)

X86_FUNC(__imp__sub_001EEA78) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEA78 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEA7D push 0x3B8
	{ uint32_t v = (uint32_t)(0x3B8u); X86_PUSH32(v); }
	// 001EEA82 push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEA87 mov eax, [ebp-0x54]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEA8A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEA8B call 0x000127D0
	X86_PUSH32(0x1EEA90u); sub_000127D0(ctx, base);
	// 001EEA90 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEA93 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEA78, __imp__sub_001EEA78)

X86_FUNC(__imp__sub_001EEA94) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEA94 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEA99 push 0x3D5
	{ uint32_t v = (uint32_t)(0x3D5u); X86_PUSH32(v); }
	// 001EEA9E push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEAA3 mov eax, [ebp-0x54]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEAA6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEAA7 call 0x000127D0
	X86_PUSH32(0x1EEAACu); sub_000127D0(ctx, base);
	// 001EEAAC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEAAF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEA94, __imp__sub_001EEA94)

X86_FUNC(__imp__sub_001EEAB0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEAB0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEAB5 push 0x3D7
	{ uint32_t v = (uint32_t)(0x3D7u); X86_PUSH32(v); }
	// 001EEABA push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEABF mov eax, [ebp-0x54]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEAC2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEAC3 call 0x000127D0
	X86_PUSH32(0x1EEAC8u); sub_000127D0(ctx, base);
	// 001EEAC8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEACB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEAB0, __imp__sub_001EEAB0)

X86_FUNC(__imp__sub_001EEACC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEACC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEAD1 push 0x42B
	{ uint32_t v = (uint32_t)(0x42Bu); X86_PUSH32(v); }
	// 001EEAD6 push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEADB mov eax, [ebp-0x54]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEADE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEADF call 0x000127D0
	X86_PUSH32(0x1EEAE4u); sub_000127D0(ctx, base);
	// 001EEAE4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEAE7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEACC, __imp__sub_001EEACC)

X86_FUNC(__imp__sub_001EEAE8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEAE8 mov eax, 0x27EAC0
	{ ctx.eax = (uint32_t)(0x27EAC0u); }
	// 001EEAED jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEAE8, __imp__sub_001EEAE8)

X86_FUNC(__imp__sub_001EEB00) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB00 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEB05 push 0x497
	{ uint32_t v = (uint32_t)(0x497u); X86_PUSH32(v); }
	// 001EEB0A push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEB0F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEB12 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEB13 call 0x000127D0
	X86_PUSH32(0x1EEB18u); sub_000127D0(ctx, base);
	// 001EEB18 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEB1B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEB00, __imp__sub_001EEB00)

X86_FUNC(__imp__sub_001EEB1C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB1C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEB21 push 0x4B7
	{ uint32_t v = (uint32_t)(0x4B7u); X86_PUSH32(v); }
	// 001EEB26 push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEB2B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEB2E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEB2F call 0x000127D0
	X86_PUSH32(0x1EEB34u); sub_000127D0(ctx, base);
	// 001EEB34 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEB37 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEB1C, __imp__sub_001EEB1C)

X86_FUNC(__imp__sub_001EEB38) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB38 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEB3D push 0x4F3
	{ uint32_t v = (uint32_t)(0x4F3u); X86_PUSH32(v); }
	// 001EEB42 push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEB47 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEB4A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEB4B call 0x000127D0
	X86_PUSH32(0x1EEB50u); sub_000127D0(ctx, base);
	// 001EEB50 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEB53 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEB38, __imp__sub_001EEB38)

X86_FUNC(__imp__sub_001EEB54) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB54 mov eax, 0x27EAF4
	{ ctx.eax = (uint32_t)(0x27EAF4u); }
	// 001EEB59 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEB54, __imp__sub_001EEB54)

X86_FUNC(__imp__sub_001EEB60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB60 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEB65 push 0x1BA
	{ uint32_t v = (uint32_t)(0x1BAu); X86_PUSH32(v); }
	// 001EEB6A push 0x26754C
	{ uint32_t v = (uint32_t)(0x26754Cu); X86_PUSH32(v); }
	// 001EEB6F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEB72 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEB73 call 0x000127D0
	X86_PUSH32(0x1EEB78u); sub_000127D0(ctx, base);
	// 001EEB78 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEB7B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEB60, __imp__sub_001EEB60)

X86_FUNC(__imp__sub_001EEB7C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB7C mov eax, 0x27EB18
	{ ctx.eax = (uint32_t)(0x27EB18u); }
	// 001EEB81 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEB7C, __imp__sub_001EEB7C)

X86_FUNC(__imp__sub_001EEB90) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB90 lea ecx, [ebp-0x10]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF0u));
	// 001EEB93 jmp 0x000C3AD0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000C3AD0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEB90, __imp__sub_001EEB90)

X86_FUNC(__imp__sub_001EEB98) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEB98 mov eax, 0x27EB3C
	{ ctx.eax = (uint32_t)(0x27EB3Cu); }
	// 001EEB9D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEB98, __imp__sub_001EEB98)

X86_FUNC(__imp__sub_001EEBB0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEBB0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEBB5 push 0x44
	{ uint32_t v = (uint32_t)(0x44u); X86_PUSH32(v); }
	// 001EEBB7 push 0x267A4C
	{ uint32_t v = (uint32_t)(0x267A4Cu); X86_PUSH32(v); }
	// 001EEBBC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEBBF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEBC0 call 0x000127D0
	X86_PUSH32(0x1EEBC5u); sub_000127D0(ctx, base);
	// 001EEBC5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEBC8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEBB0, __imp__sub_001EEBB0)

X86_FUNC(__imp__sub_001EEBC9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEBC9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEBCE push 0x49
	{ uint32_t v = (uint32_t)(0x49u); X86_PUSH32(v); }
	// 001EEBD0 push 0x267A4C
	{ uint32_t v = (uint32_t)(0x267A4Cu); X86_PUSH32(v); }
	// 001EEBD5 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEBD8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEBD9 call 0x000127D0
	X86_PUSH32(0x1EEBDEu); sub_000127D0(ctx, base);
	// 001EEBDE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEBE1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEBC9, __imp__sub_001EEBC9)

X86_FUNC(__imp__sub_001EEBE2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEBE2 mov eax, 0x27EB68
	{ ctx.eax = (uint32_t)(0x27EB68u); }
	// 001EEBE7 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEBE2, __imp__sub_001EEBE2)

X86_FUNC(__imp__sub_001EEBF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEBF0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EEBF3 jmp 0x000B02D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000B02D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEBF0, __imp__sub_001EEBF0)

X86_FUNC(__imp__sub_001EEBF8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEBF8 mov eax, 0x27EBB0
	{ ctx.eax = (uint32_t)(0x27EBB0u); }
	// 001EEBFD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEBF8, __imp__sub_001EEBF8)

X86_FUNC(__imp__sub_001EEC10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEC10 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEC15 push 0x2AA
	{ uint32_t v = (uint32_t)(0x2AAu); X86_PUSH32(v); }
	// 001EEC1A push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EEC1F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEC22 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEC23 call 0x000127D0
	X86_PUSH32(0x1EEC28u); sub_000127D0(ctx, base);
	// 001EEC28 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEC2B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEC10, __imp__sub_001EEC10)

X86_FUNC(__imp__sub_001EEC2C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEC2C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEC31 push 0x2AB
	{ uint32_t v = (uint32_t)(0x2ABu); X86_PUSH32(v); }
	// 001EEC36 push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EEC3B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEC3E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEC3F call 0x000127D0
	X86_PUSH32(0x1EEC44u); sub_000127D0(ctx, base);
	// 001EEC44 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEC47 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEC2C, __imp__sub_001EEC2C)

X86_FUNC(__imp__sub_001EEC48) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEC48 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEC4D push 0x2AC
	{ uint32_t v = (uint32_t)(0x2ACu); X86_PUSH32(v); }
	// 001EEC52 push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EEC57 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEC5A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEC5B call 0x000127D0
	X86_PUSH32(0x1EEC60u); sub_000127D0(ctx, base);
	// 001EEC60 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEC63 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEC48, __imp__sub_001EEC48)

X86_FUNC(__imp__sub_001EEC64) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEC64 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEC69 push 0x2AD
	{ uint32_t v = (uint32_t)(0x2ADu); X86_PUSH32(v); }
	// 001EEC6E push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EEC73 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEC76 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEC77 call 0x000127D0
	X86_PUSH32(0x1EEC7Cu); sub_000127D0(ctx, base);
	// 001EEC7C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEC7F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEC64, __imp__sub_001EEC64)

X86_FUNC(__imp__sub_001EEC80) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEC80 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEC85 push 0x2AE
	{ uint32_t v = (uint32_t)(0x2AEu); X86_PUSH32(v); }
	// 001EEC8A push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EEC8F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEC92 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEC93 call 0x000127D0
	X86_PUSH32(0x1EEC98u); sub_000127D0(ctx, base);
	// 001EEC98 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEC9B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEC80, __imp__sub_001EEC80)

X86_FUNC(__imp__sub_001EEC9C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEC9C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EECA1 push 0x2BF
	{ uint32_t v = (uint32_t)(0x2BFu); X86_PUSH32(v); }
	// 001EECA6 push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EECAB mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EECAE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EECAF call 0x000127D0
	X86_PUSH32(0x1EECB4u); sub_000127D0(ctx, base);
	// 001EECB4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EECB7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEC9C, __imp__sub_001EEC9C)

X86_FUNC(__imp__sub_001EECB8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EECB8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EECBD push 0x2D5
	{ uint32_t v = (uint32_t)(0x2D5u); X86_PUSH32(v); }
	// 001EECC2 push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EECC7 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EECCA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EECCB call 0x000127D0
	X86_PUSH32(0x1EECD0u); sub_000127D0(ctx, base);
	// 001EECD0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EECD3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EECB8, __imp__sub_001EECB8)

X86_FUNC(__imp__sub_001EECD4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EECD4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EECD9 push 0x2D6
	{ uint32_t v = (uint32_t)(0x2D6u); X86_PUSH32(v); }
	// 001EECDE push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EECE3 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EECE6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EECE7 call 0x000127D0
	X86_PUSH32(0x1EECECu); sub_000127D0(ctx, base);
	// 001EECEC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EECEF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EECD4, __imp__sub_001EECD4)

X86_FUNC(__imp__sub_001EECF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EECF0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EECF5 push 0x2D7
	{ uint32_t v = (uint32_t)(0x2D7u); X86_PUSH32(v); }
	// 001EECFA push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EECFF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EED02 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EED03 call 0x000127D0
	X86_PUSH32(0x1EED08u); sub_000127D0(ctx, base);
	// 001EED08 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EED0B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EECF0, __imp__sub_001EECF0)

X86_FUNC(__imp__sub_001EED0C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EED0C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EED11 push 0x2D8
	{ uint32_t v = (uint32_t)(0x2D8u); X86_PUSH32(v); }
	// 001EED16 push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EED1B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EED1E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EED1F call 0x000127D0
	X86_PUSH32(0x1EED24u); sub_000127D0(ctx, base);
	// 001EED24 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EED27 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EED0C, __imp__sub_001EED0C)

X86_FUNC(__imp__sub_001EED28) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EED28 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EED2D push 0x2D9
	{ uint32_t v = (uint32_t)(0x2D9u); X86_PUSH32(v); }
	// 001EED32 push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EED37 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EED3A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EED3B call 0x000127D0
	X86_PUSH32(0x1EED40u); sub_000127D0(ctx, base);
	// 001EED40 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EED43 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EED28, __imp__sub_001EED28)

X86_FUNC(__imp__sub_001EED44) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EED44 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EED49 push 0x2EC
	{ uint32_t v = (uint32_t)(0x2ECu); X86_PUSH32(v); }
	// 001EED4E push 0x267C94
	{ uint32_t v = (uint32_t)(0x267C94u); X86_PUSH32(v); }
	// 001EED53 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EED56 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EED57 call 0x000127D0
	X86_PUSH32(0x1EED5Cu); sub_000127D0(ctx, base);
	// 001EED5C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EED5F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EED44, __imp__sub_001EED44)

X86_FUNC(__imp__sub_001EED60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EED60 mov eax, 0x27EC2C
	{ ctx.eax = (uint32_t)(0x27EC2Cu); }
	// 001EED65 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EED60, __imp__sub_001EED60)

X86_FUNC(__imp__sub_001EED70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EED70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EED75 push 0x244
	{ uint32_t v = (uint32_t)(0x244u); X86_PUSH32(v); }
	// 001EED7A push 0x268094
	{ uint32_t v = (uint32_t)(0x268094u); X86_PUSH32(v); }
	// 001EED7F mov eax, [ebp-0x190]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFE70u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EED85 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EED86 call 0x000127D0
	X86_PUSH32(0x1EED8Bu); sub_000127D0(ctx, base);
	// 001EED8B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EED8E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EED70, __imp__sub_001EED70)

X86_FUNC(__imp__sub_001EED8F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EED8F mov eax, 0x27EC50
	{ ctx.eax = (uint32_t)(0x27EC50u); }
	// 001EED94 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EED8F, __imp__sub_001EED8F)

X86_FUNC(__imp__sub_001EEDA0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEDA0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEDA5 push 0x77
	{ uint32_t v = (uint32_t)(0x77u); X86_PUSH32(v); }
	// 001EEDA7 push 0x268094
	{ uint32_t v = (uint32_t)(0x268094u); X86_PUSH32(v); }
	// 001EEDAC mov eax, [ebp-0x120]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEDB2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEDB3 call 0x000127D0
	X86_PUSH32(0x1EEDB8u); sub_000127D0(ctx, base);
	// 001EEDB8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEDBB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEDA0, __imp__sub_001EEDA0)

X86_FUNC(__imp__sub_001EEDBC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEDBC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEDC1 push 0x79
	{ uint32_t v = (uint32_t)(0x79u); X86_PUSH32(v); }
	// 001EEDC3 push 0x268094
	{ uint32_t v = (uint32_t)(0x268094u); X86_PUSH32(v); }
	// 001EEDC8 mov eax, [ebp-0x120]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEDCE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEDCF call 0x000127D0
	X86_PUSH32(0x1EEDD4u); sub_000127D0(ctx, base);
	// 001EEDD4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEDD7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEDBC, __imp__sub_001EEDBC)

X86_FUNC(__imp__sub_001EEDD8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEDD8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEDDD push 0x7C
	{ uint32_t v = (uint32_t)(0x7Cu); X86_PUSH32(v); }
	// 001EEDDF push 0x268094
	{ uint32_t v = (uint32_t)(0x268094u); X86_PUSH32(v); }
	// 001EEDE4 mov eax, [ebp-0x120]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEDEA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEDEB call 0x000127D0
	X86_PUSH32(0x1EEDF0u); sub_000127D0(ctx, base);
	// 001EEDF0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEDF3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEDD8, __imp__sub_001EEDD8)

X86_FUNC(__imp__sub_001EEDF4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEDF4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEDF9 push 0x7F
	{ uint32_t v = (uint32_t)(0x7Fu); X86_PUSH32(v); }
	// 001EEDFB push 0x268094
	{ uint32_t v = (uint32_t)(0x268094u); X86_PUSH32(v); }
	// 001EEE00 mov eax, [ebp-0x120]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEE06 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEE07 call 0x000127D0
	X86_PUSH32(0x1EEE0Cu); sub_000127D0(ctx, base);
	// 001EEE0C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEE0F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEDF4, __imp__sub_001EEDF4)

X86_FUNC(__imp__sub_001EEE10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE10 mov eax, 0x27EC8C
	{ ctx.eax = (uint32_t)(0x27EC8Cu); }
	// 001EEE15 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEE10, __imp__sub_001EEE10)

X86_FUNC(__imp__sub_001EEE20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE20 mov ecx, [ebp-0x38]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFC8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EEE23 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEE26 jmp 0x000D1E50
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000D1E50(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEE20, __imp__sub_001EEE20)

X86_FUNC(__imp__sub_001EEE2B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE2B mov eax, 0x27ECB0
	{ ctx.eax = (uint32_t)(0x27ECB0u); }
	// 001EEE30 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEE2B, __imp__sub_001EEE2B)

X86_FUNC(__imp__sub_001EEE40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE40 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EEE43 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEE46 jmp 0x000D1E50
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000D1E50(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEE40, __imp__sub_001EEE40)

X86_FUNC(__imp__sub_001EEE4B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE4B mov eax, 0x27ECD4
	{ ctx.eax = (uint32_t)(0x27ECD4u); }
	// 001EEE50 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEE4B, __imp__sub_001EEE4B)

X86_FUNC(__imp__sub_001EEE60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE60 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEE65 push 0x49
	{ uint32_t v = (uint32_t)(0x49u); X86_PUSH32(v); }
	// 001EEE67 push 0x268694
	{ uint32_t v = (uint32_t)(0x268694u); X86_PUSH32(v); }
	// 001EEE6C mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEE6F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEE70 call 0x000127D0
	X86_PUSH32(0x1EEE75u); sub_000127D0(ctx, base);
	// 001EEE75 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEE78 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEE60, __imp__sub_001EEE60)

X86_FUNC(__imp__sub_001EEE79) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE79 mov eax, 0x27ECF8
	{ ctx.eax = (uint32_t)(0x27ECF8u); }
	// 001EEE7E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEE79, __imp__sub_001EEE79)

X86_FUNC(__imp__sub_001EEE90) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEE90 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEE95 push 0x59
	{ uint32_t v = (uint32_t)(0x59u); X86_PUSH32(v); }
	// 001EEE97 push 0x268884
	{ uint32_t v = (uint32_t)(0x268884u); X86_PUSH32(v); }
	// 001EEE9C mov eax, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEE9F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEEA0 call 0x000127D0
	X86_PUSH32(0x1EEEA5u); sub_000127D0(ctx, base);
	// 001EEEA5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEEA8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEE90, __imp__sub_001EEE90)

X86_FUNC(__imp__sub_001EEEA9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEEA9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEEAE push 0x5E
	{ uint32_t v = (uint32_t)(0x5Eu); X86_PUSH32(v); }
	// 001EEEB0 push 0x268884
	{ uint32_t v = (uint32_t)(0x268884u); X86_PUSH32(v); }
	// 001EEEB5 mov eax, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEEB8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEEB9 call 0x000127D0
	X86_PUSH32(0x1EEEBEu); sub_000127D0(ctx, base);
	// 001EEEBE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEEC1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEEA9, __imp__sub_001EEEA9)

X86_FUNC(__imp__sub_001EEEC2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEEC2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEEC7 push 0x66
	{ uint32_t v = (uint32_t)(0x66u); X86_PUSH32(v); }
	// 001EEEC9 push 0x268884
	{ uint32_t v = (uint32_t)(0x268884u); X86_PUSH32(v); }
	// 001EEECE mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEED1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEED2 call 0x000127D0
	X86_PUSH32(0x1EEED7u); sub_000127D0(ctx, base);
	// 001EEED7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEEDA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEEC2, __imp__sub_001EEEC2)

X86_FUNC(__imp__sub_001EEEDB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEEDB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEEE0 push 0x95
	{ uint32_t v = (uint32_t)(0x95u); X86_PUSH32(v); }
	// 001EEEE5 push 0x268884
	{ uint32_t v = (uint32_t)(0x268884u); X86_PUSH32(v); }
	// 001EEEEA mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEEED push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEEEE call 0x000127D0
	X86_PUSH32(0x1EEEF3u); sub_000127D0(ctx, base);
	// 001EEEF3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEEF6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEEDB, __imp__sub_001EEEDB)

X86_FUNC(__imp__sub_001EEEF7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEEF7 mov eax, 0x27ED34
	{ ctx.eax = (uint32_t)(0x27ED34u); }
	// 001EEEFC jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEEF7, __imp__sub_001EEEF7)

X86_FUNC(__imp__sub_001EEF10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEF10 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEF15 push 0x38C
	{ uint32_t v = (uint32_t)(0x38Cu); X86_PUSH32(v); }
	// 001EEF1A push 0x2689BC
	{ uint32_t v = (uint32_t)(0x2689BCu); X86_PUSH32(v); }
	// 001EEF1F mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEF22 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEF23 call 0x000127D0
	X86_PUSH32(0x1EEF28u); sub_000127D0(ctx, base);
	// 001EEF28 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEF2B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEF10, __imp__sub_001EEF10)

X86_FUNC(__imp__sub_001EEF2C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEF2C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEF31 push 0x377
	{ uint32_t v = (uint32_t)(0x377u); X86_PUSH32(v); }
	// 001EEF36 push 0x2689BC
	{ uint32_t v = (uint32_t)(0x2689BCu); X86_PUSH32(v); }
	// 001EEF3B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEF3E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEF3F call 0x000127D0
	X86_PUSH32(0x1EEF44u); sub_000127D0(ctx, base);
	// 001EEF44 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEF47 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEF2C, __imp__sub_001EEF2C)

X86_FUNC(__imp__sub_001EEF48) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEF48 mov eax, 0x27ED60
	{ ctx.eax = (uint32_t)(0x27ED60u); }
	// 001EEF4D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEF48, __imp__sub_001EEF48)

X86_FUNC(__imp__sub_001EEF60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEF60 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEF65 push 0x3C4
	{ uint32_t v = (uint32_t)(0x3C4u); X86_PUSH32(v); }
	// 001EEF6A push 0x2689BC
	{ uint32_t v = (uint32_t)(0x2689BCu); X86_PUSH32(v); }
	// 001EEF6F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEF72 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEF73 call 0x000127D0
	X86_PUSH32(0x1EEF78u); sub_000127D0(ctx, base);
	// 001EEF78 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEF7B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEF60, __imp__sub_001EEF60)

X86_FUNC(__imp__sub_001EEF7C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEF7C mov eax, 0x27ED84
	{ ctx.eax = (uint32_t)(0x27ED84u); }
	// 001EEF81 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EEF7C, __imp__sub_001EEF7C)

X86_FUNC(__imp__sub_001EEF90) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEF90 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEF95 push 0x89
	{ uint32_t v = (uint32_t)(0x89u); X86_PUSH32(v); }
	// 001EEF9A push 0x268A60
	{ uint32_t v = (uint32_t)(0x268A60u); X86_PUSH32(v); }
	// 001EEF9F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEFA2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEFA3 call 0x000127D0
	X86_PUSH32(0x1EEFA8u); sub_000127D0(ctx, base);
	// 001EEFA8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEFAB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEF90, __imp__sub_001EEF90)

X86_FUNC(__imp__sub_001EEFAC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEFAC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEFB1 push 0x6A
	{ uint32_t v = (uint32_t)(0x6Au); X86_PUSH32(v); }
	// 001EEFB3 push 0x268A60
	{ uint32_t v = (uint32_t)(0x268A60u); X86_PUSH32(v); }
	// 001EEFB8 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEFBB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEFBC call 0x000127D0
	X86_PUSH32(0x1EEFC1u); sub_000127D0(ctx, base);
	// 001EEFC1 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEFC4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEFAC, __imp__sub_001EEFAC)

X86_FUNC(__imp__sub_001EEFC5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEFC5 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEFCA push 0x48
	{ uint32_t v = (uint32_t)(0x48u); X86_PUSH32(v); }
	// 001EEFCC push 0x268A60
	{ uint32_t v = (uint32_t)(0x268A60u); X86_PUSH32(v); }
	// 001EEFD1 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEFD4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEFD5 call 0x000127D0
	X86_PUSH32(0x1EEFDAu); sub_000127D0(ctx, base);
	// 001EEFDA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEFDD ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEFC5, __imp__sub_001EEFC5)

X86_FUNC(__imp__sub_001EEFDE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEFDE push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEFE3 push 0x55
	{ uint32_t v = (uint32_t)(0x55u); X86_PUSH32(v); }
	// 001EEFE5 push 0x268A60
	{ uint32_t v = (uint32_t)(0x268A60u); X86_PUSH32(v); }
	// 001EEFEA mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EEFED push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EEFEE call 0x000127D0
	X86_PUSH32(0x1EEFF3u); sub_000127D0(ctx, base);
	// 001EEFF3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EEFF6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEFDE, __imp__sub_001EEFDE)

X86_FUNC(__imp__sub_001EEFF7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EEFF7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EEFFC push 0xD0
	{ uint32_t v = (uint32_t)(0xD0u); X86_PUSH32(v); }
	// 001EF001 push 0x268A60
	{ uint32_t v = (uint32_t)(0x268A60u); X86_PUSH32(v); }
	// 001EF006 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF009 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF00A call 0x000127D0
	X86_PUSH32(0x1EF00Fu); sub_000127D0(ctx, base);
	// 001EF00F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF012 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EEFF7, __imp__sub_001EEFF7)

X86_FUNC(__imp__sub_001EF013) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF013 mov eax, 0x27EDC8
	{ ctx.eax = (uint32_t)(0x27EDC8u); }
	// 001EF018 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF013, __imp__sub_001EF013)

X86_FUNC(__imp__sub_001EF020) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF020 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF023 add ecx, 0x2B4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2B4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF029 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF020, __imp__sub_001EF020)

X86_FUNC(__imp__sub_001EF02E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF02E mov eax, 0x27EE10
	{ ctx.eax = (uint32_t)(0x27EE10u); }
	// 001EF033 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF02E, __imp__sub_001EF02E)

X86_FUNC(__imp__sub_001EF040) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF040 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF045 push 0x91
	{ uint32_t v = (uint32_t)(0x91u); X86_PUSH32(v); }
	// 001EF04A push 0x268C78
	{ uint32_t v = (uint32_t)(0x268C78u); X86_PUSH32(v); }
	// 001EF04F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF052 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF053 call 0x000127D0
	X86_PUSH32(0x1EF058u); sub_000127D0(ctx, base);
	// 001EF058 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF05B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF040, __imp__sub_001EF040)

X86_FUNC(__imp__sub_001EF05C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF05C mov eax, 0x27EE34
	{ ctx.eax = (uint32_t)(0x27EE34u); }
	// 001EF061 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF05C, __imp__sub_001EF05C)

X86_FUNC(__imp__sub_001EF070) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF070 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF075 push 0x17C
	{ uint32_t v = (uint32_t)(0x17Cu); X86_PUSH32(v); }
	// 001EF07A push 0x268C78
	{ uint32_t v = (uint32_t)(0x268C78u); X86_PUSH32(v); }
	// 001EF07F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF082 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF083 call 0x000127D0
	X86_PUSH32(0x1EF088u); sub_000127D0(ctx, base);
	// 001EF088 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF08B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF070, __imp__sub_001EF070)

X86_FUNC(__imp__sub_001EF08C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF08C mov eax, 0x27EE58
	{ ctx.eax = (uint32_t)(0x27EE58u); }
	// 001EF091 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF08C, __imp__sub_001EF08C)

X86_FUNC(__imp__sub_001EF0A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF0A0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF0A5 push 0x32
	{ uint32_t v = (uint32_t)(0x32u); X86_PUSH32(v); }
	// 001EF0A7 push 0x268CB0
	{ uint32_t v = (uint32_t)(0x268CB0u); X86_PUSH32(v); }
	// 001EF0AC mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF0AF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF0B0 call 0x000127D0
	X86_PUSH32(0x1EF0B5u); sub_000127D0(ctx, base);
	// 001EF0B5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF0B8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF0A0, __imp__sub_001EF0A0)

X86_FUNC(__imp__sub_001EF0B9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF0B9 mov eax, 0x27EE7C
	{ ctx.eax = (uint32_t)(0x27EE7Cu); }
	// 001EF0BE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF0B9, __imp__sub_001EF0B9)

X86_FUNC(__imp__sub_001EF0D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF0D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF0D5 push 0x42
	{ uint32_t v = (uint32_t)(0x42u); X86_PUSH32(v); }
	// 001EF0D7 push 0x268CEC
	{ uint32_t v = (uint32_t)(0x268CECu); X86_PUSH32(v); }
	// 001EF0DC mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF0DF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF0E0 call 0x000127D0
	X86_PUSH32(0x1EF0E5u); sub_000127D0(ctx, base);
	// 001EF0E5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF0E8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF0D0, __imp__sub_001EF0D0)

X86_FUNC(__imp__sub_001EF0E9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF0E9 mov eax, 0x27EEA0
	{ ctx.eax = (uint32_t)(0x27EEA0u); }
	// 001EF0EE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF0E9, __imp__sub_001EF0E9)

X86_FUNC(__imp__sub_001EF100) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF100 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF105 push 0x62
	{ uint32_t v = (uint32_t)(0x62u); X86_PUSH32(v); }
	// 001EF107 push 0x268DA0
	{ uint32_t v = (uint32_t)(0x268DA0u); X86_PUSH32(v); }
	// 001EF10C mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF10F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF110 call 0x000127D0
	X86_PUSH32(0x1EF115u); sub_000127D0(ctx, base);
	// 001EF115 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF118 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF100, __imp__sub_001EF100)

X86_FUNC(__imp__sub_001EF119) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF119 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF11E push 0x66
	{ uint32_t v = (uint32_t)(0x66u); X86_PUSH32(v); }
	// 001EF120 push 0x268DA0
	{ uint32_t v = (uint32_t)(0x268DA0u); X86_PUSH32(v); }
	// 001EF125 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF128 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF129 call 0x000127D0
	X86_PUSH32(0x1EF12Eu); sub_000127D0(ctx, base);
	// 001EF12E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF131 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF119, __imp__sub_001EF119)

X86_FUNC(__imp__sub_001EF132) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF132 mov eax, 0x27EECC
	{ ctx.eax = (uint32_t)(0x27EECCu); }
	// 001EF137 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF132, __imp__sub_001EF132)

X86_FUNC(__imp__sub_001EF140) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF140 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF145 push 0xC1
	{ uint32_t v = (uint32_t)(0xC1u); X86_PUSH32(v); }
	// 001EF14A push 0x269198
	{ uint32_t v = (uint32_t)(0x269198u); X86_PUSH32(v); }
	// 001EF14F mov eax, [ebp-0x3D8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF155 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF156 call 0x000127D0
	X86_PUSH32(0x1EF15Bu); sub_000127D0(ctx, base);
	// 001EF15B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF15E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF140, __imp__sub_001EF140)

X86_FUNC(__imp__sub_001EF15F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF15F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF164 push 0xC2
	{ uint32_t v = (uint32_t)(0xC2u); X86_PUSH32(v); }
	// 001EF169 push 0x269198
	{ uint32_t v = (uint32_t)(0x269198u); X86_PUSH32(v); }
	// 001EF16E mov eax, [ebp-0x3D8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFC28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF174 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF175 call 0x000127D0
	X86_PUSH32(0x1EF17Au); sub_000127D0(ctx, base);
	// 001EF17A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF17D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF15F, __imp__sub_001EF15F)

X86_FUNC(__imp__sub_001EF17E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF17E mov eax, 0x27EEF8
	{ ctx.eax = (uint32_t)(0x27EEF8u); }
	// 001EF183 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF17E, __imp__sub_001EF17E)

X86_FUNC(__imp__sub_001EF190) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF190 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF195 push 0x37
	{ uint32_t v = (uint32_t)(0x37u); X86_PUSH32(v); }
	// 001EF197 push 0x269A50
	{ uint32_t v = (uint32_t)(0x269A50u); X86_PUSH32(v); }
	// 001EF19C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF19F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF1A0 call 0x000127D0
	X86_PUSH32(0x1EF1A5u); sub_000127D0(ctx, base);
	// 001EF1A5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF1A8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF190, __imp__sub_001EF190)

X86_FUNC(__imp__sub_001EF1A9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF1A9 mov eax, 0x27EF1C
	{ ctx.eax = (uint32_t)(0x27EF1Cu); }
	// 001EF1AE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF1A9, __imp__sub_001EF1A9)

X86_FUNC(__imp__sub_001EF1C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF1C0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF1C3 add ecx, 0x64
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x64u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF1C6 jmp 0x000F8B20
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F8B20(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF1C0, __imp__sub_001EF1C0)

X86_FUNC(__imp__sub_001EF1CB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF1CB mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF1CE add ecx, 0x110
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x110u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF1D4 jmp 0x000F6FF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F6FF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF1CB, __imp__sub_001EF1CB)

X86_FUNC(__imp__sub_001EF1D9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF1D9 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF1DC add ecx, 0x1298
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1298u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF1E2 jmp 0x000F5750
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F5750(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF1D9, __imp__sub_001EF1D9)

X86_FUNC(__imp__sub_001EF1E7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF1E7 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF1EA add ecx, 0x1398
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1398u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF1F0 jmp 0x000F53F0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F53F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF1E7, __imp__sub_001EF1E7)

X86_FUNC(__imp__sub_001EF1F5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF1F5 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF1F8 add ecx, 0x13A0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x13A0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF1FE jmp 0x000F1BB0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F1BB0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF1F5, __imp__sub_001EF1F5)

X86_FUNC(__imp__sub_001EF203) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF203 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF206 add ecx, 0x13F0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x13F0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF20C jmp 0x000EFB20
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EFB20(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF203, __imp__sub_001EF203)

X86_FUNC(__imp__sub_001EF211) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF211 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF214 add ecx, 0x14C8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x14C8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF21A jmp 0x000EEDD0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EEDD0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF211, __imp__sub_001EF211)

X86_FUNC(__imp__sub_001EF21F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF21F mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF222 add ecx, 0x14E4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x14E4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF228 jmp 0x000EEA00
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EEA00(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF21F, __imp__sub_001EF21F)

X86_FUNC(__imp__sub_001EF22D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF22D mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF230 add ecx, 0x1510
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1510u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF236 jmp 0x000EE630
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EE630(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF22D, __imp__sub_001EF22D)

X86_FUNC(__imp__sub_001EF23B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF23B mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF23E add ecx, 0x6C50
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x6C50u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF244 jmp 0x000EE4E0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EE4E0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF23B, __imp__sub_001EF23B)

X86_FUNC(__imp__sub_001EF249) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF249 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF24C add ecx, 0x6CA4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x6CA4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF252 jmp 0x000ED930
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000ED930(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF249, __imp__sub_001EF249)

X86_FUNC(__imp__sub_001EF257) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF257 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF25A add ecx, 0x77D4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x77D4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF260 jmp 0x000EC460
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC460(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF257, __imp__sub_001EF257)

X86_FUNC(__imp__sub_001EF265) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF265 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF268 add ecx, 0x77E0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x77E0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF26E jmp 0x000EC390
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC390(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF265, __imp__sub_001EF265)

X86_FUNC(__imp__sub_001EF273) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF273 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF276 add ecx, 0x7800
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7800u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF27C jmp 0x000EC2E0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC2E0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF273, __imp__sub_001EF273)

X86_FUNC(__imp__sub_001EF281) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF281 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF284 add ecx, 0x78C0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x78C0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF28A jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF281, __imp__sub_001EF281)

X86_FUNC(__imp__sub_001EF28F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF28F mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF292 add ecx, 0x78F8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x78F8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF298 jmp 0x000EC0F0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC0F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF28F, __imp__sub_001EF28F)

X86_FUNC(__imp__sub_001EF29D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF29D mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2A0 add ecx, 0x7900
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7900u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2A6 jmp 0x000EBF80
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EBF80(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF29D, __imp__sub_001EF29D)

X86_FUNC(__imp__sub_001EF2AB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2AB mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2AE add ecx, 0x79A4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x79A4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2B4 jmp 0x000EBD30
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EBD30(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2AB, __imp__sub_001EF2AB)

X86_FUNC(__imp__sub_001EF2B9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2B9 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2BC add ecx, 0x79C8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x79C8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2C2 jmp 0x000EBC60
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EBC60(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2B9, __imp__sub_001EF2B9)

X86_FUNC(__imp__sub_001EF2C7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2C7 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2CA add ecx, 0x79D0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x79D0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2D0 jmp 0x000EBB70
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EBB70(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2C7, __imp__sub_001EF2C7)

X86_FUNC(__imp__sub_001EF2D5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2D5 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2D8 add ecx, 0x7A38
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7A38u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2DE jmp 0x000EB9F0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EB9F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2D5, __imp__sub_001EF2D5)

X86_FUNC(__imp__sub_001EF2E3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2E3 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2E6 add ecx, 0x7B1C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7B1Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2EC jmp 0x000EB990
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EB990(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2E3, __imp__sub_001EF2E3)

X86_FUNC(__imp__sub_001EF2F1) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2F1 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF2F4 add ecx, 0x7B20
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7B20u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF2FA jmp 0x000EB950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EB950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2F1, __imp__sub_001EF2F1)

X86_FUNC(__imp__sub_001EF2FF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF2FF mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF302 add ecx, 0x7BC8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7BC8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF308 jmp 0x000EB7C0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EB7C0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF2FF, __imp__sub_001EF2FF)

X86_FUNC(__imp__sub_001EF30D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF30D mov eax, 0x27F0D4
	{ ctx.eax = (uint32_t)(0x27F0D4u); }
	// 001EF312 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF30D, __imp__sub_001EF30D)

X86_FUNC(__imp__sub_001EF320) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF320 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF323 add ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF326 jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF320, __imp__sub_001EF320)

X86_FUNC(__imp__sub_001EF32B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF32B mov eax, 0x27F11C
	{ ctx.eax = (uint32_t)(0x27F11Cu); }
	// 001EF330 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF32B, __imp__sub_001EF32B)

X86_FUNC(__imp__sub_001EF340) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF340 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF345 push 0xC7
	{ uint32_t v = (uint32_t)(0xC7u); X86_PUSH32(v); }
	// 001EF34A push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF34F mov eax, [ebp-0x110]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF355 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF356 call 0x000127D0
	X86_PUSH32(0x1EF35Bu); sub_000127D0(ctx, base);
	// 001EF35B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF35E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF340, __imp__sub_001EF340)

X86_FUNC(__imp__sub_001EF35F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF35F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF364 push 0xDB
	{ uint32_t v = (uint32_t)(0xDBu); X86_PUSH32(v); }
	// 001EF369 push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF36E mov eax, [ebp-0x110]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF374 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF375 call 0x000127D0
	X86_PUSH32(0x1EF37Au); sub_000127D0(ctx, base);
	// 001EF37A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF37D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF35F, __imp__sub_001EF35F)

X86_FUNC(__imp__sub_001EF37E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF37E mov eax, 0x27F148
	{ ctx.eax = (uint32_t)(0x27F148u); }
	// 001EF383 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF37E, __imp__sub_001EF37E)

X86_FUNC(__imp__sub_001EF390) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF390 lea ecx, [ebp-0x42C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBD4u));
	// 001EF396 jmp 0x001025B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001025B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF390, __imp__sub_001EF390)

X86_FUNC(__imp__sub_001EF39B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF39B lea ecx, [ebp-0x438]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBC8u));
	// 001EF3A1 jmp 0x00102410
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00102410(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF39B, __imp__sub_001EF39B)

X86_FUNC(__imp__sub_001EF3A6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF3A6 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF3AB push 0x5D5
	{ uint32_t v = (uint32_t)(0x5D5u); X86_PUSH32(v); }
	// 001EF3B0 push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF3B5 mov eax, [ebp-0x43C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBC4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF3BB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF3BC call 0x000127D0
	X86_PUSH32(0x1EF3C1u); sub_000127D0(ctx, base);
	// 001EF3C1 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF3C4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF3A6, __imp__sub_001EF3A6)

X86_FUNC(__imp__sub_001EF3C5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF3C5 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF3CA push 0x5E7
	{ uint32_t v = (uint32_t)(0x5E7u); X86_PUSH32(v); }
	// 001EF3CF push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF3D4 mov eax, [ebp-0x43C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBC4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF3DA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF3DB call 0x000127D0
	X86_PUSH32(0x1EF3E0u); sub_000127D0(ctx, base);
	// 001EF3E0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF3E3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF3C5, __imp__sub_001EF3C5)

X86_FUNC(__imp__sub_001EF3E4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF3E4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF3E9 push 0x5F0
	{ uint32_t v = (uint32_t)(0x5F0u); X86_PUSH32(v); }
	// 001EF3EE push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF3F3 mov eax, [ebp-0x43C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBC4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF3F9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF3FA call 0x000127D0
	X86_PUSH32(0x1EF3FFu); sub_000127D0(ctx, base);
	// 001EF3FF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF402 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF3E4, __imp__sub_001EF3E4)

X86_FUNC(__imp__sub_001EF403) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF403 mov eax, 0x27F18C
	{ ctx.eax = (uint32_t)(0x27F18Cu); }
	// 001EF408 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF403, __imp__sub_001EF403)

X86_FUNC(__imp__sub_001EF410) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF410 lea ecx, [ebp-0x420]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBE0u));
	// 001EF416 jmp 0x001025B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001025B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF410, __imp__sub_001EF410)

X86_FUNC(__imp__sub_001EF41B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF41B lea ecx, [ebp-0x430]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBD0u));
	// 001EF421 jmp 0x00102410
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00102410(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF41B, __imp__sub_001EF41B)

X86_FUNC(__imp__sub_001EF426) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF426 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF42B push 0x8AB
	{ uint32_t v = (uint32_t)(0x8ABu); X86_PUSH32(v); }
	// 001EF430 push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF435 mov eax, [ebp-0x410]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF43B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF43C call 0x000127D0
	X86_PUSH32(0x1EF441u); sub_000127D0(ctx, base);
	// 001EF441 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF444 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF426, __imp__sub_001EF426)

X86_FUNC(__imp__sub_001EF445) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF445 mov eax, 0x27F1C0
	{ ctx.eax = (uint32_t)(0x27F1C0u); }
	// 001EF44A jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF445, __imp__sub_001EF445)

X86_FUNC(__imp__sub_001EF450) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF450 lea ecx, [ebp-0x41C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBE4u));
	// 001EF456 jmp 0x001025B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001025B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF450, __imp__sub_001EF450)

X86_FUNC(__imp__sub_001EF45B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF45B lea ecx, [ebp-0x430]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBD0u));
	// 001EF461 jmp 0x00102410
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00102410(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF45B, __imp__sub_001EF45B)

X86_FUNC(__imp__sub_001EF466) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF466 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF46B push 0xB6A
	{ uint32_t v = (uint32_t)(0xB6Au); X86_PUSH32(v); }
	// 001EF470 push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF475 mov eax, [ebp-0x424]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF47B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF47C call 0x000127D0
	X86_PUSH32(0x1EF481u); sub_000127D0(ctx, base);
	// 001EF481 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF484 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF466, __imp__sub_001EF466)

X86_FUNC(__imp__sub_001EF485) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF485 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF48A push 0xB89
	{ uint32_t v = (uint32_t)(0xB89u); X86_PUSH32(v); }
	// 001EF48F push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF494 mov eax, [ebp-0x420]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF49A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF49B call 0x000127D0
	X86_PUSH32(0x1EF4A0u); sub_000127D0(ctx, base);
	// 001EF4A0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF4A3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF485, __imp__sub_001EF485)

X86_FUNC(__imp__sub_001EF4A4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF4A4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF4A9 push 0xB8F
	{ uint32_t v = (uint32_t)(0xB8Fu); X86_PUSH32(v); }
	// 001EF4AE push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF4B3 mov eax, [ebp-0x420]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF4B9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF4BA call 0x000127D0
	X86_PUSH32(0x1EF4BFu); sub_000127D0(ctx, base);
	// 001EF4BF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF4C2 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF4A4, __imp__sub_001EF4A4)

X86_FUNC(__imp__sub_001EF4C3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF4C3 mov eax, 0x27F204
	{ ctx.eax = (uint32_t)(0x27F204u); }
	// 001EF4C8 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF4C3, __imp__sub_001EF4C3)

X86_FUNC(__imp__sub_001EF4D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF4D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF4D5 push 0x2E9
	{ uint32_t v = (uint32_t)(0x2E9u); X86_PUSH32(v); }
	// 001EF4DA push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF4DF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF4E2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF4E3 call 0x000127D0
	X86_PUSH32(0x1EF4E8u); sub_000127D0(ctx, base);
	// 001EF4E8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF4EB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF4D0, __imp__sub_001EF4D0)

X86_FUNC(__imp__sub_001EF4EC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF4EC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF4F1 push 0x2EE
	{ uint32_t v = (uint32_t)(0x2EEu); X86_PUSH32(v); }
	// 001EF4F6 push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF4FB mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF4FE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF4FF call 0x000127D0
	X86_PUSH32(0x1EF504u); sub_000127D0(ctx, base);
	// 001EF504 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF507 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF4EC, __imp__sub_001EF4EC)

X86_FUNC(__imp__sub_001EF508) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF508 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF50D push 0x2F3
	{ uint32_t v = (uint32_t)(0x2F3u); X86_PUSH32(v); }
	// 001EF512 push 0x269BD4
	{ uint32_t v = (uint32_t)(0x269BD4u); X86_PUSH32(v); }
	// 001EF517 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF51A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF51B call 0x000127D0
	X86_PUSH32(0x1EF520u); sub_000127D0(ctx, base);
	// 001EF520 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF523 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF508, __imp__sub_001EF508)

X86_FUNC(__imp__sub_001EF524) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF524 mov eax, 0x27F238
	{ ctx.eax = (uint32_t)(0x27F238u); }
	// 001EF529 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF524, __imp__sub_001EF524)

X86_FUNC(__imp__sub_001EF530) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF530 lea ecx, [ebp-0x78]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFF88u));
	// 001EF533 jmp 0x0010BD20
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0010BD20(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF530, __imp__sub_001EF530)

X86_FUNC(__imp__sub_001EF538) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF538 mov eax, 0x27F25C
	{ ctx.eax = (uint32_t)(0x27F25Cu); }
	// 001EF53D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF538, __imp__sub_001EF538)

X86_FUNC(__imp__sub_001EF550) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF550 push 0xEE570
	{ uint32_t v = (uint32_t)(0xEE570u); X86_PUSH32(v); }
	// 001EF555 push 0x0A
	{ uint32_t v = (uint32_t)(0xAu); X86_PUSH32(v); }
	// 001EF557 push 0x7E0
	{ uint32_t v = (uint32_t)(0x7E0u); X86_PUSH32(v); }
	// 001EF55C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF55F add eax, 0x10
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF562 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF563 call 0x001DEF24
	X86_PUSH32(0x1EF568u); sub_001DEF24(ctx, base);
	// 001EF568 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF550, __imp__sub_001EF550)

X86_FUNC(__imp__sub_001EF569) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF569 mov eax, 0x27F2A4
	{ ctx.eax = (uint32_t)(0x27F2A4u); }
	// 001EF56E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF569, __imp__sub_001EF569)

X86_FUNC(__imp__sub_001EF580) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF580 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF585 push 0xB4
	{ uint32_t v = (uint32_t)(0xB4u); X86_PUSH32(v); }
	// 001EF58A push 0x26AB38
	{ uint32_t v = (uint32_t)(0x26AB38u); X86_PUSH32(v); }
	// 001EF58F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF592 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF593 call 0x000127D0
	X86_PUSH32(0x1EF598u); sub_000127D0(ctx, base);
	// 001EF598 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF59B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF580, __imp__sub_001EF580)

X86_FUNC(__imp__sub_001EF59C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF59C mov eax, 0x27F2C8
	{ ctx.eax = (uint32_t)(0x27F2C8u); }
	// 001EF5A1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF59C, __imp__sub_001EF59C)

X86_FUNC(__imp__sub_001EF5B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF5B0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF5B3 add ecx, 0x2C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF5B6 jmp 0x001025E0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001025E0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF5B0, __imp__sub_001EF5B0)

X86_FUNC(__imp__sub_001EF5BB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF5BB mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF5BE add ecx, 0xA4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xA4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF5C4 jmp 0x000A8E00
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A8E00(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF5BB, __imp__sub_001EF5BB)

X86_FUNC(__imp__sub_001EF5C9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF5C9 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF5CC add ecx, 0x104
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x104u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF5D2 jmp 0x000A8FB0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A8FB0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF5C9, __imp__sub_001EF5C9)

X86_FUNC(__imp__sub_001EF5D7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF5D7 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF5DA add ecx, 0x170
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x170u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF5E0 jmp 0x001028B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001028B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF5D7, __imp__sub_001EF5D7)

X86_FUNC(__imp__sub_001EF5E5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF5E5 mov eax, 0x27F304
	{ ctx.eax = (uint32_t)(0x27F304u); }
	// 001EF5EA jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF5E5, __imp__sub_001EF5E5)

X86_FUNC(__imp__sub_001EF5F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF5F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF5F5 push 0x3C
	{ uint32_t v = (uint32_t)(0x3Cu); X86_PUSH32(v); }
	// 001EF5F7 push 0x26AB98
	{ uint32_t v = (uint32_t)(0x26AB98u); X86_PUSH32(v); }
	// 001EF5FC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF5FF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF600 call 0x000127D0
	X86_PUSH32(0x1EF605u); sub_000127D0(ctx, base);
	// 001EF605 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF608 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF5F0, __imp__sub_001EF5F0)

X86_FUNC(__imp__sub_001EF609) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF609 mov eax, 0x27F364
	{ ctx.eax = (uint32_t)(0x27F364u); }
	// 001EF60E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF609, __imp__sub_001EF609)

X86_FUNC(__imp__sub_001EF620) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF620 lea ecx, [ebp-0x30]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFD0u));
	// 001EF623 jmp 0x0010CED0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0010CED0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF620, __imp__sub_001EF620)

X86_FUNC(__imp__sub_001EF628) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF628 mov eax, 0x27F3F4
	{ ctx.eax = (uint32_t)(0x27F3F4u); }
	// 001EF62D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF628, __imp__sub_001EF628)

X86_FUNC(__imp__sub_001EF640) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF640 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF645 push 0x472
	{ uint32_t v = (uint32_t)(0x472u); X86_PUSH32(v); }
	// 001EF64A push 0x26AB98
	{ uint32_t v = (uint32_t)(0x26AB98u); X86_PUSH32(v); }
	// 001EF64F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF652 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF653 call 0x000127D0
	X86_PUSH32(0x1EF658u); sub_000127D0(ctx, base);
	// 001EF658 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF65B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF640, __imp__sub_001EF640)

X86_FUNC(__imp__sub_001EF65C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF65C mov eax, 0x27F418
	{ ctx.eax = (uint32_t)(0x27F418u); }
	// 001EF661 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF65C, __imp__sub_001EF65C)

X86_FUNC(__imp__sub_001EF670) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF670 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF673 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF676 jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF670, __imp__sub_001EF670)

X86_FUNC(__imp__sub_001EF67B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF67B mov eax, 0x27F43C
	{ ctx.eax = (uint32_t)(0x27F43Cu); }
	// 001EF680 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF67B, __imp__sub_001EF67B)

X86_FUNC(__imp__sub_001EF690) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF690 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF693 jmp 0x000F5310
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F5310(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF690, __imp__sub_001EF690)

X86_FUNC(__imp__sub_001EF698) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF698 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF69B add ecx, 0x45D8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x45D8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF6A1 jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF698, __imp__sub_001EF698)

X86_FUNC(__imp__sub_001EF6A6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6A6 mov eax, 0x27F468
	{ ctx.eax = (uint32_t)(0x27F468u); }
	// 001EF6AB jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF6A6, __imp__sub_001EF6A6)

X86_FUNC(__imp__sub_001EF6B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6B0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF6B3 jmp 0x000F8AC0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F8AC0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF6B0, __imp__sub_001EF6B0)

X86_FUNC(__imp__sub_001EF6B8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6B8 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF6BB add ecx, 0x11A0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x11A0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF6C1 jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF6B8, __imp__sub_001EF6B8)

X86_FUNC(__imp__sub_001EF6C6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6C6 mov eax, 0x27F494
	{ ctx.eax = (uint32_t)(0x27F494u); }
	// 001EF6CB jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF6C6, __imp__sub_001EF6C6)

X86_FUNC(__imp__sub_001EF6D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6D0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF6D3 jmp 0x000F5310
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F5310(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF6D0, __imp__sub_001EF6D0)

X86_FUNC(__imp__sub_001EF6D8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6D8 mov eax, 0x27F52C
	{ ctx.eax = (uint32_t)(0x27F52Cu); }
	// 001EF6DD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF6D8, __imp__sub_001EF6D8)

X86_FUNC(__imp__sub_001EF6F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF6F0 push 0xF1900
	{ uint32_t v = (uint32_t)(0xF1900u); X86_PUSH32(v); }
	// 001EF6F5 push 0x08
	{ uint32_t v = (uint32_t)(0x8u); X86_PUSH32(v); }
	// 001EF6F7 push 0x2B0
	{ uint32_t v = (uint32_t)(0x2B0u); X86_PUSH32(v); }
	// 001EF6FC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF6FF add eax, 0x10
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF702 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF703 call 0x001DEF24
	X86_PUSH32(0x1EF708u); sub_001DEF24(ctx, base);
	// 001EF708 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF6F0, __imp__sub_001EF6F0)

X86_FUNC(__imp__sub_001EF709) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF709 mov eax, 0x27F550
	{ ctx.eax = (uint32_t)(0x27F550u); }
	// 001EF70E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF709, __imp__sub_001EF709)

X86_FUNC(__imp__sub_001EF720) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF720 mov ecx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF723 jmp 0x000F5310
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F5310(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF720, __imp__sub_001EF720)

X86_FUNC(__imp__sub_001EF728) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF728 mov ecx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF72B add ecx, 0x45D8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x45D8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF731 jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF728, __imp__sub_001EF728)

X86_FUNC(__imp__sub_001EF736) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF736 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF739 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF73C jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF736, __imp__sub_001EF736)

X86_FUNC(__imp__sub_001EF741) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF741 mov eax, 0x27F584
	{ ctx.eax = (uint32_t)(0x27F584u); }
	// 001EF746 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF741, __imp__sub_001EF741)

X86_FUNC(__imp__sub_001EF750) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF750 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF755 push 0xC6F
	{ uint32_t v = (uint32_t)(0xC6Fu); X86_PUSH32(v); }
	// 001EF75A push 0x26AFE4
	{ uint32_t v = (uint32_t)(0x26AFE4u); X86_PUSH32(v); }
	// 001EF75F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF762 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF763 call 0x000127D0
	X86_PUSH32(0x1EF768u); sub_000127D0(ctx, base);
	// 001EF768 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF76B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF750, __imp__sub_001EF750)

X86_FUNC(__imp__sub_001EF76C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF76C mov eax, 0x27F5CC
	{ ctx.eax = (uint32_t)(0x27F5CCu); }
	// 001EF771 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF76C, __imp__sub_001EF76C)

X86_FUNC(__imp__sub_001EF780) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF780 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF785 push 0xD8F
	{ uint32_t v = (uint32_t)(0xD8Fu); X86_PUSH32(v); }
	// 001EF78A push 0x26AFE4
	{ uint32_t v = (uint32_t)(0x26AFE4u); X86_PUSH32(v); }
	// 001EF78F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF792 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF793 call 0x000127D0
	X86_PUSH32(0x1EF798u); sub_000127D0(ctx, base);
	// 001EF798 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF79B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF780, __imp__sub_001EF780)

X86_FUNC(__imp__sub_001EF79C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF79C mov eax, 0x27F5F0
	{ ctx.eax = (uint32_t)(0x27F5F0u); }
	// 001EF7A1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF79C, __imp__sub_001EF79C)

X86_FUNC(__imp__sub_001EF7B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF7B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF7B5 push 0xFF0
	{ uint32_t v = (uint32_t)(0xFF0u); X86_PUSH32(v); }
	// 001EF7BA push 0x26AFE4
	{ uint32_t v = (uint32_t)(0x26AFE4u); X86_PUSH32(v); }
	// 001EF7BF mov eax, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF7C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF7C3 call 0x000127D0
	X86_PUSH32(0x1EF7C8u); sub_000127D0(ctx, base);
	// 001EF7C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF7CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF7B0, __imp__sub_001EF7B0)

X86_FUNC(__imp__sub_001EF7CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF7CC mov ecx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF7CF jmp 0x000F5310
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F5310(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF7CC, __imp__sub_001EF7CC)

X86_FUNC(__imp__sub_001EF7D4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF7D4 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF7D7 add ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF7DA jmp 0x000EC300
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000EC300(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF7D4, __imp__sub_001EF7D4)

X86_FUNC(__imp__sub_001EF7DF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF7DF mov eax, 0x27F648
	{ ctx.eax = (uint32_t)(0x27F648u); }
	// 001EF7E4 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF7DF, __imp__sub_001EF7DF)

X86_FUNC(__imp__sub_001EF7F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF7F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF7F5 push 0xBBA
	{ uint32_t v = (uint32_t)(0xBBAu); X86_PUSH32(v); }
	// 001EF7FA push 0x26AFE4
	{ uint32_t v = (uint32_t)(0x26AFE4u); X86_PUSH32(v); }
	// 001EF7FF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF802 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF803 call 0x000127D0
	X86_PUSH32(0x1EF808u); sub_000127D0(ctx, base);
	// 001EF808 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF80B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF7F0, __imp__sub_001EF7F0)

X86_FUNC(__imp__sub_001EF80C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF80C mov eax, 0x27F66C
	{ ctx.eax = (uint32_t)(0x27F66Cu); }
	// 001EF811 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF80C, __imp__sub_001EF80C)

X86_FUNC(__imp__sub_001EF820) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF820 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF823 jmp 0x0010BD50
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0010BD50(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF820, __imp__sub_001EF820)

X86_FUNC(__imp__sub_001EF828) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF828 mov eax, 0x27F6B4
	{ ctx.eax = (uint32_t)(0x27F6B4u); }
	// 001EF82D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF828, __imp__sub_001EF828)

X86_FUNC(__imp__sub_001EF840) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF840 lea ecx, [ebp-0x30]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFD0u));
	// 001EF843 jmp 0x0010CED0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0010CED0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF840, __imp__sub_001EF840)

X86_FUNC(__imp__sub_001EF848) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF848 mov eax, 0x27F6FC
	{ ctx.eax = (uint32_t)(0x27F6FCu); }
	// 001EF84D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF848, __imp__sub_001EF848)

X86_FUNC(__imp__sub_001EF860) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF860 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF865 push 0x183
	{ uint32_t v = (uint32_t)(0x183u); X86_PUSH32(v); }
	// 001EF86A push 0x26B1B8
	{ uint32_t v = (uint32_t)(0x26B1B8u); X86_PUSH32(v); }
	// 001EF86F mov eax, [ebp-0x138]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEC8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF875 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF876 call 0x000127D0
	X86_PUSH32(0x1EF87Bu); sub_000127D0(ctx, base);
	// 001EF87B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF87E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF860, __imp__sub_001EF860)

X86_FUNC(__imp__sub_001EF87F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF87F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF884 push 0x23F
	{ uint32_t v = (uint32_t)(0x23Fu); X86_PUSH32(v); }
	// 001EF889 push 0x26B1B8
	{ uint32_t v = (uint32_t)(0x26B1B8u); X86_PUSH32(v); }
	// 001EF88E mov eax, [ebp-0x138]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEC8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF894 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF895 call 0x000127D0
	X86_PUSH32(0x1EF89Au); sub_000127D0(ctx, base);
	// 001EF89A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF89D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF87F, __imp__sub_001EF87F)

X86_FUNC(__imp__sub_001EF89E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF89E mov eax, 0x27F728
	{ ctx.eax = (uint32_t)(0x27F728u); }
	// 001EF8A3 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF89E, __imp__sub_001EF89E)

X86_FUNC(__imp__sub_001EF8B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF8B0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF8B3 jmp 0x0010D5A0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0010D5A0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF8B0, __imp__sub_001EF8B0)

X86_FUNC(__imp__sub_001EF8B8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF8B8 mov eax, 0x27F770
	{ ctx.eax = (uint32_t)(0x27F770u); }
	// 001EF8BD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF8B8, __imp__sub_001EF8B8)

X86_FUNC(__imp__sub_001EF8D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF8D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF8D5 push 0xA6
	{ uint32_t v = (uint32_t)(0xA6u); X86_PUSH32(v); }
	// 001EF8DA push 0x26B404
	{ uint32_t v = (uint32_t)(0x26B404u); X86_PUSH32(v); }
	// 001EF8DF mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF8E5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF8E6 call 0x000127D0
	X86_PUSH32(0x1EF8EBu); sub_000127D0(ctx, base);
	// 001EF8EB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF8EE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF8D0, __imp__sub_001EF8D0)

X86_FUNC(__imp__sub_001EF8EF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF8EF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF8F4 push 0xDA
	{ uint32_t v = (uint32_t)(0xDAu); X86_PUSH32(v); }
	// 001EF8F9 push 0x26B404
	{ uint32_t v = (uint32_t)(0x26B404u); X86_PUSH32(v); }
	// 001EF8FE mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF904 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF905 call 0x000127D0
	X86_PUSH32(0x1EF90Au); sub_000127D0(ctx, base);
	// 001EF90A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF90D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF8EF, __imp__sub_001EF8EF)

X86_FUNC(__imp__sub_001EF90E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF90E push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF913 push 0xE5
	{ uint32_t v = (uint32_t)(0xE5u); X86_PUSH32(v); }
	// 001EF918 push 0x26B404
	{ uint32_t v = (uint32_t)(0x26B404u); X86_PUSH32(v); }
	// 001EF91D mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF923 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF924 call 0x000127D0
	X86_PUSH32(0x1EF929u); sub_000127D0(ctx, base);
	// 001EF929 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF92C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF90E, __imp__sub_001EF90E)

X86_FUNC(__imp__sub_001EF92D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF92D push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF932 push 0xE8
	{ uint32_t v = (uint32_t)(0xE8u); X86_PUSH32(v); }
	// 001EF937 push 0x26B404
	{ uint32_t v = (uint32_t)(0x26B404u); X86_PUSH32(v); }
	// 001EF93C mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF942 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF943 call 0x000127D0
	X86_PUSH32(0x1EF948u); sub_000127D0(ctx, base);
	// 001EF948 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF94B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF92D, __imp__sub_001EF92D)

X86_FUNC(__imp__sub_001EF94C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF94C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF951 push 0xEB
	{ uint32_t v = (uint32_t)(0xEBu); X86_PUSH32(v); }
	// 001EF956 push 0x26B404
	{ uint32_t v = (uint32_t)(0x26B404u); X86_PUSH32(v); }
	// 001EF95B mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF961 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF962 call 0x000127D0
	X86_PUSH32(0x1EF967u); sub_000127D0(ctx, base);
	// 001EF967 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF96A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF94C, __imp__sub_001EF94C)

X86_FUNC(__imp__sub_001EF96B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF96B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF970 push 0xEE
	{ uint32_t v = (uint32_t)(0xEEu); X86_PUSH32(v); }
	// 001EF975 push 0x26B404
	{ uint32_t v = (uint32_t)(0x26B404u); X86_PUSH32(v); }
	// 001EF97A mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF980 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF981 call 0x000127D0
	X86_PUSH32(0x1EF986u); sub_000127D0(ctx, base);
	// 001EF986 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF989 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF96B, __imp__sub_001EF96B)

X86_FUNC(__imp__sub_001EF98A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF98A mov eax, 0x27F7BC
	{ ctx.eax = (uint32_t)(0x27F7BCu); }
	// 001EF98F jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF98A, __imp__sub_001EF98A)

X86_FUNC(__imp__sub_001EF9A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF9A0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EF9A3 jmp 0x000F8AC0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000F8AC0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF9A0, __imp__sub_001EF9A0)

X86_FUNC(__imp__sub_001EF9A8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF9A8 mov eax, 0x27F804
	{ ctx.eax = (uint32_t)(0x27F804u); }
	// 001EF9AD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF9A8, __imp__sub_001EF9A8)

X86_FUNC(__imp__sub_001EF9C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF9C0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF9C5 push 0x3CD
	{ uint32_t v = (uint32_t)(0x3CDu); X86_PUSH32(v); }
	// 001EF9CA push 0x26B5A4
	{ uint32_t v = (uint32_t)(0x26B5A4u); X86_PUSH32(v); }
	// 001EF9CF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EF9D2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EF9D3 call 0x000127D0
	X86_PUSH32(0x1EF9D8u); sub_000127D0(ctx, base);
	// 001EF9D8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EF9DB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF9C0, __imp__sub_001EF9C0)

X86_FUNC(__imp__sub_001EF9DC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF9DC mov eax, 0x27F828
	{ ctx.eax = (uint32_t)(0x27F828u); }
	// 001EF9E1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EF9DC, __imp__sub_001EF9DC)

X86_FUNC(__imp__sub_001EF9F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EF9F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EF9F5 push 0x50D
	{ uint32_t v = (uint32_t)(0x50Du); X86_PUSH32(v); }
	// 001EF9FA push 0x26B680
	{ uint32_t v = (uint32_t)(0x26B680u); X86_PUSH32(v); }
	// 001EF9FF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFA02 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFA03 call 0x000127D0
	X86_PUSH32(0x1EFA08u); sub_000127D0(ctx, base);
	// 001EFA08 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFA0B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EF9F0, __imp__sub_001EF9F0)

X86_FUNC(__imp__sub_001EFA0C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA0C mov eax, 0x27F84C
	{ ctx.eax = (uint32_t)(0x27F84Cu); }
	// 001EFA11 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFA0C, __imp__sub_001EFA0C)

X86_FUNC(__imp__sub_001EFA20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA20 push 0xFE260
	{ uint32_t v = (uint32_t)(0xFE260u); X86_PUSH32(v); }
	// 001EFA25 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EFA27 push 0x48
	{ uint32_t v = (uint32_t)(0x48u); X86_PUSH32(v); }
	// 001EFA29 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFA2C add eax, 0x118
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x118u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFA31 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFA32 call 0x001DEF24
	X86_PUSH32(0x1EFA37u); sub_001DEF24(ctx, base);
	// 001EFA37 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFA20, __imp__sub_001EFA20)

X86_FUNC(__imp__sub_001EFA38) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA38 mov eax, 0x27F870
	{ ctx.eax = (uint32_t)(0x27F870u); }
	// 001EFA3D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFA38, __imp__sub_001EFA38)

X86_FUNC(__imp__sub_001EFA50) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA50 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFA55 push 0x5E
	{ uint32_t v = (uint32_t)(0x5Eu); X86_PUSH32(v); }
	// 001EFA57 push 0x26B754
	{ uint32_t v = (uint32_t)(0x26B754u); X86_PUSH32(v); }
	// 001EFA5C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFA5F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFA60 call 0x000127D0
	X86_PUSH32(0x1EFA65u); sub_000127D0(ctx, base);
	// 001EFA65 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFA68 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFA50, __imp__sub_001EFA50)

X86_FUNC(__imp__sub_001EFA69) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA69 mov eax, 0x27F8B8
	{ ctx.eax = (uint32_t)(0x27F8B8u); }
	// 001EFA6E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFA69, __imp__sub_001EFA69)

X86_FUNC(__imp__sub_001EFA80) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA80 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFA85 push 0x2D3
	{ uint32_t v = (uint32_t)(0x2D3u); X86_PUSH32(v); }
	// 001EFA8A push 0x26B754
	{ uint32_t v = (uint32_t)(0x26B754u); X86_PUSH32(v); }
	// 001EFA8F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFA92 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFA93 call 0x000127D0
	X86_PUSH32(0x1EFA98u); sub_000127D0(ctx, base);
	// 001EFA98 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFA9B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFA80, __imp__sub_001EFA80)

X86_FUNC(__imp__sub_001EFA9C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFA9C mov eax, 0x27F8DC
	{ ctx.eax = (uint32_t)(0x27F8DCu); }
	// 001EFAA1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFA9C, __imp__sub_001EFA9C)

X86_FUNC(__imp__sub_001EFAB0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFAB0 lea ecx, [ebp-0x30]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFD0u));
	// 001EFAB3 jmp 0x0010CED0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0010CED0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFAB0, __imp__sub_001EFAB0)

X86_FUNC(__imp__sub_001EFAB8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFAB8 mov eax, 0x27F900
	{ ctx.eax = (uint32_t)(0x27F900u); }
	// 001EFABD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFAB8, __imp__sub_001EFAB8)

X86_FUNC(__imp__sub_001EFAD0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFAD0 push 0xFFEC0
	{ uint32_t v = (uint32_t)(0xFFEC0u); X86_PUSH32(v); }
	// 001EFAD5 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EFAD7 push 0x430
	{ uint32_t v = (uint32_t)(0x430u); X86_PUSH32(v); }
	// 001EFADC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFADF add eax, 0xC0
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xC0u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFAE4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFAE5 call 0x001DEF24
	X86_PUSH32(0x1EFAEAu); sub_001DEF24(ctx, base);
	// 001EFAEA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFAD0, __imp__sub_001EFAD0)

X86_FUNC(__imp__sub_001EFAEB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFAEB mov eax, 0x27F948
	{ ctx.eax = (uint32_t)(0x27F948u); }
	// 001EFAF0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFAEB, __imp__sub_001EFAEB)

X86_FUNC(__imp__sub_001EFB00) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB00 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFB05 push 0x63
	{ uint32_t v = (uint32_t)(0x63u); X86_PUSH32(v); }
	// 001EFB07 push 0x26BC08
	{ uint32_t v = (uint32_t)(0x26BC08u); X86_PUSH32(v); }
	// 001EFB0C mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFB0F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFB10 call 0x000127D0
	X86_PUSH32(0x1EFB15u); sub_000127D0(ctx, base);
	// 001EFB15 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFB18 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFB00, __imp__sub_001EFB00)

X86_FUNC(__imp__sub_001EFB19) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB19 mov eax, 0x27F96C
	{ ctx.eax = (uint32_t)(0x27F96Cu); }
	// 001EFB1E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFB19, __imp__sub_001EFB19)

X86_FUNC(__imp__sub_001EFB30) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB30 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFB35 push 0x63
	{ uint32_t v = (uint32_t)(0x63u); X86_PUSH32(v); }
	// 001EFB37 push 0x26BD28
	{ uint32_t v = (uint32_t)(0x26BD28u); X86_PUSH32(v); }
	// 001EFB3C mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFB3F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFB40 call 0x000127D0
	X86_PUSH32(0x1EFB45u); sub_000127D0(ctx, base);
	// 001EFB45 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFB48 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFB30, __imp__sub_001EFB30)

X86_FUNC(__imp__sub_001EFB49) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB49 mov eax, 0x27F990
	{ ctx.eax = (uint32_t)(0x27F990u); }
	// 001EFB4E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFB49, __imp__sub_001EFB49)

X86_FUNC(__imp__sub_001EFB60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB60 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFB65 push 0x59
	{ uint32_t v = (uint32_t)(0x59u); X86_PUSH32(v); }
	// 001EFB67 push 0x26BFA4
	{ uint32_t v = (uint32_t)(0x26BFA4u); X86_PUSH32(v); }
	// 001EFB6C mov eax, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFB6F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFB70 call 0x000127D0
	X86_PUSH32(0x1EFB75u); sub_000127D0(ctx, base);
	// 001EFB75 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFB78 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFB60, __imp__sub_001EFB60)

X86_FUNC(__imp__sub_001EFB79) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB79 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFB7E push 0x63
	{ uint32_t v = (uint32_t)(0x63u); X86_PUSH32(v); }
	// 001EFB80 push 0x26BFA4
	{ uint32_t v = (uint32_t)(0x26BFA4u); X86_PUSH32(v); }
	// 001EFB85 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFB88 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFB89 call 0x000127D0
	X86_PUSH32(0x1EFB8Eu); sub_000127D0(ctx, base);
	// 001EFB8E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFB91 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFB79, __imp__sub_001EFB79)

X86_FUNC(__imp__sub_001EFB92) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFB92 mov eax, 0x27F9BC
	{ ctx.eax = (uint32_t)(0x27F9BCu); }
	// 001EFB97 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFB92, __imp__sub_001EFB92)

X86_FUNC(__imp__sub_001EFBA0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFBA0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFBA5 push 0x168
	{ uint32_t v = (uint32_t)(0x168u); X86_PUSH32(v); }
	// 001EFBAA push 0x26C038
	{ uint32_t v = (uint32_t)(0x26C038u); X86_PUSH32(v); }
	// 001EFBAF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFBB2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFBB3 call 0x000127D0
	X86_PUSH32(0x1EFBB8u); sub_000127D0(ctx, base);
	// 001EFBB8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFBBB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFBA0, __imp__sub_001EFBA0)

X86_FUNC(__imp__sub_001EFBBC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFBBC mov eax, 0x27F9E0
	{ ctx.eax = (uint32_t)(0x27F9E0u); }
	// 001EFBC1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFBBC, __imp__sub_001EFBBC)

X86_FUNC(__imp__sub_001EFBD0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFBD0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EFBD3 add ecx, 0x1C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFBD6 jmp 0x00123540
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00123540(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFBD0, __imp__sub_001EFBD0)

X86_FUNC(__imp__sub_001EFBDB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFBDB mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001EFBDE add ecx, 0x6C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x6Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFBE1 jmp 0x00123780
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00123780(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFBDB, __imp__sub_001EFBDB)

X86_FUNC(__imp__sub_001EFBE6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFBE6 mov eax, 0x27FA0C
	{ ctx.eax = (uint32_t)(0x27FA0Cu); }
	// 001EFBEB jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFBE6, __imp__sub_001EFBE6)

X86_FUNC(__imp__sub_001EFBF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFBF0 push 0x111650
	{ uint32_t v = (uint32_t)(0x111650u); X86_PUSH32(v); }
	// 001EFBF5 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 001EFBF7 push 0x10C
	{ uint32_t v = (uint32_t)(0x10Cu); X86_PUSH32(v); }
	// 001EFBFC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFBFF add eax, 0x900
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x900u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFC04 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFC05 call 0x001DEF24
	X86_PUSH32(0x1EFC0Au); sub_001DEF24(ctx, base);
	// 001EFC0A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFBF0, __imp__sub_001EFBF0)

X86_FUNC(__imp__sub_001EFC0B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFC0B mov eax, 0x27FA5C
	{ ctx.eax = (uint32_t)(0x27FA5Cu); }
	// 001EFC10 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFC0B, __imp__sub_001EFC0B)

X86_FUNC(__imp__sub_001EFC20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFC20 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFC25 push 0x7B3
	{ uint32_t v = (uint32_t)(0x7B3u); X86_PUSH32(v); }
	// 001EFC2A push 0x26C250
	{ uint32_t v = (uint32_t)(0x26C250u); X86_PUSH32(v); }
	// 001EFC2F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFC32 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFC33 call 0x000127D0
	X86_PUSH32(0x1EFC38u); sub_000127D0(ctx, base);
	// 001EFC38 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFC3B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFC20, __imp__sub_001EFC20)

X86_FUNC(__imp__sub_001EFC3C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFC3C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFC41 push 0x7B5
	{ uint32_t v = (uint32_t)(0x7B5u); X86_PUSH32(v); }
	// 001EFC46 push 0x26C250
	{ uint32_t v = (uint32_t)(0x26C250u); X86_PUSH32(v); }
	// 001EFC4B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFC4E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFC4F call 0x000127D0
	X86_PUSH32(0x1EFC54u); sub_000127D0(ctx, base);
	// 001EFC54 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFC57 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFC3C, __imp__sub_001EFC3C)

X86_FUNC(__imp__sub_001EFC58) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFC58 mov eax, 0x27FAAC
	{ ctx.eax = (uint32_t)(0x27FAACu); }
	// 001EFC5D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFC58, __imp__sub_001EFC58)

X86_FUNC(__imp__sub_001EFC70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFC70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFC75 push 0x7F1
	{ uint32_t v = (uint32_t)(0x7F1u); X86_PUSH32(v); }
	// 001EFC7A push 0x26C250
	{ uint32_t v = (uint32_t)(0x26C250u); X86_PUSH32(v); }
	// 001EFC7F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFC82 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFC83 call 0x000127D0
	X86_PUSH32(0x1EFC88u); sub_000127D0(ctx, base);
	// 001EFC88 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFC8B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFC70, __imp__sub_001EFC70)

X86_FUNC(__imp__sub_001EFC8C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFC8C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFC91 push 0x7F3
	{ uint32_t v = (uint32_t)(0x7F3u); X86_PUSH32(v); }
	// 001EFC96 push 0x26C250
	{ uint32_t v = (uint32_t)(0x26C250u); X86_PUSH32(v); }
	// 001EFC9B mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFC9E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFC9F call 0x000127D0
	X86_PUSH32(0x1EFCA4u); sub_000127D0(ctx, base);
	// 001EFCA4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFCA7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFC8C, __imp__sub_001EFC8C)

X86_FUNC(__imp__sub_001EFCA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFCA8 mov eax, 0x27FAD8
	{ ctx.eax = (uint32_t)(0x27FAD8u); }
	// 001EFCAD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFCA8, __imp__sub_001EFCA8)

X86_FUNC(__imp__sub_001EFCC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFCC0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFCC5 push 0xA3
	{ uint32_t v = (uint32_t)(0xA3u); X86_PUSH32(v); }
	// 001EFCCA push 0x26CAE8
	{ uint32_t v = (uint32_t)(0x26CAE8u); X86_PUSH32(v); }
	// 001EFCCF mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFCD5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFCD6 call 0x000127D0
	X86_PUSH32(0x1EFCDBu); sub_000127D0(ctx, base);
	// 001EFCDB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFCDE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFCC0, __imp__sub_001EFCC0)

X86_FUNC(__imp__sub_001EFCDF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFCDF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFCE4 push 0xA4
	{ uint32_t v = (uint32_t)(0xA4u); X86_PUSH32(v); }
	// 001EFCE9 push 0x26CAE8
	{ uint32_t v = (uint32_t)(0x26CAE8u); X86_PUSH32(v); }
	// 001EFCEE mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFCF4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFCF5 call 0x000127D0
	X86_PUSH32(0x1EFCFAu); sub_000127D0(ctx, base);
	// 001EFCFA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFCFD ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFCDF, __imp__sub_001EFCDF)

X86_FUNC(__imp__sub_001EFCFE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFCFE mov eax, 0x27FB04
	{ ctx.eax = (uint32_t)(0x27FB04u); }
	// 001EFD03 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFCFE, __imp__sub_001EFCFE)

X86_FUNC(__imp__sub_001EFD10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFD10 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFD15 push 0x5E
	{ uint32_t v = (uint32_t)(0x5Eu); X86_PUSH32(v); }
	// 001EFD17 push 0x26CCB4
	{ uint32_t v = (uint32_t)(0x26CCB4u); X86_PUSH32(v); }
	// 001EFD1C mov eax, [ebp-0x50]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFB0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFD1F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFD20 call 0x000127D0
	X86_PUSH32(0x1EFD25u); sub_000127D0(ctx, base);
	// 001EFD25 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFD28 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFD10, __imp__sub_001EFD10)

X86_FUNC(__imp__sub_001EFD29) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFD29 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFD2E push 0x60
	{ uint32_t v = (uint32_t)(0x60u); X86_PUSH32(v); }
	// 001EFD30 push 0x26CCB4
	{ uint32_t v = (uint32_t)(0x26CCB4u); X86_PUSH32(v); }
	// 001EFD35 mov eax, [ebp-0x50]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFB0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFD38 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFD39 call 0x000127D0
	X86_PUSH32(0x1EFD3Eu); sub_000127D0(ctx, base);
	// 001EFD3E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFD41 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFD29, __imp__sub_001EFD29)

X86_FUNC(__imp__sub_001EFD42) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFD42 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFD47 push 0x71
	{ uint32_t v = (uint32_t)(0x71u); X86_PUSH32(v); }
	// 001EFD49 push 0x26CCB4
	{ uint32_t v = (uint32_t)(0x26CCB4u); X86_PUSH32(v); }
	// 001EFD4E mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFD51 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFD52 call 0x000127D0
	X86_PUSH32(0x1EFD57u); sub_000127D0(ctx, base);
	// 001EFD57 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFD5A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFD42, __imp__sub_001EFD42)

X86_FUNC(__imp__sub_001EFD5B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFD5B mov eax, 0x27FB38
	{ ctx.eax = (uint32_t)(0x27FB38u); }
	// 001EFD60 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFD5B, __imp__sub_001EFD5B)

X86_FUNC(__imp__sub_001EFD70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFD70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFD75 push 0xB8
	{ uint32_t v = (uint32_t)(0xB8u); X86_PUSH32(v); }
	// 001EFD7A push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFD7F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFD82 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFD83 call 0x000127D0
	X86_PUSH32(0x1EFD88u); sub_000127D0(ctx, base);
	// 001EFD88 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFD8B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFD70, __imp__sub_001EFD70)

X86_FUNC(__imp__sub_001EFD8C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFD8C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFD91 push 0xD9
	{ uint32_t v = (uint32_t)(0xD9u); X86_PUSH32(v); }
	// 001EFD96 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFD9B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFD9E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFD9F call 0x000127D0
	X86_PUSH32(0x1EFDA4u); sub_000127D0(ctx, base);
	// 001EFDA4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFDA7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFD8C, __imp__sub_001EFD8C)

X86_FUNC(__imp__sub_001EFDA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFDA8 mov eax, 0x27FB64
	{ ctx.eax = (uint32_t)(0x27FB64u); }
	// 001EFDAD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFDA8, __imp__sub_001EFDA8)

X86_FUNC(__imp__sub_001EFDC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFDC0 lea ecx, [ebp-0x620]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFF9E0u));
	// 001EFDC6 jmp 0x001386B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001386B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFDC0, __imp__sub_001EFDC0)

X86_FUNC(__imp__sub_001EFDCB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFDCB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFDD0 push 0x9D9
	{ uint32_t v = (uint32_t)(0x9D9u); X86_PUSH32(v); }
	// 001EFDD5 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFDDA mov eax, [ebp-0x640]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFDE0 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFDE1 call 0x000127D0
	X86_PUSH32(0x1EFDE6u); sub_000127D0(ctx, base);
	// 001EFDE6 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFDE9 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFDCB, __imp__sub_001EFDCB)

X86_FUNC(__imp__sub_001EFDEA) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFDEA push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFDEF push 0xA67
	{ uint32_t v = (uint32_t)(0xA67u); X86_PUSH32(v); }
	// 001EFDF4 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFDF9 mov eax, [ebp-0x640]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFDFF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFE00 call 0x000127D0
	X86_PUSH32(0x1EFE05u); sub_000127D0(ctx, base);
	// 001EFE05 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFE08 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFDEA, __imp__sub_001EFDEA)

X86_FUNC(__imp__sub_001EFE09) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFE09 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFE0E push 0xA86
	{ uint32_t v = (uint32_t)(0xA86u); X86_PUSH32(v); }
	// 001EFE13 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFE18 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFE1E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFE1F call 0x000127D0
	X86_PUSH32(0x1EFE24u); sub_000127D0(ctx, base);
	// 001EFE24 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFE27 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFE09, __imp__sub_001EFE09)

X86_FUNC(__imp__sub_001EFE28) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFE28 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFE2D push 0xA89
	{ uint32_t v = (uint32_t)(0xA89u); X86_PUSH32(v); }
	// 001EFE32 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFE37 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFE3D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFE3E call 0x000127D0
	X86_PUSH32(0x1EFE43u); sub_000127D0(ctx, base);
	// 001EFE43 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFE46 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFE28, __imp__sub_001EFE28)

X86_FUNC(__imp__sub_001EFE47) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFE47 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFE4C push 0xA8C
	{ uint32_t v = (uint32_t)(0xA8Cu); X86_PUSH32(v); }
	// 001EFE51 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFE56 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFE5C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFE5D call 0x000127D0
	X86_PUSH32(0x1EFE62u); sub_000127D0(ctx, base);
	// 001EFE62 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFE65 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFE47, __imp__sub_001EFE47)

X86_FUNC(__imp__sub_001EFE66) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFE66 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFE6B push 0xA8F
	{ uint32_t v = (uint32_t)(0xA8Fu); X86_PUSH32(v); }
	// 001EFE70 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFE75 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFE7B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFE7C call 0x000127D0
	X86_PUSH32(0x1EFE81u); sub_000127D0(ctx, base);
	// 001EFE81 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFE84 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFE66, __imp__sub_001EFE66)

X86_FUNC(__imp__sub_001EFE85) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFE85 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFE8A push 0xAA6
	{ uint32_t v = (uint32_t)(0xAA6u); X86_PUSH32(v); }
	// 001EFE8F push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFE94 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFE9A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFE9B call 0x000127D0
	X86_PUSH32(0x1EFEA0u); sub_000127D0(ctx, base);
	// 001EFEA0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFEA3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFE85, __imp__sub_001EFE85)

X86_FUNC(__imp__sub_001EFEA4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFEA4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFEA9 push 0xAAB
	{ uint32_t v = (uint32_t)(0xAABu); X86_PUSH32(v); }
	// 001EFEAE push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFEB3 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFEB9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFEBA call 0x000127D0
	X86_PUSH32(0x1EFEBFu); sub_000127D0(ctx, base);
	// 001EFEBF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFEC2 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFEA4, __imp__sub_001EFEA4)

X86_FUNC(__imp__sub_001EFEC3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFEC3 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFEC8 push 0xAC0
	{ uint32_t v = (uint32_t)(0xAC0u); X86_PUSH32(v); }
	// 001EFECD push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFED2 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFED8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFED9 call 0x000127D0
	X86_PUSH32(0x1EFEDEu); sub_000127D0(ctx, base);
	// 001EFEDE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFEE1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFEC3, __imp__sub_001EFEC3)

X86_FUNC(__imp__sub_001EFEE2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFEE2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFEE7 push 0xADF
	{ uint32_t v = (uint32_t)(0xADFu); X86_PUSH32(v); }
	// 001EFEEC push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFEF1 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFEF7 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFEF8 call 0x000127D0
	X86_PUSH32(0x1EFEFDu); sub_000127D0(ctx, base);
	// 001EFEFD add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFF00 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFEE2, __imp__sub_001EFEE2)

X86_FUNC(__imp__sub_001EFF01) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF01 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFF06 push 0xAE4
	{ uint32_t v = (uint32_t)(0xAE4u); X86_PUSH32(v); }
	// 001EFF0B push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFF10 mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFF16 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFF17 call 0x000127D0
	X86_PUSH32(0x1EFF1Cu); sub_000127D0(ctx, base);
	// 001EFF1C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFF1F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFF01, __imp__sub_001EFF01)

X86_FUNC(__imp__sub_001EFF20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF20 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFF25 push 0xAE8
	{ uint32_t v = (uint32_t)(0xAE8u); X86_PUSH32(v); }
	// 001EFF2A push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFF2F mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFF35 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFF36 call 0x000127D0
	X86_PUSH32(0x1EFF3Bu); sub_000127D0(ctx, base);
	// 001EFF3B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFF3E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFF20, __imp__sub_001EFF20)

X86_FUNC(__imp__sub_001EFF3F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF3F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFF44 push 0xB6D
	{ uint32_t v = (uint32_t)(0xB6Du); X86_PUSH32(v); }
	// 001EFF49 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFF4E mov eax, [ebp-0x664]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF99Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFF54 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFF55 call 0x000127D0
	X86_PUSH32(0x1EFF5Au); sub_000127D0(ctx, base);
	// 001EFF5A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFF5D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFF3F, __imp__sub_001EFF3F)

X86_FUNC(__imp__sub_001EFF5E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF5E mov eax, 0x27FBF0
	{ ctx.eax = (uint32_t)(0x27FBF0u); }
	// 001EFF63 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFF5E, __imp__sub_001EFF5E)

X86_FUNC(__imp__sub_001EFF70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF70 lea ecx, [ebp-0x620]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFF9E0u));
	// 001EFF76 jmp 0x001386B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001386B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001EFF70, __imp__sub_001EFF70)

X86_FUNC(__imp__sub_001EFF7B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF7B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFF80 push 0xBB4
	{ uint32_t v = (uint32_t)(0xBB4u); X86_PUSH32(v); }
	// 001EFF85 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFF8A mov eax, [ebp-0x690]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF970u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFF90 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFF91 call 0x000127D0
	X86_PUSH32(0x1EFF96u); sub_000127D0(ctx, base);
	// 001EFF96 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFF99 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFF7B, __imp__sub_001EFF7B)

X86_FUNC(__imp__sub_001EFF9A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFF9A push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFF9F push 0xC4A
	{ uint32_t v = (uint32_t)(0xC4Au); X86_PUSH32(v); }
	// 001EFFA4 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFFA9 mov eax, [ebp-0x690]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF970u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFFAF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFFB0 call 0x000127D0
	X86_PUSH32(0x1EFFB5u); sub_000127D0(ctx, base);
	// 001EFFB5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFFB8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFF9A, __imp__sub_001EFF9A)

X86_FUNC(__imp__sub_001EFFB9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFFB9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFFBE push 0xC7F
	{ uint32_t v = (uint32_t)(0xC7Fu); X86_PUSH32(v); }
	// 001EFFC3 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFFC8 mov eax, [ebp-0x690]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF970u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFFCE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFFCF call 0x000127D0
	X86_PUSH32(0x1EFFD4u); sub_000127D0(ctx, base);
	// 001EFFD4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFFD7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFFB9, __imp__sub_001EFFB9)

X86_FUNC(__imp__sub_001EFFD8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFFD8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFFDD push 0xCA0
	{ uint32_t v = (uint32_t)(0xCA0u); X86_PUSH32(v); }
	// 001EFFE2 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001EFFE7 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001EFFED push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001EFFEE call 0x000127D0
	X86_PUSH32(0x1EFFF3u); sub_000127D0(ctx, base);
	// 001EFFF3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001EFFF6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFFD8, __imp__sub_001EFFD8)

X86_FUNC(__imp__sub_001EFFF7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001EFFF7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001EFFFC push 0xCA3
	{ uint32_t v = (uint32_t)(0xCA3u); X86_PUSH32(v); }
	// 001F0001 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0006 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F000C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F000D call 0x000127D0
	X86_PUSH32(0x1F0012u); sub_000127D0(ctx, base);
	// 001F0012 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0015 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001EFFF7, __imp__sub_001EFFF7)

X86_FUNC(__imp__sub_001F0016) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0016 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F001B push 0xCA6
	{ uint32_t v = (uint32_t)(0xCA6u); X86_PUSH32(v); }
	// 001F0020 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0025 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F002B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F002C call 0x000127D0
	X86_PUSH32(0x1F0031u); sub_000127D0(ctx, base);
	// 001F0031 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0034 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0016, __imp__sub_001F0016)

X86_FUNC(__imp__sub_001F0035) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0035 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F003A push 0xCA9
	{ uint32_t v = (uint32_t)(0xCA9u); X86_PUSH32(v); }
	// 001F003F push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0044 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F004A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F004B call 0x000127D0
	X86_PUSH32(0x1F0050u); sub_000127D0(ctx, base);
	// 001F0050 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0053 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0035, __imp__sub_001F0035)

X86_FUNC(__imp__sub_001F0054) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0054 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0059 push 0xCC3
	{ uint32_t v = (uint32_t)(0xCC3u); X86_PUSH32(v); }
	// 001F005E push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0063 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0069 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F006A call 0x000127D0
	X86_PUSH32(0x1F006Fu); sub_000127D0(ctx, base);
	// 001F006F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0072 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0054, __imp__sub_001F0054)

X86_FUNC(__imp__sub_001F0073) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0073 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0078 push 0xCD4
	{ uint32_t v = (uint32_t)(0xCD4u); X86_PUSH32(v); }
	// 001F007D push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0082 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0088 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0089 call 0x000127D0
	X86_PUSH32(0x1F008Eu); sub_000127D0(ctx, base);
	// 001F008E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0091 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0073, __imp__sub_001F0073)

X86_FUNC(__imp__sub_001F0092) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0092 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0097 push 0xCF2
	{ uint32_t v = (uint32_t)(0xCF2u); X86_PUSH32(v); }
	// 001F009C push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F00A1 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F00A7 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F00A8 call 0x000127D0
	X86_PUSH32(0x1F00ADu); sub_000127D0(ctx, base);
	// 001F00AD add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F00B0 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0092, __imp__sub_001F0092)

X86_FUNC(__imp__sub_001F00B1) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F00B1 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F00B6 push 0xD3A
	{ uint32_t v = (uint32_t)(0xD3Au); X86_PUSH32(v); }
	// 001F00BB push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F00C0 mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F00C6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F00C7 call 0x000127D0
	X86_PUSH32(0x1F00CCu); sub_000127D0(ctx, base);
	// 001F00CC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F00CF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F00B1, __imp__sub_001F00B1)

X86_FUNC(__imp__sub_001F00D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F00D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F00D5 push 0xD3F
	{ uint32_t v = (uint32_t)(0xD3Fu); X86_PUSH32(v); }
	// 001F00DA push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F00DF mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F00E5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F00E6 call 0x000127D0
	X86_PUSH32(0x1F00EBu); sub_000127D0(ctx, base);
	// 001F00EB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F00EE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F00D0, __imp__sub_001F00D0)

X86_FUNC(__imp__sub_001F00EF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F00EF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F00F4 push 0xD6A
	{ uint32_t v = (uint32_t)(0xD6Au); X86_PUSH32(v); }
	// 001F00F9 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F00FE mov eax, [ebp-0x69C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF964u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0104 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0105 call 0x000127D0
	X86_PUSH32(0x1F010Au); sub_000127D0(ctx, base);
	// 001F010A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F010D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F00EF, __imp__sub_001F00EF)

X86_FUNC(__imp__sub_001F010E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F010E mov eax, 0x27FC7C
	{ ctx.eax = (uint32_t)(0x27FC7Cu); }
	// 001F0113 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F010E, __imp__sub_001F010E)

X86_FUNC(__imp__sub_001F0120) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0120 lea ecx, [ebp-0x620]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFF9E0u));
	// 001F0126 jmp 0x001386B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001386B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0120, __imp__sub_001F0120)

X86_FUNC(__imp__sub_001F012B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F012B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0130 push 0xDB0
	{ uint32_t v = (uint32_t)(0xDB0u); X86_PUSH32(v); }
	// 001F0135 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F013A mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0140 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0141 call 0x000127D0
	X86_PUSH32(0x1F0146u); sub_000127D0(ctx, base);
	// 001F0146 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0149 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F012B, __imp__sub_001F012B)

X86_FUNC(__imp__sub_001F014A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F014A push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F014F push 0xE01
	{ uint32_t v = (uint32_t)(0xE01u); X86_PUSH32(v); }
	// 001F0154 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0159 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F015F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0160 call 0x000127D0
	X86_PUSH32(0x1F0165u); sub_000127D0(ctx, base);
	// 001F0165 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0168 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F014A, __imp__sub_001F014A)

X86_FUNC(__imp__sub_001F0169) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0169 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F016E push 0xE2C
	{ uint32_t v = (uint32_t)(0xE2Cu); X86_PUSH32(v); }
	// 001F0173 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0178 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F017E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F017F call 0x000127D0
	X86_PUSH32(0x1F0184u); sub_000127D0(ctx, base);
	// 001F0184 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0187 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0169, __imp__sub_001F0169)

X86_FUNC(__imp__sub_001F0188) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0188 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F018D push 0xE3E
	{ uint32_t v = (uint32_t)(0xE3Eu); X86_PUSH32(v); }
	// 001F0192 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0197 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F019D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F019E call 0x000127D0
	X86_PUSH32(0x1F01A3u); sub_000127D0(ctx, base);
	// 001F01A3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F01A6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0188, __imp__sub_001F0188)

X86_FUNC(__imp__sub_001F01A7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F01A7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F01AC push 0xE43
	{ uint32_t v = (uint32_t)(0xE43u); X86_PUSH32(v); }
	// 001F01B1 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F01B6 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F01BC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F01BD call 0x000127D0
	X86_PUSH32(0x1F01C2u); sub_000127D0(ctx, base);
	// 001F01C2 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F01C5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F01A7, __imp__sub_001F01A7)

X86_FUNC(__imp__sub_001F01C6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F01C6 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F01CB push 0xE49
	{ uint32_t v = (uint32_t)(0xE49u); X86_PUSH32(v); }
	// 001F01D0 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F01D5 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F01DB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F01DC call 0x000127D0
	X86_PUSH32(0x1F01E1u); sub_000127D0(ctx, base);
	// 001F01E1 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F01E4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F01C6, __imp__sub_001F01C6)

X86_FUNC(__imp__sub_001F01E5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F01E5 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F01EA push 0xE5A
	{ uint32_t v = (uint32_t)(0xE5Au); X86_PUSH32(v); }
	// 001F01EF push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F01F4 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F01FA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F01FB call 0x000127D0
	X86_PUSH32(0x1F0200u); sub_000127D0(ctx, base);
	// 001F0200 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0203 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F01E5, __imp__sub_001F01E5)

X86_FUNC(__imp__sub_001F0204) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0204 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0209 push 0xE79
	{ uint32_t v = (uint32_t)(0xE79u); X86_PUSH32(v); }
	// 001F020E push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0213 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0219 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F021A call 0x000127D0
	X86_PUSH32(0x1F021Fu); sub_000127D0(ctx, base);
	// 001F021F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0222 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0204, __imp__sub_001F0204)

X86_FUNC(__imp__sub_001F0223) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0223 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0228 push 0xE7F
	{ uint32_t v = (uint32_t)(0xE7Fu); X86_PUSH32(v); }
	// 001F022D push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0232 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0238 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0239 call 0x000127D0
	X86_PUSH32(0x1F023Eu); sub_000127D0(ctx, base);
	// 001F023E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0241 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0223, __imp__sub_001F0223)

X86_FUNC(__imp__sub_001F0242) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0242 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0247 push 0xE84
	{ uint32_t v = (uint32_t)(0xE84u); X86_PUSH32(v); }
	// 001F024C push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0251 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0257 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0258 call 0x000127D0
	X86_PUSH32(0x1F025Du); sub_000127D0(ctx, base);
	// 001F025D add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0260 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0242, __imp__sub_001F0242)

X86_FUNC(__imp__sub_001F0261) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0261 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0266 push 0xEAA
	{ uint32_t v = (uint32_t)(0xEAAu); X86_PUSH32(v); }
	// 001F026B push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0270 mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0276 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0277 call 0x000127D0
	X86_PUSH32(0x1F027Cu); sub_000127D0(ctx, base);
	// 001F027C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F027F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0261, __imp__sub_001F0261)

X86_FUNC(__imp__sub_001F0280) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0280 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0285 push 0xEEB
	{ uint32_t v = (uint32_t)(0xEEBu); X86_PUSH32(v); }
	// 001F028A push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F028F mov eax, [ebp-0x638]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0295 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0296 call 0x000127D0
	X86_PUSH32(0x1F029Bu); sub_000127D0(ctx, base);
	// 001F029B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F029E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0280, __imp__sub_001F0280)

X86_FUNC(__imp__sub_001F029F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F029F mov eax, 0x27FD00
	{ ctx.eax = (uint32_t)(0x27FD00u); }
	// 001F02A4 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F029F, __imp__sub_001F029F)

X86_FUNC(__imp__sub_001F02B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F02B0 lea ecx, [ebp-0x620]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFF9E0u));
	// 001F02B6 jmp 0x001386B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001386B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F02B0, __imp__sub_001F02B0)

X86_FUNC(__imp__sub_001F02BB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F02BB push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F02C0 push 0xF34
	{ uint32_t v = (uint32_t)(0xF34u); X86_PUSH32(v); }
	// 001F02C5 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F02CA mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F02D0 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F02D1 call 0x000127D0
	X86_PUSH32(0x1F02D6u); sub_000127D0(ctx, base);
	// 001F02D6 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F02D9 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F02BB, __imp__sub_001F02BB)

X86_FUNC(__imp__sub_001F02DA) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F02DA push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F02DF push 0xF82
	{ uint32_t v = (uint32_t)(0xF82u); X86_PUSH32(v); }
	// 001F02E4 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F02E9 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F02EF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F02F0 call 0x000127D0
	X86_PUSH32(0x1F02F5u); sub_000127D0(ctx, base);
	// 001F02F5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F02F8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F02DA, __imp__sub_001F02DA)

X86_FUNC(__imp__sub_001F02F9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F02F9 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F02FE push 0xFB2
	{ uint32_t v = (uint32_t)(0xFB2u); X86_PUSH32(v); }
	// 001F0303 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0308 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F030E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F030F call 0x000127D0
	X86_PUSH32(0x1F0314u); sub_000127D0(ctx, base);
	// 001F0314 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0317 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F02F9, __imp__sub_001F02F9)

X86_FUNC(__imp__sub_001F0318) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0318 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F031D push 0xFC4
	{ uint32_t v = (uint32_t)(0xFC4u); X86_PUSH32(v); }
	// 001F0322 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0327 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F032D push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F032E call 0x000127D0
	X86_PUSH32(0x1F0333u); sub_000127D0(ctx, base);
	// 001F0333 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0336 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0318, __imp__sub_001F0318)

X86_FUNC(__imp__sub_001F0337) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0337 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F033C push 0xFD6
	{ uint32_t v = (uint32_t)(0xFD6u); X86_PUSH32(v); }
	// 001F0341 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0346 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F034C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F034D call 0x000127D0
	X86_PUSH32(0x1F0352u); sub_000127D0(ctx, base);
	// 001F0352 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0355 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0337, __imp__sub_001F0337)

X86_FUNC(__imp__sub_001F0356) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0356 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F035B push 0x1005
	{ uint32_t v = (uint32_t)(0x1005u); X86_PUSH32(v); }
	// 001F0360 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0365 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F036B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F036C call 0x000127D0
	X86_PUSH32(0x1F0371u); sub_000127D0(ctx, base);
	// 001F0371 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0374 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0356, __imp__sub_001F0356)

X86_FUNC(__imp__sub_001F0375) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0375 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F037A push 0x100B
	{ uint32_t v = (uint32_t)(0x100Bu); X86_PUSH32(v); }
	// 001F037F push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0384 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F038A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F038B call 0x000127D0
	X86_PUSH32(0x1F0390u); sub_000127D0(ctx, base);
	// 001F0390 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0393 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0375, __imp__sub_001F0375)

X86_FUNC(__imp__sub_001F0394) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0394 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0399 push 0x1019
	{ uint32_t v = (uint32_t)(0x1019u); X86_PUSH32(v); }
	// 001F039E push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F03A3 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F03A9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F03AA call 0x000127D0
	X86_PUSH32(0x1F03AFu); sub_000127D0(ctx, base);
	// 001F03AF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F03B2 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0394, __imp__sub_001F0394)

X86_FUNC(__imp__sub_001F03B3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F03B3 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F03B8 push 0x102E
	{ uint32_t v = (uint32_t)(0x102Eu); X86_PUSH32(v); }
	// 001F03BD push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F03C2 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F03C8 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F03C9 call 0x000127D0
	X86_PUSH32(0x1F03CEu); sub_000127D0(ctx, base);
	// 001F03CE add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F03D1 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F03B3, __imp__sub_001F03B3)

X86_FUNC(__imp__sub_001F03D2) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F03D2 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F03D7 push 0x1076
	{ uint32_t v = (uint32_t)(0x1076u); X86_PUSH32(v); }
	// 001F03DC push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F03E1 mov eax, [ebp-0x634]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFF9CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F03E7 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F03E8 call 0x000127D0
	X86_PUSH32(0x1F03EDu); sub_000127D0(ctx, base);
	// 001F03ED add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F03F0 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F03D2, __imp__sub_001F03D2)

X86_FUNC(__imp__sub_001F03F1) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F03F1 mov eax, 0x27FD74
	{ ctx.eax = (uint32_t)(0x27FD74u); }
	// 001F03F6 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F03F1, __imp__sub_001F03F1)

X86_FUNC(__imp__sub_001F0400) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0400 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0405 push 0x175
	{ uint32_t v = (uint32_t)(0x175u); X86_PUSH32(v); }
	// 001F040A push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F040F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0412 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0413 call 0x000127D0
	X86_PUSH32(0x1F0418u); sub_000127D0(ctx, base);
	// 001F0418 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F041B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0400, __imp__sub_001F0400)

X86_FUNC(__imp__sub_001F041C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F041C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0421 push 0x18E
	{ uint32_t v = (uint32_t)(0x18Eu); X86_PUSH32(v); }
	// 001F0426 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F042B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F042E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F042F call 0x000127D0
	X86_PUSH32(0x1F0434u); sub_000127D0(ctx, base);
	// 001F0434 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0437 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F041C, __imp__sub_001F041C)

X86_FUNC(__imp__sub_001F0438) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0438 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F043D push 0x1A4
	{ uint32_t v = (uint32_t)(0x1A4u); X86_PUSH32(v); }
	// 001F0442 push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0447 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F044A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F044B call 0x000127D0
	X86_PUSH32(0x1F0450u); sub_000127D0(ctx, base);
	// 001F0450 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0453 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0438, __imp__sub_001F0438)

X86_FUNC(__imp__sub_001F0454) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0454 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0459 push 0x1C5
	{ uint32_t v = (uint32_t)(0x1C5u); X86_PUSH32(v); }
	// 001F045E push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F0463 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0466 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0467 call 0x000127D0
	X86_PUSH32(0x1F046Cu); sub_000127D0(ctx, base);
	// 001F046C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F046F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0454, __imp__sub_001F0454)

X86_FUNC(__imp__sub_001F0470) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0470 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0475 push 0x1DE
	{ uint32_t v = (uint32_t)(0x1DEu); X86_PUSH32(v); }
	// 001F047A push 0x26CE64
	{ uint32_t v = (uint32_t)(0x26CE64u); X86_PUSH32(v); }
	// 001F047F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0482 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0483 call 0x000127D0
	X86_PUSH32(0x1F0488u); sub_000127D0(ctx, base);
	// 001F0488 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F048B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0470, __imp__sub_001F0470)

X86_FUNC(__imp__sub_001F048C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F048C mov eax, 0x27FDB8
	{ ctx.eax = (uint32_t)(0x27FDB8u); }
	// 001F0491 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F048C, __imp__sub_001F048C)

X86_FUNC(__imp__sub_001F04A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F04A0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F04A3 jmp 0x000B02D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000B02D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F04A0, __imp__sub_001F04A0)

X86_FUNC(__imp__sub_001F04A8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F04A8 mov eax, 0x27FDDC
	{ ctx.eax = (uint32_t)(0x27FDDCu); }
	// 001F04AD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F04A8, __imp__sub_001F04A8)

X86_FUNC(__imp__sub_001F04C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F04C0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F04C5 push 0x7A
	{ uint32_t v = (uint32_t)(0x7Au); X86_PUSH32(v); }
	// 001F04C7 push 0x26D8A4
	{ uint32_t v = (uint32_t)(0x26D8A4u); X86_PUSH32(v); }
	// 001F04CC mov eax, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F04CF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F04D0 call 0x000127D0
	X86_PUSH32(0x1F04D5u); sub_000127D0(ctx, base);
	// 001F04D5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F04D8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F04C0, __imp__sub_001F04C0)

X86_FUNC(__imp__sub_001F04D9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F04D9 mov eax, 0x27FE00
	{ ctx.eax = (uint32_t)(0x27FE00u); }
	// 001F04DE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F04D9, __imp__sub_001F04D9)

X86_FUNC(__imp__sub_001F04F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F04F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F04F5 push 0x17C
	{ uint32_t v = (uint32_t)(0x17Cu); X86_PUSH32(v); }
	// 001F04FA push 0x26E094
	{ uint32_t v = (uint32_t)(0x26E094u); X86_PUSH32(v); }
	// 001F04FF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0502 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0503 call 0x000127D0
	X86_PUSH32(0x1F0508u); sub_000127D0(ctx, base);
	// 001F0508 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F050B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F04F0, __imp__sub_001F04F0)

X86_FUNC(__imp__sub_001F050C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F050C mov eax, 0x27FE24
	{ ctx.eax = (uint32_t)(0x27FE24u); }
	// 001F0511 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F050C, __imp__sub_001F050C)

X86_FUNC(__imp__sub_001F0520) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0520 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0525 push 0x59
	{ uint32_t v = (uint32_t)(0x59u); X86_PUSH32(v); }
	// 001F0527 push 0x26E1B8
	{ uint32_t v = (uint32_t)(0x26E1B8u); X86_PUSH32(v); }
	// 001F052C mov eax, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F052F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0530 call 0x000127D0
	X86_PUSH32(0x1F0535u); sub_000127D0(ctx, base);
	// 001F0535 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0538 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0520, __imp__sub_001F0520)

X86_FUNC(__imp__sub_001F0539) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0539 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F053E push 0x5E
	{ uint32_t v = (uint32_t)(0x5Eu); X86_PUSH32(v); }
	// 001F0540 push 0x26E1B8
	{ uint32_t v = (uint32_t)(0x26E1B8u); X86_PUSH32(v); }
	// 001F0545 mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0548 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0549 call 0x000127D0
	X86_PUSH32(0x1F054Eu); sub_000127D0(ctx, base);
	// 001F054E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0551 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0539, __imp__sub_001F0539)

X86_FUNC(__imp__sub_001F0552) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0552 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0557 push 0x68
	{ uint32_t v = (uint32_t)(0x68u); X86_PUSH32(v); }
	// 001F0559 push 0x26E1B8
	{ uint32_t v = (uint32_t)(0x26E1B8u); X86_PUSH32(v); }
	// 001F055E mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0561 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0562 call 0x000127D0
	X86_PUSH32(0x1F0567u); sub_000127D0(ctx, base);
	// 001F0567 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F056A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0552, __imp__sub_001F0552)

X86_FUNC(__imp__sub_001F056B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F056B mov eax, 0x27FE58
	{ ctx.eax = (uint32_t)(0x27FE58u); }
	// 001F0570 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F056B, __imp__sub_001F056B)

X86_FUNC(__imp__sub_001F0580) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0580 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0585 push 0x118
	{ uint32_t v = (uint32_t)(0x118u); X86_PUSH32(v); }
	// 001F058A push 0x26E348
	{ uint32_t v = (uint32_t)(0x26E348u); X86_PUSH32(v); }
	// 001F058F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0592 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0593 call 0x000127D0
	X86_PUSH32(0x1F0598u); sub_000127D0(ctx, base);
	// 001F0598 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F059B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0580, __imp__sub_001F0580)

X86_FUNC(__imp__sub_001F059C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F059C mov eax, 0x27FE7C
	{ ctx.eax = (uint32_t)(0x27FE7Cu); }
	// 001F05A1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F059C, __imp__sub_001F059C)

X86_FUNC(__imp__sub_001F05B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F05B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F05B5 push 0xDC
	{ uint32_t v = (uint32_t)(0xDCu); X86_PUSH32(v); }
	// 001F05BA push 0x26E4AC
	{ uint32_t v = (uint32_t)(0x26E4ACu); X86_PUSH32(v); }
	// 001F05BF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F05C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F05C3 call 0x000127D0
	X86_PUSH32(0x1F05C8u); sub_000127D0(ctx, base);
	// 001F05C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F05CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F05B0, __imp__sub_001F05B0)

X86_FUNC(__imp__sub_001F05CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F05CC mov eax, 0x27FEA0
	{ ctx.eax = (uint32_t)(0x27FEA0u); }
	// 001F05D1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F05CC, __imp__sub_001F05CC)

X86_FUNC(__imp__sub_001F05E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F05E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F05E5 push 0x20A
	{ uint32_t v = (uint32_t)(0x20Au); X86_PUSH32(v); }
	// 001F05EA push 0x26E514
	{ uint32_t v = (uint32_t)(0x26E514u); X86_PUSH32(v); }
	// 001F05EF mov eax, [ebp-0x30]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F05F2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F05F3 call 0x000127D0
	X86_PUSH32(0x1F05F8u); sub_000127D0(ctx, base);
	// 001F05F8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F05FB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F05E0, __imp__sub_001F05E0)

X86_FUNC(__imp__sub_001F05FC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F05FC mov eax, 0x27FEC4
	{ ctx.eax = (uint32_t)(0x27FEC4u); }
	// 001F0601 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F05FC, __imp__sub_001F05FC)

X86_FUNC(__imp__sub_001F0610) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0610 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0615 push 0x283
	{ uint32_t v = (uint32_t)(0x283u); X86_PUSH32(v); }
	// 001F061A push 0x26E514
	{ uint32_t v = (uint32_t)(0x26E514u); X86_PUSH32(v); }
	// 001F061F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0622 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0623 call 0x000127D0
	X86_PUSH32(0x1F0628u); sub_000127D0(ctx, base);
	// 001F0628 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F062B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0610, __imp__sub_001F0610)

X86_FUNC(__imp__sub_001F062C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F062C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0631 push 0x30D
	{ uint32_t v = (uint32_t)(0x30Du); X86_PUSH32(v); }
	// 001F0636 push 0x26E514
	{ uint32_t v = (uint32_t)(0x26E514u); X86_PUSH32(v); }
	// 001F063B mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F063E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F063F call 0x000127D0
	X86_PUSH32(0x1F0644u); sub_000127D0(ctx, base);
	// 001F0644 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0647 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F062C, __imp__sub_001F062C)

X86_FUNC(__imp__sub_001F0648) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0648 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F064D push 0x314
	{ uint32_t v = (uint32_t)(0x314u); X86_PUSH32(v); }
	// 001F0652 push 0x26E514
	{ uint32_t v = (uint32_t)(0x26E514u); X86_PUSH32(v); }
	// 001F0657 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F065A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F065B call 0x000127D0
	X86_PUSH32(0x1F0660u); sub_000127D0(ctx, base);
	// 001F0660 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0663 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0648, __imp__sub_001F0648)

X86_FUNC(__imp__sub_001F0664) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0664 mov eax, 0x27FEF8
	{ ctx.eax = (uint32_t)(0x27FEF8u); }
	// 001F0669 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0664, __imp__sub_001F0664)

X86_FUNC(__imp__sub_001F0670) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0670 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0673 jmp 0x000B02D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000B02D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0670, __imp__sub_001F0670)

X86_FUNC(__imp__sub_001F0678) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0678 mov eax, 0x27FF1C
	{ ctx.eax = (uint32_t)(0x27FF1Cu); }
	// 001F067D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0678, __imp__sub_001F0678)

X86_FUNC(__imp__sub_001F0690) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0690 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0695 push 0x1EA
	{ uint32_t v = (uint32_t)(0x1EAu); X86_PUSH32(v); }
	// 001F069A push 0x26EF58
	{ uint32_t v = (uint32_t)(0x26EF58u); X86_PUSH32(v); }
	// 001F069F mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F06A2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F06A3 call 0x000127D0
	X86_PUSH32(0x1F06A8u); sub_000127D0(ctx, base);
	// 001F06A8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F06AB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0690, __imp__sub_001F0690)

X86_FUNC(__imp__sub_001F06AC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F06AC mov eax, 0x27FF40
	{ ctx.eax = (uint32_t)(0x27FF40u); }
	// 001F06B1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F06AC, __imp__sub_001F06AC)

X86_FUNC(__imp__sub_001F06C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F06C0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F06C3 jmp 0x00012860
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00012860(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F06C0, __imp__sub_001F06C0)

X86_FUNC(__imp__sub_001F06C8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F06C8 mov eax, 0x27FF64
	{ ctx.eax = (uint32_t)(0x27FF64u); }
	// 001F06CD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F06C8, __imp__sub_001F06C8)

X86_FUNC(__imp__sub_001F06E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F06E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F06E5 push 0x132
	{ uint32_t v = (uint32_t)(0x132u); X86_PUSH32(v); }
	// 001F06EA push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F06EF mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F06F2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F06F3 call 0x000127D0
	X86_PUSH32(0x1F06F8u); sub_000127D0(ctx, base);
	// 001F06F8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F06FB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F06E0, __imp__sub_001F06E0)

X86_FUNC(__imp__sub_001F06FC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F06FC mov eax, 0x27FF88
	{ ctx.eax = (uint32_t)(0x27FF88u); }
	// 001F0701 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F06FC, __imp__sub_001F06FC)

X86_FUNC(__imp__sub_001F0710) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0710 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0715 push 0x1AE
	{ uint32_t v = (uint32_t)(0x1AEu); X86_PUSH32(v); }
	// 001F071A push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F071F mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0725 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0726 call 0x000127D0
	X86_PUSH32(0x1F072Bu); sub_000127D0(ctx, base);
	// 001F072B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F072E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0710, __imp__sub_001F0710)

X86_FUNC(__imp__sub_001F072F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F072F push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0734 push 0x1D5
	{ uint32_t v = (uint32_t)(0x1D5u); X86_PUSH32(v); }
	// 001F0739 push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F073E mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0744 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0745 call 0x000127D0
	X86_PUSH32(0x1F074Au); sub_000127D0(ctx, base);
	// 001F074A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F074D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F072F, __imp__sub_001F072F)

X86_FUNC(__imp__sub_001F074E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F074E push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0753 push 0x1E3
	{ uint32_t v = (uint32_t)(0x1E3u); X86_PUSH32(v); }
	// 001F0758 push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F075D mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0763 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0764 call 0x000127D0
	X86_PUSH32(0x1F0769u); sub_000127D0(ctx, base);
	// 001F0769 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F076C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F074E, __imp__sub_001F074E)

X86_FUNC(__imp__sub_001F076D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F076D push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0772 push 0x218
	{ uint32_t v = (uint32_t)(0x218u); X86_PUSH32(v); }
	// 001F0777 push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F077C mov eax, [ebp-0xB4]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF4Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0782 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0783 call 0x000127D0
	X86_PUSH32(0x1F0788u); sub_000127D0(ctx, base);
	// 001F0788 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F078B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F076D, __imp__sub_001F076D)

X86_FUNC(__imp__sub_001F078C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F078C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0791 push 0x21E
	{ uint32_t v = (uint32_t)(0x21Eu); X86_PUSH32(v); }
	// 001F0796 push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F079B mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F07A1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F07A2 call 0x000127D0
	X86_PUSH32(0x1F07A7u); sub_000127D0(ctx, base);
	// 001F07A7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F07AA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F078C, __imp__sub_001F078C)

X86_FUNC(__imp__sub_001F07AB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F07AB mov eax, 0x27FFCC
	{ ctx.eax = (uint32_t)(0x27FFCCu); }
	// 001F07B0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F07AB, __imp__sub_001F07AB)

X86_FUNC(__imp__sub_001F07C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F07C0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F07C5 push 0x8FC
	{ uint32_t v = (uint32_t)(0x8FCu); X86_PUSH32(v); }
	// 001F07CA push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F07CF mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F07D2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F07D3 call 0x000127D0
	X86_PUSH32(0x1F07D8u); sub_000127D0(ctx, base);
	// 001F07D8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F07DB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F07C0, __imp__sub_001F07C0)

X86_FUNC(__imp__sub_001F07DC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F07DC mov eax, 0x27FFF0
	{ ctx.eax = (uint32_t)(0x27FFF0u); }
	// 001F07E1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F07DC, __imp__sub_001F07DC)

X86_FUNC(__imp__sub_001F07F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F07F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F07F5 push 0x254
	{ uint32_t v = (uint32_t)(0x254u); X86_PUSH32(v); }
	// 001F07FA push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F07FF mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0802 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0803 call 0x000127D0
	X86_PUSH32(0x1F0808u); sub_000127D0(ctx, base);
	// 001F0808 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F080B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F07F0, __imp__sub_001F07F0)

X86_FUNC(__imp__sub_001F080C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F080C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0811 push 0x29D
	{ uint32_t v = (uint32_t)(0x29Du); X86_PUSH32(v); }
	// 001F0816 push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F081B mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F081E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F081F call 0x000127D0
	X86_PUSH32(0x1F0824u); sub_000127D0(ctx, base);
	// 001F0824 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0827 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F080C, __imp__sub_001F080C)

X86_FUNC(__imp__sub_001F0828) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0828 mov eax, 0x28001C
	{ ctx.eax = (uint32_t)(0x28001Cu); }
	// 001F082D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0828, __imp__sub_001F0828)

X86_FUNC(__imp__sub_001F0840) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0840 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0845 push 0x3C4
	{ uint32_t v = (uint32_t)(0x3C4u); X86_PUSH32(v); }
	// 001F084A push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F084F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0852 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0853 call 0x000127D0
	X86_PUSH32(0x1F0858u); sub_000127D0(ctx, base);
	// 001F0858 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F085B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0840, __imp__sub_001F0840)

X86_FUNC(__imp__sub_001F085C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F085C mov eax, 0x280040
	{ ctx.eax = (uint32_t)(0x280040u); }
	// 001F0861 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F085C, __imp__sub_001F085C)

X86_FUNC(__imp__sub_001F0870) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0870 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0875 push 0x700
	{ uint32_t v = (uint32_t)(0x700u); X86_PUSH32(v); }
	// 001F087A push 0x26F184
	{ uint32_t v = (uint32_t)(0x26F184u); X86_PUSH32(v); }
	// 001F087F mov eax, [ebp-0x50]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFB0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0882 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0883 call 0x000127D0
	X86_PUSH32(0x1F0888u); sub_000127D0(ctx, base);
	// 001F0888 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F088B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0870, __imp__sub_001F0870)

X86_FUNC(__imp__sub_001F088C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F088C mov eax, 0x280064
	{ ctx.eax = (uint32_t)(0x280064u); }
	// 001F0891 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F088C, __imp__sub_001F088C)

X86_FUNC(__imp__sub_001F08A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F08A0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F08A5 push 0x57
	{ uint32_t v = (uint32_t)(0x57u); X86_PUSH32(v); }
	// 001F08A7 push 0x26F694
	{ uint32_t v = (uint32_t)(0x26F694u); X86_PUSH32(v); }
	// 001F08AC mov eax, [ebp-0x90]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF70u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F08B2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F08B3 call 0x000127D0
	X86_PUSH32(0x1F08B8u); sub_000127D0(ctx, base);
	// 001F08B8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F08BB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F08A0, __imp__sub_001F08A0)

X86_FUNC(__imp__sub_001F08BC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F08BC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F08C1 push 0x97
	{ uint32_t v = (uint32_t)(0x97u); X86_PUSH32(v); }
	// 001F08C6 push 0x26F694
	{ uint32_t v = (uint32_t)(0x26F694u); X86_PUSH32(v); }
	// 001F08CB mov eax, [ebp-0x90]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF70u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F08D1 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F08D2 call 0x000127D0
	X86_PUSH32(0x1F08D7u); sub_000127D0(ctx, base);
	// 001F08D7 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F08DA ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F08BC, __imp__sub_001F08BC)

X86_FUNC(__imp__sub_001F08DB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F08DB mov eax, 0x280090
	{ ctx.eax = (uint32_t)(0x280090u); }
	// 001F08E0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F08DB, __imp__sub_001F08DB)

X86_FUNC(__imp__sub_001F08F0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F08F0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F08F5 push 0x595
	{ uint32_t v = (uint32_t)(0x595u); X86_PUSH32(v); }
	// 001F08FA push 0x26F730
	{ uint32_t v = (uint32_t)(0x26F730u); X86_PUSH32(v); }
	// 001F08FF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0902 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0903 call 0x000127D0
	X86_PUSH32(0x1F0908u); sub_000127D0(ctx, base);
	// 001F0908 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F090B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F08F0, __imp__sub_001F08F0)

X86_FUNC(__imp__sub_001F090C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F090C mov eax, 0x2800B4
	{ ctx.eax = (uint32_t)(0x2800B4u); }
	// 001F0911 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F090C, __imp__sub_001F090C)

X86_FUNC(__imp__sub_001F0920) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0920 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0923 add ecx, 0x0C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0926 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0920, __imp__sub_001F0920)

X86_FUNC(__imp__sub_001F092B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F092B mov eax, 0x2800D8
	{ ctx.eax = (uint32_t)(0x2800D8u); }
	// 001F0930 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F092B, __imp__sub_001F092B)

X86_FUNC(__imp__sub_001F0940) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0940 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0945 push 0x47C
	{ uint32_t v = (uint32_t)(0x47Cu); X86_PUSH32(v); }
	// 001F094A push 0x26F730
	{ uint32_t v = (uint32_t)(0x26F730u); X86_PUSH32(v); }
	// 001F094F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0952 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0953 call 0x000127D0
	X86_PUSH32(0x1F0958u); sub_000127D0(ctx, base);
	// 001F0958 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F095B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0940, __imp__sub_001F0940)

X86_FUNC(__imp__sub_001F095C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F095C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0961 push 0x4AF
	{ uint32_t v = (uint32_t)(0x4AFu); X86_PUSH32(v); }
	// 001F0966 push 0x26F730
	{ uint32_t v = (uint32_t)(0x26F730u); X86_PUSH32(v); }
	// 001F096B mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F096E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F096F call 0x000127D0
	X86_PUSH32(0x1F0974u); sub_000127D0(ctx, base);
	// 001F0974 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0977 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F095C, __imp__sub_001F095C)

X86_FUNC(__imp__sub_001F0978) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0978 mov eax, 0x280104
	{ ctx.eax = (uint32_t)(0x280104u); }
	// 001F097D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0978, __imp__sub_001F0978)

X86_FUNC(__imp__sub_001F0990) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0990 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0993 jmp 0x000B02D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000B02D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0990, __imp__sub_001F0990)

X86_FUNC(__imp__sub_001F0998) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0998 mov eax, 0x280128
	{ ctx.eax = (uint32_t)(0x280128u); }
	// 001F099D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0998, __imp__sub_001F0998)

X86_FUNC(__imp__sub_001F09B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F09B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F09B5 push 0x5D7
	{ uint32_t v = (uint32_t)(0x5D7u); X86_PUSH32(v); }
	// 001F09BA push 0x26F92C
	{ uint32_t v = (uint32_t)(0x26F92Cu); X86_PUSH32(v); }
	// 001F09BF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F09C2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F09C3 call 0x000127D0
	X86_PUSH32(0x1F09C8u); sub_000127D0(ctx, base);
	// 001F09C8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F09CB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F09B0, __imp__sub_001F09B0)

X86_FUNC(__imp__sub_001F09CC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F09CC mov eax, 0x28014C
	{ ctx.eax = (uint32_t)(0x28014Cu); }
	// 001F09D1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F09CC, __imp__sub_001F09CC)

X86_FUNC(__imp__sub_001F09E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F09E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F09E5 push 0x2CC
	{ uint32_t v = (uint32_t)(0x2CCu); X86_PUSH32(v); }
	// 001F09EA push 0x26FAF0
	{ uint32_t v = (uint32_t)(0x26FAF0u); X86_PUSH32(v); }
	// 001F09EF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F09F2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F09F3 call 0x000127D0
	X86_PUSH32(0x1F09F8u); sub_000127D0(ctx, base);
	// 001F09F8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F09FB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F09E0, __imp__sub_001F09E0)

X86_FUNC(__imp__sub_001F09FC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F09FC mov eax, 0x280170
	{ ctx.eax = (uint32_t)(0x280170u); }
	// 001F0A01 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F09FC, __imp__sub_001F09FC)

X86_FUNC(__imp__sub_001F0A10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A10 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0A15 push 0x74E
	{ uint32_t v = (uint32_t)(0x74Eu); X86_PUSH32(v); }
	// 001F0A1A push 0x26FAF0
	{ uint32_t v = (uint32_t)(0x26FAF0u); X86_PUSH32(v); }
	// 001F0A1F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0A22 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0A23 call 0x000127D0
	X86_PUSH32(0x1F0A28u); sub_000127D0(ctx, base);
	// 001F0A28 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0A2B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0A10, __imp__sub_001F0A10)

X86_FUNC(__imp__sub_001F0A2C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A2C mov eax, 0x280194
	{ ctx.eax = (uint32_t)(0x280194u); }
	// 001F0A31 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0A2C, __imp__sub_001F0A2C)

X86_FUNC(__imp__sub_001F0A40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A40 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0A45 push 0x1FF
	{ uint32_t v = (uint32_t)(0x1FFu); X86_PUSH32(v); }
	// 001F0A4A push 0x26FAF0
	{ uint32_t v = (uint32_t)(0x26FAF0u); X86_PUSH32(v); }
	// 001F0A4F mov eax, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0A52 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0A53 call 0x000127D0
	X86_PUSH32(0x1F0A58u); sub_000127D0(ctx, base);
	// 001F0A58 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0A5B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0A40, __imp__sub_001F0A40)

X86_FUNC(__imp__sub_001F0A5C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A5C mov eax, 0x2801B8
	{ ctx.eax = (uint32_t)(0x2801B8u); }
	// 001F0A61 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0A5C, __imp__sub_001F0A5C)

X86_FUNC(__imp__sub_001F0A70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A70 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0A73 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0A70, __imp__sub_001F0A70)

X86_FUNC(__imp__sub_001F0A78) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A78 mov eax, 0x2801DC
	{ ctx.eax = (uint32_t)(0x2801DCu); }
	// 001F0A7D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0A78, __imp__sub_001F0A78)

X86_FUNC(__imp__sub_001F0A90) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0A90 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0A95 push 0x56
	{ uint32_t v = (uint32_t)(0x56u); X86_PUSH32(v); }
	// 001F0A97 push 0x26FCDC
	{ uint32_t v = (uint32_t)(0x26FCDCu); X86_PUSH32(v); }
	// 001F0A9C mov eax, [ebp-0x50]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFB0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0A9F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0AA0 call 0x000127D0
	X86_PUSH32(0x1F0AA5u); sub_000127D0(ctx, base);
	// 001F0AA5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0AA8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0A90, __imp__sub_001F0A90)

X86_FUNC(__imp__sub_001F0AA9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0AA9 mov eax, 0x280200
	{ ctx.eax = (uint32_t)(0x280200u); }
	// 001F0AAE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0AA9, __imp__sub_001F0AA9)

X86_FUNC(__imp__sub_001F0AC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0AC0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0AC5 push 0x29F
	{ uint32_t v = (uint32_t)(0x29Fu); X86_PUSH32(v); }
	// 001F0ACA push 0x26FD7C
	{ uint32_t v = (uint32_t)(0x26FD7Cu); X86_PUSH32(v); }
	// 001F0ACF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0AD2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0AD3 call 0x000127D0
	X86_PUSH32(0x1F0AD8u); sub_000127D0(ctx, base);
	// 001F0AD8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0ADB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0AC0, __imp__sub_001F0AC0)

X86_FUNC(__imp__sub_001F0ADC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0ADC mov eax, 0x280224
	{ ctx.eax = (uint32_t)(0x280224u); }
	// 001F0AE1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0ADC, __imp__sub_001F0ADC)

X86_FUNC(__imp__sub_001F0AF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0AF0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0AF3 add ecx, 0x24
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0AF6 jmp 0x000A8BA0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A8BA0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0AF0, __imp__sub_001F0AF0)

X86_FUNC(__imp__sub_001F0AFB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0AFB mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0AFE add ecx, 0x98
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x98u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0B04 jmp 0x000A8E00
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A8E00(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0AFB, __imp__sub_001F0AFB)

X86_FUNC(__imp__sub_001F0B09) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B09 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F0B0C add ecx, 0xF8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xF8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0B12 jmp 0x000A8FB0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000A8FB0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0B09, __imp__sub_001F0B09)

X86_FUNC(__imp__sub_001F0B17) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B17 mov eax, 0x280258
	{ ctx.eax = (uint32_t)(0x280258u); }
	// 001F0B1C jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0B17, __imp__sub_001F0B17)

X86_FUNC(__imp__sub_001F0B30) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B30 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0B35 push 0x1C7
	{ uint32_t v = (uint32_t)(0x1C7u); X86_PUSH32(v); }
	// 001F0B3A push 0x26FEA0
	{ uint32_t v = (uint32_t)(0x26FEA0u); X86_PUSH32(v); }
	// 001F0B3F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0B42 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0B43 call 0x000127D0
	X86_PUSH32(0x1F0B48u); sub_000127D0(ctx, base);
	// 001F0B48 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0B4B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0B30, __imp__sub_001F0B30)

X86_FUNC(__imp__sub_001F0B4C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B4C mov eax, 0x2802B0
	{ ctx.eax = (uint32_t)(0x2802B0u); }
	// 001F0B51 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0B4C, __imp__sub_001F0B4C)

X86_FUNC(__imp__sub_001F0B60) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B60 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0B65 push 0x14A
	{ uint32_t v = (uint32_t)(0x14Au); X86_PUSH32(v); }
	// 001F0B6A push 0x26FF00
	{ uint32_t v = (uint32_t)(0x26FF00u); X86_PUSH32(v); }
	// 001F0B6F mov eax, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0B72 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0B73 call 0x000127D0
	X86_PUSH32(0x1F0B78u); sub_000127D0(ctx, base);
	// 001F0B78 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0B7B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0B60, __imp__sub_001F0B60)

X86_FUNC(__imp__sub_001F0B7C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B7C mov eax, 0x2802D4
	{ ctx.eax = (uint32_t)(0x2802D4u); }
	// 001F0B81 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0B7C, __imp__sub_001F0B7C)

X86_FUNC(__imp__sub_001F0B90) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0B90 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0B95 push 0x16A
	{ uint32_t v = (uint32_t)(0x16Au); X86_PUSH32(v); }
	// 001F0B9A push 0x26FF40
	{ uint32_t v = (uint32_t)(0x26FF40u); X86_PUSH32(v); }
	// 001F0B9F mov eax, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0BA2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0BA3 call 0x000127D0
	X86_PUSH32(0x1F0BA8u); sub_000127D0(ctx, base);
	// 001F0BA8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0BAB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0B90, __imp__sub_001F0B90)

X86_FUNC(__imp__sub_001F0BAC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0BAC mov eax, 0x2802F8
	{ ctx.eax = (uint32_t)(0x2802F8u); }
	// 001F0BB1 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0BAC, __imp__sub_001F0BAC)

X86_FUNC(__imp__sub_001F0BC0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0BC0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0BC5 push 0x8B
	{ uint32_t v = (uint32_t)(0x8Bu); X86_PUSH32(v); }
	// 001F0BCA push 0x26FFCC
	{ uint32_t v = (uint32_t)(0x26FFCCu); X86_PUSH32(v); }
	// 001F0BCF mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0BD2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0BD3 call 0x000127D0
	X86_PUSH32(0x1F0BD8u); sub_000127D0(ctx, base);
	// 001F0BD8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0BDB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0BC0, __imp__sub_001F0BC0)

X86_FUNC(__imp__sub_001F0BDC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0BDC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0BE1 push 0xB7
	{ uint32_t v = (uint32_t)(0xB7u); X86_PUSH32(v); }
	// 001F0BE6 push 0x26FFCC
	{ uint32_t v = (uint32_t)(0x26FFCCu); X86_PUSH32(v); }
	// 001F0BEB mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0BEE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0BEF call 0x000127D0
	X86_PUSH32(0x1F0BF4u); sub_000127D0(ctx, base);
	// 001F0BF4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0BF7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0BDC, __imp__sub_001F0BDC)

X86_FUNC(__imp__sub_001F0BF8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0BF8 mov eax, 0x280324
	{ ctx.eax = (uint32_t)(0x280324u); }
	// 001F0BFD jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0BF8, __imp__sub_001F0BF8)

X86_FUNC(__imp__sub_001F0C10) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0C10 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0C15 push 0x476
	{ uint32_t v = (uint32_t)(0x476u); X86_PUSH32(v); }
	// 001F0C1A push 0x26FFCC
	{ uint32_t v = (uint32_t)(0x26FFCCu); X86_PUSH32(v); }
	// 001F0C1F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0C22 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0C23 call 0x000127D0
	X86_PUSH32(0x1F0C28u); sub_000127D0(ctx, base);
	// 001F0C28 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0C2B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0C10, __imp__sub_001F0C10)

X86_FUNC(__imp__sub_001F0C2C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0C2C mov eax, 0x280348
	{ ctx.eax = (uint32_t)(0x280348u); }
	// 001F0C31 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0C2C, __imp__sub_001F0C2C)

X86_FUNC(__imp__sub_001F0C40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0C40 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0C45 push 0x363
	{ uint32_t v = (uint32_t)(0x363u); X86_PUSH32(v); }
	// 001F0C4A push 0x26FFCC
	{ uint32_t v = (uint32_t)(0x26FFCCu); X86_PUSH32(v); }
	// 001F0C4F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0C52 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0C53 call 0x000127D0
	X86_PUSH32(0x1F0C58u); sub_000127D0(ctx, base);
	// 001F0C58 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0C5B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0C40, __imp__sub_001F0C40)

X86_FUNC(__imp__sub_001F0C5C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0C5C mov eax, 0x28036C
	{ ctx.eax = (uint32_t)(0x28036Cu); }
	// 001F0C61 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0C5C, __imp__sub_001F0C5C)

X86_FUNC(__imp__sub_001F0C70) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0C70 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0C75 push 0x10D
	{ uint32_t v = (uint32_t)(0x10Du); X86_PUSH32(v); }
	// 001F0C7A push 0x270114
	{ uint32_t v = (uint32_t)(0x270114u); X86_PUSH32(v); }
	// 001F0C7F mov eax, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0C82 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0C83 call 0x000127D0
	X86_PUSH32(0x1F0C88u); sub_000127D0(ctx, base);
	// 001F0C88 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0C8B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0C70, __imp__sub_001F0C70)

X86_FUNC(__imp__sub_001F0C8C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0C8C mov eax, 0x280390
	{ ctx.eax = (uint32_t)(0x280390u); }
	// 001F0C91 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0C8C, __imp__sub_001F0C8C)

X86_FUNC(__imp__sub_001F0CA0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0CA0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0CA5 push 0x4E
	{ uint32_t v = (uint32_t)(0x4Eu); X86_PUSH32(v); }
	// 001F0CA7 push 0x270164
	{ uint32_t v = (uint32_t)(0x270164u); X86_PUSH32(v); }
	// 001F0CAC mov eax, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0CAF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0CB0 call 0x000127D0
	X86_PUSH32(0x1F0CB5u); sub_000127D0(ctx, base);
	// 001F0CB5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0CB8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0CA0, __imp__sub_001F0CA0)

X86_FUNC(__imp__sub_001F0CB9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0CB9 mov eax, 0x2803B4
	{ ctx.eax = (uint32_t)(0x2803B4u); }
	// 001F0CBE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0CB9, __imp__sub_001F0CB9)

X86_FUNC(__imp__sub_001F0CD0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0CD0 push 0x1D5950
	{ uint32_t v = (uint32_t)(0x1D5950u); X86_PUSH32(v); }
	// 001F0CD5 push 0x0A
	{ uint32_t v = (uint32_t)(0xAu); X86_PUSH32(v); }
	// 001F0CD7 push 0x20
	{ uint32_t v = (uint32_t)(0x20u); X86_PUSH32(v); }
	// 001F0CD9 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0CDC add eax, 0x28
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x28u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0CDF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0CE0 call 0x001DEF24
	X86_PUSH32(0x1F0CE5u); sub_001DEF24(ctx, base);
	// 001F0CE5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0CD0, __imp__sub_001F0CD0)

X86_FUNC(__imp__sub_001F0CE6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0CE6 mov eax, 0x2803D8
	{ ctx.eax = (uint32_t)(0x2803D8u); }
	// 001F0CEB jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0CE6, __imp__sub_001F0CE6)

X86_FUNC(__imp__sub_001F0CF0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0CF0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0CF5 push 0x12D
	{ uint32_t v = (uint32_t)(0x12Du); X86_PUSH32(v); }
	// 001F0CFA push 0x2701AC
	{ uint32_t v = (uint32_t)(0x2701ACu); X86_PUSH32(v); }
	// 001F0CFF mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0D02 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0D03 call 0x000127D0
	X86_PUSH32(0x1F0D08u); sub_000127D0(ctx, base);
	// 001F0D08 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0D0B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0CF0, __imp__sub_001F0CF0)

X86_FUNC(__imp__sub_001F0D0C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D0C mov eax, 0x2803FC
	{ ctx.eax = (uint32_t)(0x2803FCu); }
	// 001F0D11 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0D0C, __imp__sub_001F0D0C)

X86_FUNC(__imp__sub_001F0D17) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D17 int3
	x86_trap(ctx, 0x1F0D17, "int3");
}
X86_WEAK_FUNC(sub_001F0D17, __imp__sub_001F0D17)

X86_FUNC(__imp__sub_001F0D20) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D20 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0D25 push 0x19
	{ uint32_t v = (uint32_t)(0x19u); X86_PUSH32(v); }
	// 001F0D27 push 0x2702CC
	{ uint32_t v = (uint32_t)(0x2702CCu); X86_PUSH32(v); }
	// 001F0D2C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0D2F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0D30 call 0x000127D0
	X86_PUSH32(0x1F0D35u); sub_000127D0(ctx, base);
	// 001F0D35 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0D38 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0D20, __imp__sub_001F0D20)

X86_FUNC(__imp__sub_001F0D39) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D39 mov eax, 0x280444
	{ ctx.eax = (uint32_t)(0x280444u); }
	// 001F0D3E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0D39, __imp__sub_001F0D39)

X86_FUNC(__imp__sub_001F0D50) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D50 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0D55 push 0x1AE
	{ uint32_t v = (uint32_t)(0x1AEu); X86_PUSH32(v); }
	// 001F0D5A push 0x270328
	{ uint32_t v = (uint32_t)(0x270328u); X86_PUSH32(v); }
	// 001F0D5F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0D62 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0D63 call 0x000127D0
	X86_PUSH32(0x1F0D68u); sub_000127D0(ctx, base);
	// 001F0D68 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0D6B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0D50, __imp__sub_001F0D50)

X86_FUNC(__imp__sub_001F0D6C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D6C mov eax, 0x280468
	{ ctx.eax = (uint32_t)(0x280468u); }
	// 001F0D71 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0D6C, __imp__sub_001F0D6C)

X86_FUNC(__imp__sub_001F0D80) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D80 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0D85 push 0x953
	{ uint32_t v = (uint32_t)(0x953u); X86_PUSH32(v); }
	// 001F0D8A push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0D8F mov eax, [ebp-0x210]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0D95 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0D96 call 0x000127D0
	X86_PUSH32(0x1F0D9Bu); sub_000127D0(ctx, base);
	// 001F0D9B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0D9E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0D80, __imp__sub_001F0D80)

X86_FUNC(__imp__sub_001F0D9F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0D9F mov eax, 0x28048C
	{ ctx.eax = (uint32_t)(0x28048Cu); }
	// 001F0DA4 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0D9F, __imp__sub_001F0D9F)

X86_FUNC(__imp__sub_001F0DAE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0DAE int3
	x86_trap(ctx, 0x1F0DAE, "int3");
}
X86_WEAK_FUNC(sub_001F0DAE, __imp__sub_001F0DAE)

X86_FUNC(__imp__sub_001F0DB0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0DB0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0DB5 push 0x1F4
	{ uint32_t v = (uint32_t)(0x1F4u); X86_PUSH32(v); }
	// 001F0DBA push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0DBF mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0DC5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0DC6 call 0x000127D0
	X86_PUSH32(0x1F0DCBu); sub_000127D0(ctx, base);
	// 001F0DCB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0DCE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0DB0, __imp__sub_001F0DB0)

X86_FUNC(__imp__sub_001F0DCF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0DCF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0DD4 push 0x214
	{ uint32_t v = (uint32_t)(0x214u); X86_PUSH32(v); }
	// 001F0DD9 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0DDE mov eax, [ebp-0x1F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFE10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0DE4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0DE5 call 0x000127D0
	X86_PUSH32(0x1F0DEAu); sub_000127D0(ctx, base);
	// 001F0DEA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0DED ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0DCF, __imp__sub_001F0DCF)

X86_FUNC(__imp__sub_001F0DEE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0DEE push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0DF3 push 0x235
	{ uint32_t v = (uint32_t)(0x235u); X86_PUSH32(v); }
	// 001F0DF8 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0DFD mov eax, [ebp-0x1F0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFE10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0E03 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0E04 call 0x000127D0
	X86_PUSH32(0x1F0E09u); sub_000127D0(ctx, base);
	// 001F0E09 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0E0C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0DEE, __imp__sub_001F0DEE)

X86_FUNC(__imp__sub_001F0E0D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0E0D push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0E12 push 0x26B
	{ uint32_t v = (uint32_t)(0x26Bu); X86_PUSH32(v); }
	// 001F0E17 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0E1C mov eax, [ebp-0x1E8]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFE18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0E22 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0E23 call 0x000127D0
	X86_PUSH32(0x1F0E28u); sub_000127D0(ctx, base);
	// 001F0E28 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0E2B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0E0D, __imp__sub_001F0E0D)

X86_FUNC(__imp__sub_001F0E2C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0E2C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0E31 push 0x52B
	{ uint32_t v = (uint32_t)(0x52Bu); X86_PUSH32(v); }
	// 001F0E36 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0E3B mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0E41 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0E42 call 0x000127D0
	X86_PUSH32(0x1F0E47u); sub_000127D0(ctx, base);
	// 001F0E47 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0E4A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0E2C, __imp__sub_001F0E2C)

X86_FUNC(__imp__sub_001F0E4B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0E4B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0E50 push 0x559
	{ uint32_t v = (uint32_t)(0x559u); X86_PUSH32(v); }
	// 001F0E55 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0E5A mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0E60 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0E61 call 0x000127D0
	X86_PUSH32(0x1F0E66u); sub_000127D0(ctx, base);
	// 001F0E66 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0E69 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0E4B, __imp__sub_001F0E4B)

X86_FUNC(__imp__sub_001F0E6A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0E6A push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0E6F push 0x572
	{ uint32_t v = (uint32_t)(0x572u); X86_PUSH32(v); }
	// 001F0E74 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0E79 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0E7F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0E80 call 0x000127D0
	X86_PUSH32(0x1F0E85u); sub_000127D0(ctx, base);
	// 001F0E85 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0E88 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0E6A, __imp__sub_001F0E6A)

X86_FUNC(__imp__sub_001F0E89) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0E89 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0E8E push 0x589
	{ uint32_t v = (uint32_t)(0x589u); X86_PUSH32(v); }
	// 001F0E93 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0E98 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0E9E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0E9F call 0x000127D0
	X86_PUSH32(0x1F0EA4u); sub_000127D0(ctx, base);
	// 001F0EA4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0EA7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0E89, __imp__sub_001F0E89)

X86_FUNC(__imp__sub_001F0EA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0EA8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0EAD push 0x5AF
	{ uint32_t v = (uint32_t)(0x5AFu); X86_PUSH32(v); }
	// 001F0EB2 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0EB7 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0EBD push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0EBE call 0x000127D0
	X86_PUSH32(0x1F0EC3u); sub_000127D0(ctx, base);
	// 001F0EC3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0EC6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0EA8, __imp__sub_001F0EA8)

X86_FUNC(__imp__sub_001F0EC7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0EC7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0ECC push 0x60C
	{ uint32_t v = (uint32_t)(0x60Cu); X86_PUSH32(v); }
	// 001F0ED1 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0ED6 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0EDC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0EDD call 0x000127D0
	X86_PUSH32(0x1F0EE2u); sub_000127D0(ctx, base);
	// 001F0EE2 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0EE5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0EC7, __imp__sub_001F0EC7)

X86_FUNC(__imp__sub_001F0EE6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0EE6 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0EEB push 0x64C
	{ uint32_t v = (uint32_t)(0x64Cu); X86_PUSH32(v); }
	// 001F0EF0 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0EF5 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0EFB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0EFC call 0x000127D0
	X86_PUSH32(0x1F0F01u); sub_000127D0(ctx, base);
	// 001F0F01 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0F04 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0EE6, __imp__sub_001F0EE6)

X86_FUNC(__imp__sub_001F0F05) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0F05 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0F0A push 0x674
	{ uint32_t v = (uint32_t)(0x674u); X86_PUSH32(v); }
	// 001F0F0F push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0F14 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0F1A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0F1B call 0x000127D0
	X86_PUSH32(0x1F0F20u); sub_000127D0(ctx, base);
	// 001F0F20 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0F23 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0F05, __imp__sub_001F0F05)

X86_FUNC(__imp__sub_001F0F24) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0F24 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0F29 push 0x722
	{ uint32_t v = (uint32_t)(0x722u); X86_PUSH32(v); }
	// 001F0F2E push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0F33 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0F39 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0F3A call 0x000127D0
	X86_PUSH32(0x1F0F3Fu); sub_000127D0(ctx, base);
	// 001F0F3F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0F42 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0F24, __imp__sub_001F0F24)

X86_FUNC(__imp__sub_001F0F43) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0F43 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0F48 push 0x72A
	{ uint32_t v = (uint32_t)(0x72Au); X86_PUSH32(v); }
	// 001F0F4D push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0F52 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0F58 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0F59 call 0x000127D0
	X86_PUSH32(0x1F0F5Eu); sub_000127D0(ctx, base);
	// 001F0F5E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0F61 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0F43, __imp__sub_001F0F43)

X86_FUNC(__imp__sub_001F0F62) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0F62 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0F67 push 0x74F
	{ uint32_t v = (uint32_t)(0x74Fu); X86_PUSH32(v); }
	// 001F0F6C push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0F71 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0F77 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0F78 call 0x000127D0
	X86_PUSH32(0x1F0F7Du); sub_000127D0(ctx, base);
	// 001F0F7D add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0F80 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0F62, __imp__sub_001F0F62)

X86_FUNC(__imp__sub_001F0F81) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0F81 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0F86 push 0x759
	{ uint32_t v = (uint32_t)(0x759u); X86_PUSH32(v); }
	// 001F0F8B push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0F90 mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0F96 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0F97 call 0x000127D0
	X86_PUSH32(0x1F0F9Cu); sub_000127D0(ctx, base);
	// 001F0F9C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0F9F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0F81, __imp__sub_001F0F81)

X86_FUNC(__imp__sub_001F0FA0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0FA0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0FA5 push 0x762
	{ uint32_t v = (uint32_t)(0x762u); X86_PUSH32(v); }
	// 001F0FAA push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0FAF mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0FB5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0FB6 call 0x000127D0
	X86_PUSH32(0x1F0FBBu); sub_000127D0(ctx, base);
	// 001F0FBB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0FBE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0FA0, __imp__sub_001F0FA0)

X86_FUNC(__imp__sub_001F0FBF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0FBF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0FC4 push 0x769
	{ uint32_t v = (uint32_t)(0x769u); X86_PUSH32(v); }
	// 001F0FC9 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0FCE mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0FD4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0FD5 call 0x000127D0
	X86_PUSH32(0x1F0FDAu); sub_000127D0(ctx, base);
	// 001F0FDA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0FDD ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0FBF, __imp__sub_001F0FBF)

X86_FUNC(__imp__sub_001F0FDE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0FDE push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F0FE3 push 0x833
	{ uint32_t v = (uint32_t)(0x833u); X86_PUSH32(v); }
	// 001F0FE8 push 0x270594
	{ uint32_t v = (uint32_t)(0x270594u); X86_PUSH32(v); }
	// 001F0FED mov eax, [ebp-0x234]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFDCCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F0FF3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F0FF4 call 0x000127D0
	X86_PUSH32(0x1F0FF9u); sub_000127D0(ctx, base);
	// 001F0FF9 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F0FFC ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F0FDE, __imp__sub_001F0FDE)

X86_FUNC(__imp__sub_001F0FFD) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F0FFD mov eax, 0x280540
	{ ctx.eax = (uint32_t)(0x280540u); }
	// 001F1002 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F0FFD, __imp__sub_001F0FFD)

X86_FUNC(__imp__sub_001F1010) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1010 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1015 push 0x62
	{ uint32_t v = (uint32_t)(0x62u); X86_PUSH32(v); }
	// 001F1017 push 0x271BEC
	{ uint32_t v = (uint32_t)(0x271BECu); X86_PUSH32(v); }
	// 001F101C mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F101F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1020 call 0x000127D0
	X86_PUSH32(0x1F1025u); sub_000127D0(ctx, base);
	// 001F1025 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1028 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1010, __imp__sub_001F1010)

X86_FUNC(__imp__sub_001F1029) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1029 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F102E push 0x68
	{ uint32_t v = (uint32_t)(0x68u); X86_PUSH32(v); }
	// 001F1030 push 0x271BEC
	{ uint32_t v = (uint32_t)(0x271BECu); X86_PUSH32(v); }
	// 001F1035 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1038 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1039 call 0x000127D0
	X86_PUSH32(0x1F103Eu); sub_000127D0(ctx, base);
	// 001F103E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1041 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1029, __imp__sub_001F1029)

X86_FUNC(__imp__sub_001F1042) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1042 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1047 push 0x70
	{ uint32_t v = (uint32_t)(0x70u); X86_PUSH32(v); }
	// 001F1049 push 0x271BEC
	{ uint32_t v = (uint32_t)(0x271BECu); X86_PUSH32(v); }
	// 001F104E mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1051 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1052 call 0x000127D0
	X86_PUSH32(0x1F1057u); sub_000127D0(ctx, base);
	// 001F1057 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F105A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1042, __imp__sub_001F1042)

X86_FUNC(__imp__sub_001F105B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F105B mov eax, 0x280574
	{ ctx.eax = (uint32_t)(0x280574u); }
	// 001F1060 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F105B, __imp__sub_001F105B)

X86_FUNC(__imp__sub_001F1070) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1070 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1075 push 0x5B6
	{ uint32_t v = (uint32_t)(0x5B6u); X86_PUSH32(v); }
	// 001F107A push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F107F mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1082 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1083 call 0x000127D0
	X86_PUSH32(0x1F1088u); sub_000127D0(ctx, base);
	// 001F1088 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F108B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1070, __imp__sub_001F1070)

X86_FUNC(__imp__sub_001F108C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F108C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1091 push 0x5B8
	{ uint32_t v = (uint32_t)(0x5B8u); X86_PUSH32(v); }
	// 001F1096 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F109B mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F109E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F109F call 0x000127D0
	X86_PUSH32(0x1F10A4u); sub_000127D0(ctx, base);
	// 001F10A4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F10A7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F108C, __imp__sub_001F108C)

X86_FUNC(__imp__sub_001F10A8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F10A8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F10AD push 0x5BA
	{ uint32_t v = (uint32_t)(0x5BAu); X86_PUSH32(v); }
	// 001F10B2 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F10B7 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F10BA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F10BB call 0x000127D0
	X86_PUSH32(0x1F10C0u); sub_000127D0(ctx, base);
	// 001F10C0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F10C3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F10A8, __imp__sub_001F10A8)

X86_FUNC(__imp__sub_001F10C4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F10C4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F10C9 push 0x5BC
	{ uint32_t v = (uint32_t)(0x5BCu); X86_PUSH32(v); }
	// 001F10CE push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F10D3 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F10D6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F10D7 call 0x000127D0
	X86_PUSH32(0x1F10DCu); sub_000127D0(ctx, base);
	// 001F10DC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F10DF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F10C4, __imp__sub_001F10C4)

X86_FUNC(__imp__sub_001F10E0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F10E0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F10E5 push 0x5C4
	{ uint32_t v = (uint32_t)(0x5C4u); X86_PUSH32(v); }
	// 001F10EA push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F10EF mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F10F2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F10F3 call 0x000127D0
	X86_PUSH32(0x1F10F8u); sub_000127D0(ctx, base);
	// 001F10F8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F10FB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F10E0, __imp__sub_001F10E0)

X86_FUNC(__imp__sub_001F10FC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F10FC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1101 push 0x5C6
	{ uint32_t v = (uint32_t)(0x5C6u); X86_PUSH32(v); }
	// 001F1106 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F110B mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F110E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F110F call 0x000127D0
	X86_PUSH32(0x1F1114u); sub_000127D0(ctx, base);
	// 001F1114 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1117 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F10FC, __imp__sub_001F10FC)

X86_FUNC(__imp__sub_001F1118) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1118 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F111D push 0x5C8
	{ uint32_t v = (uint32_t)(0x5C8u); X86_PUSH32(v); }
	// 001F1122 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1127 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F112A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F112B call 0x000127D0
	X86_PUSH32(0x1F1130u); sub_000127D0(ctx, base);
	// 001F1130 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1133 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1118, __imp__sub_001F1118)

X86_FUNC(__imp__sub_001F1134) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1134 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1139 push 0x5CA
	{ uint32_t v = (uint32_t)(0x5CAu); X86_PUSH32(v); }
	// 001F113E push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1143 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1146 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1147 call 0x000127D0
	X86_PUSH32(0x1F114Cu); sub_000127D0(ctx, base);
	// 001F114C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F114F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1134, __imp__sub_001F1134)

X86_FUNC(__imp__sub_001F1150) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1150 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1155 push 0x5EF
	{ uint32_t v = (uint32_t)(0x5EFu); X86_PUSH32(v); }
	// 001F115A push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F115F mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1162 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1163 call 0x000127D0
	X86_PUSH32(0x1F1168u); sub_000127D0(ctx, base);
	// 001F1168 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F116B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1150, __imp__sub_001F1150)

X86_FUNC(__imp__sub_001F116C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F116C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1171 push 0x60C
	{ uint32_t v = (uint32_t)(0x60Cu); X86_PUSH32(v); }
	// 001F1176 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F117B mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F117E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F117F call 0x000127D0
	X86_PUSH32(0x1F1184u); sub_000127D0(ctx, base);
	// 001F1184 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1187 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F116C, __imp__sub_001F116C)

X86_FUNC(__imp__sub_001F1188) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1188 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F118D push 0x632
	{ uint32_t v = (uint32_t)(0x632u); X86_PUSH32(v); }
	// 001F1192 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1197 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F119A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F119B call 0x000127D0
	X86_PUSH32(0x1F11A0u); sub_000127D0(ctx, base);
	// 001F11A0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F11A3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1188, __imp__sub_001F1188)

X86_FUNC(__imp__sub_001F11A4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F11A4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F11A9 push 0x63C
	{ uint32_t v = (uint32_t)(0x63Cu); X86_PUSH32(v); }
	// 001F11AE push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F11B3 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F11B6 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F11B7 call 0x000127D0
	X86_PUSH32(0x1F11BCu); sub_000127D0(ctx, base);
	// 001F11BC add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F11BF ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F11A4, __imp__sub_001F11A4)

X86_FUNC(__imp__sub_001F11C0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F11C0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F11C5 push 0x688
	{ uint32_t v = (uint32_t)(0x688u); X86_PUSH32(v); }
	// 001F11CA push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F11CF mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F11D2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F11D3 call 0x000127D0
	X86_PUSH32(0x1F11D8u); sub_000127D0(ctx, base);
	// 001F11D8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F11DB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F11C0, __imp__sub_001F11C0)

X86_FUNC(__imp__sub_001F11DC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F11DC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F11E1 push 0x698
	{ uint32_t v = (uint32_t)(0x698u); X86_PUSH32(v); }
	// 001F11E6 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F11EB mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F11EE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F11EF call 0x000127D0
	X86_PUSH32(0x1F11F4u); sub_000127D0(ctx, base);
	// 001F11F4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F11F7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F11DC, __imp__sub_001F11DC)

X86_FUNC(__imp__sub_001F11F8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F11F8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F11FD push 0x6A0
	{ uint32_t v = (uint32_t)(0x6A0u); X86_PUSH32(v); }
	// 001F1202 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1207 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F120A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F120B call 0x000127D0
	X86_PUSH32(0x1F1210u); sub_000127D0(ctx, base);
	// 001F1210 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1213 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F11F8, __imp__sub_001F11F8)

X86_FUNC(__imp__sub_001F1214) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1214 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1219 push 0x6A2
	{ uint32_t v = (uint32_t)(0x6A2u); X86_PUSH32(v); }
	// 001F121E push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1223 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1226 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1227 call 0x000127D0
	X86_PUSH32(0x1F122Cu); sub_000127D0(ctx, base);
	// 001F122C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F122F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1214, __imp__sub_001F1214)

X86_FUNC(__imp__sub_001F1230) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1230 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1235 push 0x704
	{ uint32_t v = (uint32_t)(0x704u); X86_PUSH32(v); }
	// 001F123A push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F123F mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1242 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1243 call 0x000127D0
	X86_PUSH32(0x1F1248u); sub_000127D0(ctx, base);
	// 001F1248 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F124B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1230, __imp__sub_001F1230)

X86_FUNC(__imp__sub_001F124C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F124C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1251 push 0x746
	{ uint32_t v = (uint32_t)(0x746u); X86_PUSH32(v); }
	// 001F1256 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F125B mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F125E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F125F call 0x000127D0
	X86_PUSH32(0x1F1264u); sub_000127D0(ctx, base);
	// 001F1264 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1267 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F124C, __imp__sub_001F124C)

X86_FUNC(__imp__sub_001F1268) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1268 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F126D push 0x75B
	{ uint32_t v = (uint32_t)(0x75Bu); X86_PUSH32(v); }
	// 001F1272 push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1277 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F127A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F127B call 0x000127D0
	X86_PUSH32(0x1F1280u); sub_000127D0(ctx, base);
	// 001F1280 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1283 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1268, __imp__sub_001F1268)

X86_FUNC(__imp__sub_001F1284) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1284 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1289 push 0x773
	{ uint32_t v = (uint32_t)(0x773u); X86_PUSH32(v); }
	// 001F128E push 0x27211C
	{ uint32_t v = (uint32_t)(0x27211Cu); X86_PUSH32(v); }
	// 001F1293 mov eax, [ebp-0x70]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF90u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1296 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1297 call 0x000127D0
	X86_PUSH32(0x1F129Cu); sub_000127D0(ctx, base);
	// 001F129C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F129F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1284, __imp__sub_001F1284)

X86_FUNC(__imp__sub_001F12A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F12A0 mov eax, 0x280630
	{ ctx.eax = (uint32_t)(0x280630u); }
	// 001F12A5 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F12A0, __imp__sub_001F12A0)

X86_FUNC(__imp__sub_001F12B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F12B0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F12B5 push 0x116
	{ uint32_t v = (uint32_t)(0x116u); X86_PUSH32(v); }
	// 001F12BA push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F12BF mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F12C5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F12C6 call 0x000127D0
	X86_PUSH32(0x1F12CBu); sub_000127D0(ctx, base);
	// 001F12CB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F12CE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F12B0, __imp__sub_001F12B0)

X86_FUNC(__imp__sub_001F12CF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F12CF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F12D4 push 0x119
	{ uint32_t v = (uint32_t)(0x119u); X86_PUSH32(v); }
	// 001F12D9 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F12DE mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F12E4 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F12E5 call 0x000127D0
	X86_PUSH32(0x1F12EAu); sub_000127D0(ctx, base);
	// 001F12EA add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F12ED ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F12CF, __imp__sub_001F12CF)

X86_FUNC(__imp__sub_001F12EE) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F12EE push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F12F3 push 0x11F
	{ uint32_t v = (uint32_t)(0x11Fu); X86_PUSH32(v); }
	// 001F12F8 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F12FD mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1303 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1304 call 0x000127D0
	X86_PUSH32(0x1F1309u); sub_000127D0(ctx, base);
	// 001F1309 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F130C ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F12EE, __imp__sub_001F12EE)

X86_FUNC(__imp__sub_001F130D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F130D push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1312 push 0x122
	{ uint32_t v = (uint32_t)(0x122u); X86_PUSH32(v); }
	// 001F1317 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F131C mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1322 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1323 call 0x000127D0
	X86_PUSH32(0x1F1328u); sub_000127D0(ctx, base);
	// 001F1328 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F132B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F130D, __imp__sub_001F130D)

X86_FUNC(__imp__sub_001F132C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F132C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1331 push 0x127
	{ uint32_t v = (uint32_t)(0x127u); X86_PUSH32(v); }
	// 001F1336 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F133B mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1341 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1342 call 0x000127D0
	X86_PUSH32(0x1F1347u); sub_000127D0(ctx, base);
	// 001F1347 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F134A ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F132C, __imp__sub_001F132C)

X86_FUNC(__imp__sub_001F134B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F134B push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1350 push 0x175
	{ uint32_t v = (uint32_t)(0x175u); X86_PUSH32(v); }
	// 001F1355 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F135A mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1360 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1361 call 0x000127D0
	X86_PUSH32(0x1F1366u); sub_000127D0(ctx, base);
	// 001F1366 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1369 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F134B, __imp__sub_001F134B)

X86_FUNC(__imp__sub_001F136A) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F136A push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F136F push 0x178
	{ uint32_t v = (uint32_t)(0x178u); X86_PUSH32(v); }
	// 001F1374 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F1379 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F137F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1380 call 0x000127D0
	X86_PUSH32(0x1F1385u); sub_000127D0(ctx, base);
	// 001F1385 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1388 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F136A, __imp__sub_001F136A)

X86_FUNC(__imp__sub_001F1389) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1389 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F138E push 0x17B
	{ uint32_t v = (uint32_t)(0x17Bu); X86_PUSH32(v); }
	// 001F1393 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F1398 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F139E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F139F call 0x000127D0
	X86_PUSH32(0x1F13A4u); sub_000127D0(ctx, base);
	// 001F13A4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F13A7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1389, __imp__sub_001F1389)

X86_FUNC(__imp__sub_001F13A8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F13A8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F13AD push 0x17E
	{ uint32_t v = (uint32_t)(0x17Eu); X86_PUSH32(v); }
	// 001F13B2 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F13B7 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F13BD push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F13BE call 0x000127D0
	X86_PUSH32(0x1F13C3u); sub_000127D0(ctx, base);
	// 001F13C3 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F13C6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F13A8, __imp__sub_001F13A8)

X86_FUNC(__imp__sub_001F13C7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F13C7 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F13CC push 0x181
	{ uint32_t v = (uint32_t)(0x181u); X86_PUSH32(v); }
	// 001F13D1 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F13D6 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F13DC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F13DD call 0x000127D0
	X86_PUSH32(0x1F13E2u); sub_000127D0(ctx, base);
	// 001F13E2 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F13E5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F13C7, __imp__sub_001F13C7)

X86_FUNC(__imp__sub_001F13E6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F13E6 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F13EB push 0x184
	{ uint32_t v = (uint32_t)(0x184u); X86_PUSH32(v); }
	// 001F13F0 push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F13F5 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F13FB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F13FC call 0x000127D0
	X86_PUSH32(0x1F1401u); sub_000127D0(ctx, base);
	// 001F1401 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1404 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F13E6, __imp__sub_001F13E6)

X86_FUNC(__imp__sub_001F1405) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1405 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F140A push 0x187
	{ uint32_t v = (uint32_t)(0x187u); X86_PUSH32(v); }
	// 001F140F push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F1414 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F141A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F141B call 0x000127D0
	X86_PUSH32(0x1F1420u); sub_000127D0(ctx, base);
	// 001F1420 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1423 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1405, __imp__sub_001F1405)

X86_FUNC(__imp__sub_001F1424) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1424 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1429 push 0x18A
	{ uint32_t v = (uint32_t)(0x18Au); X86_PUSH32(v); }
	// 001F142E push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F1433 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1439 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F143A call 0x000127D0
	X86_PUSH32(0x1F143Fu); sub_000127D0(ctx, base);
	// 001F143F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1442 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1424, __imp__sub_001F1424)

X86_FUNC(__imp__sub_001F1443) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1443 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1448 push 0x18D
	{ uint32_t v = (uint32_t)(0x18Du); X86_PUSH32(v); }
	// 001F144D push 0x272AB4
	{ uint32_t v = (uint32_t)(0x272AB4u); X86_PUSH32(v); }
	// 001F1452 mov eax, [ebp-0xB0]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF50u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1458 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1459 call 0x000127D0
	X86_PUSH32(0x1F145Eu); sub_000127D0(ctx, base);
	// 001F145E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1461 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1443, __imp__sub_001F1443)

X86_FUNC(__imp__sub_001F1462) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1462 mov eax, 0x2806BC
	{ ctx.eax = (uint32_t)(0x2806BCu); }
	// 001F1467 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1462, __imp__sub_001F1462)

X86_FUNC(__imp__sub_001F1470) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1470 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1475 push 0x11F
	{ uint32_t v = (uint32_t)(0x11Fu); X86_PUSH32(v); }
	// 001F147A push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F147F mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1482 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1483 call 0x000127D0
	X86_PUSH32(0x1F1488u); sub_000127D0(ctx, base);
	// 001F1488 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F148B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1470, __imp__sub_001F1470)

X86_FUNC(__imp__sub_001F148C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F148C mov eax, 0x2806E0
	{ ctx.eax = (uint32_t)(0x2806E0u); }
	// 001F1491 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F148C, __imp__sub_001F148C)

X86_FUNC(__imp__sub_001F14A0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F14A0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F14A5 push 0x4AA
	{ uint32_t v = (uint32_t)(0x4AAu); X86_PUSH32(v); }
	// 001F14AA push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F14AF mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F14B2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F14B3 call 0x000127D0
	X86_PUSH32(0x1F14B8u); sub_000127D0(ctx, base);
	// 001F14B8 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F14BB ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F14A0, __imp__sub_001F14A0)

X86_FUNC(__imp__sub_001F14BC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F14BC push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F14C1 push 0x4E7
	{ uint32_t v = (uint32_t)(0x4E7u); X86_PUSH32(v); }
	// 001F14C6 push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F14CB mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F14CE push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F14CF call 0x000127D0
	X86_PUSH32(0x1F14D4u); sub_000127D0(ctx, base);
	// 001F14D4 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F14D7 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F14BC, __imp__sub_001F14BC)

X86_FUNC(__imp__sub_001F14D8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F14D8 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F14DD push 0x4F7
	{ uint32_t v = (uint32_t)(0x4F7u); X86_PUSH32(v); }
	// 001F14E2 push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F14E7 mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F14EA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F14EB call 0x000127D0
	X86_PUSH32(0x1F14F0u); sub_000127D0(ctx, base);
	// 001F14F0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F14F3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F14D8, __imp__sub_001F14D8)

X86_FUNC(__imp__sub_001F14F4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F14F4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F14F9 push 0x50A
	{ uint32_t v = (uint32_t)(0x50Au); X86_PUSH32(v); }
	// 001F14FE push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F1503 mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1506 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1507 call 0x000127D0
	X86_PUSH32(0x1F150Cu); sub_000127D0(ctx, base);
	// 001F150C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F150F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F14F4, __imp__sub_001F14F4)

X86_FUNC(__imp__sub_001F1510) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1510 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1515 push 0x51A
	{ uint32_t v = (uint32_t)(0x51Au); X86_PUSH32(v); }
	// 001F151A push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F151F mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1522 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1523 call 0x000127D0
	X86_PUSH32(0x1F1528u); sub_000127D0(ctx, base);
	// 001F1528 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F152B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1510, __imp__sub_001F1510)

X86_FUNC(__imp__sub_001F152C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F152C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1531 push 0x52A
	{ uint32_t v = (uint32_t)(0x52Au); X86_PUSH32(v); }
	// 001F1536 push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F153B mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F153E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F153F call 0x000127D0
	X86_PUSH32(0x1F1544u); sub_000127D0(ctx, base);
	// 001F1544 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1547 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F152C, __imp__sub_001F152C)

X86_FUNC(__imp__sub_001F1548) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1548 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F154D push 0x53E
	{ uint32_t v = (uint32_t)(0x53Eu); X86_PUSH32(v); }
	// 001F1552 push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F1557 mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F155A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F155B call 0x000127D0
	X86_PUSH32(0x1F1560u); sub_000127D0(ctx, base);
	// 001F1560 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1563 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1548, __imp__sub_001F1548)

X86_FUNC(__imp__sub_001F1564) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1564 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1569 push 0x54C
	{ uint32_t v = (uint32_t)(0x54Cu); X86_PUSH32(v); }
	// 001F156E push 0x272CF8
	{ uint32_t v = (uint32_t)(0x272CF8u); X86_PUSH32(v); }
	// 001F1573 mov eax, [ebp-0x80]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFF80u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1576 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1577 call 0x000127D0
	X86_PUSH32(0x1F157Cu); sub_000127D0(ctx, base);
	// 001F157C add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F157F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1564, __imp__sub_001F1564)

X86_FUNC(__imp__sub_001F1580) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1580 mov eax, 0x28073C
	{ ctx.eax = (uint32_t)(0x28073Cu); }
	// 001F1585 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1580, __imp__sub_001F1580)

X86_FUNC(__imp__sub_001F1590) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1590 mov ecx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1593 jmp 0x000B02D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000B02D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1590, __imp__sub_001F1590)

X86_FUNC(__imp__sub_001F1598) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1598 mov eax, 0x280760
	{ ctx.eax = (uint32_t)(0x280760u); }
	// 001F159D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1598, __imp__sub_001F1598)

X86_FUNC(__imp__sub_001F15B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F15B0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F15B3 add ecx, 0x04
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F15B6 jmp 0x001D5950
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001D5950(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F15B0, __imp__sub_001F15B0)

X86_FUNC(__imp__sub_001F15BB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F15BB mov eax, 0x2807A8
	{ ctx.eax = (uint32_t)(0x2807A8u); }
	// 001F15C0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F15BB, __imp__sub_001F15BB)

X86_FUNC(__imp__sub_001F15D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F15D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F15D5 push 0x77
	{ uint32_t v = (uint32_t)(0x77u); X86_PUSH32(v); }
	// 001F15D7 push 0x273490
	{ uint32_t v = (uint32_t)(0x273490u); X86_PUSH32(v); }
	// 001F15DC mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F15DF push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F15E0 call 0x000127D0
	X86_PUSH32(0x1F15E5u); sub_000127D0(ctx, base);
	// 001F15E5 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F15E8 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F15D0, __imp__sub_001F15D0)

X86_FUNC(__imp__sub_001F15E9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F15E9 mov eax, 0x2807CC
	{ ctx.eax = (uint32_t)(0x2807CCu); }
	// 001F15EE jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F15E9, __imp__sub_001F15E9)

X86_FUNC(__imp__sub_001F1600) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1600 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1605 push 0xA0
	{ uint32_t v = (uint32_t)(0xA0u); X86_PUSH32(v); }
	// 001F160A push 0x273490
	{ uint32_t v = (uint32_t)(0x273490u); X86_PUSH32(v); }
	// 001F160F mov eax, [ebp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1612 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1613 call 0x000127D0
	X86_PUSH32(0x1F1618u); sub_000127D0(ctx, base);
	// 001F1618 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F161B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1600, __imp__sub_001F1600)

X86_FUNC(__imp__sub_001F161C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F161C mov eax, 0x2807F0
	{ ctx.eax = (uint32_t)(0x2807F0u); }
	// 001F1621 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F161C, __imp__sub_001F161C)

X86_FUNC(__imp__sub_001F1630) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1630 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1633 add ecx, 0x10
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1636 jmp 0x000AD930
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_000AD930(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1630, __imp__sub_001F1630)

X86_FUNC(__imp__sub_001F163B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F163B mov eax, 0x280814
	{ ctx.eax = (uint32_t)(0x280814u); }
	// 001F1640 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F163B, __imp__sub_001F163B)

X86_FUNC(__imp__sub_001F1650) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1650 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1655 push 0x39
	{ uint32_t v = (uint32_t)(0x39u); X86_PUSH32(v); }
	// 001F1657 push 0x273910
	{ uint32_t v = (uint32_t)(0x273910u); X86_PUSH32(v); }
	// 001F165C mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F165F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1660 call 0x000127D0
	X86_PUSH32(0x1F1665u); sub_000127D0(ctx, base);
	// 001F1665 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1668 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1650, __imp__sub_001F1650)

X86_FUNC(__imp__sub_001F1669) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1669 mov eax, 0x280838
	{ ctx.eax = (uint32_t)(0x280838u); }
	// 001F166E jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1669, __imp__sub_001F1669)

X86_FUNC(__imp__sub_001F1680) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1680 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1683 add ecx, 0x4C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x4Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1686 jmp 0x00194A50
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00194A50(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1680, __imp__sub_001F1680)

X86_FUNC(__imp__sub_001F168B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F168B mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F168E add ecx, 0x1368
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1368u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1694 jmp 0x00192C60
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00192C60(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F168B, __imp__sub_001F168B)

X86_FUNC(__imp__sub_001F1699) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1699 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F169C add ecx, 0x1428
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1428u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16A2 jmp 0x001922A0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001922A0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1699, __imp__sub_001F1699)

X86_FUNC(__imp__sub_001F16A7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16A7 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16AA add ecx, 0x1470
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1470u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16B0 jmp 0x00190EA0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00190EA0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16A7, __imp__sub_001F16A7)

X86_FUNC(__imp__sub_001F16B5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16B5 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16B8 add ecx, 0x1C50
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1C50u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16BE jmp 0x001907F0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001907F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16B5, __imp__sub_001F16B5)

X86_FUNC(__imp__sub_001F16C3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16C3 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16C6 add ecx, 0x1EE0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1EE0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16CC jmp 0x00190520
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00190520(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16C3, __imp__sub_001F16C3)

X86_FUNC(__imp__sub_001F16D1) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16D1 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16D4 add ecx, 0x201C
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x201Cu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16DA jmp 0x001900D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001900D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16D1, __imp__sub_001F16D1)

X86_FUNC(__imp__sub_001F16DF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16DF mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16E2 add ecx, 0x2060
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2060u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16E8 jmp 0x0018FE70
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018FE70(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16DF, __imp__sub_001F16DF)

X86_FUNC(__imp__sub_001F16ED) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16ED mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16F0 add ecx, 0x25B4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x25B4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F16F6 jmp 0x0018FBA0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018FBA0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16ED, __imp__sub_001F16ED)

X86_FUNC(__imp__sub_001F16FB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F16FB mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F16FE add ecx, 0x25C0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x25C0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1704 jmp 0x0018EC40
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018EC40(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F16FB, __imp__sub_001F16FB)

X86_FUNC(__imp__sub_001F1709) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1709 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F170C add ecx, 0x25CC
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x25CCu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1712 jmp 0x0018EAD0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018EAD0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1709, __imp__sub_001F1709)

X86_FUNC(__imp__sub_001F1717) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1717 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F171A add ecx, 0x25F0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x25F0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1720 jmp 0x0018E6E0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018E6E0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1717, __imp__sub_001F1717)

X86_FUNC(__imp__sub_001F1725) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1725 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1728 add ecx, 0x26B0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x26B0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F172E jmp 0x0018E8D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018E8D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1725, __imp__sub_001F1725)

X86_FUNC(__imp__sub_001F1733) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1733 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1736 add ecx, 0x26E8
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x26E8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F173C jmp 0x0018E6A0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018E6A0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1733, __imp__sub_001F1733)

X86_FUNC(__imp__sub_001F1741) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1741 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1744 add ecx, 0x26F4
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x26F4u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F174A jmp 0x0018E500
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018E500(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1741, __imp__sub_001F1741)

X86_FUNC(__imp__sub_001F174F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F174F mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1752 add ecx, 0x26FC
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x26FCu); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1758 jmp 0x0018E3B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018E3B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F174F, __imp__sub_001F174F)

X86_FUNC(__imp__sub_001F175D) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F175D mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1760 add ecx, 0x2738
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2738u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1766 jmp 0x0018DFF0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018DFF0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F175D, __imp__sub_001F175D)

X86_FUNC(__imp__sub_001F176B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F176B mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F176E add ecx, 0x2740
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2740u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1774 jmp 0x0018DDC0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018DDC0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F176B, __imp__sub_001F176B)

X86_FUNC(__imp__sub_001F1779) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1779 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F177C add ecx, 0x2840
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2840u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1782 jmp 0x0018DCD0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018DCD0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1779, __imp__sub_001F1779)

X86_FUNC(__imp__sub_001F1787) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1787 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F178A add ecx, 0x2850
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x2850u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1790 jmp 0x0018DC00
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018DC00(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1787, __imp__sub_001F1787)

X86_FUNC(__imp__sub_001F1795) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1795 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F1798 add ecx, 0x28F0
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x28F0u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F179E jmp 0x0018DAE0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018DAE0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1795, __imp__sub_001F1795)

X86_FUNC(__imp__sub_001F17A3) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F17A3 mov eax, 0x2808FC
	{ ctx.eax = (uint32_t)(0x2808FCu); }
	// 001F17A8 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F17A3, __imp__sub_001F17A3)

X86_FUNC(__imp__sub_001F17B0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F17B0 mov ecx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001F17B3 add ecx, 0x08
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F17B6 jmp 0x0018E8D0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0018E8D0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F17B0, __imp__sub_001F17B0)

X86_FUNC(__imp__sub_001F17BB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F17BB mov eax, 0x280A08
	{ ctx.eax = (uint32_t)(0x280A08u); }
	// 001F17C0 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F17BB, __imp__sub_001F17BB)

X86_FUNC(__imp__sub_001F17D0) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F17D0 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F17D5 push 0xE1
	{ uint32_t v = (uint32_t)(0xE1u); X86_PUSH32(v); }
	// 001F17DA push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F17DF mov eax, [ebp-0x110]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F17E5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F17E6 call 0x000127D0
	X86_PUSH32(0x1F17EBu); sub_000127D0(ctx, base);
	// 001F17EB add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F17EE ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F17D0, __imp__sub_001F17D0)

X86_FUNC(__imp__sub_001F17EF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F17EF push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F17F4 push 0xF5
	{ uint32_t v = (uint32_t)(0xF5u); X86_PUSH32(v); }
	// 001F17F9 push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F17FE mov eax, [ebp-0x110]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1804 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1805 call 0x000127D0
	X86_PUSH32(0x1F180Au); sub_000127D0(ctx, base);
	// 001F180A add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F180D ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F17EF, __imp__sub_001F17EF)

X86_FUNC(__imp__sub_001F180E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F180E mov eax, 0x280A58
	{ ctx.eax = (uint32_t)(0x280A58u); }
	// 001F1813 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F180E, __imp__sub_001F180E)

X86_FUNC(__imp__sub_001F1820) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1820 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1825 push 0x8FC
	{ uint32_t v = (uint32_t)(0x8FCu); X86_PUSH32(v); }
	// 001F182A push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F182F mov eax, [ebp-0x134]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFECCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1835 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1836 call 0x000127D0
	X86_PUSH32(0x1F183Bu); sub_000127D0(ctx, base);
	// 001F183B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F183E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1820, __imp__sub_001F1820)

X86_FUNC(__imp__sub_001F183F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F183F mov eax, 0x280AA0
	{ ctx.eax = (uint32_t)(0x280AA0u); }
	// 001F1844 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F183F, __imp__sub_001F183F)

X86_FUNC(__imp__sub_001F1850) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1850 lea ecx, [ebp-0x420]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBE0u));
	// 001F1856 jmp 0x001989B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001989B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1850, __imp__sub_001F1850)

X86_FUNC(__imp__sub_001F185B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F185B lea ecx, [ebp-0x430]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBD0u));
	// 001F1861 jmp 0x00198700
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00198700(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F185B, __imp__sub_001F185B)

X86_FUNC(__imp__sub_001F1866) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1866 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F186B push 0xE4C
	{ uint32_t v = (uint32_t)(0xE4Cu); X86_PUSH32(v); }
	// 001F1870 push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F1875 mov eax, [ebp-0x410]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F187B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F187C call 0x000127D0
	X86_PUSH32(0x1F1881u); sub_000127D0(ctx, base);
	// 001F1881 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1884 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1866, __imp__sub_001F1866)

X86_FUNC(__imp__sub_001F1885) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1885 mov eax, 0x280B1C
	{ ctx.eax = (uint32_t)(0x280B1Cu); }
	// 001F188A jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1885, __imp__sub_001F1885)

X86_FUNC(__imp__sub_001F1890) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1890 lea ecx, [ebp-0x41C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBE4u));
	// 001F1896 jmp 0x001989B0
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001989B0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1890, __imp__sub_001F1890)

X86_FUNC(__imp__sub_001F189B) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F189B lea ecx, [ebp-0x42C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFBD4u));
	// 001F18A1 jmp 0x00198700
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00198700(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F189B, __imp__sub_001F189B)

X86_FUNC(__imp__sub_001F18A6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F18A6 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F18AB push 0x11BE
	{ uint32_t v = (uint32_t)(0x11BEu); X86_PUSH32(v); }
	// 001F18B0 push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F18B5 mov eax, [ebp-0x430]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBD0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F18BB push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F18BC call 0x000127D0
	X86_PUSH32(0x1F18C1u); sub_000127D0(ctx, base);
	// 001F18C1 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F18C4 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F18A6, __imp__sub_001F18A6)

X86_FUNC(__imp__sub_001F18C5) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F18C5 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F18CA push 0x11FC
	{ uint32_t v = (uint32_t)(0x11FCu); X86_PUSH32(v); }
	// 001F18CF push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F18D4 mov eax, [ebp-0x430]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBD0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F18DA push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F18DB call 0x000127D0
	X86_PUSH32(0x1F18E0u); sub_000127D0(ctx, base);
	// 001F18E0 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F18E3 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F18C5, __imp__sub_001F18C5)

X86_FUNC(__imp__sub_001F18E4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F18E4 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F18E9 push 0x1209
	{ uint32_t v = (uint32_t)(0x1209u); X86_PUSH32(v); }
	// 001F18EE push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F18F3 mov eax, [ebp-0x430]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFBD0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F18F9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F18FA call 0x000127D0
	X86_PUSH32(0x1F18FFu); sub_000127D0(ctx, base);
	// 001F18FF add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1902 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F18E4, __imp__sub_001F18E4)

X86_FUNC(__imp__sub_001F1903) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1903 mov eax, 0x280B84
	{ ctx.eax = (uint32_t)(0x280B84u); }
	// 001F1908 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1903, __imp__sub_001F1903)

X86_FUNC(__imp__sub_001F1910) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1910 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1915 push 0x170E
	{ uint32_t v = (uint32_t)(0x170Eu); X86_PUSH32(v); }
	// 001F191A push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F191F mov eax, [ebp-0x124]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFEDCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1925 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1926 call 0x000127D0
	X86_PUSH32(0x1F192Bu); sub_000127D0(ctx, base);
	// 001F192B add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F192E ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1910, __imp__sub_001F1910)

X86_FUNC(__imp__sub_001F192F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F192F mov eax, 0x280BCC
	{ ctx.eax = (uint32_t)(0x280BCCu); }
	// 001F1934 jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F192F, __imp__sub_001F192F)

X86_FUNC(__imp__sub_001F1940) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1940 lea ecx, [ebp-0x38]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFC8u));
	// 001F1943 jmp 0x00196A70
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_00196A70(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1940, __imp__sub_001F1940)

X86_FUNC(__imp__sub_001F1948) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1948 mov eax, 0x280BF0
	{ ctx.eax = (uint32_t)(0x280BF0u); }
	// 001F194D jmp 0x001DEA96
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_001DEA96(ctx, base); return; }
}
X86_WEAK_FUNC(sub_001F1948, __imp__sub_001F1948)

X86_FUNC(__imp__sub_001F1960) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F1960 push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1965 push 0x2D1
	{ uint32_t v = (uint32_t)(0x2D1u); X86_PUSH32(v); }
	// 001F196A push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F196F mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F1972 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F1973 call 0x000127D0
	X86_PUSH32(0x1F1978u); sub_000127D0(ctx, base);
	// 001F1978 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F197B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F1960, __imp__sub_001F1960)

X86_FUNC(__imp__sub_001F197C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001F197C push 0x1001
	{ uint32_t v = (uint32_t)(0x1001u); X86_PUSH32(v); }
	// 001F1981 push 0x2E0
	{ uint32_t v = (uint32_t)(0x2E0u); X86_PUSH32(v); }
	// 001F1986 push 0x273BEC
	{ uint32_t v = (uint32_t)(0x273BECu); X86_PUSH32(v); }
	// 001F198B mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001F198E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001F198F call 0x000127D0
	X86_PUSH32(0x1F1994u); sub_000127D0(ctx, base);
	// 001F1994 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001F1997 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001F197C, __imp__sub_001F197C)

