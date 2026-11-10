#!/usr/bin/env python3
"""Set up an experimental, user-local Emscripten toolchain.

Usage (from the repository root):
    python3 tools/setup-wasm-toolchain.py
    source .tools/emscripten-env.sh

This installs Debian's Emscripten 3.1.69+dfsg-3, LLVM/LLD 19.1.7, and
Binaryen 120 under ignored .tools/. The project's pinned 3.1.50 SDK download
was blocked at storage.googleapis.com; this package route is experimental.
Downloads use the inherited urllib proxy and CA configuration. No system
packages, repositories, or proxy settings are changed.

This fallback targets Debian 13 x86-64 with its system libllvm19 and normal
LLVM shared-library dependencies already installed, as in the managed workspace.
It is not a Windows installer or a standalone cross-distribution LLVM bundle.
"""

from __future__ import annotations

import base64
import hashlib
import io
import os
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import sys
import tarfile
import urllib.request


ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / ".tools"
PACKAGE_DIR = TOOLS / "debpkgs"
DEBROOT = TOOLS / "debroot"
EMSCRIPTEN_DIR = DEBROOT / "usr/share/emscripten"
EMSCRIPTEN_VERSION = "3.1.69+dfsg-3"
LLVM_VERSION = "19.1.7-3+b1"
ACORN_VERSION = "8.19.0"

# SHA-256 values are from the retained .deb downloads used for this setup.
DEBIAN_PACKAGES = (
    (
        "https://deb.debian.org/debian/pool/main/e/emscripten/"
        "emscripten_3.1.69+dfsg-3_all.deb",
        "emscripten_3.1.69+dfsg-3_all.deb",
        "d2ac788b952c56ab6736b4db2ac0825aa63cfb4c08b8abaefe19e324779d1b2d",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "clang-19_19.1.7-3+b1_amd64.deb",
        "clang-19_19.1.7-3+b1_amd64.deb",
        "bbb74458d782b7792b61b0eed92746df5e6d033778c045cf8c2b3835b7c4efc4",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "lld-19_19.1.7-3+b1_amd64.deb",
        "lld-19_19.1.7-3+b1_amd64.deb",
        "756a805e8811d527dd2b5417f016c9342b8631fa9c1ddcd56265c13ae33bf3a4",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "llvm-19_19.1.7-3+b1_amd64.deb",
        "llvm-19_19.1.7-3+b1_amd64.deb",
        "0b1b7263f4f45e261099240fdbf00fc14b42b88410fd829e041bf83ea45866cb",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "libclang-cpp19_19.1.7-3+b1_amd64.deb",
        "libclang-cpp19_19.1.7-3+b1_amd64.deb",
        "a7d2b6df51050834f6ea5c0d5e34869c2298b0bcdaa91eff17ba3db2cd545c47",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "libclang1-19_19.1.7-3+b1_amd64.deb",
        "libclang1-19_19.1.7-3+b1_amd64.deb",
        "4e7e2a5fe236e280d32fb529350b3c1cfd9410a964629a57ea079a314c45d7a3",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "libclang-common-19-dev_19.1.7-3+b1_amd64.deb",
        "libclang-common-19-dev_19.1.7-3+b1_amd64.deb",
        "4018e40e2d4144fe38c45822e68cca63c1bf6c7872c3073ebb262ad4b2f9b8bc",
    ),
    (
        "https://deb.debian.org/debian/pool/main/l/llvm-toolchain-19/"
        "llvm-19-linker-tools_19.1.7-3+b1_amd64.deb",
        "llvm-19-linker-tools_19.1.7-3+b1_amd64.deb",
        "874a0a5cde797cda3e94e9310a0a9958608f526f177637414a8ce9620c804818",
    ),
    (
        "https://deb.debian.org/debian/pool/main/b/binaryen/"
        "binaryen_120-4_amd64.deb",
        "binaryen_120-4_amd64.deb",
        "47be56009e56170f4338a0154fbff2028bd607bc3c8b6a5414658a985bef6470",
    ),
)

ACORN_URL = "https://registry.npmjs.org/acorn/-/acorn-8.19.0.tgz"
ACORN_SHA512 = (
    "oJlA3XiRm7Cyk6qFD2Jn8ak9B7jSy0qy00ADO3+8dpT0LSjFihQYv4C02LFCSYJV"
    "3Q37xYwwRy5m+IpIiUzqWw=="
)


def download(url: str, destination: Path, algorithm: str, expected: str) -> None:
    def digest_file(path: Path) -> str:
        digest = hashlib.new(algorithm)
        with path.open("rb") as source:
            while chunk := source.read(1024 * 1024):
                digest.update(chunk)
        if algorithm == "sha512":
            return base64.b64encode(digest.digest()).decode("ascii")
        return digest.hexdigest()

    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.is_file():
        actual = digest_file(destination)
        if actual == expected:
            return
        destination.unlink()

    digest = hashlib.new(algorithm)
    temporary = destination.with_suffix(destination.suffix + ".part")
    try:
        with urllib.request.urlopen(url, timeout=120) as response, temporary.open("wb") as out:
            while chunk := response.read(1024 * 1024):
                digest.update(chunk)
                out.write(chunk)
        actual = (
            base64.b64encode(digest.digest()).decode("ascii")
            if algorithm == "sha512"
            else digest.hexdigest()
        )
        matches = actual == expected if algorithm == "sha512" else actual.lower() == expected.lower()
        if not matches:
            raise RuntimeError(f"checksum mismatch for {url}")
        temporary.replace(destination)
    finally:
        temporary.unlink(missing_ok=True)


