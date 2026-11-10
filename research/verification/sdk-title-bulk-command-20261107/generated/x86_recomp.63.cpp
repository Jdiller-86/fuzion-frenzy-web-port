#include "x86_recomp_shared.h"

X86_FUNC(__imp__sub_002524EC) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002524EC push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 002524ED mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 002524EF sub esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002524F2 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 002524F3 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 002524F5 xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002524F7 cmp [ebx+0x434], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x434u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002524FD mov [ebp-0x08], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00252500 mov [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252503 jz 0x002525B6
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_002525B6; }
	// 00252509 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0025250A push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
loc_0025250B:
	// 0025250B mov esi, [ebx+0x434]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x434u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252511 mov eax, [esi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252514 mov [ebx+0x434], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x434u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0025251A mov edi, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0025251D mov eax, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252520 cmp eax, [edi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Cu); uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252523 jnb 0x0025252D
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_0025252D; }
loc_00252525:
	// 00252525 mov [esi+0x14], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252528 mov [ebp-0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0025252B jmp 0x002525A4
	goto loc_002525A4;
loc_0025252D:
	// 0025252D mov al, [edi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252530 test al, 0x40
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x40u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252532 jz 0x0025253B
	if (((uint8_t)(ca & cb)) == 0) { goto loc_0025253B; }
	// 00252534 and al, 0xBF
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0xFFFFFFBFu); uint8_t r = (uint8_t)(a & b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252536 mov [edi+0x10], al
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252539 jmp 0x00252525
	X86_BARRIER();
	goto loc_00252525;
loc_0025253B:
	// 0025253B cmp byte ptr [esi+0x01], 0x4A
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x4Au); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025253F mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 00252541 jnz 0x0025254C
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_0025254C; }
	// 00252543 mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 00252545 call 0x002528D7
	X86_PUSH32(0x25254Au); sub_002528D7(ctx, base);
	// 0025254A jmp 0x002525A4
	goto loc_002525A4;
loc_0025254C:
	// 0025254C mov edx, edi
	{ ctx.edx = (uint32_t)(ctx.edi); }
	// 0025254E call 0x0025240F
	X86_PUSH32(0x252553u); sub_0025240F(ctx, base);
	// 00252553 mov edx, [esi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252556 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252558 jz 0x0025258C
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0025258C; }
	// 0025255A mov ecx, [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025255C mov [ebp-0x0C], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0025255F shr ecx, 0x07
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x7u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 00252562 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252564 and ecx, 0x0F
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFu); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252567 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252568 shl eax, cl
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(((uint8_t)ctx.ecx)) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 0025256A mov ecx, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025256D and ecx, 0x1800
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x1800u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252573 cmp ecx, 0x1000
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x1000u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252579 jnz 0x0025257E
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_0025257E; }
	// 0025257B shl eax, 0x10
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x10u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
loc_0025257E:
	// 0025257E test byte ptr [edi+0x08], 0x02
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x8u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252582 jz 0x00252588
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252588; }
	// 00252584 or [edx], eax
	{ uint32_t ea = (uint32_t)(ctx.edx); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252586 jmp 0x0025258C
	goto loc_0025258C;
loc_00252588:
	// 00252588 not eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(~a); ctx.eax = (uint32_t)(r);  }
	// 0025258A and [edx], eax
	{ uint32_t ea = (uint32_t)(ctx.edx); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0025258C:
	// 0025258C mov eax, [0x003190A8]
	{ uint32_t ea = (uint32_t)(0x3190A8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252591 mov [edi+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252594 mov [0x003190A8], edi
	{ uint32_t ea = (uint32_t)(0x3190A8u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0025259A and dword ptr [esi+0x04], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025259E push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0025259F call 0x00250769
	X86_PUSH32(0x2525A4u); sub_00250769(ctx, base);
loc_002525A4:
	// 002525A4 cmp dword ptr [ebx+0x434], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x434u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525AB mov ecx, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002525AE jnz 0x0025250B
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_0025250B; }
	// 002525B4 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 002525B5 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
loc_002525B6:
	// 002525B6 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525B8 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525BA mov [ebx+0x434], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x434u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002525C0 setnz al
	{ ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)((((uint32_t)(ca & cb)) != 0) ? 1u : 0u); }
	// 002525C3 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 002525C4 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 002525C5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_002524EC, __imp__sub_002524EC)

X86_FUNC(__imp__sub_002525C6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002525C6 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 002525C7 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 002525C9 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 002525CA push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 002525CB push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 002525CC push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 002525CD mov esi, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 002525D0 and byte ptr [ebp+0x0F], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002525D4 mov ebx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
	// 002525D6 lea eax, [esi+0x438]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esi + 0x438u));
	// 002525DC mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002525DE push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 002525DF xor edi, edi
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a ^ b); ctx.edi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525E1 and [eax], edi
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525E3 test cl, 0x02
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002525E6 mov [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002525E9 jz 0x00252642
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252642; }
	// 002525EB mov ecx, [esi+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002525EE add ecx, 0x84
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x84u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525F4 mov eax, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002525F6 and eax, 0xFFFFFFF0
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFFFFF0u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002525F9 mov [ecx], edi
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 002525FB jz 0x0025262E
	if (zf) { goto loc_0025262E; }
loc_002525FD:
	// 002525FD mov ecx, [0x003190A0]
	{ uint32_t ea = (uint32_t)(0x3190A0u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252603 add ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252605 mov eax, [ecx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252608 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025260A mov [ecx+0x08], edi
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0025260D mov edi, ecx
	{ ctx.edi = (uint32_t)(ctx.ecx); }
	// 0025260F jnz 0x002525FD
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_002525FD; }
loc_00252611:
	// 00252611 mov edx, edi
	{ ctx.edx = (uint32_t)(ctx.edi); }
	// 00252613 test byte ptr [edx+0x02], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252617 mov edi, [edi+0x08]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0025261A mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 0025261C jz 0x00252625
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252625; }
	// 0025261E call 0x00252C40
	X86_PUSH32(0x252623u); sub_00252C40(ctx, base);
	// 00252623 jmp 0x0025262A
	goto loc_0025262A;
loc_00252625:
	// 00252625 call 0x00252261
	X86_PUSH32(0x25262Au); sub_00252261(ctx, base);
loc_0025262A:
	// 0025262A test edi, edi
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025262C jnz 0x00252611
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_00252611; }
loc_0025262E:
	// 0025262E and dword ptr [ebp-0x04], 0xFFFFFFFD
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0xFFFFFFFDu); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252632 mov dword ptr [ebx+0x0C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEM32(ea) = (uint32_t)(0x2u); }
	// 00252639 mov eax, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025263B mov dword ptr [eax+0x08], 0x06
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x6u); }
loc_00252642:
	// 00252642 test byte ptr [ebp-0x04], 0x04
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252646 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 00252648 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252649 jz 0x00252655
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252655; }
	// 0025264B and dword ptr [ebp-0x04], 0xFFFFFFFB
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0xFFFFFFFBu); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025264F mov [ebx+0x0C], edi
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00252652 mov [ebx+0x14], edi
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
loc_00252655:
	// 00252655 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 00252657 call 0x0025209B
	X86_PUSH32(0x25265Cu); sub_0025209B(ctx, base);
	// 0025265C mov edx, eax
	{ ctx.edx = (uint32_t)(ctx.eax); }
	// 0025265E mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 00252660 mov [ebp-0x08], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00252663 call 0x0025230D
	X86_PUSH32(0x252668u); sub_0025230D(ctx, base);
	// 00252668 test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025266A jz 0x00252670
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252670; }
	// 0025266C mov byte ptr [ebp+0x0F], 0x01
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFu); X86_MEM8(ea) = (uint8_t)(0x1u); }
loc_00252670:
	// 00252670 mov edx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252673 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 00252675 call 0x00252465
	X86_PUSH32(0x25267Au); sub_00252465(ctx, base);
	// 0025267A test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025267C jz 0x00252682
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252682; }
	// 0025267E mov byte ptr [ebp+0x0F], 0x01
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFu); X86_MEM8(ea) = (uint8_t)(0x1u); }
loc_00252682:
	// 00252682 mov edx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252685 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 00252687 call 0x002524EC
	X86_PUSH32(0x25268Cu); sub_002524EC(ctx, base);
	// 0025268C test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025268E jz 0x00252694
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252694; }
	// 00252690 mov byte ptr [ebp+0x0F], 0x01
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFu); X86_MEM8(ea) = (uint8_t)(0x1u); }
loc_00252694:
	// 00252694 cmp byte ptr [ebp+0x0F], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252698 jz 0x002526A4
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_002526A4; }
	// 0025269A mov eax, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025269C mov [eax+0x0C], edi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0025269F mov eax, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002526A1 mov [eax+0x10], edi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
loc_002526A4:
	// 002526A4 test byte ptr [ebp-0x04], 0x40
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x40u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002526A8 jz 0x002526BC
	if (((uint8_t)(ca & cb)) == 0) { goto loc_002526BC; }
	// 002526AA mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 002526AC call 0x002514DE
	X86_PUSH32(0x2526B1u); sub_002514DE(ctx, base);
	// 002526B1 and dword ptr [ebp-0x04], 0xFFFFFFBF
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0xFFFFFFBFu); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002526B5 mov dword ptr [ebx+0x0C], 0x40
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEM32(ea) = (uint32_t)(0x40u); }
loc_002526BC:
	// 002526BC mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002526BF test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002526C1 jz 0x002526C6
	if (((uint32_t)(ca & cb)) == 0) { goto loc_002526C6; }
	// 002526C3 mov [ebx+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_002526C6:
	// 002526C6 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 002526C7 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 002526C8 mov dword ptr [ebx+0x10], 0x80000000
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x10u); X86_MEM32(ea) = (uint32_t)(0x80000000u); }
	// 002526CF pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 002526D0 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 002526D1 ret 0x10
	ctx.esp += 20; return;
}
X86_WEAK_FUNC(sub_002525C6, __imp__sub_002525C6)

