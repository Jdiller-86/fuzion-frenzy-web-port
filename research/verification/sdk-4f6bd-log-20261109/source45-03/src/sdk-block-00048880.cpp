#include "sdk-shared-prototypes.hpp"
#include "shared-abi.h"
X86_FUNC(__imp__sub_00048880) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x00048880u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00048880 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00048884 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00048885 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 00048886 mov dword ptr [0x002CC1B0], 0x00
	{ uint32_t ea = (uint32_t)(0x2CC1B0u); X86_MEM32(ea) = (uint32_t)(0x0u); }
	// 00048890 call 0x00049D40
	X86_PUSH32(0x48895u); sub_00049D40(ctx, base);
	if (ff_probe_sdk_chain_word(0u)>=15u) ff_sdk_observe_exit(ctx, base, 0x00048895u);
	// 00048895 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00048897 mov ecx, 0x310
	{ ctx.ecx = (uint32_t)(0x310u); }
	// 0004889C mov edi, 0x2CB570
	{ ctx.edi = (uint32_t)(0x2CB570u); }
	// 000488A1 push 0x259470
	{ uint32_t v = (uint32_t)(0x259470u); X86_PUSH32(v); }
	// 000488A6 rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 000488A8 call 0x0004D4F0
	X86_PUSH32(0x488ADu); sub_0004D4F0(ctx, base);
	if (ff_probe_sdk_chain_word(0u)>=21u) ff_sdk_observe_exit(ctx, base, 0x000488ADu);
	// 000488AD add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000488B0 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 000488B1 mov dword ptr [esp+0x04], 0x256410
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); X86_MEM32(ea) = (uint32_t)(0x256410u); }
	if (ff_probe_sdk_chain_word(0u)>=21u) { X86_SAVE_FLAGS(); ff_sdk_observe_exit(ctx, base, 0x000488B9u); if (ff_probe_sdk_chain_word(0u)<22u) ff_sdk_raise(ctx, 0x000488B9u, "unsupported original tail re-entry; no guest CALL"); }
	// 000488B9 jmp 0x0004D4F0
	{ X86_SAVE_FLAGS(); sub_0004D4F0(ctx, base); return; }
}
X86_WEAK_FUNC(sub_00048880, __imp__sub_00048880)