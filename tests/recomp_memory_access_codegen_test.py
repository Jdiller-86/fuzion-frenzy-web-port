#!/usr/bin/env python3
"""Generator-to-compiled regression for checked vector access.

The guard emitter is executed for a read and a store, the emitted C is compiled
against the real virtual-memory component, and the compiled program proves:
  * a generated read is structurally non-writable (assigning through it fails to
    compile, so a read instruction can never mutate guest memory),
  * a permitted read and store work through the emitted form,
  * a denied store leaves the guest bytes unchanged (denial before mutation),
  * denied reads fail closed.
No original code, XBE or private profile is used.
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "translator"))
import recomp_memory_access as guard  # noqa: E402

SITE = 0x00100000
LIMIT = 0x00100030
READ_SOURCE = "loc_00100000: ;\nxmm0 = XMM_MEM(eax); /* movaps */"
STORE_SOURCE = "loc_00100000: ;\nXMM_STORE(eax, xmm0); /* movaps */"

HARNESS = r"""
#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "recomp_memory_access.h"
#include "virtual_memory.h"

typedef union { uint8_t u8[16]; uint32_t u32[4]; } RecompXmm;
static uint8_t ram[0x40000];
static ff_virtual_memory vm;
static jmp_buf stop_jump;
static uint32_t stop_address, stop_access, stop_site, stop_count;

/* The host contract for a refused access is to stop the machine, not to return a
 * pointer the translated code would then dereference. This hook longjmps so the
 * test can observe the stop without touching guest memory. */
static _Noreturn void stop(uint32_t address, uint32_t access, uint32_t site)
{
    stop_address = address; stop_access = access; stop_site = site; ++stop_count;
    longjmp(stop_jump, 1);
}

void *ff_probe_guest_memory(uint32_t address, uint64_t bytes, uint32_t access, uint32_t site)
{
    if (!access || (access & ~(uint64_t)7)) stop(address, access, site);
    if (ff_vm_check(&vm, address, bytes, access)) stop(address, access, site);
    return ram + address;
}
void *ff_probe_guest_vector(uint32_t address, uint32_t access, uint32_t site)
{
    if (address & 15u) stop(address, access, site);
    return ff_probe_guest_memory(address, 16u, access, site);
}

static uint32_t eax;
static RecompXmm xmm0;

void generated_read(void)
{
%(read)s
}

void generated_store(void)
{
%(store)s
}

int main(void)
{
    memset(ram, 0, sizeof ram);
    assert(ff_vm_init(&vm, ram, sizeof ram) == 0);
    assert(ff_vm_claim_fixed(&vm, 0x00010000u, 0x00020000u) == 0);
    assert(ff_vm_map_fixed(&vm, 0x00010000u, 0x00010000u, 0x02u) == 0);
    assert(ff_vm_map_fixed(&vm, 0x00020000u, 0x00010000u, 0x04u) == 0);
    for (unsigned i = 0; i < 16; ++i) ram[0x00010000u + i] = (uint8_t)(0x50u + i);

    /* A permitted read through the emitted form returns the guest bytes. */
    eax = 0x00010000u;
    memset(&xmm0, 0, sizeof xmm0);
    generated_read();
    for (unsigned i = 0; i < 16; ++i) assert(xmm0.u8[i] == (uint8_t)(0x50u + i));

    /* A permitted store through the emitted form writes them. */
    for (unsigned i = 0; i < 16; ++i) xmm0.u8[i] = (uint8_t)(0xA0u + i);
    eax = 0x00020000u;
    generated_store();
    for (unsigned i = 0; i < 16; ++i) assert(ram[0x00020000u + i] == (uint8_t)(0xA0u + i));

    /* A generated store into a read-only region reaches the stopping hook with no
     * mutation: the emitted form is executed, not just inspected. */
    uint8_t before[16];
    memcpy(before, ram + 0x00010000u, sizeof before);
    eax = 0x00010000u;
    stop_count = 0;
    if (!setjmp(stop_jump)) {
        generated_store();
        assert(0 && "a denied generated store must stop the machine");
    }
    assert(stop_count == 1 && stop_address == 0x00010000u && stop_access == 2);
    assert(stop_site == 0x00100000u);
    assert(memcmp(before, ram + 0x00010000u, sizeof before) == 0);

    /* A generated read of an unmapped page stops the same way. */
    eax = 0x00030000u;
    stop_count = 0;
    if (!setjmp(stop_jump)) {
        generated_read();
        assert(0 && "a denied generated read must stop the machine");
    }
    assert(stop_count == 1 && stop_address == 0x00030000u && stop_access == 1);

    puts("generated vector ops: permitted read/store, denied ops stop before mutation");
    return 0;
}
"""


def compiler():
    for name in (os.environ.get("FF_CC"), "cc", "gcc", "clang"):
        if name and shutil.which(name):
            return shutil.which(name)
    return None


def body(text):
    """Strip the generated basic-block label and trailing mnemonic comment so the
    emitted expression can be compiled inside a test function."""
    lines = [line for line in text.splitlines() if not line.strip().startswith("loc_")]
    stripped = "\n".join(lines).strip()
    return re.sub(r"\s*/\*.*?\*/\s*$", "", stripped)


def right_hand_side(text):
    """The checked lvalue a generated read produces, without its assignment."""
    expression = body(text)
    if "=" in expression:
        expression = expression.split("=", 1)[1]
    return expression.strip().rstrip(";").strip()


@unittest.skipUnless(compiler(), "no C compiler available for the compiled regression")
class CompiledVectorAccessTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        read, _ = guard.instrument_memory_accesses(READ_SOURCE, SITE, LIMIT)
        store, _ = guard.instrument_memory_accesses(STORE_SOURCE, SITE, LIMIT)
        cls.read, cls.store = read, store
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        (cls.path / "harness.c").write_text(
            HARNESS % {"read": body(read), "store": body(store)}, encoding="utf-8", newline="\n")
        (cls.path / "writable.c").write_text(
            '#include "recomp_memory_access.h"\n'
            "typedef union { uint8_t u8[16]; } RecompXmm;\n"
            "void *ff_probe_guest_memory(uint32_t, uint64_t, uint32_t, uint32_t);\n"
            "void *ff_probe_guest_vector(uint32_t, uint32_t, uint32_t);\n"
            "uint32_t eax; RecompXmm xmm0;\n"
            "void f(void) { " + right_hand_side(read) + " = xmm0; }\n", encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def command(self, source, output):
        return [compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                "-I", str(ROOT / "runtime" / "core"), str(self.path / source),
                str(ROOT / "runtime" / "core" / "virtual_memory.c"),
                str(ROOT / "runtime" / "core" / "thread_bootstrap.c"), "-o", str(self.path / output)]

    def test_generated_read_is_not_assignable(self):
        self.assertIn("(*(const RecompXmm*)", self.read)
        self.assertNotIn("(*(const RecompXmm*)", self.store)
        result = subprocess.run(self.command("writable.c", "writable"), capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0, "a store through a generated read must not compile")
        self.assertIn("read-only", (result.stderr or "") + (result.stdout or ""))

    def test_compiled_generated_access_denies_before_mutation(self):
        build = subprocess.run(self.command("harness.c", "harness"), capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(self.path / "harness")], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertIn("denied ops stop before mutation", run.stdout)


if __name__ == "__main__":
    unittest.main()
