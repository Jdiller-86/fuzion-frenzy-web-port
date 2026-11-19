#include "full-platform.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <mutex>
#include <thread>
#include <vector>

#include "audio/audio.h"
#include "app/app.h"
#include "core/input.h"
#include "gpu/backend.h"
#include "gpu/texture.h"
#include "kernel/memory.h"
#include "kernel/kernel.h"
#include "full-memory.h"
#include "reference_renderer.h"
#include "full-webgl.h"
void ff_webgl_advance_frame();
void ff_webgl_flush();
bool ff_webgl_pipelined();
uint64_t ff_webgl_raw_draw_reuses();

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/threading.h>
#endif

#ifdef __EMSCRIPTEN__
extern "C" int pthread_setname_np(pthread_t thread, const char* name) {
    emscripten_set_thread_name(thread, name);
    return 0;
}
#endif

extern "C" bool ff_browser_precise_pacing_enabled() {
#ifdef __EMSCRIPTEN__
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return Module.onFuzionGpu?.precisePacing === true ? 1 : 0;
    }) != 0;
    return enabled;
#else
    return false;
#endif
}

extern "C" bool ff_browser_pace_before_present() {
#ifdef __EMSCRIPTEN__
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return Module.onFuzionGpu?.pipelined === true && Module.onFuzionGpu?.paceBeforePresent === true ? 1 : 0;
    }) != 0;
    return enabled;
#else
    return false;
#endif
}

static std::atomic<uint32_t> browser_vblank_ticks{0};
extern "C" void ff_browser_observe_vblank() {
    browser_vblank_ticks.fetch_add(1,std::memory_order_relaxed);
}

bool ff_browser_pacing_timing_enabled() {
#ifdef __EMSCRIPTEN__
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return typeof Module.onFuzionPacingWait === 'function' ? 1 : 0;
    }) != 0;
    return enabled;
#else
    return false;
#endif
}

int ff_browser_resource_fence_mode() {
#ifdef __EMSCRIPTEN__
    static const int mode = MAIN_THREAD_EM_ASM_INT({
        const fence = Module.onFuzionGpu?.resourceFences;
        return fence === 'selective' ? 2 : fence === 'trace' ? 1 : 0;
    });
    return mode;
#else
    return 0;
#endif
}

bool ff_browser_grid_contours_enabled() {
#ifdef __EMSCRIPTEN__
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return Module.onFuzionGpu?.gridContours === true ? 1 : 0;
    }) != 0;
    return enabled;
#else
    return false;
#endif
}

bool ff_browser_fast_integer_enabled() {
#ifdef __EMSCRIPTEN__
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return Module.onFuzionGpu?.fastInteger === true ? 1 : 0;
    }) != 0;
    return enabled;
#else
    return false;
#endif
}

#ifdef __EMSCRIPTEN__
constexpr double kFrameCapPrecisionMs = 2.0;

static bool FrameCapPrecisionEnabled() {
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return Module.onFuzionGpu?.frameCapPrecision === true ? 1 : 0;
    }) != 0;
    return enabled;
}

static void RecordPacingWait(uint32_t kind, double queued, double deadline,
                             double armed, double signalled, double finished, double timers, double precision_ms) {
    MAIN_THREAD_ASYNC_EM_ASM({
        Module.onFuzionPacingWait?.($0, $1, $2, $3, $4, $5, $6, $7);
    }, kind, queued, deadline, armed, signalled, finished, timers, precision_ms);
}