X86_FUNC(__imp__sub_002526D4) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002526D4 mov eax, [0x003190C8]
	{ uint32_t ea = (uint32_t)(0x3190C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002526D9 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002526DB jz 0x002526E5
	if (((uint32_t)(ca & cb)) == 0) { goto loc_002526E5; }
	// 002526DD mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002526DF mov [0x003190C8], ecx
	{ uint32_t ea = (uint32_t)(0x3190C8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
loc_002526E5:
	// 002526E5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_002526D4, __imp__sub_002526D4)

X86_FUNC(__imp__sub_002526E6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002526E6 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 002526E7 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 002526E9 sub esp, 0x14
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x14u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002526EC mov eax, [0x003190CC]
	{ uint32_t ea = (uint32_t)(0x3190CCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002526F1 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 002526F2 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 002526F3 mov ebx, edx
	{ ctx.ebx = (uint32_t)(ctx.edx); }
	// 002526F5 mov [ebp-0x14], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002526F8 mov [ebp-0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002526FB call 0x002526D4
	X86_PUSH32(0x252700u); sub_002526D4(ctx, base);
	// 00252700 mov edi, eax
	{ ctx.edi = (uint32_t)(ctx.eax); }
	// 00252702 test edi, edi
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252704 mov [ebp-0x0C], edi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00252707 jnz 0x00252713
	if (((uint32_t)(ca & cb)) != 0) { goto loc_00252713; }
	// 00252709 mov edi, 0x80000100
	{ ctx.edi = (uint32_t)(0x80000100u); }
	// 0025270E jmp 0x0025288A
	goto loc_0025288A;
loc_00252713:
	// 00252713 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252714 mov esi, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252717 shl esi, 0x06
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.esi = (uint32_t)(r); } }
	// 0025271A xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025271C lea ecx, [esi+0x30]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esi + 0x30u));
	// 0025271F mov edx, ecx
	{ ctx.edx = (uint32_t)(ctx.ecx); }
	// 00252721 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 00252724 rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 00252726 mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
	// 00252728 and ecx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025272B rep stosb
	{ x86_rep_stos(ctx, base, 1); }
	// 0025272D mov eax, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252730 add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252732 mov [esi+0x2C], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252735 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 00252737 sub eax, [0x003190A0]
	{ uint32_t ea = (uint32_t)(0x3190A0u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025273D mov byte ptr [esi+0x11], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x11u); X86_MEM8(ea) = (uint8_t)(0x1u); }
	// 00252741 mov [esi+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252744 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252746 mov byte ptr [esi+0x13], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x13u); X86_MEM8(ea) = (uint8_t)(0x1u); }
	// 0025274A mov ax, [ebx+0x16]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x16u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 0025274E push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00252750 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 00252752 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252753 call 0x0025077D
	X86_PUSH32(0x252758u); sub_0025077D(ctx, base);
	// 00252758 mov edx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0025275B mov [esi+0x22], ax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x22u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 0025275F mov [esi+0x24], dl
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 00252762 mov al, [ebx+0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x18u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252765 and al, 0x01
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252767 mov [esi+0x10], al
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 0025276A xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025276C mov al, [ebx+0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x14u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 0025276F push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00252771 xor eax, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252773 and eax, 0x7F
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x7Fu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252776 xor [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252778 movzx eax, byte ptr [ebx+0x15]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x15u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 0025277C mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025277E shl eax, 0x07
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x7u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252781 xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252783 and eax, 0x780
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x780u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252788 xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025278A mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0025278C test byte ptr [ebx+0x15], 0x80
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x15u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0xFFFFFF80u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252790 pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 00252791 setnz cl
	{ ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)((((uint8_t)(ca & cb)) != 0) ? 1u : 0u); }
	// 00252794 and eax, 0xFFFFC7FF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFFC7FFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252799 inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025279A and ecx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025279D or ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527A0 shl ecx, 0x0B
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0xBu) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 002527A3 or ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527A5 mov [esi], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002527A7 movzx eax, word ptr [ebx+0x16]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x16u); ctx.eax = (uint32_t)((uint16_t)(X86_MEM16(ea))); }
	// 002527AB shl eax, 0x10
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x10u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 002527AE xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527B0 and eax, 0x7FF0000
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x7FF0000u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527B5 xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527B7 mov ecx, [esi+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002527BA mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002527BC mov eax, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002527BF sub eax, [0x003190A0]
	{ uint32_t ea = (uint32_t)(0x3190A0u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527C5 xor ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527C7 and ecx, 0x0F
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFu); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527CA xor ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527CC mov [esi+0x08], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002527CF xor cl, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t r = (uint8_t)(a ^ b); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002527D1 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527D3 mov [esi+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002527D6 mov [ebp-0x01], cl
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 002527D9 jbe 0x00252838
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252838; }
	// 002527DB xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_002527DD:
	// 002527DD mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 002527E0 shl eax, 0x06
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 002527E3 lea edi, [eax+edx*1]
	ctx.edi = (uint32_t)((uint32_t)(ctx.eax + ctx.edx));
	// 002527E6 mov [ebp-0x10], edi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 002527E9 sub edi, [0x003190A0]
	{ uint32_t ea = (uint32_t)(0x3190A0u); uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.edi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527EF mov edx, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 002527F2 add edi, 0x40
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x40u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002527F5 dec cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx));  uint8_t r = (uint8_t)(a - 1); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002527F7 mov [edx+0x2D], cl
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 002527FA mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 002527FD mov cl, [ebp-0x01]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252800 mov [eax+edx*1+0x2C], cl
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0x2Cu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 00252804 mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252807 mov [eax+edx*1+0x20], esi
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0025280B mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0025280E and dword ptr [eax+edx*1+0x28], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0x28u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252813 mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252816 and dword ptr [eax+edx*1+0x24], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0x24u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025281B mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0025281E add edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252820 or byte ptr [edx+0x02], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a | b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252824 mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252827 inc cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx));  uint8_t r = (uint8_t)(a + 1); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); of = r == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252829 mov [eax+edx*1+0x08], edi
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0025282D movzx eax, cl
	{ ctx.eax = (uint32_t)((uint8_t)(((uint8_t)ctx.ecx))); }
	// 00252830 cmp eax, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252833 mov [ebp-0x01], cl
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 00252836 jb 0x002527DD
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_002527DD; }
loc_00252838:
	// 00252838 mov edx, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0025283B movzx eax, cl
	{ ctx.eax = (uint32_t)((uint8_t)(((uint8_t)ctx.ecx))); }
	// 0025283E shl eax, 0x06
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252841 and dword ptr [eax+edx*1-0x38], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0xFFFFFFC8u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252846 mov eax, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252849 dec cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx));  uint8_t r = (uint8_t)(a - 1); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025284B mov [eax+0x2D], cl
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 0025284E call [0x002534D4]
	X86_PUSH32(0x252854u); x86_kernel(ctx, base, 129);
	// 00252854 mov ecx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252857 mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 00252859 mov [ebp-0x01], al
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 0025285C call 0x00251CB7
	X86_PUSH32(0x252861u); sub_00251CB7(ctx, base);
	// 00252861 mov cl, [ebp-0x01]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252864 mov edi, eax
	{ ctx.edi = (uint32_t)(ctx.eax); }
	// 00252866 call [0x0025349C]
	X86_PUSH32(0x25286Cu); x86_kernel(ctx, base, 161);
	// 0025286C test edi, edi
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025286E jl 0x00252875
	if (((int32_t)(ca & cb)) < 0) { goto loc_00252875; }
	// 00252870 mov [ebx+0x10], esi
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252873 jmp 0x00252889
	goto loc_00252889;
