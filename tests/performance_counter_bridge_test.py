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


def extract_blocks():
    source = (ROOT / "tools" / "fixtures" / "thread_startup_probe.c").read_text(encoding="utf-8")
    tsc_signature = "static uint64_t ff_probe_read_tsc(uint32_t site)"
    tsc_at = source.index(tsc_signature)
    guard_start = source.rfind("#ifdef FF_TIMESTAMP_COUNTER_GUARDS", 0, tsc_at)
    guard_end_marker = "\n#endif\n#ifdef FF_DEBUG_SERVICE_PROOF"
    guard_end = source.index(guard_end_marker, guard_start) + len("\n#endif")
    guard = source[guard_start:guard_end]
    for required in ("ff_probe_performance_counter_profile", "ff_probe_performance_counter_word",
                     "performance_counter_enabled && ticks<performance_counter_ticks"):
        if required not in guard:
            raise AssertionError(f"timestamp block missing counter contract: {required}")

    query_signature = "static void query_performance_counter(void)"
    query_at = source.index(query_signature)
    query_start = source.rfind("#ifdef FF_PERFORMANCE_COUNTER_PROOF", 0, query_at)
    query_end = source.index("\n#endif", query_at) + len("\n#endif")
    query = source[query_start:query_end]
    for required in ("kernel_record(126,0)", "FF_PERFORMANCE_CLOCK_CAPACITY_REACHED",
                     "performance_clock.last_ticks", "row[12]=performance_clock.frequency_hz"):
        if required not in query:
            raise AssertionError(f"counter query block missing contract: {required}")

    # Lookup is too coupled to the full fixture to compile independently. Keep
    # the precise opt-in gate as source evidence alongside the compiled helper.
    if "ordinal==126u && performance_clock.ready && performance_counter_enabled" not in source:
        raise AssertionError("ordinal-126 lookup must require both initialized clocks and explicit enable")
    return guard, query


