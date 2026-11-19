// Included at global scope after texture-warmup.h. Guest bytes are borrowed
// only until queue_warm_texture has copied the admitted source synchronously.
#include "full-memory.h"
#include "gpu/texture.h"
#include <array>
#include <cstdint>
#include <cstring>

namespace ff_browser_ready_texture {

inline uint32_t Read32(const uint8_t* p) {
    uint32_t value;
    std::memcpy(&value, p, sizeof(value));
    return value;
}

inline uint16_t Read16(const uint8_t* p) {
    uint16_t value;
    std::memcpy(&value, p, sizeof(value));
    return value;
}

// Match the original inclusive binary search, without calling guest code or
// keeping a view into a package after this producer returns.
inline const uint8_t* ResourceRow(const uint8_t* package, uint32_t id) {
    const uint32_t count = Read32(package + 0x0c);
    if (!count || count > 100000) return nullptr;
    const auto* table = ff_browser_memory::TryWindow(Read32(package + 0x10), size_t(count) * 32);
    if (!table) return nullptr;
    uint32_t lo = 0, hi = count - 1;
    while (lo <= hi) {
        const uint32_t mid = lo + (hi - lo) / 2;
        const auto* row = table + size_t(mid) * 32;
        const uint32_t key = Read32(row + 4);
        if (key == id) return row;
        if (key < id) lo = mid + 1;
        else {
            if (!mid) return nullptr;
            hi = mid - 1;
        }
    }
    return nullptr;
}

} // namespace ff_browser_ready_texture

void ff_browser_texture_bank_ready(uint32_t object, uint32_t stack) {
    if (!ff_browser_early_texture_decode_enabled() || !object) return;
    using ff_browser_memory::TryWindow;
    using ff_browser_ready_texture::Read16;
    using ff_browser_ready_texture::Read32;

    // At the reviewed successful RET the original package and resource-id
    // arguments follow the still-unconsumed guest return address.
    const auto* args = TryWindow(stack, 12);
    if (!args) return;
    const uint32_t package_va = Read32(args + 4);
    if (!package_va) return;
    const auto* package = TryWindow(package_va, 0x40);
    if (!package) return;
    const auto* row = ff_browser_ready_texture::ResourceRow(package, Read32(args + 8));
    if (!row) return;
    const uint32_t resource_bytes = Read32(row + 8);
    if (resource_bytes < 0x80 || resource_bytes > ff_browser_memory::kWindowSize) return;
    const auto* resource = TryWindow(object, resource_bytes);
    if (!resource) return;

    // Resident packages return a view; file-backed packages have already
    // completed the original allocation and read before reaching this RET.
    if (Read32(package) & 0x80000000u) {
        const uint64_t resident_va = uint64_t(Read32(package + 0x18)) +
                                     uint64_t(Read32(package + 4)) * Read32(row);
        if (resident_va > UINT32_MAX ||
            TryWindow(uint32_t(resident_va), resource_bytes) != resource) return;
    }

    // This trial admits the verified custom A1R5G5B5 tag only. Its frames
    // share dimensions and occupy independently aligned encoded spans.
    const uint32_t count = resource[0x37];
    const uint32_t width = Read16(resource + 0x2e), height = Read16(resource + 0x30);
    if (Read16(resource + 0x2c) != 3 || !count ||
        !width || width > 512 || !height || height > 512) return;
    const uint32_t headers_end = 0x80 + count * 24;
    const uint64_t payload_start = 0x80 + ((uint64_t(count) * 24 + 127) & ~uint64_t(127));
    const uint64_t stride = (uint64_t(Read32(resource + 0x28)) * 2 + 127) & ~uint64_t(127);
    if (headers_end > resource_bytes || payload_start >= resource_bytes ||
        !stride || stride > resource_bytes ||
        TryWindow(Read32(resource + 0x24), size_t(count) * 24) != resource + 0x80) return;

    // Validate the complete bank before queuing any of its frames. The array
    // is local: neither it nor its borrowed guest pointers survive this call.
    std::array<ff_browser_texture::Source, 255> sources;
    for (uint32_t i = 0; i < count; ++i) {
        const uint32_t header_offset = 0x80 + i * 24;
        const auto* header = resource + header_offset;
        if ((Read32(header) & 0x70000u) != 0x40000u || Read32(header + 16) ||
            ((Read32(header + 12) >> 4) & 15u) != 2) return;
        gpu::TextureDesc desc;
        if (!gpu::Describe(object + header_offset, desc) || desc.format != 0x02 ||
            desc.cube || desc.linear || desc.depth != 1 ||
            desc.width != width || desc.height != height) return;
        const uint32_t bytes = gpu::TotalSize(desc);
        const uint64_t offset = payload_start + uint64_t(i) * stride;
        if (!bytes || bytes > 1024 * 1024 || bytes > stride ||
            offset > resource_bytes || bytes > resource_bytes - offset) return;
        const auto* pixels = TryWindow(desc.data, bytes);
        if (pixels != resource + size_t(offset)) return;
        sources[i] = {desc, pixels, uint32_t(pixels - ff_sdk_guest_base()), bytes};
    }
    for (uint32_t i = 0; i < count; ++i) ff_browser_queue_warm_texture(sources[i]);
}
