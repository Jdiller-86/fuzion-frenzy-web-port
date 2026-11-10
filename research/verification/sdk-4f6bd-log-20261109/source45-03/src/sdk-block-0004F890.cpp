#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0004F890) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004F890u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004F890 mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004F894 xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004F896 mov [eax+0x18], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004F899 mov [eax+0x08], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004F89C mov [eax+0x0C], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004F89F mov [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004F8A1 mov [eax+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004F8A4 mov ecx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004F8A8 mov [eax+0x1C], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004F8AB ret
	ff_sdk_observe_exit(ctx, base, 0x0004F890u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004F890, __imp__sub_0004F890)