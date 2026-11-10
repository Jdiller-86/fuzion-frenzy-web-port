#include "ff_sdk_adapter.hpp"
#include "shared-abi.h"
#include "sdk-shared-prototypes.hpp"
X86_EXTERN_FUNC(sub_001E2038);
X86_EXTERN_FUNC(sub_001E6AAF);
X86_EXTERN_FUNC(sub_000343F3);
X86_EXTERN_FUNC(sub_001E564E);
X86_FUNC(__imp__sub_00031127) {
	if (!ff_sdk_context_active(ctx, base) || (ff_probe_sdk_chain_word(0u)!=33u && (ff_probe_sdk_chain_word(0u)!=34u && (ff_probe_sdk_chain_word(0u)!=35u && (ff_probe_sdk_chain_word(0u)!=36u && (ff_probe_sdk_chain_word(0u)!=37u && (ff_probe_sdk_chain_word(0u)!=38u && (ff_probe_sdk_chain_word(0u)!=39u && (ff_probe_sdk_chain_word(0u)!=40u && (ff_probe_sdk_chain_word(0u)!=41u && (ff_probe_sdk_chain_word(0u)!=42u && (ff_probe_sdk_chain_word(0u)!=43u && (ff_probe_sdk_chain_word(0u)!=44u && ff_probe_sdk_chain_word(0u)!=45u)))))))))))) ||
	    ff_probe_sdk_chain_word(17u) != 0x0004F5E0u || ff_probe_sdk_chain_word(2956u) != 0u ||
	    ctx.esp != 0x03C10AECu)
		ff_sdk_raise(ctx, 0x00031127u, "original error helper outside exact profile-33 root/invocation");
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x00031127u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00031127 movzx eax, byte ptr fs:[0x00000024]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x24u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 0003112F cmp al, 0x02
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00031131 jnb 0x00031139
	if ((uint8_t)ca >= (uint8_t)cb) { goto loc_00031139; }
	// 00031133 mov eax, fs:[0x00000028]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
loc_00031139:
	// 00031139 mov ecx, fs:[0x00000004]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 00031140 mov eax, [0x002BB130]
	{ uint32_t ea = (uint32_t)(0x2BB130u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00031145 mov eax, [ecx+eax*4]
	{ uint32_t ea = (uint32_t)(ctx.ecx + ctx.eax * 4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00031148 mov ecx, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0003114C mov [eax+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 00031152 ret 0x04
	ff_sdk_observe_exit(ctx, base, 0x00031127u); ctx.esp += 8; return;
}
X86_WEAK_FUNC(sub_00031127, __imp__sub_00031127)

X86_FUNC(__imp__sub_001E4F99) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E4F99 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E4F9A call 0x001E2038
	X86_PUSH32(0x1E4F9Fu); sub_001E2038(ctx, base);
	// 001E4F9F mov ecx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E4FA3 mov [eax+0x0C], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E4FA6 xor esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a ^ b); ctx.esi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001E4FA8:
	// 001E4FA8 cmp ecx, [esi*8+0x2B7888]
	{ uint32_t ea = (uint32_t)(ctx.esi * 8u + 0x2B7888u); uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(X86_MEM32(ea)); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FAF jz 0x001E4FCF
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_001E4FCF; }
	// 001E4FB1 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FB2 cmp esi, 0x2D
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x2Du); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FB5 jb 0x001E4FA8
	if ((uint32_t)ca < (uint32_t)cb) { X86_BARRIER(); goto loc_001E4FA8; }
	// 001E4FB7 cmp ecx, 0x13
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x13u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FBA jb 0x001E4FE0
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001E4FE0; }
	// 001E4FBC cmp ecx, 0x24
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FBF jnbe 0x001E4FE0
	if ((uint32_t)ca > (uint32_t)cb) { goto loc_001E4FE0; }
	// 001E4FC1 call 0x001E2038
	X86_PUSH32(0x1E4FC6u); sub_001E2038(ctx, base);
	// 001E4FC6 mov dword ptr [eax+0x08], 0x0D
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0xDu); }
	// 001E4FCD pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E4FCE ret
	ctx.esp += 4; return;
loc_001E4FCF:
	// 001E4FCF call 0x001E2038
	X86_PUSH32(0x1E4FD4u); sub_001E2038(ctx, base);
	// 001E4FD4 mov ecx, [esi*8+0x2B788C]
	{ uint32_t ea = (uint32_t)(ctx.esi * 8u + 0x2B788Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E4FDB mov [eax+0x08], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E4FDE pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E4FDF ret
	ctx.esp += 4; return;
loc_001E4FE0:
	// 001E4FE0 cmp ecx, 0xBC
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0xBCu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FE6 jb 0x001E4FFE
	if ((uint32_t)ca < (uint32_t)cb) { goto loc_001E4FFE; }
	// 001E4FE8 cmp ecx, 0xCA
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0xCAu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E4FEE jnbe 0x001E4FFE
	if ((uint32_t)ca > (uint32_t)cb) { goto loc_001E4FFE; }
	// 001E4FF0 call 0x001E2038
	X86_PUSH32(0x1E4FF5u); sub_001E2038(ctx, base);
	// 001E4FF5 mov dword ptr [eax+0x08], 0x08
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x8u); }
	// 001E4FFC pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E4FFD ret
	ctx.esp += 4; return;
loc_001E4FFE:
	// 001E4FFE call 0x001E2038
	X86_PUSH32(0x1E5003u); sub_001E2038(ctx, base);
	// 001E5003 mov dword ptr [eax+0x08], 0x16
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM32(ea) = (uint32_t)(0x16u); }
	// 001E500A pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E500B ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001E4F99, __imp__sub_001E4F99)

