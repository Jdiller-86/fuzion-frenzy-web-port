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
MAX_SEED_EXPORTS = 16
MAX_EXPANDED_EXPORTS = 32
MAX_SEED_FUNCTIONS = 64
MAX_EXPANDED_FUNCTIONS = 512
MAX_SEED_BYTES = 8 * 1024 * 1024
MAX_TOTAL_SEED_BYTES = 64 * 1024 * 1024


def digest(data):
    return hashlib.sha256(data).hexdigest()


def validate_instruction_count(count, limit=8192):
    if type(limit) is not int or limit not in (8192,16384) or type(count) is not int or not 0<count<=limit:
        raise ValueError('Instruction count exceeds the explicit supported budget')


def checked_kernel_data_binding(row, imports):
    """Admit the reached DWORD data import, never a fabricated call thunk.

    Ordinal 40 is HalDiskCachePartitionCount. The current reached form is an
    exact A1 absolute pointer load. Original-byte/layout and independent decoder
    admission still apply to this row before any generated module is accepted.
    """
    slots = {int(ref['target'], 16) for ref in row.get('references', [])
             if ref.get('type') == 'READ' and re.fullmatch(r'[0-9a-fA-F]+', ref.get('target', ''))
             and imports.get(int(ref['target'], 16)) == 40}
    if not slots:
        return None
    encoded = bytes.fromhex(row.get('analysisBytesHex', ''))
    if len(slots) != 1 or row.get('mnemonic') != 'MOV' or row.get('call') or row.get('computed') or \
       row.get('length') != 5 or len(encoded) != 5 or encoded[0] != 0xa1 or int.from_bytes(encoded[1:], 'little') not in slots:
        raise ValueError('Unsupported kernel-data pointer-load encoding')
    return slots.pop()


def checked_kernel_code_pointer_binding(row, imports):
    """Bind the reached NtDeviceIoControlFile pointer load before CALL reg.

    Restrict this extension to the declared function ordinal 196. Other data or
    unknown imports are not silently reclassified as callable function tokens.
    """
    slots={int(ref['target'],16) for ref in row.get('references',[])
           if ref.get('type')=='READ' and re.fullmatch(r'[0-9a-fA-F]+',ref.get('target',''))
           and imports.get(int(ref['target'],16))==196}
    if not slots:return None
    raw=bytes.fromhex(row.get('analysisBytesHex',''))
    encoded_slot=None
    if len(raw)==5 and raw[0]==0xa1:encoded_slot=int.from_bytes(raw[1:],'little')
    if len(raw)==6 and raw[0]==0x8b and raw[1]&0xc7==5:encoded_slot=int.from_bytes(raw[2:],'little')
    if len(slots)!=1 or row.get('mnemonic')!='MOV' or row.get('call') or row.get('computed') or \
       row.get('length')!=len(raw) or encoded_slot not in slots:
        raise ValueError('Unsupported kernel-code pointer-load encoding')
    return slots.pop()


def check_decoder_agreement(rows, decoded, original_bytes):
    """Retain exact boundaries except the explicit x87 WAIT/FNSTCW grouping.

    Ghidra's FSTCW includes 9B; Capstone exposes WAIT then FNSTCW. Both
    instructions remain in translated code. Never trim, skip or relabel bytes.
    """
    cursor, normalizations = 0, []
    for row in rows:
        address, size = int(row['address'], 16), row['length']
        if cursor < len(decoded) and (decoded[cursor].address, decoded[cursor].size) == (address, size):
            cursor += 1
            continue
        pair = decoded[cursor:cursor + 2]
        raw = original_bytes(address, size)
        if not (row.get('mnemonic') == 'FSTCW' and size >= 3 and len(pair) == 2 and
                (pair[0].address, pair[0].size, pair[0].mnemonic) == (address, 1, 'wait') and
                (pair[1].address, pair[1].size, pair[1].mnemonic) == (address + 1, size - 1, 'fnstcw') and
                raw[:2] == b'\x9b\xd9' and len(raw) == size and (raw[2] >> 3) & 7 == 7 and raw[2] >> 6 != 3):
            raise ValueError('Capstone and Ghidra instruction boundaries disagree')
        normalizations.append({'address': address, 'bytes': size, 'ghidra': 'FSTCW',
                               'capstone': ['wait', 'fnstcw'], 'translatedInstructions': 2})
        cursor += 2
    if cursor != len(decoded):
        raise ValueError('Capstone and Ghidra instruction coverage differs')
    return normalizations


