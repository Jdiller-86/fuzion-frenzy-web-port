#include "cpu/x86_ops.h"
#include "full-memory.h"

#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>

#if defined(__wasm_simd128__)
#include <wasm_simd128.h>
#endif

namespace ff_browser_grid {

inline float ReadFloat(const uint8_t* bytes) noexcept {
    return *reinterpret_cast<const x86_f32u*>(bytes);
}

inline bool Overlaps(const uint8_t* first, size_t firstSize,
                     const uint8_t* second, size_t secondSize) noexcept {
    return first < second + secondSize && second < first + firstSize;
}

#if defined(__wasm_simd128__)
using Float4 = float __attribute__((vector_size(16)));
using Double2 = double __attribute__((vector_size(16)));

struct SideMasks {
    uint32_t below;
    uint32_t above;
};

[[gnu::always_inline]] inline SideMasks ClassifySides(v128_t results, float tolerance, float zero) noexcept {
    const auto magnitude = wasm_f32x4_abs(results);
    const auto finite = wasm_f32x4_le(magnitude, wasm_f32x4_splat(std::numeric_limits<float>::max()));
    const auto valid = wasm_v128_and(finite, wasm_f32x4_ge(magnitude, wasm_f32x4_splat(tolerance)));
    const auto below = wasm_v128_and(valid, wasm_f32x4_lt(results, wasm_f32x4_splat(zero)));
    return {uint32_t(wasm_i32x4_bitmask(below)), uint32_t(wasm_i32x4_bitmask(wasm_v128_xor(valid, below)))};
}

template<bool Wide, typename Side>
[[gnu::always_inline]] inline SideMasks ClassifyFour(Side side, const std::array<float, 3>& terms,
                                                     const uint8_t* points, float tolerance, float zero) noexcept {
    const v128_t first = wasm_v128_load(points);
    const v128_t second = wasm_v128_load(points + 16);
    const v128_t xs = wasm_i32x4_shuffle(first, second, 0, 2, 4, 6);
    const v128_t ys = wasm_i32x4_shuffle(first, second, 1, 3, 5, 7);
    v128_t results;
    if constexpr (Wide) {
        const auto splat = [](float term) { return std::bit_cast<Double2>(wasm_f64x2_splat(double(term))); };
        const auto a = splat(terms[0]), b = splat(terms[1]), c = splat(terms[2]);
        const auto pair = [&](v128_t x, v128_t y) {
            const auto px = std::bit_cast<Double2>(wasm_f64x2_promote_low_f32x4(x));
            const auto py = std::bit_cast<Double2>(wasm_f64x2_promote_low_f32x4(y));
            const auto values = side(px, py, a, b, c);
            return wasm_f32x4_demote_f64x2_zero(std::bit_cast<v128_t>(values));
        };
        const auto low = pair(xs, ys);
        const auto high = pair(wasm_i32x4_shuffle(xs, xs, 2, 3, 2, 3), wasm_i32x4_shuffle(ys, ys, 2, 3, 2, 3));
        results = wasm_i32x4_shuffle(low, high, 0, 1, 4, 5);
    } else {
        // With float inputs, each add, subtract or product rounds directly to
        // the same binary32 value as the SDK's double operation then cast.
        const auto splat = [](float term) { return std::bit_cast<Float4>(wasm_f32x4_splat(term)); };
        results = std::bit_cast<v128_t>(side(std::bit_cast<Float4>(xs), std::bit_cast<Float4>(ys),
                                           splat(terms[0]), splat(terms[1]), splat(terms[2])));
    }
    return ClassifySides(results, tolerance, zero);
}

template<bool Wide, typename Side>
[[gnu::always_inline]] inline auto MakeOppositeTest(Side side, const uint8_t* query,
                                                   float tolerance, float zero) noexcept {
    const auto unpack = [](v128_t lanes) {
        if constexpr (Wide) return std::bit_cast<Double2>(wasm_f64x2_promote_low_f32x4(lanes));
        else return std::bit_cast<Float4>(lanes);
    };
    const auto splat = [](float term) {
        if constexpr (Wide) return std::bit_cast<Double2>(wasm_f64x2_splat(double(term)));
        else return std::bit_cast<Float4>(wasm_f32x4_splat(term));
    };
    const auto endpoints = wasm_v128_load(query);
    const auto x = unpack(wasm_i32x4_shuffle(endpoints, endpoints, 0, 2, 0, 2));
    const auto y = unpack(wasm_i32x4_shuffle(endpoints, endpoints, 1, 3, 1, 3));
    return [=](const std::array<float, 3>& terms) {
        auto results = std::bit_cast<v128_t>(side(x, y, splat(terms[0]), splat(terms[1]), splat(terms[2])));
        if constexpr (Wide) results = wasm_f32x4_demote_f64x2_zero(results);
        const auto sides = ClassifySides(results, tolerance, zero);
        // Only the two query endpoints participate, in the low lanes.
        return (sides.below & 3u) == 3u || (sides.above & 3u) == 3u;
    };
}
#endif

template<typename Line, typename Side>
inline uint8_t* BatchRejectedEdges(X86Context& ctx, Line lineTerms, Side pointSide,
                                  uint32_t returnAddress, uint32_t toleranceAddress, uint32_t zeroAddress) noexcept {
    using ff_browser_memory::TryWindow;
    const uint32_t esp = ctx.esp;
    if (int32_t(ctx.edi) < 3 || ctx.eax != esp + 0x40 || ctx.ecx != esp + 0x50 || ctx.edx != esp + 0x60) return nullptr;
    auto* stack = TryWindow(esp - 32, 0x98);
    if (!stack) return nullptr;
    const auto word = [stack](uint32_t offset) { return *reinterpret_cast<const x86_u32u*>(stack + offset); };
    if (word(32) != returnAddress || word(36) != ctx.ecx || word(40) != ctx.eax || word(44) != ctx.edx) return nullptr;

    const auto* toleranceBytes = TryWindow(toleranceAddress, 4);
    const auto* zeroBytes = TryWindow(zeroAddress, 4);
    if (!toleranceBytes || !zeroBytes) return stack;

    // The original call rewrites its frame and the caller's edge copy.
    // Keep the root operands live and disjoint from both physical spans.
    const auto touchesWrites = [stack](const uint8_t* bytes, size_t size) {
        return Overlaps(bytes, size, stack, 0x30) || Overlaps(bytes, size, stack + 0x60, 0x10);
    };
    const auto edgeSpan = [&](uint32_t count) -> const uint8_t* {
        const auto* bytes = TryWindow(ctx.esi, count * 8);
        return bytes && !touchesWrites(bytes, count * 8) ? bytes : nullptr;
    };
    constexpr uint32_t smallBatch = 64, largeBatch = 256;
    uint32_t count = ctx.edi < largeBatch ? ctx.edi : largeBatch;
    const auto* rest = edgeSpan(count);
    if (!rest && count > smallBatch) {
        count = smallBatch;
        rest = edgeSpan(count);
    }
    if (!rest || touchesWrites(toleranceBytes, 4) || touchesWrites(zeroBytes, 4)) return stack;
    auto* edge = stack + 0x60;
    if (std::memcmp(edge + 8, rest, 8)) return stack;
    const float tolerance = ReadFloat(toleranceBytes);
    const float zero = ReadFloat(zeroBytes);
    if (!std::isfinite(tolerance) || !std::isfinite(zero)) return stack;

    const auto* query = stack + 0x70;
    const auto scan = [&]<typename Real>() {
        const auto queryTerms = lineTerms.template operator()<Real>(query);
        const auto classify = [&](const auto& terms, const uint8_t* point) {
            const float side = float(pointSide(Real(ReadFloat(point)), Real(ReadFloat(point + 4)),
                                               Real(terms[0]), Real(terms[1]), Real(terms[2])));
            if (!std::isfinite(side) || !(std::fabs(side) >= tolerance)) return 0;
            return side < zero ? -1 : 1;
        };

#if defined(__wasm_simd128__)
        const auto oppositeTest = MakeOppositeTest<sizeof(Real) == sizeof(double)>(pointSide, query, tolerance, zero);
#endif
        const auto rejectsOpposite = [&](uint32_t index) {
            const auto* segment = index ? rest + (index - 1) * 8 : edge;
            const auto terms = lineTerms.template operator()<Real>(segment);
#if defined(__wasm_simd128__)
            return oppositeTest(terms);
#else
            const int first = classify(terms, query);
            return first && first == classify(terms, query + 8);
#endif
        };

        int previous = classify(queryTerms, edge);
        uint32_t rejected = 0;
#if defined(__wasm_simd128__)
        const auto classifyFour = [&](uint32_t first) {
            return ClassifyFour<sizeof(Real) == sizeof(double)>(
                pointSide, queryTerms, rest + first * 8, tolerance, zero);
        };
        uint32_t previousBelow = previous < 0, previousAbove = previous > 0;
        const auto rejectTile = [&](SideMasks sides, uint32_t lanes) {
            // Align each side mask with the preceding endpoint. The low
            // lanes represent the earliest unresolved edges.
            const uint32_t same = (sides.below & ((sides.below << 1) | previousBelow)) |
                                  (sides.above & ((sides.above << 1) | previousAbove));
            uint32_t remaining = same ^ ((1u << lanes) - 1u);
            while (remaining && rejectsOpposite(rejected + std::countr_zero(remaining))) {
                remaining &= remaining - 1;
            }
            if (remaining) {
                rejected += std::countr_zero(remaining);
                return false;
            }
            rejected += lanes;
            previousBelow = sides.below >> (lanes - 1);
            previousAbove = sides.above >> (lanes - 1);
            return true;
        };
        while (count - rejected >= 8) {
            const auto low = classifyFour(rejected);
            const auto high = classifyFour(rejected + 4);
            if (!rejectTile({low.below | (high.below << 4), low.above | (high.above << 4)}, 8))
                return rejected;
        }
        if (count - rejected >= 4 && !rejectTile(classifyFour(rejected), 4)) return rejected;
        previous = int(previousAbove) - int(previousBelow);
#endif
        while (rejected < count) {
            const int current = classify(queryTerms, rest + rejected * 8);
            if ((!previous || previous != current) && !rejectsOpposite(rejected)) break;
            previous = current;
            ++rejected;
        }
        return rejected;
    };
    // The predicate has no guest side effects, including changes to the control word.
    const uint32_t rejected = (ctx.fcw & 0x300) != 0
        ? scan.template operator()<double>() : scan.template operator()<float>();
    // Run the last proven rejection normally so its stack, registers and
    // both touched x87 slots retain the original values at loop exit.
    if (rejected >= 2) {
        const uint32_t skipped = rejected - 1;
        std::memcpy(edge, rest + (skipped - 1) * 8, 16);
        ctx.esi += skipped * 8;
        ctx.edi -= skipped;
    }
    // The remaining original edge uses the same checked frame and arguments.
    return stack;
}

} // namespace ff_browser_grid
