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

inline const uint8_t* QuietSceneLineEnd(const uint8_t* input, const uint8_t* limit,
                                        uint16_t first, uint16_t second) noexcept {
    const auto cr = wasm_i8x16_splat('\r'), lf = wasm_i8x16_splat('\n'), zero = wasm_i8x16_splat(0);
    const auto firstLow = wasm_i8x16_splat(uint8_t(first)), firstHigh = wasm_i8x16_splat(uint8_t(first >> 8));
    const auto secondLow = wasm_i8x16_splat(uint8_t(second)), secondHigh = wasm_i8x16_splat(uint8_t(second >> 8));
    for (; limit - input > kChunkBytes; input += kChunkBytes) {
        const auto current = wasm_v128_load(input), following = wasm_v128_load(input + 1);
        const auto delimiters = wasm_v128_or(wasm_i8x16_eq(current, cr), wasm_i8x16_eq(current, lf));
        const auto a = wasm_v128_and(wasm_i8x16_eq(current, firstLow), wasm_i8x16_eq(following, firstHigh));
        const auto b = wasm_v128_and(wasm_i8x16_eq(current, secondLow), wasm_i8x16_eq(following, secondHigh));
        const auto stops = wasm_v128_or(wasm_v128_or(a, b), wasm_v128_or(delimiters, wasm_i8x16_eq(current, zero)));
        const uint32_t mask = wasm_i8x16_bitmask(stops);
        if (mask) {
            const auto* stop = input + std::countr_zero(mask);
            return SceneDelimiter(*stop) ? stop : nullptr;
        }
    }
    for (; limit - input > 1; ++input) {
        if (SceneDelimiter(*input)) return input;
        const uint16_t pair = uint16_t(input[0]) | (uint16_t(input[1]) << 8);
        if (!*input || pair == first || pair == second) return nullptr;
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

    auto readPair = [&](uint32_t address) -> uint16_t {
        const auto* pair = TryWindow(address, 2);
        if (!pair || !pair[0] || !pair[1] || Overlaps(pair, 2, frame, savedScratch + frameBytes) ||
            Overlaps(pair, 2, descriptor, 16)) return 0;
        return uint16_t(pair[0]) | (uint16_t(pair[1]) << 8);
    };
    const uint16_t first = readPair(0x267c10);
    const uint16_t second = fill ? readPair(0x267bf4) : (countActive ? readPair(0x267c50) : first);
    if (!first || !second) return;
    // The fill pass can search three more markers inside a matching section.
    // Use the same scan only when all three live prefixes match the second pair.
    if (fill && saved[0x12] &&
        (readPair(0x267c50) != second || readPair(0x267be0) != second || readPair(0x267bcc) != second)) return;

    const auto* limit = source + available;
    const auto* line = source;
    const uint8_t* last = nullptr;
    size_t lastLength = 0;
    while (line < limit) {
        const auto* stop = QuietSceneLineEnd(line, line + std::min<size_t>(limit - line, lineCapacity + 1),
                                             first, second);
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
