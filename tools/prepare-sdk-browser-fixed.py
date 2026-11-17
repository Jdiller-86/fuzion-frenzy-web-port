#!/usr/bin/env python3
"""Extract pinned SDK fixed-function constant builders into ignored build output."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


FUNCTIONS = (
    "Bits",
    "Color",
    "MatMul",
    "Transpose",
    "InverseTranspose3x3",
    "FillTransformConstants",
    "LightToView",
    "FillLightConstants",
)
TRANSFORM_CONSTANTS = ("kTsView", "kTsProjection", "kTsTexture0", "kTsWorld")


def _unique(pattern: str, source: str, label: str) -> re.Match[str]:
    matches = list(re.finditer(pattern, source, re.MULTILINE))
    if len(matches) != 1:
        raise ValueError(f"expected exactly one {label} anchor, found {len(matches)}")
    return matches[0]


def _skip_quoted(source: str, at: int) -> int:
    quote = source[at]
    i = at + 1
    while i < len(source):
        if source[i] == "\\":
            i += 2
        elif source[i] == quote:
            return i + 1
        else:
            i += 1
    raise ValueError("unterminated quoted literal while scanning C++ source")


def _code_char(source: str, start: int, wanted: str) -> int:
    i = start
    while i < len(source):
        if source.startswith("//", i):
            newline = source.find("\n", i + 2)
            i = len(source) if newline < 0 else newline + 1
        elif source.startswith("/*", i):
            close = source.find("*/", i + 2)
            if close < 0:
                raise ValueError("unterminated C++ block comment")
            i = close + 2
        elif source.startswith('R"', i):
            delimiter_end = source.find("(", i + 2)
            if delimiter_end < 0:
                raise ValueError("malformed C++ raw string")
            delimiter = source[i + 2 : delimiter_end]
            close = source.find(")" + delimiter + '"', delimiter_end + 1)
            if close < 0:
                raise ValueError("unterminated C++ raw string")
            i = close + len(delimiter) + 2
        elif source[i] in "\"'":
            i = _skip_quoted(source, i)
        elif source[i] == wanted:
            return i
        else:
            i += 1
    raise ValueError(f"missing {wanted!r} in C++ source")


def _extract_braced(source: str, anchor: re.Match[str], label: str) -> str:
    opening = _code_char(source, anchor.end(), "{")
    depth = 0
    i = opening
    while i < len(source):
        if source.startswith("//", i):
            newline = source.find("\n", i + 2)
            i = len(source) if newline < 0 else newline + 1
        elif source.startswith("/*", i):
            close = source.find("*/", i + 2)
            if close < 0:
                raise ValueError(f"unterminated comment in {label}")
            i = close + 2
        elif source.startswith('R"', i):
            delimiter_end = source.find("(", i + 2)
            if delimiter_end < 0:
                raise ValueError(f"malformed raw string in {label}")
            delimiter = source[i + 2 : delimiter_end]
            close = source.find(")" + delimiter + '"', delimiter_end + 1)
            if close < 0:
                raise ValueError(f"unterminated raw string in {label}")
            i = close + len(delimiter) + 2
        elif source[i] in "\"'":
            i = _skip_quoted(source, i)
        elif source[i] == "{":
            depth += 1
            i += 1
        elif source[i] == "}":
            depth -= 1
            i += 1
            if depth == 0:
                return source[anchor.start() : i].strip()
        else:
            i += 1
    raise ValueError(f"unterminated braced definition: {label}")


def _definition(source: str, name: str) -> str:
    # The pinned SDK functions are free-standing float/void definitions. Anchor
    # the return type and name at line start, then let the brace scanner handle
    # their full parameter lists and bodies.
    anchor = _unique(rf"^[ \t]*(?:float|void)[ \t]+{re.escape(name)}[ \t]*\(", source, f"function {name}")
    return _extract_braced(source, anchor, f"function {name}")


def _struct(source: str, name: str) -> str:
    anchor = _unique(rf"^[ \t]*struct[ \t]+{re.escape(name)}\b", source, f"struct {name}")
    return _extract_braced(source, anchor, f"struct {name}") + ";"


def _constants(source: str) -> list[str]:
    names = (*TRANSFORM_CONSTANTS, "kLightFloats")
    pattern = re.compile(r"^[ \t]*constexpr[ \t]+(?:size_t|int)[ \t]+(k(?:Ts\w+|Light\w+))[ \t]*=[^;]+;[^\r\n]*$", re.MULTILINE)
    matches = list(pattern.finditer(source))
    by_name: dict[str, list[re.Match[str]]] = {}
    for match in matches:
        by_name.setdefault(match.group(1), []).append(match)
    required = set(names) | {
        "kLightDiffuse", "kLightSpecular", "kLightAmbient", "kLightPosition", "kLightDirection",
        "kLightRange", "kLightFalloff", "kLightAttenuation0", "kLightAttenuation1",
        "kLightAttenuation2", "kLightTheta", "kLightPhi",
    }
    for name in sorted(required):
        if len(by_name.get(name, [])) != 1:
            raise ValueError(f"expected exactly one constexpr {name}, found {len(by_name.get(name, []))}")
    # Preserve the SDK's declaration order and text exactly.
    selected = [m for m in matches if m.group(1) in TRANSFORM_CONSTANTS or m.group(1).startswith("kLight")]
    selected_names = [m.group(1) for m in selected]
    if len(selected_names) != len(set(selected_names)) or not required.issubset(selected_names):
        raise ValueError("duplicate or missing transform/light constexpr declarations")
    return [m.group(0).strip() for m in selected]


def prepare_fixed_constants(sdk: Path, output: Path) -> dict[str, object]:
    """Extract exact source definitions to output/fixed-constants.inc and manifest."""
    root = Path(__file__).resolve().parents[1]
    out = output.resolve()
    ignored = ((root / "artifacts").resolve(), (root / ".tools/artifacts").resolve())
    if not any(out.is_relative_to(directory) and out != directory for directory in ignored):
        raise ValueError(f"output must be a subdirectory of ignored build output: {', '.join(map(str, ignored))}")
    sdk_root = sdk.resolve(strict=True)
    source_path = (sdk_root / "runtime/gpu/d3d12.cpp").resolve(strict=True)
    if not source_path.is_relative_to(sdk_root):
        raise ValueError("pinned source path escapes the SDK directory")
    if source_path.is_relative_to(out) or out.is_relative_to(sdk_root):
        raise ValueError("SDK input and generated output must not overlap")
    source_bytes = source_path.read_bytes()
    source = source_bytes.decode("utf-8", errors="strict").replace("\r\n","\n")

    constant_lines = _constants(source)
    pieces = [_struct(source, "GuestExtent"), *constant_lines]
    pieces.extend(_definition(source, name) for name in FUNCTIONS)
    generated = "// Extracted from pinned runtime/gpu/d3d12.cpp; do not edit.\nnamespace ff_fixed {\nusing namespace gpu;\n\n"
    generated += "\n\n".join(pieces) + "\n\n}  // namespace ff_fixed\n"
    generated_bytes = generated.encode("utf-8")
    manifest: dict[str, object] = {
        "schema": "sdk-browser-fixed-constants-v1",
        "source": "runtime/gpu/d3d12.cpp",
        "source_sha256": hashlib.sha256(source_bytes).hexdigest(),
        "normalization": "CRLF to LF in generated copies only",
        "extract": "fixed-constants.inc",
        "extract_sha256": hashlib.sha256(generated_bytes).hexdigest(),
        "definitions": [],
    }
    # Keep the manifest names in the same auditable groups as the emitted code.
    manifest["definitions"] = ["GuestExtent", *[line.split()[2] for line in constant_lines], *FUNCTIONS]

    out.mkdir(parents=True, exist_ok=True)
    inc_path = out / "fixed-constants.inc"
    manifest_path = out / "fixed-constants.manifest.json"
    inc_path.write_bytes(generated_bytes)
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return manifest


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True, help="pinned XboxXBERecomp SDK root")
    parser.add_argument("--output", type=Path, required=True, help="ignored artifacts subdirectory")
    args = parser.parse_args()
    try:
        manifest = prepare_fixed_constants(args.sdk, args.output)
    except (OSError, UnicodeError, ValueError) as error:
        parser.error(str(error))
    print(f"Extracted fixed constants {manifest['extract_sha256']} from {manifest['source_sha256']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
