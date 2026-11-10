#include "sdk-shared-prototypes.hpp"
#include "shared-abi.h"
X86_FUNC(__imp__sub_0004D4F0) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004D4F0u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004D4F0 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 0004D4F1 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0004D4F2 mov edi, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0004D4F6 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0004D4F7 call 0x0004DC30
	X86_PUSH32(0x4D4FCu); sub_0004DC30(ctx, base);
	// 0004D4FC mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 0004D4FE mov eax, [0x002D02FC]
	{ uint32_t ea = (uint32_t)(0x2D02FCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004D503 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D506 cmp eax, 0x2D02D8
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x2D02D8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D50B jz 0x0004D51F
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004D51F; }
	// 0004D50D lea ecx, [ecx]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.ecx));
loc_0004D510:
	// 0004D510 cmp [eax+0x20], ebx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x20u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D513 jz 0x0004D582
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004D582; }
	// 0004D515 mov eax, [eax+0x24]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x24u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004D518 cmp eax, 0x2D02D8
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x2D02D8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D51D jnz 0x0004D510
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_0004D510; }
loc_0004D51F:
	// 0004D51F push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004D521 push 0x25BFCC
	{ uint32_t v = (uint32_t)(0x25BFCCu); X86_PUSH32(v); }
	// 0004D526 push 0x2001
	{ uint32_t v = (uint32_t)(0x2001u); X86_PUSH32(v); }
	// 0004D52B push 0x5B
	{ uint32_t v = (uint32_t)(0x5Bu); X86_PUSH32(v); }
	// 0004D52D push 0x25BFA8
	{ uint32_t v = (uint32_t)(0x25BFA8u); X86_PUSH32(v); }
	// 0004D532 push 0x50
	{ uint32_t v = (uint32_t)(0x50u); X86_PUSH32(v); }
	// 0004D534 call 0x0004BF10
	X86_PUSH32(0x4D539u); sub_0004BF10(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004D539u);
	// 0004D539 add esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D53C test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D53E jnz 0x0004D543
	if (((uint32_t)(ca & cb)) != 0) { goto loc_0004D543; }
	// 0004D540 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004D541 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 0004D542 ret
	ff_sdk_observe_exit(ctx, base, 0x0004D4F0u);
	ctx.esp += 4; return;
loc_0004D543:
	// 0004D543 mov ecx, [0x002D0300]
	{ uint32_t ea = (uint32_t)(0x2D0300u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004D549 mov [eax+0x28], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004D54C mov edx, ecx
	{ ctx.edx = (uint32_t)(ctx.ecx); }
	// 0004D54E mov dword ptr [eax+0x24], 0x2D02D8
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x24u); X86_MEM32(ea) = (uint32_t)(0x2D02D8u); }
	if (ff_probe_sdk_chain_word(0u)<21u) { X86_SAVE_FLAGS(); sub_0004D555(ctx, base); return; }
	ff_sdk_observe_exit(ctx, base, 0x0004D555u);
	// 0004D555 mov [edx+0x24], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x24u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004D558 mov ecx, [eax+0x24]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x24u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004D55B push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004D55C mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 0004D55E mov [ecx+0x28], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x28u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004D561 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 0004D563 sub esi, edi
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ctx.esi = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0004D565:
	// 0004D565 mov dl, [ecx]
	{ uint32_t ea = (uint32_t)(ctx.ecx); ctx.edx = (ctx.edx & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 0004D567 mov [esi+ecx*1], dl
	{ uint32_t ea = (uint32_t)(ctx.esi + ctx.ecx); X86_MEM8(ea) = (uint8_t)(((uint8_t)ctx.edx)); }
	// 0004D56A inc ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a + 1); ctx.ecx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004D56B test dl, dl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.edx)), b = (uint8_t)(((uint8_t)ctx.edx)); uint8_t r = (uint8_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 0004D56D jnz 0x0004D565
	if (((uint8_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_0004D565; }
	// 0004D56F lea ecx, [eax+0x2C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.eax + 0x2Cu));
	// 0004D572 mov [eax+0x30], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x30u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004D575 mov [ecx], ecx
	{ uint32_t ea = (uint32_t)(ctx.ecx); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004D577 mov [eax+0x20], ebx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 0004D57A mov dword ptr [eax+0x4C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4Cu); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 0004D581 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }

loc_0004D582:
	// 0004D582 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004D583 mov eax, 0x01
	{ ctx.eax = (uint32_t)(0x1u); }
	// 0004D588 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 0004D589 ret
	ff_sdk_observe_exit(ctx, base, 0x0004D4F0u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004D4F0, __imp__sub_0004D4F0)