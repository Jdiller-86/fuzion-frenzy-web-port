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


def function_envelope(item, kernel_imports=None):
    """Validate declared body membership separately from the translated span."""
    if not item.get("established") or item.get("external"):
        raise ValueError("Only established local functions are admitted")
    if item.get("thunk"):
        rows = item.get("instructions", [])
        if len(rows) != 1 or rows[0].get("mnemonic") != "JMP" or not rows[0].get("computed") or not any(
                ref.get("type") == "INDIRECTION" and re.fullmatch(r"[0-9a-fA-F]+", ref.get("target", "")) and
                int(ref["target"], 16) in (kernel_imports or {}) for ref in rows[0].get("references", [])):
            raise ValueError("A local thunk must be one checked indirect jump to a declared kernel slot")
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


def combine_seed_functions(seeds, source_sha256, kernel_imports=None):
    """Compose bounded exports without silently replacing an earlier function."""
    if not 1 <= len(seeds) <= 8:
        raise ValueError("Use 1-8 bounded seed exports")
    functions, seen = [], set()
    for seed in seeds:
        if seed.get("kind") != "bounded-ghidra-recompilation-seed" or seed.get("sourceSha256", "").lower() != source_sha256:
            raise ValueError("Seed kind/executable identity differs")
        items = seed.get("functions", [])
        if not 1 <= len(items) <= 8:
            raise ValueError("Each seed must contain 1-8 functions")
        for item in items:
            address, _ = function_envelope(item, kernel_imports)
            if address in seen:
                raise ValueError("Duplicate function across seed exports")
            seen.add(address)
            functions.append(item)
    return functions


def _alignment_padding(raw):
    """Known x86 no-effect alignment encodings, not arbitrary skipped code."""
    cursor = 0
    encodings = (b"\x8d\x49\x00", b"\x8b\xff", b"\x90")
    while cursor < len(raw):
        match = next((code for code in encodings if raw.startswith(code, cursor)), None)
        if match is None:
            return False
        cursor += len(match)
    return bool(raw)


def checked_function_layout(item, read_bytes, kernel_imports=None):
    """Account for the full original envelope, including typed local tables."""
    start, end = function_envelope(item, kernel_imports)
    rows = item.get("instructions", [])
    if not rows or len(rows) > 4096:
        raise ValueError("Instruction budget exceeded")
    by_address, previous = {}, start
    for row in rows:
        address, length = int(row["address"], 16), row["length"]
        if type(length) is not int or not 1 <= length <= 15 or address < previous or address + length > end:
            raise ValueError("Unordered or overlapping instruction rows")
        raw = read_bytes(address, length)
        if len(raw) != length or raw.hex() != row["analysisBytesHex"]:
            raise ValueError("Ghidra instruction bytes differ from the original executable")
        by_address[address] = row
        previous = address + length
    layout = item.get("layout")
    if layout is None:
        cursor = start
        for address, row in by_address.items():
            if address != cursor:
                raise ValueError("Gaps or inconsistent instruction boundaries in seed")
            cursor += row["length"]
        if cursor != end:
            raise ValueError("Instruction listing does not cover the complete selected envelope")
        return {"start": start, "end": end, "rows": rows, "tables": [], "mixed": False,
                "dataBytes": 0, "paddingBytes": 0}
    if not isinstance(layout, list) or not 1 <= len(layout) <= 8192:
        raise ValueError("Invalid bounded envelope layout")
    cursor, seen, tables, padding_bytes, data_bytes = start, set(), [], 0, 0
    for part in layout:
        address, size, kind = int(part["start"], 16), part["bytes"], part["kind"]
        if address != cursor or type(size) is not int or size <= 0 or address + size > end:
            raise ValueError("Layout must cover the entire envelope without gaps or overlap")
        if kind == "instruction":
            if address not in by_address or by_address[address]["length"] != size or address in seen:
                raise ValueError("Layout and instruction rows differ")
            seen.add(address)
        else:
            raw = read_bytes(address, size)
            if len(raw) != size or raw.hex() != part.get("analysisBytesHex"):
                raise ValueError("Ghidra non-code bytes differ from the original executable")
            if kind == "undefined":
                if not _alignment_padding(raw):
                    raise ValueError("Undefined bytes are not established alignment padding")
                padding_bytes += size
                if padding_bytes > 256:
                    raise ValueError("Alignment padding budget exceeded")
            elif kind == "data":
                if size != 4 or part.get("dataType") != "pointer":
                    raise ValueError("Only declared 32-bit local pointer-table data is supported")
                target = int.from_bytes(raw, "little")
                if target not in by_address:
                    raise ValueError("Table data must target an established local instruction boundary")
                if tables and tables[-1]["end"] == address:
                    tables[-1]["end"] += 4; tables[-1]["targets"].append(target)
                else:
                    tables.append({"start": address, "end": address + 4, "targets": [target]})
                data_bytes += 4
            else:
                raise ValueError("Unknown envelope layout kind")
        cursor += size
    if cursor != end or seen != by_address.keys():
        raise ValueError("Layout leaves envelope bytes or instructions unaccounted for")
    if not (data_bytes or padding_bytes):
        return {"start": start, "end": end, "rows": rows, "tables": [], "mixed": False,
                "dataBytes": 0, "paddingBytes": 0}
    for row in rows:
        if row.get("fallthrough"):
            target = int(row["fallthrough"], 16)
            if target not in by_address:
                raise ValueError("Fallthrough reaches data or excluded padding")
        if not row.get("call"):
            for value in row.get("flows", []):
                target = int(value, 16)
                if not start <= target < end or target not in by_address:
                    raise ValueError("Mixed-layout control flow must remain in checked local code")
    return {"start": start, "end": end, "rows": rows, "tables": tables, "mixed": bool(data_bytes or padding_bytes),
            "dataBytes": data_bytes, "paddingBytes": padding_bytes}


