#!/usr/bin/env python3
"""Admit checked single-stream raw vertices in the disposable browser SDK copy."""
from pathlib import Path
import hashlib
import json


def adapt_browser_vertices(runtime_root: str | Path) -> None:
    runtime = Path(runtime_root).resolve()
    root = Path(__file__).resolve().parents[1]
    if not any(runtime.is_relative_to(base) and runtime != base for base in (root/'.tools/artifacts', root/'artifacts')):
        raise RuntimeError('Raw vertex adaptation requires a private disposable SDK overlay')
    path = runtime/'gpu/vertex.cpp'
    text = path.read_text(encoding='utf-8')
    before = hashlib.sha256(text.encode()).hexdigest()

    def replace(old: str, new: str) -> None:
        nonlocal text
        if text.count(old) != 1:
            raise RuntimeError('Browser raw vertex source anchor differs: '+old[:64])
        text = text.replace(old, new, 1)

    replace('namespace gpu {', '#include <bit>\n#include <cstdlib>\n#include "render-page-fences.h"\n'
            'bool ff_webgl_raw_vertices_enabled();\n\nnamespace gpu {')
    replace('bool RawReadable(const State& s, const Inputs& inputs) {', '''bool RawReadable(const State& s, const Inputs& inputs) {
    // The first browser admission deliberately keeps a single native stream.
    // Other layouts retain the original converted representation.
    int stream = -1;
    for (int k = 0; k < inputs.nregs; ++k) {
        const auto& in = inputs.layout.inputs[size_t(inputs.regs[k])];
        const auto he = HostFormat(in.type);
        const uint32_t stride = StrideOf(s, inputs.layout, in);
        if (stream < 0) stream = in.stream;
        if (stream != in.stream || !he.format || !stride || stride > 252 ||
            uint64_t(in.offset) + he.bytes > stride) return false;
    }''')
    replace('constexpr uint64_t kDenseSpanPerVertex = 4;', '''struct BrowserRawLayout {
    std::array<VertexElement, 16> elements{};
    uint64_t decode = 0;
    uint16_t regs = 0;
    uint32_t stream = 0, stride = 0;
};

// Only the recorded consumer enables reuse. It invalidates the projection
// before state and other non-draw commands. The projection owns only values.
thread_local bool browser_recorded_vertices = false;
thread_local BrowserRawLayout browser_raw_layout;

BrowserRawLayout ProjectBrowserRawLayout(const State& s, const Inputs& inputs, uint16_t regs) {
    BrowserRawLayout projection;
    const auto& first = inputs.layout.inputs[size_t(inputs.regs[0])];
    projection.stream = uint32_t(first.stream);
    projection.stride = StrideOf(s, inputs.layout, first);
    projection.regs = regs;
    for (int k = 0; k < inputs.nregs; ++k) {
        const int reg = inputs.regs[k];
        const auto& input = inputs.layout.inputs[size_t(reg)];
        const auto element = HostFormat(input.type);
        projection.elements[size_t(reg)] = {element.format, 0, uint16_t(input.offset)};
        projection.decode |= uint64_t(element.decode) << (reg * 4);
    }
    return projection;
}

constexpr uint64_t kDenseSpanPerVertex = 4;''')
    replace('''int AddRawBuffer(const State& s, const VertexShader& layout, const VertexShader::Input& in, const DrawRange& d,
                 bool dense, Assembled& out) {''', '''void AddRawBuffer(const State& s, uint32_t stream, uint32_t stride, const DrawRange& d,
                  bool dense, Assembled& out) {''')
    replace('''    const uint8_t* data = static_cast<const uint8_t*>(HostPtr(mem::VirtualFromPhysical(R32(st.vb + kResData))));
    int slot = int(out.buffer_count);''', '''    const uint32_t address = mem::VirtualFromPhysical(R32(st.vb + kResData));
    const uint64_t end = (uint64_t(d.first_vertex) + d.span) * stride;
    const uint64_t captured = uint64_t(dense ? d.span : d.count) * stride;
    // Check the complete source range before either the dense view or sparse
    // gather can read it; pointer arithmetic cannot wrap to another guest bank.
    if (!end || end > 128u * 1024 * 1024 || captured > 128u * 1024 * 1024) std::abort();
    const uint8_t* data = ff_sdk_guest_ptr(address, size_t(end));''')
    replace('''    const auto& st = s.streams[size_t(in.stream)];
    uint32_t stride = StrideOf(s, layout, in);
    const uint32_t address''', '''    const auto& st = s.streams[stream];
    const uint32_t address''')
    replace('    return slot;\n}', '}')
    raw_begin = text.index('bool AssembleRaw(')
    raw_body_end = text.index('    out.vertex_count = dense ? d.span : d.count;', raw_begin)
    raw_body = text[raw_begin:raw_body_end]
    if hashlib.sha256(raw_body.encode()).hexdigest() != 'a4fe86f6c164979b5584e97835450accf31a158127478653a71db1abfc83726e':
        raise RuntimeError('Browser raw assembly requires the pinned SDK body')
    replace(raw_body, '''bool AssembleRaw(const State& s, const BrowserRawLayout& projection, const DrawRange& d, Assembled& out) {
    if (d.count > 1024u * 1024 || !d.span) std::abort();
    const bool dense = uint64_t(d.span) <= uint64_t(d.count) * kDenseSpanPerVertex + kDenseSpanSlack;
    AddRawBuffer(s, projection.stream, projection.stride, d, dense, out);
    out.regs = projection.regs;
    out.elements = projection.elements;
    out.decode = projection.decode;
''')
    replace('    if (NativePrimitive(d.prim)) return DrawNative(d, dense, out);',
            '    // Use the same ordered list expansion as the converted reference.\n'
            '    // WebGL has no base-vertex draw: the list below rebases every index.')
    anchor = 'bool Assemble(const State& s, uint32_t prim, uint32_t start, uint32_t count, const uint8_t* ip, Assembled& out) {'
    replace(anchor, '''void ResetBrowserRawLayout() {
    browser_recorded_vertices = true;
    browser_raw_layout.regs = 0;
}

static void CaptureBrowserVertexReads(const State& s, BrowserVertexReads& reuse, uint64_t batch) {
    reuse.batch = batch;
    reuse.streams = 0;
    reuse.unknown = false;
    VertexShader fvf;
    const auto* layout = PickLayout(s, fvf);
    if (!layout) return;
    std::array<uint64_t, 16> tails{};
    uint32_t streams = 0;
    for (const auto& in : layout->inputs) {
        if (in.stream < 0 || !in.type || in.type == kTypeNone) continue;
        if (size_t(in.stream) >= s.streams.size()) {
            reuse.unknown = true;
            return;
        }
        if (!s.streams[in.stream].vb) continue;
        const auto element = HostFormat(in.type);
        const uint64_t tail = uint64_t(in.offset) + (element.bytes ? element.bytes : 16u);
        tails[in.stream] = std::max(tails[in.stream], tail);
        streams |= 1u << in.stream;
    }
    reuse.streams = streams;
    while (streams) {
        const auto stream = std::countr_zero(streams);
        streams &= streams - 1;
        const auto& binding = s.streams[stream];
        const uint32_t stride = binding.stride ? binding.stride : layout->stream_stride[stream];
        const auto* header = ff_browser_memory::TryWindow(binding.vb, 8);
        if (!header) {
            reuse.unknown = true;
            return;
        }
        uint32_t data;
        std::memcpy(&data, header + kResData, sizeof(data));
        // Include the whole raw stride and any converted element that extends beyond it.
        reuse.spans[stream] = {mem::VirtualFromPhysical(data), stride, std::max<uint64_t>(stride, tails[stream])};
    }
}

void RecordBrowserVertexReads(const State& s, uint32_t prim, uint32_t start, uint32_t count,
                              const uint8_t* indices, ff_browser_render::PageFences& fences,
                              uint64_t batch, BrowserVertexReads& reuse) {
    constexpr auto vertex = ff_browser_render::PageFences::Vertex;
    if (!count || prim < 1 || prim > 10) return;
    // Sent state invalidates the cached layout. A new batch also refreshes
    // the copied header addresses because the scratch ring can reuse them.
    if (reuse.batch != batch) CaptureBrowserVertexReads(s, reuse, batch);
    if (reuse.unknown) {
        fences.Unknown(batch, vertex);
        return;
    }
    if (!reuse.streams) return;

    const IndexBounds bounds = indices ? IndexRange(indices, count) : IndexBounds{0, count - 1};
    const uint64_t base = indices ? s.base_vertex : start;
    const uint64_t first = base + bounds.first;
    const uint64_t last = base + bounds.last;
    if (last > UINT32_MAX) {
        fences.Unknown(batch, vertex);
        return;
    }
    uint32_t streams = reuse.streams;
    while (streams) {
        const auto stream = std::countr_zero(streams);
        streams &= streams - 1;
        const auto& span = reuse.spans[stream];
        fences.Record(batch, span.base, first * span.stride, last * span.stride + span.tail, vertex);
    }
}

''' + anchor)
    replace('''    VertexShader fvf_layout;
    const VertexShader* layout = PickLayout(s, fvf_layout);
    out.pretransformed = Pretransformed(s);
    if (!layout) return false;''', '''    out.pretransformed = Pretransformed(s);
    const bool reuse = browser_recorded_vertices && browser_raw_layout.regs != 0;
    if (!reuse && (s.vs_handle & 1) && !s.vs) return false;''')
    replace('''    int regs[16];
    int nregs = CollectRegs(s, *layout, regs, out);''', '''    if (reuse) return AssembleRaw(s, browser_raw_layout, d, out);

    VertexShader fvf_layout;
    const auto* layout = PickLayout(s, fvf_layout);
    int regs[16];
    int nregs = CollectRegs(s, *layout, regs, out);''')
    replace('''    bool raw = kRawVertices && RawReadable(s, inputs);
    if (raw) return AssembleRaw(s, inputs, d, out);''', '''    if (ff_webgl_raw_vertices_enabled() && RawReadable(s, inputs)) {
        const auto projection = ProjectBrowserRawLayout(s, inputs, out.regs);
        if (browser_recorded_vertices) browser_raw_layout = projection;
        return AssembleRaw(s, projection, d, out);
    }''')
    path.write_text(text, encoding='utf-8')
    (runtime/'browser-vertices-manifest.json').write_text(json.dumps({
        'inputSha256':before, 'outputSha256':hashlib.sha256(text.encode()).hexdigest(),
        'admission':'single stream; stride <=252; attribute ends inside stride; checked complete guest spans',
        'lifetime':'raw spans are synchronously copied by the owned browser bridge',
        'fallback':'converted assembler when raw mode/layout is not admitted',
    }, indent=2)+'\n', encoding='utf-8')
