#include <algorithm>
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

inline uint8_t* AccumulateMantissa(uint32_t bp, uint32_t sp, uint32_t output, uint32_t count,
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
    if (digits[0] == 0 || digits[0] > 9) return nullptr;

    // Up to nineteen decimal digits fit in a single unsigned 64-bit value.
    const uint32_t lowDigits = std::min(count, 19u);
    uint64_t initial = digits[0];
    for (uint32_t i = 1; i < lowDigits; ++i) {
        if (digits[i] > 9) return nullptr;
        initial = initial * 10 + digits[i];
    }
    uint32_t words[]{uint32_t(initial), uint32_t(initial >> 32), 0u};
    for (uint32_t i = lowDigits; i < count; ++i) {
        if (digits[i] > 9) return nullptr;
        const uint64_t low = uint64_t(words[0]) * 10 + digits[i];
        const uint64_t middle = uint64_t(words[1]) * 10 + (low >> 32);
        words[0] = uint32_t(low);
        words[1] = uint32_t(middle);
        words[2] = words[2] * 10 + uint32_t(middle >> 32);
    }
    // Restore the final iteration's scratch. The last high-word add has a
    // zero addend, so its saved high word is the high word of the result.
    const uint32_t lastCalls[]{
        output, 0x001e7ee6, words[2], 0, output + 8,  // Last high-word add.
        bp - 4, output + 12, 0x001e7fad, output, bp - 16,  // Last digit add.
        output, output, bp - 16, output, output,  // Earlier shifts and sum.
    };
    const uint32_t digitAddend[]{digits[count - 1], 0, 0};
    std::memcpy(frame - scratchBelowFrame, lastCalls, sizeof(lastCalls));
    std::memcpy(frame - 16, digitAddend, sizeof(digitAddend));
    std::memcpy(frame + kMantissaOffset, words, sizeof(words));
    WriteWord(frame + 8, bp + kDigitOffset + count);
    WriteWord(frame + 16, 0);
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

inline void AccumulateProduct(uint32_t bp, uint32_t sp, uint32_t leftAddress,
                              uint32_t rightAddress) noexcept {
    if (sp != bp - 48u) return;
    constexpr uint32_t scratchBelow = 68;
    constexpr uint32_t frameBytes = scratchBelow + 16;
    constexpr uint32_t operandBytes = 10;
    using ff_browser_memory::TryWindow;
    auto* scratch = TryWindow(bp - scratchBelow, frameBytes);
    const auto* left = TryWindow(leftAddress, operandBytes);
    const auto* right = TryWindow(rightAddress, operandBytes);
    if (!scratch || !left || !right) return;
    const auto aliasesScratch = [scratch](const uint8_t* bytes) {
        return bytes < scratch + frameBytes && bytes + operandBytes > scratch;
    };
    auto* frame = scratch + scratchBelow;
    if (ReadWord(frame + 4) != 0x001E8761u || aliasesScratch(left) || aliasesScratch(right)) return;

    uint16_t a[5], b[5], product[6];
    std::memcpy(a, left, sizeof(a));
    std::memcpy(b, right, sizeof(b));
    // The original starts at diagonal four of the full product. Lower
    // diagonals contribute no carry to this truncated multiplication.
    uint64_t carry = 0;
    for (uint32_t diagonal = 0; diagonal < 4; ++diagonal) {
        uint64_t sum = carry;
        for (uint32_t i = diagonal; i < 5; ++i)
            sum += uint32_t(a[i]) * b[4 + diagonal - i];
        product[diagonal] = uint16_t(sum);
        carry = sum >> 16;
    }
    product[4] = uint16_t(carry);
    product[5] = uint16_t(carry >> 16);
    std::memcpy(frame - 36, product, sizeof(product));
    // Keep the final multiply-add and its guest stack writes.
    WriteWord(frame - 16, 4);
    WriteWord(frame - 4, bp - 24u);
    WriteWord(frame + 12, 1);
}

} // namespace ff_browser_decimal
