#include <algorithm>
#include <bit>
#include <wasm_simd128.h>

#include "full-memory.h"

namespace ff_browser_strings {

inline uint32_t MatchingBytesToSkip(uint32_t left, uint32_t right) noexcept {
    using namespace ff_browser_memory;
    constexpr uint32_t chunkBytes = 16;
    constexpr uint32_t maxBytes = 256;
    const uint32_t available = std::min({maxBytes, kWindowSize - (left & (kWindowSize - 1)),
                                         kWindowSize - (right & (kWindowSize - 1))});
    if (available < chunkBytes) return 0;
    const auto* leftBytes = TryWindow(left, available);
    const auto* rightBytes = TryWindow(right, available);
    if (!leftBytes || !rightBytes) return 0;

    uint32_t matched = 0;
    const auto zero = wasm_i8x16_splat(0);
    for (; matched + chunkBytes <= available; matched += chunkBytes) {
        const auto a = wasm_v128_load(leftBytes + matched);
        const auto b = wasm_v128_load(rightBytes + matched);
        const auto stops = wasm_v128_or(wasm_i8x16_ne(a, b), wasm_i8x16_eq(a, zero));
        const auto stopMask = uint32_t(wasm_i8x16_bitmask(stops));
        if (stopMask) {
            matched += std::countr_zero(stopMask);
            break;
        }
    }

    // Keep the final known-valid pair in the scalar loop so it writes the
    // original byte registers, comparison operands and flags before exiting.
    return matched ? matched - 1 : 0;
}

} // namespace ff_browser_strings
