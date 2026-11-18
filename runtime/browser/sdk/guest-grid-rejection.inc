#include "cpu/x86_ops.h"
#include "full-memory.h"

#include <array>
#include <cmath>
#include <cstring>

namespace ff_browser_grid {

inline float ReadFloat(const uint8_t* bytes) noexcept {
    return *reinterpret_cast<const x86_f32u*>(bytes);
}

inline bool Overlaps(const uint8_t* first, size_t firstSize,
                     const uint8_t* second, size_t secondSize) noexcept {
    return first < second + secondSize && second < first + firstSize;
}

template<typename Line, typename Side>
inline void BatchFirstRejects(X86Context& ctx, Line lineTerms, Side pointSide,
                              uint32_t returnAddress, uint32_t toleranceAddress, uint32_t zeroAddress) noexcept {
    using ff_browser_memory::TryWindow;
    const uint32_t esp = ctx.esp;
    if (int32_t(ctx.edi) < 3 || ctx.eax != esp + 0x40 || ctx.ecx != esp + 0x50 || ctx.edx != esp + 0x60) return;
    auto* stack = TryWindow(esp - 32, 0x98);
    if (!stack) return;
    const auto word = [stack](uint32_t offset) { return *reinterpret_cast<const x86_u32u*>(stack + offset); };
    if (word(32) != returnAddress || word(36) != ctx.ecx || word(40) != ctx.eax || word(44) != ctx.edx) return;

    const uint32_t count = ctx.edi < 64 ? ctx.edi : 64;
    const auto* rest = TryWindow(ctx.esi, count * 8);
    const auto* toleranceBytes = TryWindow(toleranceAddress, 4);
    const auto* zeroBytes = TryWindow(zeroAddress, 4);
    if (!rest || !toleranceBytes || !zeroBytes) return;

    // The original call rewrites its frame and the caller's edge copy.
    // Keep the root operands live and disjoint from both physical spans.
    const auto touchesWrites = [stack](const uint8_t* bytes, size_t size) {
        return Overlaps(bytes, size, stack, 0x30) || Overlaps(bytes, size, stack + 0x60, 0x10);
    };
    if (touchesWrites(rest, count * 8) || touchesWrites(toleranceBytes, 4) || touchesWrites(zeroBytes, 4)) return;
    auto* edge = stack + 0x60;
    if (std::memcmp(edge + 8, rest, 8)) return;
    const float tolerance = ReadFloat(toleranceBytes);
    const float zero = ReadFloat(zeroBytes);
    if (!std::isfinite(tolerance) || !std::isfinite(zero)) return;

    const auto terms = lineTerms(ctx, stack + 0x70);
    const auto classify = [&](const uint8_t* point) {
        const float side = pointSide(ctx, terms, point);
        if (!std::isfinite(side) || !(std::fabs(side) >= tolerance)) return 0;
        return side < zero ? -1 : 1;
    };
    const int direction = classify(edge);
    if (!direction) return;

    uint32_t rejected = 0;
    while (rejected < count && classify(rest + rejected * 8) == direction) ++rejected;
    // Run the last proven rejection normally so its stack, registers and
    // both touched x87 slots retain the original values at loop exit.
    if (rejected < 2) return;
    const uint32_t skipped = rejected - 1;
    std::memcpy(edge, rest + (skipped - 1) * 8, 16);
    ctx.esi += skipped * 8;
    ctx.edi -= skipped;
}

} // namespace ff_browser_grid