@unittest.skipUnless(compiler(), "no C compiler available for performance counter bridge regression")
class PerformanceCounterBridgeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        guard, query = extract_blocks()
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        (cls.path / "fixture.h").write_text(
            "#define FF_TIMESTAMP_COUNTER_GUARDS 1\n"
            "#define FF_PERFORMANCE_CLOCK_PROOF 1\n"
            "#define FF_PERFORMANCE_COUNTER_PROOF 1\n"
            "#define FF_TIMESTAMP_COUNTER_GUARD_COUNT 2\n"
            "#define FF_TIMESTAMP_COUNTER_SITES { 0x1234u, 0x5678u }\n",
            encoding="utf-8")
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "fixture.h"
#include "runtime/core/performance_clock.h"
static uint8_t guest_ram[32];
static uint32_t timestamp_boundary[2],probe_trace_count,kernel_call_count;
static ff_performance_clock performance_clock;
static uint32_t performance_status,performance_last_site,performance_frequency_queries;
static uint32_t performance_counter_enabled,performance_counter_status,performance_counter_count;
static uint64_t performance_counter_ticks,performance_counter_value;
static struct { uint64_t interrupt_time; } timers;
static uint32_t clock_words[4],host_samples,stop_status,stop_site,stop_ordinal;
static uint32_t g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi;
static uint32_t kernel_row[20],kernel_seen_ordinal,kernel_seen_argc;
static uint64_t synthetic_ticks[80];static unsigned synthetic_tick_count,synthetic_tick_index;
static jmp_buf stop_jump;
static _Noreturn void stop(uint32_t status,uint32_t site,uint32_t ordinal) {
    stop_status=status;stop_site=site;stop_ordinal=ordinal;longjmp(stop_jump,1);
}
static void ff_host_clock(uint32_t address) {
    uint64_t ticks;
    assert(address==(uint32_t)(uintptr_t)clock_words);++host_samples;
    ticks=synthetic_tick_index<synthetic_tick_count?synthetic_ticks[synthetic_tick_index++]:0;
    clock_words[0]=(uint32_t)ticks;clock_words[1]=(uint32_t)(ticks>>32);
}
static uint32_t *kernel_record(uint32_t ordinal,uint32_t argc) {
    kernel_seen_ordinal=ordinal;kernel_seen_argc=argc;++kernel_call_count;
    memset(kernel_row,0,sizeof kernel_row);return kernel_row;
}
''' + guard + '\n' + query + r'''
static uint64_t read_tsc(uint32_t site) { return ff_probe_read_tsc(site); }
static void set_ticks(const uint64_t *values,unsigned count) {
    memcpy(synthetic_ticks,values,count*sizeof *values);synthetic_tick_count=count;
    synthetic_tick_index=host_samples=0;
}
static void reset_run(void) {
    memset(&performance_clock,0,sizeof performance_clock);
    performance_status=performance_last_site=performance_frequency_queries=0;
    performance_counter_enabled=performance_counter_status=performance_counter_count=0;
    performance_counter_ticks=performance_counter_value=0;
    probe_trace_count=kernel_call_count=0;memset(timestamp_boundary,0,sizeof timestamp_boundary);
    host_samples=synthetic_tick_index=0;stop_status=stop_site=stop_ordinal=0;
}
static void expect_counter_stop(uint32_t expected_stop,uint32_t expected_status) {
    if(!setjmp(stop_jump)){query_performance_counter();assert(0 && "counter query must stop");}
    assert(stop_status==expected_stop&&stop_ordinal==126&&stop_site==expected_status);
}
int main(void) {
    const uint64_t first[]={10000000ull,15000000ull,20000000ull,0x0000000200000000ull,UINT64_MAX};
    reset_run();set_ticks(first,5);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(ff_probe_performance_counter_word(0)==0&&ff_probe_performance_counter_word(2)==0);
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_word(0)==0&&ff_probe_performance_counter_word(1)==10000000u);
    assert(ff_probe_performance_counter_word(3)==64&&ff_probe_performance_counter_word(99)==0);
    assert(ff_probe_performance_counter_profile(0)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_word(0)==0);
    assert(ff_probe_performance_counter_profile(2)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(ff_probe_performance_counter_word(0)==0);
    /* Opt-in off: the ordinary admitted TSC path works but does not populate query state. */
    assert(read_tsc(0x1234u)==10000000ull);
    assert(host_samples==1&&ff_probe_performance_counter_word(2)==0);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_word(0)==1&&ff_probe_performance_counter_word(2)==0);
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_word(0)==0&&ff_probe_performance_counter_word(2)==0);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);

    timers.interrupt_time=UINT64_C(0x1122334455667788);
    g_eax=0x11111111u;g_edx=0x22222222u;g_esp=0x33333333u;
    g_ecx=0x44444444u;g_ebx=0x55555555u;g_esi=0x66666666u;g_edi=0x77777777u;
    for(unsigned i=0;i<sizeof guest_ram;i++)guest_ram[i]=(uint8_t)(0xa0u+i);
    query_performance_counter();
    assert(kernel_seen_ordinal==126&&kernel_seen_argc==0);
    assert(g_eax==15000000u&&g_edx==0&&g_esp==0x33333337u);
    assert(g_ecx==0x44444444u&&g_ebx==0x55555555u&&g_esi==0x66666666u&&g_edi==0x77777777u);
    for(unsigned i=0;i<sizeof guest_ram;i++)assert(guest_ram[i]==(uint8_t)(0xa0u+i));
    assert(kernel_row[1]==0&&kernel_row[2]==15000000u&&kernel_row[3]==0);
    assert(kernel_row[4]==0x11111111u&&kernel_row[5]==0x22222222u&&kernel_row[6]==0x33333333u);
    assert(kernel_row[7]==15000000u&&kernel_row[8]==0&&kernel_row[9]==0x33333337u);
    assert(kernel_row[10]==15000000u&&kernel_row[11]==0&&kernel_row[12]==10000000u);
    assert(kernel_row[13]==0x55667788u&&kernel_row[14]==0x11223344u);
    assert(ff_probe_performance_counter_word(2)==1&&ff_probe_performance_counter_word(5)==15000000u);

    /* Fresh host samples, high-word conversion, and the UINT64_MAX identity scale. */
    query_performance_counter();
    assert(g_eax==20000000u&&ff_probe_performance_counter_word(5)==20000000u);
    query_performance_counter();
    assert(g_eax==0&&g_edx==2&&ff_probe_performance_counter_word(5)==0&&
        ff_probe_performance_counter_word(6)==2);
    query_performance_counter();
    assert(g_eax==0xffffffffu&&g_edx==0xffffffffu&&
        ff_probe_performance_counter_word(5)==0xffffffffu&&ff_probe_performance_counter_word(6)==0xffffffffu);
    assert(host_samples==5&&synthetic_tick_index==5&&ff_probe_performance_counter_word(2)==4);
    /* Post-start setters reject atomically, preserving both clock objects and counter metadata. */
    {ff_performance_clock before=performance_clock;
    uint32_t enabled=performance_counter_enabled,status=performance_counter_status,count=performance_counter_count;
    uint64_t ticks=performance_counter_ticks,value=performance_counter_value;
    assert(ff_probe_performance_counter_profile(0)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&before,&performance_clock,sizeof before)==0&&performance_counter_enabled==enabled&&
        performance_counter_status==status&&performance_counter_count==count&&
        performance_counter_ticks==ticks&&performance_counter_value==value);
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_INVALID);
    assert(memcmp(&before,&performance_clock,sizeof before)==0&&performance_counter_enabled==enabled&&
        performance_counter_status==status&&performance_counter_count==count&&
        performance_counter_ticks==ticks&&performance_counter_value==value);}

    /* Cross-source ordering refuses a query older than the admitted TSC sample. */
    reset_run();
    {const uint64_t ordered[]={30000000ull,29999999ull};set_ticks(ordered,2);}
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);
    assert(read_tsc(0x5678u)==30000000ull);
    g_eax=0xabcdef01u;g_edx=0xabcdef02u;g_esp=0xabcdef03u;
    expect_counter_stop(9,FF_PERFORMANCE_CLOCK_ORDER);
    assert(host_samples==2&&ff_probe_performance_counter_word(2)==0&&
        ff_probe_performance_counter_word(4)==FF_PERFORMANCE_CLOCK_ORDER&&
        ff_probe_performance_counter_word(5)==0&&ff_probe_performance_counter_word(7)==0);
    assert(g_eax==0xabcdef01u&&g_edx==0xabcdef02u&&g_esp==0xabcdef03u&&kernel_row[1]==1);

    /* Counter-query order is enforced independently when no TSC sample exists. */
    reset_run();{const uint64_t ordered[]={20000000ull,19000000ull};set_ticks(ordered,2);}
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);
    query_performance_counter();
    assert(performance_clock.count==0&&performance_clock.last_ticks==0&&performance_counter_count==1);
    g_eax=0xaabbcc01u;g_edx=0xaabbcc02u;g_esp=0xaabbcc03u;
    expect_counter_stop(9,FF_PERFORMANCE_CLOCK_ORDER);
    assert(host_samples==2&&performance_clock.count==0&&performance_clock.last_ticks==0);
    assert(performance_counter_count==1&&performance_counter_ticks==20000000ull&&
        performance_counter_value==20000000ull&&performance_counter_status==FF_PERFORMANCE_CLOCK_ORDER);
    assert(g_eax==0xaabbcc01u&&g_edx==0xaabbcc02u&&g_esp==0xaabbcc03u&&kernel_row[1]==1);

    /* An RDTSC older than a query refuses before its journal changes. */
    reset_run();{const uint64_t ordered[]={30000000ull,29000000ull};set_ticks(ordered,2);}
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);
    query_performance_counter();
    {ff_performance_clock before=performance_clock;uint32_t counter_count=performance_counter_count;
    uint64_t counter_ticks=performance_counter_ticks,counter_value=performance_counter_value;
    g_eax=0xc0dec001u;g_edx=0xc0dec002u;g_esp=0xc0dec003u;
    if(!setjmp(stop_jump)){(void)read_tsc(0x5678u);assert(0);}
    assert(memcmp(&before,&performance_clock,sizeof before)==0);
    assert(performance_clock.count==0&&performance_clock.last_ticks==0);
    assert(performance_counter_count==counter_count&&performance_counter_ticks==counter_ticks&&
        performance_counter_value==counter_value&&performance_counter_status==FF_PERFORMANCE_CLOCK_OK);
    assert(performance_status==FF_PERFORMANCE_CLOCK_ORDER&&performance_last_site==0x5678u&&
        stop_status==9&&stop_site==0x5678u&&stop_ordinal==0&&host_samples==2&&
        g_eax==0xc0dec001u&&g_edx==0xc0dec002u&&g_esp==0xc0dec003u&&kernel_row[1]==0);}

    /* Checked scale overflow preserves outputs/state and reports the service refusal. */
    reset_run();{const uint64_t max[]={UINT64_MAX};set_ticks(max,1);}
    assert(ff_probe_performance_profile(1000000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);
    g_eax=0x11112222u;g_edx=0x33334444u;g_esp=0x55556666u;
    expect_counter_stop(9,FF_PERFORMANCE_CLOCK_OVERFLOW);
    assert(performance_counter_count==0&&performance_counter_ticks==0&&performance_counter_value==0);
    assert(g_eax==0x11112222u&&g_edx==0x33334444u&&g_esp==0x55556666u&&kernel_row[1]==1);

    /* Capacity refusal occurs before the host sample and preserves the last accepted state. */
    reset_run();{uint64_t many[64];for(unsigned i=0;i<64;i++)many[i]=(uint64_t)(i+1)*10000000ull;set_ticks(many,64);}
    assert(ff_probe_performance_profile(10000000u)==FF_PERFORMANCE_CLOCK_OK);
    assert(ff_probe_performance_counter_profile(1)==FF_PERFORMANCE_CLOCK_OK);
    for(unsigned i=0;i<64;i++)query_performance_counter();
    assert(performance_counter_count==64&&host_samples==64);
    {uint32_t eax=g_eax,edx=g_edx,esp=g_esp;uint64_t ticks=performance_counter_ticks,value=performance_counter_value;
    expect_counter_stop(4,FF_PERFORMANCE_CLOCK_CAPACITY_REACHED);
    assert(host_samples==64&&synthetic_tick_index==64&&performance_counter_count==64&&
        performance_counter_ticks==ticks&&performance_counter_value==value);
    assert(g_eax==eax&&g_edx==edx&&g_esp==esp&&kernel_row[1]==1);}
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_extracted_counter_bridge(self):
        executable = self.path / "counter-bridge"
        runtime = ROOT / "runtime" / "core" / "performance_clock.c"
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                "-I", str(ROOT), str(self.path / "harness.c"), str(runtime), "-o", str(executable)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)


if __name__ == "__main__":
    unittest.main()