loc_00252875:
	// 00252875 and dword ptr [ebx+0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x10u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252879 mov ecx, [0x003190C8]
	{ uint32_t ea = (uint32_t)(0x3190C8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025287F mov eax, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252882 mov [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252884 mov [0x003190C8], eax
	{ uint32_t ea = (uint32_t)(0x3190C8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_00252889:
	// 00252889 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
loc_0025288A:
	// 0025288A mov [ebx+0x04], edi
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0025288D mov eax, edi
	{ ctx.eax = (uint32_t)(ctx.edi); }
	// 0025288F pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252890 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00252891 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 00252892 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_002526E6, __imp__sub_002526E6)

X86_FUNC(__imp__sub_00252893) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252893 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00252894 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252895 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252896 mov esi, edx
	{ ctx.esi = (uint32_t)(ctx.edx); }
	// 00252898 mov ebp, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.ebp = (uint32_t)(X86_MEM32(ea)); }
	// 0025289B push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0025289C mov edi, ecx
	{ ctx.edi = (uint32_t)(ctx.ecx); }
	// 0025289E call [0x002534D4]
	X86_PUSH32(0x2528A4u); x86_kernel(ctx, base, 129);
	// 002528A4 mov edx, ebp
	{ ctx.edx = (uint32_t)(ctx.ebp); }
	// 002528A6 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 002528A8 mov bl, al
	{ ctx.ebx = (ctx.ebx & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ctx.eax)); }
	// 002528AA call 0x00251875
	X86_PUSH32(0x2528AFu); sub_00251875(ctx, base);
	// 002528AF mov edx, ebp
	{ ctx.edx = (uint32_t)(ctx.ebp); }
	// 002528B1 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 002528B3 call 0x00251E93
	X86_PUSH32(0x2528B8u); sub_00251E93(ctx, base);
	// 002528B8 lea eax, [edi+0x434]
	ctx.eax = (uint32_t)((uint32_t)(ctx.edi + 0x434u));
	// 002528BE mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002528C0 mov [esi+0x14], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002528C3 mov cl, bl
	{ ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ctx.ebx)); }
	// 002528C5 mov [eax], esi
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 002528C7 call [0x0025349C]
	X86_PUSH32(0x2528CDu); x86_kernel(ctx, base, 161);
	// 002528CD pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 002528CE pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 002528CF pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 002528D0 mov eax, 0x40000000
	{ ctx.eax = (uint32_t)(0x40000000u); }
	// 002528D5 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 002528D6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00252893, __imp__sub_00252893)

X86_FUNC(__imp__sub_002528D7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002528D7 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 002528D8 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 002528D9 mov ebp, edx
	{ ctx.ebp = (uint32_t)(ctx.edx); }
	// 002528DB push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 002528DC mov esi, [ebp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x10u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 002528DF mov al, [esi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x25u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 002528E2 movzx ebx, byte ptr [esi+0x26]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x26u); ctx.ebx = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 002528E6 movzx ecx, al
	{ ctx.ecx = (uint32_t)((uint8_t)(((uint8_t)ctx.eax))); }
	// 002528E9 sub ebx, ecx
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.ebx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002528EB movzx ecx, byte ptr [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.ecx = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 002528EF add ebx, ecx
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); ctx.ebx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002528F1 test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002528F3 jz 0x00252939
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252939; }
	// 002528F5 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
loc_002528F6:
	// 002528F6 movzx ecx, byte ptr [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.ecx = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 002528FA mov eax, ebx
	{ ctx.eax = (uint32_t)(ctx.ebx); }
	// 002528FC xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002528FE div ecx
	{ uint64_t n = ((uint64_t)ctx.edx << 32) | ctx.eax; uint32_t d = (uint32_t)ctx.ecx; if (!d) x86_trap(ctx, 0x2528FE, "divide error"); uint64_t q = n / d; if (q > 0xFFFFFFFFull) x86_trap(ctx, 0x2528FE, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 00252900 dec [esi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x25u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a - 1); X86_MEM8(ea) = (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252903 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 00252905 mov ebx, edx
	{ ctx.ebx = (uint32_t)(ctx.edx); }
	// 00252907 mov edi, ebx
	{ ctx.edi = (uint32_t)(ctx.ebx); }
	// 00252909 shl edi, 0x06
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.edi = (uint32_t)(r); } }
	// 0025290C add edi, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025290F inc ebx
	{ uint32_t a = (uint32_t)(ctx.ebx);  uint32_t r = (uint32_t)(a + 1); ctx.ebx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252910 push [edi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252913 call [0x00253518]
	X86_PUSH32(0x252919u); x86_kernel(ctx, base, 176);
	// 00252919 mov eax, [edi+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025291C mov ecx, [edi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025291F xor ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252921 test ecx, 0xFFFFF000
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0xFFFFF000u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252927 jz 0x00252932
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252932; }
	// 00252929 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 0025292B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0025292C call [0x00253518]
	X86_PUSH32(0x252932u); x86_kernel(ctx, base, 176);
loc_00252932:
	// 00252932 cmp byte ptr [esi+0x25], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x25u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252936 jnz 0x002528F6
	if ((uint8_t)ca != (uint8_t)cb) { X86_BARRIER(); goto loc_002528F6; }
	// 00252938 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
loc_00252939:
	// 00252939 movzx eax, byte ptr [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 0025293D dec [esi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x25u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a - 1); X86_MEM8(ea) = (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252940 shl eax, 0x06
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252943 sub esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252945 mov eax, [0x003190C8]
	{ uint32_t ea = (uint32_t)(0x3190C8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025294A mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0025294C mov [0x003190C8], esi
	{ uint32_t ea = (uint32_t)(0x3190C8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252952 and dword ptr [ebp+0x04], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252956 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252957 call 0x00250769
	X86_PUSH32(0x25295Cu); sub_00250769(ctx, base);
	// 0025295C pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0025295D pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 0025295E pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 0025295F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_002528D7, __imp__sub_002528D7)

X86_FUNC(__imp__sub_00252960) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252960 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252961 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 00252963 sub esp, 0x24
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252966 and dword ptr [ebp-0x14], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025296A push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0025296B mov esi, edx
	{ ctx.esi = (uint32_t)(ctx.edx); }
	// 0025296D push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0025296E mov edi, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252971 mov [ebp-0x10], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252974 mov [ebp-0x24], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252977 mov [ebp-0x20], edi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0025297A call [0x002534D4]
	X86_PUSH32(0x252980u); x86_kernel(ctx, base, 129);
	// 00252980 mov cl, [edi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x24u); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252983 cmp cl, [edi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x25u); uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252986 mov [ebp-0x01], al
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252989 jnz 0x00252997
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_00252997; }
	// 0025298B mov dword ptr [ebp-0x14], 0xC0000D00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(0xC0000D00u); }
	// 00252992 jmp 0x00252AAC
	goto loc_00252AAC;
loc_00252997:
	// 00252997 movzx eax, byte ptr [edi+0x26]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x26u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 0025299B push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 0025299C mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 0025299E shl ebx, 0x06
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ebx = (uint32_t)(r); } }
	// 002529A1 add ebx, [edi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2Cu); uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a + b); ctx.ebx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529A4 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529A5 cdq
	ctx.edx = (uint32_t)((int32_t)ctx.eax >> 31);
	// 002529A6 movzx ecx, cl
	{ ctx.ecx = (uint32_t)((uint8_t)(((uint8_t)ctx.ecx))); }
	// 002529A9 idiv ecx
	{ int64_t n = (int64_t)(((uint64_t)ctx.edx << 32) | ctx.eax); int32_t d = (int32_t)ctx.ecx; if (!d) x86_trap(ctx, 0x2529A9, "divide error"); if (n == INT64_MIN && d == -1) x86_trap(ctx, 0x2529A9, "divide error"); int64_t q = n / d; if (q != (int32_t)q) x86_trap(ctx, 0x2529A9, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 002529AB mov esi, [esi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 002529AE and dword ptr [ebp-0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529B2 mov [ebp-0x1C], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 002529B5 mov [edi+0x26], dl
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x26u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 002529B8 mov eax, [esi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002529BB mov [ebx+0x24], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002529BE mov eax, [esi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002529C1 mov [ebx+0x28], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002529C4 mov eax, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002529C7 mov [ebx+0x20], edi
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 002529CA movzx eax, byte ptr [eax+0x14]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 002529CE shl eax, 0x15
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x15u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 002529D1 xor eax, [ebx]
	{ uint32_t ea = (uint32_t)(ctx.ebx); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529D3 and eax, 0xE00000
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xE00000u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529D8 xor [ebx], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529DA mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 002529DC mov eax, [ebx]
	{ uint32_t ea = (uint32_t)(ctx.ebx); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002529DE dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529DF shl ecx, 0x18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x18u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 002529E2 xor ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529E4 and ecx, 0x7000000
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7000000u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529EA xor ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529EC mov [ebx], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebx); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 002529EE mov eax, [esi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002529F1 and eax, 0xFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529F6 cmp dword ptr [esi], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002529F9 mov [ebp-0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002529FC jbe 0x00252A29
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_00252A29; }
	// 002529FE lea ecx, [esi+0x08]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esi + 0x8u));
	// 00252A01 mov [ebp-0x08], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252A04 lea ecx, [ebx+0x10]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebx + 0x10u));
loc_00252A07:
	// 00252A07 mov edx, eax
	{ ctx.edx = (uint32_t)(ctx.eax); }
	// 00252A09 or dx, 0xE000
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.edx)); uint16_t b = (uint16_t)(0xFFFFE000u); uint16_t r = (uint16_t)(a | b); ctx.edx = (ctx.edx & 0xFFFF0000u) | (uint16_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252A0E mov [ecx], dx
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.edx)); }
	// 00252A11 mov edx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252A14 movzx edx, word ptr [edx]
	{ uint32_t ea = (uint32_t)(ctx.edx); ctx.edx = (uint32_t)((uint16_t)(X86_MEM16(ea))); }
	// 00252A17 add dword ptr [ebp-0x08], 0x02
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x2u); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A1B add eax, edx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A1D inc [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(X86_MEM32(ea));  uint32_t r = (uint32_t)(a + 1); X86_MEM32(ea) = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A20 mov edx, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252A23 inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A24 inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A25 cmp edx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A27 jb 0x00252A07
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_00252A07; }
loc_00252A29:
	// 00252A29 sub eax, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A2C push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00252A2E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252A2F push [esi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252A32 mov [ebp-0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252A35 call [0x002534B4]
	X86_PUSH32(0x252A3Bu); x86_kernel(ctx, base, 175);
	// 00252A3B push [esi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252A3E call [0x002534B8]
	X86_PUSH32(0x252A44u); x86_kernel(ctx, base, 173);
	// 00252A44 mov ecx, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252A47 mov [ebx+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252A4A mov eax, [esi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252A4D lea eax, [eax+ecx*1-0x01]
	ctx.eax = (uint32_t)((uint32_t)(ctx.eax + ctx.ecx + 0xFFFFFFFFu));
	// 00252A51 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252A52 call [0x002534B8]
	X86_PUSH32(0x252A58u); x86_kernel(ctx, base, 173);
	// 00252A58 mov [ebx+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252A5B test byte ptr [edi+0x10], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252A5F jz 0x00252A71
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252A71; }
	// 00252A61 lea esi, [ebx+0x10]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebx + 0x10u));
	// 00252A64 lea edi, [ebx+0x30]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ebx + 0x30u));
	// 00252A67 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252A68 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252A69 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252A6A movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252A6B mov esi, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252A6E mov edi, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
loc_00252A71:
	// 00252A71 test byte ptr [edi+0x10], 0x02
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252A75 jz 0x00252A97
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252A97; }
	// 00252A77 mov ecx, [ebp-0x24]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252A7A call 0x0025209B
	X86_PUSH32(0x252A7Fu); sub_0025209B(ctx, base);
	// 00252A7F mov ecx, [edi+0x28]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x28u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252A82 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A83 mov edx, ecx
	{ ctx.edx = (uint32_t)(ctx.ecx); }
	// 00252A85 sub edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A87 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A89 jle 0x00252A8D
	if (((int32_t)(ca & cb)) <= 0) { goto loc_00252A8D; }
	// 00252A8B mov eax, ecx
	{ ctx.eax = (uint32_t)(ctx.ecx); }
loc_00252A8D:
	// 00252A8D mov [ebx], ax
	{ uint32_t ea = (uint32_t)(ctx.ebx); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 00252A90 mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252A92 add ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252A94 mov [edi+0x28], ecx
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
loc_00252A97:
	// 00252A97 inc [edi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x25u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a + 1); X86_MEM8(ea) = (uint8_t)(r); of = r == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252A9A mov al, [edi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x25u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252A9D cmp al, [edi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x24u); uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252AA0 mov esi, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252AA3 jz 0x00252AAB
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_00252AAB; }
	// 00252AA5 mov eax, [ebx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252AA8 mov [edi+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_00252AAB:
	// 00252AAB pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
loc_00252AAC:
	// 00252AAC mov cl, [ebp-0x01]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252AAF call [0x0025349C]
	X86_PUSH32(0x252AB5u); x86_kernel(ctx, base, 161);
	// 00252AB5 mov edi, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252AB8 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252AB9 mov [esi+0x04], edi
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00252ABC call 0x00250769
	X86_PUSH32(0x252AC1u); sub_00250769(ctx, base);
	// 00252AC1 mov eax, edi
	{ ctx.eax = (uint32_t)(ctx.edi); }
	// 00252AC3 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252AC4 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00252AC5 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 00252AC6 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00252960, __imp__sub_00252960)

X86_FUNC(__imp__sub_00252AC7) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252AC7 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252AC8 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 00252ACA sub esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252ACD and dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252AD1 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00252AD2 mov ebx, [0x002534D4]
	{ uint32_t ea = (uint32_t)(0x2534D4u); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
	// 00252AD8 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252AD9 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252ADA mov edi, edx
	{ ctx.edi = (uint32_t)(ctx.edx); }
	// 00252ADC mov esi, [edi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252ADF mov [ebp-0x0C], edi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00252AE2 mov [ebp-0x08], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252AE5 call ebx
	{ uint32_t t = ctx.ebx; X86_PUSH32(0x252AE7u); x86_icall(ctx, base, t); }
	// 00252AE7 test byte ptr [esi+0x10], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252AEB mov [ebp-0x01], al
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252AEE jz 0x00252AFA
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252AFA; }
	// 00252AF0 mov esi, 0xC0000E00
	{ ctx.esi = (uint32_t)(0xC0000E00u); }
	// 00252AF5 jmp 0x00252BC6
	goto loc_00252BC6;
loc_00252AFA:
	// 00252AFA mov ecx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252AFD call 0x0025209B
	X86_PUSH32(0x252B02u); sub_0025209B(ctx, base);
	// 00252B02 mov dl, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252B05 test dl, 0x04
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B08 jz 0x00252B44
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252B44; }
	// 00252B0A and dl, 0xFB
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)); uint8_t b = (uint8_t)(0xFFFFFFFBu); uint8_t r = (uint8_t)(a & b); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B0D cmp [esi+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252B10 mov [esi+0x10], dl
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 00252B13 jnz 0x00252B3C
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_00252B3C; }
	// 00252B15 mov cl, [ebp-0x01]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252B18 call [0x0025349C]
	X86_PUSH32(0x252B1Eu); x86_kernel(ctx, base, 161);
	// 00252B1E or dword ptr [ebp-0x14], 0xFFFFFFFF
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a | b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252B22 lea eax, [ebp-0x18]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFE8u));
	// 00252B25 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252B26 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00252B28 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00252B2A mov dword ptr [ebp-0x18], 0xFFFFD8F0
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); X86_MEM32(ea) = (uint32_t)(0xFFFFD8F0u); }
	// 00252B31 call [0x002533A8]
	X86_PUSH32(0x252B37u); x86_kernel(ctx, base, 99);
	// 00252B37 call ebx
	{ uint32_t t = ctx.ebx; X86_PUSH32(0x252B39u); x86_icall(ctx, base, t); }
	// 00252B39 mov [ebp-0x01], al
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
loc_00252B3C:
	// 00252B3C mov ecx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252B3F call 0x0025209B
	X86_PUSH32(0x252B44u); sub_0025209B(ctx, base);
