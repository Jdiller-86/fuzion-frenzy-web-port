#!/usr/bin/env python3
"""Copy the pinned SDK runtime and apply browser guest-address adapters.

The output is disposable build material. The SDK checkout remains untouched.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import subprocess
import tempfile
import runpy
from pathlib import Path


def adapt_dispatcher_waits(runtime: Path) -> None:
    helper_path = Path(__file__).resolve().parents[1] / "runtime/browser/sdk/dispatcher-waits.inc"
    helper = helper_path.read_text(encoding="utf-8")
    dispatcher = "std::condition_variable g_cv;\n"
    wait_for = "static uint32_t WaitFor(const std::vector<ObjectRef>& objs,"
    wait = "uint32_t Wait(const std::vector<ObjectRef>& objs, bool wait_all, int64_t timeout, bool alertable)"
    finite = "            g_cv.wait_for(lock, nanoseconds(std::min<int64_t>(deadline - now, 50000000)));"
    infinite = "            g_cv.wait_for(lock, milliseconds(50));"
    release = "        o.owner = 0;\n        SetSignal(o, 1);\n        g_cv.notify_all();"
    single_wait = "uint32_t WaitOne(const ObjectRef& object, int64_t timeout, bool alertable)"
    set_event = "int32_t SetEvent(Object& o) {"
    event_signal = set_event + "\n    std::lock_guard<std::mutex> lock(g_dispatch);\n    int32_t old = o.signal;\n    SetSignal(o, 1);\n    g_cv.notify_all();"
    probe_exports = """\
#ifdef FF_BROWSER_DISPATCHER_PROBE
extern "C" EMSCRIPTEN_KEEPALIVE void ff_browser_set_event_wake_probe(uint32_t mode) {
    event_wake_probe.store(mode <= 2 ? mode : 0, std::memory_order_relaxed);
}

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t ff_browser_event_wake_count(uint32_t index) {
    switch (index) {
    case 0: return event_signals.load(std::memory_order_relaxed);
    case 1: return event_matched.load(std::memory_order_relaxed);
    case 2: return event_unrelated.load(std::memory_order_relaxed);
    default: return 0;
    }
}
#endif

"""
    wrappers = f"""\
{wait} {{
    return WaitObjects(objs, wait_all, timeout, alertable);
}}

{single_wait} {{
    // The caller owns this reference through both suspend points and the wait.
    return WaitObjects({{&object, 1}}, false, timeout, alertable);
}}

