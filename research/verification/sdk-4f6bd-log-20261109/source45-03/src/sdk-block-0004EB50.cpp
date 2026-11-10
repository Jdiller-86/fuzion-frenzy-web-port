#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_0004EB50) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x0004EB50u);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 0004EB50 sub esp, 0x1F4
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x1F4u); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EB56 push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 0004EB57 lea eax, [esp+0x38]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x38u));
	// 0004EB5B push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004EB5C push 0x25C4A8
	{ uint32_t v = (uint32_t)(0x25C4A8u); X86_PUSH32(v); }
	// 0004EB61 call 0x001E95DB
	X86_PUSH32(0x4EB66u); sub_001E95DB(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004EB66u);
	// 0004EB66 mov ebp, eax
	{ ctx.ebp = (uint32_t)(ctx.eax); }
	// 0004EB68 cmp ebp, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.ebp), b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EB6B jz 0x0004ECE4
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004ECE4; }
	// 0004EB71 push 0x25C470
	{ uint32_t v = (uint32_t)(0x25C470u); X86_PUSH32(v); }
	// 0004EB76 call 0x0004B230
	X86_PUSH32(0x4EB7Bu); sub_0004B230(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004EB7Bu);
	// 0004EB7B add esp, 0x04
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EB7E push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004EB7F push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
loc_0004EB80:
	// 0004EB80 lea ecx, [esp+0x6C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EB84 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004EB85 push 0x25C468
	{ uint32_t v = (uint32_t)(0x25C468u); X86_PUSH32(v); }
	// 0004EB8A call 0x0004E960
	X86_PUSH32(0x4EB8Fu); sub_0004E960(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004EB8Fu);
	// 0004EB8F add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EB92 mov edi, 0x25C464
	{ ctx.edi = (uint32_t)(0x25C464u); }
	// 0004EB97 lea esi, [esp+0x6C]
	ctx.esi = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EB9B mov ecx, 0x02
	{ ctx.ecx = (uint32_t)(0x2u); }
	// 0004EBA0 xor edx, edx
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.edx); uint32_t r = (uint32_t)(a ^ b); ctx.edx = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EBA2 repe cmpsb
	{ int32_t d = ctx.df ? -1 : 1; while (ctx.ecx) { uint8_t a = X86_MEM8(ctx.esi), b = X86_MEM8(ctx.edi); ctx.esi += d; ctx.edi += d; uint8_t r = (uint8_t)(a - b); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); ctx.ecx--; if (!zf) break; } }
	// 0004EBA4 jz 0x0004ECC9
	if (zf) { goto loc_0004ECC9; }
	// 0004EBAA mov edi, 0x25C460
	{ ctx.edi = (uint32_t)(0x25C460u); }
	// 0004EBAF lea esi, [esp+0x6C]
	ctx.esi = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EBB3 mov ecx, 0x03
	{ ctx.ecx = (uint32_t)(0x3u); }
	// 0004EBB8 xor eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a ^ b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EBBA repe cmpsb
	{ int32_t d = ctx.df ? -1 : 1; while (ctx.ecx) { uint8_t a = X86_MEM8(ctx.esi), b = X86_MEM8(ctx.edi); ctx.esi += d; ctx.edi += d; uint8_t r = (uint8_t)(a - b); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); ctx.ecx--; if (!zf) break; } }
	// 0004EBBC jz 0x0004ECC9
	if (zf) { goto loc_0004ECC9; }
	// 0004EBC2 lea ecx, [esp+0x6C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EBC6 push 0x25C450
	{ uint32_t v = (uint32_t)(0x25C450u); X86_PUSH32(v); }
	// 0004EBCB push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004EBCC call 0x001E056D
	X86_PUSH32(0x4EBD1u); sub_001E056D(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004EBD1u);
	// 0004EBD1 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EBD4 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EBD6 jz 0x0004ECC9
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004ECC9; }
	// 0004EBDC lea edx, [esp+0x6C]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EBE0 push 0x25C440
	{ uint32_t v = (uint32_t)(0x25C440u); X86_PUSH32(v); }
	// 0004EBE5 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004EBE6 call 0x001E056D
	X86_PUSH32(0x4EBEBu); sub_001E056D(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x0004EBEBu);
	// 0004EBEB add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EBEE test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EBF0 jz 0x0004ECC9
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004ECC9; }
	// 0004EBF6 lea eax, [esp+0x6C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EBFA push 0x2E
	{ uint32_t v = (uint32_t)(0x2Eu); X86_PUSH32(v); }
	// 0004EBFC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004EBFD call 0x001E0EA0
	X86_PUSH32(0x4EC02u); sub_001E0EA0(ctx, base);
	// 0004EC02 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EC05 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EC07 jz 0x0004ECC9
	if (((uint32_t)(ca & cb)) == 0) { goto loc_0004ECC9; }
	// 0004EC0D push 0x25C438
	{ uint32_t v = (uint32_t)(0x25C438u); X86_PUSH32(v); }
	// 0004EC12 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004EC13 call 0x001E056D
	X86_PUSH32(0x4EC18u); sub_001E056D(ctx, base);
	// 0004EC18 add esp, 0x08
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x8u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EC1B test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EC1D jnz 0x0004ECC9
	if (((uint32_t)(ca & cb)) != 0) { goto loc_0004ECC9; }
	// 0004EC23 lea ecx, [esp+0x6C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EC27 push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004EC28 lea edx, [esp+0x184]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x184u));
	// 0004EC2F push 0x25C430
	{ uint32_t v = (uint32_t)(0x25C430u); X86_PUSH32(v); }
	// 0004EC34 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004EC35 call 0x001DEE6E
	X86_PUSH32(0x4EC3Au); sub_001DEE6E(ctx, base);
	// 0004EC3A add esp, 0x0C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0xCu); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EC3D push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004EC3F push 0x80
	{ uint32_t v = (uint32_t)(0x80u); X86_PUSH32(v); }
	// 0004EC44 push 0x03
	{ uint32_t v = (uint32_t)(0x3u); X86_PUSH32(v); }
	// 0004EC46 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004EC48 push 0x00
	{ uint32_t v = (uint32_t)(0x0u); X86_PUSH32(v); }
	// 0004EC4A push 0x80000000
	{ uint32_t v = (uint32_t)(0x80000000u); X86_PUSH32(v); }
	// 0004EC4F lea eax, [esp+0x198]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x198u));
	// 0004EC56 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004EC57 call 0x001E93E4
	X86_PUSH32(0x4EC5Cu); sub_001E93E4(ctx, base);
	// 0004EC5C mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 0004EC5E cmp esi, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004EC61 jnz 0x0004EC72
	if ((uint32_t)ca != (uint32_t)cb) { goto loc_0004EC72; }
	// 0004EC63 lea ecx, [esp+0x180]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x180u));
	// 0004EC6A push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004EC6B call 0x001E97CF
	X86_PUSH32(0x4EC70u); sub_001E97CF(ctx, base);
	// 0004EC70 jmp 0x0004ECC9
	goto loc_0004ECC9;
