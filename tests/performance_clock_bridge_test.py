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


def extract_source():
    source = (ROOT / "tools" / "fixtures" / "thread_startup_probe.c").read_text(encoding="utf-8")
    signature = "static uint64_t ff_probe_read_tsc(uint32_t site)"
    at = source.index(signature)
    start = source.rfind("#ifdef FF_TIMESTAMP_COUNTER_GUARDS", 0, at)
    if start < 0:
        raise AssertionError("timestamp guard block not found")
    marker = "\n#endif\n#ifdef FF_DEBUG_SERVICE_PROOF"
    end = source.index(marker, start) + len("\n#endif")
    guarded = source[start:end]
    if "ff_probe_performance_profile" not in guarded or "ff_probe_performance_word" not in guarded:
        raise AssertionError("extracted timestamp block must include performance profile and getters")

    signature = "static void query_performance_frequency(void)"
    at = source.index(signature)
    start = source.rfind("#ifdef FF_PERFORMANCE_CLOCK_PROOF", 0, at)
    end = source.index("\n#endif\nrecomp_func_t recomp_lookup_kernel", at) + len("\n#endif")
    query = source[start:end]
    if "kernel_record(127,0)" not in query:
        raise AssertionError("extracted frequency query must retain ordinal 127 with argc zero")
    return guarded, query