loc_00252B44:
	// 00252B44 test byte ptr [esi+0x10], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B48 jz 0x00252B59
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252B59; }
	// 00252B4A mov cl, [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252B4D cmp cl, [esi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x25u); uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)), b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B50 jz 0x00252B59
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_00252B59; }
	// 00252B52 mov esi, 0xC0001000
	{ ctx.esi = (uint32_t)(0xC0001000u); }
	// 00252B57 jmp 0x00252BC6
	goto loc_00252BC6;
loc_00252B59:
	// 00252B59 test byte ptr [edi+0x18], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x18u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B5D lea ecx, [eax+0x01]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.eax + 0x1u));
	// 00252B60 jnz 0x00252B74
	if (((uint8_t)(ca & cb)) != 0) { goto loc_00252B74; }
	// 00252B62 mov edx, [edi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x14u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252B65 mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 00252B67 sub eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252B69 js 0x00252BDF
	if (sf) { goto loc_00252BDF; }
	// 00252B6B cmp eax, 0x400
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x400u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252B70 jnle 0x00252BDF
	if ((int32_t)ca > (int32_t)cb) { goto loc_00252BDF; }
	// 00252B72 mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
loc_00252B74:
	// 00252B74 mov dl, [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252B77 mov al, dl
	{ ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ctx.edx)); }
	// 00252B79 sub al, [esi+0x25]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x25u); uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B7C movzx edi, dl
	{ ctx.edi = (uint32_t)((uint8_t)(((uint8_t)ctx.edx))); }
	// 00252B7F add al, [esi+0x26]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x26u); uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252B82 movzx eax, al
	{ ctx.eax = (uint32_t)((uint8_t)(((uint8_t)ctx.eax))); }
	// 00252B85 cdq
	ctx.edx = (uint32_t)((int32_t)ctx.eax >> 31);
	// 00252B86 idiv edi
	{ int64_t n = (int64_t)(((uint64_t)ctx.edx << 32) | ctx.eax); int32_t d = (int32_t)ctx.edi; if (!d) x86_trap(ctx, 0x252B86, "divide error"); if (n == INT64_MIN && d == -1) x86_trap(ctx, 0x252B86, "divide error"); int64_t q = n / d; if (q != (int32_t)q) x86_trap(ctx, 0x252B86, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 00252B88 mov edi, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
loc_00252B8B:
	// 00252B8B movzx edx, dl
	{ ctx.edx = (uint32_t)((uint8_t)(((uint8_t)ctx.edx))); }
	// 00252B8E mov eax, edx
	{ ctx.eax = (uint32_t)(ctx.edx); }
	// 00252B90 shl eax, 0x06
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252B93 mov [edi+eax*1], cx
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.eax); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 00252B97 mov edi, [esi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252B9A movzx eax, byte ptr [edi+eax*1+0x03]
	{ uint32_t ea = (uint32_t)(ctx.edi + ctx.eax + 0x3u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00252B9F movzx ebx, byte ptr [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.ebx = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00252BA3 and eax, 0x07
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x7u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252BA6 lea ecx, [ecx+eax*1+0x01]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx + ctx.eax + 0x1u));
	// 00252BAA lea eax, [edx+0x01]
	ctx.eax = (uint32_t)((uint32_t)(ctx.edx + 0x1u));
	// 00252BAD cdq
	ctx.edx = (uint32_t)((int32_t)ctx.eax >> 31);
	// 00252BAE idiv ebx
	{ int64_t n = (int64_t)(((uint64_t)ctx.edx << 32) | ctx.eax); int32_t d = (int32_t)ctx.ebx; if (!d) x86_trap(ctx, 0x252BAE, "divide error"); if (n == INT64_MIN && d == -1) x86_trap(ctx, 0x252BAE, "divide error"); int64_t q = n / d; if (q != (int32_t)q) x86_trap(ctx, 0x252BAE, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 00252BB0 cmp dl, [esi+0x26]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x26u); uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252BB3 jnz 0x00252B8B
	if ((uint8_t)ca != (uint8_t)cb) { X86_BARRIER(); goto loc_00252B8B; }
	// 00252BB5 and byte ptr [esi+0x01], 0xBF
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0xFFFFFFBFu); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252BB9 or byte ptr [esi+0x10], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a | b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252BBD mov edi, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252BC0 mov [esi+0x28], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252BC3 mov esi, [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
loc_00252BC6:
	// 00252BC6 mov cl, [ebp-0x01]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252BC9 call [0x0025349C]
	X86_PUSH32(0x252BCFu); x86_kernel(ctx, base, 161);
	// 00252BCF push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252BD0 mov [edi+0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252BD3 call 0x00250769
	X86_PUSH32(0x252BD8u); sub_00250769(ctx, base);
	// 00252BD8 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252BD9 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 00252BDB pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00252BDC pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00252BDD leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 00252BDE ret
	ctx.esp += 4; return;
loc_00252BDF:
	// 00252BDF mov esi, 0xC0000B00
	{ ctx.esi = (uint32_t)(0xC0000B00u); }
	// 00252BE4 jmp 0x00252BC6
	X86_BARRIER();
	goto loc_00252BC6;
}
X86_WEAK_FUNC(sub_00252AC7, __imp__sub_00252AC7)

X86_FUNC(__imp__sub_00252BE6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252BE6 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 00252BE7 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00252BE8 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252BE9 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252BEA push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252BEB mov edi, edx
	{ ctx.edi = (uint32_t)(ctx.edx); }
	// 00252BED mov esi, [edi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252BF0 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 00252BF2 xor ebp, ebp
	{ uint32_t a = (uint32_t)(ctx.ebp); uint32_t b = (uint32_t)(ctx.ebp); uint32_t r = (uint32_t)(a ^ b); ctx.ebp = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252BF4 call [0x002534D4]
	X86_PUSH32(0x252BFAu); x86_kernel(ctx, base, 129);
	// 00252BFA test byte ptr [esi+0x10], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252BFE mov [esp+0x13], al
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x13u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252C02 jz 0x00252C20
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252C20; }
	// 00252C04 or byte ptr [esi+0x01], 0x40
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x40u); uint8_t r = (uint8_t)(a | b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252C08 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 00252C0A call 0x0025209B
	X86_PUSH32(0x252C0Fu); sub_0025209B(ctx, base);
	// 00252C0F inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C10 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C11 mov [esi+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252C14 mov al, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252C17 and al, 0xFD
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0xFFFFFFFDu); uint8_t r = (uint8_t)(a & b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252C19 or al, 0x04
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a | b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252C1B mov [esi+0x10], al
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252C1E jmp 0x00252C25
	goto loc_00252C25;
loc_00252C20:
	// 00252C20 mov ebp, 0xC0000F00
	{ ctx.ebp = (uint32_t)(0xC0000F00u); }
loc_00252C25:
	// 00252C25 mov cl, [esp+0x13]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x13u); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252C29 call [0x0025349C]
	X86_PUSH32(0x252C2Fu); x86_kernel(ctx, base, 161);
	// 00252C2F push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252C30 mov [edi+0x04], ebp
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ebp); }
	// 00252C33 call 0x00250769
	X86_PUSH32(0x252C38u); sub_00250769(ctx, base);
	// 00252C38 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252C39 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00252C3A mov eax, ebp
	{ ctx.eax = (uint32_t)(ctx.ebp); }
	// 00252C3C pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 00252C3D pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00252C3E pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 00252C3F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00252BE6, __imp__sub_00252BE6)

X86_FUNC(__imp__sub_00252C40) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252C40 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252C41 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 00252C43 sub esp, 0x2C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x2Cu); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C46 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00252C47 mov ebx, edx
	{ ctx.ebx = (uint32_t)(ctx.edx); }
	// 00252C49 mov eax, [ebx]
	{ uint32_t ea = (uint32_t)(ctx.ebx); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252C4B push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252C4C mov edx, [ebx+0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x20u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252C4F and dword ptr [ebx+0x08], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C53 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 00252C55 shr esi, 0x1C
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t c = (uint32_t)(0x1Cu) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.esi = (uint32_t)(r); } }
	// 00252C58 mov [ebp-0x2C], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD4u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252C5B shr eax, 0x18
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x18u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252C5E and eax, 0x07
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x7u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C61 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252C62 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C63 mov [ebp-0x28], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252C66 mov eax, [ebx+0x28]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252C69 lea esi, [ebx+0x10]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebx + 0x10u));
	// 00252C6C mov [ebp-0x08], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252C6F lea edi, [ebp-0x24]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFDCu));
	// 00252C72 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252C73 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252C74 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252C75 mov [ebp-0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252C78 mov eax, [ebx+0x24]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x24u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252C7B movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252C7C mov edi, [edx+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2Cu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252C7F mov [ebp-0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252C82 movzx eax, byte ptr [ebx+0x2D]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x2Du); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00252C86 mov esi, ebx
	{ ctx.esi = (uint32_t)(ctx.ebx); }
	// 00252C88 sub esi, [0x003190A0]
	{ uint32_t ea = (uint32_t)(0x3190A0u); uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252C8E shl eax, 0x06
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x6u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252C91 mov [eax+edi*1+0x08], esi
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edi + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252C95 test byte ptr [edx+0x10], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x10u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252C99 mov [ebp-0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00252C9C mov [ebp-0x0C], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252C9F jz 0x00252CE7
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252CE7; }
	// 00252CA1 call 0x0025209B
	X86_PUSH32(0x252CA6u); sub_0025209B(ctx, base);
	// 00252CA6 mov ecx, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252CA9 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 00252CAB mov eax, [ecx+0x28]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252CAE inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252CAF mov edx, eax
	{ ctx.edx = (uint32_t)(ctx.eax); }
	// 00252CB1 sub edx, esi
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252CB3 js 0x00252CB7
	if (sf) { goto loc_00252CB7; }
	// 00252CB5 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
loc_00252CB7:
	// 00252CB7 mov edi, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252CBA mov [ebx], si
	{ uint32_t ea = (uint32_t)(ctx.ebx); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.esi)); }
	// 00252CBD xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252CBF mov al, [ebx+0x03]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x3u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252CC2 and eax, 0x07
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x7u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252CC5 lea eax, [eax+esi*1+0x01]
	ctx.eax = (uint32_t)((uint32_t)(ctx.eax + ctx.esi + 0x1u));
	// 00252CC9 mov [ecx+0x28], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252CCC lea esi, [ebx+0x30]
	ctx.esi = (uint32_t)((uint32_t)(ctx.ebx + 0x30u));
	// 00252CCF movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252CD0 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252CD1 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252CD2 movsd
	{ int32_t d = ctx.df ? -4 : 4; X86_MEM32(ctx.edi) = X86_MEM32(ctx.esi); ctx.esi += d; ctx.edi += d; }
	// 00252CD3 movzx eax, byte ptr [ecx+0x26]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x26u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00252CD7 movzx esi, byte ptr [ecx+0x24]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x24u); ctx.esi = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00252CDB inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252CDC cdq
	ctx.edx = (uint32_t)((int32_t)ctx.eax >> 31);
	// 00252CDD idiv esi
	{ int64_t n = (int64_t)(((uint64_t)ctx.edx << 32) | ctx.eax); int32_t d = (int32_t)ctx.esi; if (!d) x86_trap(ctx, 0x252CDD, "divide error"); if (n == INT64_MIN && d == -1) x86_trap(ctx, 0x252CDD, "divide error"); int64_t q = n / d; if (q != (int32_t)q) x86_trap(ctx, 0x252CDD, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 00252CDF mov esi, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252CE2 mov [ecx+0x26], dl
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x26u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 00252CE5 jmp 0x00252D1E
	goto loc_00252D1E;
loc_00252CE7:
	// 00252CE7 mov edi, [0x00253518]
	{ uint32_t ea = (uint32_t)(0x253518u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252CED push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 00252CEF push [ebx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252CF2 call edi
	{ uint32_t t = ctx.edi; X86_PUSH32(0x252CF4u); x86_icall(ctx, base, t); }
	// 00252CF4 mov eax, [ebx+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0xCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252CF7 mov ecx, [ebx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252CFA xor ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252CFC test ecx, 0xFFFFF000
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0xFFFFF000u); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D02 jz 0x00252D09
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252D09; }
	// 00252D04 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 00252D06 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252D07 call edi
	{ uint32_t t = ctx.edi; X86_PUSH32(0x252D09u); x86_icall(ctx, base, t); }
loc_00252D09:
	// 00252D09 mov ecx, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252D0C mov al, [ecx+0x25]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x25u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252D0F cmp al, [ecx+0x24]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x24u); uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252D12 setz dl
	{ ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ca == (uint8_t)cb) ? 1u : 0u); }
	// 00252D15 dec al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax));  uint8_t r = (uint8_t)(a - 1); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252D17 test dl, dl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(((uint8_t)ctx.edx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252D19 mov [ecx+0x25], al
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x25u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252D1C jz 0x00252D21
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00252D21; }
loc_00252D1E:
	// 00252D1E mov [ecx+0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