// Cancellation stays disabled in the caller until the JS callback releases
// the signal and timing storage. A result of 3 hands off the final time check.
static uint32_t WaitForBrowserDeadline(double deadline, double precision_ms, double* timing) {
    std::atomic<uint32_t> ready{0};
    static_assert(sizeof(ready) == sizeof(uint32_t));
    MAIN_THREAD_ASYNC_EM_ASM({
        const address = $0 >>> 2;
        const deadline = $1;
        const timing = $2;
        const maximumPrecision = $3;
        const pacing = maximumPrecision > 0 ? Module.onFuzionGpu : undefined;
        // Allow one millisecond for rounding the delay up and keep a short
        // recent margin for callback lateness. Start with the full budget.
        const precision = pacing
            ? Math.min(maximumPrecision, Math.max(1.25, 1 + (pacing.frameCapTimerDelay ?? 1)))
            : maximumPrecision;
        const armed = timing ? performance.timeOrigin + performance.now() : 0;
        let timers = 0;
        let completed = false;
        let watchdog = null;
        const signal = value => {
            const memory = Module.wasmMemory?.buffer || Module.HEAPU8.buffer;
            const heap = new Int32Array(memory);
            // Publish these words before the atomic release. Once signalled,
            // the waiting thread can release both of its stack slots.
            if (timing && value !== 2) {
                const out = new Float64Array(memory, timing, 3);
                out[0] = armed;
                out[1] = performance.timeOrigin + performance.now();
                out[2] = timers;
            }
            Atomics.store(heap, address, value);
            Atomics.notify(heap, address, 1);
            completed = true;
            if (watchdog !== null) clearTimeout(watchdog);
        };
        const fail = error => {
            if (completed) return;
            try { signal(2); } catch {}
            postMessage({ type: 'log', text: `Precise pacing timer failed: ${error?.message || error}` });
        };
        const arm = expected => {
            if (completed) return;
            try {
                const now = performance.timeOrigin + performance.now();
                if (!Number.isFinite(now)) throw new Error('No finite synchronized browser clock');
                if (pacing && expected !== undefined) {
                    // Recover promptly from a late callback and decrease
                    // the estimate gradually when the timers stay on time.
                    pacing.frameCapTimerDelay = Math.min(1, Math.max(
                        0, now - expected, (pacing.frameCapTimerDelay ?? 1) - 0.035));
                }
                const remaining = deadline - now;
                if (remaining > precision) {
                    if (timing) ++timers;
                    const delay = Math.ceil(remaining - precision);
                    setTimeout(arm, delay, now + delay);
                } else {
                    signal(remaining > 0 ? 3 : 1);
                }
            } catch (error) {
                fail(error);
            }
        };
        try {
            watchdog = setTimeout(() => fail(new Error('Timer acknowledgement exceeded 5 seconds')), 5000);
            arm();
        } catch (error) {
            fail(error);
        }
    }, &ready, deadline, timing, precision_ms);
    while (ready.load(std::memory_order_acquire) == 0) {
        const int result = emscripten_futex_wait(&ready, 0, std::numeric_limits<double>::infinity());
        if (result < 0 && result != -EWOULDBLOCK && ready.load(std::memory_order_acquire) == 0) std::abort();
    }
    return ready.load(std::memory_order_acquire);
}
#endif

extern "C" void ff_browser_sleep_for(double milliseconds, uint32_t kind) {
    if (!std::isfinite(milliseconds)) return;
    milliseconds = std::min(milliseconds, 1000.0);
#ifdef __EMSCRIPTEN__
    if (ff_browser_precise_pacing_enabled() && !emscripten_is_main_runtime_thread()) {
        // Frame cap, vertical blank, and event hold share a thread in different
        // combinations. Sample each source separately so none is missed.
        static thread_local uint32_t samples[3]{};
        const bool trace = ff_browser_pacing_timing_enabled() && kind < 3 &&
                           (samples[kind]++ & 3u) == 0;
        if (milliseconds <= 0.0 && !trace) return;
        const double queued = emscripten_get_now();
        const double deadline = queued + milliseconds;
        if (!std::isfinite(deadline)) std::abort();
        if (milliseconds <= 0.0) {
            RecordPacingWait(kind, queued, deadline, queued, queued, queued, 0, 0);
            return;
        }
        const double precision = kind == 0 && milliseconds > kFrameCapPrecisionMs && FrameCapPrecisionEnabled()
            ? kFrameCapPrecisionMs : 0.0;
        double timing[3];
        double precision_spent = 0;
        int previous_cancel_state = PTHREAD_CANCEL_ENABLE;
        if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &previous_cancel_state) != 0) std::abort();
        uint32_t result = WaitForBrowserDeadline(deadline, precision, trace ? timing : nullptr);
        if (result == 3u) {
            const double start = emscripten_get_now();
            if (deadline - start > precision) {
                // A clock discrepancy must stay on the ordinary timer path.
                result = WaitForBrowserDeadline(deadline, 0, trace ? timing : nullptr);
            } else {
                double now = start;
                while (now < deadline) now = emscripten_get_now();
                precision_spent = now - start;
                result = 1;
            }
        }
        if (result != 1u) std::abort();
        if (trace) RecordPacingWait(kind, queued, deadline, timing[0], timing[1],
                                   emscripten_get_now(), timing[2], precision_spent);
        if (pthread_setcancelstate(previous_cancel_state, nullptr) != 0) std::abort();
        return;
    }