def checked_dispatch_tables(layout, sites):
    """Validate table anchors (including biased anchors) and exhaustive flows.

    Sites are normalized from the independent decoder, not inferred by scanning
    words that happen to resemble code pointers. Emission loads the original
    target value; the anchor does not manufacture or rewrite table entries.
    """
    tables, used, census = layout["tables"], set(), {}
    rows = {int(row["address"], 16): row for row in layout["rows"]}
    expected_sites = {address for address, row in rows.items()
                      if row.get("computed") and not row.get("call") and row.get("mnemonic") == "JMP"}
    if len({site["address"] for site in sites}) != len(sites) or {site["address"] for site in sites} != expected_sites:
        raise ValueError("Computed-jump census does not cover every established dispatch")
    for site in sites:
        if site["base"] or not site["index"] or site["scale"] != 4 or site["size"] != 4:
            raise ValueError("Unsupported computed-jump operand")
        row = rows[site["address"]]
        if row.get("call") or not row.get("computed") or row.get("mnemonic") != "JMP":
            raise ValueError("Decoder and Ghidra disagree on computed dispatch")
        flows = {int(value, 16) for value in row.get("flows", [])}
        candidates = [i for i, table in enumerate(tables)
                      if table["start"] - 4 <= site["anchor"] <= table["end"] and (site["anchor"] - table["start"]) % 4 == 0
                      and set(table["targets"]) == flows]
        if not flows or len(candidates) != 1:
            raise ValueError("Computed jump lacks one complete declared local table")
        index = candidates[0]; targets = tables[index]["targets"]
        if site["anchor"] in census and census[site["anchor"]] != targets:
            raise ValueError("Conflicting dispatch tables for one anchor")
        census[site["anchor"]] = targets; used.add(index)
    if used != set(range(len(tables))):
        raise ValueError("Declared data is not accounted for by validated dispatches")
    return census


