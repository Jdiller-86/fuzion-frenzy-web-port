import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def compiler():
    for name in (os.environ.get("FF_CC"), "cc", "gcc", "clang"):
        if name and shutil.which(name):
            return shutil.which(name)
    return None


def extract_getter():
    source = (ROOT / "tools" / "fixtures" / "thread_startup_probe.c").read_text(encoding="utf-8")
    signature = "uint32_t ff_probe_boundary_stack_word(uint32_t field)"
    start = source.index(signature)
    end = source.index("\nuint32_t ff_probe_binding_count", start)
    function = source[start:end].strip()
    if not function.startswith(signature) or not function.endswith("}"):
        raise AssertionError("boundary getter extraction did not preserve a function")
    return function


@unittest.skipUnless(compiler(), "no C compiler available for boundary stack getter regression")
class BoundaryStackSnapshotTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        getter = extract_getter()
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        harness = r'''#include <assert.h>
#include <stdint.h>
#include <string.h>
static uint32_t g_esp,g_ebp;
static uint32_t g_eax,g_ecx,g_edx,g_ebx,g_esi,g_edi;
static uint8_t guest_ram[32];
''' + getter + r'''
int main(void) {
    const uint32_t vectors[][2]={{0x80000000u,0x7fffffffu},{UINT32_MAX,0u},{0u,UINT32_MAX}};
    for(unsigned n=0;n<3;n++) {
        uint32_t cpu_before[8];uint8_t memory_before[sizeof guest_ram];
        g_esp=vectors[n][0];g_ebp=vectors[n][1];
        g_eax=0x12345678u;g_ecx=0x87654321u;g_edx=0xf0e1d2c3u;
        g_ebx=0x0badc0deu;g_esi=0xfeedfaceu;g_edi=0xc001d00du;
        for(unsigned i=0;i<sizeof guest_ram;i++)guest_ram[i]=(uint8_t)(0x80u+i+n);
        cpu_before[0]=g_esp;cpu_before[1]=g_ebp;cpu_before[2]=g_eax;cpu_before[3]=g_ecx;
        cpu_before[4]=g_edx;cpu_before[5]=g_ebx;cpu_before[6]=g_esi;cpu_before[7]=g_edi;
        memcpy(memory_before,guest_ram,sizeof guest_ram);
        for(unsigned repeat=0;repeat<8;repeat++) {
            assert(ff_probe_boundary_stack_word(0)==vectors[n][0]);
            assert(ff_probe_boundary_stack_word(1)==vectors[n][1]);
            assert(ff_probe_boundary_stack_word(2)==0);
            assert(ff_probe_boundary_stack_word(3)==0);
            assert(ff_probe_boundary_stack_word(UINT32_MAX)==0);
        }
        assert(g_esp==cpu_before[0]&&g_ebp==cpu_before[1]&&g_eax==cpu_before[2]&&
            g_ecx==cpu_before[3]&&g_edx==cpu_before[4]&&g_ebx==cpu_before[5]&&
            g_esi==cpu_before[6]&&g_edi==cpu_before[7]);
        assert(memcmp(guest_ram,memory_before,sizeof guest_ram)==0);
    }
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_extracted_boundary_getter_is_read_only(self):
        executable = self.path / "boundary-stack"
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                str(self.path / "harness.c"), "-o", str(executable)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)


if __name__ == "__main__":
    unittest.main()
