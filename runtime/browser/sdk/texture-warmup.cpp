#include "texture-warmup.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <map>
#include <new>
#include <emscripten.h>

uint64_t ff_webgl_frame();
bool ff_webgl_equal_bytes(const void*, const void*, size_t);
namespace gpu {
std::shared_ptr<uint8_t[]> DecodeBrowserColor(const TextureDesc&, const uint8_t*, size_t, const uint32_t*);
}

bool ff_browser_early_texture_decode_enabled() {
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({
        return Module.onFuzionGpu?.earlyTextureDecode === true;
    });
    return enabled;
}

namespace ff_browser_texture {
namespace {
constexpr size_t kBudget = 24u * 1024 * 1024;
constexpr uint64_t kReadyLifetime = 8;
std::atomic<size_t> reserved{0};
// Only the preparation consumer reads or mutates this table.
thread_local std::map<uint32_t, std::unique_ptr<Work>> ready;

bool Reserve(size_t charge) {
    auto used = reserved.load(std::memory_order_relaxed);
    while (charge <= kBudget - used) {
        if (reserved.compare_exchange_weak(used, used + charge, std::memory_order_relaxed)) return true;
    }
    return false;
}

bool SameLayout(const gpu::TextureDesc& a, const gpu::TextureDesc& b) {
    return a.format == b.format && a.width == b.width && a.height == b.height &&
        a.depth == b.depth && a.levels == b.levels && a.pitch == b.pitch &&
        a.cube == b.cube && a.linear == b.linear;
}

void Mark(unsigned phase, size_t bytes) {
#ifdef FF_BROWSER_TEXTURE_WARMUP_PROBE
    EM_ASM({
        console.timeStamp?.("ff-warm-texture:" + [$0, $1, $2].join(":"));
    }, phase, double(ff_webgl_frame() + 1), double(bytes));
#endif
}
} // namespace

Work::~Work() {
    // Release the allocation budget after destroying both private buffers.
    rgba.reset();
    std::vector<uint8_t>().swap(encoded);
    if (charge) reserved.fetch_sub(charge, std::memory_order_relaxed);
}

std::unique_ptr<Work> Copy(const Source& source) {
    // Charge the eventual decoded bytes up front and also bound small jobs.
    if (!source.pixels || !source.bytes || source.bytes > 1024 * 1024 ||
        source.desc.width > 512 || source.desc.height > 512 || source.desc.format != 0x02) return {};
    const size_t charge = std::max<size_t>(65536, source.bytes + size_t(source.desc.width) * source.desc.height * 4);
    if (!Reserve(charge)) return {};
    std::unique_ptr<Work> work;
    try {
        work = std::make_unique<Work>();
        work->charge = charge;
        work->desc = source.desc;
        work->backing = source.backing;
        work->encoded.assign(source.pixels, source.pixels + source.bytes);
    } catch (const std::bad_alloc&) {
        if (!work) reserved.fetch_sub(charge, std::memory_order_relaxed);
        return {};
    }
    return work;
}

void Prepare(std::unique_ptr<Work> work) {
    Mark(0, work->encoded.size());
    try {
        work->rgba = gpu::DecodeBrowserColor(work->desc, work->encoded.data(), work->encoded.size(), nullptr);
        if (work->rgba) {
            work->prepared_frame = ff_webgl_frame();
            const auto backing = work->backing;
            const auto bytes = work->encoded.size();
            ready.insert_or_assign(backing, std::move(work));
            Mark(1, bytes);
            return;
        }
    } catch (const std::bad_alloc&) {
        // Speculative work may lose to an ordinary draw when memory is tight.
    }
    Mark(2, 0);
}

std::unique_ptr<Work> Take(const gpu::TextureDesc& desc, uint32_t backing, const uint8_t* pixels, size_t bytes) {
    if (desc.format != 0x02 || ready.empty()) return {};
    auto found = ready.find(backing);
    if (found == ready.end()) return {};
    auto work = std::move(found->second);
    ready.erase(found);
    if (work->encoded.size() != bytes || !SameLayout(desc, work->desc) ||
        !ff_webgl_equal_bytes(work->encoded.data(), pixels, bytes)) {
        Mark(3, bytes);
        return {};
    }
    Mark(4, bytes);
    return work;
}

void Expire(uint64_t frame) {
    for (auto it = ready.begin(); it != ready.end();) {
        if (frame - it->second->prepared_frame >= kReadyLifetime) it = ready.erase(it);
        else ++it;
    }
}

} // namespace ff_browser_texture

#include "guest-ready-texture.inc"