#endif
    if (milliseconds > 0.0) std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(milliseconds));
}

namespace {
struct PadState {
    std::atomic<uint32_t> buttons{0};
    std::atomic<uint32_t> left_stick{0};
    std::atomic<uint32_t> right_stick{0};
    std::atomic<uint32_t> triggers{0};
    std::atomic<bool> connected{false};
};
std::array<PadState, 4> g_pads;

uint32_t Pair16(int16_t a, int16_t b) {
    return uint16_t(a) | (uint32_t(uint16_t(b)) << 16);
}

class BrowserBackend final : public gpu::Backend {
public:
    bool Init(uint32_t w, uint32_t h) override {
        width_ = w; height_ = h;
        reference::BeginFrame();
        return w != 0 && h != 0;
    }
    void Clear(const gpu::State& s, uint32_t n, const uint8_t* r, uint32_t flags,
               uint32_t color, float z, uint32_t stencil) override {
        TouchGpuFrame();
        const auto start = Clock::now();
        if (ff_webgl_enabled()) ff_webgl_clear(s, n, r, flags, color, z, stencil);
        else reference::Clear(s, n, r, flags, color, z, stencil);
        clear_ms_ += ElapsedMs(start);
    }
    void Draw(const gpu::State& s, uint32_t prim, uint32_t start, uint32_t count,
              const uint8_t* indices) override {
        if(ff_webgl_enabled()) {
            TouchGpuFrame();reference::Draw(s,prim,start,count,indices);++draw_count_;return;
        }
        const auto started = Clock::now();
        reference::Draw(s, prim, start, count, indices);
        draw_ms_ += ElapsedMs(started);
        ++draw_count_;
    }
    void Copy(const gpu::Surface& src, const gpu::Surface& dst, uint32_t n,
              uint32_t rects, uint32_t points) override {
        TouchGpuFrame();
        const auto start = Clock::now();
        if (ff_webgl_enabled()) {
            gpu::TextureDesc a,b;
            if (gpu::DescribeSurface(src,a)) ff_webgl_sync(a.data,gpu::TotalSize(a));
            if (gpu::DescribeSurface(dst,b)) ff_webgl_sync(b.data,gpu::TotalSize(b));
        }
        reference::Copy(src, dst, n, rects, points);
        if (ff_webgl_enabled()) {gpu::TextureDesc d;if(gpu::DescribeSurface(dst,d))ff_webgl_invalidate(d.data);}
        copy_ms_ += ElapsedMs(start);
    }
    void Present(const gpu::Surface& front) override {
        const auto submitted = Clock::now();
        if (ff_webgl_enabled() && ff_webgl_present(front)) {
            // A single interval includes guest work between rendering calls and
            // synchronous JS submission. Avoid two Wasm/JS clock calls per draw.
            draw_ms_=ElapsedMs(gpu_frame_active_?gpu_frame_start_:submitted);EndFrame();return;
        }
        // The SDK CPU renderer writes the title's actual render target in guest
        // RAM. Copy it into an owned RGBA buffer before crossing into JS.
        gpu::TextureDesc desc;
        if (!gpu::DescribeSurface(front, desc) || !desc.width || !desc.height || desc.width > 4096 || desc.height > 4096) return;
        if (gpu::Format(desc.format).host != gpu::Host::BGRA8) return;
        uint32_t decodedPitch = 0;
        gpu::DecodeLevel(desc, 0, 0, ff_sdk_guest_ptr(desc.data, gpu::TotalSize(desc)), nullptr, decoded_, decodedPitch);
        const uint32_t pitch = desc.width * 4;
        if (decodedPitch < pitch || decoded_.size() < uint64_t(decodedPitch) * desc.height) return;
        rgba_.resize(size_t(desc.width) * desc.height * 4);
        for (uint32_t y = 0; y < desc.height; ++y) for (uint32_t x = 0; x < desc.width; ++x) {
            const auto* source = decoded_.data() + size_t(y) * decodedPitch + x * 4;
            auto* target = rgba_.data() + (size_t(y) * desc.width + x) * 4;
            target[0] = source[2]; target[1] = source[1]; target[2] = source[0]; target[3] = 255;
        }
#ifdef __EMSCRIPTEN__
        MAIN_THREAD_EM_ASM({ if (Module.onFuzionFrame) Module.onFuzionFrame($0, $1, $2, $3, $4); },
                            rgba_.data(), desc.width, desc.height, pitch, 0);
#endif
        EndFrame();
    }
    void EndFrame() {
        ff_webgl_advance_frame();
#ifdef __EMSCRIPTEN__
        // This is backend-consumer completion time, after Present/QueuePresent;
        // it is not the title thread's frame-start or physical display time.
        const double completion_ms = emscripten_get_now();
        const double frame_sequence = static_cast<double>(frames_ + 1);
        const double raw_draw_reuses = static_cast<double>(ff_webgl_raw_draw_reuses());
        if(ff_webgl_enabled()&&ff_webgl_pipelined()) {
            MAIN_THREAD_ASYNC_EM_ASM({ if (Module.onFuzionMetrics) Module.onFuzionMetrics($0,$1,$2,$3,$4,$5,$6,$7); },
                                    draw_ms_,clear_ms_,copy_ms_,static_cast<uint32_t>(draw_count_),completion_ms,frame_sequence,
                                    browser_vblank_ticks.load(std::memory_order_relaxed),raw_draw_reuses);
        } else {
            MAIN_THREAD_EM_ASM({ if (Module.onFuzionMetrics) Module.onFuzionMetrics($0,$1,$2,$3,$4,$5,$6,$7); },
                              draw_ms_,clear_ms_,copy_ms_,static_cast<uint32_t>(draw_count_),completion_ms,frame_sequence,
                              browser_vblank_ticks.load(std::memory_order_relaxed),raw_draw_reuses);
        }
#endif
        ++frames_;
        draw_ms_ = clear_ms_ = copy_ms_ = 0.0;
        draw_count_ = 0;
        gpu_frame_active_=false;
        reference::BeginFrame();
    }
    void Sync(uint32_t data, uint32_t size) override { if (ff_webgl_enabled()) ff_webgl_sync(data,size); }
    void SyncLate(uint32_t data, uint32_t size) override { Sync(data,size); }
    void Flush() override {if(ff_webgl_enabled())ff_webgl_flush();}
    void SetGamma(const uint8_t*) override {}
    bool WidensFrame() const override { return false; }

private:
    using Clock = std::chrono::steady_clock;
    void TouchGpuFrame() {
        if(ff_webgl_enabled()&&!gpu_frame_active_){gpu_frame_start_=Clock::now();gpu_frame_active_=true;}
    }
    Clock::time_point gpu_frame_start_;
    bool gpu_frame_active_=false;
    static double ElapsedMs(Clock::time_point start) {
        return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
    }
    uint32_t width_ = 0, height_ = 0;
    uint64_t frames_ = 0;
    std::vector<uint8_t> decoded_, rgba_;
    double draw_ms_ = 0.0, clear_ms_ = 0.0, copy_ms_ = 0.0;
    uint64_t draw_count_ = 0;
};
} // namespace