{set_event}"""
    kernel_namespace = "namespace kernel {\n"
    declaration = kernel_namespace + "\n" + single_wait + ";\n"
    replacements = {
        "objects.cpp": (
            ("#include <queue>\n", "#include <queue>\n#include <span>\n"
             "#ifdef FF_BROWSER_DISPATCHER_PROBE\n#include <emscripten.h>\n#endif\n"),
            (dispatcher, dispatcher + "\n" + helper),
            (wait_for, "static uint32_t WaitFor(std::span<const ObjectRef> objs,"),
            (wait, "static uint32_t WaitObjects(std::span<const ObjectRef> objs, bool wait_all, "
                   "int64_t timeout, bool alertable)"),
            (set_event, wrappers),
            (event_signal, probe_exports + event_signal.replace("g_cv.notify_all();", "if (ShouldWakeEvent(o)) g_cv.notify_all();")),
            (finite, "            ParkedWait parked(objs);\n" + finite),
            (infinite, "            ParkedWait parked(objs);\n" + infinite),
            (release, release.replace("g_cv.notify_all();", "if (ParkedWait::Includes(o)) g_cv.notify_all();")),
        ),
        "xboxkrnl.cpp": (
            (kernel_namespace, declaration),
            ("Wait({ev}, false, INT64_MAX, false)", "WaitOne(ev, INT64_MAX, false)"),
            ("Wait({w}, false, TimeoutArg(a[4]), a[3] & 0xFF)", "WaitOne(w, TimeoutArg(a[4]), a[3] & 0xFF)"),
            ("Wait({o}, false, TimeoutArg(a[2]), a[1] & 0xFF)", "WaitOne(o, TimeoutArg(a[2]), a[1] & 0xFF)"),
            ("Wait({o}, false, TimeoutArg(a[3]), a[2] & 0xFF)", "WaitOne(o, TimeoutArg(a[3]), a[2] & 0xFF)"),
            ("Wait({o}, false, TimeoutArg(a[4]), a[3] & 0xFF)", "WaitOne(o, TimeoutArg(a[4]), a[3] & 0xFF)"),
        ),
        "iocp.cpp": (
            (kernel_namespace, declaration),
            ("Wait({port}, false, wait_arg, false)", "WaitOne(port, wait_arg, false)"),
        ),
    }
    for filename, changes in replacements.items():
        source = runtime / "kernel" / filename
        text = source.read_text(encoding="utf-8")
        for anchor, replacement in changes:
            if text.count(anchor) != 1:
                raise RuntimeError(f"Browser dispatcher wait source differs: {filename}: {anchor}")
            text = text.replace(anchor, replacement, 1)
        source.write_text(text, encoding="utf-8")


def adapt_nonindexed_converted_assembler(runtime: Path) -> None:
    """Avoid a per-draw index sequence when converted draws are non-indexed."""
    source = runtime / "gpu/vertex.cpp"
    text = source.read_text(encoding="utf-8")
    anchors = (
        "    std::vector<uint32_t> seq(d.count);\n"
        "    for (uint32_t i = 0; i < d.count; i++) seq[i] = DrawVertex(s, d, i);",
        "            uint32_t id = ranged ? d.first_vertex + v : seq[v];",
        "    auto local = [&](uint32_t i) { return ranged ? seq[i] - d.first_vertex : i; };",
    )
    replacements = (
        "    std::vector<uint32_t> seq;\n"
        "    if (d.ip) {\n"
        "        seq.resize(d.count);\n"
        "        for (uint32_t i = 0; i < d.count; i++) seq[i] = DrawVertex(s, d, i);\n"
        "    }",
        "            uint32_t id = ranged ? d.first_vertex + v : (d.ip ? seq[v] : d.start + v);",
        "    auto local = [&](uint32_t i) { return ranged ? (d.ip ? seq[i] : d.start + i) - d.first_vertex : i; };",
    )
    for anchor in anchors:
        if text.count(anchor) != 1:
            raise RuntimeError("Converted vertex assembler optimization anchor differs")
    for anchor, replacement in zip(anchors, replacements):
        text = text.replace(anchor, replacement, 1)
    source.write_text(text, encoding="utf-8")


def adapt_browser_texture_decode(runtime: Path, root: Path) -> None:
    source = runtime / "gpu/texture.cpp"
    text = source.read_text(encoding="utf-8")
    texel = "void Texel(uint32_t fmt, const uint8_t* p, uint8_t* o) {"
    next_function = "uint32_t LinearColorFormat(uint32_t f) {"
    if text.count(texel) != 1 or text.count(next_function) != 1:
        raise RuntimeError("Browser color decoder anchors differ from the pinned SDK")
    text = text.replace(texel, "template<uint32_t KnownFormat = UINT32_MAX>\n" + texel +
                        "\n    if constexpr (KnownFormat != UINT32_MAX) fmt = KnownFormat;", 1)
    helper = (root / "runtime/browser/sdk/texture-rgba.inc").read_text(encoding="utf-8")
    source.write_text(text.replace(next_function, helper + "\n" + next_function, 1), encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    sdk = args.sdk.resolve()
    out = args.output.resolve()
    source = sdk / "runtime"
    root = Path(__file__).resolve().parents[1]
    if not any(out.is_relative_to(directory) and out != directory for directory in (root / '.tools/artifacts', root / 'artifacts')):
        parser.error('Overlay output must be a build subdirectory under .tools/artifacts or artifacts')
    if not (source / "kernel/kernel.h").is_file() or not (source / "kernel/memory.cpp").is_file():
        parser.error(f"not an XboxXBERecomp runtime tree: {source}")
    if source == out or source.is_relative_to(out) or out.is_relative_to(source):
        parser.error('SDK input and disposable overlay must not overlap')
    if out.exists():
        shutil.rmtree(out)
    shutil.copytree(source, out)
    patch_root = root / 'desktop/windows/patches'
    manifest = json.loads((patch_root / 'manifest.json').read_text())
    identities = []
    for record in manifest['files']:
        relative = Path(record['path']).relative_to('runtime')
        content = (out / relative).read_bytes().replace(b'\r\n', b'\n')
        identities.append(hashlib.sha256(content).hexdigest())
        (out / relative).write_bytes(content)
    if identities == [record['beforeSha256'] for record in manifest['files']]:
        # Windows worktree .git pointers cannot be interpreted by Linux Git in
        # WSL. Apply the hash-admitted patch outside repository discovery.
        with tempfile.TemporaryDirectory(prefix='ff-sdk-patch-') as patch_cwd:
            subprocess.run(['git', 'apply', '--unsafe-paths', '-p2', '--directory', str(out), str(patch_root / 'runtime-fixes.patch')], cwd=patch_cwd, check=True)
    elif identities != [record['afterSha256'] for record in manifest['files']]:
        raise RuntimeError('SDK rendering sources differ from the published runtime-patch manifest')
    for record in manifest['files']:
        content = (out / Path(record['path']).relative_to('runtime')).read_bytes().replace(b'\r\n', b'\n')
        if hashlib.sha256(content).hexdigest() != record['afterSha256']:
            raise RuntimeError(f"Rendering patch did not produce its recorded source: {record['path']}")

    # The browser backing has four bounded 64 MiB windows. Keep the SDK's
    # allocators inside the user/system/stack windows represented by it.
    mh = out / "kernel/memory.h"
    text = mh.read_text(encoding="utf-8")
    text = text.replace('kUserHigh = 0x7FFE0000', 'kUserHigh = 0x04000000')
    text = text.replace('kSystemSize = 0x10000000', 'kSystemSize = 0x04000000')
    text = text.replace('kStackSize = 0x10000000', 'kStackSize = 0x04000000')
    mh.write_text(text, encoding="utf-8")

    # Kernel services use helpers rather than host pointer arithmetic. The
    # checked span size is supplied by typed scalar helpers and explicit spans.
    kh = out / "kernel/kernel.h"
    text = kh.read_text(encoding="utf-8")
    text = text.replace('#include "kernel/memory.h"', '#include "kernel/memory.h"\n#include "ff_sdk_guest_memory.hpp"')
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, sizeof(T))', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, 4)', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, 4)', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, 2)', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, 2)', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, 8)', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va, 8)', 1)
    text = text.replace('g_base + va', 'ff_sdk_guest_ptr(va)', 1)
    kh.write_text(text, encoding="utf-8")

    # Generated CPU helpers use these lvalues for every scalar and REP span.
    ops = out / "cpu/x86_ops.h"
    text = ops.read_text(encoding="utf-8")
    # A generated guest jump must not accumulate a second host frame in this
    # noinline weak wrapper. Title objects compile with -mtail-call.
    text = text.replace('{ impl(ctx, base); }', '{ [[clang::musttail]] return impl(ctx, base); }')
    text = text.replace('#include "x86_context.h"', '#include "x86_context.h"\n#include "ff_sdk_guest_memory.hpp"')
    # Two ordered comparisons recover the same C0, C2 and C3 bits, including
    # unordered inputs. The translated x87-heavy paths read these bits often.
    x87_compare = '''    uint16_t c;
    if (std::isnan(a) || std::isnan(b)) c = 0x4500;
    else if (a > b) c = 0;
    else if (a < b) c = 0x0100;
    else c = 0x4000;
    ctx.fsw = uint16_t((ctx.fsw & ~0x4500) | c);'''
    if text.count(x87_compare) != 1:
        raise RuntimeError('Browser x87 compare anchor differs from the pinned SDK')
    text = text.replace(x87_compare, '''    const bool at_most = a <= b;
    const bool at_least = a >= b;
    const uint16_t c = (at_least ? 0 : 0x0100) |
                       ((at_most || at_least) ? 0 : 0x0400) |
                       ((at_most == at_least) ? 0x4000 : 0);
    ctx.fsw = uint16_t((ctx.fsw & ~0x4500) | c);''', 1)
    for macro, width in (("X86_MEM8", 1), ("X86_MEM16", 2), ("X86_MEM32", 4),
                         ("X86_MEM64", 8), ("X86_MEMF32", 4), ("X86_MEMF64", 8)):
        import re
        pattern = rf"#define {macro}\(a\).*"
        cast = {"X86_MEM8":"uint8_t", "X86_MEM16":"x86_u16u", "X86_MEM32":"x86_u32u",
                "X86_MEM64":"x86_u64u", "X86_MEMF32":"x86_f32u", "X86_MEMF64":"x86_f64u"}[macro]
        pointer = f"ff_sdk_guest_scalar_ptr<{width}>(uint32_t(a))"
        text = re.sub(pattern, f"#define {macro}(a) (*reinterpret_cast<{cast}*>({pointer}))", text, count=1)
    text = text.replace('#define X86_PTR(T, a) ((T*)(base + (uint32_t)(a)))',
                        '#define X86_PTR(T, a) (reinterpret_cast<T*>(ff_sdk_guest_ptr(uint32_t(a), std::is_same_v<T, uint8_t> ? 10 : sizeof(T))))')
    text = text.replace('std::memmove(base + ctx.edi, base + ctx.esi, bytes)',
                        'std::memmove(ff_sdk_guest_ptr(ctx.edi, bytes), ff_sdk_guest_ptr(ctx.esi, bytes), bytes)')
    text = text.replace('std::memcpy(base + ctx.edi, base + ctx.esi, w)',
                        'std::memcpy(ff_sdk_guest_ptr(ctx.edi, w), ff_sdk_guest_ptr(ctx.esi, w), w)')
    text = text.replace('std::memset(base + ctx.edi, uint8_t(ctx.eax), n)',
                        'std::memset(ff_sdk_guest_ptr(ctx.edi, n), uint8_t(ctx.eax), n)')
    text = text.replace('std::memcpy(base + ctx.edi, &v, w)',
                        'std::memcpy(ff_sdk_guest_ptr(ctx.edi, w), &v, w)')
    ops.write_text(text, encoding="utf-8")

    dispatch = out / "cpu/dispatch.cpp"
    text = dispatch.read_text(encoding="utf-8").replace('#include <x86intrin.h>\n', '').replace('#include <intrin.h>\n', '')
    text = text.replace('(base + ctx.ecx)', 'ff_sdk_guest_ptr(ctx.ecx, 2)', 1)
    text = text.replace('(base + ctx.ecx + 4)', 'ff_sdk_guest_ptr(ctx.ecx + 4, 4)', 1)
    text = text.replace('base[buf + i]', 'ff_sdk_guest_ptr(buf + i)[0]')
    dispatch.write_text(text, encoding="utf-8")

    boot = out / "kernel/boot.cpp"
    text = boot.read_text(encoding="utf-8")
    text = text.replace('g_base + img.base', 'ff_sdk_guest_ptr(img.base, std::min<size_t>(img.header_size, img.file.size()))')
    text = text.replace('g_base + s.va', 'ff_sdk_guest_ptr(s.va, std::min(s.rsize, s.vsize))')
    relaunch = text.index('void Relaunch(const char* exe) {')
    body = relaunch + len('void Relaunch(const char* exe) {')
    text = text[:body] + '\n#ifdef __EMSCRIPTEN__\n    (void)exe;\n    LOGE("relaunch/handover is unsupported in browser runtime");\n    std::abort();\n#else\n' + text[body:]
    shutdown = text.index('\nvoid Shutdown(int code)', body)
    close = text.rfind('\n}', body, shutdown)
    text = text[:close] + '\n#endif  // __EMSCRIPTEN__' + text[close:]
    boot.write_text(text, encoding="utf-8")
    winsock = out / 'xdk/winsock_compat.h'
    winsock.write_text(winsock.read_text(encoding='utf-8').replace('#include <sys/socket.h>', '#include <sys/socket.h>\n#include <sys/time.h>'), encoding='utf-8')
    adapter_header = out / "ff_sdk_guest_memory.hpp"
    adapter_header.write_text('#pragma once\n#include "full-memory.h"\n', encoding="utf-8")

    adapt_dispatcher_waits(out)
    adapt_nonindexed_converted_assembler(out)
    runpy.run_path(str(root / 'tools/prepare-sdk-browser-vertices.py'))['adapt_browser_vertices'](out)
    runpy.run_path(str(root / 'tools/prepare-sdk-browser-pacing.py'))['adapt_browser_pacing'](out)

    threading = runpy.run_path(str(root / 'tools/prepare-sdk-browser-threading.py'))['adapt_browser_threading'](out)
    adapt_browser_texture_decode(out, root)
    d3d = out / 'xdk/d3d8.cpp'
    text = d3d.read_text(encoding='utf-8')
    anchor = 'if (kRenderThread) g_backend = gpu::CreateThreadedBackend(std::move(g_backend));'
    if text.count(anchor)!=1: raise RuntimeError('Browser render-thread selection anchor differs')
    text = 'bool ff_webgl_threaded_enabled();\n' + text.replace(anchor,'if (ff_webgl_threaded_enabled()) g_backend = gpu::CreateThreadedBackend(std::move(g_backend));')
    sync = 'void SyncDeferred() {'
    next_function = '\nfloat ZScale('
    if text.count(sync) != 1 or text.count(next_function) != 1:
        raise RuntimeError('Browser deferred-state refresh anchors differ')
    start = text.index(sync)
    end = text.index(next_function, start)
    expected = 'a4915944ea5e39311ba11bcfe47e1c226a9e9215592c331fe44ab1292bdd639e'
    if hashlib.sha256(text[start:end].encode('utf-8')).hexdigest() != expected:
        raise RuntimeError('Browser deferred-state refresh source differs')
    helper = root / 'runtime/browser/sdk/guest-deferred-state.inc'
    digest = hashlib.sha256(helper.read_bytes()).hexdigest()
    text = ('#include <utility>\n#include <wasm_simd128.h>\n#include "full-memory.h"\n'
            + text[:start] + f'// Deferred state helper SHA-256: {digest}\n'
            + '#include "guest-deferred-state.inc"\n' + text[end:])
    d3d.write_text(text,encoding='utf-8')
    (out/'browser-threading-manifest.json').write_text(json.dumps(threading,indent=2)+'\n',encoding='utf-8')

    # Emit an auditable record of remaining direct guest arithmetic; the full
    # adapter must handle each of these before it is linked into the module.
    remaining = []
    for path in sorted(out.rglob("*.cpp")):
        for number, line in enumerate(path.read_text(encoding="utf-8", errors="replace").splitlines(), 1):
            if "g_base +" in line or "Base() +" in line:
                remaining.append(f"{path.relative_to(out)}:{number}:{line.strip()}")
    (out / "BROWSER-OVERLAY-REMAINING-ARITHMETIC.txt").write_text(
        "\n".join(remaining) + ("\n" if remaining else ""), encoding="utf-8")
    print(f"copied runtime to {out}; remaining direct guest arithmetic: {len(remaining)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
