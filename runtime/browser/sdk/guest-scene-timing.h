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
    std::array<Measurement, PhaseCount> phases{};
};

inline thread_local Trace trace;

// Only the profiling build injects these scopes. Aggregate nested calls before
// emitting so the browser bridge does not run inside the measured scene setup.
class Scope {
public:
    explicit Scope(Phase phase) : phase_(phase) {
        if (phase == Scene) {
            if (trace.active || !ff_browser_pacing_timing_enabled()) return;
            trace = {};
            trace.active = true;
        }
        if (trace.active) started_ = emscripten_get_now();
    }

    ~Scope() {
        if (!started_) return;
        const double finished = emscripten_get_now();
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
};

} // namespace ff_browser_scene_timing
