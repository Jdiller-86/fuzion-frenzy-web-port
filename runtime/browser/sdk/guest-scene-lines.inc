#include <array>

namespace ff_browser_loader_bytes {

inline bool SceneLineCaller(uint32_t caller) noexcept {
    switch (caller) {
        case 0x001b2dc6: case 0x001b2f7b: case 0x001b2f9e: case 0x001b2fc1: case 0x001b2fe4:
        case 0x001b304a: case 0x001b3068: case 0x001b3086: case 0x001b30a8: case 0x001b30ca:
        case 0x001b30ec: case 0x001b310e: case 0x001b3130:
            return true;
        default:
            return false;
    }
}

inline bool SceneDelimiter(uint8_t byte) noexcept {
    return byte == '\r' || byte == '\n';
}

inline uint64_t ReadSceneMarkerWord(const uint8_t* bytes) noexcept {
    uint64_t word;
    std::memcpy(&word, bytes, sizeof(word));
    return word;
}

struct SceneMarker {
    uint16_t pair;
    uint64_t prefix;
};

struct SceneMarkers {
    SceneMarker first;
    uint16_t secondPair;
    std::array<uint64_t, 4> secondPrefixes;

    bool CouldMatch(const uint8_t* bytes, size_t available) const noexcept {
        const uint16_t pair = uint16_t(bytes[0]) | (uint16_t(bytes[1]) << 8);
        if (pair != first.pair && pair != secondPair) return false;
        if (available < sizeof(uint64_t)) return true;
        const uint64_t word = ReadSceneMarkerWord(bytes);
        if (pair == first.pair && (!first.prefix || word == first.prefix)) return true;
        if (pair == secondPair) {
            for (const auto prefix : secondPrefixes)
                if (!prefix || word == prefix) return true;
        }
        return false;
    }
};

inline const uint8_t* QuietSceneLineEnd(const uint8_t* input, const uint8_t* limit,
                                       const SceneMarkers& markers) noexcept {
    const auto cr = wasm_i8x16_splat('\r'), lf = wasm_i8x16_splat('\n'), zero = wasm_i8x16_splat(0);
    const uint16_t first = markers.first.pair, second = markers.secondPair;
    const auto firstLow = wasm_i8x16_splat(uint8_t(first)), firstHigh = wasm_i8x16_splat(uint8_t(first >> 8));
    const auto secondLow = wasm_i8x16_splat(uint8_t(second)), secondHigh = wasm_i8x16_splat(uint8_t(second >> 8));
    for (; limit - input > kChunkBytes; input += kChunkBytes) {
        const auto current = wasm_v128_load(input), following = wasm_v128_load(input + 1);
        const auto delimiters = wasm_v128_or(wasm_i8x16_eq(current, cr), wasm_i8x16_eq(current, lf));
        const auto a = wasm_v128_and(wasm_i8x16_eq(current, firstLow), wasm_i8x16_eq(following, firstHigh));
        const auto b = wasm_v128_and(wasm_i8x16_eq(current, secondLow), wasm_i8x16_eq(following, secondHigh));
        const auto stops = wasm_v128_or(wasm_v128_or(a, b), wasm_v128_or(delimiters, wasm_i8x16_eq(current, zero)));
        uint32_t mask = wasm_i8x16_bitmask(stops);
        while (mask) {
            const auto* stop = input + std::countr_zero(mask);
            if (SceneDelimiter(*stop)) return stop;
            if (!*stop || markers.CouldMatch(stop, size_t(limit - stop))) return nullptr;
            mask &= mask - 1;
        }
    }
    for (; limit - input > 1; ++input) {
        if (SceneDelimiter(*input)) return input;
        if (!*input || markers.CouldMatch(input, size_t(limit - input))) return nullptr;
    }
    return nullptr;
}

inline void SkipQuietSceneLines(uint32_t stack, uint32_t file, bool fill, bool countActive) noexcept {
    using ff_browser_memory::TryWindow;
    constexpr uint32_t savedScratch = 24, frameBytes = 0x450, lineOffset = 0x38;
    constexpr uint32_t returnOffset = 0x438, lineCapacity = returnOffset - lineOffset;
    if (stack < savedScratch) return;
    auto* frame = TryWindow(stack - savedScratch, savedScratch + frameBytes);
    auto* descriptor = TryWindow(file, 16);
    if (!frame || !descriptor || Overlaps(frame, savedScratch + frameBytes, descriptor, 16)) return;
    auto* saved = frame + savedScratch;
    if (ReadWord(saved + returnOffset + 4) != file || !SceneLineCaller(ReadWord(saved + returnOffset))) return;
    auto* cursor = descriptor + 8;
    const uint32_t start = ReadWord(cursor), end = ReadWord(cursor + 4);
    if (end <= start) return;
    const uint32_t available = std::min({end - start, BytesInWindow(start), kMaxPrefixBytes});
    const auto* source = TryWindow(start, available);
    if (!source || Overlaps(source, available, frame, savedScratch + frameBytes) ||
        Overlaps(source, available, descriptor, 16)) return;

    auto markerSpan = [&](uint32_t address, size_t width) -> const uint8_t* {
        const auto* bytes = TryWindow(address, width);
        if (!bytes || Overlaps(bytes, width, frame, savedScratch + frameBytes) ||
            Overlaps(bytes, width, descriptor, 16)) return nullptr;
        return bytes;
    };
    auto readMarker = [&](uint32_t address) -> SceneMarker {
        const auto* bytes = markerSpan(address, sizeof(uint64_t));
        const uint64_t word = bytes ? ReadSceneMarkerWord(bytes) : 0;
        if (!bytes) bytes = markerSpan(address, 2);
        if (!bytes || !bytes[0] || !bytes[1]) return {};
        // A zero byte requires the original pair check.
        constexpr uint64_t lowBits = 0x0101010101010101ull, highBits = 0x8080808080808080ull;
        const uint64_t prefix = ((word - lowBits) & ~word & highBits) ? 0 : word;
        return {uint16_t(uint16_t(bytes[0]) | (uint16_t(bytes[1]) << 8)), prefix};
    };
    const auto first = readMarker(0x267c10);
    const auto second = fill ? readMarker(0x267bf4) : (countActive ? readMarker(0x267c50) : first);
    if (!first.pair || !second.pair) return;
    SceneMarkers markers{first, second.pair, {second.prefix, second.prefix, second.prefix, second.prefix}};
    // The fill pass can search three more markers inside a matching section.
    if (fill && saved[0x12]) {
        constexpr std::array<uint32_t, 3> extraMarkers{0x267c50, 0x267be0, 0x267bcc};
        for (size_t i = 0; i < extraMarkers.size(); ++i) {
            const auto marker = readMarker(extraMarkers[i]);
            if (marker.pair != second.pair) return;
            markers.secondPrefixes[i + 1] = marker.prefix;
        }
    }

    const auto* limit = source + available;
    const auto* line = source;
    const uint8_t* last = nullptr;
    size_t lastLength = 0;
    while (line < limit) {
        const auto* stop = QuietSceneLineEnd(line, line + std::min<size_t>(limit - line, lineCapacity + 1),
                                             markers);
        if (!stop || limit - stop < 2 || !SceneDelimiter(stop[1])) break;
        const size_t length = size_t(stop - line);
        if (length >= lineCapacity) break;
        if (last && lastLength > length) {
            // Keep the old suffix that this shorter line leaves in scratch.
            auto* output = saved + lineOffset;
            const size_t suffix = length + 1;
            std::memcpy(output + suffix, last + suffix, lastLength - suffix);
            output[lastLength] = 0;
        }
        last = line;
        lastLength = length;
        // The original copy consumes one delimiter. Its following helper
        // consumes the rest of the run when the next byte is a delimiter.
        line = stop + 2;
        while (line < limit && SceneDelimiter(*line)) ++line;
    }
    // Execute the last admitted line normally to reproduce the registers,
    // flags and call scratch before any later marker, EOF or possible fault.
    if (last && last != source) WriteWord(cursor, start + uint32_t(last - source));
}

} // namespace ff_browser_loader_bytes