def extract_acorn(archive_bytes: bytes, destination: Path) -> None:
    staging = destination.with_name("acorn-extract")
    shutil.rmtree(staging, ignore_errors=True)
    staging.mkdir(parents=True)
    with tarfile.open(fileobj=io.BytesIO(archive_bytes), mode="r:gz") as archive:
        for member in archive.getmembers():
            path = PurePosixPath(member.name)
            if not path.parts or path.parts[0] != "package":
                raise RuntimeError(f"unexpected path in npm tarball: {member.name}")
            relative = Path(*path.parts[1:])
            if relative.is_absolute() or ".." in relative.parts:
                raise RuntimeError(f"unsafe path in npm tarball: {member.name}")
            target = staging / relative
            if member.isdir():
                target.mkdir(parents=True, exist_ok=True)
            elif member.isfile():
                target.parent.mkdir(parents=True, exist_ok=True)
                source = archive.extractfile(member)
                if source is None:
                    raise RuntimeError(f"cannot read npm tarball entry: {member.name}")
                with source, target.open("wb") as out:
                    shutil.copyfileobj(source, out)
            else:
                raise RuntimeError(f"unsupported entry in npm tarball: {member.name}")

    shutil.rmtree(destination, ignore_errors=True)
    staging.replace(destination)


def write_local_config(node_path: str) -> None:
    config_path = TOOLS / "emscripten-local.config"
    config_path.write_text(
        "import os\n"
        f"ROOT = {str(DEBROOT)!r}\n"
        "LLVM_ROOT = os.path.join(ROOT, 'usr/lib/llvm-19/bin')\n"
        "BINARYEN_ROOT = os.path.join(ROOT, 'usr')\n"
        f"NODE_JS = {node_path!r}\n"
        f"CACHE = {str(TOOLS / 'emscripten-cache')!r}\n"
        "FROZEN_CACHE = False\n"
        "LLVM_ADD_VERSION = '19'\n"
        "CLANG_ADD_VERSION = '19'\n"
    )

    env_path = TOOLS / "emscripten-env.sh"
    env_path.write_text(
        "# Source from the repository root to use the local Debian Emscripten toolchain.\n"
        "_TOOLCHAIN_ROOT=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\n"
        "export EM_CONFIG=\"$_TOOLCHAIN_ROOT/emscripten-local.config\"\n"
        "export PYTHON=\"$(command -v python3)\"\n"
        "export LD_LIBRARY_PATH=\"$_TOOLCHAIN_ROOT/debroot/usr/lib/x86_64-linux-gnu:"
        "$_TOOLCHAIN_ROOT/debroot/usr/lib/llvm-19/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}\"\n"
        "export PATH=\"$_TOOLCHAIN_ROOT/debroot/usr/share/emscripten:"
        "$_TOOLCHAIN_ROOT/debroot/usr/share/emscripten/node_modules/.bin:"
        "$_TOOLCHAIN_ROOT/debroot/usr/bin:"
        "$_TOOLCHAIN_ROOT/debroot/usr/lib/llvm-19/bin:$PATH\"\n"
        "unset _TOOLCHAIN_ROOT\n"
    )


def main() -> None:
    node = shutil.which("node")
    if node is None:
        raise RuntimeError("Node.js is required and must be on PATH")
    dpkg_deb = shutil.which("dpkg-deb")
    if dpkg_deb is None:
        raise RuntimeError("dpkg-deb is required to extract the local Debian packages")

    TOOLS.mkdir(parents=True, exist_ok=True)
    for url, filename, sha256 in DEBIAN_PACKAGES:
        deb = PACKAGE_DIR / filename
        download(url, deb, "sha256", sha256)
        subprocess.run([dpkg_deb, "-x", str(deb), str(DEBROOT)], check=True)

    llvm_bin = DEBROOT / "usr/lib/llvm-19/bin"
    aliases = (
        "clang++-19:clang++",
        "wasm-ld-19:wasm-ld",
        "llvm-ar-19:llvm-ar",
        "llvm-nm-19:llvm-nm",
        "llvm-ranlib-19:llvm-ranlib",
        "llvm-objcopy-19:llvm-objcopy",
        "llvm-readobj-19:llvm-readobj",
        "llvm-strip-19:llvm-strip",
        "llvm-link-19:llvm-link",
        "llvm-rc-19:llvm-rc",
    )
    for alias in aliases:
        name, target = alias.split(":", 1)
        link = llvm_bin / name
        if link.exists() or link.is_symlink():
            link.unlink()
        link.symlink_to(target)

    acorn_tarball = PACKAGE_DIR / f"acorn-{ACORN_VERSION}.tgz"
    download(ACORN_URL, acorn_tarball, "sha512", ACORN_SHA512)
    extract_acorn(acorn_tarball.read_bytes(), EMSCRIPTEN_DIR / "node_modules/acorn")
    write_local_config(node)

    print(f"Prepared experimental Emscripten {EMSCRIPTEN_VERSION} under {TOOLS}.")
    print("Use: source .tools/emscripten-env.sh")
    print("This is not the pinned SDK version; it uses Debian Emscripten 3.1.69.")


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f"setup-wasm-toolchain: {error}", file=sys.stderr)
        raise SystemExit(1)
