#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0024D379) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0024D379u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0024D379 jmp 0x0024CA33
	X86_BARRIER();
	{ X86_SAVE_FLAGS(); sub_0024CA33(ctx, base); return; }
}
X86_WEAK_FUNC(sub_0024D379, __imp__sub_0024D379)