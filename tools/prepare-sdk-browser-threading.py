#!/usr/bin/env python3
"""Adapt the copied SDK render thread and resource tracking for browser use.

This deliberately edits only a disposable runtime overlay below this checkout's
.tools/artifacts/ or artifacts/ directory. The pinned SDK is never modified.
"""
from __future__ import annotations

from pathlib import Path


def _replace_once(text: str, before: str, after: str, label: str) -> str:
    count = text.count(before)
    if count != 1:
        raise RuntimeError(f"{label}: expected one source anchor, found {count}")
    return text.replace(before, after, 1)


def adapt_browser_threading(runtimeRoot: str | Path) -> dict[str, object]:
    """Apply the browser-threading safety adaptations to one copied SDK runtime."""
    runtime = Path(runtimeRoot).resolve()
    project = Path(__file__).resolve().parents[1]
    allowed = (project / ".tools" / "artifacts", project / "artifacts")
    if not any(runtime.is_relative_to(base.resolve()) and runtime != base.resolve() for base in allowed):
        raise RuntimeError("threading adapter requires a disposable runtime overlay under .tools/artifacts or artifacts")
    render_path = runtime / "gpu" / "render_thread.cpp"
    d3d_path = runtime / "xdk" / "d3d8.cpp"
    texture_path = runtime / "gpu" / "texture.cpp"
    if not all(source.is_file() for source in (render_path, d3d_path, texture_path)):
        raise RuntimeError(f"not a copied SDK runtime overlay: {runtime}")

    render = render_path.read_text(encoding="utf-8")
    d3d = d3d_path.read_text(encoding="utf-8")
    render = _replace_once(render,
        "    uint32_t Put(const void* p, size_t n) {\n        size_t at =",
        "    uint32_t Put(const void* p, size_t n) {\n        if(n>128u*1024*1024 || cur_->bytes.size()>256u*1024*1024-n-3) std::abort();\n        size_t at =",
        "Bound captured payload bytes")

    render = _replace_once(
        render,
        """        PerfTimer perf;  // the capture leaf and its state part start together
        SendState(s);""",
        """        PerfTimer perf;  // the capture leaf and its state part start together
        if(indices && count>16u*1024*1024) std::abort();
        ReserveScratch(0);
        SendState(s);""",
        "Draw scratch reservation",
    )
    render = _replace_once(
        render,
        """        Written(s.rt);
        if (cur_->cmds.size() >= kBatchCommands) Submit();""",
        """        Written(s.rt);
        if ((s.rs[kRsZEnable] && s.simple[kMZWriteEnable >> 2]) || s.rs[kRsStencilEnable]) Written(s.zs);
        if (cur_->cmds.size() >= kBatchCommands) Submit();""",
        "Draw depth/stencil write tracking",
    )
    render = _replace_once(
        render,
        """        PerfTimer perf;
        SendState(s);
        perf.Add(g_perf_sendstate_ns);""",
        """        PerfTimer perf;
        if(count>8u*1024*1024) std::abort();
        ReserveScratch(0);
        SendState(s);
        perf.Add(g_perf_sendstate_ns);""",
        "Clear scratch reservation",
    )
    render = _replace_once(
        render,
        """        if (flags & kClearTarget) Written(s.rt);
        perf.Add(g_perf_capture_ns);""",
        """        if (flags & kClearTarget) Written(s.rt);
        if (flags & (kClearZ | kClearStencil)) Written(s.zs);
        if (cur_->cmds.size() >= kBatchCommands) Submit();
        perf.Add(g_perf_capture_ns);""",
        "Clear depth/stencil write tracking",
    )
    render = _replace_once(
        render,
        """        PerfTimer perf;
        Cmd c{Op::Copy};
        c.a = count;
        c.b = count && src_rects ? Scratch(kernel::HostPtr(src_rects), count * 16) : 0;
        c.c = count && dst_points ? Scratch(kernel::HostPtr(dst_points), count * 8) : 0;""",
        """        PerfTimer perf;
        constexpr size_t scratch_payload_limit = kHeaderRing - 32u * kHeaderSlot;
        if ((count && src_rects && count > scratch_payload_limit / 16) ||
            (count && dst_points && count > scratch_payload_limit / 8)) std::abort();
        const size_t rect_bytes = count && src_rects ? ((size_t(count) * 16 + kHeaderSlot - 1) & ~(size_t(kHeaderSlot) - 1)) : 0;
        const size_t point_bytes = count && dst_points ? ((size_t(count) * 8 + kHeaderSlot - 1) & ~(size_t(kHeaderSlot) - 1)) : 0;
        if (rect_bytes > scratch_payload_limit || point_bytes > scratch_payload_limit - rect_bytes) std::abort();
        ReserveScratch(rect_bytes + point_bytes);
        Cmd c{Op::Copy};
        c.a = count;
        c.b = count && src_rects ? Scratch(kernel::HostPtr(src_rects), count * 16) : 0;
        c.c = count && dst_points ? Scratch(kernel::HostPtr(dst_points), count * 8) : 0;""",
        "Copy scratch reservation",
    )
    render = _replace_once(
        render,
        """        Use(dst.header);
        Written(dst);
        perf.Add(g_perf_capture_ns);""",
        """        Use(dst.header);
        Written(dst);
        if (cur_->cmds.size() >= kBatchCommands) Submit();
        perf.Add(g_perf_capture_ns);""",
        "Copy command threshold",
    )
    render = _replace_once(
        render,
        """        cur_->cmds.push_back(c);
    }

    void Flush() override""",
        """        cur_->cmds.push_back(c);
        if (cur_->cmds.size() >= kBatchCommands) Submit();
    }

    void Flush() override""",
        "RecordLate command threshold",
    )
    render = _replace_once(
        render,
        """    void SetGamma(const uint8_t* ramp) override {
        Cmd c{Op::Gamma};
        c.data = Put(ramp, 768);
        cur_->cmds.push_back(c);
    }""",
        """    void SetGamma(const uint8_t* ramp) override {
        Cmd c{Op::Gamma};
        c.data = Put(ramp, 768);
        cur_->cmds.push_back(c);
        if (cur_->cmds.size() >= kBatchCommands) Submit();
    }""",
        "Gamma command threshold",
    )
    render = _replace_once(
        render,
        """        PerfTimer perf;
        Cmd c{Op::Present};""",
        """        PerfTimer perf;
        ReserveScratch(0);
        Cmd c{Op::Present};""",
        "Present header reservation",
    )
    render = _replace_once(
        render,
        """    uint32_t Scratch(const void* p, uint32_t n) {
        uint32_t size = (n + kHeaderSlot - 1) & ~(kHeaderSlot - 1);
        if (ring_pos_ % kHeaderRing + size > kHeaderRing) ring_pos_ += kHeaderRing - ring_pos_ % kHeaderRing;
        uint32_t at = ring_ + uint32_t(ring_pos_ % kHeaderRing);
        ring_pos_ += size;
        std::memcpy(kernel::HostPtr(at), p, n);
        return at;
    }""",
        """    void ReserveScratch(size_t extra) {
        constexpr size_t header_reserve = 32u * kHeaderSlot;
        if (extra > kHeaderRing - header_reserve) std::abort();
        const size_t reserve = (header_reserve + extra + kHeaderSlot - 1) & ~(size_t(kHeaderSlot) - 1);
        if (reserve > kHeaderRing - size_t(ring_pos_ % kHeaderRing)) {
            // No partial state/payload for the next operation exists yet; draining here
            // makes reuse of the ring's following lap safe for all prior consumers.
            Wait(Submit(), WaitFor::Resource, "header scratch ring");
            ring_pos_ += kHeaderRing - ring_pos_ % kHeaderRing;
            headers_.clear();
            recent_headers_.fill({});
        }
        scratch_reserve_end_ = ring_pos_ + reserve;
    }

    uint32_t Scratch(const void* p, uint32_t n) {
        if (n > kHeaderRing - kHeaderSlot) std::abort();
        uint32_t size = (n + kHeaderSlot - 1) & ~(kHeaderSlot - 1);
        if (ring_pos_ > scratch_reserve_end_ || size > scratch_reserve_end_ - ring_pos_ ||
            ring_pos_ % kHeaderRing + size > kHeaderRing) std::abort();
        uint32_t at = ring_ + uint32_t(ring_pos_ % kHeaderRing);
        ring_pos_ += size;
        std::memcpy(kernel::HostPtr(at), p, n);
        return at;
    }""",
        "Scratch ring reservation and fence",
    )
    render = _replace_once(
        render,
        'namespace gpu {\n',
        'const uint8_t* ff_sdk_try_guest_ptr(uint32_t va, size_t bytes) noexcept;\n'
        'bool ff_browser_pacing_timing_enabled();\n\nnamespace gpu {\n',
        "Optional checked browser span declaration",
    )
    render = _replace_once(render, '#include <smmintrin.h>\n', '#include <smmintrin.h>\n#include <emscripten.h>\n', "Resource wait timing bridge")
    render = _replace_once(
        render,
        """    uint32_t Header(uint32_t header) {
        if (!header || !ring_) return header;""",
        """    [[gnu::always_inline]] uint32_t Header(uint32_t header) {
        if (!header || !ring_) return header;
        return CaptureHeader(header);
    }

    uint32_t CaptureHeader(uint32_t header) {""",
        "Skip out-of-line header capture for empty bindings",
    )
    render = _replace_once(
        render,
        """        const uint32_t type = kernel::R32(header) & 0x70000;  // 0 vertex, 0x10000 index, 0x30000 palette
        uint8_t now[kHeaderBytes] = {};
        std::memcpy(now, kernel::HostPtr(header), type < 0x20000 || type == 0x30000 ? 12 : kHeaderBytes);""",
        """        constexpr size_t short_bytes = 12;
        const auto* whole = ff_sdk_try_guest_ptr(header, kHeaderBytes);
        const auto* data = whole ? whole : ff_sdk_guest_ptr(header, short_bytes);
        uint8_t now[kHeaderBytes] = {};
        std::memcpy(now, data, short_bytes);
        uint32_t common;
        std::memcpy(&common, now, sizeof(common));
        const uint32_t type = common & 0x70000;
        if (type >= 0x20000 && type != 0x30000) {
            // The long header must fit within the same admitted guest window.
            if (!whole) data = ff_sdk_guest_ptr(header, kHeaderBytes);
            std::memcpy(now + short_bytes, data + short_bytes, kHeaderBytes - short_bytes);
        }""",
        "One checked translation for each full resource header",
    )
    render = _replace_once(
        render,
        """    uint32_t ring_ = 0;
    uint64_t ring_pos_ = 0;""",
        """    uint32_t ring_ = 0;
    uint64_t ring_pos_ = 0, scratch_reserve_end_ = 0;""",
        "Scratch reservation state",
    )
    render = _replace_once(
        render,
        """    uint64_t Submit() {
        uint64_t id = cur_->id;
        {
            std::lock_guard lock(m_);
            queue_.push_back(cur_);
        }
        cv_.notify_one();
        cur_ = NewBatch();
        return id;
    }""",
        """    uint64_t Submit() {
        uint64_t id = cur_->id, oldest = 0;
        size_t pending = 0;
        {
            std::lock_guard lock(m_);
            queue_.push_back(cur_);
            pending = queue_.size();
            if (pending >= 128) oldest = queue_.front()->id;
        }
        cv_.notify_one();
        cur_ = NewBatch();
        // Never hold m_ while waiting; worker completion needs that mutex.
        if (oldest) Wait(oldest, WaitFor::Resource, "render queue backpressure");
        return id;
    }""",
        "Bound pending render batches",
    )
    render = _replace_once(
        render,
        """    void WaitResource(uint32_t resource) override {
        auto it = last_use_.find(resource);
        if (it != last_use_.end()) Wait(it->second, WaitFor::Resource, "resource");
    }""",
        """    void WaitResource(uint32_t resource) override {
        uint32_t header[2];
        bool sampled = false;
        if (resource && ff_browser_pacing_timing_enabled()) {
            if (const auto* span = ff_sdk_try_guest_ptr(resource, sizeof(header))) {
                std::memcpy(header, span, sizeof(header));
                sampled = true;
            }
        }
        // A guest resource can alias another header or backing span. Until the
        // browser route proves canonical spans, drain all prior work here.
        const uint64_t through = (!cur_->cmds.empty() || !cur_->bytes.empty()) ? Submit() : cur_->id - 1;
        const uint64_t waited = Wait(through, WaitFor::Resource, "resource");
        if (sampled && waited >= 500000) {
            const auto data = mem::VirtualFromPhysical(header[1]);
            const auto backing = mem::VirtualFromPhysical(mem::PhysicalFromVirtual(data));
            MAIN_THREAD_ASYNC_EM_ASM({
                Module.onFuzionPacingResourceWait?.($0 >>> 0, $1, $2 >>> 0, $3, $4, $5);
            }, resource, header[0] & 0x70000, backing, waited / 1e6, double(through), emscripten_get_now());
        }
    }""",
        "Alias-safe resource barrier",
    )
    render = _replace_once(
        render,
        """    void Wait(uint64_t id, WaitFor kind, const char* why) {
        if (!id || done_.load() >= id) return;""",
        """    uint64_t Wait(uint64_t id, WaitFor kind, const char* why) {
        if (!id || done_.load() >= id) return 0;""",
        "Read the measured resource wait duration",
    )
    render = _replace_once(
        render,
        '        }\n    }\n\n    void Run() {',
        '        }\n        return ns;\n    }\n\n    void Run() {',
        "Return the existing wait counter sample",
    )

    d3d = _replace_once(
        d3d,
        """            g_backend->Sync(data, SurfaceBytes(res));
        } else {
            g_backend->WaitResource(res);""",
        """            g_backend->WaitResource(res);
            g_backend->Sync(data, SurfaceBytes(res));
        } else {
            g_backend->WaitResource(res);""",
        "Wait for resource reads before CPU readback",
    )

    texture = texture_path.read_text(encoding="utf-8")
    texture = _replace_once(texture, '#include <bit>\n', '#include <atomic>\n#include <bit>\n', "Dirty revision include")
    texture = _replace_once(
        texture,
        'namespace gpu {\n',
        'const uint8_t* ff_sdk_try_guest_ptr(uint32_t va, size_t bytes) noexcept;\n\nnamespace gpu {\n',
        "Checked pixel header span declaration",
    )
    texture = _replace_once(
        texture,
        """    uint32_t common = R32(header);
    uint32_t type = common & 0x70000;  // 0x40000 texture, 0x50000 surface
    if (type != 0x40000 && type != 0x50000) return false;
    // D3DPixelContainer: Common, Data, Lock, Format, Size
    uint32_t fmt = R32(header + 12), size = R32(header + 16);
    d.data = mem::VirtualFromPhysical(R32(header + 4));""",
        """    constexpr size_t header_bytes = 20;
    const auto* span = ff_sdk_try_guest_ptr(header, header_bytes);
    uint32_t common;
    if (span) std::memcpy(&common, span, sizeof(common));
    else common = R32(header);
    const uint32_t type = common & 0x70000;
    if (type != 0x40000 && type != 0x50000) return false;
    if (!span) span = ff_sdk_guest_ptr(header, header_bytes);
    uint32_t data, fmt, size;
    std::memcpy(&fmt, span + 12, sizeof(fmt));
    std::memcpy(&size, span + 16, sizeof(size));
    std::memcpy(&data, span + 4, sizeof(data));
    d.data = mem::VirtualFromPhysical(data);""",
        "Decode the pixel header through one checked guest mapping",
    )
    texture = _replace_once(texture, 'uint64_t g_epoch = 1;', 'std::atomic<uint64_t> g_epoch{1};', "Published dirty revision")
    texture = _replace_once(
        texture,
        "    g_dirty[data] = ++g_epoch;",
        """    const uint64_t changed = g_epoch.load(std::memory_order_relaxed) + 1;
    g_dirty[data] = changed;
    g_epoch.store(changed, std::memory_order_release);""",
        "Publish only completed dirty-map updates",
    )
    texture = _replace_once(
        texture,
        """uint64_t DirtyEpoch(uint32_t data) {
    std::lock_guard<std::mutex> lock(g_dirty_lock);
    auto it = g_dirty.find(data);
    return it == g_dirty.end() ? 0 : it->second;
}""",
        """uint64_t DirtyEpoch(uint32_t data) {
    struct RecentEpoch {
        uint32_t data = 0;
        uint64_t revision = 0;
        uint64_t epoch = 0;
    };
    static thread_local RecentEpoch recent[32];
    auto& entry = recent[((data >> 12) ^ (data >> 5)) & 31];
    const uint64_t revision = g_epoch.load(std::memory_order_acquire);
    // The revision read is the observation point for a cache hit.
    if (entry.data == data && entry.revision == revision) return entry.epoch;

    std::lock_guard<std::mutex> lock(g_dirty_lock);
    const auto it = g_dirty.find(data);
    const uint64_t epoch = it == g_dirty.end() ? 0 : it->second;
    entry = {data, g_epoch.load(std::memory_order_relaxed), epoch};
    return epoch;
}""",
        "Reuse dirty lookups while the published revision matches",
    )

    render_path.write_text(render, encoding="utf-8", newline="\n")
    d3d_path.write_text(d3d, encoding="utf-8", newline="\n")
    texture_path.write_text(texture, encoding="utf-8", newline="\n")
    return {
        "runtime": str(runtime),
        "files": ["gpu/render_thread.cpp", "xdk/d3d8.cpp", "gpu/texture.cpp"],
        "changes": [
            "contiguous 32-header-slot plus payload scratch reservations before Draw/Clear/Copy and Present",
            "wait for submitted consumers before scratch-ring lap reuse; clear stale header-copy references",
            "abort on oversized reservation or any scratch allocation beyond its reservation",
            "skip resource header capture calls for empty bindings or an unavailable scratch ring",
            "capture each resource header from a checked single-window span while admitting short window-end headers",
            "wait on the oldest queued batch when pending queue reaches 128, outside the queue mutex",
            "track depth/stencil writes only when Draw enables Z writes or stencil; flagged Clear is tracked",
            "resource waits drain all preceding work to preserve aliases; D3D_BlockOnResource waits before Sync",
            "pacing diagnostics relate waits of at least 0.5 ms to the resource header, canonical data address and batch",
            "pixel header descriptions read all required fields through one checked guest mapping",
            "dirty epoch reads reuse thread-local values at the same published revision, with locked lookup on misses",
            "submit Clear/Copy/RecordLate/Gamma once a complete command reaches the batch threshold",
            "NOOVERWRITE path remains unchanged",
        ],
    }
