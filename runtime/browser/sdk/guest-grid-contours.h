#pragma once

#include "cpu/x86_ops.h"
#include "full-memory.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <memory>
#include <new>

#include <emscripten.h>

bool ff_browser_grid_contours_enabled();
bool ff_browser_grid_caller_frame_enabled();
bool ff_browser_pacing_timing_enabled();

X86_EXTERN_FUNC(ff_browser_grid_default_intersection);
extern "C" void ff_browser_grid_call_from_frame(X86Context& ctx, uint8_t* base, uint8_t* frame);

namespace ff_browser_grid {

struct CallerFrameUse {
    uint32_t calls = 0;

    ~CallerFrameUse() {
        if (!calls || !ff_browser_pacing_timing_enabled()) return;
        EM_ASM({
            console.timeStamp?.("ff-grid-caller-frame:" + $0);
        }, calls);
    }
};

// The same contour ordinal and first edge recur for each cell in this
// digest-matched grid builder. Capture bounds at most once per ordinal.
class ContourScope {
public:
    explicit ContourScope(const X86Context& ctx) noexcept : entry_(ctx.esp) {
        const auto* ret = ff_browser_memory::TryWindow(entry_, sizeof(uint32_t));
        if (!ret || *reinterpret_cast<const x86_u32u*>(ret) != 0x0019f3f3 ||
            !ff_browser_grid_contours_enabled()) return;
        previous_ = active_;
        active_ = this;
    }

    ~ContourScope() {
        if (active_ != this) return;
        active_ = previous_;
        if (!ff_browser_pacing_timing_enabled()) return;
        MAIN_THREAD_ASYNC_EM_ASM({
            Module.onFuzionGridContours?.($0, $1, $2, $3, $4);
        }, captured_, considered_, accepted_, skipped_, occupied_);
    }

    ContourScope(const ContourScope&) = delete;
    ContourScope& operator=(const ContourScope&) = delete;

    template<typename Proof>
    static uint32_t Reject(const X86Context& ctx, const uint8_t* stack, Proof proves) noexcept {
        auto* scope = active_;
        if (!scope || ctx.esp != scope->entry_ - 0x80 || ctx.ebx >= scope->contours_.size() ||
            ctx.edi < kMinPoints || ctx.edi > kMaxPoints) return 0;
        auto& saved = scope->contours_[ctx.ebx];
        const uint32_t length = ctx.edi * kPointBytes;
        if (!saved.length) scope->Capture(saved, ctx, stack, length);
        if (saved.address != ctx.esi || saved.length != length || !saved.bounded) return 0;
        ++scope->considered_;
        if (!proves(saved.minX, saved.minY, saved.maxX, saved.maxY)) return 0;

        const auto* live = ff_browser_memory::TryWindow(ctx.esi, length);
        if (!live || TouchesWrites(live, length, stack)) return 0;
        // Only trust the bounds if the whole live span still matches its
        // snapshot. The builder can change bindings and other threads can
        // change memory between queries. A mismatch keeps the current scan.
        if (std::memcmp(live, scope->bytes_.get() + saved.offset, length)) return 0;
        ++scope->accepted_;
        scope->skipped_ += ctx.edi - 1;
        return ctx.edi;
    }

private:
    static constexpr uint32_t kPointBytes = 8;
    static constexpr uint32_t kMinPoints = 24;
    static constexpr uint32_t kMaxPoints = 2048;
    static constexpr uint32_t kSnapshotBytes = 128 * 1024;

    struct Contour {
        uint32_t address = 0, length = 0, offset = 0;
        float minX = 0, minY = 0, maxX = 0, maxY = 0;
        bool bounded = false;
    };

    static bool TouchesWrites(const uint8_t* first, uint32_t bytes, const uint8_t* stack) noexcept {
        const auto overlaps = [&](const uint8_t* target, uint32_t length) {
            return first < target + length && target < first + bytes;
        };
        return overlaps(stack, 0x30) || overlaps(stack + 0x60, 0x10);
    }

    void Capture(Contour& saved, const X86Context& ctx, const uint8_t* stack, uint32_t length) noexcept {
        saved.address = ctx.esi;
        saved.length = length;
        const auto* live = ff_browser_memory::TryWindow(ctx.esi, length);
        if (!live || TouchesWrites(live, length, stack) || length > kSnapshotBytes - occupied_) return;
        if (!bytes_) bytes_.reset(new (std::nothrow) uint8_t[kSnapshotBytes]);
        if (!bytes_) return;
        auto* snapshot = bytes_.get() + occupied_;
        std::memcpy(snapshot, live, length);

        const auto point = [&](uint32_t offset) { return *reinterpret_cast<const x86_f32u*>(snapshot + offset); };
        saved.minX = saved.maxX = point(0);
        saved.minY = saved.maxY = point(4);
        for (uint32_t i = 0; i < length; i += kPointBytes) {
            const float x = point(i), y = point(i + 4);
            if (!(std::abs(x) <= 4096.0f && std::abs(y) <= 4096.0f)) return;
            saved.minX = std::min(saved.minX, x);
            saved.maxX = std::max(saved.maxX, x);
            saved.minY = std::min(saved.minY, y);
            saved.maxY = std::max(saved.maxY, y);
        }
        saved.offset = occupied_;
        occupied_ += length;
        saved.bounded = true;
        ++captured_;
    }

    inline static thread_local ContourScope* active_ = nullptr;
    uint32_t entry_;
    ContourScope* previous_ = nullptr;
    std::array<Contour, 64> contours_{};
    std::unique_ptr<uint8_t[]> bytes_;
    uint32_t occupied_ = 0, captured_ = 0, considered_ = 0, accepted_ = 0, skipped_ = 0;
};

} // namespace ff_browser_grid
