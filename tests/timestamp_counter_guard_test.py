import unittest
import sys
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from translator.timestamp_counter_guard import guard_timestamp_reads

ROOT = Path(__file__).resolve().parents[1]


def row(address, *, length=2, opcode="0f31", mnemonic="RDTSC"):
    return {"address": address, "length": length, "analysisBytesHex": opcode,
            "mnemonic": mnemonic}


class TimestampCounterGuardTests(unittest.TestCase):
    def test_no_sites_preserves_legacy_text(self):
        source = "int x = 1;\n"
        self.assertEqual(guard_timestamp_reads(source, [row("10", mnemonic="MOV")]), (source, []))

    def test_one_site(self):
        source = "a(); xbox_ReadTimeStampCounter(); z();"
        self.assertEqual(guard_timestamp_reads(source, [row("00001234")]),
                         ("a(); ff_probe_read_tsc(0x00001234u); z();", [0x1234]))

    def test_multiple_sites_keep_order(self):
        source = "xbox_ReadTimeStampCounter() + xbox_ReadTimeStampCounter()"
        result, sites = guard_timestamp_reads(source, [row("10"), row("20")])
        self.assertEqual(result, "ff_probe_read_tsc(0x00000010u) + ff_probe_read_tsc(0x00000020u)")
        self.assertEqual(sites, [0x10, 0x20])

    def test_rejects_reversed_sites(self):
        with self.assertRaises(ValueError):
            guard_timestamp_reads("xbox_ReadTimeStampCounter()" * 2, [row("20"), row("10")])

    def test_rejects_wrong_opcode(self):
        with self.assertRaises(ValueError):
            guard_timestamp_reads("xbox_ReadTimeStampCounter()", [row("10", opcode="0f30")])

    def test_rejects_wrong_length(self):
        with self.assertRaises(ValueError):
            guard_timestamp_reads("xbox_ReadTimeStampCounter()", [row("10", length=3)])

    def test_rejects_invalid_address(self):
        for address in ("xyz", "100000000", "FFFFFFFF"):
            with self.subTest(address=address), self.assertRaises(ValueError):
                guard_timestamp_reads("xbox_ReadTimeStampCounter()", [row(address)])

    def test_rejects_duplicate_sites(self):
        with self.assertRaises(ValueError):
            guard_timestamp_reads("xbox_ReadTimeStampCounter()" * 2, [row("10"), row("10")])

    def test_rejects_call_count_mismatch(self):
        with self.assertRaises(ValueError):
            guard_timestamp_reads("xbox_ReadTimeStampCounter()", [])
        with self.assertRaises(ValueError):
            guard_timestamp_reads("xbox_ReadTimeStampCounter()", [row("10"), row("20")])

    def test_rejects_malformed_helper_reference(self):
        for source in ("ff_probe_read_tsc(0x10u)", "xbox_ReadTimeStampCounter(1)",
                       "xbox_ReadTimeStampCounter() + ff_probe_read_tsc2(1)"):
            with self.subTest(source=source), self.assertRaises(ValueError):
                guard_timestamp_reads(source, [row("10")])


def compiler():
    for name in (os.environ.get("FF_CC"), "cc", "gcc", "clang"):
        if name and shutil.which(name):
            return shutil.which(name)
    return None


@unittest.skipUnless(compiler(), "no C compiler available for the compiled timestamp guard regression")
class CompiledTimestampGuardTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        fixture = (ROOT / "tools" / "fixtures" / "thread_startup_probe.c").read_text(encoding="utf-8")
        signature = "static uint64_t ff_probe_read_tsc(uint32_t site)"
        function_at = fixture.index(signature)
        start = fixture.rfind("#ifdef FF_TIMESTAMP_COUNTER_GUARDS", 0, function_at)
        if start < 0:
            raise AssertionError("timestamp guard conditional not found before function")
        end_marker = "\n#endif\n#ifdef FF_DEBUG_SERVICE_PROOF"
        end = fixture.index(end_marker, start) + len("\n#endif")
        cls.guard_source = fixture[start:end]
        if signature not in cls.guard_source or "uint32_t ff_probe_timestamp_boundary_word(uint32_t field)" not in cls.guard_source:
            raise AssertionError("extracted timestamp section must include the guard and boundary getter")
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        (cls.path / "guard.h").write_text(
            "#define FF_TIMESTAMP_COUNTER_GUARDS 1\n"
            "#define FF_TIMESTAMP_COUNTER_GUARD_COUNT 2\n"
            "#define FF_TIMESTAMP_COUNTER_SITES { 0x1234u, 0x5678u }\n", encoding="utf-8")
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "guard.h"
static uint32_t timestamp_boundary[2];
static uint32_t g_eax, g_edx, g_esp;
static uint8_t guest_ram[16];
static jmp_buf stop_jump;
static uint32_t stop_status, stop_site, stop_ordinal, stop_count;
static _Noreturn void stop(uint32_t status, uint32_t site, uint32_t ordinal) {
    stop_status=status; stop_site=site; stop_ordinal=ordinal; ++stop_count;
    longjmp(stop_jump, 1);
}
''' + cls.guard_source + r'''
static void invoke(uint32_t site) { (void)ff_probe_read_tsc(site); }
static void reset_observations(void) {
    memset(timestamp_boundary,0,sizeof timestamp_boundary);
    stop_status=stop_site=stop_ordinal=stop_count=0;
}
int main(void) {
    const uint32_t regs[3]={0x11111111u,0x22222222u,0x33333333u};
    g_eax=regs[0]; g_edx=regs[1]; g_esp=regs[2];
    for(unsigned i=0;i<sizeof guest_ram;++i)guest_ram[i]=(uint8_t)(0xA0u+i);
    for(uint32_t site_index=0;site_index<2;++site_index) {
        uint32_t site=site_index?0x5678u:0x1234u;
        reset_observations();
        if(!setjmp(stop_jump)){invoke(site);assert(0 && "guard must stop");}
        assert(stop_count==1 && stop_status==9 && stop_site==site && stop_ordinal==0);
        assert(ff_probe_timestamp_boundary_word(0)==1);
        assert(ff_probe_timestamp_boundary_word(1)==site);
        assert(ff_probe_timestamp_boundary_word(2)==1);
        assert(g_eax==regs[0] && g_edx==regs[1] && g_esp==regs[2]);
        for(unsigned i=0;i<sizeof guest_ram;++i)assert(guest_ram[i]==(uint8_t)(0xA0u+i));
    }
    reset_observations();
    if(!setjmp(stop_jump)){invoke(0x9999u);assert(0 && "unknown site must stop");}
    assert(stop_count==1 && stop_status==9 && stop_site==0x9999u && stop_ordinal==0);
    assert(ff_probe_timestamp_boundary_word(0)==0 && ff_probe_timestamp_boundary_word(1)==0);
    assert(g_eax==regs[0] && g_edx==regs[1] && g_esp==regs[2]);
    for(unsigned i=0;i<sizeof guest_ram;++i)assert(guest_ram[i]==(uint8_t)(0xA0u+i));
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_compiled_guard_stops_at_sites_without_changing_state(self):
        executable = self.path / "guard"
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                str(self.path / "harness.c"), "-o", str(executable)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)
        # The compiled default-off fixture above proves no timestamp returns.
        # An explicitly enabled performance-clock branch may return a value.


if __name__ == "__main__":
    unittest.main()
