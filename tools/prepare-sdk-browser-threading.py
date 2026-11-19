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
    backend_path = runtime / "gpu" / "backend.h"
    d3d_path = runtime / "xdk" / "d3d8.cpp"
    texture_path = runtime / "gpu" / "texture.cpp"
    if not all(source.is_file() for source in (render_path, backend_path, d3d_path, texture_path)):
        raise RuntimeError(f"not a copied SDK runtime overlay: {runtime}")

    render = render_path.read_text(encoding="utf-8")
    d3d = d3d_path.read_text(encoding="utf-8")
    backend = _replace_once(
        backend_path.read_text(encoding="utf-8"),
        "    virtual void WaitResource(uint32_t /*resource*/) {}",
        "    virtual void WaitResource(uint32_t /*resource*/, uint32_t /*slotBase*/ = 0, uint32_t /*slotBytes*/ = 0) {}",
        "Pass an optional observed vertex slot through the wait diagnostic",
    )
    backend = _replace_once(backend, '#include "gpu/state.h"\n',
        '#include "gpu/state.h"\n\nnamespace ff_browser_texture { struct Source; }\n',
        "Declare the borrowed packed-texture input")
    backend = _replace_once(backend, '    virtual void Sync(uint32_t data_va, uint32_t size) = 0;\n',
        '    virtual void Sync(uint32_t data_va, uint32_t size) = 0;\n'
        '    virtual void QueueWarmTexture(const ff_browser_texture::Source&) {}\n',
        "Default to ordinary first-use decode on direct backends")
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
        '#include "full-memory.h"\n#include "render-page-fences.h"\n#include "texture-warmup.h"\n#include <new>\n\n'
        'const uint8_t* ff_sdk_try_guest_ptr(uint32_t va, size_t bytes) noexcept;\n'
        'bool ff_browser_pacing_timing_enabled();\n'
        'int ff_browser_resource_fence_mode();\n'
        'void ff_webgl_begin_recorded_draws();\n'
        'void ff_webgl_invalidate_prepared_draw();\n\nnamespace gpu {\n',
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
        """    State sent_;  // the state as the render thread will have it""",
        """    State sent_;  // the state as the render thread will have it
    const int fence_mode_ = ff_browser_resource_fence_mode();
    std::unique_ptr<ff_browser_render::PageFences> pages_ = fence_mode_
        ? std::make_unique<ff_browser_render::PageFences>() : nullptr;""",
        "Optional recorder-owned physical page fences",
    )
    render = _replace_once(
        render,
        """        cur_->cmds.push_back(c);
        for (uint32_t tex : s.textures) Use(tex);""",
        """        if (pages_) {
            using Access = ff_browser_render::PageFences;
            const uint64_t batch = cur_->id;
            if (!ring_) pages_->Unknown(batch, Access::Uncaptured);
            else {
                RecordBrowserVertexReads(sent_, prim, start, count, c.d ? cur_->bytes.data() + c.data : nullptr,
                                        *pages_, batch);
                for (uint32_t texture : sent_.textures) pages_->Pixels(batch, texture, Access::Image);
                for (uint32_t palette : sent_.palettes) pages_->Colors(batch, palette);
                pages_->Pixels(batch, sent_.rt.header, Access::Surface);
                pages_->Pixels(batch, sent_.zs.header, Access::Surface);
            }
        }
        cur_->cmds.push_back(c);
        for (uint32_t tex : s.textures) Use(tex);""",
        "Capture draw footprints from the sent state and owned indices",
    )
    render = _replace_once(
        render,
        """        if (flags & kClearTarget) Written(s.rt);""",
        """        if (pages_) {
            using Access = ff_browser_render::PageFences;
            if (!ring_) pages_->Unknown(cur_->id, Access::Uncaptured);
            else {
                if (flags & kClearTarget) pages_->Pixels(cur_->id, sent_.rt.header, Access::Surface);
                if (flags & (kClearZ | kClearStencil)) pages_->Pixels(cur_->id, sent_.zs.header, Access::Surface);
            }
        }
        if (flags & kClearTarget) Written(s.rt);""",
        "Track flagged clear planes",
    )
    render = _replace_once(
        render,
        """        c.s2.header = Header(dst.header);
        cur_->cmds.push_back(c);""",
        """        c.s2.header = Header(dst.header);
        // Copy can read back an older cached target layout at the same address.
        if (pages_) pages_->Unknown(cur_->id, ff_browser_render::PageFences::Readback);
        cur_->cmds.push_back(c);""",
        "Keep a full barrier around Copy and its possible readbacks",
    )
    render = _replace_once(
        render,
        """        c.s1.header = Header(front.header);
        cur_->cmds.push_back(c);""",
        """        c.s1.header = Header(front.header);
        if (pages_) {
            using Access = ff_browser_render::PageFences;
            if (ring_) pages_->Pixels(cur_->id, c.s1.header, Access::Surface);
            else pages_->Unknown(cur_->id, Access::Uncaptured);
        }
        cur_->cmds.push_back(c);""",
        "Track the source bytes that Present may decode in software",
    )
    render = _replace_once(
        render,
        """        c.b = size;
        cur_->cmds.push_back(c);
        Wait(Submit(), WaitFor::ReadBack, "read-back");""",
        """        c.b = size;
        if (pages_ && size) pages_->Unknown(cur_->id, ff_browser_render::PageFences::Readback);
        cur_->cmds.push_back(c);
        Wait(Submit(), WaitFor::ReadBack, "read-back");""",
        "Keep a full barrier around queued readback writes",
    )
    render = _replace_once(
        render,
        '        cur_ = NewBatch();\n        std::thread',
        '        cur_ = new Batch;\n        cur_->id = ++next_id_;\n        std::thread',
        "Initial recording batch before the worker starts",
    )
    render = _replace_once(
        render,
        """    Batch* NewBatch() {
        Batch* b;
        {
            std::lock_guard lock(m_);
            if (free_.empty()) {
                b = new Batch;
            } else {
                b = free_.back();
                free_.pop_back();
            }
        }
        b->id = ++next_id_;
        return b;
    }

""",
        "",
        "Remove the second batch-pool lock",
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
        const uint64_t id = cur_->id;
        uint64_t oldest = 0;
        bool wake_worker;
        {
            std::lock_guard lock(m_);
            // Obtain the successor before handing the current batch to the worker.
            std::unique_ptr<Batch> created;
            if (free_.empty()) created = std::make_unique<Batch>();
            Batch* next = created ? created.get() : free_.back();
            wake_worker = queue_.empty();
            queue_.push_back(cur_);
            if (created) (void)created.release();
            else free_.pop_back();
            next->id = ++next_id_;
            cur_ = next;
            if (queue_.size() >= 128) oldest = queue_.front()->id;
        }
        if (wake_worker) cv_.notify_one();
        // Never hold m_ while waiting; worker completion needs that mutex.
        if (oldest) Wait(oldest, WaitFor::Resource, "render queue backpressure");
        return id;
    }""",
        "Publish and replenish bounded render batches under one lock",
    )
    render = _replace_once(
        render,
        """    void WaitResource(uint32_t resource) override {
        auto it = last_use_.find(resource);
        if (it != last_use_.end()) Wait(it->second, WaitFor::Resource, "resource");
    }""",
        """    void WaitResource(uint32_t resource, uint32_t slotBase, uint32_t slotBytes) override {
        uint32_t header[2];
        bool sampled = false;
        if (resource && ff_browser_pacing_timing_enabled()) {
            if (const auto* span = ff_sdk_try_guest_ptr(resource, sizeof(header))) {
                std::memcpy(header, span, sizeof(header));
                sampled = true;
            }
        }
        const uint64_t completed = sampled ? done_.load() : 0;
        uint64_t last = 0;
        if (sampled || (pages_ && slotBytes)) {
            const auto use = last_use_.find(resource);
            if (use != last_use_.end()) last = use->second;
        }
        const bool recording = !cur_->cmds.empty() || !cur_->bytes.empty();
        const uint64_t full = recording ? cur_->id : cur_->id - 1;
        const uint64_t page_fence = pages_ && slotBytes
            ? std::min(full, std::max(last, pages_->LastAccess(slotBase, slotBytes))) : full;
        const bool selective = fence_mode_ == 2 && page_fence < full;
        const uint64_t through = selective ? page_fence : (recording ? Submit() : full);
        const uint64_t waited = Wait(through, WaitFor::Resource, "resource");
        if (sampled && (waited >= 500000 || selective)) {
            const double finished = emscripten_get_now();
            const auto data = mem::VirtualFromPhysical(header[1]);
            const auto backing = mem::VirtualFromPhysical(mem::PhysicalFromVirtual(data));
            const uint32_t allocation = mem::AllocationSize(backing);
            const auto owner = ff_browser_memory::FindContiguousAllocation(backing);
            const uint64_t unknown = pages_ ? pages_->UnknownBatch() : 0;
            const uint32_t unknown_kinds = pages_ ? pages_->UnknownKinds() : 0;
            MAIN_THREAD_ASYNC_EM_ASM({
                Module.onFuzionPacingResourceWait?.($0 >>> 0, $1, $2 >>> 0, $3, $4, $5, $6, $7, $8 >>> 0,
                                                   $9 >>> 0, $10 >>> 0, $11 >>> 0, $12 >>> 0, $13, $14, $15);
            }, resource, header[0] & 0x70000, backing, waited / 1e6, double(through), finished,
               double(last), double(completed), allocation, owner.base, owner.bytes, slotBase, slotBytes,
               double(page_fence), double(unknown), unknown_kinds);
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
    render = _replace_once(
        render,
        '    void Run() {\n',
        '    void Run() {\n        ff_webgl_begin_recorded_draws();\n',
        "Enable draw preparation reuse on the command consumer",
    )
    render = _replace_once(render, '    std::deque<Batch*> queue_;\n',
        '    std::deque<Batch*> queue_;\n'
        '    std::deque<std::unique_ptr<ff_browser_texture::Work>> warm_textures_;\n',
        "Only the existing preparation worker consumes early textures")
    render = _replace_once(render,
        '    bool Init(uint32_t width, uint32_t height) override { return inner_->Init(width, height); }\n',
        '''    void QueueWarmTexture(const ff_browser_texture::Source& source) override {
        auto work = ff_browser_texture::Copy(source);
        if (!work) return;
        {
            std::unique_lock lock(m_, std::try_to_lock);
            if (!lock.owns_lock() || stop_) return;
            try {
                warm_textures_.push_back(std::move(work));
            } catch (const std::bad_alloc&) {
                return;
            }
        }
        cv_.notify_one();
    }

    bool Init(uint32_t width, uint32_t height) override { return inner_->Init(width, height); }
''', "Publish an owned copy without waiting on the render queue")
    render = _replace_once(
        render,
        """        for (;;) {
            Batch* b;
            {
                std::unique_lock lock(m_);
                cv_.wait(lock, [&] { return stop_ || !queue_.empty(); });
                if (queue_.empty()) break;
                b = queue_.front();
                queue_.pop_front();
            }
            for (const Cmd& c : b->cmds) Execute(*b, c);""",
        """        Batch* b = nullptr;
        for (;;) {
            if (!b) {
                std::unique_ptr<ff_browser_texture::Work> warm;
                {
                    std::unique_lock lock(m_);
                    cv_.wait(lock, [&] { return stop_ || !queue_.empty() || !warm_textures_.empty(); });
                    if (!queue_.empty()) {
                        b = queue_.front();
                        queue_.pop_front();
                    } else {
                        if (stop_) break;
                        warm = std::move(warm_textures_.front());
                        warm_textures_.pop_front();
                    }
                }
                if (warm) {
                    ff_browser_texture::Prepare(std::move(warm));
                    continue;
                }
            }
            ff_browser_texture::FlushUploads();
            for (const Cmd& c : b->cmds) Execute(*b, c);""",
        "Wait for a batch only when none was taken at completion",
    )
    render = _replace_once(
        render,
        """                free_.push_back(b);
                done_.store(id);
            }
            done_cv_.notify_all();""",
        """                free_.push_back(b);
                done_.store(id);
                b = nullptr;
                // Take the next queued batch under the completion lock.
                if (!queue_.empty()) {
                    b = queue_.front();
                    queue_.pop_front();
                }
            }
            done_cv_.notify_all();""",
        "Combine completion publication with the next queue pop",
    )
    render = _replace_once(
        render,
        '    void Execute(const Batch& b, const Cmd& c) {\n',
        '    void Execute(const Batch& b, const Cmd& c) {\n'
        '        if (c.op != Op::Draw) ff_webgl_invalidate_prepared_draw();\n',
        "Invalidate prepared draws at state and non-draw commands",
    )

    d3d = _replace_once(
        d3d,
        "HLE_STDCALL(D3D_BlockOnResource, 1) {",
        '#include "guest-vertex-lock.inc"\n\nbool ff_browser_pacing_timing_enabled();\n'
        'int ff_browser_resource_fence_mode();\n\n'
        '#include "texture-warmup.h"\n\n'
        'void ff_browser_queue_warm_texture(const ff_browser_texture::Source& source) {\n'
        '    std::lock_guard<std::recursive_mutex> lock(g_lock);\n'
        '    if (g_backend) g_backend->QueueWarmTexture(source);\n'
        '}\n\n'
        "HLE_STDCALL(D3D_BlockOnResource, 1) {",
        "Use the checked title vertex lock observer only on the browser",
    )
    d3d = _replace_once(
        d3d,
        """            g_backend->Sync(data, SurfaceBytes(res));
        } else {
            g_backend->WaitResource(res);""",
        """            g_backend->WaitResource(res);
            g_backend->Sync(data, SurfaceBytes(res));
        } else {
            const auto slot = type == 0 && (ff_browser_resource_fence_mode() || ff_browser_pacing_timing_enabled())
                ? ff_browser_vertex_lock::FindSlot(ctx.esp, res) : ff_browser_vertex_lock::Slot{};
            g_backend->WaitResource(res, slot.base, slot.bytes);""",
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
        """uint64_t BrowserDirtyRevision() {
    return g_epoch.load(std::memory_order_acquire);
}

uint64_t DirtyEpoch(uint32_t data) {
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
    backend_path.write_text(backend, encoding="utf-8", newline="\n")
    d3d_path.write_text(d3d, encoding="utf-8", newline="\n")
    texture_path.write_text(texture, encoding="utf-8", newline="\n")
    return {
        "runtime": str(runtime),
        "files": ["gpu/render_thread.cpp", "gpu/backend.h", "xdk/d3d8.cpp", "gpu/texture.cpp"],
        "changes": [
            "contiguous 32-header-slot plus payload scratch reservations before Draw/Clear/Copy and Present",
            "wait for submitted consumers before scratch-ring lap reuse; clear stale header-copy references",
            "abort on oversized reservation or any scratch allocation beyond its reservation",
            "skip resource header capture calls for empty bindings or an unavailable scratch ring",
            "capture each resource header from a checked single-window span while admitting short window-end headers",
            "publish a batch and take its successor under the same queue lock",
            "wake the consumer on empty-to-nonempty transitions and take queued work under the completion lock",
            "invalidate prepared raw draw state at state updates and non-draw commands",
            "wait on the oldest queued batch when pending queue reaches 128, outside the queue mutex",
            "track depth/stencil writes only when Draw enables Z writes or stencil; flagged Clear is tracked",
            "optional page fences for validated rotating vertex slots, with a full barrier for unknown accesses",
            "pacing diagnostics record the full or selective drain and the latest page and unknown batch IDs",
            "pixel header descriptions read all required fields through one checked guest mapping",
            "dirty epoch reads reuse thread-local values at the same published revision, with locked lookup on misses",
            "submit Clear/Copy/RecordLate/Gamma once a complete command reaches the batch threshold",
            "NOOVERWRITE path remains unchanged",
        ],
    }