X86_FUNC(__imp__sub_001E2038) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E2038 movzx eax, byte ptr fs:[0x00000024]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x24u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 001E2040 cmp al, 0x02
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E2042 jb 0x001E204C
	if ((uint8_t)ca < (uint8_t)cb) { goto loc_001E204C; }
	// 001E2044 push 0x0A
	{ uint32_t v = (uint32_t)(0xAu); X86_PUSH32(v); }
	// 001E2046 call [0x002534C0]
	X86_PUSH32(0x1E204Cu); x86_kernel(ctx, base, 95);
loc_001E204C:
	// 001E204C push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E204D push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E204E mov eax, fs:[0x00000028]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001E2054 cmp dword ptr [eax+0x28], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x28u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(0x0u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E2058 jz 0x001E2071
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_001E2071; }
	// 001E205A mov eax, [0x002BB130]
	{ uint32_t ea = (uint32_t)(0x2BB130u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001E205F mov ecx, fs:[0x00000004]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E2066 mov edi, [ecx+eax*4]
	{ uint32_t ea = (uint32_t)(ctx.ecx + ctx.eax * 4u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 001E2069 add edi, 0x08
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E206F jmp 0x001E2076
	goto loc_001E2076;
loc_001E2071:
	// 001E2071 mov edi, 0x31D974
	{ ctx.edi = (uint32_t)(0x31D974u); }
loc_001E2076:
	// 001E2076 mov esi, [edi]
	{ uint32_t ea = (uint32_t)(ctx.edi); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 001E2078 test esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E207A jnz 0x001E20B5
	if (((uint32_t)(ca & cb)) != 0) { goto loc_001E20B5; }
	// 001E207C push 0x84
	{ uint32_t v = (uint32_t)(0x84u); X86_PUSH32(v); }
	// 001E2081 push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001E2083 call 0x001E6AAF
	X86_PUSH32(0x1E2088u); sub_001E6AAF(ctx, base);
	// 001E2088 mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 001E208A test esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E208C pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001E208D pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
	// 001E208E jz 0x001E20AD
	if (((uint32_t)(ca & cb)) == 0) { goto loc_001E20AD; }
	// 001E2090 mov [edi], esi
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 001E2092 mov dword ptr [esi+0x54], 0x2B7D10
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x54u); X86_MEM32(ea) = (uint32_t)(0x2B7D10u); }
	// 001E2099 mov dword ptr [esi+0x14], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(0x1u); }
	// 001E20A0 call 0x000343F3
	X86_PUSH32(0x1E20A5u); sub_000343F3(ctx, base);
	// 001E20A5 or dword ptr [esi+0x04], 0xFFFFFFFF
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a | b); X86_MEM32(ea) = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E20A9 mov [esi], eax
	{ uint32_t ea = (uint32_t)(ctx.esi); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001E20AB jmp 0x001E20B5
	goto loc_001E20B5;
loc_001E20AD:
	// 001E20AD push 0x10
	{ uint32_t v = (uint32_t)(0x10u); X86_PUSH32(v); }
	// 001E20AF call 0x001E564E
	X86_PUSH32(0x1E20B4u); sub_001E564E(ctx, base);
	// 001E20B4 pop ecx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ecx = (uint32_t)(v); }
loc_001E20B5:
	// 001E20B5 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001E20B6 mov eax, esi
	{ ctx.eax = (uint32_t)(ctx.esi); }
	// 001E20B8 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E20B9 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_001E2038, __imp__sub_001E2038)

X86_FUNC(__imp__sub_000310FF) {
	X86_LOAD_FLAGS();
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 000310FF movzx eax, byte ptr fs:[0x00000024]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x24u); ctx.eax = (uint32_t)((uint8_t)(X86_MEM8(ea))); }
	// 00031107 cmp al, 0x02
	{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)), b = (uint8_t)(0x2u); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 00031109 jnb 0x00031111
	if ((uint8_t)ca >= (uint8_t)cb) { goto loc_00031111; }
	// 0003110B mov eax, fs:[0x00000028]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x28u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
loc_00031111:
	// 00031111 mov eax, [0x002BB130]
	{ uint32_t ea = (uint32_t)(0x2BB130u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00031116 mov ecx, fs:[0x00000004]
	{ uint32_t ea = (uint32_t)(ctx.fs_base + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0003111D mov eax, [ecx+eax*4]
	{ uint32_t ea = (uint32_t)(ctx.ecx + ctx.eax * 4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00031120 mov eax, [eax+0x04]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00031126 ret
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_000310FF, __imp__sub_000310FF)