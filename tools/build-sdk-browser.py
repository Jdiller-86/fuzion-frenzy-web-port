#!/usr/bin/env python3
"""Build a browser-callable bridge for retained SDK-generated title C++.

It compiles all generated shards and links a bounded entry adapter with an
initial subset of pinned SDK kernel services. This is not a complete SDK
runtime or title execution. Its generated checked header and browser CPU shim
adapt pinned SDK code covered by runtime/browser/sdk/SDK-NOTICE.txt.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time


ROOT = Path(__file__).resolve().parents[1]
SDK_REL = Path(".tools/XboxXBERecomp")
SDK_COMMIT = "58757ab255c0a6b3929f405d7a5f1567fcd7fda3"
GENERATED_REL = Path("research/verification/sdk-title-bulk-command-20261107/generated")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="Emscripten em++ path (defaults to .tools/emsdk or PATH)")
    parser.add_argument("--sdk", type=Path, default=ROOT / SDK_REL)
    parser.add_argument("--generated", type=Path, default=ROOT / GENERATED_REL)
    parser.add_argument("--output", type=Path, default=ROOT / ".tools/artifacts/sdk-browser-objects")
    parser.add_argument("--jobs", type=int, default=max(1, min(os.cpu_count() or 1, 2)))
    parser.add_argument("--opt", default="-O0", choices=("-O0", "-O1", "-O2", "-O3"))
    parser.add_argument("--compile-only", action="store_true", help="emit relocatable objects without linking the browser module")
    parser.add_argument("--reuse-objects", action="store_true", help="link existing wasm objects in --output without recompiling")
    args = parser.parse_args()

    sdk = args.sdk.resolve()
    generated = args.generated.resolve()
    output = args.output.resolve()
    emsdk_compiler = ROOT / ".tools/emsdk/upstream/emscripten/em++"
    compiler = args.compiler or (str(emsdk_compiler) if emsdk_compiler.is_file() else shutil.which("em++"))
    if not compiler:
        print("missing dependency: em++; install pinned Emscripten 3.1.50 under .tools/emsdk or pass --compiler", file=sys.stderr)
        return 2
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    try:
        sdk_commit = subprocess.check_output(["git", "-C", str(sdk), "rev-parse", "HEAD"], text=True).strip()
    except (OSError, subprocess.CalledProcessError) as error:
        print(f"cannot identify SDK checkout at {sdk}: {error}", file=sys.stderr)
        return 2
    if sdk_commit != SDK_COMMIT:
        print(f"SDK revision mismatch: expected {SDK_COMMIT}, got {sdk_commit}", file=sys.stderr)
        return 2

    shards = sorted(generated.glob("x86_recomp.[0-9]*.cpp"), key=lambda p: int(p.stem.rsplit(".", 1)[1]))
    mapping = generated / "x86_func_mapping.cpp"
    if len(shards) != 64 or not mapping.is_file() or not (generated / "x86_recomp_shared.h").is_file():
        print(f"expected the retained 64-shard generated package and mapping under {generated}", file=sys.stderr)
        return 2
    output.mkdir(parents=True, exist_ok=True)
    runtime = sdk / "runtime"
    overlay = output / "sdk-overlay"
    (overlay / "cpu").mkdir(parents=True, exist_ok=True)
    (overlay / "ff_sdk_guest_memory.hpp").write_text(guest_memory_header(), encoding="utf-8")
    overlay_header = overlay / "cpu/x86_ops.h"
    overlay_header.write_text(checked_x86_ops(runtime / "cpu/x86_ops.h"), encoding="utf-8")
    sources = [*shards, mapping]
    version = subprocess.check_output([compiler, "--version"], text=True).splitlines()[0]
    # Generated SDK functions use C++ exceptions to model guest catch/rethrow
    # continuations, so exception support must stay enabled in browser builds.
    common = [compiler, "-std=c++20", args.opt, "-fexceptions", "-fno-rtti", "-c",
              "-I", str(overlay), "-I", str(runtime), "-I", str(runtime / "cpu")]
    start = time.monotonic()
    source_identities = [{"path": p.name, "sha256": sha256(p)} for p in sources]
    guest_memory_header_path = overlay / "ff_sdk_guest_memory.hpp"
    overlay_identity = {
        "source": "pinned SDK x86_ops.h plus checked guest-memory lvalues/REP spans",
        "path": str(overlay_header.relative_to(ROOT)),
        "sha256": sha256(overlay_header),
        "guestMemoryHeader": {"path": str(guest_memory_header_path.relative_to(ROOT)), "sha256": sha256(guest_memory_header_path)},
    }

    if args.reuse_objects:
        prior_path = output / "build-manifest.json"
        try:
            prior = json.loads(prior_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as error:
            print(f"cannot safely reuse objects without a readable prior manifest: {error}", file=sys.stderr)
            return 2
        if (prior.get("sdkCommit") != sdk_commit or prior.get("compiler") != version or
                prior.get("opt") != args.opt or prior.get("generatedSources") != source_identities or
                prior.get("checkedOverlay") != overlay_identity):
            print("refusing stale object reuse: SDK, compiler, optimization, generated-source, or checked-overlay identity differs", file=sys.stderr)
            return 2
        prior_objects = {item.get("source"): item for item in prior.get("objects", [])}
        for source in sources:
            item = prior_objects.get(source.name)
            object_path = output / (source.stem + ".o")
            if not item or not object_path.is_file() or item.get("sha256") != sha256(object_path):
                print(f"refusing stale or unrecorded wasm object reuse: {object_path}", file=sys.stderr)
                return 2

    # Each shard is independent. Bound workers so parallel C++ compilation does
    # not consume the entire shared environment.
    from concurrent.futures import ThreadPoolExecutor, as_completed

    def compile_one(source: Path) -> dict[str, object]:
        object_path = output / (source.stem + ".o")
        command = [*common, str(source), "-o", str(object_path)]
        result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if result.returncode:
            return {"source": source.name, "returncode": result.returncode, "output": result.stdout[-12000:]}
        with object_path.open("rb") as object_file:
            magic = object_file.read(4)
        if magic != b"\0asm":
            return {"source": source.name, "returncode": 1, "output": f"expected wasm32 object magic, got {magic!r}"}
        return {"source": source.name, "object": object_path.name, "bytes": object_path.stat().st_size,
                "sha256": sha256(object_path), "status": "compiled-wasm32"}

    results: list[dict[str, object]] = []
    if args.reuse_objects:
        for source in sources:
            object_path = output / (source.stem + ".o")
            if not object_path.is_file():
                print(f"missing wasm object for --reuse-objects: {object_path}", file=sys.stderr)
                return 2
            with object_path.open("rb") as object_file:
                if object_file.read(4) != b"\0asm":
                    print(f"not a wasm32 object: {object_path}", file=sys.stderr)
                    return 2
            results.append({"source": source.name, "object": object_path.name, "bytes": object_path.stat().st_size,
                            "sha256": sha256(object_path), "status": "reused-wasm32"})
    else:
        with ThreadPoolExecutor(max_workers=args.jobs) as pool:
            futures = [pool.submit(compile_one, source) for source in sources]
            for future in as_completed(futures):
                item = future.result()
                results.append(item)
                if item.get("returncode", 0):
                    for pending in futures:
                        pending.cancel()
                    print(json.dumps(item, indent=2), file=sys.stderr)
                    return int(item["returncode"])
                print(f"{item['status']}: {item['source']} -> {item['object']} ({item['bytes']} bytes)", flush=True)

    manifest = {
        "scope": "actual generated guest objects and checked browser call bridge; not title execution",
        "sdkCommit": sdk_commit,
        "compiler": version,
        "target": "wasm32-unknown-emscripten",
        "generatedPackage": str(generated.relative_to(ROOT)) if generated.is_relative_to(ROOT) else "external-local-package",
        "generatedSources": source_identities,
        "checkedOverlay": overlay_identity,
        "opt": args.opt,
        "objects": sorted(results, key=lambda item: str(item["source"])),
        "elapsedSeconds": round(time.monotonic() - start, 3),
        "unimplementedRuntimeBoundary": ["remaining kernel/XDK providers stop with exact ordinal", "full image initialization and title data admission"],
    }
    adapter_source = output / "browser_adapter.cpp"
    adapter_source.write_text(browser_adapter_source(), encoding="utf-8")
    manifest["adapterSource"] = {"path": adapter_source.name, "sha256": sha256(adapter_source)}
    if not args.compile_only:
        exports = ["_malloc", "_free", "_ff_sdk_call_guest", "_ff_sdk_image_base", "_ff_sdk_image_end",
                   "_ff_sdk_entrypoint", "_ff_sdk_kernel_thunk", "_ff_sdk_guest_capacity",
                   "_ff_sdk_last_stop_code", "_ff_sdk_last_stop_address", "_ff_sdk_last_stop_ordinal"]
        runtime_sources = [ROOT / "runtime/browser/sdk/kernel-services.cpp", runtime / "kernel/crypto.cpp"]
        for runtime_source in runtime_sources:
            if not runtime_source.is_file():
                print(f"missing pinned browser runtime source: {runtime_source}", file=sys.stderr)
                return 2
            result = compile_one(runtime_source)
            if result.get("returncode", 0):
                print(json.dumps(result, indent=2), file=sys.stderr)
                return int(result["returncode"])
            print(f"{result['status']}: {result['source']} -> {result['object']} ({result['bytes']} bytes)", flush=True)
            results.append(result)
        manifest["runtimeSources"] = [{"path": str(p.relative_to(ROOT)), "sha256": sha256(p)} for p in runtime_sources]
        manifest["objects"] = sorted(results, key=lambda item: str(item["source"]))
        manifest["elapsedSeconds"] = round(time.monotonic() - start, 3)
        manifest_path = output / "build-manifest.json"
        manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
        link_command = [
            compiler, "-std=c++20", args.opt, "-fexceptions", "-fno-rtti",
            "-I", str(overlay), "-I", str(runtime), "-I", str(runtime / "cpu"),
            str(adapter_source), *(str(output / (source.stem + ".o")) for source in sources),
            *(str(output / (source.stem + ".o")) for source in runtime_sources),
            "-lworkerfs.js",
            "--no-entry", "-sMODULARIZE=1", "-sEXPORT_ES6=1", "-sEXPORT_NAME=createFuzionSdkRuntime",
            "-sENVIRONMENT=web,worker", "-sALLOW_MEMORY_GROWTH=1", "-sINITIAL_MEMORY=268435456",
            "-sMAXIMUM_MEMORY=1879048192", "-sSTACK_SIZE=8388608", "-sNO_EXIT_RUNTIME=1",
            "-sFORCE_FILESYSTEM=1",
            "-sEXPORTED_FUNCTIONS=" + json.dumps(exports),
            "-sEXPORTED_RUNTIME_METHODS=" + json.dumps(["HEAPU8", "FS", "WORKERFS"]),
            "-o", str(output / "fuzion-sdk-title.mjs"),
        ]
        linked = subprocess.run(link_command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if linked.returncode:
            print(linked.stdout[-16000:], file=sys.stderr)
            return linked.returncode
        wasm_path = output / "fuzion-sdk-title.wasm"
        manifest["browserModule"] = {"js": "fuzion-sdk-title.mjs", "wasm": "fuzion-sdk-title.wasm",
                                     "jsSha256": sha256(output / "fuzion-sdk-title.mjs"),
                                     "wasmSha256": sha256(wasm_path), "wasmBytes": wasm_path.stat().st_size,
                                     "abi": "ff_sdk_call_guest(basePtr, stackTopVa, guestVa); 64 MiB checked guest window; SDK kernel services stop explicitly",
                                     "filesystem": "MEMFS and WorkerFS exports; 256 MiB initial linear memory; 1.75 GiB maximum",
                                     "stopCodes": {"0": "guest call returned", "1": "unimplemented kernel ordinal", "2": "guest trap", "3": "unmapped indirect call", "4": "unhandled continuation", "5": "checked guest-memory bounds", "6": "unsupported DebugService", "7": "unexpected exception"}}
        print(f"linked browser module: {output / 'fuzion-sdk-title.mjs'} ({wasm_path.stat().st_size} wasm bytes)")
    manifest_path = output / "build-manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"prepared {len(results)} WebAssembly objects in {manifest['elapsedSeconds']}s; manifest: {manifest_path}")
    print("status: module linked; no title boot, original frame, or runtime behavior is claimed")
    return 0


def guest_memory_header() -> str:
    return r'''#pragma once
#include <cstdint>
#include <cstring>
#include <type_traits>
#include "cpu/x86_context.h"
inline constexpr uint32_t FFSDK_GUEST_CAPACITY = 64u * 1024u * 1024u;
struct FFSDKGuestMemoryFault { uint32_t address; uint32_t bytes; };
inline uint8_t* ff_sdk_checked_guest_span(X86Context&, uint8_t* base, uint32_t address, uint64_t bytes) {
    if (address > FFSDK_GUEST_CAPACITY || bytes > uint64_t(FFSDK_GUEST_CAPACITY) - address)
        throw FFSDKGuestMemoryFault{address, bytes > 0xFFFFFFFFu ? 0xFFFFFFFFu : uint32_t(bytes)};
    return base + address;
}
template<class T> class FFSDKGuestRef {
    static_assert(std::is_trivially_copyable<T>::value, "scalar guest reference only");
    X86Context* ctx_; uint8_t* base_; uint32_t address_;
public:
    FFSDKGuestRef(X86Context& ctx, uint8_t* base, uint32_t address) : ctx_(&ctx), base_(base), address_(address) {}
    operator T() const { T value; std::memcpy(&value, ff_sdk_checked_guest_span(*ctx_, base_, address_, sizeof(T)), sizeof(T)); return value; }
    FFSDKGuestRef& operator=(T value) { std::memcpy(ff_sdk_checked_guest_span(*ctx_, base_, address_, sizeof(T)), &value, sizeof(T)); return *this; }
    FFSDKGuestRef& operator=(const FFSDKGuestRef& other) { return *this = static_cast<T>(other); }
};
template<class T> T* ff_sdk_checked_guest_pointer(X86Context& ctx, uint8_t* base, uint32_t address) {
    constexpr uint32_t span = std::is_same<T, uint8_t>::value ? 10u : uint32_t(sizeof(T));
    return reinterpret_cast<T*>(ff_sdk_checked_guest_span(ctx, base, address, span));
}
'''


def checked_x86_ops(upstream: Path) -> str:
    source = upstream.read_text(encoding="utf-8")
    source = source.replace('#include "x86_context.h"', '#include "x86_context.h"\n#include "ff_sdk_guest_memory.hpp"', 1)
    replacements = {
        '#define X86_MEM8(a) (*(uint8_t*)(base + (uint32_t)(a)))': '#define X86_MEM8(a) ::FFSDKGuestRef<uint8_t>(ctx, base, (uint32_t)(a))',
        '#define X86_MEM16(a) (*(x86_u16u*)(base + (uint32_t)(a)))': '#define X86_MEM16(a) ::FFSDKGuestRef<x86_u16u>(ctx, base, (uint32_t)(a))',
        '#define X86_MEM32(a) (*(x86_u32u*)(base + (uint32_t)(a)))': '#define X86_MEM32(a) ::FFSDKGuestRef<x86_u32u>(ctx, base, (uint32_t)(a))',
        '#define X86_MEM64(a) (*(x86_u64u*)(base + (uint32_t)(a)))': '#define X86_MEM64(a) ::FFSDKGuestRef<x86_u64u>(ctx, base, (uint32_t)(a))',
        '#define X86_MEMF32(a) (*(x86_f32u*)(base + (uint32_t)(a)))': '#define X86_MEMF32(a) ::FFSDKGuestRef<x86_f32u>(ctx, base, (uint32_t)(a))',
        '#define X86_MEMF64(a) (*(x86_f64u*)(base + (uint32_t)(a)))': '#define X86_MEMF64(a) ::FFSDKGuestRef<x86_f64u>(ctx, base, (uint32_t)(a))',
        '#define X86_PTR(T, a) ((T*)(base + (uint32_t)(a)))': '#define X86_PTR(T, a) ::ff_sdk_checked_guest_pointer<T>(ctx, base, (uint32_t)(a))',
    }
    for old, new in replacements.items():
        if source.count(old) != 1:
            raise RuntimeError(f"pinned SDK x86_ops.h changed expected memory macro: {old}")
        source = source.replace(old, new)
    movs = r'''// Guest element memory is bounded before any span is accessed. Descending copies keep the SDK element order.
inline void x86_rep_movs(X86Context& ctx, uint8_t* base, uint32_t w) {
    const uint32_t n = ctx.ecx;
    if (!n) return;
    if (!w || uint64_t(n) * w > FFSDK_GUEST_CAPACITY) throw FFSDKGuestMemoryFault{ctx.esi, 0xFFFFFFFFu};
    const uint64_t bytes = uint64_t(n) * w;
    const uint64_t back = bytes - w;
    const uint64_t src = ctx.df ? (uint64_t(ctx.esi) < back ? uint64_t(FFSDK_GUEST_CAPACITY) + 1 : uint64_t(ctx.esi) - back) : ctx.esi;
    const uint64_t dst = ctx.df ? (uint64_t(ctx.edi) < back ? uint64_t(FFSDK_GUEST_CAPACITY) + 1 : uint64_t(ctx.edi) - back) : ctx.edi;
    ff_sdk_checked_guest_span(ctx, base, uint32_t(src), bytes);
    ff_sdk_checked_guest_span(ctx, base, uint32_t(dst), bytes);
    if (!ctx.df) std::memmove(base + ctx.edi, base + ctx.esi, size_t(bytes));
    else for (uint32_t i = 0; i < n; ++i) std::memmove(base + (ctx.edi - i * w), base + (ctx.esi - i * w), w);
    const uint32_t delta = uint32_t(bytes);
    ctx.esi += ctx.df ? 0u - delta : delta;
    ctx.edi += ctx.df ? 0u - delta : delta;
    ctx.ecx = 0;
}

inline void x86_rep_stos(X86Context& ctx, uint8_t* base, uint32_t w) {
    const uint32_t n = ctx.ecx;
    if (!n) return;
    if (!w || uint64_t(n) * w > FFSDK_GUEST_CAPACITY) throw FFSDKGuestMemoryFault{ctx.edi, 0xFFFFFFFFu};
    const uint64_t bytes = uint64_t(n) * w;
    const uint64_t back = bytes - w;
    const uint64_t dst = ctx.df ? (uint64_t(ctx.edi) < back ? uint64_t(FFSDK_GUEST_CAPACITY) + 1 : uint64_t(ctx.edi) - back) : ctx.edi;
    ff_sdk_checked_guest_span(ctx, base, uint32_t(dst), bytes);
    const uint32_t value = ctx.eax;
    if (!ctx.df && w == 1) std::memset(base + ctx.edi, uint8_t(value), size_t(n));
    else for (uint32_t i = 0; i < n; ++i) std::memcpy(base + (ctx.df ? ctx.edi - i * w : ctx.edi + i * w), &value, w);
    const uint32_t delta = uint32_t(bytes);
    ctx.edi += ctx.df ? 0u - delta : delta;
    ctx.ecx = 0;
}
'''
    start = source.index("// rep movs:")
    source = source[:start] + movs
    return source


def browser_adapter_source() -> str:
    return r'''#include <cstdint>
#include <cstring>
#include <emscripten/emscripten.h>
#include <emscripten/console.h>
#include <emscripten/heap.h>
#include "cpu/x86_context.h"
#include "cpu/x86_ops.h"

namespace {
struct FFSDKStop { int code; uint32_t address; uint32_t ordinal; };
constexpr uint32_t kKernelThunkBase = 0xFE000000u;
constexpr uint32_t kKernelThunkBytes = 400u * 4u;
constexpr uint32_t kGuestCapacity = 64u * 1024u * 1024u;
X86Context g_context{};
uint8_t* g_guest_base = nullptr;
int g_stop_code = 0;
uint32_t g_stop_address = 0, g_stop_ordinal = 0;
X86Func* FindGuest(uint32_t guest) {
    for (const X86FuncMapping* entry = X86FuncMappings; entry->host; ++entry)
        if (entry->guest == guest) return entry->host;
    return nullptr;
}
bool IsContinuation(uint32_t guest) {
    for (const uint32_t* item = X86Continuations; *item; ++item) if (*item == guest) return true;
    return false;
}
[[noreturn]] void Stop(int code, uint32_t address, uint32_t ordinal) { throw FFSDKStop{code, address, ordinal}; }
}

extern "C" __attribute__((weak)) bool ff_sdk_browser_kernel_dispatch(X86Context&, uint8_t*, uint32_t) { return false; }

void x86_icall(X86Context& ctx, uint8_t* base, uint32_t target) {
    ctx.last_icall = target;
    if (IsContinuation(target)) throw X86Resume{target, ctx.esp};
    if (X86Func* function = FindGuest(target)) { function(ctx, base); return; }
    if (target >= kKernelThunkBase && target < kKernelThunkBase + kKernelThunkBytes) {
        x86_kernel(ctx, base, (target - kKernelThunkBase) / 4u);
        return;
    }
    Stop(3, target, 0);
}

void x86_resume(X86Context& ctx, uint8_t* base, const X86Resume& resume) {
    ctx.esp = resume.esp;
    if (X86Func* function = FindGuest(resume.target)) { function(ctx, base); return; }
    Stop(4, resume.target, 0);
}

void x86_kernel(X86Context& ctx, uint8_t* base, uint32_t ordinal) {
    if (ff_sdk_browser_kernel_dispatch(ctx, base, ordinal)) return;
    Stop(1, ctx.esp, ordinal);
}

[[noreturn]] void x86_trap(X86Context&, uint32_t va, const char*) { Stop(2, va, 0); }
void x86_priv(X86Context&, uint32_t, const char*) {}
void x86_debug_service(X86Context& ctx, uint8_t* base, uint32_t va) {
    if (ctx.eax != 1) Stop(6, va, ctx.eax);
    auto* descriptor = ff_sdk_checked_guest_span(ctx, base, ctx.ecx, 8);
    uint16_t length; uint32_t text_va;
    std::memcpy(&length, descriptor, 2); std::memcpy(&text_va, descriptor + 4, 4);
    const uint32_t count = length > 1024 ? 1024 : length;
    const auto* text = ff_sdk_checked_guest_span(ctx, base, text_va, count);
    char line[1025] = {};
    uint32_t used = 0;
    for (; used < count && text[used]; ++used) line[used] = text[used] == '\n' || text[used] == '\r' ? ' ' : char(text[used]);
    while (used && line[used - 1] == ' ') --used;
    line[used] = 0;
    if (used) emscripten_console_log(line);
}
uint32_t x86_trace(X86Context& ctx, uint32_t, uint32_t ret) {
    if (ret) return ret;
    auto* stack = ff_sdk_checked_guest_span(ctx, g_guest_base, ctx.esp, 4);
    uint32_t return_address; std::memcpy(&return_address, stack, 4); return return_address;
}
uint64_t x86_rdtsc() { static double start = emscripten_get_now(); return uint64_t((emscripten_get_now() - start) * 733333.333); }
void x86_cpuid(X86Context& ctx) {
    switch (ctx.eax) {
        case 0: ctx.eax = 2; ctx.ebx = 0x756E6547; ctx.edx = 0x49656E69; ctx.ecx = 0x6C65746E; break;
        case 1: ctx.eax = 0x0000068A; ctx.ebx = 0; ctx.ecx = 0; ctx.edx = 0x0383F9FF; break;
        default: ctx.eax = ctx.ebx = ctx.ecx = ctx.edx = 0; break;
    }
}

extern "C" {
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_image_base() { return X86ImageBase; }
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_image_end() { return X86ImageEnd; }
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_entrypoint() { return X86EntryPoint; }
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_kernel_thunk() { return X86KernelThunk; }
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_guest_capacity() { return kGuestCapacity; }
EMSCRIPTEN_KEEPALIVE int ff_sdk_last_stop_code() { return g_stop_code; }
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_last_stop_address() { return g_stop_address; }
EMSCRIPTEN_KEEPALIVE uint32_t ff_sdk_last_stop_ordinal() { return g_stop_ordinal; }
EMSCRIPTEN_KEEPALIVE int ff_sdk_call_guest(uint32_t guest_base_ptr, uint32_t stack_top_va, uint32_t guest_va) {
    g_stop_code = 0; g_stop_address = 0; g_stop_ordinal = 0;
    const size_t heap_size = emscripten_get_heap_size();
    if (!guest_base_ptr || guest_base_ptr > heap_size || kGuestCapacity > heap_size - guest_base_ptr || stack_top_va >= kGuestCapacity) {
        g_stop_code = 5; g_stop_address = stack_top_va; return g_stop_code;
    }
    std::memset(&g_context, 0, sizeof(g_context));
    g_context.esp = stack_top_va;
    g_context.fcw = 0x027F;
    g_context.mxcsr = 0x1F80;
    uint8_t* base = reinterpret_cast<uint8_t*>(uintptr_t(guest_base_ptr));
    g_guest_base = base;
    try {
        if (X86Func* function = FindGuest(guest_va)) { function(g_context, base); return 0; }
        Stop(3, guest_va, 0);
    } catch (const FFSDKGuestMemoryFault& fault) {
        g_stop_code = 5; g_stop_address = fault.address; g_stop_ordinal = fault.bytes;
    } catch (const FFSDKStop& stop) {
        g_stop_code = stop.code; g_stop_address = stop.address; g_stop_ordinal = stop.ordinal;
    } catch (const X86Resume& resume) {
        g_stop_code = 4; g_stop_address = resume.target; g_stop_ordinal = resume.esp;
    } catch (...) {
        g_stop_code = 7;
    }
    return g_stop_code;
}
}
'''


if __name__ == "__main__":
    raise SystemExit(main())