loc_00252D21:
	// 00252D21 push [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252D24 lea eax, [ebp-0x2C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFD4u));
	// 00252D27 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252D28 call [ebp-0x14]
	{ uint32_t t = X86_MEM32((uint32_t)(ctx.ebp + 0xFFFFFFECu)); X86_PUSH32(0x252D2Bu); x86_icall(ctx, base, t); }
	// 00252D2B pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252D2C pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00252D2D pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00252D2E leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 00252D2F ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00252C40, __imp__sub_00252C40)

X86_FUNC(__imp__sub_00252D30) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252D30 mov eax, [0x003190AC]
	{ uint32_t ea = (uint32_t)(0x3190ACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252D35 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D37 jz 0x00252D42
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252D42; }
	// 00252D39 mov ecx, [eax+0x14]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252D3C mov [0x003190AC], ecx
	{ uint32_t ea = (uint32_t)(0x3190ACu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
loc_00252D42:
	// 00252D42 mov cl, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252D46 and byte ptr [eax+0x02], 0xFE
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0xFFFFFFFEu); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252D4A mov [eax+0x1F], cl
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Fu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 00252D4D ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00252D30, __imp__sub_00252D30)

X86_FUNC(__imp__sub_00252D50) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252D50 mov ax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00252D55 cmp [0x003190C2], ax
	{ uint32_t ea = (uint32_t)(0x3190C2u); uint16_t a = (uint16_t)(X86_MEM16(ea)), b = (uint16_t)(((uint16_t)ctx.eax)); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252D5C jnb 0x00252D62
	if ((uint16_t)ca >= (uint16_t)cb) { goto loc_00252D62; }
	// 00252D5E xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D60 jmp 0x00252D6C
	goto loc_00252D6C;
loc_00252D62:
	// 00252D62 sub [0x003190C2], ax
	{ uint32_t ea = (uint32_t)(0x3190C2u); uint16_t a = (uint16_t)(X86_MEM16(ea)); uint16_t b = (uint16_t)(((uint16_t)ctx.eax)); uint16_t r = (uint16_t)(a - b); X86_MEM16(ea) = (uint16_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252D69 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D6B inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00252D6C:
	// 00252D6C ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00252D50, __imp__sub_00252D50)

X86_FUNC(__imp__sub_00252D6F) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252D6F mov ax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00252D74 cmp [0x003190C6], ax
	{ uint32_t ea = (uint32_t)(0x3190C6u); uint16_t a = (uint16_t)(X86_MEM16(ea)), b = (uint16_t)(((uint16_t)ctx.eax)); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252D7B jnb 0x00252D81
	if ((uint16_t)ca >= (uint16_t)cb) { goto loc_00252D81; }
	// 00252D7D xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D7F jmp 0x00252D8B
	goto loc_00252D8B;
loc_00252D81:
	// 00252D81 sub [0x003190C6], ax
	{ uint32_t ea = (uint32_t)(0x3190C6u); uint16_t a = (uint16_t)(X86_MEM16(ea)); uint16_t b = (uint16_t)(((uint16_t)ctx.eax)); uint16_t r = (uint16_t)(a - b); X86_MEM16(ea) = (uint16_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252D88 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D8A inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00252D8B:
	// 00252D8B ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00252D6F, __imp__sub_00252D6F)

X86_FUNC(__imp__sub_00252D8E) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252D8E mov ecx, [ecx+0x14]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252D91 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252D92 mov esi, edx
	{ ctx.esi = (uint32_t)(ctx.edx); }
	// 00252D94 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D96 mov dx, [esi+0x02]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); ctx.edx = (ctx.edx & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00252D9A xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252D9C and edx, 0x7FF
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x7FFu); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DA2 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DA4 jz 0x00252DB6
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252DB6; }
	// 00252DA6 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252DA7 movzx edi, dx
	{ ctx.edi = (uint32_t)((uint16_t)(((uint16_t)ctx.edx))); }
	// 00252DAA xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DAC mov eax, ecx
	{ ctx.eax = (uint32_t)(ctx.ecx); }
	// 00252DAE div edi
	{ uint64_t n = ((uint64_t)ctx.edx << 32) | ctx.eax; uint32_t d = (uint32_t)ctx.edi; if (!d) x86_trap(ctx, 0x252DAE, "divide error"); uint64_t q = n / d; if (q > 0xFFFFFFFFull) x86_trap(ctx, 0x252DAE, "divide error"); ctx.eax = (uint32_t)q; ctx.edx = (uint32_t)(n % d); }
	// 00252DB0 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252DB1 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DB3 jz 0x00252DB6
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252DB6; }
	// 00252DB5 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00252DB6:
	// 00252DB6 cmp byte ptr [esi+0x11], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x11u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252DBA pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00252DBB jnz 0x00252DC0
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_00252DC0; }
	// 00252DBD add eax, 0x03
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00252DC0:
	// 00252DC0 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00252D8E, __imp__sub_00252D8E)

X86_FUNC(__imp__sub_00252DC1) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252DC1 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00252DC2 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252DC3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252DC4 mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 00252DC6 push [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252DC8 mov edi, edx
	{ ctx.edi = (uint32_t)(ctx.edx); }
	// 00252DCA call [0x002534B8]
	X86_PUSH32(0x252DD0u); x86_kernel(ctx, base, 173);
	// 00252DD0 mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252DD2 and ecx, 0xFFF
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFFFu); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DD8 mov edx, 0x1000
	{ ctx.edx = (uint32_t)(0x1000u); }
	// 00252DDD sub edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DDF mov ecx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252DE3 mov [ecx], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00252DE5 mov ebx, [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
	// 00252DE7 cmp edx, ebx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DE9 jbe 0x00252DED
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_00252DED; }
	// 00252DEB mov [ecx], ebx
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
loc_00252DED:
	// 00252DED mov edx, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252DEF sub [edi], edx
	{ uint32_t ea = (uint32_t)(ctx.edi); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a - b); X86_MEM32(ea) = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DF1 mov ecx, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252DF3 add [esi], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252DF5 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00252DF6 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00252DF7 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00252DF8 ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00252DC1, __imp__sub_00252DC1)

X86_FUNC(__imp__sub_00252DFB) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00252DFB push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00252DFC mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 00252DFE sub esp, 0x28
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x28u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E01 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E03 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00252E04 mov ebx, edx
	{ ctx.ebx = (uint32_t)(ctx.edx); }
	// 00252E06 mov ax, [ebx+0x02]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x2u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00252E0A push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00252E0B push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00252E0C mov edi, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 00252E0F mov [ebp-0x20], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252E12 mov cl, [ecx+0x45C]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x45Cu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252E18 mov [ebp-0x18], cl
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 00252E1B and eax, 0x7FF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x7FFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E20 mov [ebp-0x28], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E23 mov eax, [edi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252E26 mov [ebp-0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E29 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E2B and [ebp+0x0B], al
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xBu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252E2E dec [ebx+0x26]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x26u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a - 1); X86_MEM8(ea) = (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252E31 inc [ebx+0x27]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x27u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a + 1); X86_MEM8(ea) = (uint8_t)(r); of = r == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252E34 mov cx, [edi+0x22]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x22u); ctx.ecx = (ctx.ecx & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00252E38 and cx, 0xFFFD
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.ecx)); uint16_t b = (uint16_t)(0xFFFFFFFDu); uint16_t r = (uint16_t)(a & b); ctx.ecx = (ctx.ecx & 0xFFFF0000u) | (uint16_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252E3D or cx, 0x04
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.ecx)); uint16_t b = (uint16_t)(0x4u); uint16_t r = (uint16_t)(a | b); ctx.ecx = (ctx.ecx & 0xFFFF0000u) | (uint16_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00252E41 mov [edi+0x22], cx
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x22u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 00252E45 mov esi, [ebx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 00252E48 cmp esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E4A mov [ebp-0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E4D mov [ebp-0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E50 mov [ebp-0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E53 mov [ebp-0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E56 jz 0x00252E61
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00252E61; }
	// 00252E58 mov eax, [0x003190A0]
	{ uint32_t ea = (uint32_t)(0x3190A0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252E5D add esi, eax
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.esi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E5F jmp 0x00252E7A
	goto loc_00252E7A;
loc_00252E61:
	// 00252E61 push [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252E64 call 0x00252D30
	X86_PUSH32(0x252E69u); sub_00252D30(ctx, base);
	// 00252E69 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 00252E6B mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252E6E xor eax, [ebx+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E71 and eax, 0x0F
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E74 xor eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E77 mov [ebx+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_00252E7A:
	// 00252E7A cmp byte ptr [ebx+0x11], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x11u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252E7E mov [ebp-0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252E81 jnz 0x00252EE9
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_00252EE9; }
	// 00252E83 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252E85 mov al, 0xFE
	{ ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(0xFFFFFFFEu); }
	// 00252E87 sub al, [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(X86_MEM8(ea)); uint8_t r = (uint8_t)(a - b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252E8A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252E8B call 0x00252D30
	X86_PUSH32(0x252E90u); sub_00252D30(ctx, base);
	// 00252E90 mov ecx, [edi+0x28]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x28u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252E93 push [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252E96 mov [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252E98 mov ecx, [edi+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x2Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252E9B mov [ebp-0x24], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252E9E mov [eax+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252EA1 call 0x00252D30
	X86_PUSH32(0x252EA6u); sub_00252D30(ctx, base);
	// 00252EA6 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 00252EA8 mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252EAB mov ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252EAD and ecx, 0x3FFFF
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3FFFFu); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252EB3 or ecx, 0xE2E00000
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xE2E00000u); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252EB9 mov [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252EBB mov ecx, [ebp-0x24]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252EBE mov edx, [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252EC1 mov [eax+0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00252EC4 mov edx, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252EC7 mov [eax+0x08], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00252ECA mov ecx, [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252ECD and byte ptr [eax+0x1C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252ED1 add ecx, 0x07
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7u); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252ED4 and byte ptr [eax+0x1D], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Du); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252ED8 mov byte ptr [ebp+0x0B], 0x02
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xBu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 00252EDC mov [eax+0x0C], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252EDF mov [eax+0x14], ebx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 00252EE2 mov byte ptr [eax+0x1E], 0x01
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Eu); X86_MEM8(ea) = (uint8_t)(0x1u); }
	// 00252EE6 mov [eax+0x18], edi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
loc_00252EE9:
	// 00252EE9 cmp dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252EED jz 0x00252F05
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00252F05; }
	// 00252EEF push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00252EF1 push [ebp-0x10]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252EF4 push [edi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x18u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252EF7 call [0x002534B4]
	X86_PUSH32(0x252EFDu); x86_kernel(ctx, base, 175);
	// 00252EFD mov eax, [edi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x18u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F00 mov [ebp-0x24], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFDCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252F03 jmp 0x00252F09
	goto loc_00252F09;
loc_00252F05:
	// 00252F05 mov byte ptr [edi+0x1C], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Cu); X86_MEM8(ea) = (uint8_t)(0x1u); }
loc_00252F09:
	// 00252F09 cmp dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F0D jz 0x00253024
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00253024; }
loc_00252F13:
	// 00252F13 lea eax, [ebp-0x14]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFECu));
	// 00252F16 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252F17 lea edx, [ebp-0x10]
	ctx.edx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF0u));
	// 00252F1A lea ecx, [ebp-0x24]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFDCu));
	// 00252F1D call 0x00252DC1
	X86_PUSH32(0x252F22u); sub_00252DC1(ctx, base);
	// 00252F22 mov [ebp-0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252F25 mov edx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252F28 mov eax, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F2B mov ecx, [ebp-0x28]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252F2E add eax, edx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F30 cmp eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F32 jnb 0x00252F46
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_00252F46; }
	// 00252F34 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F36 jz 0x00252FFE
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252FFE; }
	// 00252F3C cmp dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F40 jnz 0x00252FFE
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_00252FFE; }
loc_00252F46:
	// 00252F46 cmp dword ptr [ebp-0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F4A jz 0x00252F6E
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00252F6E; }
	// 00252F4C sub ecx, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F4F mov eax, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F52 cmp edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F54 mov [esi+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252F57 jnb 0x00252F5B
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_00252F5B; }
	// 00252F59 mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
