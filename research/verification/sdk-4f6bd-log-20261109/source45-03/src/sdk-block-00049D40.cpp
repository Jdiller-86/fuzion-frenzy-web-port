#include "sdk-shared-prototypes.hpp"
#include "shared-abi.h"
X86_FUNC(__imp__sub_00049D40) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x00049D40u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00049D40 sub esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049D43 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00049D44 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049D46 mov ecx, 0x73
	{ ctx.ecx = (uint32_t)(0x73u); }
	// 00049D4B mov edi, 0x2CC1F0
	{ ctx.edi = (uint32_t)(0x2CC1F0u); }
	// 00049D50 rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 00049D52 mov eax, 0x04
	{ ctx.eax = (uint32_t)(0x4u); }
	// 00049D57 mov [esp+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00049D5B mov [esp+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00049D5F lea eax, [esp+0x04]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x4u));
	// 00049D63 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00049D64 push 0x02
	{ uint32_t v = (uint32_t)(0x2u); X86_PUSH32(v); }
	// 00049D66 mov dword ptr [esp+0x0C], 0x24C2BC
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); X86_MEM32(ea) = (uint32_t)(0x24C2BCu); }
	// 00049D6E mov dword ptr [esp+0x14], 0x24C238
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); X86_MEM32(ea) = (uint32_t)(0x24C238u); }
	// 00049D76 call 0x0024D379
	X86_PUSH32(0x49D7Bu); sub_0024D379(ctx, base);
	// 00049D7B call 0x00049990
	X86_PUSH32(0x49D80u); sub_00049990(ctx, base);
	// 00049D80 call 0x001E893C
	X86_PUSH32(0x49D85u); sub_001E893C(ctx, base);
	if (ff_probe_sdk_chain_word(0u)>=15u) ff_sdk_observe_exit(ctx, base, 0x00049D85u);
	// 00049D85 mov ecx, [0x002CC280]
	{ uint32_t ea = (uint32_t)(0x2CC280u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00049D8B push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 00049D8C push 0x25B628
	{ uint32_t v = (uint32_t)(0x25B628u); X86_PUSH32(v); }
	// 00049D91 mov [0x002CC3B8], eax
	{ uint32_t ea = (uint32_t)(0x2CC3B8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00049D96 call 0x0004B230
	X86_PUSH32(0x49D9Bu); sub_0004B230(ctx, base);
	// 00049D9B add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049D9E pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00049D9F add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049DA2 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00049D40, __imp__sub_00049D40)