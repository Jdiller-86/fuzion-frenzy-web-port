#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_001E956F) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x001E956Fu);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E956F mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001E9573 mov ecx, [eax+0x38]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x38u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E9576 mov edx, [esp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x8u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001E957A mov [edx], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E957C mov ecx, [eax+0x08]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E957F mov [edx+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E9582 mov ecx, [eax+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E9585 mov [edx+0x08], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x8u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E9588 mov ecx, [eax+0x10]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E958B mov [edx+0x0C], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E958E mov ecx, [eax+0x14]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E9591 mov [edx+0x10], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E9594 mov ecx, [eax+0x18]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E9597 mov [edx+0x14], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E959A mov ecx, [eax+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E959D mov [edx+0x18], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E95A0 mov ecx, [eax+0x2C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E95A3 mov [edx+0x1C], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E95A6 mov ecx, [eax+0x28]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x28u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E95A9 mov [edx+0x20], ecx
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x20u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 001E95AC mov ecx, [eax+0x3C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x3Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E95AF push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 001E95B0 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E95B1 mov ebx, ecx
	{ ctx.ebx = (uint32_t)(ctx.ecx); }
	// 001E95B3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E95B4 shr ecx, 0x02
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t c = (uint32_t)(0x2u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a >> c); cf = c <= 32 ? ((uint32_t)a >> (c - 1)) & 1 : 0; of = (uint32_t)(a >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.ecx = (uint32_t)(r); } }
	// 001E95B7 lea esi, [eax+0x40]
	ctx.esi = (uint32_t)((uint32_t)(ctx.eax + 0x40u));
	// 001E95BA lea edi, [edx+0x2C]
	ctx.edi = (uint32_t)((uint32_t)(ctx.edx + 0x2Cu));
	// 001E95BD rep movsd
	{ x86_rep_movs(ctx, base, 4); }
	// 001E95BF mov ecx, ebx
	{ ctx.ecx = (uint32_t)(ctx.ebx); }
	// 001E95C1 and ecx, 0x03
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(0x3u); uint32_t r = (uint32_t)(a & b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E95C4 rep movsb
	{ x86_rep_movs(ctx, base, 1); }
	// 001E95C6 mov eax, [eax+0x3C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x3Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001E95C9 and byte ptr [eax+edx*1+0x2C], 0x00
	{ uint32_t ea = (uint32_t)(ctx.eax + ctx.edx + 0x2Cu); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E95CE and byte ptr [edx+0x130], 0x00
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x130u); uint8_t a = (uint8_t)(X86_MEM8(ea)); uint8_t b = (uint8_t)(0x0u); uint8_t r = (uint8_t)(a & b); X86_MEM8(ea) = (uint8_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E95D5 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001E95D6 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E95D7 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 001E95D8 ret 0x08
	ff_sdk_observe_exit(ctx, base, 0x001E956Fu);
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_001E956F, __imp__sub_001E956F)