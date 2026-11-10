#include "sdk-shared-prototypes.hpp"
X86_FUNC(__imp__sub_001E95DB) {
	X86_LOAD_FLAGS();
	ff_sdk_observe_enter(ctx, base, 0x001E95DBu);
	uint32_t ca = 0, cb = 0;
	(void)ca; (void)cb;
	// 001E95DB push ebp
	{ uint32_t v = (uint32_t)(ctx.ebp); X86_PUSH32(v); }
	// 001E95DC mov ebp, esp
	{ ctx.ebp = (uint32_t)(ctx.esp); }
	// 001E95DE sub esp, 0x16C
	{ uint32_t a = (uint32_t)(ctx.esp); uint32_t b = (uint32_t)(0x16Cu); uint32_t r = (uint32_t)(a - b); ctx.esp = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E95E4 push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
	// 001E95E5 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E95E6 push [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 001E95E9 lea eax, [ebp-0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF8u));
	// 001E95EC push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E95ED call [0x0025338C]
	X86_PUSH32(0x1E95F3u); x86_kernel(ctx, base, 289);
	// 001E95F3 mov edx, [ebp-0x04]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFCu); ctx.edx = (uint32_t)(X86_MEM32(ea)); }
	// 001E95F6 lea eax, [ebp-0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF8u));
	// 001E95F9 mov [ebp-0x18], eax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE8u); X86_MEM32(ea) = (uint32_t)(ctx.eax); }
	// 001E95FC movzx eax, word ptr [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.eax = (uint32_t)((uint16_t)(X86_MEM16(ea))); }
	// 001E9600 xor edi, edi
	{ uint32_t a = (uint32_t)(ctx.edi); uint32_t b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a ^ b); ctx.edi = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9602 cmp eax, edi
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9604 mov dword ptr [ebp-0x1C], 0xFFFFFFFD
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFE4u); X86_MEM32(ea) = (uint32_t)(0xFFFFFFFDu); }
	// 001E960B mov dword ptr [ebp-0x14], 0x40
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFECu); X86_MEM32(ea) = (uint32_t)(0x40u); }
	// 001E9612 jbe 0x001E961E
	if ((uint32_t)ca <= (uint32_t)cb) { goto loc_001E961E; }
