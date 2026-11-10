#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_001E893C) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x001E893Cu);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E893C mov eax, [0x002534DC]
	{ uint32_t ea = (uint32_t)(0x2534DCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001E8941 mov eax, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	ff_sdk_observe_exit(ctx, base, 0x001E893Cu);
	// 001E8943 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001E893C, __imp__sub_001E893C)