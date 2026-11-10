#include "sdk-shared-prototypes.hpp"
#include "shared-abi.h"
X86_FUNC(__imp__sub_0004F5E0) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004F5E0u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004F5E0 sub esp, 0x18
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x18u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F5E3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0004F5E4 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F5E6 mov ecx, 0x232E
	{ ctx.ecx = (uint32_t)(0x232Eu); }
	// 0004F5EB mov edi, 0x2D15E8
	{ ctx.edi = (uint32_t)(0x2D15E8u); }
	// 0004F5F0 rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 0004F5F2 mov ecx, 0x0B
	{ ctx.ecx = (uint32_t)(0xBu); }
	// 0004F5F7 mov edi, 0x2D15B8
	{ ctx.edi = (uint32_t)(0x2D15B8u); }
	// 0004F5FC rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 0004F5FE call 0x0004EB50
	X86_PUSH32(0x4F603u); sub_0004EB50(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x4F603u);
	// 0004F603 push 0x258EC4
	{ uint32_t v = (uint32_t)(0x258EC4u); X86_PUSH32(v); }
	// 0004F608 push 0x25C908
	{ uint32_t v = (uint32_t)(0x25C908u); X86_PUSH32(v); }
	// 0004F60D call 0x001E01AD
	X86_PUSH32(0x4F612u); sub_001E01AD(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x4F612u);
	// 0004F612 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F615 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F617 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004F618 jz 0x0004F625
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004F625; }
	// 0004F61A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004F61B call 0x001DFE85
	X86_PUSH32(0x4F620u); sub_001DFE85(ctx, base);
	// 0004F620 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F623 jmp 0x0004F62F
	goto loc_0004F62F;
loc_0004F625:
	// 0004F625 mov dword ptr [0x002D73AC], 0x01
	{ uint32_t ea = (uint32_t)(0x2D73ACu); X86_MEM32(ea) = (uint32_t)(0x1u); }
loc_0004F62F:
	// 0004F62F push 0x25C4D0
	{ uint32_t v = (uint32_t)(0x25C4D0u); X86_PUSH32(v); }
	// 0004F634 call 0x0004D4F0
	X86_PUSH32(0x4F639u); sub_0004D4F0(ctx, base);
	// 0004F639 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004F63B push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 0004F63D push 0x25C4D0
	{ uint32_t v = (uint32_t)(0x25C4D0u); X86_PUSH32(v); }
	// 0004F642 call 0x0004D620
	X86_PUSH32(0x4F647u); sub_0004D620(ctx, base);
	// 0004F647 push 0x25C4C0
	{ uint32_t v = (uint32_t)(0x25C4C0u); X86_PUSH32(v); }
	// 0004F64C mov [0x002D73B0], eax
	{ uint32_t ea = (uint32_t)(0x2D73B0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004F651 call 0x0004D4F0
	X86_PUSH32(0x4F656u); sub_0004D4F0(ctx, base);
	// 0004F656 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004F658 push 0x04
	{ uint32_t v = (uint32_t)(0x4u); X86_PUSH32(v); }
	// 0004F65A push 0x25C4C0
	{ uint32_t v = (uint32_t)(0x25C4C0u); X86_PUSH32(v); }
	// 0004F65F call 0x0004D620
	X86_PUSH32(0x4F664u); sub_0004D620(ctx, base);
	// 0004F664 mov [0x002D73B4], eax
	{ uint32_t ea = (uint32_t)(0x2D73B4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004F669 mov eax, [0x002D73AC]
	{ uint32_t ea = (uint32_t)(0x2D73ACu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004F66E add esp, 0x20
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x20u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F671 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F673 jz 0x0004F6B8
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004F6B8; }
	// 0004F675 push 0x2006
	{ uint32_t v = (uint32_t)(0x2006u); X86_PUSH32(v); }
	// 0004F67A push 0x162
	{ uint32_t v = (uint32_t)(0x162u); X86_PUSH32(v); }
	// 0004F67F push 0x25C558
	{ uint32_t v = (uint32_t)(0x25C558u); X86_PUSH32(v); }
	// 0004F684 push 0x40000
	{ uint32_t v = (uint32_t)(0x40000u); X86_PUSH32(v); }
	// 0004F689 call 0x0004C480
	X86_PUSH32(0x4F68Eu); sub_0004C480(ctx, base);
	// 0004F68E add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F691 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F693 mov [0x002D15E0], eax
	{ uint32_t ea = (uint32_t)(0x2D15E0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004F698 jnz 0x0004F6B1
	if (((uint32_t)(ca & cb)) != 0) { goto loc_0004F6B1; }
	// 0004F69A push 0x25C8C8
	{ uint32_t v = (uint32_t)(0x25C8C8u); X86_PUSH32(v); }
	// 0004F69F call 0x0004B230
	X86_PUSH32(0x4F6A4u); sub_0004B230(ctx, base);
	// 0004F6A4 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F6A7 mov dword ptr [0x002D73AC], 0x00
	{ uint32_t ea = (uint32_t)(0x2D73ACu); X86_MEM32(ea) = (uint32_t)(0x0u); }
loc_0004F6B1:
	// 0004F6B1 push 0x25C89C
	{ uint32_t v = (uint32_t)(0x25C89Cu); X86_PUSH32(v); }
	// 0004F6B6 jmp 0x0004F6BD
	goto loc_0004F6BD;
loc_0004F6B8:
	// 0004F6B8 push 0x25C870
	{ uint32_t v = (uint32_t)(0x25C870u); X86_PUSH32(v); }
loc_0004F6BD:
	// 0004F6BD call 0x0004B230
	X86_PUSH32(0x4F6C2u); sub_0004B230(ctx, base);
	if(ff_probe_sdk_chain_word(0u)==45u)ff_sdk_observe_exit(ctx,base,0x4f6c2u);
	// 0004F6C2 add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F6C5 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 0004F6C7 call 0x0004EA30
	X86_PUSH32(0x4F6CCu); sub_0004EA30(ctx, base);
	// 0004F6CC add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F6CF lea eax, [esp+0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x8u));
	// 0004F6D3 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004F6D4 lea ecx, [esp+0x14]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x14u));
	// 0004F6D8 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004F6D9 lea edx, [esp+0x08]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x8u));
	// 0004F6DD push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004F6DE push 0x25C580
	{ uint32_t v = (uint32_t)(0x25C580u); X86_PUSH32(v); }
	// 0004F6E3 call 0x00030B2D
	X86_PUSH32(0x4F6E8u); sub_00030B2D(ctx, base);
	// 0004F6E8 mov eax, [esp]
	{ uint32_t ea = (uint32_t)(ctx.esp); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004F6EC mov ecx, [0x002D73A8]
	{ uint32_t ea = (uint32_t)(0x2D73A8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004F6F2 shr eax, 0x0A
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0xAu) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 0004F6F5 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004F6F6 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004F6F7 push 0x25C830
	{ uint32_t v = (uint32_t)(0x25C830u); X86_PUSH32(v); }
	// 0004F6FC call 0x0004B230
	X86_PUSH32(0x4F701u); sub_0004B230(ctx, base);
	// 0004F701 add esp, 0x24
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F704 ret
	ff_sdk_observe_exit(ctx, base, 0x0004F5E0u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004F5E0, __imp__sub_0004F5E0)