def observe_return_frames(text, address):
    """Observe the restored local EBP, not the vendor's last-frame bridge.

    The pinned translator keeps EBP local while g_ebp publishes a frame for
    callees. This hook only observes the value at existing exit-trace sites;
    it changes no translated instruction or register/stack behavior.
    """
    if not re.search(r"\buint32_t\s+[^;]*\bebp\s*=\s*0\b[^;]*;", text):
        return text, 0
    marker = f'RECOMP_TRACE_EXIT("ff_title_{address:08X}", 0x{address:08X})'
    count = text.count(marker)
    if not count:
        raise ValueError("A local-frame function lacks its pinned exit observation")
    return text.replace(marker, f'FF_RETURN_FRAME(0x{address:08X}u, ebp); ' + marker), count


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--xbe", required=True, type=Path)
    parser.add_argument("--profile", required=True, type=Path)
    parser.add_argument("--seed", required=True, type=Path, action="append", help="Repeat for up to 8 checked exports (8 functions each)")
    parser.add_argument("--attempt", required=True)
    parser.add_argument("--guard-memory", action="store_true", help="Admit only checked scalar/stack/bulk memory surfaces")
    args = parser.parse_args()
    if not re.fullmatch(r"[a-zA-Z0-9_-]{1,48}", args.attempt):
        parser.error("Use a simple new attempt label")
    research = (ROOT / "research").resolve(strict=True)
    if not research.is_relative_to(ROOT):
        parser.error("Research must remain inside the project")
    if len(args.seed) > 8:
        parser.error("Use at most 8 seed exports")
    metadata = []
    for path, limit in [(args.profile, 1024 * 1024), *[(path, 8 * 1024 * 1024) for path in args.seed]]:
        path = path.resolve(strict=True)
        if not path.is_relative_to(research) or path.suffix != ".json" or path.stat().st_size > limit:
            parser.error("Use bounded existing private research JSON inputs")
        metadata.append(path.read_bytes())
    profile_report, *seeds = map(json.loads, metadata)
    if profile_report.get("kind") != "static-execution-dependencies":
        parser.error("Unexpected input metadata kind")
    xbe_path = args.xbe.resolve(strict=True)
    if xbe_path.stat().st_size != profile_report.get("bytes") or xbe_path.stat().st_size > 16 * 1024 * 1024:
        parser.error("Executable identity/size differs from the existing profile")
    xbe = xbe_path.read_bytes()
    sha = digest(xbe)
    if sha != profile_report.get("sha256"):
        parser.error("Executable and analysis identities differ")
    profile = profile_report["profile"]
    imports = {entry["address"]: entry["ordinal"] for entry in profile["imports"]}
    try:
        functions_in = combine_seed_functions(seeds, sha, imports)
    except (KeyError, TypeError, ValueError) as error:
        parser.error(str(error))
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
    functions, direct_targets, bindings, layouts = {}, set(), {}, {}
    instruction_count = 0
    for item in functions_in:
        try:
            layout = checked_function_layout(item, original_bytes, imports)
            start, end = layout["start"], layout["end"]
        except (KeyError, TypeError, ValueError) as error:
            parser.error(str(error))
        if any(start < info["end"] and address < end for address, info in functions.items()):
            parser.error("Selected function envelopes overlap")
        rows = layout["rows"]
        for row in rows:
            if row["call"] and not row["computed"]:
                direct_targets.update(int(target, 16) for target in row["flows"])
            if row["computed"] and (row["call"] or row.get("mnemonic") == "JMP"):
                for ref in row["references"]:
                    if ref["type"] in ("READ", "INDIRECTION") and re.fullmatch(r"[0-9a-fA-F]+", ref["target"]):
                        slot = int(ref["target"], 16)
                        if slot in imports:
                            bindings[slot] = imports[slot]
        instruction_count += len(rows)
        layouts[start] = layout
        functions[start] = {"name": f"ff_title_{start:08X}", "end": end, "size": end - start, "bodyBytes": item["bodyBytes"],
                            "method": "entry_point" if start == profile["entry"] else "call_target",
                            "dataBytes": layout["dataBytes"], "paddingBytes": layout["paddingBytes"]}
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
            if layouts[start]["mixed"]:
                decoded = [instruction for row in item["instructions"] for instruction in disassembler.disasm(
                    original_bytes(int(row["address"], 16), row["length"]), int(row["address"], 16))]
            else:
                decoded = list(disassembler.disasm(original_bytes(start, functions[start]["size"]), start))
            if [(r.address, r.size) for r in decoded] != [(int(r["address"], 16), r["length"]) for r in item["instructions"]]:
                raise ValueError("Capstone and Ghidra instruction boundaries disagree")
        translator = FunctionTranslator(xbe, database, trace_functions=set(functions))
        dispatch_census = []
        for start, layout in layouts.items():
            if not layout["mixed"]:
                continue
            instructions = [instruction for row in layout["rows"] for instruction in translator.disasm.disassemble_function(
                original_bytes(int(row["address"], 16), row["length"]), int(row["address"], 16),
                int(row["address"], 16) + row["length"])]
            if [(instruction.address, instruction.size) for instruction in instructions] != [
                    (int(row["address"], 16), row["length"]) for row in layout["rows"]]:
                raise ValueError("Translator decoder differs from the checked sparse instruction set")
            addresses = {instruction.address for instruction in instructions}
            sites = []
            for instruction in instructions:
                if instruction.is_branch and instruction.jump_target is not None and instruction.jump_target not in addresses:
                    raise ValueError("Decoded mixed-layout branch leaves established local code")
                if not instruction.is_terminator and instruction.end_address not in addresses:
                    raise ValueError("Decoded mixed-layout fallthrough enters excluded bytes")
                if instruction.is_jump and instruction.jump_target is None:
                    if len(instruction.operands) != 1 or instruction.operands[0].type != "mem":
                        raise ValueError("Unsupported mixed-layout computed jump")
                    op = instruction.operands[0]
                    sites.append({"address": instruction.address, "base": op.mem_base, "index": op.mem_index,
                                  "scale": op.mem_scale, "size": op.mem_size, "anchor": op.mem_disp})
            tables = checked_dispatch_tables(layout, sites)
            # The clean pinned translator explicitly supports authoritative,
            # sparse recovered CFGs. Never ask it to decode table bytes as code.
            translator._recovered_cfg[start] = {"end": layout["end"], "instructions": instructions, "jump_tables": tables}
            translator.coalesced_function_starts.add(start)
            dispatch_census.append({"function": start, "sites": len(sites), "tableRegions": len(layout["tables"]),
                                    "anchors": len(tables), "dataBytes": layout["dataBytes"], "paddingBytes": layout["paddingBytes"]})
        generated, return_observations, memory_observations = [], [], []
        for address, info in functions.items():
            text = translator.translate_function(address, info)
            if not text or translator.lifter.unimplemented:
                raise ValueError("Translation contains unsupported instructions: " + str(translator.lifter.unimplemented))
            returns = any(row.get("mnemonic", "").startswith("RET") for row in layouts[address]["rows"])
            text, observations = observe_return_frames(text, address) if returns else (text, 0)
            if observations:
                return_observations.append({"function": address, "sites": observations})
            if args.guard_memory:
                from recomp_memory_access import instrument_memory_accesses
                text, access_counts = instrument_memory_accesses(text)
                memory_observations.append({"function": address, **access_counts})
            generated.append(text)
    header = [f'#define FF_SOURCE_SHA256 "{sha}"', f'#define FF_ENTRY_ADDRESS 0x{profile["entry"]:08X}u',
              f'#define FF_ENTRY_FUNCTION {functions[profile["entry"]]["name"]}',
              f'#define FF_CODE_LOW 0x{min(s["address"] for s in profile["sections"] if s["executable"]):08X}u',
              f'#define FF_CODE_HIGH 0x{max(s["address"] + s["virtualBytes"] for s in profile["sections"] if s["executable"]):08X}u',
              f'#define FF_KERNEL_BINDING_COUNT {len(bindings)}u',
              '#define FF_KERNEL_BINDINGS { ' + ', '.join(f'{{0x{slot:08X}u,{ordinal}u}}' for slot, ordinal in sorted(bindings.items())) + ' }',
              '#define FF_TITLE_LOOKUP_CASES ' + ' '.join(f'case 0x{address:08X}u: return {info["name"]};' for address, info in functions.items())]
    header += ['#ifndef FF_RETURN_FRAME', '#define FF_RETURN_FRAME(address, frame) ((void)0)', '#endif']
    if args.guard_memory:
        header += ['#define FF_GUARDED_MEMORY 1', '#include "recomp_memory_access.h"']
    header += [f'void {info["name"]}(void);' for info in database.values()]
    for filename, text in [("title-profile.h", "\n".join(header) + "\n"),
                           ("title-functions.inc", "\n".join(generated) + "\n" + "\n".join(
                               f'void ff_title_{a:08X}(void) {{ ff_probe_stop_direct(0x{a:08X}u); }}' for a in boundaries) + "\n")]:
        with (out / filename).open("x", newline="\n") as file:
            file.write(text)
    report = {"kind": "bounded-original-startup-recompilation", "sourceSha256": sha, "toolkitCommit": pin["commit"],
              "seedSha256": digest(metadata[1]), "metadataSha256": digest(metadata[0]),
              "seedSha256s": [digest(data) for data in metadata[1:]],
              "functions": [{"entry": address, "bytes": info["size"], "bodyBytes": info["bodyBytes"],
                             "dataBytes": info["dataBytes"], "paddingBytes": info["paddingBytes"]} for address, info in functions.items()],
              "checkedMixedLayouts": dispatch_census,
              "returnFrameObservations": return_observations,
              "checkedMemoryAccesses": memory_observations,
              "recognizedSehPrologs": sorted(translator.lifter.SEH_PROLOGS),
              "kernelThunks": [int(item["entry"], 16) for item in functions_in if item.get("thunk")],
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
