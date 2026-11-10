#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"
X86_EXTERN_FUNC(sub_001E566A);

X86_FUNC(__imp__sub_001E056D) {
	if (!ff_sdk_context_active(ctx, base) || (ff_probe_sdk_chain_word(0u)!=30u && (ff_probe_sdk_chain_word(0u)!=31u && (ff_probe_sdk_chain_word(0u)!=32u && (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u))))))))))))))) ||
	    ff_probe_sdk_chain_word(17u) != 0x0004F5E0u || ff_probe_sdk_chain_word(2090u) >= 2u || ctx.esp != 0x03c10c50u)
		ff_sdk_raise(ctx, 0x001E056Du, "original comparison helper outside exact profile-30 root/invocation");
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x001E056Du);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E056D cmp dword ptr [0x00315AC4], 0x00
	{ uint32_t ea = (uint32_t)(0x315AC4u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E0574 jnz 0x001E057B
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_001E057B; }
	// 001E0576 jmp 0x001E56A0
	goto loc_001E56A0;
loc_001E057B:
	// 001E057B push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001E057C push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E057D mov esi, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001E0581 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E0582 mov edi, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
loc_001E0586:
	// 001E0586 movzx eax, byte ptr [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 001E0589 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E058A call 0x001E566A
	X86_PUSH32(0x1E058Fu); sub_001E566A(ctx, base);
	// 001E058F mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 001E0591 movzx eax, byte ptr [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 001E0594 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E0595 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E0596 call 0x001E566A
	X86_PUSH32(0x1E059Bu); sub_001E566A(ctx, base);
	// 001E059B pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001E059C inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E059D test ebx, ebx
	{ uint32_t a = (uint32_t)(ctx.ebx), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E059F pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001E05A0 mov ecx, eax
	{ ctx.ecx = (uint32_t)(ctx.eax); }
	// 001E05A2 jz 0x001E05A8
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001E05A8; }
	// 001E05A4 cmp ebx, ecx
	{ uint32_t a = (uint32_t)(ctx.ebx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E05A6 jz 0x001E0586
	if ((uint32_t)ca == (uint32_t)cb) { X86_BARRIER(); goto loc_001E0586; }
loc_001E05A8:
	// 001E05A8 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001E05A9 mov eax, ebx
	{ ctx.eax = (uint32_t)(ctx.ebx); }
	// 001E05AB pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E05AC sub eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ctx.eax = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E05AE pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001E05AF ret
	ff_sdk_observe_exit(ctx, base, 0x001E056Du); ctx.esp += 4; return;
loc_001E56A0:
	// 001E56A0 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001E56A1 mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001E56A3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E56A4 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E56A5 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001E56A6 mov esi, [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001E56A9 mov edi, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001E56AC mov al, 0xFF
	{ ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(0xFFFFFFFFu); }
	// 001E56AE mov edi, edi
	{ ctx.edi = (uint32_t)(ctx.edi); }
loc_001E56B0:
	// 001E56B0 or al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a | b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56B2 jz 0x001E56E2
	if (zf) { goto loc_001E56E2; }
	// 001E56B4 mov al, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }
	// 001E56B6 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E56B7 mov ah, [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.eax = (ctx.eax & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(X86_MEM8(ea)) << 8); }
	// 001E56B9 inc edi
	{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E56BA cmp ah, al
	{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(((uint8_t)ctx.eax)); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56BC jz 0x001E56B0
	if ((uint8_t)ca == (uint8_t)cb) { X86_BARRIER(); goto loc_001E56B0; }
	// 001E56BE sub al, 0x41
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0x41u); uint8_t r = (uint8_t)(a - b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56C0 cmp al, 0x1A
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x1Au); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56C2 sbb cl, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint32_t c0 = cf; uint8_t r = (uint8_t)(a - b - c0); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56C4 and cl, 0x20
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(0x20u); uint8_t r = (uint8_t)(a & b); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56C7 add al, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56C9 add al, 0x41
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0x41u); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56CB xchg al, ah
	{ uint8_t t = (uint8_t)(((uint8_t)ctx.eax)); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(((uint8_t)(ctx.eax >> 8))); ctx.eax = (ctx.eax & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(t) << 8); }
	// 001E56CD sub al, 0x41
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0x41u); uint8_t r = (uint8_t)(a - b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56CF cmp al, 0x1A
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x1Au); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56D1 sbb cl, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint32_t c0 = cf; uint8_t r = (uint8_t)(a - b - c0); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56D3 and cl, 0x20
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t b = (uint8_t)(0x20u); uint8_t r = (uint8_t)(a & b); ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56D6 add al, cl
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.ecx)); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56D8 add al, 0x41
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0x41u); uint8_t r = (uint8_t)(a + b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56DA cmp al, ah
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(((uint8_t)(ctx.eax >> 8))); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56DC jz 0x001E56B0
	if ((uint8_t)ca == (uint8_t)cb) { X86_BARRIER(); goto loc_001E56B0; }
	// 001E56DE sbb al, al
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.eax)); uint32_t c0 = cf; uint8_t r = (uint8_t)(a - b - c0); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E56E0 sbb al, 0xFF
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(0xFFFFFFFFu); uint32_t c0 = cf; uint8_t r = (uint8_t)(a - b - c0); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); cf = (uint64_t)a < (uint64_t)b + c0; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
loc_001E56E2:
	// 001E56E2 movsx eax, al
	{ ctx.eax = (uint32_t)((uint32_t)(int8_t)(((uint8_t)ctx.eax))); }
	// 001E56E5 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001E56E6 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E56E7 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001E56E8 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001E56E9 ret
	ff_sdk_observe_exit(ctx, base, 0x001E056Du); ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001E056D, __imp__sub_001E056D)