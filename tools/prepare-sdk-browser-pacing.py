#!/usr/bin/env python3
"""Adapt the pinned SDK's browser frame and vertical-blank waits."""
from __future__ import annotations

from pathlib import Path


def adapt_browser_pacing(runtime: Path) -> None:
    runtime=Path(runtime).resolve()
    root=Path(__file__).resolve().parents[1]
    if not any(runtime.is_relative_to(base) and runtime!=base for base in (root/'.tools/artifacts',root/'artifacts')):
        raise RuntimeError('Pacing adaptation requires a private disposable SDK overlay')
    source = runtime / "xdk/d3d8.cpp"
    text = source.read_text(encoding="utf-8")

    include_anchor = "using namespace kernel;"
    if text.count(include_anchor) != 1:
        raise RuntimeError("Browser pacing declaration anchor differs")
    text = text.replace(
        include_anchor,
        '#if defined(__EMSCRIPTEN__)\n'
        'extern "C" bool ff_browser_precise_pacing_enabled();\n'
        'extern "C" void ff_browser_sleep_for(double, uint32_t);\n'
        'extern "C" void ff_browser_observe_vblank();\n'
        'extern "C" bool ff_browser_pace_before_present();\n'
        '#endif\n\n'
        + include_anchor,
        1,
    )

    counter="        SetDev(XDK_CDevice_VBlankCount, count);"
    if text.count(counter)!=1:
        raise RuntimeError('Browser vertical-blank observation anchor differs')
    text=text.replace(counter,counter+'\n#if defined(__EMSCRIPTEN__)\n        ff_browser_observe_vblank();\n#endif',1)
    wait_anchor = "        std::this_thread::sleep_until(next);"
    wait_replacement = (
        "#if defined(__EMSCRIPTEN__)\n"
        "        if (ff_browser_precise_pacing_enabled()) {\n"
        "            ff_browser_sleep_for(std::chrono::duration<double, std::milli>(next - clock::now()).count(), 1);\n"
        "        } else {\n"
        "            std::this_thread::sleep_until(next);\n"
        "        }\n"
        "#else\n"
        "        std::this_thread::sleep_until(next);\n"
        "#endif"
    )
    blank_anchor = "        std::this_thread::sleep_for(std::chrono::milliseconds(1));"
    blank_replacement = (
        "#if defined(__EMSCRIPTEN__)\n"
        "        ff_browser_sleep_for(1.0, 2);\n"
        "#else\n"
        "        std::this_thread::sleep_for(std::chrono::milliseconds(1));\n"
        "#endif"
    )
    if text.count(wait_anchor) != 1 or text.count(blank_anchor) != 1:
        raise RuntimeError("Browser vertical-blank sleep anchors differ")
    text = text.replace(wait_anchor, wait_replacement, 1)
    text = text.replace(blank_anchor, blank_replacement, 1)
    present_anchor = (
        "    if (g_backend) g_backend->Present(SurfaceOf(g_buffers[1]));\n"
        "    app::Pace();"
    )
    if text.count(present_anchor) != 1:
        raise RuntimeError("Browser frame pacing anchor differs")
    text = text.replace(present_anchor, (
        "#if defined(__EMSCRIPTEN__)\n"
        "    const bool pace_before_present = ff_browser_pace_before_present();\n"
        "#else\n"
        "    constexpr bool pace_before_present = false;\n"
        "#endif\n"
        "    if (pace_before_present) app::Pace();\n"
        "    if (g_backend) g_backend->Present(SurfaceOf(g_buffers[1]));\n"
        "    if (!pace_before_present) app::Pace();"
    ), 1)
    source.write_text(text, encoding="utf-8")
