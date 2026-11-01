#!/usr/bin/env python3
"""Prepare a bounded original-code recompilation proof; never execute it here.

Requires an existing read-only Ghidra seed, executable metadata and the explicitly
selected local XBE. All original/translated code stays in a new research folder.
The resulting module may only be executed through the browser-local proof host.
"""
import argparse
import contextlib
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent


def digest(data):
    return hashlib.sha256(data).hexdigest()


def function_envelope(item):
    """Validate declared body membership separately from the translated span."""
    if not item.get("established") or item.get("external") or item.get("thunk"):
        raise ValueError("Only established local functions are admitted")
    start = int(item["entry"], 16)
    ranges = [(int(r["start"], 16), int(r["endInclusive"], 16) + 1) for r in item["bodyRanges"]]
    if not ranges or ranges[0][0] != start:
        raise ValueError("Function body must start at its entry")
    previous, body_bytes = start, 0
    for low, high in ranges:
        if low < previous or high <= low:
            raise ValueError("Unordered, overlapping or empty function body ranges")
        body_bytes += high - low
        previous = high
    end = ranges[-1][1]
    if not 0 < end - start <= 65536 or item["bodyBytes"] != body_bytes:
        raise ValueError("Unsupported function body size")
    if len(ranges) > 1 and item.get("envelopeBytes") != end - start:
        raise ValueError("Split functions require an explicitly exported full envelope")
    return start, end


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--xbe", required=True, type=Path)
    parser.add_argument("--profile", required=True, type=Path)
    parser.add_argument("--seed", required=True, type=Path)
    parser.add_argument("--attempt", required=True)
    args = parser.parse_args()
    if not re.fullmatch(r"[a-zA-Z0-9_-]{1,48}", args.attempt):
        parser.error("Use a simple new attempt label")
    research = (ROOT / "research").resolve(strict=True)
    if not research.is_relative_to(ROOT):
        parser.error("Research must remain inside the project")
    metadata = []
    for path, limit in [(args.profile, 1024 * 1024), (args.seed, 8 * 1024 * 1024)]:
        path = path.resolve(strict=True)
        if not path.is_relative_to(research) or path.suffix != ".json" or path.stat().st_size > limit:
            parser.error("Use bounded existing private research JSON inputs")
        metadata.append(path.read_bytes())
    profile_report, seed = map(json.loads, metadata)
    if profile_report.get("kind") != "static-execution-dependencies" or seed.get("kind") != "bounded-ghidra-recompilation-seed":
        parser.error("Unexpected input metadata kind")
    xbe_path = args.xbe.resolve(strict=True)
    if xbe_path.stat().st_size != profile_report.get("bytes") or xbe_path.stat().st_size > 16 * 1024 * 1024:
        parser.error("Executable identity/size differs from the existing profile")
    xbe = xbe_path.read_bytes()
    sha = digest(xbe)
    if sha != profile_report.get("sha256") or sha != seed.get("sourceSha256", "").lower():
        parser.error("Executable and analysis identities differ")
    profile = profile_report["profile"]
    functions_in = seed.get("functions", [])
    if not 1 <= len(functions_in) <= 8:
        parser.error("The seed must contain 1-8 established functions")
    manifest = json.loads((ROOT / "tools/original-execution-toolchain.json").read_text())
    pin = manifest["nativeRecompProbe"]["source"]
    private = (ROOT / ".tools/emulation").resolve(strict=True)
    source = (private / pin["directory"]).resolve(strict=True)
    if not private.is_relative_to(ROOT) or not source.is_relative_to(private):
        parser.error("The pinned tools must remain private inside the project")
    def git(*arguments):
        return subprocess.check_output(["git", "-C", str(source), *arguments], text=True).strip()
    if git("rev-parse", "HEAD") != pin["commit"] or git("status", "--porcelain", "--untracked-files=no"):
        parser.error("A clean pinned recompilation source checkout is required")
    import capstone
    if capstone.__version__ != manifest["nativeRecompProbe"]["capstoneVersion"]:
        parser.error("Capstone differs from the probe pin")
    sys.path.insert(0, str(source))
    from tools.recomp import config
    from tools.recomp.translator import FunctionTranslator

    verification = (research / "verification").resolve(strict=True)
    if not verification.is_relative_to(research):
        parser.error("Verification output must remain inside private research")
    out = verification / ("recomp-title-" + args.attempt)
    if out.exists():
        parser.error("Attempt exists; preserve previous evidence")
    def original_bytes(address, size):
        for section in profile["sections"]:
            delta = address - section["address"]
            if section["executable"] and 0 <= delta and size <= section["fileBytes"] - delta:
                start = section["fileOffset"] + delta
                if start >= 0 and start + size <= len(xbe):
                    return xbe[start:start + size]
        raise ValueError("Instruction range is not original executable file-backed data")
    functions, direct_targets, bindings = {}, set(), {}
    imports = {entry["address"]: entry["ordinal"] for entry in profile["imports"]}
    instruction_count = 0
    for item in functions_in:
        try:
            start, end = function_envelope(item)
        except (KeyError, TypeError, ValueError) as error:
            parser.error(str(error))
        if any(start < info["end"] and address < end for address, info in functions.items()):
            parser.error("Selected function envelopes overlap")
        cursor = start
        rows = item.get("instructions", [])
        if not rows or len(rows) > 4096:
            parser.error("Instruction budget exceeded")
        for row in rows:
            address, length = int(row["address"], 16), row["length"]
            if address != cursor or type(length) is not int or not 1 <= length <= 15 or address + length > end:
                parser.error("Gaps or inconsistent instruction boundaries in seed")
            if original_bytes(address, length).hex() != row["analysisBytesHex"]:
                parser.error("Ghidra analysis bytes differ from the original executable")
            cursor += length
            if row["call"] and not row["computed"]:
                direct_targets.update(int(target, 16) for target in row["flows"])
            if row["call"] and row["computed"]:
                for ref in row["references"]:
                    if ref["type"] == "READ" and re.fullmatch(r"[0-9a-fA-F]+", ref["target"]):
                        slot = int(ref["target"], 16)
                        if slot in imports:
                            bindings[slot] = imports[slot]
        if cursor != end:
            parser.error("Instruction listing does not cover the complete selected body")
        instruction_count += len(rows)
        functions[start] = {"name": f"ff_title_{start:08X}", "end": end, "size": end - start, "bodyBytes": item["bodyBytes"],
                            "method": "entry_point" if start == profile["entry"] else "call_target"}
    if instruction_count > 8192 or profile["entry"] not in functions:
        parser.error("Include the actual entry within the bounded instruction budget")
    boundaries = sorted(direct_targets - functions.keys())
    database = dict(functions)
    for address in boundaries:
        database[address] = {"name": f"ff_title_{address:08X}", "size": 0, "method": "untranslated_boundary"}
    out.mkdir()
    with (out / "prepare.log").open("x") as log, contextlib.redirect_stdout(log), contextlib.redirect_stderr(log):
        config.configure_from_xbe(str(xbe_path))
        disassembler = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        # Separate decoders must agree on exact instruction addresses/lengths.
        for item in functions_in:
            start = int(item["entry"], 16)
            decoded = list(disassembler.disasm(original_bytes(start, functions[start]["size"]), start))
            if [(r.address, r.size) for r in decoded] != [(int(r["address"], 16), r["length"]) for r in item["instructions"]]:
                raise ValueError("Capstone and Ghidra instruction boundaries disagree")
        translator = FunctionTranslator(xbe, database, trace_functions=set(functions))
        generated = []
        for address, info in functions.items():
            text = translator.translate_function(address, info)
            if not text or translator.lifter.unimplemented:
                raise ValueError("Translation contains unsupported instructions: " + str(translator.lifter.unimplemented))
            generated.append(text)
    header = [f'#define FF_SOURCE_SHA256 "{sha}"', f'#define FF_ENTRY_ADDRESS 0x{profile["entry"]:08X}u',
              f'#define FF_ENTRY_FUNCTION {functions[profile["entry"]]["name"]}',
              f'#define FF_CODE_LOW 0x{min(s["address"] for s in profile["sections"] if s["executable"]):08X}u',
              f'#define FF_CODE_HIGH 0x{max(s["address"] + s["virtualBytes"] for s in profile["sections"] if s["executable"]):08X}u',
              f'#define FF_KERNEL_BINDING_COUNT {len(bindings)}u',
              '#define FF_KERNEL_BINDINGS { ' + ', '.join(f'{{0x{slot:08X}u,{ordinal}u}}' for slot, ordinal in sorted(bindings.items())) + ' }',
              '#define FF_TITLE_LOOKUP_CASES ' + ' '.join(f'case 0x{address:08X}u: return {info["name"]};' for address, info in functions.items())]
    header += [f'void {info["name"]}(void);' for info in database.values()]
    for filename, text in [("title-profile.h", "\n".join(header) + "\n"),
                           ("title-functions.inc", "\n".join(generated) + "\n" + "\n".join(
                               f'void ff_title_{a:08X}(void) {{ ff_probe_stop_direct(0x{a:08X}u); }}' for a in boundaries) + "\n")]:
        with (out / filename).open("x", newline="\n") as file:
            file.write(text)
    report = {"kind": "bounded-original-startup-recompilation", "sourceSha256": sha, "toolkitCommit": pin["commit"],
              "seedSha256": digest(metadata[1]), "metadataSha256": digest(metadata[0]),
              "functions": [{"entry": address, "bytes": info["size"], "bodyBytes": info["bodyBytes"]} for address, info in functions.items()],
              "recognizedSehPrologs": sorted(translator.lifter.SEH_PROLOGS),
              "instructions": instruction_count, "originalBytesMatchGhidra": True, "decoderBoundariesAgree": True,
              "untranslatedDirectTargets": boundaries,
              "kernelBindings": [{"slot": slot, "ordinal": ordinal} for slot, ordinal in sorted(bindings.items())],
              "unimplementedInstructions": {}, "originalCodeExecuted": False,
              "outputSha256": {name: digest((out / name).read_bytes()) for name in ["title-profile.h", "title-functions.inc"]},
              "limitation": "Only selected startup functions translated; unsupported calls must stop. No original execution outside the browser."}
    with (out / "generation.json").open("x") as file:
        json.dump(report, file, indent=2)
    print(json.dumps({"output": str(out.relative_to(ROOT)), "functions": len(functions), "instructions": instruction_count,
                      "untranslatedDirectTargets": boundaries, "kernelBindings": report["kernelBindings"], "originalCodeExecuted": False}))


if __name__ == "__main__":
    main()