extern "C" void ff_browser_set_pad(int port, uint32_t buttons, int16_t lx, int16_t ly,
                                    int16_t rx, int16_t ry, uint8_t lt, uint8_t rt) {
    if (port < 0 || port >= int(g_pads.size())) return;
    auto& p = g_pads[size_t(port)];
    p.connected.store(true, std::memory_order_relaxed);
    p.buttons.store(buttons, std::memory_order_relaxed);
    p.left_stick.store(Pair16(lx, ly), std::memory_order_relaxed);
    p.right_stick.store(Pair16(rx, ry), std::memory_order_relaxed);
    p.triggers.store(uint32_t(lt) | (uint32_t(rt) << 8), std::memory_order_relaxed);
}

namespace input {
void Init() {}
Pad Poll(int port) {
    Pad result;
    if (port < 0 || port >= int(g_pads.size())) return result;
    const auto& p = g_pads[size_t(port)];
    result.connected = p.connected.load(std::memory_order_relaxed);
    uint32_t button_pack = p.buttons.load(std::memory_order_relaxed);
    result.buttons = uint16_t(button_pack);
    uint32_t sticks = p.left_stick.load(std::memory_order_relaxed);
    uint32_t right = p.right_stick.load(std::memory_order_relaxed);
    result.lx = int16_t(sticks); result.ly = int16_t(sticks >> 16);
    result.rx = int16_t(right); result.ry = int16_t(right >> 16);
    uint32_t triggers = p.triggers.load(std::memory_order_relaxed);
    result.analog[6] = uint8_t(triggers); result.analog[7] = uint8_t(triggers >> 8);
    for (int i = 0; i < 6; ++i) result.analog[i] = (button_pack & (1u << (i + 16))) ? 255 : 0;
    return result;
}
void Rumble(int, uint16_t, uint16_t) {}
} // namespace input