def function_envelope(item, kernel_imports=None, local_entries=None):
    """Validate declared body membership separately from the translated span."""
    if not item.get("established") or item.get("external"):
        raise ValueError("Only established local functions are admitted")
    start = int(item["entry"], 16)
    if item.get("thunk"):
        rows = item.get("instructions", [])
        kernel_jump = len(rows) == 1 and rows[0].get("mnemonic") == "JMP" and rows[0].get("computed") and any(
                ref.get("type") == "INDIRECTION" and re.fullmatch(r"[0-9a-fA-F]+", ref.get("target", "")) and
                int(ref["target"], 16) in (kernel_imports or {}) for ref in rows[0].get("references", []))
        local_jump = False
        if len(rows) == 1 and rows[0].get('mnemonic') == 'JMP' and rows[0].get('computed') is False and not rows[0].get('call'):
            row=rows[0];flows=row.get('flows',[]);raw_hex=row.get('analysisBytesHex','')
            if len(flows)==1 and re.fullmatch(r'[0-9a-fA-F]{1,8}',flows[0]) and re.fullmatch(r'(?:[0-9a-fA-F]{4}|[0-9a-fA-F]{10})',raw_hex):
                raw=bytes.fromhex(raw_hex);target=int(flows[0],16)
                local_jump=(int(row.get('address','-1'),16)==start and row.get('length')==len(raw) and
                            (raw[0],len(raw)) in ((0xeb,2),(0xe9,5)) and target!=start and target in (local_entries or set()) and
                            (start+len(raw)+int.from_bytes(raw[1:],'little',signed=True))&0xffffffff==target)
        if not (kernel_jump or local_jump):
            raise ValueError("A local thunk must be one checked jump to an admitted function or declared kernel slot")
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


def combine_seed_functions(seeds, source_sha256, kernel_imports=None, function_limit=MAX_SEED_FUNCTIONS):
    """Compose bounded exports without silently replacing an earlier function."""
    if type(function_limit) is not int or function_limit not in (MAX_SEED_FUNCTIONS, 128, 256, MAX_EXPANDED_FUNCTIONS):
        raise ValueError("Use an explicit supported function limit")
    export_limit = MAX_EXPANDED_EXPORTS if function_limit >=256 else MAX_SEED_EXPORTS
    functions_per_export = 32 if function_limit >=256 else 8
    if not 1 <= len(seeds) <= export_limit:
        raise ValueError(f"Use 1-{export_limit} bounded seed exports")
    items_in = []
    for seed in seeds:
        if seed.get("kind") != "bounded-ghidra-recompilation-seed" or seed.get("sourceSha256", "").lower() != source_sha256:
            raise ValueError("Seed kind/executable identity differs")
        items = seed.get("functions", [])
        if not 1 <= len(items) <= functions_per_export:
            raise ValueError(f"Each seed must contain 1-{functions_per_export} functions")
        if sum(len(item.get('instructions',[])) for item in items)>8192:
            raise ValueError('Per-export instruction-row budget exceeded')
        items_in.extend(items)
    if len(items_in)>function_limit:
        raise ValueError("Aggregate function budget exceeded")
    local_entries={int(item['entry'],16) for item in items_in}
    functions, seen = [], set()
    for item in items_in:
        address, _ = function_envelope(item, kernel_imports, local_entries)
        if address in seen:
            raise ValueError("Duplicate function across seed exports")
        seen.add(address)
        functions.append(item)
    return functions