loc_0004EC72:
	// 0004EC72 lea edx, [esp+0x0C]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0xCu));
	// 0004EC76 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004EC77 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004EC78 call 0x001E91CA
	X86_PUSH32(0x4EC7Du); sub_001E91CA(ctx, base);
	// 0004EC7D lea eax, [esp+0x10]
	ctx.eax = (uint32_t)((uint32_t)(ctx.esp + 0x10u));
	// 0004EC81 lea ecx, [esp+0x6C]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x6Cu));
	// 0004EC85 call 0x0004EAF0
	X86_PUSH32(0x4EC8Au); sub_0004EAF0(ctx, base);
	// 0004EC8A push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 0004EC8B call 0x0003417A
	X86_PUSH32(0x4EC90u); sub_0003417A(ctx, base);
	// 0004EC90 mov ecx, [0x002D73A8]
	{ uint32_t ea = (uint32_t)(0x2D73A8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 0004EC96 lea eax, [ecx+ecx*2]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ecx + ctx.ecx * 2u));
	// 0004EC99 shl eax, 0x03
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t c = (uint32_t)(0x3u) & 31; if (c) { uint32_t r = (uint32_t)((uint32_t)a << c); cf = c <= 32 ? ((uint32_t)a >> (32 - c)) & 1 : 0; of = ((uint32_t)(r >> 31) & 1) ^ cf; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); ctx.eax = (uint32_t)(r); } }
	// 0004EC9C mov edx, [eax+0x2D15E0]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2D15E0u); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 0004ECA2 mov eax, [eax+0x2D15DC]
	{ uint32_t ea = (uint32_t)(ctx.eax + 0x2D15DCu); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004ECA8 push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004ECA9 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 0004ECAA push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004ECAB lea ecx, [esp+0x78]
	ctx.ecx = (uint32_t)((uint32_t)(ctx.esp + 0x78u));
	// 0004ECAF push ecx
	{ uint32_t v = (uint32_t)(ctx.ecx); X86_PUSH32(v); }
	// 0004ECB0 push 0x25C3F4
	{ uint32_t v = (uint32_t)(0x25C3F4u); X86_PUSH32(v); }
	// 0004ECB5 call 0x0004E960
	X86_PUSH32(0x4ECBAu); sub_0004E960(ctx, base);
	// 0004ECBA mov eax, [0x002D73A8]
	{ uint32_t ea = (uint32_t)(0x2D73A8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 0004ECBF add esp, 0x14
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x14u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004ECC2 cmp eax, 0x3E8
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(0x3E8u); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004ECC7 jz 0x0004ECDC
	if ((uint32_t)ca == (uint32_t)cb) { goto loc_0004ECDC; }
loc_0004ECC9:
	// 0004ECC9 lea edx, [esp+0x40]
	ctx.edx = (uint32_t)((uint32_t)(ctx.esp + 0x40u));
	// 0004ECCD push edx
	{ uint32_t v = (uint32_t)(ctx.edx); X86_PUSH32(v); }
	// 0004ECCE push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 0004ECCF call 0x001E96E8
	X86_PUSH32(0x4ECD4u); sub_001E96E8(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x4ECD4u);
	// 0004ECD4 test eax, eax
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a & b); ca = a; cb = b; cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004ECD6 jnz 0x0004EB80
	if (((uint32_t)(ca & cb)) != 0) { X86_BARRIER(); goto loc_0004EB80; }
loc_0004ECDC:
	// 0004ECDC push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 0004ECDD call 0x0003417A
	X86_PUSH32(0x4ECE2u); sub_0003417A(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x4ECE2u);
	// 0004ECE2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 0004ECE3 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
loc_0004ECE4:
	// 0004ECE4 pop ebp
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.ebp = (uint32_t)(v); }
	// 0004ECE5 add esp, 0x1F4
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x1F4u); uint32_t r = (uint32_t)(a + b); ctx.esp = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 0004ECEB ret
	ff_sdk_observe_exit(ctx, base, 0x0004EB50u);
	ctx.esp += 4; return;
}
X86_WEAK_FUNC(sub_0004EB50, __imp__sub_0004EB50)