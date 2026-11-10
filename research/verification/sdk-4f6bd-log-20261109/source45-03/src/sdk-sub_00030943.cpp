#include "sdk-shared-prototypes.hpp"

X86_FUNC(__imp__sub_00030943) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x00030943u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00030943 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 00030945 push [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 00030949 push [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 0003094D call 0x0003082D
	X86_PUSH32(0x30952u); sub_0003082D(ctx, base);
	// 00030952 ret 0x08
	ff_sdk_observe_exit(ctx, base, 0x00030943u);
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_00030943, __imp__sub_00030943)
