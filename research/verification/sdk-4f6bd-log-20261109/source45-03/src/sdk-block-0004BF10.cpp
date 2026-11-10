#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0004BF10) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004BF10u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004BF10 mov eax, [0x00285338]
	{ uint32_t ea = (uint32_t)(0x285338u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF15 cmp eax, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF18 jz 0x0004BF36
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004BF36; }
	// 0004BF1A push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004BF1B push 0x25BB3C
	{ uint32_t v = (uint32_t)(0x25BB3Cu); X86_PUSH32(v); }
	// 0004BF20 call 0x0004B230
	X86_PUSH32(0x4BF25u); sub_0004B230(ctx, base);
	// 0004BF25 mov eax, [0x00285338]
	{ uint32_t ea = (uint32_t)(0x285338u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF2A shl eax, 0x0A
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0xAu) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 0004BF2D add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF30 add eax, 0x2CCF00
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0x2CCF00u); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF35 ret
	ff_sdk_observe_exit(ctx, base, 0x0004BF10u);
	ctx.esp += 4; return;
loc_0004BF36:
	// 0004BF36 mov ecx, [esp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF3A test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF3C mov eax, [esp+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x4u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF40 jz 0x0004BF44
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004BF44; }
	// 0004BF42 add eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_0004BF44:
	// 0004BF44 mov ecx, [0x002CD78C]
	{ uint32_t ea = (uint32_t)(0x2CD78Cu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF4A test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF4C jnz 0x0004BF6B
	if (((uint32_t)(ca & cb)) != 0) { goto loc_0004BF6B; }
	// 0004BF4E mov ecx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF52 mov edx, [esp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0xCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF56 push 0xDEFA
	{ uint32_t v = (uint32_t)(0xDEFAu); X86_PUSH32(v); }
	// 0004BF5B push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004BF5C mov ecx, [esp+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF60 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004BF61 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004BF62 call 0x0004BEC0
	X86_PUSH32(0x4BF67u); sub_0004BEC0(ctx, base);
	// 0004BF67 add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF6A ret
	ff_sdk_observe_exit(ctx, base, 0x0004BF10u);
	ctx.esp += 4; return;
loc_0004BF6B:
	// 0004BF6B mov edx, [0x002CD794]
	{ uint32_t ea = (uint32_t)(0x2CD794u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF71 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 0004BF72 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004BF73 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 0004BF74 lea ebx, [eax+0x0F]
	ctx.ebx = (uint32_t)((uint32_t)(ctx.eax + 0xFu));
	// 0004BF77 push 0xFFFFFFFF
	{ uint32_t v = (uint32_t)(0xFFFFFFFFu); X86_PUSH32(v); }
	// 0004BF79 and ebx, 0xFFFFFFF0
	{ uint32_t a = (uint32_t)(ctx.ebx); uint32_t b = (uint32_t)(0xFFFFFFF0u); uint32_t r = (uint32_t)(a & b); ctx.ebx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF7C push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004BF7D lea edi, [ebx+0x30]
	ctx.edi = (uint32_t)((uint32_t)(ctx.ebx + 0x30u));
	// 0004BF80 call 0x00030943
	X86_PUSH32(0x4BF85u); sub_00030943(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004BF85u);
	// 0004BF85 mov esi, [0x002CCEA8]
	{ uint32_t ea = (uint32_t)(0x2CCEA8u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF8B cmp esi, 0x2CCE98
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x2CCE98u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF91 jz 0x0004BFAD
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004BFAD; }
loc_0004BF93:
	// 0004BF93 cmp [esi+0x04], edi
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BF96 jnb 0x0004BFA5
	if ((uint32_t)ca >= (uint32_t)cb) { goto loc_0004BFA5; }
	// 0004BF98 mov esi, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.esi = (uint32_t)(X86_MEM32(ea)); }
	// 0004BF9B cmp esi, 0x2CCE98
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x2CCE98u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFA1 jnz 0x0004BF93
	if ((uint32_t)ca != (uint32_t)cb) { X86_BARRIER(); goto loc_0004BF93; }
	// 0004BFA3 jmp 0x0004BFAD
	goto loc_0004BFAD;
loc_0004BFA5:
	// 0004BFA5 cmp esi, 0x2CCE98
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0x2CCE98u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFAB jnz 0x0004BFD6
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_0004BFD6; }
loc_0004BFAD:
	// 0004BFAD mov eax, [0x002CD794]
	{ uint32_t ea = (uint32_t)(0x2CD794u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFB2 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004BFB3 call 0x0003080D
	X86_PUSH32(0x4BFB8u); sub_0003080D(ctx, base);
	// 0004BFB8 mov ecx, [esp+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFBC mov edx, [esp+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x14u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFC0 push ebx
	{ uint32_t v = (uint32_t)(ctx.ebx); X86_PUSH32(v); }
	// 0004BFC1 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004BFC2 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004BFC3 push 0x25BB00
	{ uint32_t v = (uint32_t)(0x25BB00u); X86_PUSH32(v); }
	// 0004BFC8 call 0x0004B230
	X86_PUSH32(0x4BFCDu); sub_0004B230(ctx, base);
	// 0004BFCD add esp, 0x10
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x10u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFD0 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004BFD1 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004BFD2 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFD4 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 0004BFD5 ret
	ff_sdk_observe_exit(ctx, base, 0x0004BF10u);
	ctx.esp += 4; return;
loc_0004BFD6:
	// 0004BFD6 mov eax, [0x002CCEC8]
	{ uint32_t ea = (uint32_t)(0x2CCEC8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFDB mov ecx, [0x002CD700]
	{ uint32_t ea = (uint32_t)(0x2CD700u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFE1 add eax, edi
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFE3 cmp eax, ecx
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFE5 mov [0x002CCEC8], eax
	{ uint32_t ea = (uint32_t)(0x2CCEC8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004BFEA jle 0x0004BFF1
	if ((int32_t)ca <= (int32_t)cb) { goto loc_0004BFF1; }
	// 0004BFEC mov [0x002CD700], eax
	{ uint32_t ea = (uint32_t)(0x2CD700u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_0004BFF1:
	// 0004BFF1 mov ecx, [esi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFF4 mov ebx, [esp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); ctx.ebx = (uint32_t)(X86_MEM32(ea)); }
	// 0004BFF8 lea eax, [edi+0x30]
	ctx.eax = (uint32_t)((uint32_t)(ctx.edi + 0x30u));
	// 0004BFFB cmp ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004BFFD push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 0004BFFE mov ebp, [0x002CCE88]
	{ uint32_t ea = (uint32_t)(0x2CCE88u); ctx.ebp = (uint32_t)(X86_MEM32(ea)); }
	// 0004C004 jbe 0x0004C074
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_0004C074; }
	// 0004C006 mov ecx, [esi]
	{ uint32_t ea = (uint32_t)(ctx.esi); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C008 mov edx, [0x002CCE90]
	{ uint32_t ea = (uint32_t)(0x2CCE90u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C00E lea eax, [ecx+edx*1]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx + ctx.edx));
	// 0004C011 add eax, edi
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a + b); ctx.eax = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C013 add ecx, edi
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a + b); ctx.ecx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C015 mov [eax], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004C017 mov ecx, [esi+0x04]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C01A sub ecx, edi
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C01C mov [eax+0x04], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004C01F xor ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a ^ b); ctx.ecx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C021 mov [eax+0x08], cx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x8u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 0004C025 mov [eax+0x0A], cx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xAu); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 0004C029 mov edx, [esi+0x18]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x18u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C02C mov [eax+0x18], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0004C02F mov [eax+0x1C], esi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0004C032 mov ecx, edx
	{ ctx.ecx = (uint32_t)(ctx.edx); }
	// 0004C034 mov [ecx+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004C037 mov edx, [eax+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x1Cu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C03A mov [edx+0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004C03D mov [eax+0x0C], ebx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 0004C040 mov [esi+0x04], edi
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x4u); X86_MEM32(ea) = (uint32_t)(ctx.edi); }
	// 0004C043 call 0x0004BE40
	X86_PUSH32(0x4C048u); sub_0004BE40(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004C048u);
	// 0004C048 mov ecx, [eax+0x18]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C04B cmp ecx, ebp
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ebp); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C04D jz 0x0004C074
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004C074; }
	// 0004C04F cmp word ptr [ecx+0x08], 0x00
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x8u); uint16_t a = (uint16_t)(X86_MEM16(ea)), b = (uint16_t)(0x0u); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 0004C054 jnz 0x0004C074
	if ((uint16_t)ca != (uint16_t)cb) { goto loc_0004C074; }
	// 0004C056 mov ecx, [ecx+0x10]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C059 mov [eax+0x10], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004C05C mov [ecx+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004C05F mov ecx, [eax+0x18]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C062 mov edx, [ecx+0x04]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x4u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C065 add [eax+0x04], edx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x4u); uint32_t a = (uint32_t)(X86_MEM32(ea)); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a + b); X86_MEM32(ea) = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C068 mov ecx, [eax+0x18]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C06B mov ecx, [ecx+0x18]
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x18u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C06E mov [eax+0x18], ecx
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x18u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004C071 mov [ecx+0x1C], eax
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x1Cu); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_0004C074:
	// 0004C074 mov edx, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C077 mov eax, [esi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004C07A mov [edx+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 0004C07D mov ecx, [esi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C080 mov edx, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C083 mov ax, [esp+0x20]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x20u); ctx.eax = (ctx.eax & 0xFFFF0000u) | (uint16_t)(X86_MEM16(ea)); }
	// 0004C088 mov [ecx+0x10], edx
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0004C08B cmp esi, ebp
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.ebp); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C08D mov word ptr [esi+0x08], 0x01
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x8u); X86_MEM16(ea) = (uint16_t)(0x1u); }
	// 0004C093 mov [esi+0x0A], ax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xAu); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 0004C097 mov [esi+0x0C], ebx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0xCu); X86_MEM32(ea) = (uint32_t)(ctx.ebx); }
	// 0004C09A pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 0004C09B jnz 0x0004C0AF
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_0004C0AF; }
	// 0004C09D mov ecx, [0x002CCEDC]
	{ uint32_t ea = (uint32_t)(0x2CCEDCu); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0A3 mov [esi+0x10], ecx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.ecx); }
	// 0004C0A6 mov dword ptr [esi+0x14], 0x2CCECC
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(0x2CCECCu); }
	// 0004C0AD jmp 0x0004C0BB
	goto loc_0004C0BB;