loc_00252F5B:
	// 00252F5B mov al, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252F5E add [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F61 add al, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252F63 sub edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F65 and dword ptr [ebp-0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F69 mov [esi+0x1D], al
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252F6C jmp 0x00252F8C
	goto loc_00252F8C;
loc_00252F6E:
	// 00252F6E cmp edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F70 mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F73 mov [esi+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252F76 jnb 0x00252F84
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_00252F84; }
	// 00252F78 add [ebp-0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F7B and dword ptr [ebp-0x14], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F7F mov [esi+0x1D], dl
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 00252F82 jmp 0x00252F8F
	goto loc_00252F8F;
loc_00252F84:
	// 00252F84 add [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F87 mov [esi+0x1D], cl
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 00252F8A sub edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00252F8C:
	// 00252F8C mov [ebp-0x14], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_00252F8F:
	// 00252F8F mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F92 mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252F94 xor byte ptr [ebp+0x0B], 0x01
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xBu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a ^ b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252F98 and byte ptr [esi+0x1C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252F9C and byte ptr [esi+0x1E], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Eu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252FA0 dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FA1 mov [esi+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252FA4 movzx eax, byte ptr [ebp+0x0B]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xBu); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	{ X86_SAVE_FLAGS(); sub_00252FA8(ctx, base); return; }
loc_00252FFE:
	// 00252FFE cmp dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253002 mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00253005 mov [ebp-0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253008 mov [ebp-0x0C], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0025300B jnz 0x00252F13
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_00252F13; }
	// 00253011 cmp dword ptr [ebp-0x08], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253015 jz 0x00253024
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00253024; }
	// 00253017 cmp byte ptr [edi+0x1D], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Du); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025301B jz 0x00253024
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_00253024; }
	// 0025301D mov eax, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00253020 or byte ptr [eax+0x02], 0x04
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a | b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_00253024:
	// 00253024 cmp byte ptr [ebx+0x11], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x11u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00253028 jnz 0x00253094
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_00253094; }
	// 0025302A and byte ptr [esi+0x02], 0xFB
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0xFFFFFFFBu); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025302E mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00253030 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253032 cmp byte ptr [edi+0x1C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Cu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00253036 push [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00253039 setnz al
	{ ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ca != (uint8_t)cb) ? 1u : 0u); }
	// 0025303C mov [ebp-0x08], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0025303F inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253040 shl eax, 0x13
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x13u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00253043 xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253045 and eax, 0x180000
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x180000u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025304A xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025304C xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025304E mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253050 mov cl, [edi+0x1E]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Eu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00253053 and dword ptr [esi+0x04], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253057 and dword ptr [esi+0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025305B and byte ptr [esi+0x1D], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025305F and eax, 0x1FFFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x1FFFFFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253064 mov [esi+0x14], ebx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 00253067 mov byte ptr [esi+0x1C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 0025306B and ecx, 0x07
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025306E or ecx, 0xFFFFFF18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFFFFFF18u); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253074 shl ecx, 0x15
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x15u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 00253077 or ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253079 mov [esi], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0025307B mov byte ptr [esi+0x1E], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Eu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 0025307F mov [esi+0x18], edi
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00253082 call 0x00252D30
	X86_PUSH32(0x253087u); sub_00252D30(ctx, base);
	// 00253087 mov ecx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025308A mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 0025308C mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025308F mov [ecx+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253092 jmp 0x002530AC
	goto loc_002530AC;
loc_00253094:
	// 00253094 movzx ecx, byte ptr [edi+0x1E]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Eu); ctx.ecx = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00253098 mov eax, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025309B shl ecx, 0x15
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x15u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 0025309E xor ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002530A0 mov byte ptr [eax+0x1C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 002530A4 and ecx, 0xE00000
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xE00000u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002530AA xor [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_002530AC:
	// 002530AC mov byte ptr [esi+0x1E], 0x03
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Eu); X86_MEM8(ea) = (uint8_t)(0x3u); }
	// 002530B0 mov ax, [edi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x14u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 002530B4 and dword ptr [edi+0x14], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x14u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002530B8 mov [edi+0x20], ax
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x20u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 002530BC cmp byte ptr [ebx+0x20], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x20u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002530C0 mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530C3 mov [ebx+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002530C6 jnz 0x002530CC
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_002530CC; }
	// 002530C8 and byte ptr [ebx+0x01], 0xBF
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x1u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0xFFFFFFBFu); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_002530CC:
	// 002530CC mov bl, [ebx+0x11]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x11u); ctx.ebx = (ctx.ebx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 002530CF test bl, bl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ebx)), b = (uint8_t)(((uint8_t)ctx.ebx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002530D1 jnz 0x002530E1
	if (((uint8_t)(ca & cb)) != 0) { goto loc_002530E1; }
	// 002530D3 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530D6 mov eax, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530D8 mov dword ptr [eax+0x08], 0x02
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x2u); }
	// 002530DF jmp 0x002530F2
	goto loc_002530F2;
loc_002530E1:
	// 002530E1 cmp bl, 0x02
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ebx)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002530E4 jnz 0x002530F2
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_002530F2; }
	// 002530E6 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530E9 mov eax, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530EB mov dword ptr [eax+0x08], 0x04
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x4u); }
loc_002530F2:
	// 002530F2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 002530F3 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 002530F4 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 002530F5 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 002530F6 ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00252DFB, __imp__sub_00252DFB)

