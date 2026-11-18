#include "guest-loader-bytes.inc"

namespace ff_browser_loader_bytes {

inline uint32_t CopyBoundedLinePrefix(uint32_t start, uint32_t end, uint32_t dest) noexcept {
    if (end <= start) return 0;
    const auto available = std::min({kMaxPrefixBytes, end - start, BytesInWindow(start), BytesInWindow(dest)});
    if (available < kChunkBytes) return 0;
    using ff_browser_memory::TryWindow;
    const auto* source = TryWindow(start, available);
    auto* output = TryWindow(dest, available);
    if (!source || !output) return 0;
    const uint32_t plain = ScanUntilEither(source, available, '\r', '\n');
    if (plain < 2 || Overlaps(source, plain, output, plain)) return 0;

    // Preserve the last original iteration's registers and flags.
    const uint32_t advance = plain - 1;
    std::memcpy(output, source, advance);
    return advance;
}

inline uint32_t RepeatedLinePrefix(uint32_t start, uint32_t stop, uint32_t end,
                                   uint32_t dest, uint32_t endAddress) noexcept {
    if (stop <= start || stop > end || stop - start > kMaxPrefixBytes) return 0;
    const bool atEnd = stop == end;
    const uint32_t plain = stop - start;
    const uint32_t sourceBytes = plain + !atEnd;
    const uint32_t outputBytes = plain + 1;
    using ff_browser_memory::TryWindow;
    const auto* source = TryWindow(start, sourceBytes);
    const auto* output = TryWindow(dest, outputBytes);
    const auto* endWord = TryWindow(endAddress, 4);
    if (!source || !output || !endWord || Overlaps(source, sourceBytes, output, outputBytes) ||
        Overlaps(endWord, 4, output, outputBytes)) return 0;
    uint32_t currentEnd;
    std::memcpy(&currentEnd, endWord, sizeof(currentEnd));
    if (currentEnd != end) return 0;

    // Skip bytes already checked by the first scan. At the bound, keep
    // the last ordinary iteration so it performs the original EOF check.
    return plain - atEnd;
}

} // namespace ff_browser_loader_bytes