@unittest.skipUnless(compiler(), "no C compiler available for performance clock bridge regression")
class PerformanceClockBridgeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        guarded, query = extract_source()
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        (cls.path / "fixture.h").write_text(
            "#define FF_TIMESTAMP_COUNTER_GUARDS 1\n"
            "#define FF_PERFORMANCE_CLOCK_PROOF 1\n"
            "#define FF_TIMESTAMP_COUNTER_GUARD_COUNT 2\n"
            "#define FF_TIMESTAMP_COUNTER_SITES { 0x1234u, 0x5678u }\n",
            encoding="utf-8")
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "fixture.h"
#include "runtime/core/performance_clock.h"
static uint8_t guest_ram[16];
static uint32_t timestamp_boundary[2], probe_trace_count, kernel_call_count;
static ff_performance_clock performance_clock;
static uint32_t performance_status, performance_last_site, performance_frequency_queries;
static uint32_t clock_words[4], host_samples, stop_status, stop_site, stop_ordinal;
static uint32_t g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi;
static uint32_t kernel_row[20], kernel_seen_ordinal, kernel_seen_argc;
static uint64_t synthetic_ticks[80];
static unsigned synthetic_tick_count, synthetic_tick_index;
static jmp_buf stop_jump;
static _Noreturn void stop(uint32_t status,uint32_t site,uint32_t ordinal) {
    stop_status=status;stop_site=site;stop_ordinal=ordinal;longjmp(stop_jump,1);
}
static void ff_host_clock(uint32_t address) {
    uint64_t ticks;
    assert(address==(uint32_t)(uintptr_t)clock_words);
    ++host_samples;
    ticks=synthetic_tick_index<synthetic_tick_count?synthetic_ticks[synthetic_tick_index++]:0;
    clock_words[0]=(uint32_t)ticks;clock_words[1]=(uint32_t)(ticks>>32);
}
static uint32_t *kernel_record(uint32_t ordinal,uint32_t argc) {
    kernel_seen_ordinal=ordinal;kernel_seen_argc=argc;memset(kernel_row,0,sizeof kernel_row);return kernel_row;
}
''' + guarded + '\n' + query + r'''
static uint64_t invoke(uint32_t site) { return ff_probe_read_tsc(site); }
static void set_ticks(const uint64_t *ticks,unsigned count) {
    memcpy(synthetic_ticks,ticks,count*sizeof *ticks);synthetic_tick_count=count;synthetic_tick_index=0;host_samples=0;
}
static void clear_stop(void) { stop_status=stop_site=stop_ordinal=0; }
int main(void) {
    const uint64_t ticks[]={10000000ull,20000000ull,30000000ull};
    uint64_t result;
    set_ticks(ticks,3);clear_stop();
    assert(!performance_clock.ready);
    if(!setjmp(stop_jump)){(void)invoke(0x1234u);assert(0);}
    assert(stop_status==9&&stop_site==0x1234u&&stop_ordinal==0&&host_samples==0);
    assert(ff_probe_performance_word(UINT32_MAX,0)==0);

    assert(ff_probe_performance_profile(1000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_word(0,5)==0x1234u&&ff_probe_performance_word(1,5)==0x5678u);
    assert(ff_probe_performance_word(2,5)==0&&ff_probe_performance_word(0,0)==0);
    result=invoke(0x1234u);assert(result==1000000ull);
    result=invoke(0x5678u);assert(result==2000000ull);
    assert(host_samples==2&&synthetic_tick_index==2);
    assert(ff_probe_performance_word(0,0)==0x1234u&&ff_probe_performance_word(1,0)==0x5678u);
    assert(ff_probe_performance_word(1,1)==(uint32_t)ticks[1]);
    assert(ff_probe_performance_word(UINT32_MAX,2)==2&&ff_probe_performance_word(UINT32_MAX,3)==64);
    assert(ff_probe_performance_word(UINT32_MAX,7)==2);
    assert(ff_probe_performance_word(2,0)==0&&ff_probe_performance_word(2,5)==0);
    assert(ff_probe_performance_word(UINT32_MAX,99)==0&&ff_probe_performance_word(0,99)==0);
    assert(ff_probe_performance_word(0,5)==0x1234u&&ff_probe_performance_word(1,5)==0x5678u);

    /* A bad site does not consume a host sample or alter the accumulated rows. */
    clear_stop();if(!setjmp(stop_jump)){(void)invoke(0x9999u);assert(0);}
    assert(stop_status==9&&stop_site==0x9999u&&host_samples==2&&ff_probe_performance_word(UINT32_MAX,2)==2);

    /* Reset and invalid profiles clear readiness; invalid profiles stay closed. */
    assert(ff_probe_performance_profile(0)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(ff_probe_performance_word(UINT32_MAX,0)==0&&ff_probe_performance_word(UINT32_MAX,2)==0);
    assert(ff_probe_performance_profile(1000000u)==FF_PERFORMANCE_CLOCK_OK);
    {ff_performance_clock before=performance_clock;
    probe_trace_count=1;assert(ff_probe_performance_profile(10)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&before,&performance_clock,sizeof before)==0);}
    probe_trace_count=0;assert(ff_probe_performance_word(UINT32_MAX,0)==1);
    {ff_performance_clock before=performance_clock;
    kernel_call_count=1;assert(ff_probe_performance_profile(10)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&before,&performance_clock,sizeof before)==0);}
    kernel_call_count=0;
    /* Invalid profile leaves the bridge disabled; it must stop without sampling. */
    assert(ff_probe_performance_profile(0)==FF_PERFORMANCE_CLOCK_INVALID);
    set_ticks(ticks,3);clear_stop();if(!setjmp(stop_jump)){(void)invoke(0x1234u);assert(0);}
    assert(stop_status==9&&host_samples==0&&ff_probe_performance_word(UINT32_MAX,2)==0);

    /* Every admitted read samples anew. A rollback is recorded as ORDER then stops. */
    assert(ff_probe_performance_profile(1000000u)==FF_PERFORMANCE_CLOCK_OK);
    {const uint64_t pair[]={20000000ull,15000000ull};set_ticks(pair,2);}
    (void)invoke(0x1234u);clear_stop();
    if(!setjmp(stop_jump)){(void)invoke(0x5678u);assert(0);}
    assert(stop_status==9&&stop_site==0x5678u&&host_samples==2);
    assert(ff_probe_performance_word(UINT32_MAX,4)==FF_PERFORMANCE_CLOCK_ORDER);
    assert(ff_probe_performance_word(UINT32_MAX,2)==1&&ff_probe_performance_word(0,1)==20000000u);

    /* The 65th read samples, hits fixed capacity, and leaves exactly 64 rows. */
    {uint64_t many[65];for(unsigned i=0;i<65;i++)many[i]=(uint64_t)(i+1)*10000000ull;set_ticks(many,65);}
    assert(ff_probe_performance_profile(1u)==FF_PERFORMANCE_CLOCK_OK);
    for(unsigned i=0;i<64;i++)(void)invoke(0x1234u);
    clear_stop();if(!setjmp(stop_jump)){(void)invoke(0x1234u);assert(0);}
    assert(stop_status==4&&stop_site==0x1234u&&host_samples==65);
    assert(ff_probe_performance_word(UINT32_MAX,2)==64);
    assert(ff_probe_performance_word(UINT32_MAX,4)==FF_PERFORMANCE_CLOCK_CAPACITY_REACHED);

    /* Frequency query preserves all unrelated CPU/guest sentinels and emits an exact 0-arg row. */
    assert(ff_probe_performance_profile(123456789u)==FF_PERFORMANCE_CLOCK_OK);
    g_eax=0x11111111u;g_edx=0x22222222u;g_esp=0x33333333u;
    g_ecx=0x44444444u;g_ebx=0x55555555u;g_esi=0x66666666u;g_edi=0x77777777u;
    for(unsigned i=0;i<sizeof guest_ram;i++)guest_ram[i]=(uint8_t)(0xa0u+i);
    query_performance_frequency();
    assert(kernel_seen_ordinal==127&&kernel_seen_argc==0);
    assert(g_eax==123456789u&&g_edx==0&&g_esp==0x33333337u);
    assert(g_ecx==0x44444444u&&g_ebx==0x55555555u&&g_esi==0x66666666u&&g_edi==0x77777777u);
    for(unsigned i=0;i<sizeof guest_ram;i++)assert(guest_ram[i]==(uint8_t)(0xa0u+i));
    assert(kernel_row[1]==0&&kernel_row[2]==123456789u&&kernel_row[3]==0);
    assert(kernel_row[4]==0x11111111u&&kernel_row[5]==0x22222222u&&kernel_row[6]==0x33333333u);
    assert(kernel_row[7]==123456789u&&kernel_row[8]==0&&kernel_row[9]==0x33333337u);
    assert(ff_probe_performance_word(UINT32_MAX,6)==1);
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_extracted_clock_bridge_and_frequency_query(self):
        executable = self.path / "clock-bridge"
        runtime = ROOT / "runtime" / "core" / "performance_clock.c"
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                "-I", str(ROOT), str(self.path / "harness.c"), str(runtime), "-o", str(executable)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)


if __name__ == "__main__":
    unittest.main()