namespace gpu {
std::unique_ptr<Backend> CreateBrowserBackend() { return std::make_unique<BrowserBackend>(); }
std::unique_ptr<Backend> CreateBackend() { return CreateBrowserBackend(); }
}

namespace app {
Settings& Get() { static Settings s; return s; }
void Load() {}
void Save() {}
void LogSettings() {}
void LogSystem() {}
void ApplyDisplay() {}
Mode Desktop() { return {640, 480}; }
Mode Output() { return Desktop(); }
float RenderScale(uint32_t) { return 1.0f; }
bool Widescreen() { return false; }
float AspectWiden() { return 1.0f; }
float TargetAspect() { return 4.0f / 3.0f; }
float HostAspect() { return TargetAspect(); }
HdrMode HdrWanted() { return HdrMode::Off; }
uint32_t PaperWhiteNits() { return 200; }
uint32_t PeakNits() { return 0; }
HdrStatus Hdr() { return {false, false, 0, "browser SDR output"}; }
void SetHdr(const HdrStatus&) {}
std::filesystem::path ExeDir() { return std::filesystem::current_path(); }
bool FindGame(GameFiles&) { return false; }
int Instance() { return 0; }
void* Window() { return nullptr; }
bool OpenSplash() { return false; }
void SplashFirstFrame() {}
void CloseSplash() {}
void OpenGameWindow(bool) {}
bool StartHandover() { return false; }
bool HandingOver() { return false; }
void FinishHandover() {}
std::recursive_mutex& UiLock() { static std::recursive_mutex m; return m; }
void Pace() {
    using clock = std::chrono::steady_clock;
    static clock::time_point next = clock::now();
    const uint32_t cap = Get().fps_cap;
    const auto now = clock::now();
    if (!cap) { next = now; return; }
    const auto period = std::chrono::nanoseconds(1000000000ull / cap);
    next += period;
    if (next < now - period) next = now;
    if (ff_browser_precise_pacing_enabled()) {
        ff_browser_sleep_for(std::chrono::duration<double, std::milli>(next - clock::now()).count(), 0);
    } else {
        std::this_thread::sleep_until(next);
    }
}
} // namespace app

namespace audio {
void StartOutput(std::function<void(float*, uint32_t)> render) {
#ifdef __EMSCRIPTEN__
    // JS must synchronously copy PCM during this callback; the buffer is reused.
    static std::vector<float> pcm(kRate / 100 * 2);
    static std::atomic_flag active = ATOMIC_FLAG_INIT;
    if (active.test_and_set()) return;
    std::thread([render = std::move(render)] {
        for (;;) {
            render(pcm.data(), kRate / 100);
            MAIN_THREAD_EM_ASM({ if (Module.onFuzionAudio) Module.onFuzionAudio($0, $1, $2); }, pcm.data(), kRate / 100, kRate);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }).detach();
#else
    (void)render;
#endif
}
} // namespace audio
