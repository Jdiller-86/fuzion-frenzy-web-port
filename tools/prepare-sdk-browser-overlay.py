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
from pathlib import Path


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
    if identities == [record['beforeSha256'] for record in manifest['files']]:
        subprocess.run(['git', 'apply', '-p2', '--directory', str(out.relative_to(root)), str(patch_root / 'runtime-fixes.patch')], cwd=root, check=True)
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
    text = text.replace('#include "x86_context.h"', '#include "x86_context.h"\n#include "ff_sdk_guest_memory.hpp"')
    for macro, width in (("X86_MEM8", 1), ("X86_MEM16", 2), ("X86_MEM32", 4),
                         ("X86_MEM64", 8), ("X86_MEMF32", 4), ("X86_MEMF64", 8)):
        import re
        pattern = rf"#define {macro}\(a\).*"
        cast = {"X86_MEM8":"uint8_t", "X86_MEM16":"x86_u16u", "X86_MEM32":"x86_u32u",
                "X86_MEM64":"x86_u64u", "X86_MEMF32":"x86_f32u", "X86_MEMF64":"x86_f64u"}[macro]
        text = re.sub(pattern, f"#define {macro}(a) (*reinterpret_cast<{cast}*>(ff_sdk_guest_ptr(uint32_t(a), {width})))", text, count=1)
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
