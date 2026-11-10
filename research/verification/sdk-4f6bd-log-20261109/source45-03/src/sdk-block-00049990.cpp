#include "sdk-shared-prototypes.hpp"
#include "shared-abi.h"
X86_FUNC(__imp__sub_00049990) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x00049990u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 00049990 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00049991 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00049992 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 00049993 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049995 mov ecx, 0x24
	{ ctx.ecx = (uint32_t)(0x24u); }
	// 0004999A mov edi, 0x2CC1F0
	{ ctx.edi = (uint32_t)(0x2CC1F0u); }
	// 0004999F rep stosd
	{ x86_rep_stos(ctx, base, 4); }
	// 000499A1 mov [0x002CC3A8], eax
	{ uint32_t ea = (uint32_t)(0x2CC3A8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 000499A6 mov [0x002CC3AC], eax
	{ uint32_t ea = (uint32_t)(0x2CC3ACu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 000499AB mov [0x002CC3B0], eax
	{ uint32_t ea = (uint32_t)(0x2CC3B0u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 000499B0 xor esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a ^ b); ctx.esi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000499B2 push 0x24C2BC
	{ uint32_t v = (uint32_t)(0x24C2BCu); X86_PUSH32(v); }
	// 000499B7 mov [0x002CC3B4], eax
	{ uint32_t ea = (uint32_t)(0x2CC3B4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 000499BC mov [0x002CC280], esi
	{ uint32_t ea = (uint32_t)(0x2CC280u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 000499C2 call 0x0024D37E
	X86_PUSH32(0x499C7u); sub_0024D37E(ctx, base);
	// 000499C7 mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 000499C9 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 000499CA push 0x25B55C
	{ uint32_t v = (uint32_t)(0x25B55Cu); X86_PUSH32(v); }
	// 000499CF call 0x0004B230
	X86_PUSH32(0x499D4u); sub_0004B230(ctx, base);
	// 000499D4 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000499D7 mov edi, 0x2CC1F0
	{ ctx.edi = (uint32_t)(0x2CC1F0u); }
	// 000499DC lea esp, [esp]
	ctx.esp = (uint32_t)((uint32_t)(ctx.esp));
loc_000499E0:
	// 000499E0 test [esi*4+0x285130], ebx
	{ uint32_t ea = (uint32_t)(ctx.esi * 4u + 0x285130u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000499E7 mov [edi], esi
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 000499E9 jz 0x00049A41
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00049A41; }
	// 000499EB xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000499ED cmp esi, 0x04
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000499F0 setnl cl
	{ ctx.ecx = (ctx.ecx & 0xFFFFFF00u) | (uint8_t)(((int32_t)ca >= (int32_t)cb) ? 1u : 0u); }
	// 000499F3 mov edx, esi
	{ ctx.edx = (uint32_t)(ctx.esi); }
	// 000499F5 and edx, 0x80000003
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0x80000003u); uint32_t r = (uint32_t)(a & b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 000499FB push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 000499FD mov dword ptr [edi+0x04], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); X86_MEM32(ea) = (uint32_t)(0x1u); }
	// 00049A04 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 00049A05 jns 0x00049A0C
	if (!sf) { goto loc_00049A0C; }
	// 00049A07 dec edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(a - 1); ctx.edx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A08 or edx, 0xFFFFFFFC
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(0xFFFFFFFCu); uint32_t r = (uint32_t)(a | b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A0B inc edx
	{ uint32_t a = (uint32_t)(ctx.edx);  uint32_t r = (uint32_t)(a + 1); ctx.edx = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_00049A0C:
	// 00049A0C push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 00049A0D push 0x24C2BC
	{ uint32_t v = (uint32_t)(0x24C2BCu); X86_PUSH32(v); }
	// 00049A12 call 0x0024D22D
	X86_PUSH32(0x49A17u); sub_0024D22D(ctx, base);
	// 00049A17 mov [edi+0x08], eax
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00049A1A mov eax, [0x002CC280]
	{ uint32_t ea = (uint32_t)(0x2CC280u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00049A1F mov [eax*4+0x2CC3A8], edi
	{ uint32_t ea = (uint32_t)(ctx.eax * 4u + 0x2CC3A8u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 00049A26 mov eax, [0x002CC280]
	{ uint32_t ea = (uint32_t)(0x2CC280u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00049A2B inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A2C push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00049A2D push 0x25B528
	{ uint32_t v = (uint32_t)(0x25B528u); X86_PUSH32(v); }
	// 00049A32 mov [0x002CC280], eax
	{ uint32_t ea = (uint32_t)(0x2CC280u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00049A37 call 0x0004B230
	X86_PUSH32(0x49A3Cu); sub_0004B230(ctx, base);
	// 00049A3C add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A3F jmp 0x00049A48
	goto loc_00049A48;
loc_00049A41:
	// 00049A41 mov dword ptr [edi+0x04], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edi + 0x4u); X86_MEM32(ea) = (uint32_t)(0x0u); }
loc_00049A48:
	// 00049A48 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A49 add edi, 0x24
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A4C cmp esi, 0x04
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A4F jl 0x000499E0
	if ((int32_t)ca < (int32_t)cb) { X86_BARRIER(); goto loc_000499E0; }
	// 00049A51 push 0x24C238
	{ uint32_t v = (uint32_t)(0x24C238u); X86_PUSH32(v); }
	// 00049A56 call 0x0024D37E
	X86_PUSH32(0x49A5Bu); sub_0024D37E(ctx, base);
	// 00049A5B mov ebx, eax
	{ ctx.ebx = (uint32_t)(ctx.eax); }
	// 00049A5D push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 00049A5E push 0x25B4F8
	{ uint32_t v = (uint32_t)(0x25B4F8u); X86_PUSH32(v); }
	// 00049A63 call 0x0004B230
	X86_PUSH32(0x49A68u); sub_0004B230(ctx, base);
	// 00049A68 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A6B xor esi, esi
	{ uint32_t a = (uint32_t)(ctx.esi); uint32_t b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a ^ b); ctx.esi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A6D mov edi, 0x2CC288
	{ ctx.edi = (uint32_t)(0x2CC288u); }
loc_00049A72:
	// 00049A72 test [esi*4+0x285130], ebx
	{ uint32_t ea = (uint32_t)(ctx.esi * 4u + 0x285130u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.ebx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A79 mov [edi-0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.edi + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 00049A7C jz 0x00049A9F
	if (((uint32_t)(ca & cb)) == 0) { goto loc_00049A9F; }
	// 00049A7E mov dword ptr [edi], 0x01
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(0x1u); }
	// 00049A84 mov eax, [0x002CC3A4]
	{ uint32_t ea = (uint32_t)(0x2CC3A4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 00049A89 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A8A push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 00049A8B push 0x25B4BC
	{ uint32_t v = (uint32_t)(0x25B4BCu); X86_PUSH32(v); }
	// 00049A90 mov [0x002CC3A4], eax
	{ uint32_t ea = (uint32_t)(0x2CC3A4u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 00049A95 call 0x0004B230
	X86_PUSH32(0x49A9Au); sub_0004B230(ctx, base);
	// 00049A9A add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049A9D jmp 0x00049AA5
	goto loc_00049AA5;
loc_00049A9F:
	// 00049A9F mov dword ptr [edi], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edi); X86_MEM32(ea) = (uint32_t)(0x0u); }
loc_00049AA5:
	// 00049AA5 inc esi
	{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049AA6 add edi, 0x24
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(0x24u); uint32_t r = (uint32_t)(a + b); ctx.edi = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049AA9 cmp esi, 0x08
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 00049AAC jl 0x00049A72
	if ((int32_t)ca < (int32_t)cb) { X86_BARRIER(); goto loc_00049A72; }
	// 00049AAE pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 00049AAF pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 00049AB0 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 00049AB1 ret
	if(ff_probe_sdk_chain_word(0u)>=13u) ff_sdk_observe_exit(ctx,base,0x00049990u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_00049990, __imp__sub_00049990)
