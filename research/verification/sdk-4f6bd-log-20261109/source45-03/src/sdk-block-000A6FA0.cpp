#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_000A6FA0) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x000A6FA0u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 000A6FA0 mov eax, 0x01
	{ ctx.eax = (uint32_t)(0x1u); }
	// 000A6FA5 ret
	ff_sdk_observe_exit(ctx, base, 0x000A6FA0u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_000A6FA0, __imp__sub_000A6FA0)