def validate_seed_sizes(sizes, export_limit=MAX_SEED_EXPORTS):
    if type(export_limit) is not int or export_limit not in (MAX_SEED_EXPORTS, MAX_EXPANDED_EXPORTS):
        raise ValueError("Unsupported export-group limit")
    if not 1 <= len(sizes) <= export_limit or any(type(size) is not int or size < 0 or size > MAX_SEED_BYTES for size in sizes):
        raise ValueError("Invalid bounded seed metadata sizes")
    if sum(sizes) > MAX_TOTAL_SEED_BYTES:
        raise ValueError("Aggregate seed metadata budget exceeded")


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


def checked_function_layout(item, read_bytes, kernel_imports=None, local_entries=None):
    """Account for the full original envelope, including typed local tables."""
    start, end = function_envelope(item, kernel_imports, local_entries)
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
    parser.add_argument("--seed", required=True, type=Path, action="append", help="Repeat for checked exports (8 functions each)")
    parser.add_argument("--function-limit", type=int, choices=(64, 128, 256, 512), default=64, help="Explicit aggregate startup limit; 256/512 admit up to 32 export groups")
    parser.add_argument("--instruction-limit", type=int, choices=(8192,16384), default=8192, help="Explicit aggregate instruction budget; older proofs retain 8192")
    parser.add_argument("--attempt", required=True)
    parser.add_argument("--guard-memory", action="store_true", help="Admit only checked scalar/stack/bulk memory surfaces")
    args = parser.parse_args()
    if not re.fullmatch(r"[a-zA-Z0-9_-]{1,48}", args.attempt):
        parser.error("Use a simple new attempt label")
    research = (ROOT / "research").resolve(strict=True)
    if not research.is_relative_to(ROOT):
        parser.error("Research must remain inside the project")
    export_limit = MAX_EXPANDED_EXPORTS if args.function_limit >=256 else MAX_SEED_EXPORTS
    if len(args.seed) > export_limit:
        parser.error(f"Use at most {export_limit} seed exports for this function limit")
    metadata, resolved, sizes = [], [], []
    for path, limit in [(args.profile, 1024 * 1024), *[(path, MAX_SEED_BYTES) for path in args.seed]]:
        path = path.resolve(strict=True)
        if not path.is_relative_to(research) or path.suffix != ".json" or path.stat().st_size > limit:
            parser.error("Use bounded existing private research JSON inputs")
        resolved.append((path, limit)); sizes.append(path.stat().st_size)
    try:
        validate_seed_sizes(sizes[1:], export_limit)
    except ValueError as error:
        parser.error(str(error))
    consumed = 0
    for index, (path, limit) in enumerate(resolved):
        if index:
            limit = min(limit, MAX_TOTAL_SEED_BYTES - consumed)
        with path.open("rb") as stream:
            data = stream.read(limit + 1)
        if len(data) > limit:
            parser.error("Metadata changed or exceeds its bounded input budget")
        if index:
            consumed += len(data)
        metadata.append(data)
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
        functions_in = combine_seed_functions(seeds, sha, imports, args.function_limit)
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
    functions, direct_targets, bindings, layouts, data_bindings, code_pointer_bindings = {}, set(), {}, {}, {}, {}
    local_entries={int(item['entry'],16) for item in functions_in}
    instruction_count = 0
    for item in functions_in:
        try:
            layout = checked_function_layout(item, original_bytes, imports, local_entries)
            start, end = layout["start"], layout["end"]
        except (KeyError, TypeError, ValueError) as error:
            parser.error(str(error))
        if any(start < info["end"] and address < end for address, info in functions.items()):
            parser.error("Selected function envelopes overlap")
        rows = layout["rows"]
        for row in rows:
            try:
                data_slot=checked_kernel_data_binding(row,imports)
            except ValueError as error:
                parser.error(str(error))
            if data_slot is not None:
                bindings[data_slot]=imports[data_slot]
                data_bindings[data_slot]=imports[data_slot]
            try:
                code_slot=checked_kernel_code_pointer_binding(row,imports)
            except ValueError as error:
                parser.error(str(error))
            if code_slot is not None:
                bindings[code_slot]=imports[code_slot]
                code_pointer_bindings[code_slot]=imports[code_slot]
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
    try:validate_instruction_count(instruction_count,args.instruction_limit)
    except ValueError as error:parser.error(str(error))
    if profile["entry"] not in functions:
        parser.error("Include the actual entry")
    boundaries = sorted(direct_targets - functions.keys())
    database = dict(functions)
    for address in boundaries:
        database[address] = {"name": f"ff_title_{address:08X}", "size": 0, "method": "untranslated_boundary"}
    out.mkdir()
    with (out / "prepare.log").open("x") as log, contextlib.redirect_stdout(log), contextlib.redirect_stderr(log):
        config.configure_from_xbe(str(xbe_path))
        disassembler = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        # Exact boundaries are retained except explicitly witnessed x87 grouping.
        decoder_normalizations = []
        for item in functions_in:
            start = int(item["entry"], 16)
            if layouts[start]["mixed"]:
                decoded = [instruction for row in item["instructions"] for instruction in disassembler.disasm(
                    original_bytes(int(row["address"], 16), row["length"]), int(row["address"], 16))]
            else:
                decoded = list(disassembler.disasm(original_bytes(start, functions[start]["size"]), start))
            decoder_normalizations.extend(check_decoder_agreement(item['instructions'], decoded, original_bytes))
        decoded_instruction_count = instruction_count + len(decoder_normalizations)
        validate_instruction_count(decoded_instruction_count,args.instruction_limit)
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
              "functionLimit": args.function_limit,
              "exportLimit": export_limit,
              "functionsPerExport": 32 if args.function_limit >=256 else 8,
              "instructionLimit": args.instruction_limit,
              "checkedMixedLayouts": dispatch_census,
              "returnFrameObservations": return_observations,
              "checkedMemoryAccesses": memory_observations,
              "recognizedSehPrologs": sorted(translator.lifter.SEH_PROLOGS),
              "kernelThunks": [int(item["entry"], 16) for item in functions_in if item.get("thunk") and item['instructions'][0].get('computed')],
              "localTailThunks": [{"entry":int(item['entry'],16),"target":int(item['instructions'][0]['flows'][0],16)}
                                  for item in functions_in if item.get('thunk') and not item['instructions'][0].get('computed')],
              "instructions": decoded_instruction_count, "ghidraInstructions": instruction_count,
              "originalBytesMatchGhidra": True, "decoderBoundariesAgree": not decoder_normalizations,
              "decoderInstructionsAgree": True, "decoderNormalizations": decoder_normalizations,
              "untranslatedDirectTargets": boundaries,
              "kernelBindings": [{"slot": slot, "ordinal": ordinal} for slot, ordinal in sorted(bindings.items())],
              "kernelDataBindings": [{"slot":slot,"ordinal":ordinal,"bytes":4} for slot,ordinal in sorted(data_bindings.items())],
              "kernelCodePointerBindings": [{"slot":slot,"ordinal":ordinal} for slot,ordinal in sorted(code_pointer_bindings.items())],
              "unimplementedInstructions": {}, "originalCodeExecuted": False,
              "outputSha256": {name: digest((out / name).read_bytes()) for name in ["title-profile.h", "title-functions.inc"]},
              "limitation": "Only selected startup functions translated; unsupported calls must stop. No original execution outside the browser."}
    with (out / "generation.json").open("x") as file:
        json.dump(report, file, indent=2)
    print(json.dumps({"output": str(out.relative_to(ROOT)), "functions": len(functions), "instructions": decoded_instruction_count,
                      "untranslatedDirectTargets": boundaries, "kernelBindings": report["kernelBindings"], "originalCodeExecuted": False}))


if __name__ == "__main__":
    main()
