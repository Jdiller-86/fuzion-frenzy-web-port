#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0004BE40) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004BE40u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004BE40 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0004BE41 mov edi, [0x002CCEA8]
	{ uint32_t ea = (uint32_t)(0x2CCEA8u); ctx.edi = (uint32_t)(X86_MEM32(ea)); }
	// 0004BE47 cmp edi, 0x2CCE98
	{ uint32_t a = (uint32_t)(ctx.edi), b = (uint32_t)(0x2CCE98u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BE4D jnz 0x0004BE69
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_0004BE69; }
	// 0004BE4F mov [0x002CCEAC], eax
	{ uint32_t ea = (uint32_t)(0x2CCEACu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BE54 mov [0x002CCEA8], eax
	{ uint32_t ea = (uint32_t)(0x2CCEA8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BE59 mov dword ptr [eax+0x14], 0x2CCE98
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); X86_MEM32(ea) = (uint32_t)(0x2CCE98u); }
	// 0004BE60 mov dword ptr [eax+0x10], 0x2CCE98
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); X86_MEM32(ea) = (uint32_t)(0x2CCE98u); }
	// 0004BE67 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004BE68 ret
	ff_sdk_observe_exit(ctx, base, 0x0004BE40u);
	ctx.esp += 4; return;
loc_0004BE69:
	// 0004BE69 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004BE6A mov esi, [eax]
	{ uint32_t ea = (uint32_t)(ctx.eax); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0004BE6C mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 0004BE6E mov edi, edi
	{ ctx.edi = (uint32_t)(ctx.edi); }
loc_0004BE70:
	// 0004BE70 cmp [ecx], esi
	{ uint32_t ea = (uint32_t)(ctx.ecx); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BE72 jnl 0x0004BE83
	if ((int32_t)ca >= (int32_t)cb) { goto loc_0004BE83; }
	// 0004BE74 mov edx, [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BE77 cmp edx, 0x2CCE98
	{ uint32_t a = (uint32_t)(ctx.edx), b = (uint32_t)(0x2CCE98u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BE7D jz 0x0004BEA6
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004BEA6; }
	// 0004BE7F cmp [edx], esi
	{ uint32_t ea = (uint32_t)(ctx.edx); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.esi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BE81 jnle 0x0004BEA6
	if ((int32_t)ca > (int32_t)cb) { goto loc_0004BEA6; }
loc_0004BE83:
	// 0004BE83 mov ecx, [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BE86 cmp ecx, 0x2CCE98
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(0x2CCE98u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BE8C jnz 0x0004BE70
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_0004BE70; }
	// 0004BE8E mov [eax+0x10], edi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0004BE91 mov dword ptr [eax+0x14], 0x2CCE98
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); X86_MEM32(ea) = (uint32_t)(0x2CCE98u); }
	// 0004BE98 mov ecx, edi
	{ ctx.ecx = (uint32_t)(ctx.edi); }
	// 0004BE9A pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004BE9B mov [ecx+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BE9E mov edx, [eax+0x14]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BEA1 mov [edx+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BEA4 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004BEA5 ret
	ff_sdk_observe_exit(ctx, base, 0x0004BE40u);
	ctx.esp += 4; return;
loc_0004BEA6:
	// 0004BEA6 mov edx, [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BEA9 mov [eax+0x14], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004BEAC mov [eax+0x10], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0004BEAF mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
	// 0004BEB1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004BEB2 mov [ecx+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BEB5 mov edx, [eax+0x14]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BEB8 mov [edx+0x10], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BEBB pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004BEBC ret
	ff_sdk_observe_exit(ctx, base, 0x0004BE40u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004BE40, __imp__sub_0004BE40)