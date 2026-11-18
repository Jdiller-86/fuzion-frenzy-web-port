#include <bit>
#include <cstring>

#include "full-memory.h"

namespace ff_browser_decimal {

inline constexpr uint32_t kDigitOffset = 32;
inline constexpr uint32_t kMantissaOffset = 60;

inline uint32_t ReadWord(const uint8_t* address) noexcept {
    uint32_t value;
    std::memcpy(&value, address, sizeof(value));
    return value;
}

inline void WriteWord(uint8_t* address, uint32_t value) noexcept {
    std::memcpy(address, &value, sizeof(value));
}

inline uint8_t* AccumulatePrefix(uint32_t bp, uint32_t sp, uint32_t output, uint32_t count,
                                 bool backwards) noexcept {
    if (backwards || count == 0 || count > 24 || sp != bp - 28u || output != bp + kMantissaOffset) return nullptr;
    constexpr uint32_t scratchBelowFrame = 88;
    auto* frame = ff_browser_memory::TryWindow(bp - scratchBelowFrame, scratchBelowFrame + kMantissaOffset + 12);
    if (!frame) return nullptr;
    frame += scratchBelowFrame;

    // The checked caller owns the digits and mantissa in separate parts of
    // its stack. The admitted range includes every nested helper's pushes.
    if (ReadWord(frame) != bp + 116u || ReadWord(frame + 4) != 0x001E7DA5u ||
        ReadWord(frame + 8) != bp + kDigitOffset) return nullptr;
    const auto* digits = frame + kDigitOffset;
    if (digits[0] == 0 || digits[count - 1] > 9) return nullptr;
    if (count == 1) return frame;

    uint32_t words[3]{};
    for (uint32_t i = 0; i < count - 1; ++i) {
        if (digits[i] > 9) return nullptr;
        const uint64_t low = uint64_t(words[0]) * 10 + digits[i];
        const uint64_t middle = uint64_t(words[1]) * 10 + (low >> 32);
        words[0] = uint32_t(low);
        words[1] = uint32_t(middle);
        words[2] = words[2] * 10 + uint32_t(middle >> 32);
    }
    // The last digit takes the original helper sequence and rewrites the
    // same guest stack scratch and registers as the scalar loop.
    std::memcpy(frame + kMantissaOffset, words, sizeof(words));
    WriteWord(frame + 8, bp + kDigitOffset + count - 1);
    WriteWord(frame + 16, 1);
    return frame;
}

inline void NormalizePrefix(uint8_t* frame) noexcept {
    uint32_t words[3];
    std::memcpy(words, frame + kMantissaOffset, sizeof(words));
    if (words[2] == 0 || words[2] >= 0x4000u) return;
    const unsigned shift = std::countl_zero(words[2]) - 17;
    words[2] = (words[2] << shift) | (words[1] >> (32 - shift));
    words[1] = (words[1] << shift) | (words[0] >> (32 - shift));
    words[0] <<= shift;
    std::memcpy(frame + kMantissaOffset, words, sizeof(words));
    // Keep the final shift and carry/flag updates in the original body.
    WriteWord(frame - 4, ReadWord(frame - 4) + 0xFFFFu * shift);
}

} // namespace ff_browser_decimal
