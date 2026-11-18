#include <algorithm>
#include <bit>
#include <cstring>

#include "cpu/x86_context.h"
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

inline uint8_t* ConversionStack(const X86Context& ctx, uint32_t& distance) noexcept {
    constexpr uint32_t saves = 32;
    distance = ctx.ebp - ctx.esp;
    if (ctx.esp < saves || distance < 0x30 || distance > 0x48) return nullptr;
    auto* window = ff_browser_memory::TryWindow(ctx.esp - saves, saves + distance);
    return window ? window + saves : nullptr;
}

inline bool CopyConversionMantissa(X86Context& ctx) noexcept {
    uint32_t distance;
    auto* stack = ConversionStack(ctx, distance);
    if (!stack || distance != 0x30) return false;
    const uint32_t caller = ReadWord(stack);
    if (caller != 0x001e66eau && caller != 0x001e6729u) return false;
    const uint32_t sourceOffset = caller == 0x001e66eau ? 12 : 24;
    const uint32_t destinationOffset = 36 - sourceOffset;
    const uint32_t source = ctx.ebp - sourceOffset, destination = ctx.ebp - destinationOffset;
    if (ReadWord(stack + 4) != destination || ReadWord(stack + 8) != source) return false;

    WriteWord(stack - 4, ctx.esi);
    std::memcpy(stack + distance - destinationOffset, stack + distance - sourceOffset, 12);
    ctx.eax = source + 12;
    ctx.ecx = destination - source;
    ctx.edx = 0;
    ctx.esp += 4;
    return true;
}

inline bool ShiftConversionMantissa(X86Context& ctx) noexcept {
    uint32_t distance;
    auto* stack = ConversionStack(ctx, distance);
    if (!stack) return false;
    switch (ReadWord(stack)) {
        case 0x001e6770u: case 0x001e678eu: if (distance != 0x30) return false; break;
        case 0x001e6733u: if (distance != 0x38) return false; break;
        case 0x001e674du: if (distance != 0x48) return false; break;
        default: return false;
    }
    const uint32_t mantissaAddress = ctx.ebp - 12;
    const uint32_t count = ReadWord(stack + 8);
    if (ReadWord(stack + 4) != mantissaAddress || count >= 96) return false;

    auto* mantissa = stack + distance - 12;
    const uint32_t high = ReadWord(mantissa), middle = ReadWord(mantissa + 4), low = ReadWord(mantissa + 8);
    const uint32_t bits = count % 32, words = count / 32;
    const auto carry = [bits](uint32_t value) { return bits ? value << (32 - bits) : 0u; };
    const uint32_t shifted[]{high >> bits, (middle >> bits) | carry(high), (low >> bits) | carry(middle)};
    const uint32_t result[]{words == 0 ? shifted[0] : 0u, words < 2 ? shifted[1 - words] : 0u, shifted[2 - words]};
    const uint32_t scratch[]{2, ctx.edi, ctx.esi, ctx.ebx, low & ((1u << bits) - 1u), words, carry(low), ctx.ebp};
    std::memcpy(stack - 32, scratch, sizeof(scratch));
    WriteWord(stack + 8, 32 - bits);
    std::memcpy(mantissa, result, sizeof(result));

    // Both original loops end after the most significant word is moved.
    ctx.eax = UINT32_MAX;
    ctx.ecx = mantissaAddress - 4 * (words + 1);
    ctx.edx = shifted[0];
    ctx.esp += 4;
    return true;
}

inline uint32_t RoundConversionMantissa(uint8_t* stack, uint8_t* mantissa, uint32_t address,
                                       uint32_t precision, uint32_t word, uint32_t bit) noexcept {
    const uint32_t lowerMask = (1u << bit) - 1;
    bool more = (ReadWord(mantissa + word * 4) & lowerMask) != 0;
    for (uint32_t index = word + 1; index < 3; ++index)
        more |= ReadWord(mantissa + index * 4) != 0;
    if (!more) {
        const uint32_t tieScratch[]{31, 0x001e658fu, address};
        std::memcpy(stack - 36, tieScratch, sizeof(tieScratch));
        return 0;
    }

    const uint32_t retained = precision - 1;
    uint32_t index = retained / 32;
    uint32_t addend = 1u << (31 - retained % 32);
    uint32_t returnAddress = 0x001e6532u;
    for (;;) {
        const uint32_t before = ReadWord(mantissa + index * 4);
        const uint32_t sum = before + addend;
        const uint32_t carry = sum < before;
        WriteWord(mantissa + index * 4, sum);
        if (!carry || index == 0) {
            // The final add overwrites these call words on each carry step.
            const uint32_t scratch[]{index, returnAddress, before, addend, address + index * 4,
                                     retained, bit, 0x001e659eu, address};
            std::memcpy(stack - 60, scratch, sizeof(scratch));
            return carry;
        }
        --index;
        addend = 1;
        returnAddress = 0x001e6549u;
    }
}

template<typename RoundingBindings>
inline bool TruncateConversionMantissa(X86Context& ctx, RoundingBindings useDefaultRounding) noexcept {
    uint32_t distance;
    auto* stack = ConversionStack(ctx, distance);
    if (!stack || ctx.df) return false;
    const uint32_t caller = ReadWord(stack);
    if (!((caller == 0x001e66f6u && distance == 0x38) || (caller == 0x001e673fu && distance == 0x40))) return false;
    const uint32_t precision = ReadWord(stack + 8);
    if (ReadWord(stack + 4) != ctx.ebp - 12 || precision == 0 || precision >= 96) return false;

    const uint32_t word = precision / 32, bit = 31 - precision % 32;
    auto* mantissa = stack + distance - 12;
    uint32_t value = ReadWord(mantissa + word * 4);
    uint32_t carry = 0;
    if (value & (1u << bit)) {
        // The add helper uses a deeper part of this same caller-owned stack.
        if (!useDefaultRounding() || !ff_browser_memory::TryWindow(ctx.esp - 60, 60 + distance))
            return false;
        carry = RoundConversionMantissa(stack, mantissa, ctx.ebp - 12, precision, word, bit);
        value = ReadWord(mantissa + word * 4);
    }
    const uint32_t mask = ~0u << bit;
    const uint32_t scratch[]{3, ctx.edi, ctx.esi, ctx.ebx, carry, ctx.ebp};
    std::memcpy(stack - 24, scratch, sizeof(scratch));
    WriteWord(mantissa + word * 4, value & mask);
    if (word == 0) WriteWord(mantissa + 4, 0);
    if (word < 2) WriteWord(mantissa + 8, 0);
    ctx.eax = carry;
    ctx.ecx = word == 2 ? 3 : 0;
    ctx.edx = mask;
    ctx.esp += 4;
    return true;
}

} // namespace ff_browser_decimal
