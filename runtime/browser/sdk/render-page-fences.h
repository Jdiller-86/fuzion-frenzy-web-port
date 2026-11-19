#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>

#include "full-memory.h"
#include "gpu/texture.h"
#include "kernel/memory.h"

namespace ff_browser_render {

// Recorder-only state. Completion is published independently by the FIFO consumer.
class PageFences {
public:
    enum Access : uint32_t { Vertex = 1, Image = 2, Surface = 4, Palette = 8, Readback = 16, Uncaptured = 32 };

    void Unknown(uint64_t batch, Access kind) {
        if (unknown_batch_ != batch) unknown_kinds_ = 0;
        unknown_batch_ = batch;
        unknown_kinds_ |= kind;
    }

    // Preserve the backing window of the base pointer, including for SDK
    // consumers that use host pointer arithmetic after translating the base.
    void Record(uint64_t batch, uint32_t base, uint64_t begin, uint64_t end, Access kind) {
        using namespace ff_browser_memory;
        const uint32_t window = kWindowOffsets[base >> kWindowBits];
        const uint32_t offset = base & (kWindowSize - 1);
        if (window == kBackingSize || end <= begin || end > kWindowSize - offset) {
            Unknown(batch, kind);
            return;
        }
        if (window != kWindowSize) return;
        uint32_t first = (offset + uint32_t(begin)) / kPageBytes;
        const uint32_t last = (offset + uint32_t(end) - 1) / kPageBytes + 1;
        while (first < last && first % kGroupPages) SetPage(first++, batch);
        while (last - first >= kGroupPages) {
            groups_[first / kGroupPages].all = batch;
            first += kGroupPages;
        }
        while (first < last) SetPage(first++, batch);
    }

    void Pixels(uint64_t batch, uint32_t header, Access kind) {
        if (!header || AlreadyRecorded(batch, header, kind)) return;
        gpu::TextureDesc d;
        if (!gpu::Describe(header, d)) {
            Unknown(batch, kind);
            return;
        }
        const auto& format = gpu::Format(d.format);
        const uint32_t row = d.width * (format.bits / 8);
        if (d.width > 4096 || d.height > 4096 || d.cube || d.depth != 1 || format.host == gpu::Host::None ||
            (kind == Surface && (format.compressed || (format.bits != 16 && format.bits != 32) || d.levels != 1)) ||
            (d.linear && (d.pitch < row || format.compressed)) ||
            ((d.format == 0x24 || d.format == 0x25) && (d.width % 2 || !d.linear))) {
            Unknown(batch, kind);
            return;
        }
        // The admitted dimensions and formats bound TotalSize below 2^32.
        // Reference Texel may read two bytes from an 8-bit texel. The depth
        // fallback may access four bytes through a 16-bit surface descriptor.
        const bool wide_depth_access = (kind == Surface || format.depth) && format.bits == 16;
        const uint32_t tail = wide_depth_access ? 2u : uint32_t(format.bits == 8 && !format.compressed);
        const uint64_t bytes = uint64_t(gpu::TotalSize(d)) + tail;
        Record(batch, d.data, 0, bytes, kind);
    }

    void Colors(uint64_t batch, uint32_t header) {
        if (!header || AlreadyRecorded(batch, header, Palette)) return;
        std::array<uint32_t, 2> words;
        const auto* descriptor = ff_browser_memory::TryWindow(header, sizeof(words));
        if (!descriptor) {
            Unknown(batch, Palette);
            return;
        }
        std::memcpy(words.data(), descriptor, sizeof(words));
        const uint32_t bytes = (256u >> (words[0] >> 30)) * sizeof(uint32_t);
        Record(batch, mem::VirtualFromPhysical(words[1]), 0, bytes, Palette);
    }

    uint64_t LastAccess(uint32_t base, uint32_t bytes) const {
        using namespace ff_browser_memory;
        const uint32_t offset = base & (kWindowSize - 1);
        if (kWindowOffsets[base >> kWindowBits] != kWindowSize || !bytes || bytes > kWindowSize - offset)
            return UINT64_MAX;
        uint64_t latest = unknown_batch_;
        const uint32_t last = (offset + bytes - 1) / kPageBytes;
        for (uint32_t page = offset / kPageBytes; page <= last; ++page) {
            const auto& group = groups_[page / kGroupPages];
            latest = std::max({latest, group.all, group.pages[page % kGroupPages]});
        }
        return latest;
    }

    uint64_t UnknownBatch() const { return unknown_batch_; }
    uint32_t UnknownKinds() const { return unknown_kinds_; }

private:
    static constexpr uint32_t kPageBytes = 4096;
    static constexpr uint32_t kGroupPages = 64;
    struct Group {
        uint64_t all = 0;
        std::array<uint64_t, kGroupPages> pages{};
    };
    struct DescriptorStamp {
        uint64_t batch = 0;
        uint32_t header = 0;
        Access kind = Uncaptured;
    };

    void SetPage(uint32_t page, uint64_t batch) {
        groups_[page / kGroupPages].pages[page % kGroupPages] = batch;
    }

    bool AlreadyRecorded(uint64_t batch, uint32_t header, Access kind) {
        // Captured headers are immutable within a batch. Ring reuse drains it
        // and starts a new batch, so the same address can be stamped again.
        auto& stamp = descriptors_[((header >> 5) ^ kind) & 63];
        if (stamp.batch == batch && stamp.header == header && stamp.kind == kind) return true;
        stamp = {batch, header, kind};
        return false;
    }

    std::array<Group, ff_browser_memory::kWindowSize / (kPageBytes * kGroupPages)> groups_{};
    std::array<DescriptorStamp, 64> descriptors_{};
    uint64_t unknown_batch_ = 0;
    uint32_t unknown_kinds_ = 0;
};

} // namespace ff_browser_render

namespace gpu {
struct BrowserVertexReads {
    struct Stream {
        uint32_t base, stride;
        uint64_t tail;
    };
    std::array<Stream, 16> spans{};
    uint64_t batch = 0;
    uint32_t streams = 0;
    bool unknown = false;
};

void RecordBrowserVertexReads(const State& s, uint32_t prim, uint32_t start, uint32_t count,
                              const uint8_t* indices, ff_browser_render::PageFences& fences,
                              uint64_t batch, BrowserVertexReads& reuse);
}