loc_001E9614:
	// 001E9614 cmp byte ptr [edx+eax*1-0x01], 0x5C
	{ uint32_t ea = (uint32_t)(ctx.edx + ctx.eax + 0xFFFFFFFFu); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x5Cu); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E9619 jz 0x001E961E
	if ((uint8_t)ca == (uint8_t)cb) { goto loc_001E961E; }
	// 001E961B dec eax
	{ uint32_t a = (uint32_t)(ctx.eax);  uint32_t r = (uint32_t)(a - 1); ctx.eax = (uint32_t)(r); of = a == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E961C jnz 0x001E9614
	if (!zf) { X86_BARRIER(); goto loc_001E9614; }
loc_001E961E:
	// 001E961E mov ecx, [ebp-0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); ctx.ecx = (uint32_t)(X86_MEM32(ea)); }
	// 001E9621 sub ecx, eax
	{ uint32_t a = (uint32_t)(ctx.ecx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a - b); ctx.ecx = (uint32_t)(r); cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9623 add edx, eax
	{ uint32_t a = (uint32_t)(ctx.edx); uint32_t b = (uint32_t)(ctx.eax); uint32_t r = (uint32_t)(a + b); ctx.edx = (uint32_t)(r); cf = r < a; of = (uint32_t)(((a ^ r) & (b ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9625 cmp ax, di
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.eax)), b = (uint16_t)(((uint16_t)ctx.edi)); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 001E9628 mov [ebp-0x10], cx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 001E962C mov [ebp-0x0E], cx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF2u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.ecx)); }
	// 001E9630 mov [ebp-0x0C], edx
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF4u); X86_MEM32(ea) = (uint32_t)(ctx.edx); }
	// 001E9633 mov [ebp-0x08], ax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF8u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 001E9637 mov [ebp-0x06], ax
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFFAu); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.eax)); }
	// 001E963B jz 0x001E96D8
	if ((uint16_t)ca == (uint16_t)cb) { goto loc_001E96D8; }
	// 001E9641 cmp cx, di
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.ecx)), b = (uint16_t)(((uint16_t)ctx.edi)); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 001E9644 jz 0x001E96D8
	if ((uint16_t)ca == (uint16_t)cb) { goto loc_001E96D8; }
	// 001E964A cmp cx, 0x03
	{ uint16_t a = (uint16_t)(((uint16_t)ctx.ecx)), b = (uint16_t)(0x3u); uint16_t r = (uint16_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 15) & 1; zf = r == 0; sf = (uint32_t)(r >> 15) & 1; pf = X86_PARITY(r); }
	// 001E964E jnz 0x001E9665
	if ((uint16_t)ca != (uint16_t)cb) { goto loc_001E9665; }
	// 001E9650 cmp byte ptr [edx], 0x2A
	{ uint32_t ea = (uint32_t)(ctx.edx); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2Au); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E9653 jnz 0x001E9665
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_001E9665; }
	// 001E9655 cmp byte ptr [edx+0x01], 0x2E
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x1u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2Eu); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E9659 jnz 0x001E9665
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_001E9665; }
	// 001E965B cmp byte ptr [edx+0x02], 0x2A
	{ uint32_t ea = (uint32_t)(ctx.edx + 0x2u); uint8_t a = (uint8_t)(X86_MEM8(ea)), b = (uint8_t)(0x2Au); uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }
	// 001E965F jnz 0x001E9665
	if ((uint8_t)ca != (uint8_t)cb) { goto loc_001E9665; }
	// 001E9661 mov [ebp-0x10], di
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xFFFFFFF0u); X86_MEM16(ea) = (uint16_t)(((uint16_t)ctx.edi)); }
loc_001E9665:
	// 001E9665 push 0x4021
	{ uint32_t v = (uint32_t)(0x4021u); X86_PUSH32(v); }
	// 001E966A push 0x03
	{ uint32_t v = (uint32_t)(0x3u); X86_PUSH32(v); }
	// 001E966C lea eax, [ebp-0x24]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFDCu));
	// 001E966F push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E9670 lea eax, [ebp-0x1C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFE4u));
	// 001E9673 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E9674 push 0x100001
	{ uint32_t v = (uint32_t)(0x100001u); X86_PUSH32(v); }
	// 001E9679 lea eax, [ebp+0x08]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0x8u));
	// 001E967C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E967D call [0x002533D0]
	X86_PUSH32(0x1E9683u); x86_kernel(ctx, base, 202);
	// 001E9683 cmp eax, edi
	{ uint32_t a = (uint32_t)(ctx.eax), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E9685 jnl 0x001E968A
	if ((int32_t)ca >= (int32_t)cb) { goto loc_001E968A; }
	// 001E9687 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E9688 jmp 0x001E96BD
	goto loc_001E96BD;
loc_001E968A:
	// 001E968A push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E968B lea eax, [ebp-0x10]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFF0u));
	// 001E968E push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E968F push 0x01
	{ uint32_t v = (uint32_t)(0x1u); X86_PUSH32(v); }
	// 001E9691 push 0x148
	{ uint32_t v = (uint32_t)(0x148u); X86_PUSH32(v); }
	// 001E9696 lea eax, [ebp-0x16C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFE94u));
	// 001E969C push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E969D lea eax, [ebp-0x24]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFFDCu));
	// 001E96A0 push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E96A1 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E96A2 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E96A3 push edi
	{ uint32_t v = (uint32_t)(ctx.edi); X86_PUSH32(v); }
	// 001E96A4 push [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 001E96A7 call [0x002534F8]
	X86_PUSH32(0x1E96ADu); x86_kernel(ctx, base, 207);
	// 001E96AD mov esi, eax
	{ ctx.esi = (uint32_t)(ctx.eax); }
	// 001E96AF cmp esi, edi
	{ uint32_t a = (uint32_t)(ctx.esi), b = (uint32_t)(ctx.edi); uint32_t r = (uint32_t)(a - b); ca = a; cb = b; cf = a < b; of = (uint32_t)(((a ^ b) & (a ^ r)) >> 31) & 1; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
	// 001E96B1 jnl 0x001E96C4
	if ((int32_t)ca >= (int32_t)cb) { goto loc_001E96C4; }
	// 001E96B3 push [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 001E96B6 call [0x00253378]
	X86_PUSH32(0x1E96BCu); x86_kernel(ctx, base, 187);
	// 001E96BC push esi
	{ uint32_t v = (uint32_t)(ctx.esi); X86_PUSH32(v); }
loc_001E96BD:
	// 001E96BD call 0x00031155
	X86_PUSH32(0x1E96C2u); sub_00031155(ctx, base);
	// 001E96C2 jmp 0x001E96DF
	goto loc_001E96DF;
loc_001E96C4:
	// 001E96C4 push [ebp+0x0C]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0xCu); uint32_t v = (uint32_t)(X86_MEM32(ea)); X86_PUSH32(v); }
	// 001E96C7 lea eax, [ebp-0x16C]
	ctx.eax = (uint32_t)((uint32_t)(ctx.ebp + 0xFFFFFE94u));
	// 001E96CD push eax
	{ uint32_t v = (uint32_t)(ctx.eax); X86_PUSH32(v); }
	// 001E96CE call 0x001E956F
	X86_PUSH32(0x1E96D3u); sub_001E956F(ctx, base);
	ff_sdk_observe_exit(ctx, base, 0x001E96D3u);
	// 001E96D3 mov eax, [ebp+0x08]
	{ uint32_t ea = (uint32_t)(ctx.ebp + 0x8u); ctx.eax = (uint32_t)(X86_MEM32(ea)); }
	// 001E96D6 jmp 0x001E96E2
	goto loc_001E96E2;
loc_001E96D8:
	// 001E96D8 push 0x57
	{ uint32_t v = (uint32_t)(0x57u); X86_PUSH32(v); }
	// 001E96DA call 0x00031127
	X86_PUSH32(0x1E96DFu); sub_00031127(ctx, base);
loc_001E96DF:
	// 001E96DF or eax, 0xFFFFFFFF
	{ uint32_t a = (uint32_t)(ctx.eax); uint32_t b = (uint32_t)(0xFFFFFFFFu); uint32_t r = (uint32_t)(a | b); ctx.eax = (uint32_t)(r); cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }
loc_001E96E2:
	// 001E96E2 pop edi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.edi = (uint32_t)(v); }
	// 001E96E3 pop esi
	{ uint32_t v = X86_MEM32(ctx.esp); ctx.esp += 4; ctx.esi = (uint32_t)(v); }
	// 001E96E4 leave
	ctx.esp = ctx.ebp; ctx.ebp = X86_MEM32(ctx.esp); ctx.esp += 4;
	// 001E96E5 ret 0x08
	ff_sdk_observe_exit(ctx, base, 0x001E95DBu);
	ctx.esp += 12; return;
}
X86_WEAK_FUNC(sub_001E95DB, __imp__sub_001E95DB)