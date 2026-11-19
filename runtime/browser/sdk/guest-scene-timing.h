#pragma once

#include <array>
#include <cstdint>

#include <emscripten.h>

bool ff_browser_pacing_timing_enabled();

namespace ff_browser_scene_timing {

enum Phase : uint32_t { Scene, Loader, Tree, Reader, TwoPass, Parser, Grid, PhaseCount };

struct Measurement {
    double first = 0, last = 0, total = 0;
    uint32_t calls = 0;
};

struct Trace {
    bool active = false;
    uint32_t markedScopes = 0;
    std::array<Measurement, PhaseCount> phases{};
};

inline thread_local Trace trace;

inline double PhaseClock(Phase phase, bool entering, bool mark) {
    if (!mark) return emscripten_get_now();
    // Emit on the guest pthread so Chrome records that thread's CPU clock.
    return EM_ASM_DOUBLE({
        const time = performance.timeOrigin + performance.now();
        console.timeStamp?.("ff-scene-cpu:" + $0 + ":" + $1 + ":" + time);
        return time;
    }, phase, int(entering));
}

// Only the profiling build injects these scopes. The worker bridge receives
// the aggregated wall spans once the guest scene setup exits.
class Scope {
public:
    explicit Scope(Phase phase) : phase_(phase) {
        if (phase == Scene) {
            if (trace.active || !ff_browser_pacing_timing_enabled()) return;
            trace = {};
            trace.active = true;
        }
        if (trace.active) {
            marked_ = trace.markedScopes < 128;
            if (marked_) ++trace.markedScopes;
            started_ = PhaseClock(phase_, true, marked_);
        }
    }

    ~Scope() {
        if (!started_) return;
        const double finished = PhaseClock(phase_, false, marked_);
        auto& sample = trace.phases[phase_];
        if (!sample.calls++) sample.first = started_;
        sample.last = finished;
        sample.total += finished - started_;
        if (phase_ != Scene) return;

        trace.active = false;
        for (uint32_t phase = 0; phase < PhaseCount; ++phase) {
            const auto& span = trace.phases[phase];
            if (!span.calls) continue;
            MAIN_THREAD_ASYNC_EM_ASM({
                Module.onFuzionSceneTiming?.($0, $1, $2, $3, $4, $5, $6);
            }, phase, started_, finished, span.calls, span.first, span.last, span.total);
        }
    }

    Scope(const Scope&) = delete;
    Scope& operator=(const Scope&) = delete;

private:
    Phase phase_;
    double started_ = 0;
    bool marked_ = false;
};

} // namespace ff_browser_scene_timing
