#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

// Browser SDK guest address translation. A range must fit wholly within one
// admitted guest window; arithmetic never wraps across the 32-bit VA space.
uint8_t* ff_sdk_guest_ptr(uint32_t va, size_t bytes = 1);
uint8_t* ff_sdk_guest_base();

namespace ff_browser_memory {
inline constexpr uint32_t kWindowBits = 26;
inline constexpr uint32_t kWindowSize = 1u << kWindowBits;
inline constexpr uint32_t kBackingSize = 4 * kWindowSize;
alignas(65536) extern uint8_t backing[kBackingSize];

inline constexpr auto kWindowOffsets = [] {
    std::array<uint32_t, 1u << (32 - kWindowBits)> offsets;
    offsets.fill(kBackingSize);
    offsets[0] = 0;
    offsets[0x80000000u >> kWindowBits] = kWindowSize;
    offsets[0xF0000000u >> kWindowBits] = kWindowSize;
    offsets[0xB0000000u >> kWindowBits] = 2 * kWindowSize;
    offsets[0xD0000000u >> kWindowBits] = 3 * kWindowSize;
    return offsets;
}();

[[gnu::always_inline]] inline uint8_t* TryWindow(uint32_t va, size_t bytes) noexcept {
    const uint32_t offset = va & (kWindowSize - 1);
    const uint32_t window = kWindowOffsets[va >> kWindowBits];
    if (window == kBackingSize || bytes > kWindowSize - offset) return nullptr;
    return backing + window + offset;
}
} // namespace ff_browser_memory

// Check each fixed-width access with one rejection test. Window entries use
// kBackingSize for unmapped addresses. The last byte carries into kWindowSize
// only when the complete span crosses the first byte's window.
template<size_t Width>
[[gnu::always_inline]] inline uint8_t* ff_sdk_guest_scalar_ptr(uint32_t va) {
    using namespace ff_browser_memory;
    static_assert(Width > 0 && Width <= kWindowSize);
    const uint32_t offset = va & (kWindowSize - 1);
    const uint32_t window = kWindowOffsets[va >> kWindowBits];
    const uint32_t lastByte = offset + uint32_t(Width - 1);
    const uint32_t crossing = (lastByte & kWindowSize) * (kBackingSize / kWindowSize);
    if ((window | crossing) < kBackingSize) return backing + window + offset;
    return ff_sdk_guest_ptr(va, Width);
}
