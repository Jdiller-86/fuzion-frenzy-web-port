#include "full-platform.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstring>
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
        reference::Clear(s, n, r, flags, color, z, stencil);
    }
    void Draw(const gpu::State& s, uint32_t prim, uint32_t start, uint32_t count,
              const uint8_t* indices) override {
        reference::Draw(s, prim, start, count, indices);
    }
    void Copy(const gpu::Surface& src, const gpu::Surface& dst, uint32_t n,
              uint32_t rects, uint32_t points) override {
        reference::Copy(src, dst, n, rects, points);
    }
    void Present(const gpu::Surface& front) override {
        // The SDK CPU renderer writes the title's actual render target in guest
        // RAM. Copy it into an owned RGBA buffer before crossing into JS.
        gpu::TextureDesc desc;
        if (!gpu::DescribeSurface(front, desc) || !desc.width || !desc.height || desc.width > 4096 || desc.height > 4096) return;
        if (gpu::Format(desc.format).host != gpu::Host::BGRA8) return;
        std::vector<uint8_t> decoded;
        uint32_t decodedPitch = 0;
        gpu::DecodeLevel(desc, 0, 0, ff_sdk_guest_ptr(desc.data, gpu::TotalSize(desc)), nullptr, decoded, decodedPitch);
        const uint32_t pitch = desc.width * 4;
        if (decodedPitch < pitch || decoded.size() < uint64_t(decodedPitch) * desc.height) return;
        std::vector<uint8_t> rgba(size_t(desc.width) * desc.height * 4);
        for (uint32_t y = 0; y < desc.height; ++y) for (uint32_t x = 0; x < desc.width; ++x) {
            const auto* source = decoded.data() + size_t(y) * decodedPitch + x * 4;
            auto* target = rgba.data() + (size_t(y) * desc.width + x) * 4;
            target[0] = source[2]; target[1] = source[1]; target[2] = source[0]; target[3] = 255;
        }
#ifdef __EMSCRIPTEN__
        MAIN_THREAD_EM_ASM({ if (Module.onFuzionFrame) Module.onFuzionFrame($0, $1, $2, $3, $4); },
                            rgba.data(), desc.width, desc.height, pitch, 0);
#endif
        ++frames_;
        reference::BeginFrame();
    }
    void Sync(uint32_t, uint32_t) override {}
    void SyncLate(uint32_t, uint32_t) override {}
    void Flush() override {}
    void SetGamma(const uint8_t*) override {}
    bool WidensFrame() const override { return false; }

private:
    uint32_t width_ = 0, height_ = 0;
    uint64_t frames_ = 0;
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
    std::this_thread::sleep_until(next);
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
