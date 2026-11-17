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

    replace('namespace gpu {', '#include <cstdlib>\nbool ff_webgl_raw_vertices_enabled();\n\nnamespace gpu {')
    replace('bool raw = kRawVertices && RawReadable(s, inputs);',
            'bool raw = ff_webgl_raw_vertices_enabled() && RawReadable(s, inputs);')
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
    replace('''    const uint8_t* data = static_cast<const uint8_t*>(HostPtr(mem::VirtualFromPhysical(R32(st.vb + kResData))));
    int slot = int(out.buffer_count);''', '''    const uint32_t address = mem::VirtualFromPhysical(R32(st.vb + kResData));
    const uint64_t end = (uint64_t(d.first_vertex) + d.span) * stride;
    const uint64_t captured = uint64_t(dense ? d.span : d.count) * stride;
    // Check the complete source range before either the dense view or sparse
    // gather can read it; pointer arithmetic cannot wrap to another guest bank.
    if (!end || end > 128u * 1024 * 1024 || captured > 128u * 1024 * 1024) std::abort();
    const uint8_t* data = ff_sdk_guest_ptr(address, size_t(end));
    int slot = int(out.buffer_count);''')
    replace('''bool AssembleRaw(const State& s, const Inputs& inputs, const DrawRange& d, Assembled& out) {
    bool dense''', '''bool AssembleRaw(const State& s, const Inputs& inputs, const DrawRange& d, Assembled& out) {
    if (d.count > 1024u * 1024 || !d.span) std::abort();
    bool dense''')
    replace('    if (NativePrimitive(d.prim)) return DrawNative(d, dense, out);',
            '    // Use the same ordered list expansion as the converted reference.\n'
            '    // WebGL has no base-vertex draw: the list below rebases every index.')
    path.write_text(text, encoding='utf-8')
    (runtime/'browser-vertices-manifest.json').write_text(json.dumps({
        'inputSha256':before, 'outputSha256':hashlib.sha256(text.encode()).hexdigest(),
        'admission':'single stream; stride <=252; attribute ends inside stride; checked complete guest spans',
        'lifetime':'raw spans are synchronously copied by the owned browser bridge',
        'fallback':'converted assembler when raw mode/layout is not admitted',
    }, indent=2)+'\n', encoding='utf-8')