X86_FUNC(__imp__sub_00252FA8) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	goto loc_00252FA8;
loc_00252F13:
	// 00252F13 lea eax, [ebp-0x14]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFECu));
	// 00252F16 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00252F17 lea edx, [ebp-0x10]
	ctx.edx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF0u));
	// 00252F1A lea ecx, [ebp-0x24]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFDCu));
	// 00252F1D call 0x00252DC1
	X86_PUSH32(0x252F22u); sub_00252DC1(ctx, base);
	// 00252F22 mov [ebp-0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_00252F25:
	// 00252F25 mov edx, [ebp-0x14]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00252F28 mov eax, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F2B mov ecx, [ebp-0x28]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFD8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252F2E add eax, edx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F30 cmp eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F32 jnb 0x00252F46
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_00252F46; }
	// 00252F34 test edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F36 jz 0x00252FFE
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00252FFE; }
	// 00252F3C cmp dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F40 jnz 0x00252FFE
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_00252FFE; }
loc_00252F46:
	// 00252F46 cmp dword ptr [ebp-0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F4A jz 0x00252F6E
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00252F6E; }
	// 00252F4C sub ecx, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F4F mov eax, [ebp-0x1C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F52 cmp edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F54 mov [esi+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252F57 jnb 0x00252F5B
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_00252F5B; }
	// 00252F59 mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
loc_00252F5B:
	// 00252F5B mov al, [ebp-0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252F5E add [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F61 add al, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252F63 sub edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F65 and dword ptr [ebp-0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F69 mov [esi+0x1D], al
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 00252F6C jmp 0x00252F8C
	goto loc_00252F8C;
loc_00252F6E:
	// 00252F6E cmp edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F70 mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F73 mov [esi+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252F76 jnb 0x00252F84
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_00252F84; }
	// 00252F78 add [ebp-0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F7B and dword ptr [ebp-0x14], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F7F mov [esi+0x1D], dl
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 00252F82 jmp 0x00252F8F
	goto loc_00252F8F;
loc_00252F84:
	// 00252F84 add [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252F87 mov [esi+0x1D], cl
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.ecx)); }
	// 00252F8A sub edx, ecx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.edx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00252F8C:
	// 00252F8C mov [ebp-0x14], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_00252F8F:
	// 00252F8F mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252F92 mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252F94 xor byte ptr [ebp+0x0B], 0x01
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xBu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x1u); uint8_t r = (uint8_t)(a ^ b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252F98 and byte ptr [esi+0x1C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252F9C and byte ptr [esi+0x1E], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Eu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00252FA0 dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FA1 mov [esi+0x0C], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252FA4 movzx eax, byte ptr [ebp+0x0B]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xBu); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
loc_00252FA8:
	// 00252FA8 push [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00252FAB and ecx, 0xFFBFFFF
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFFBFFFFu); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FB1 or ecx, 0xE0000000
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xE0000000u); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FB7 shl eax, 0x18
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x18u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00252FBA xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FBC and eax, 0x3000000
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x3000000u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FC1 xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FC3 mov [esi], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252FC5 or eax, 0xE00000
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xE00000u); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FCA xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FCC mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252FCE mov [esi+0x14], ebx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 00252FD1 mov cl, [edi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Cu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00252FD4 and eax, 0xF3E7FFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xF3E7FFFFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FD9 mov [esi+0x18], edi
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00252FDC mov [ebp-0x08], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00252FDF and ecx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FE2 shl ecx, 0x13
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x13u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 00252FE5 or ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00252FE7 mov [esi], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00252FE9 call 0x00252D30
	X86_PUSH32(0x252FEEu); sub_00252D30(ctx, base);
	// 00252FEE mov ecx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00252FF1 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 00252FF3 mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00252FF6 mov [ecx+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00252FF9 jmp 0x00252F25
	X86_BARRIER();
	goto loc_00252F25;
loc_00252FFE:
	// 00252FFE cmp dword ptr [ebp-0x10], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253002 mov eax, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00253005 mov [ebp-0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253008 mov [ebp-0x0C], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0025300B jnz 0x00252F13
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_00252F13; }
	// 00253011 cmp dword ptr [ebp-0x08], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253015 jz 0x00253024
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00253024; }
	// 00253017 cmp byte ptr [edi+0x1D], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Du); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025301B jz 0x00253024
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_00253024; }
	// 0025301D mov eax, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00253020 or byte ptr [eax+0x02], 0x04
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x4u); uint8_t r = (uint8_t)(a | b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_00253024:
	// 00253024 cmp byte ptr [ebx+0x11], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x11u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00253028 jnz 0x00253094
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_00253094; }
	// 0025302A and byte ptr [esi+0x02], 0xFB
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0xFFFFFFFBu); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025302E mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00253030 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253032 cmp byte ptr [edi+0x1C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Cu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00253036 push [ebp-0x18]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00253039 setnz al
	{ ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ca != (uint8_t)cb) ? 1u : 0u); }
	// 0025303C mov [ebp-0x08], esi
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0025303F inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253040 shl eax, 0x13
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x13u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 00253043 xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253045 and eax, 0x180000
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x180000u); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025304A xor eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025304C xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025304E mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253050 mov cl, [edi+0x1E]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Eu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 00253053 and dword ptr [esi+0x04], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253057 and dword ptr [esi+0x0C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025305B and byte ptr [esi+0x1D], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Du); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025305F and eax, 0x1FFFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x1FFFFFu); uint32_t r = (uint32_t)(a & b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253064 mov [esi+0x14], ebx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 00253067 mov byte ptr [esi+0x1C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 0025306B and ecx, 0x07
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x7u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025306E or ecx, 0xFFFFFF18
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xFFFFFF18u); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253074 shl ecx, 0x15
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x15u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 00253077 or ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a | b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253079 mov [esi], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0025307B mov byte ptr [esi+0x1E], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Eu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 0025307F mov [esi+0x18], edi
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00253082 call 0x00252D30
	X86_PUSH32(0x253087u); sub_00252D30(ctx, base);
	// 00253087 mov ecx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025308A mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 0025308C mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025308F mov [ecx+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253092 jmp 0x002530AC
	goto loc_002530AC;
loc_00253094:
	// 00253094 movzx ecx, byte ptr [edi+0x1E]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x1Eu); ctx.ecx = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00253098 mov eax, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025309B shl ecx, 0x15
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x15u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 0025309E xor ecx, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002530A0 mov byte ptr [eax+0x1C], 0x02
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); X86_MEM8(ea) = (uint8_t)(0x2u); }
	// 002530A4 and ecx, 0xE00000
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0xE00000u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002530AA xor [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_002530AC:
	// 002530AC mov byte ptr [esi+0x1E], 0x03
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Eu); X86_MEM8(ea) = (uint8_t)(0x3u); }
	// 002530B0 mov ax, [edi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x14u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 002530B4 and dword ptr [edi+0x14], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x14u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002530B8 mov [edi+0x20], ax
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x20u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 002530BC cmp byte ptr [ebx+0x20], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x20u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002530C0 mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530C3 mov [ebx+0x04], eax
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002530C6 jnz 0x002530CC
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_002530CC; }
	// 002530C8 and byte ptr [ebx+0x01], 0xBF
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x1u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0xFFFFFFBFu); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_002530CC:
	// 002530CC mov bl, [ebx+0x11]
	{ uint32_t ea = (uint32_t)(ctx.ebx + 0x11u); ctx.ebx = (ctx.ebx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 002530CF test bl, bl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ebx)), b = (uint8_t)(((uint8_t)ctx.ebx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002530D1 jnz 0x002530E1
	if (((uint8_t)(ca & cb)) != 0) { goto loc_002530E1; }
	// 002530D3 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530D6 mov eax, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530D8 mov dword ptr [eax+0x08], 0x02
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x2u); }
	// 002530DF jmp 0x002530F2
	goto loc_002530F2;
loc_002530E1:
	// 002530E1 cmp bl, 0x02
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ebx)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002530E4 jnz 0x002530F2
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_002530F2; }
	// 002530E6 mov eax, [ebp-0x20]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE0u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530E9 mov eax, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002530EB mov dword ptr [eax+0x08], 0x04
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x4u); }
loc_002530F2:
	// 002530F2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 002530F3 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 002530F4 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 002530F5 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 002530F6 ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00252FA8, __imp__sub_00252FA8)

X86_FUNC(__imp__sub_002530F9) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002530F9 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 002530FA mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 002530FC push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 002530FD push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 002530FE mov esi, edx
	{ ctx.esi = (uint32_t)(ctx.edx); }
	// 00253100 cmp dword ptr [esi+0x28], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x28u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253104 mov [ebp-0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00253107 jz 0x00253149
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00253149; }
	// 00253109 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
loc_0025310A:
	// 0025310A mov eax, [esi+0x28]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025310D mov dx, [esi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); ctx.edx = (ctx.edx & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00253111 movzx ebx, word ptr [eax+0x20]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x20u); ctx.ebx = (uint32_t)((uint16_t)(X86_MEM16(ea))); }
	// 00253115 movzx ecx, dx
	{ ctx.ecx = (uint32_t)((uint16_t)(((uint16_t)ctx.edx))); }
	// 00253118 add ecx, ebx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025311A cmp ecx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0025311D jnle 0x00253148
	if ((int32_t)ca > (int32_t)cb) { goto loc_00253148; }
	// 0025311F mov ecx, [eax+0x24]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x24u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00253122 test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253124 mov [esi+0x28], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00253127 jnz 0x0025312C
	if (((uint32_t)(ca & cb)) != 0) { goto loc_0025312C; }
	// 00253129 and [esi+0x2C], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0025312C:
	// 0025312C mov cx, [eax+0x20]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x20u); ctx.ecx = (ctx.ecx & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00253130 add cx, dx
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.ecx)); uint16_t b = (uint16_t)(((uint16_t)ctx.edx)); uint16_t r = (uint16_t)(a + b); ctx.ecx = (ctx.ecx & 0xFFFF0000u) | (uint16_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00253133 mov [esi+0x24], cx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 00253137 mov ecx, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0025313A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0025313B mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 0025313D call 0x00252DFB
	X86_PUSH32(0x253142u); sub_00252DFB(ctx, base);
	// 00253142 cmp dword ptr [esi+0x28], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x28u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253146 jnz 0x0025310A
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_0025310A; }
loc_00253148:
	// 00253148 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
loc_00253149:
	// 00253149 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0025314A leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 0025314B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_002530F9, __imp__sub_002530F9)

X86_FUNC(__imp__sub_0025314C) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0025314C push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0025314D mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 0025314F cmp dword ptr [esi+0x424], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x424u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253156 jz 0x00253197
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00253197; }
	// 00253158 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
