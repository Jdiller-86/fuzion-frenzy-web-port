#!/usr/bin/env python3
"""Generate an owned x86 arithmetic fixture with the pinned recompilation tool.

This developer probe neither downloads tools nor reads/executes game code.
An optional metadata-only report supplies declared kernel import ordinals.
Generated C and reports stay in a new ignored local tool directory.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent
FIXTURE = bytes.fromhex("8b4424046bc00303442408c3")


def git(source, *args):
    result = subprocess.run(["git", "-C", str(source), *args], capture_output=True, text=True, check=True)
    return result.stdout.strip()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--attempt", required=True, help="new private output label")
    parser.add_argument("--profile", type=Path, help="existing metadata JSON under ignored research/")
    args = parser.parse_args()
    if not re.fullmatch(r"[a-zA-Z0-9_-]{1,48}", args.attempt):
        parser.error("Use a simple new attempt label")
    manifest = json.loads((ROOT / "tools/original-execution-toolchain.json").read_text())
    private = (ROOT / ".tools/emulation").resolve(strict=True)
    if not private.is_relative_to(ROOT):
        parser.error("Private tools must remain inside the project")
    candidate = manifest["nativeRecompProbe"]["source"]
    source = (private / candidate["directory"]).resolve(strict=True)
    if not source.is_relative_to(private) or git(source, "rev-parse", "HEAD") != candidate["commit"]:
        parser.error("Pinned local recompilation source is required")
    if git(source, "status", "--porcelain", "--untracked-files=no"):
        parser.error("Preserve a clean pinned source checkout")
    out = private / ("recomp-fixture-" + args.attempt)
    if out.exists():
        parser.error("Attempt already exists; preserve prior evidence")
    profile = None
    profile_bytes = None
    if args.profile:
        profile_path = args.profile.resolve(strict=True)
        research = (ROOT / "research").resolve(strict=True)
        if not research.is_relative_to(ROOT) or not profile_path.is_relative_to(research) or profile_path.suffix != ".json":
            parser.error("Only private research metadata JSON is accepted")
        if profile_path.stat().st_size > 1024 * 1024:
            parser.error("Metadata exceeds the probe limit")
        profile_bytes = profile_path.read_bytes()
        profile = json.loads(profile_bytes)
        if profile.get("kind") != "static-execution-dependencies" or not re.fullmatch(r"[0-9a-f]{64}", profile.get("sha256", "")):
            parser.error("Unexpected metadata identity/schema")
        imports = profile.get("profile", {}).get("imports")
        if not isinstance(imports, list) or not 1 <= len(imports) <= 4096 or any(
                not isinstance(entry, dict) or type(entry.get("ordinal")) is not int or
                not 1 <= entry["ordinal"] <= 65535 for entry in imports):
            parser.error("Unexpected metadata import list")
    if importlib.util.find_spec("capstone") is None:
        parser.error("Use a local Python environment with the pinned Capstone dependency")
    import capstone
    if capstone.__version__ != manifest["nativeRecompProbe"]["capstoneVersion"]:
        parser.error("Capstone version differs from the probe pin")
    sys.path.insert(0, str(source))
    from tools.recomp import config
    from tools.recomp.translator import FunctionTranslator
    from tools.kernel_audit.coverage import classify
    from tools.xbe_parser.xbe_parser import KERNEL_EXPORTS

    out.mkdir()
    # Install an explicitly synthetic section, not a disguised or modified XBE.
    base = 0x11000
    config._install([config.Section(".text", base, len(FIXTURE), 0, len(FIXTURE), True)],
                    base, 0, "source-owned-synthetic-fixture")
    info = {"name": "owned_arithmetic", "end": base + len(FIXTURE), "size": len(FIXTURE), "method": "entry_point"}
    translated = FunctionTranslator(FIXTURE, {base: info}).translate_function(base, info)
    if not translated or "UNIMPLEMENTED" in translated:
        raise RuntimeError("Owned fixture could not be translated")
    harness = '''#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include <assert.h>
#include <stdio.h>
ptrdiff_t g_xbox_mem_offset;
RECOMP_TLS uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_ebp;
RECOMP_TLS int g_df;
RECOMP_TLS uint32_t g_eflags;
''' + translated + '''
int main(void) {
    static uint32_t memory[16];
    const uint32_t values[] = {0,1,0x7fffffffu,0x80000000u,0xffffffffu};
    g_xbox_mem_offset = (ptrdiff_t)(uintptr_t)memory - 0x10000;
    unsigned checks = 0;
    for (unsigned i=0;i<5;i++) for (unsigned j=0;j<5;j++) {
        g_esp=0x10000; MEM32(g_esp)=0x11000;
        MEM32(g_esp+4)=values[i]; MEM32(g_esp+8)=values[j];
        owned_arithmetic();
        assert(g_eax == values[i]*3u+values[j]);
        assert(g_esp == 0x10004);
        checks++;
    }
    printf("Synthetic native-recomp fixture: %u cases passed\\n",checks);
    return 0;
}
'''
    with (out / "check.c").open("x", newline="\n") as file:
        file.write(harness)
    coverage = None
    if profile:
        imports = [{"ordinal": entry["ordinal"], "name": KERNEL_EXPORTS.get(entry["ordinal"], "Unknown")}
                   for entry in profile["profile"]["imports"]]
        covered, wrappers, data, todo, _sizes = classify(imports)
        coverage = {"imports": len(imports), "declaredRoutes": len(covered), "needsWrapper": len(wrappers),
                    "unroutedData": data, "unimplemented": todo,
                    "profileSha256": hashlib.sha256(profile_bytes).hexdigest(), "xbeSha256": profile["sha256"],
                    "limitation": "Declared routes only; stubs/behavior/browser support are not verified."}
    report = {"kind": "source-owned-native-recomp-fixture", "toolkitCommit": candidate["commit"],
              "capstoneVersion": capstone.__version__, "fixtureHex": FIXTURE.hex(),
              "fixtureSha256": hashlib.sha256(FIXTURE).hexdigest(),
              "generatedCSha256": hashlib.sha256((out / "check.c").read_bytes()).hexdigest(),
              "headerSha256": hashlib.sha256((source / "templates/runtime/recomp_types.h").read_bytes()).hexdigest(),
              "compileContract": ["-fwrapv", "-ffp-contract=off"], "kernelRoutes": coverage,
              "originalGameExecuted": False,
              "limitation": "One owned four-instruction integer/stack fixture, not title instruction coverage or a runtime."}
    with (out / "generation.json").open("x") as file:
        json.dump(report, file, indent=2)
    print(json.dumps({"output": str(out.relative_to(ROOT)), "kernelRoutes": coverage,
                      "originalGameExecuted": False}))


if __name__ == "__main__":
    main()
