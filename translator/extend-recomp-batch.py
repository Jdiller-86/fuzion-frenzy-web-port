#!/usr/bin/env python3
"""Add one reached Ghidra seed to an existing bounded recompilation batch.

This coordinator prepares private static artifacts only. Translation, XBE byte
checks, layout/decoder admission and memory guards remain in prepare-title-recomp.py.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent
VERIFY = ROOT / "research" / "verification"
GEN_KIND = "bounded-original-startup-recompilation"
INDEX_KIND = "recomp-seed-index"
SEED_KIND = "bounded-ghidra-recompilation-seed"
TOOLKIT_COMMIT = "f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b"
MAX_SEED_BYTES = 8 * 1024 * 1024
MAX_TOTAL_SEED_BYTES = 64 * 1024 * 1024


def sha(data):
    return hashlib.sha256(data).hexdigest()


def sha_file(path, limit):
    if path.stat().st_size > limit:
        raise ValueError(f"Input exceeds {limit} bytes: {path}")
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def address_value(value):
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        return int(value, 16)
    raise ValueError("Address must be an integer or hexadecimal string")


def read_json(path, limit=MAX_SEED_BYTES):
    if path.stat().st_size > limit:
        raise ValueError(f"Input exceeds {limit} bytes: {path}")
    with path.open("rb") as stream:
        data = stream.read(limit + 1)
    if len(data) > limit:
        raise ValueError(f"Input changed or exceeds {limit} bytes: {path}")
    return json.loads(data), data


def private_path(value, *, must_exist=True):
    path = Path(value).resolve(strict=must_exist)
    if not path.is_relative_to(VERIFY.resolve(strict=True)):
        raise ValueError("All input/output artifacts must remain inside research/verification")
    return path


def research_path(value, *, must_exist=True):
    path = Path(value).resolve(strict=must_exist)
    if not path.is_relative_to((ROOT / "research").resolve(strict=True)):
        raise ValueError("Input artifact must remain inside private research")
    return path


def identity(generation, xbe_path, metadata_path):
    if generation.get("kind") != GEN_KIND:
        raise ValueError("Previous report is not a bounded recompilation generation")
    if xbe_path.stat().st_size > 16 * 1024 * 1024 or metadata_path.stat().st_size > 1024 * 1024:
        raise ValueError("XBE or metadata exceeds existing preparation input bounds")
    xbe_hash = sha_file(xbe_path, 16 * 1024 * 1024)
    metadata_hash = sha_file(metadata_path, 1024 * 1024)
    if xbe_hash != generation.get("sourceSha256"):
        raise ValueError("XBE identity differs from previous generation")
    if metadata_hash != generation.get("metadataSha256"):
        raise ValueError("Metadata identity differs from previous generation")
    if generation.get("toolkitCommit") != TOOLKIT_COMMIT:
        raise ValueError("Previous generation toolkit pin differs from the current manifest")
    return xbe_hash, metadata_hash


def load_seed_index(index, index_path, generation, generation_bytes):
    if index.get("kind") != INDEX_KIND or index.get("baseGenerationSha256") != sha(generation_bytes):
        raise ValueError("Seed index does not identify the supplied base generation")
    entries = index.get("entries")
    expected = generation.get("seedSha256s")
    if not isinstance(entries, list) or not isinstance(expected, list) or len(entries) != len(expected):
        raise ValueError("Seed index must have one ordered path per previous seed")
    if len(entries) != generation.get("exportLimit"):
        raise ValueError("Previous seed count differs from its export limit")
    resolved, total = [], 0
    seen_paths = set()
    for row, expected_hash in zip(entries, expected):
        path = Path(row["path"])
        if not path.is_absolute():
            path = ROOT / path
        path = research_path(path)
        if path in seen_paths:
            raise ValueError("Duplicate seed path in explicit index")
        seen_paths.add(path)
        if path.suffix.lower() != ".json":
            raise ValueError("Seed paths must be JSON")
        if path.stat().st_size > MAX_SEED_BYTES:
            raise ValueError("Seed byte budget exceeded")
        data = path.read_bytes()
        total += len(data)
        if len(data) > MAX_SEED_BYTES or total > MAX_TOTAL_SEED_BYTES:
            raise ValueError("Seed byte budget exceeded")
        digest = sha(data)
        if digest != row.get("sha256") or digest != expected_hash:
            raise ValueError("Seed identity/order differs from previous generation")
        seed = json.loads(data)
        if seed.get("kind") != SEED_KIND or seed.get("sourceSha256", "").lower() != generation["sourceSha256"].lower():
            raise ValueError("Seed kind or source identity differs")
        resolved.append((path, seed, data))
    return resolved


def validate_reached(report, xbe_hash, source_size, addition):
    if report.get("sourceSha256", "").lower() != xbe_hash.lower():
        raise ValueError("Reached report source identity differs from XBE")
    body = report.get("report")
    if isinstance(body, dict) and body.get("sourceSha256") not in (None, report.get("sourceSha256")):
        raise ValueError("Reached wrapper/report source identities differ")
    if isinstance(body, dict) and body.get("moduleSha256") not in (None, report.get("moduleSha256")):
        raise ValueError("Reached wrapper/report module identities differ")
    stop = body.get("stop") if isinstance(body, dict) else None
    if not isinstance(stop, dict) or stop.get("status") != 2 or not isinstance(stop.get("target"), int):
        raise ValueError("Reached report must document a status-2 untranslated target")
    reached_target = stop["target"]
    if reached_target not in {address_value(item["entry"]) for item in addition.get("functions", [])}:
        raise ValueError("Addition seed must contain the exact status-2 stop target")
    if report.get("moduleSha256") is None or not re.fullmatch(r"[0-9a-fA-F]{64}", report["moduleSha256"]):
        raise ValueError("Reached report lacks a valid module identity")
    if not isinstance(source_size, int) or source_size <= 0:
        raise ValueError("Invalid XBE size")


def binding_pairs(generation):
    def slot_value(value):
        return int(value, 16) if isinstance(value, str) else int(value)
    return {(slot_value(row["slot"]), int(row["ordinal"])) for row in generation.get("kernelBindings", [])}


def pack_addition(previous_seeds, addition, generation):
    if addition.get("kind") != SEED_KIND or addition.get("sourceSha256", "").lower() != generation["sourceSha256"].lower():
        raise ValueError("Addition seed kind/source identity differs")
    funcs = addition.get("functions")
    old_funcs = [item for _, seed, _ in previous_seeds for item in seed.get("functions", [])]
    entries = [address_value(item["entry"]) for item in old_funcs]
    new_entries = [address_value(item["entry"]) for item in funcs or []]
    if not funcs or len(set(entries + new_entries)) != len(entries) + len(new_entries):
        raise ValueError("Addition is empty or duplicates a retained function")
    new_instruction_count = sum(len(item.get("instructions", [])) for item in funcs)
    if generation.get("instructions", 0) + new_instruction_count > generation.get("instructionLimit", 0):
        raise ValueError("Addition exceeds the existing aggregate instruction budget")
    max_per = generation.get("functionsPerExport")
    if not isinstance(max_per, int) or not previous_seeds:
        raise ValueError("Missing existing export-group capacity")
    selected = next((i for i in range(len(previous_seeds) - 1, -1, -1)
                     if len(previous_seeds[i][1].get("functions", [])) + len(funcs) <= max_per), None)
    if selected is None:
        raise ValueError("No existing seed group has room within its current cap")
    merged = dict(previous_seeds[selected][1])
    merged["functions"] = list(merged["functions"]) + list(funcs)
    return selected, merged


def parse_allowlist(values):
    pairs = set()
    for value in values:
        match = re.fullmatch(r"(0x[0-9a-fA-F]+|[0-9a-fA-F]+):(0x[0-9a-fA-F]+|[0-9]+)", value)
        if not match:
            raise ValueError("Binding allow-list entries must be exact slot:ordinal pairs")
        slot_text, ordinal_text = match.groups()
        slot = int(slot_text, 16) if not slot_text.lower().startswith("0x") else int(slot_text, 16)
        ordinal = int(ordinal_text, 16) if ordinal_text.lower().startswith("0x") else int(ordinal_text, 10)
        pairs.add((slot, ordinal))
    if len(pairs) != len(values):
        raise ValueError("Duplicate binding allow-list pair")
    return pairs


def function_definitions(source):
    """Return generated ff_title function definitions including exact bodies."""
    found = {}
    for match in re.finditer(r"(?m)^void (ff_title_[0-9A-Fa-f]{8})\(void\)\s*\{", source):
        name = match.group(1)
        if name in found:
            raise ValueError(f"Duplicate generated function definition: {name}")
        start = match.end() - 1
        depth = 0
        state = "code"
        escaped = False
        i = start
        while i < len(source):
            char = source[i]
            nxt = source[i + 1] if i + 1 < len(source) else ""
            if state == "line_comment":
                if char == "\n":
                    state = "code"
            elif state == "block_comment":
                if char == "*" and nxt == "/":
                    state = "code"
                    i += 1
            elif state in ("string", "char"):
                if escaped:
                    escaped = False
                elif char == "\\":
                    escaped = True
                elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                    state = "code"
            elif char == "/" and nxt == "/":
                state = "line_comment"
                i += 1
            elif char == "/" and nxt == "*":
                state = "block_comment"
                i += 1
            elif char == '"':
                state = "string"
            elif char == "'":
                state = "char"
            elif char == "{":
                depth += 1
            elif char == "}":
                depth -= 1
                if depth == 0:
                    found[name] = source[match.start():i + 1]
                    break
            i += 1
        else:
            raise ValueError(f"Unclosed generated function body: {name}")
    return found


def verify_generation_outputs(generation, directory):
    outputs = generation.get("outputSha256")
    if not isinstance(outputs, dict):
        raise ValueError("Generation report omits output identities")
    for name in ("title-functions.inc", "title-profile.h"):
        expected = outputs.get(name)
        path = directory / name
        if not isinstance(expected, str) or not re.fullmatch(r"[0-9a-fA-F]{64}", expected):
            raise ValueError(f"Generation report omits a valid {name} identity")
        if not path.is_file() or sha_file(path, 64 * 1024 * 1024) != expected:
            raise ValueError(f"Generated output identity differs: {name}")


def check_prior_bodies(old_generation, new_generation, old_bodies, new_bodies, allowed_replacements=()):
    old_targets = {address_value(value) for value in old_generation.get("untranslatedDirectTargets", [])}
    new_entries = {address_value(row["entry"]) for row in new_generation.get("functions", [])}
    replaceable = {f"ff_title_{value:08X}" for value in old_targets & new_entries & set(allowed_replacements)
                   if f"ff_title_{value:08X}" in old_bodies and f"ff_title_{value:08X}" in new_bodies}
    retained = set(old_bodies) & set(new_bodies) - replaceable
    if set(old_bodies) - set(new_bodies) or any(old_bodies[name] != new_bodies[name] for name in retained):
        raise ValueError("Previous generated function body changed or disappeared")
    return retained, replaceable


def validate_binding_delta(old, new, allowlisted, profile, xbe):
    before, after = binding_pairs(old), binding_pairs(new)
    added, removed = after - before, before - after
    if removed:
        raise ValueError("Previous kernel binding pairs were removed")
    if added != allowlisted:
        raise ValueError("Kernel binding delta must exactly equal the explicit reviewed allow-list")
    imports = {address_value(row["address"]): int(row["ordinal"]) for row in profile["profile"]["imports"]}
    sections = profile["profile"].get("sections", [])
    for slot, ordinal in added:
        if imports.get(slot) != ordinal:
            raise ValueError("Allow-listed binding does not match a slot/ordinal in the XBE profile")
        token = None
        for section in sections:
            delta = slot - address_value(section["address"])
            if 0 <= delta and delta + 4 <= int(section["fileBytes"]):
                offset = address_value(section["fileOffset"]) + delta
                if offset >= 0 and offset + 4 <= len(xbe):
                    token = int.from_bytes(xbe[offset:offset + 4], "little")
                    break
        if token != (0x80000000 | ordinal):
            raise ValueError("Allow-listed binding does not match the original XBE thunk word")
    return added


def generation_args(generation, profile_path, xbe_path, seed_paths, attempt):
    args = [sys.executable, str(ROOT / "translator" / "prepare-title-recomp.py"),
            "--xbe", str(xbe_path), "--profile", str(profile_path),
            "--function-limit", str(generation["functionLimit"]),
            "--instruction-limit", str(generation["instructionLimit"]), "--attempt", attempt]
    memory = generation.get("checkedMemoryAccesses")
    timestamp = generation.get("timestampCounterGuards")
    views = generation.get("checkedDirectEntryViews")
    if not isinstance(memory, list) or not isinstance(timestamp, list) or not isinstance(views, list):
        raise ValueError("Previous generation omits guard configuration evidence")
    if memory:
        args.append("--guard-memory")
    if timestamp:
        if not memory:
            raise ValueError("Timestamp guard is recorded without checked memory")
        args.append("--guard-timestamp-counter")
    known_sites = {0x001E4DC2, 0x001E515D, 0x001E561D}
    view_sites = {int(row["site"]) for row in views}
    if len(view_sites) != len(views) or not view_sites <= known_sites:
        raise ValueError("Previous generation has unknown/duplicate CRT view flags")
    for site, flag in ((0x001E4DC2, "--crt-cleanup-entry"), (0x001E515D, "--crt-seek-cleanup-entry"),
                       (0x001E561D, "--crt-write-cleanup-entry")):
        if site in view_sites:
            args.append(flag)
    for path in seed_paths:
        args.extend(("--seed", str(path)))
    return args


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--previous-generation", required=True, type=Path)
    parser.add_argument("--seed-index", required=True, type=Path)
    parser.add_argument("--addition-seed", required=True, type=Path)
    parser.add_argument("--reached-report", required=True, type=Path)
    parser.add_argument("--xbe", required=True, type=Path)
    parser.add_argument("--metadata", required=True, type=Path, help="Existing static-execution-dependencies profile")
    parser.add_argument("--previous-module", required=True, type=Path, help="Exact module used by the reached report")
    parser.add_argument("--toolkit", type=Path, help="Optional explicit pinned toolkit checkout")
    parser.add_argument("--attempt", required=True)
    parser.add_argument("--output-dir", required=True, type=Path, help="Fresh artifact directory inside research/verification")
    parser.add_argument("--allow-new-binding", action="append", default=[], metavar="SLOT:ORDINAL")
    args = parser.parse_args(argv)
    try:
        if not re.fullmatch(r"[A-Za-z0-9_-]{1,48}", args.attempt):
            raise ValueError("Attempt must be a simple label")
        paths = [research_path(p) for p in (args.previous_generation, args.seed_index, args.addition_seed,
                                            args.reached_report, args.metadata)]
        old_path, index_path, addition_path, report_path, metadata_path = paths
        xbe_path = research_path(args.xbe)
        previous_module = research_path(args.previous_module)
        output = private_path(args.output_dir, must_exist=False)
        if output.exists():
            raise ValueError("Output directory already exists; preserving prior evidence")
        if output == old_path.parent or output in old_path.parents:
            raise ValueError("Output directory may not contain or overwrite prior generation")
        old, old_bytes = read_json(old_path)
        index, _ = read_json(index_path)
        generation_index = load_seed_index(index, index_path, old, old_bytes)
        addition, addition_bytes = read_json(addition_path)
        reached, _ = read_json(report_path, 16 * 1024 * 1024)
        profile, _ = read_json(metadata_path, 1024 * 1024)
        if profile.get("kind") != "static-execution-dependencies":
            raise ValueError("Metadata is not a static execution dependency profile")
        xbe_hash, _ = identity(old, xbe_path, metadata_path)
        selected_group, merged = pack_addition(generation_index, addition, old)
        validate_reached(reached, xbe_hash, xbe_path.stat().st_size, addition)
        if previous_module.stat().st_size > 64 * 1024 * 1024:
            raise ValueError("Previous module exceeds private input bound")
        module_hash = sha_file(previous_module, 64 * 1024 * 1024)
        if module_hash != reached.get("moduleSha256"):
            raise ValueError("Selected previous module identity differs from reached report")
        allow = parse_allowlist(args.allow_new_binding)
        # Check supplied permissions against XBE metadata up front. Actual generated
        # binding deltas are checked below; prepare-title-recomp also checks original bytes.
        imports = {(address_value(row["address"]), int(row["ordinal"])) for row in profile["profile"]["imports"]}
        if not allow <= imports:
            raise ValueError("New binding allow-list is not present in XBE metadata")
        if (old.get("functionLimit") not in (64, 128, 256, 512) or
                old.get("instructionLimit") not in (8192, 16384, 32768) or
                old.get("exportLimit", 0) > 32 or old.get("functionsPerExport", 0) > 32):
            raise ValueError("Previous generation exceeds fixed admission ceilings")
        if len(old.get("seedSha256s", [])) != old.get("exportLimit") or old.get("exportLimit", 0) > 32:
            raise ValueError("Previous generation export count/cap metadata differs")
        if len(old.get("functions", [])) + len(addition.get("functions", [])) > old["functionLimit"]:
            raise ValueError("Addition exceeds existing aggregate function budget")
        if args.toolkit:
            toolkit = Path(args.toolkit).resolve(strict=True)
            pin = json.loads((ROOT / "tools/original-execution-toolchain.json").read_text())["nativeRecompProbe"]["source"]
            if toolkit != (ROOT / ".tools" / "emulation" / pin["directory"]).resolve(strict=True):
                raise ValueError("Toolkit path differs from pinned private checkout")
        merged_path = output / f"merged-export-{selected_group + 1:02d}.json"
        output.parent.mkdir(parents=True, exist_ok=True)
        output.mkdir()
        with merged_path.open("x", encoding="utf-8", newline="\n") as stream:
            json.dump(merged, stream, separators=(",", ":"))
        seed_paths = [p for p, _, _ in generation_index]
        seed_paths[selected_group] = merged_path
        command = generation_args(old, metadata_path, xbe_path, seed_paths, args.attempt)
        with (output / "prepare.log").open("x", encoding="utf-8") as stream:
            proc = subprocess.run(command, cwd=ROOT, stdout=stream, stderr=subprocess.STDOUT)
        if proc.returncode:
            raise subprocess.CalledProcessError(proc.returncode, command)
        generated_path = VERIFY / ("recomp-title-" + args.attempt)
        new, new_bytes = read_json(generated_path / "generation.json")
        if new.get("sourceSha256") != old.get("sourceSha256") or new.get("metadataSha256") != old.get("metadataSha256"):
            raise ValueError("Generated source identities differ from base generation")
        if new.get("toolkitCommit") != TOOLKIT_COMMIT:
            raise ValueError("Generated toolkit commit differs from pin")
        added = validate_binding_delta(old, new, allow, profile, xbe_path.read_bytes())
        verify_generation_outputs(old, old_path.parent)
        verify_generation_outputs(new, generated_path)
        old_inc = (old_path.parent / "title-functions.inc").read_text(encoding="utf-8")
        new_inc = (generated_path / "title-functions.inc").read_text(encoding="utf-8")
        old_bodies, new_bodies = function_definitions(old_inc), function_definitions(new_inc)
        if (len(old_bodies) != len(old.get("functions", [])) + len(old.get("checkedDirectEntryViews", [])) +
                len(old.get("untranslatedDirectTargets", [])) or
                len(new_bodies) != len(new.get("functions", [])) + len(new.get("checkedDirectEntryViews", [])) +
                len(new.get("untranslatedDirectTargets", []))):
            raise ValueError("Generated-body parser did not identify every declared body")
        reached_target = reached["report"]["stop"]["target"]
        retained, replaceable = check_prior_bodies(old, new, old_bodies, new_bodies, {reached_target})
        audit = {"kind": "recompilation-batch-extension-audit", "status": "PREPARED_STATIC_ONLY",
                 "baseGenerationSha256": sha(old_bytes), "newGenerationSha256": sha(new_bytes),
                 "sourceSha256": xbe_hash, "metadataSha256": sha_file(metadata_path, 1024 * 1024),
                 "reachedReportSha256": sha(report_path.read_bytes()), "moduleSha256": reached["moduleSha256"],
                 "previousModuleSha256": module_hash,
                 "additionSeedSha256": sha(addition_bytes), "functionCountBefore": len(old.get("functions", [])),
                 "functionCountAfter": len(new.get("functions", [])), "instructionCountBefore": old.get("instructions"),
                 "instructionCountAfter": new.get("instructions"), "generatedBodiesBefore": len(old_bodies),
                 "generatedBodiesAfter": len(new_bodies), "priorBodiesRetained": len(retained),
                 "kernelBindingsBefore": len(binding_pairs(old)), "kernelBindingsAfter": len(binding_pairs(new)),
                 "addedKernelBindings": [{"slot": f"0x{s:08X}", "ordinal": o} for s, o in sorted(added)],
                 "additionExportGroup": selected_group + 1,
                 "exportCount": new.get("exportLimit"), "functionLimit": new.get("functionLimit"),
                 "instructionLimit": new.get("instructionLimit"), "originalCodeExecuted": False,
                 "browserReplayed": False}
        with (output / "audit.json").open("x", encoding="utf-8") as stream:
            json.dump(audit, stream, indent=2)
        print(json.dumps({"output": str(output.relative_to(ROOT)), "generation": str(generated_path.relative_to(ROOT)),
                          "auditSha256": sha((output / "audit.json").read_bytes()), "originalCodeExecuted": False}))
    except (OSError, KeyError, TypeError, ValueError, json.JSONDecodeError, subprocess.CalledProcessError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
