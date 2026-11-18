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

struct PairSearch {
    uint32_t advance;
    bool atEnd;
};

inline PairSearch SearchPairBytes(uint32_t source, uint16_t target, uint32_t limit = kMaxPrefixBytes) noexcept {
    const uint32_t available = std::min(limit, BytesInWindow(source));
    if (available <= kChunkBytes) return {};
    const auto* input = ff_browser_memory::TryWindow(source, available);
    if (!input) return {};
    const auto low = wasm_i8x16_splat(uint8_t(target));
    const auto high = wasm_i8x16_splat(uint8_t(target >> 8));
    const auto zero = wasm_i8x16_splat(0);
    uint32_t plain = 0;
    for (; plain + kChunkBytes < available; plain += kChunkBytes) {
        const auto current = wasm_v128_load(input + plain);
        const auto next = wasm_v128_load(input + plain + 1);
        const auto pair = wasm_v128_and(wasm_i8x16_eq(current, low), wasm_i8x16_eq(next, high));
        const auto stop = uint32_t(wasm_i8x16_bitmask(wasm_v128_or(pair, wasm_i8x16_eq(current, zero))));
        if (stop) {
            const uint32_t offset = plain + std::countr_zero(stop);
            const bool atEnd = input[offset] == 0;
            return {offset + uint32_t(atEnd), atEnd};
        }
    }
    // At the span limit, preserve a checked scalar step before any fault.
    return {plain ? plain - 1 : 0, false};
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

inline uint32_t ReadWord(const uint8_t* p) noexcept {
    uint32_t word;
    std::memcpy(&word, p, sizeof(word));
    return word;
}

inline void WriteWord(uint8_t* p, uint32_t word) noexcept {
    std::memcpy(p, &word, sizeof(word));
}

inline uint32_t SkipSuppressedWord(uint32_t frame, uint32_t stack) noexcept {
    // The scanner's SEH frame and its caller's stack-backed string stream.
    constexpr uint32_t frameOffset = 0x1e4;
    constexpr uint32_t streamOffset = 0x14;
    constexpr uint32_t frameBytes = frameOffset + streamOffset + 16;
    if (stack != frame - (frameOffset - 4)) return 0;
    using ff_browser_memory::TryWindow;
    auto* span = TryWindow(stack - 4u, frameBytes);
    if (!span) return 0;
    auto* bp = span + frameOffset;
    auto* stream = bp + streamOffset;
    if (ReadWord(bp + 4) != 0x001df9b9 || ReadWord(bp + 8) != frame + streamOffset ||
        ReadWord(stream + 12) != 0x49 || ReadWord(bp - 0x64) != 's' ||
        !bp[-0x47] || bp[-0x49] || ReadWord(bp - 0x40)) return 0;

    const uint32_t source = ReadWord(stream);
    const uint32_t remaining = ReadWord(stream + 4);
    if (remaining < 2 || remaining > 0x7fffffffu) return 0;
    const uint32_t available = std::min({remaining, 128u, BytesInWindow(source)});
    const auto* text = TryWindow(source, available);
    if (!text || Overlaps(text, available, span, frameBytes)) return 0;
    uint32_t ordinary = 0;
    while (ordinary < available) {
        const uint8_t byte = text[ordinary];
        if (!byte || byte == ' ' || uint32_t(byte) - 9u <= 4u) break;
        ++ordinary;
    }
    if (ordinary < 2) return 0;
    // One mapped ordinary byte remains. The original iteration writes
    // its return slot, scratch character, registers and flags before any
    // delimiter, EOF or other operation that could fault.
    const uint32_t advance = ordinary - 1;
    WriteWord(stream, source + advance);
    WriteWord(stream + 4, remaining - advance);
    WriteWord(bp - 0x2c, ReadWord(bp - 0x2c) + advance);
    return advance;
}

struct SubstringMiss {
    uint32_t needle;
    uint16_t pair;
    bool complete;
};

inline SubstringMiss TrySubstringMiss(uint32_t stack, uint32_t esi, uint32_t ebx, uint32_t edi) noexcept {
    using ff_browser_memory::TryWindow;
    constexpr size_t saves = 3 * sizeof(uint32_t);
    auto* frame = TryWindow(stack - uint32_t(saves), saves + 3 * sizeof(uint32_t));
    if (!frame) return {};
    uint32_t caller;
    std::memcpy(&caller, frame + saves, 4);
    if (caller != 0x001ba1b4 && caller != 0x001ba2c6) return {};
    uint32_t source, needle;
    std::memcpy(&source, frame + saves + 4, 4);
    std::memcpy(&needle, frame + saves + 8, 4);
    const auto* prefix = TryWindow(needle, 2);
    if (!prefix || !prefix[0] || !prefix[1] || Overlaps(prefix, 2, frame, saves)) return {};
    uint16_t pair;
    std::memcpy(&pair, prefix, 2);
    const auto match = SearchPairBytes(source, pair, 128);
    if (!match.atEnd) return {};
    const auto* text = TryWindow(source, match.advance);
    if (!text || Overlaps(text, match.advance, frame, saves)) return {};
    // These writes reproduce the original pushes in stack order. A miss
    // before any candidate leaves the saved registers as they were on entry.
    const uint32_t saved[] = {esi, ebx, edi};
    std::memcpy(frame, saved, saves);
    return {needle, pair, true};
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

struct NormalizedLinePrefix {
    uint32_t consumed;
    uint32_t written;
    uint8_t nextByte;
};

inline NormalizedLinePrefix NormalizeTreeLinePrefix(uint32_t source, uint32_t destination,
                                                    uint32_t stack, uint8_t first) noexcept {
    constexpr uint32_t scratch = 0x002ed220;
    constexpr uint32_t lineBytes = 0x400;
    const uint32_t offset = source - scratch;
    // The tree reader's line buffer starts 0x48 bytes above the two saved registers.
    if (offset >= lineBytes || destination != stack + 0x48u) return {};
    const uint32_t available = std::min({lineBytes - offset, BytesInWindow(source), BytesInWindow(destination)});
    if (available < kChunkBytes) return {};
    using ff_browser_memory::TryWindow;
    const auto* frame = TryWindow(stack, 12);
    const auto* input = TryWindow(source, available);
    auto* output = TryWindow(destination, available);
    if (!frame || !input || !output || input[0] != first || Overlaps(input, available, output, available)) return {};
    uint32_t returnAddress;
    std::memcpy(&returnAddress, frame + 8, 4);
    if (returnAddress != 0x0007f0a0) return {};

    const auto nul = wasm_i8x16_splat(0);
    const auto quote = wasm_i8x16_splat('"');
    const auto slash = wasm_i8x16_splat('/');
    uint32_t plain = 0;
    for (; plain + kChunkBytes <= available; plain += kChunkBytes) {
        const auto chunk = wasm_v128_load(input + plain);
        const auto special = wasm_v128_or(wasm_i8x16_eq(chunk, nul),
                             wasm_v128_or(wasm_i8x16_eq(chunk, quote), wasm_i8x16_eq(chunk, slash)));
        const auto mask = uint32_t(wasm_i8x16_bitmask(special));
        if (mask) { plain += std::countr_zero(mask); break; }
    }
    if (plain < 2) return {};
    // Keep an ordinary byte in the original loop so it updates CL and flags
    // before handling a quote, comment, or mapping boundary.
    const uint32_t consumed = plain - 1;
    uint32_t written = 0;
    for (uint32_t i = 0; i < consumed; ++i) {
        const uint8_t byte = input[i];
        if (byte != ' ' && byte != '\t') output[written++] = byte;
    }
    return {consumed, written, input[consumed]};
}

} // namespace ff_browser_loader_bytes
