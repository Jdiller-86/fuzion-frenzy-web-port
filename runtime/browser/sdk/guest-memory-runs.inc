#include <algorithm>

#include "cpu/x86_ops.h"
#include "full-memory.h"

namespace ff_browser_memory_runs {

template<int32_t... Offsets>
[[gnu::always_inline]] inline uint8_t* TrySpan(uint32_t base) {
    constexpr int32_t low = std::min({Offsets...});
    constexpr int32_t high = std::max({Offsets...});
    constexpr int64_t bytes = int64_t(high) - low + 4;
    static_assert(bytes <= ff_browser_memory::kWindowSize);
    return ff_browser_memory::TryWindow(base + uint32_t(low), size_t(bytes));
}

template<int32_t... Offsets, typename... Registers>
[[gnu::noinline, gnu::cold]] void FaultableLoad32(uint32_t base, Registers&... registers) {
    ((registers = X86_MEM32(base + uint32_t(Offsets))), ...);
}

template<int32_t... Offsets, typename... Registers>
[[gnu::always_inline]] inline void Load32(uint32_t base, Registers&... registers) {
    const auto* span = TrySpan<Offsets...>(base);
    if (!span) [[unlikely]] {
        FaultableLoad32<Offsets...>(base, registers...);
        return;
    }
    constexpr int32_t low = std::min({Offsets...});
    ((registers = *reinterpret_cast<const x86_u32u*>(span + (Offsets - low))), ...);
}

template<int32_t... Offsets, typename... Values>
[[gnu::noinline, gnu::cold]] void FaultableStore32(uint32_t base, Values... values) {
    ((X86_MEM32(base + uint32_t(Offsets)) = uint32_t(values)), ...);
}

template<int32_t... Offsets, typename... Values>
[[gnu::always_inline]] inline void Store32(uint32_t base, Values... values) {
    auto* span = TrySpan<Offsets...>(base);
    if (!span) [[unlikely]] {
        FaultableStore32<Offsets...>(base, values...);
        return;
    }
    constexpr int32_t low = std::min({Offsets...});
    ((*reinterpret_cast<x86_u32u*>(span + (Offsets - low)) = uint32_t(values)), ...);
}

} // namespace ff_browser_memory_runs
