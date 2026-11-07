#!/usr/bin/env python3
"""Run the pinned XboxXBERecomp SDK on one explicitly selected private XBE.

This produces offline generated C++ only. It never builds or executes the game.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path, PureWindowsPath
import platform
import re
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
RESEARCH = ROOT / "research"
VERIFY = RESEARCH / "verification"
SDK_COMMIT = "58757ab255c0a6b3929f405d7a5f1567fcd7fda3"
PORT_COMMIT = "071dbd97bdc098c4647cc24c99f9eae693fde1eb"
CONFIG_SHA256 = "c45bed067026b71ff2415d2b901f5bece66ad54dd94796405ce2cd0c2e1cec2f"
XBE_SHA256 = "45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9"
XBE_SIZE = 2_834_432
TOOL_SHA256 = "553d31fb7f177e6f852708f383d2e745a18ac521f29eaf5d6459a40db9e15f28"
MAX_XBE_BYTES = 8 * 1024 * 1024
MAX_CONFIG_BYTES = 1024 * 1024
MAX_GENERATED_BYTES = 512 * 1024 * 1024
MAX_GENERATED_FILES = 4096


def sha256_file(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def check_pinned_file(path, expected_sha, max_bytes, expected_size=None):
    size = path.stat().st_size
    if size > max_bytes or (expected_size is not None and size != expected_size):
        raise ValueError("Input size differs from pinned bounds")
    digest = sha256_file(path)
    if digest != expected_sha:
        raise ValueError("Input digest differs from pinned identity")
    return digest


def inside(path, parent):
    return path == parent or parent in path.parents


def resolve_private(value, parent, *, suffix=None):
    original = Path(value)
    if original.is_symlink():
        raise ValueError("Input must not be a symbolic link")
    path = Path(value).resolve(strict=True)
    parent = parent.resolve(strict=True)
    if not inside(path, parent):
        raise ValueError(f"Input must remain inside {parent}")
    if suffix and path.suffix.lower() != suffix:
        raise ValueError(f"Expected a {suffix} input")
    if not path.is_file() or path.is_symlink():
        raise ValueError("Input must be a regular non-symlink file")
    return path


def verify_sources(sdk_root, config, xbe, tool):
    sdk_root = Path(sdk_root).resolve(strict=True)
    tools_root = (ROOT / ".tools").resolve(strict=True)
    if not inside(sdk_root, tools_root) or not sdk_root.is_dir():
        raise ValueError("Pinned SDK source must remain inside project .tools")
    head = subprocess.run(["git", "-C", str(sdk_root), "rev-parse", "HEAD"],
                          check=True, capture_output=True, text=True).stdout.strip()
    dirty = subprocess.run(["git", "-C", str(sdk_root), "status", "--porcelain", "--untracked-files=all"],
                           check=True, capture_output=True, text=True).stdout
    if head != SDK_COMMIT or dirty:
        raise ValueError("SDK source is not the clean pinned commit")
    if not (sdk_root / "xdk" / "3925" / "layout.toml").is_file():
        raise ValueError("Pinned SDK is missing XDK 3925 layout")
    config = resolve_private(config, RESEARCH, suffix=".toml")
    xbe = resolve_private(xbe, RESEARCH, suffix=".xbe")
    config_hash = check_pinned_file(config, CONFIG_SHA256, MAX_CONFIG_BYTES)
    xbe_hash = check_pinned_file(xbe, XBE_SHA256, MAX_XBE_BYTES, XBE_SIZE)
    try:
        import tomllib
        parsed = tomllib.loads(config.read_text(encoding="utf-8"))
    except (ImportError, OSError, ValueError) as error:
        raise ValueError(f"Cannot parse pinned native config: {error}") from error
    if parsed.get("xbe", {}).get("xdk") != 3925:
        raise ValueError("Pinned config does not select XDK build 3925")
    tool = resolve_private(tool, VERIFY, suffix=None)
    if sha256_file(tool) != TOOL_SHA256:
        raise ValueError("Translator executable identity differs from the verified local CLI")
    if platform.system() != "Windows" and not os.access(tool, os.X_OK):
        raise ValueError("Translator executable is not executable on this host")
    return sdk_root, config_hash, xbe_hash, head, tool


def windows_path_to_wsl(value):
    """Convert a Windows drive path without invoking a shell parser."""
    windows = PureWindowsPath(str(value))
    if not windows.drive or windows.drive.startswith("\\"):
        raise ValueError("WSL conversion requires a local drive-letter path")
    drive = windows.drive.rstrip(":").lower()
    parts = [part for part in windows.parts[1:] if part not in ("\\", "/")]
    return "/mnt/" + drive + "/" + "/".join(parts)


def wsl_path(path):
    return windows_path_to_wsl(str(Path(path).resolve(strict=False)))


def build_command(*, sdk_root, config, xbe, tool, generated, use_wsl):
    if use_wsl:
        if platform.system() != "Windows":
            raise ValueError("--wsl is only valid on Windows")
        wsl = shutil.which("wsl.exe")
        if not wsl:
            raise ValueError("wsl.exe was not found")
        return [wsl, "--cd", wsl_path(generated.parent), "--exec", wsl_path(tool),
                wsl_path(config), wsl_path(xbe), wsl_path(generated), "--xdk", wsl_path(sdk_root / "xdk")]
    if platform.system() != "Linux":
        raise ValueError("Windows requires explicit --wsl; native execution is Linux-only")
    return [str(tool), str(config), str(xbe), str(generated), "--xdk", str(sdk_root / "xdk")]


def inventory(directory):
    files, total = [], 0
    for path in sorted(directory.rglob("*")):
        if path.is_symlink():
            raise ValueError("Generated output contains a symlink")
        if not path.is_file():
            continue
        size = path.stat().st_size
        total += size
        if len(files) >= MAX_GENERATED_FILES or total > MAX_GENERATED_BYTES:
            raise ValueError("Generated output exceeds fixed inventory bounds")
        files.append({"path": path.relative_to(directory).as_posix(), "bytes": size,
                      "sha256": sha256_file(path)})
    return files, total


def ensure_output_scope(output, generated, log_path):
    for path in output.rglob("*"):
        if path.is_symlink():
            raise ValueError("Attempt output contains a symlink")
        if path == generated or generated in path.parents or path == log_path:
            continue
        raise ValueError("Translator wrote an unexpected path outside its generated directory")


def parse_cli_stats(stats_path):
    values = {}
    for line in stats_path.read_text(encoding="utf-8", errors="strict").splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[1].isdigit() and parts[0] in ("functions", "instructions"):
            values[parts[0]] = int(parts[1])
        elif len(parts) == 3 and parts[0] == "unimplemented" and parts[2].isdigit():
            values.setdefault("unimplemented", {})[parts[1]] = int(parts[2])
    if not {"functions", "instructions", "unimplemented"} <= values.keys():
        raise ValueError("Generated CLI stats file is missing expected counters")
    return values


def prepare(output, *, sdk_root, config, xbe, tool, use_wsl):
    if not re.fullmatch(r"[A-Za-z0-9_-]{1,48}", output.name):
        raise ValueError("Output directory name must be a simple attempt label")
    if Path(output).is_symlink():
        raise ValueError("Output path must not be a symbolic link")
    output = output.resolve(strict=False)
    if not inside(output, VERIFY.resolve(strict=True)) or output == VERIFY.resolve(strict=True):
        raise ValueError("Output must be a new child of private research/verification")
    if output.exists():
        raise ValueError("Output attempt already exists; preserving prior evidence")
    config = resolve_private(config, RESEARCH, suffix=".toml")
    xbe = resolve_private(xbe, RESEARCH, suffix=".xbe")
    sdk_root, config_hash, xbe_hash, sdk_head, tool = verify_sources(sdk_root, config, xbe, tool)
    generated = output / "generated"
    command = build_command(sdk_root=sdk_root, config=config, xbe=xbe, tool=tool,
                            generated=generated, use_wsl=use_wsl)
    output.mkdir(parents=True)
    log_path = output / "translator.log"
    launch_error = None
    try:
        with log_path.open("xb") as log:
            result = subprocess.run(command, cwd=output, stdout=log, stderr=subprocess.STDOUT, check=False)
            exit_code = result.returncode
    except OSError as error:
        launch_error = str(error)
        with log_path.open("ab") as log:
            log.write(("LAUNCH_ERROR: " + launch_error + "\n").encode("utf-8", errors="replace"))
        exit_code = -1
    details = {"kind": "pinned-sdk-static-generation-attempt",
               "sourcePins": {"sdk": sdk_head, "nativePort": PORT_COMMIT,
                              "nativeConfigSha256": config_hash},
               "sdkCommit": sdk_head, "nativePortCommit": PORT_COMMIT,
               "configSha256": config_hash, "configBytes": config.stat().st_size,
               "xbeSha256": xbe_hash, "xbeBytes": xbe.stat().st_size,
               "sdkPath": sdk_root.relative_to(ROOT).as_posix(),
               "configPath": config.relative_to(ROOT).as_posix(), "xbePath": xbe.relative_to(ROOT).as_posix(),
               "translatorPath": tool.relative_to(ROOT).as_posix(),
               "translatorSha256": sha256_file(tool), "command": command,
               "exitCode": exit_code, "launchError": launch_error, "log": "translator.log",
               "logSha256": sha256_file(log_path), "originalCodeExecuted": False,
               "admission": "NOT_ADMITTED_BROWSER"}
    if exit_code != 0:
        details["status"] = "GENERATION_FAILED"
        with (output / "failure.json").open("x", encoding="utf-8") as stream:
            json.dump(details, stream, indent=2)
        raise RuntimeError(f"Translator failed with exit code {exit_code}; log retained at {log_path}")
    try:
        ensure_output_scope(output, generated, log_path)
        files, total_bytes = inventory(generated)
        stats = parse_cli_stats(generated / "x86_recomp_stats.txt")
    except (OSError, ValueError) as error:
        details["status"] = "OUTPUT_VALIDATION_FAILED"
        details["validationError"] = str(error)
        with (output / "failure.json").open("x", encoding="utf-8") as stream:
            json.dump(details, stream, indent=2)
        raise RuntimeError(f"Generated output validation failed; artifacts retained at {output}") from error
    details.update({"status": "GENERATED_STATIC_ONLY", "generatedFileCount": len(files),
                    "generatedBytes": total_bytes,
                    "inventorySha256": hashlib.sha256(json.dumps(files, sort_keys=True,
                                                                    separators=(",", ":")).encode()).hexdigest(),
                    "files": files, "cliStats": stats, "moduleBuilt": False,
                    "browserReplayed": False})
    with (output / "report.json").open("x", encoding="utf-8") as stream:
        json.dump(details, stream, indent=2)
    return details


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk-root", required=True, type=Path)
    parser.add_argument("--config", required=True, type=Path)
    parser.add_argument("--xbe", required=True, type=Path)
    parser.add_argument("--tool", required=True, type=Path, help="Explicit verified xbe_recomp executable")
    parser.add_argument("--output-dir", required=True, type=Path)
    parser.add_argument("--wsl", action="store_true", help="Run the Linux translator through WSL on Windows")
    args = parser.parse_args(argv)
    try:
        result = prepare(args.output_dir, sdk_root=args.sdk_root, config=args.config,
                         xbe=args.xbe, tool=args.tool, use_wsl=args.wsl)
        print(json.dumps({"output": str(args.output_dir), "status": result["status"],
                          "files": result["generatedFileCount"], "bytes": result["generatedBytes"],
                          "admission": result["admission"]}))
    except (OSError, ValueError, subprocess.SubprocessError, RuntimeError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
