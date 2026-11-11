#!/usr/bin/env python3
"""Emit the SDK's host-binding header from its existing TOML configuration.

This follows LoadConfig and CodeGen::Run in the pinned XboxXBERecomp SDK.
It needs no XBE and performs no new symbol discovery. Output belongs in an
ignored build directory beside the retained generated title sources.
Upstream attribution/license: runtime/browser/sdk/SDK-NOTICE.txt.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import tomllib


def load_config(path: Path, xdk: Path, active: tuple[Path, ...] = ()) -> dict:
    path = path.resolve()
    if path in active:
        raise ValueError(f"configuration include cycle: {path}")
    with path.open("rb") as stream:
        source = tomllib.load(stream)
    merged = {key: {} for key in ("xbe", "symbols", "data", "layout", "abi")}
    dependencies = []
    if "xdk" in source.get("xbe", {}):
        dependencies.append(xdk / str(source["xbe"]["xdk"]) / "layout.toml")
    dependencies.extend(path.parent / name for name in source.get("includes", []))
    for dependency in dependencies:
        inherited = load_config(dependency, xdk, (*active, path))
        for key in merged:
            merged[key].update(inherited[key])
    for key in merged:
        merged[key].update(source.get(key, {}))
    return merged


def emit(config: dict) -> str:
    image = Path(config["xbe"]["path"]).name
    lines = ["#pragma once", f"#define IMAGE_XBE {json.dumps(image)}"]
    missing = 0
    for section in ("symbols", "data"):
        for name, address in sorted(config[section].items()):
            if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
                raise ValueError(f"invalid symbol name: {name}")
            if type(address) is not int or not 0 <= address <= 0xFFFFFFFF:
                raise ValueError(f"invalid guest address for {name}")
            emitted = address
            if section == "symbols" and not address:
                if missing >= 0x10000:
                    raise ValueError("missing-symbol placeholder range exhausted")
                emitted = 0xFFFF0000 + missing
                missing += 1
            lines.extend((f"#define SYM_{name} {emitted:08X}", f"#define HAS_{name} {int(bool(address))}"))
    for name, value in sorted(config["layout"].items()):
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name) or type(value) is not int:
            raise ValueError(f"invalid layout entry: {name}")
        lines.append(f"#define XDK_{name} 0x{value & 0xFFFFFFFF:X}")
    if config["abi"]:
        rows = []
        for name, locations in sorted(config["abi"].items()):
            if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name) or not isinstance(locations, list):
                raise ValueError(f"invalid ABI entry: {name}")
            if len(locations) > 16 or any(location not in ("stack", "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp") for location in locations):
                raise ValueError(f"invalid argument locations for {name}")
            registers = [location for location in locations if location != "stack"]
            if len(registers) != len(set(registers)):
                raise ValueError(f"repeated argument register for {name}")
            rows.append("\tX(" + ", ".join([name, *locations]) + ")")
        lines.append("#define IMAGE_ABI(X) \\\n" + " \\\n".join(rows))
    return "\n".join(lines) + "\n"


def main() -> None:
    root = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path, default=root / ".tools/fuzion-native/config/game.toml")
    parser.add_argument("--xdk", type=Path, default=root / ".tools/XboxXBERecomp/xdk")
    parser.add_argument("--output", type=Path, default=root / ".tools/artifacts/sdk-browser-host/image_symbols.h")
    args = parser.parse_args()
    header = emit(load_config(args.config, args.xdk))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(header, encoding="utf-8")
    print(f"Wrote host bindings from existing SDK configuration: {args.output}")


if __name__ == "__main__":
    main()
