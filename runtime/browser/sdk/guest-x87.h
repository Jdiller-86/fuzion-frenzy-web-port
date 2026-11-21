#pragma once

#include "cpu/x86_ops.h"

namespace ff_browser_x87 {

template<char Operation>
[[gnu::always_inline]] inline double Calculate(X86Context& ctx, double left, double right) {
    if constexpr (Operation == '+') return x87_prec(ctx, left + right);
    if constexpr (Operation == '-') return x87_prec(ctx, left - right);
    if constexpr (Operation == '*') return x87_prec(ctx, left * right);
    static_assert(Operation == '+' || Operation == '-' || Operation == '*' || Operation == '/');
    return x87_prec(ctx, left / right);
}

// The original sequence may leave a pushed operand or a completed result on
// the stack when a later address faults. Retain the original order there.
template<char Operation>
[[gnu::noinline, gnu::cold]] void FaultableMemoryF32(X86Context& ctx, uint32_t left, uint32_t right, uint32_t dst) {
    x87_push(ctx, double(X86_MEMF32(left)));
    const double operand = double(X86_MEMF32(right));
    X87_ST(0) = Calculate<Operation>(ctx, X87_ST(0), operand);
    X86_MEMF32(dst) = float(X87_ST(0));
    x87_pop(ctx);
}

[[gnu::always_inline]] inline uint32_t CheckedF32Offset(uint32_t address) {
    using namespace ff_browser_memory;
    const uint32_t offset = address & (kWindowSize - 1);
    const uint32_t window = kWindowOffsets[address >> kWindowBits];
    const uint32_t crossing = ((offset + 3) & kWindowSize) * (kBackingSize / kWindowSize);
    return window | offset | crossing;
}

template<char Operation>
[[gnu::always_inline]] inline void MemoryF32(X86Context& ctx, uint32_t left, uint32_t right, uint32_t dst) {
    using namespace ff_browser_memory;
    const uint32_t leftOffset = CheckedF32Offset(left);
    const uint32_t rightOffset = CheckedF32Offset(right);
    const uint32_t dstOffset = CheckedF32Offset(dst);
    if ((leftOffset | rightOffset | dstOffset) >= kBackingSize) [[unlikely]] {
        FaultableMemoryF32<Operation>(ctx, left, right, dst);
        return;
    }
    const double value = double(*reinterpret_cast<const x86_f32u*>(backing + leftOffset));
    const double operand = double(*reinterpret_cast<const x86_f32u*>(backing + rightOffset));
    const double result = Calculate<Operation>(ctx, value, operand);
    ctx.st[(ctx.top - 1) & 7] = result;
    *reinterpret_cast<x86_f32u*>(backing + dstOffset) = float(result);
    ctx.top &= 7;
}

} // namespace ff_browser_x87