loc_00253159:
	// 00253159 mov edi, [esi+0x424]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x424u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0025315F xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253161 mov ax, [edi+0x20]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x20u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00253165 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00253166 call 0x00252D6F
	X86_PUSH32(0x25316Bu); sub_00252D6F(ctx, base);
	// 0025316B test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0025316D jz 0x00253196
	if (((uint8_t)(ca & cb)) == 0) { goto loc_00253196; }
	// 0025316F mov eax, [edi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x24u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00253172 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253174 mov [esi+0x424], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x424u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0025317A jnz 0x00253182
	if (((uint32_t)(ca & cb)) != 0) { goto loc_00253182; }
	// 0025317C and [esi+0x428], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x428u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00253182:
	// 00253182 mov edx, [edi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 00253185 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00253186 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 00253188 call 0x00252DFB
	X86_PUSH32(0x25318Du); sub_00252DFB(ctx, base);
	// 0025318D cmp dword ptr [esi+0x424], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x424u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253194 jnz 0x00253159
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_00253159; }
loc_00253196:
	// 00253196 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
loc_00253197:
	// 00253197 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00253198 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0025314C, __imp__sub_0025314C)

X86_FUNC(__imp__sub_00253199) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00253199 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0025319A mov esi, ecx
	{ ctx.esi = (uint32_t)(ctx.ecx); }
	// 0025319C cmp dword ptr [esi+0x41C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x41Cu); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002531A3 jz 0x002531E4
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_002531E4; }
	// 002531A5 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
loc_002531A6:
	// 002531A6 mov edi, [esi+0x41C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x41Cu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 002531AC xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002531AE mov ax, [edi+0x20]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x20u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 002531B2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 002531B3 call 0x00252D50
	X86_PUSH32(0x2531B8u); sub_00252D50(ctx, base);
	// 002531B8 test al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002531BA jz 0x002531E3
	if (((uint8_t)(ca & cb)) == 0) { goto loc_002531E3; }
	// 002531BC mov eax, [edi+0x24]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x24u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002531BF test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002531C1 mov [esi+0x41C], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x41Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 002531C7 jnz 0x002531CF
	if (((uint32_t)(ca & cb)) != 0) { goto loc_002531CF; }
	// 002531C9 and [esi+0x420], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x420u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_002531CF:
	// 002531CF mov edx, [edi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 002531D2 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 002531D3 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 002531D5 call 0x00252DFB
	X86_PUSH32(0x2531DAu); sub_00252DFB(ctx, base);
	// 002531DA cmp dword ptr [esi+0x41C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x41Cu); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002531E1 jnz 0x002531A6
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_002531A6; }
loc_002531E3:
	// 002531E3 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
loc_002531E4:
	// 002531E4 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 002531E5 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00253199, __imp__sub_00253199)

X86_FUNC(__imp__sub_002531E6) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 002531E6 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 002531EA cmp word ptr [eax+0x20], 0x03
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x20u); uint16_t a = (uint16_t)(X86_MEM16(ea)), b = (uint16_t)(0x3u); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 002531EF jbe 0x002531F8
	if ((uint16_t)ca <= (uint16_t)cb) { goto loc_002531F8; }
	// 002531F1 mov eax, 0x80000500
	{ ctx.eax = (uint32_t)(0x80000500u); }
	// 002531F6 jmp 0x00253216
	goto loc_00253216;
loc_002531F8:
	// 002531F8 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 002531F9 mov esi, [edx+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2Cu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 002531FC test esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002531FE jz 0x00253205
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00253205; }
	// 00253200 mov [esi+0x24], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00253203 jmp 0x00253208
	goto loc_00253208;
loc_00253205:
	// 00253205 mov [edx+0x28], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_00253208:
	// 00253208 mov [edx+0x2C], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0025320B call 0x002530F9
	X86_PUSH32(0x253210u); sub_002530F9(ctx, base);
	// 00253210 mov eax, 0x40000000
	{ ctx.eax = (uint32_t)(0x40000000u); }
	// 00253215 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
loc_00253216:
	// 00253216 ret 0x04
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_002531E6, __imp__sub_002531E6)

X86_FUNC(__imp__sub_00253219) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00253219 mov ax, [edx+0x20]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x20u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 0025321D cmp ax, [0x003190C4]
	{ uint32_t ea = (uint32_t)(0x3190C4u); uint16_t a = (uint16_t)(((uint16_t)ctx.eax)), b = (uint16_t)(X86_MEM16(ea)); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00253224 jbe 0x0025322C
	if ((uint16_t)ca <= (uint16_t)cb) { goto loc_0025322C; }
	// 00253226 mov eax, 0x80000500
	{ ctx.eax = (uint32_t)(0x80000500u); }
	// 0025322B ret
	ctx.esp += 4; return;
loc_0025322C:
	// 0025322C lea eax, [ecx+0x424]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx + 0x424u));
	// 00253232 cmp dword ptr [eax], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253235 jz 0x00253242
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_00253242; }
	// 00253237 mov eax, [ecx+0x428]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x428u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0025323D mov [eax+0x24], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00253240 jmp 0x00253244
	goto loc_00253244;
loc_00253242:
	// 00253242 mov [eax], edx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_00253244:
	// 00253244 mov [ecx+0x428], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x428u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0025324A call 0x0025314C
	X86_PUSH32(0x25324Fu); sub_0025314C(ctx, base);
	// 0025324F mov eax, 0x40000000
	{ ctx.eax = (uint32_t)(0x40000000u); }
	// 00253254 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00253219, __imp__sub_00253219)

X86_FUNC(__imp__sub_00253255) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00253255 mov ax, [edx+0x20]
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x20u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 00253259 cmp ax, [0x003190C0]
	{ uint32_t ea = (uint32_t)(0x3190C0u); uint16_t a = (uint16_t)(((uint16_t)ctx.eax)), b = (uint16_t)(X86_MEM16(ea)); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00253260 jbe 0x00253268
	if ((uint16_t)ca <= (uint16_t)cb) { goto loc_00253268; }
	// 00253262 mov eax, 0x80000500
	{ ctx.eax = (uint32_t)(0x80000500u); }
	// 00253267 ret
	ctx.esp += 4; return;
loc_00253268:
	// 00253268 lea eax, [ecx+0x41C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx + 0x41Cu));
	// 0025326E cmp dword ptr [eax], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00253271 jz 0x0025327E
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0025327E; }
	// 00253273 mov eax, [ecx+0x420]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x420u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00253279 mov [eax+0x24], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0025327C jmp 0x00253280
	goto loc_00253280;
loc_0025327E:
	// 0025327E mov [eax], edx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
loc_00253280:
	// 00253280 mov [ecx+0x420], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x420u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 00253286 call 0x00253199
	X86_PUSH32(0x25328Bu); sub_00253199(ctx, base);
	// 0025328B mov eax, 0x40000000
	{ ctx.eax = (uint32_t)(0x40000000u); }
	// 00253290 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00253255, __imp__sub_00253255)

X86_FUNC(__imp__sub_00253291) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00253291 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 00253292 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 00253294 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 00253295 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00253296 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00253297 mov esi, edx
	{ ctx.esi = (uint32_t)(ctx.edx); }
	// 00253299 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0025329A mov edi, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0025329D mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 0025329F mov edx, edi
	{ ctx.edx = (uint32_t)(ctx.edi); }
	// 002532A1 mov ecx, esi
	{ ctx.ecx = (uint32_t)(ctx.esi); }
	// 002532A3 call 0x00252D8E
	X86_PUSH32(0x2532A8u); sub_00252D8E(ctx, base);
	// 002532A8 mov [esi+0x20], ax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x20u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 002532AC call [0x002534D4]
	X86_PUSH32(0x2532B2u); x86_kernel(ctx, base, 129);
	// 002532B2 inc [edi+0x26]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x26u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a + 1); X86_MEM8(ea) = (uint8_t)(r); of = r == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 002532B5 and dword ptr [esi+0x24], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x24u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002532B9 mov [ebp-0x01], al
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.eax)); }
	// 002532BC mov word ptr [esi+0x22], 0x02
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x22u); X86_MEM16(ea) = (uint16_t)(0x2u); }
	// 002532C2 movzx eax, byte ptr [edi+0x11]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x11u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 002532C6 sub eax, 0x00
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002532C9 jz 0x002532F0
	if (zf) { goto loc_002532F0; }
	// 002532CB dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002532CC dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002532CD jz 0x002532E5
	if (zf) { goto loc_002532E5; }
	// 002532CF dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002532D0 jz 0x002532D9
	if (zf) { goto loc_002532D9; }
	// 002532D2 mov ebx, 0x80000600
	{ ctx.ebx = (uint32_t)(0x80000600u); }
	// 002532D7 jmp 0x002532FF
	goto loc_002532FF;
loc_002532D9:
	// 002532D9 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 002532DA mov edx, edi
	{ ctx.edx = (uint32_t)(ctx.edi); }
	// 002532DC mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 002532DE call 0x002531E6
	X86_PUSH32(0x2532E3u); sub_002531E6(ctx, base);
	// 002532E3 jmp 0x002532F9
	goto loc_002532F9;
loc_002532E5:
	// 002532E5 mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 002532E7 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 002532E9 call 0x00253219
	X86_PUSH32(0x2532EEu); sub_00253219(ctx, base);
	// 002532EE jmp 0x002532F9
	goto loc_002532F9;
loc_002532F0:
	// 002532F0 mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 002532F2 mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 002532F4 call 0x00253255
	X86_PUSH32(0x2532F9u); sub_00253255(ctx, base);
loc_002532F9:
	// 002532F9 mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 002532FB test ebx, ebx
	{ uint32_t a = (uint32_t)(ctx.ebx), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 002532FD jnl 0x00253307
	if (((int32_t)(ca & cb)) >= 0) { goto loc_00253307; }
loc_002532FF:
	// 002532FF and word ptr [esi+0x22], 0x00
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x22u); uint16_t a = (uint16_t)(X86_MEM16(ea)); uint16_t b = (uint16_t)(0x0u); uint16_t r = (uint16_t)(a & b); X86_MEM16(ea) = (uint16_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 00253304 dec [edi+0x26]
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x26u); uint8_t a = (uint8_t)(X86_MEM8(ea));  uint8_t r = (uint8_t)(a - 1); X86_MEM8(ea) = (uint8_t)(r); of = a == (uint8_t)(1u << 7); zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_00253307:
	// 00253307 mov cl, [ebp-0x01]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFFu); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 0025330A call [0x0025349C]
	X86_PUSH32(0x253310u); x86_kernel(ctx, base, 161);
	// 00253310 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00253311 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00253312 mov eax, ebx
	{ ctx.eax = (uint32_t)(ctx.ebx); }
	// 00253314 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00253315 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 00253316 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00253291, __imp__sub_00253291)

