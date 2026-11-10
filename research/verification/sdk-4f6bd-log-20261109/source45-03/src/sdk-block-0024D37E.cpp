#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0024D37E) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0024D37Eu);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0024D37E push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0024D37F call [0x002534D4]
	X86_PUSH32(0x24D385u); x86_kernel(ctx, base, 129);
	// 0024D385 mov edx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0024D389 mov esi, [edx]
	{ uint32_t ea = (uint32_t)(ctx.edx); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0024D38B and dword ptr [edx+0x04], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a & b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0024D38F mov cl, al
	{ ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(((uint8_t)ctx.eax)); }
	// 0024D391 mov [edx+0x08], esi
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0024D394 call [0x0025349C]
	X86_PUSH32(0x24D39Au); x86_kernel(ctx, base, 161);
	// 0024D39A mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 0024D39C pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0024D39D ret 0x04
	ff_sdk_observe_exit(ctx, base, 0x0024D37Eu);
	ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_0024D37E, __imp__sub_0024D37E)