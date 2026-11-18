#include "guest-x87.h"

namespace ff_browser_x87 {

template<char Operation>
[[gnu::noinline, gnu::cold]] void FaultableCompareF32(
    X86Context& ctx, uint32_t left, uint32_t comparison, uint32_t operand) {
    x87_push(ctx, double(X86_MEMF32(left)));
    if constexpr (Operation) {
        const double other = double(X86_MEMF32(operand));
        X87_ST(0) = Calculate<Operation>(ctx, X87_ST(0), other);
    }
    x87_compare(ctx, X87_ST(0), double(X86_MEMF32(comparison)));
    x87_pop(ctx);
}

// Collect the checks for a load, optional arithmetic, and compare/pop.
// On a fault the original order determines which FPU state was already written.
template<char Operation = 0>
[[gnu::always_inline]] inline void CompareF32(
    X86Context& ctx, uint32_t left, uint32_t comparison, uint32_t operand = 0) {
    using namespace ff_browser_memory;
    const uint32_t leftOffset = CheckedF32Offset(left);
    const uint32_t comparisonOffset = CheckedF32Offset(comparison);
    uint32_t operandOffset = 0;
    if constexpr (Operation) operandOffset = CheckedF32Offset(operand);
    if ((leftOffset | comparisonOffset | operandOffset) >= kBackingSize) [[unlikely]] {
        FaultableCompareF32<Operation>(ctx, left, comparison, operand);
        return;
    }
    double value = double(*reinterpret_cast<const x86_f32u*>(backing + leftOffset));
    if constexpr (Operation) {
        const double other = double(*reinterpret_cast<const x86_f32u*>(backing + operandOffset));
        value = Calculate<Operation>(ctx, value, other);
    }
    const double limit = double(*reinterpret_cast<const x86_f32u*>(backing + comparisonOffset));
    ctx.st[(ctx.top - 1) & 7] = value;
    x87_compare(ctx, value, limit);
    ctx.top &= 7;
}

} // namespace ff_browser_x87