loc_0004C0AF:
	// 0004C0AF mov eax, [esi+0x1C]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x1Cu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0B2 mov edx, [eax+0x10]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x10u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0B5 mov [esi+0x10], edx
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 0004C0B8 mov [esi+0x14], eax
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
loc_0004C0BB:
	// 0004C0BB mov eax, [esi+0x10]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x10u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0BE mov edx, [0x002CD794]
	{ uint32_t ea = (uint32_t)(0x2CD794u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0C4 mov [eax+0x14], esi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x14u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0004C0C7 mov ecx, [esi+0x14]
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x14u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0CA mov [ecx+0x10], esi
	{ uint32_t ea = (uint32_t)(ctx.ecx + 0x10u); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0004C0CD push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004C0CE mov dword ptr [esi+0x2C], 0xDEADBEEF
	{ uint32_t ea = (uint32_t)(ctx.esi + 0x2Cu); X86_MEM32(ea) = (uint32_t)(0xDEADBEEFu); }
	// 0004C0D5 call 0x0003080D
	X86_PUSH32(0x4C0DAu); sub_0003080D(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004C0DAu);
	// 0004C0DA mov ecx, [esp+0x24]
	{ uint32_t ea = (uint32_t)(ctx.esp + 0x24u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004C0DE test ecx, ecx
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.ecx); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C0E0 jz 0x0004C0FC
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004C0FC; }
	// 0004C0E2 dec ecx
	{ uint32_t a = (uint32_t)(ctx.ecx);  uint32_t r = (uint32_t)(a - 1); ctx.ecx = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C0E3 lea eax, [esi+0x31]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esi + 0x31u));
	// 0004C0E6 test ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C0E8 jz 0x0004C0F5
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004C0F5; }
	// 0004C0EA lea ebx, [ebx]
	ctx.ebx = (uint32_t)((uint32_t)(ctx.ebx));
loc_0004C0F0:
	// 0004C0F0 inc eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a + 1); ctx.eax = (uint32_t)(r); of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C0F1 test ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004C0F3 jnz 0x0004C0F0
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_0004C0F0; }
loc_0004C0F5:
	// 0004C0F5 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004C0F6 mov [eax-0x04], esi
	{ uint32_t ea = (uint32_t)(ctx.eax + 0xFFFFFFFCu); X86_MEM32(ea) = (uint32_t)(ctx.esi); }
	// 0004C0F9 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004C0FA pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 0004C0FB ret
	ff_sdk_observe_exit(ctx, base, 0x0004BF10u);
	ctx.esp += 4; return;
loc_0004C0FC:
	// 0004C0FC pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004C0FD lea eax, [esi+0x30]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esi + 0x30u));
	// 0004C100 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 0004C101 pop ebx
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebx = (uint32_t)(v); }
	// 0004C102 ret
	ff_sdk_observe_exit(ctx, base, 0x0004BF10u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004BF10, __imp__sub_0004BF10)