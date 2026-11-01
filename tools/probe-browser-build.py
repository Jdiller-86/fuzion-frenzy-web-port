"""Pinned, isolated xemu/Emscripten configure-and-build attempt.

Run with Linux Python in the existing WSL environment. Sources/SDK/dependencies
must already be installed locally; this script never acquires game or firmware
files, never activates global tools, and never removes a previous attempt.
"""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys


def browser_host_source(root, source, attempt, commit):
    """Prepare an isolated partial host port; retain original machine/render code."""
    destination = root / ("xemu-browser-source-" + attempt)
    if destination.exists():
        raise FileExistsError("preserve the existing browser source; choose a new label")
    subprocess.run(["git", "-C", str(source), "worktree", "add", "--detach",
                    str(destination), commit], check=True)
    changes = {}

    def replace(relative, before, after):
        path = destination / relative
        text = path.read_text(encoding="utf-8")
        if text.count(before) != 1:
            raise ValueError(f"reviewed source anchor changed: {relative}")
        path.write_text(text.replace(before, after), encoding="utf-8")
        changes[relative] = hashlib.sha256(path.read_bytes()).hexdigest()

    meson = (destination / "meson.build").read_text(encoding="utf-8")
    start = meson.index("curl = dependency('libcurl'")
    end = meson.index("libudev = not_found", start)
    native_curl = meson[start:end]
    replace("meson.build", native_curl,
            "curl = not_found\nif host_os != 'emscripten'\n" +
            "".join("  " + line if line.strip() else line
                    for line in native_curl.splitlines(keepends=True)) + "endif\n")
    replace("util/meson.build", "util_ss.add([curl, files('http.c')])",
            "if host_os == 'emscripten'\n"
            "  util_ss.add(files('browser-http-disabled.c'))\nelse\n"
            "  util_ss.add([curl, files('http.c')])\nendif")
    replace("ui/xui/meson.build", "xemu_ss.add([curl, files('github-issue.cc')])",
            "if host_os == 'emscripten'\n"
            "  xemu_ss.add(files('browser-reporting-disabled.cc'))\nelse\n"
            "  xemu_ss.add([curl, files('github-issue.cc')])\nendif")
    for relative in ["util/browser-http-disabled.c", "ui/xui/browser-reporting-disabled.cc",
                     "util/wasm32-dyncall.js"]:
        content = Path(__file__).with_name("xemu").joinpath(Path(relative).name).read_bytes()
        with (destination / relative).open("xb") as output:
            output.write(content)
        changes[relative] = hashlib.sha256(content).hexdigest()
    callback_library = repr(str(destination / "util/wasm32-dyncall.js"))
    for kind in ["c", "cpp"]:
        replace("configs/meson/emscripten.txt", kind + "_link_args = [",
                kind + "_link_args = ['-sDYNCALLS=1','--js-library'," + callback_library + ",")
    # Reuse the pinned public key map already present; never initialize firmware submodules.
    keymap = destination / "subprojects/keycodemapdb"
    if keymap.is_symlink() or (keymap.exists() and
                              (not keymap.is_dir() or any(keymap.iterdir()))):
        raise FileExistsError("unexpected existing key map in the fresh worktree")
    shutil.copytree(source / "subprojects/keycodemapdb", keymap,
                    dirs_exist_ok=True, ignore=shutil.ignore_patterns(".git"))
    return destination, changes


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--tool-root", required=True)
    parser.add_argument("--attempt", required=True)
    parser.add_argument("--browser-host", action="store_true",
                        help="isolate native HTTP/report services; not a complete browser frontend or renderer")
    args = parser.parse_args()
    root = Path(args.tool_root).resolve(strict=True)
    if re.search(r"[\s:]", str(root)):
        parser.error("QEMU requires a space-free real path; use the verified Windows short path in WSL")
    if not re.fullmatch(r"[a-zA-Z0-9_-]{1,64}", args.attempt):
        parser.error("attempt must be a new simple label")
    manifest = json.loads(Path(__file__).with_name("original-execution-toolchain.json").read_text(encoding="utf-8"))
    for source in manifest["sources"].values():
        directory, expected = source["directory"], source["commit"]
        actual = subprocess.check_output(["git", "-C", str(root / directory), "rev-parse", "HEAD"], text=True).strip()
        if actual != expected:
            parser.error(f"{directory} does not match the reviewed source commit")
    if subprocess.run(["git", "-C", str(root / "xemu-source"), "diff", "HEAD", "--quiet"]).returncode:
        parser.error("xemu source has local changes; review those before probing")
    build = root / ("xemu-wasm-" + args.attempt)
    log = root / ("xemu-wasm-" + args.attempt + ".log")
    report = root / ("xemu-wasm-" + args.attempt + ".json")
    source = root / "xemu-source"
    browser_source = root / ("xemu-browser-source-" + args.attempt)
    if any(p.exists() for p in [build, log, report]) or (args.browser_host and browser_source.exists()):
        parser.error("preserve the existing attempt; choose a new label")
    build.mkdir()
    source_changes = {}
    if args.browser_host:
        source, source_changes = browser_host_source(
            root, source, args.attempt, manifest["sources"]["xemu"]["commit"])
    prefix = root / "wasm-deps"
    (prefix / "lib/pkgconfig").mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env.update({"EM_CONFIG": str(root / "emsdk/.emscripten"),
                "EMSDK": str(root / "emsdk"),
                "EM_CACHE": str(root / "emsdk/upstream/emscripten/cache"),
                "PYTHONPATH": str(root / "python"), "PWD": str(build),
                "PKG_CONFIG": "/usr/bin/pkg-config",
                "PKG_CONFIG_LIBDIR": str(prefix / "lib/pkgconfig"),
                "PKG_CONFIG_PATH": str(prefix / "lib/pkgconfig"),
                "EM_PKG_CONFIG_PATH": str(prefix / "lib/pkgconfig"),
                "CPATH": str(prefix / "include"),
                "LIBRARY_PATH": str(prefix / "lib"),
                "PIP_NO_CACHE_DIR": "1", "PYTHONDONTWRITEBYTECODE": "1"})
    bins = [root / p for p in ["python/ninja/data/bin", "python/bin", "emsdk/upstream/emscripten", "emsdk/node/16.20.0_64bit/bin"]]
    env["PATH"] = os.pathsep.join(map(str, bins)) + os.pathsep + env["PATH"]
    commands = [["emcc", "--version"],
                ["bash", str(source / "configure"),
                 "--target-list=i386-softmmu", "--cpu=wasm32", "--cross-prefix=em",
                 "--cc=emcc", "--cxx=em++", "--enable-tcg-interpreter",
                 "--disable-docs", "--disable-werror", "--disable-download",
                 "--disable-slirp"],
                ["ninja", "-C", str(build), "-j2", "qemu-system-i386"]]
    result = {"schemaVersion": 1, "startedUtc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
              "xemuCommit": manifest["sources"]["xemu"]["commit"],
              "emsdkCommit": manifest["sources"]["emsdk"]["commit"],
              "kind": "cross-build-attempt-only", "commands": [],
              "hostProfile": "native-http-disabled" if args.browser_host else "upstream",
              "sourceDirectory": str(source), "modifiedSourceSha256": source_changes,
              "limitations": ["A successful build is not original-game execution, browser playability or fidelity acceptance.",
                              "Game files and firmware are neither inputs nor outputs of this build probe."]}
    with log.open("x", encoding="utf-8") as output:
        for command in commands:
            output.write("COMMAND " + json.dumps(command) + "\n")
            output.flush()
            completed = subprocess.run(command, cwd=build, env=env, stdout=output, stderr=subprocess.STDOUT)
            output.write(f"EXIT {completed.returncode}\n")
            output.flush()
            result["commands"].append({"argv": command, "exitCode": completed.returncode})
            print(json.dumps({"command": command[0], "exit": completed.returncode}), flush=True)
            if completed.returncode:
                break
    result["status"] = "build-completed" if len(result["commands"]) == 3 and completed.returncode == 0 else "attempt-failed"
    with report.open("x", encoding="utf-8") as output:
        json.dump(result, output, indent=2)
        output.write("\n")
    print(json.dumps({"status": result["status"], "report": str(report), "log": str(log)}))
    return completed.returncode


if __name__ == "__main__":
    sys.exit(main())
