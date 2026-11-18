#include <algorithm>
#include <bit>
#include <cstring>
#include <wasm_simd128.h>

#include "full-memory.h"

namespace ff_browser_loader_bytes {

inline constexpr uint32_t kChunkBytes = 16;
inline constexpr uint32_t kMaxPrefixBytes = 4096;

inline uint32_t BytesInWindow(uint32_t address) noexcept {
    using ff_browser_memory::kWindowSize;
    return kWindowSize - (address & (kWindowSize - 1));
}

inline uint32_t ScanUntilEither(const uint8_t* source, uint32_t available, uint8_t a, uint8_t b) noexcept {
    const auto first = wasm_i8x16_splat(a);
    const auto second = wasm_i8x16_splat(b);
    uint32_t scanned = 0;
    for (; scanned + kChunkBytes <= available; scanned += kChunkBytes) {
        const auto bytes = wasm_v128_load(source + scanned);
        const auto matches = wasm_v128_or(wasm_i8x16_eq(bytes, first), wasm_i8x16_eq(bytes, second));
        const auto mask = uint32_t(wasm_i8x16_bitmask(matches));
        if (mask) return scanned + std::countr_zero(mask);
    }
    return scanned;
}

inline uint32_t SearchBytesToSkip(uint32_t source, uint8_t target) noexcept {
    const auto available = std::min(kMaxPrefixBytes, BytesInWindow(source));
    if (available < kChunkBytes) return 0;
    const auto* bytes = ff_browser_memory::TryWindow(source, available);
    if (!bytes) return 0;
    const uint32_t plain = ScanUntilEither(bytes, available, target, 0);
    return plain ? plain - 1 : 0;
}

inline uint32_t SearchPairBytesToSkip(uint32_t source, uint16_t target) noexcept {
    const uint32_t available = std::min(kMaxPrefixBytes, BytesInWindow(source));
    if (available <= kChunkBytes) return 0;
    const auto* input = ff_browser_memory::TryWindow(source, available);
    if (!input) return 0;
    const auto low = wasm_i8x16_splat(uint8_t(target));
    const auto high = wasm_i8x16_splat(uint8_t(target >> 8));
    const auto zero = wasm_i8x16_splat(0);
    uint32_t plain = 0;
    for (; plain + kChunkBytes < available; plain += kChunkBytes) {
        const auto current = wasm_v128_load(input + plain);
        const auto next = wasm_v128_load(input + plain + 1);
        const auto pair = wasm_v128_and(wasm_i8x16_eq(current, low), wasm_i8x16_eq(next, high));
        const auto stop = uint32_t(wasm_i8x16_bitmask(wasm_v128_or(pair, wasm_i8x16_eq(current, zero))));
        if (stop) { plain += std::countr_zero(stop); break; }
    }
    return plain ? plain - 1 : 0;
}

inline uint32_t SearchAsciiWordsToSkip(uint32_t source, uint8_t target) noexcept {
    const uint32_t available = std::min(kMaxPrefixBytes, BytesInWindow(source));
    if (available < kChunkBytes) return 0;
    const auto* input = ff_browser_memory::TryWindow(source, available);
    if (!input) return 0;
    const auto needle = wasm_i8x16_splat(target);
    const auto zero = wasm_i8x16_splat(0);
    uint32_t plain = 0;
    // Leave non-ASCII input and the last admitted word to the original
    // word detector, including its full-word loads near a stop or fault.
    for (; plain + kChunkBytes <= available; plain += kChunkBytes) {
        const auto bytes = wasm_v128_load(input + plain);
        const auto hits = wasm_v128_or(wasm_i8x16_eq(bytes, needle), wasm_i8x16_eq(bytes, zero));
        const auto stop = uint32_t(wasm_i8x16_bitmask(wasm_v128_or(bytes, hits)));
        if (stop) { plain += std::countr_zero(stop); break; }
    }
    const uint32_t words = plain & ~3u;
    return words ? words - 4 : 0;
}

inline bool Overlaps(const uint8_t* a, size_t aSize, const uint8_t* b, size_t bSize) noexcept {
    return a < b + bSize && b < a + aSize;
}

inline uint32_t CopyStringPrefix(uint32_t source, uint32_t destination) noexcept {
    const auto available = std::min({kMaxPrefixBytes, BytesInWindow(source), BytesInWindow(destination)});
    if (available < kChunkBytes) return 0;
    const auto* input = ff_browser_memory::TryWindow(source, available);
    auto* output = ff_browser_memory::TryWindow(destination, available);
    if (!input || !output) return 0;
    const uint32_t plain = ScanUntilEither(input, available, 0, 0);
    if (plain < 2 || Overlaps(input, plain, output, plain)) return 0;
    const uint32_t copied = plain - 1;
    std::memcpy(output, input, copied);
    return copied;
}

struct LineCopy {
    uint32_t copied;
    uint32_t delimiterEax;
    bool complete;
};

inline LineCopy CopyLine(uint32_t source, uint32_t remaining, uint32_t destination,
                         uint32_t cursorAddress) noexcept {
    const auto available = std::min({kMaxPrefixBytes, remaining, BytesInWindow(source), BytesInWindow(destination)});
    if (available < kChunkBytes) return {};
    using ff_browser_memory::TryWindow;
    const auto* input = TryWindow(source, available);
    auto* output = TryWindow(destination, available);
    auto* cursor = TryWindow(cursorAddress, 8);
    if (!input || !output || !cursor) return {};

    const uint32_t plain = ScanUntilEither(input, available, '\r', '\n');
    // The scanner returns the full chunk count when it finds no delimiter.
    const bool complete = plain < available - available % kChunkBytes;
    const uint32_t span = plain + uint32_t(complete);
    if ((!complete && plain < 2) || Overlaps(input, span, output, span) ||
        Overlaps(input, span, cursor, 8) || Overlaps(output, span, cursor, 8)) return {};

    // At a scan limit leave a checked scalar iteration before any later fault.
    const uint32_t copied = complete ? plain : plain - 1;
    const uint32_t next = source + copied;
    const uint32_t eax = complete ? (next & ~0xffu) | input[copied] : 0;
    std::memcpy(output, input, copied);
    std::memcpy(cursor, &next, sizeof(next));
    return {copied, eax, complete};
}

} // namespace ff_browser_loader_bytes
