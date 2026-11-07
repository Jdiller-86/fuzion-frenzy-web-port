#!/usr/bin/env python3
"""Compile the exact trace-window bridge helpers against synthetic CPU/RAM."""
import hashlib
import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CC = next((shutil.which(c) for c in (os.environ.get('FF_CC'), 'cc', 'gcc', 'clang')
           if c and shutil.which(c)), None)
ROWS = 8192


def row(i):
    kind = 1 if i % 16 == 0 else 2
    address = 0x20000 + ((i // 16) % 512) if kind == 1 else 0x10000 + i
    stack, eax, fs = 0x70000000 + i, 0x81000000 + i, 0x1000 + i * 4
    head = 0xA5000000 ^ i
    return (kind, address, stack, eax, fs, head)


def digest(rows):
    payload = b'ff-original-trace-v1\0' + b''.join(
        int(value).to_bytes(4, 'little') for values in rows for value in values)
    return hashlib.sha256(payload).digest()


@unittest.skipUnless(CC, 'no C compiler for source-owned trace bridge fixture')
class TraceCaptureBridgeTests(unittest.TestCase):
    def build_and_run(self, capture=True):
        source = (ROOT / 'tools/fixtures/thread_startup_probe.c').read_text(encoding='utf-8')
        if capture:
            start = source.index('/* FF_TRACE_WINDOW_HELPERS_BEGIN */')
            end = source.index('/* FF_TRACE_WINDOW_HELPERS_END */', start) + len('/* FF_TRACE_WINDOW_HELPERS_END */')
            helpers = source[start:end]
        else:
            helpers = ''
        trace_start = source.index('static void trace(uint32_t kind, uint32_t address)\n{')
        trace_end = source.index('void recomp_trace_enter', trace_start)
        trace = source[trace_start:trace_end]
        cpu_getter_start = source.index('uint32_t ff_probe_trace_cpu(')
        cpu_getter_end = source.index('/* Read-only diagnostic globals.', cpu_getter_start)
        cpu_getter = source[cpu_getter_start:cpu_getter_end]
        definitions = r'''
#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "runtime/core/trace_capture.h"
/* Mirror the translator's register aliases so a local name cannot shadow a
 * register token during the extracted bridge compilation. */
#define eax g_eax
#define esp g_esp
#define esi g_esi
#define edi g_edi
#define FF_TRACE_CAPACITY 8192u
#define FF_GUEST_BYTES 0x100000u
#define FF_VM_READ 1u
#define FF_BOOT_INVALID 9u
static uint32_t probe_trace[FF_TRACE_CAPACITY * 3], probe_cpu_trace[FF_TRACE_CAPACITY * 3];
static uint32_t probe_trace_count, kernel_call_count, parent_returned;
static uint32_t g_esp, g_eax, g_fs_base;
#ifdef FF_TRACE_CAPTURE_PROOF
static ff_trace_capture trace_capture;
static uint32_t trace_capture_ready;
#endif
static uint32_t head_reads, fault_head;
static jmp_buf stopped;
static uint32_t stop_status, stop_target;
static uint32_t memory_read(uint32_t address) {
    ++head_reads;
    if (fault_head) longjmp(stopped, 1);
    return 0xA5000000u ^ ((address - 0x1000u) / 4u);
}
#define FF_VM_PROOF 1
#define FF_MEMORY_U32(address, access) ((void)sizeof(access), memory_read(address))
#define MEM32(address) memory_read(address)
static void stop(uint32_t status, uint32_t target, uint32_t ordinal) {
    (void)ordinal; stop_status=status; stop_target=target; longjmp(stopped,1);
}
'''
        setup = r'''
static void seed(uint32_t i) {
    uint32_t kind = i % 16u == 0u ? 1u : 2u;
    uint32_t address = kind == 1u ? 0x20000u + ((i / 16u) % 512u) : 0x10000u + i;
    g_esp=0x70000000u+i;g_eax=0x81000000u+i;g_fs_base=0x1000u+i*4u;
    trace(kind,address);
}
static void reset(void) {
    memset(probe_trace,0,sizeof probe_trace);memset(probe_cpu_trace,0,sizeof probe_cpu_trace);
    probe_trace_count=kernel_call_count=parent_returned=0;head_reads=fault_head=0;
    g_esp=g_eax=g_fs_base=0;stop_status=stop_target=0;
#ifdef FF_TRACE_CAPTURE_PROOF
    memset(&trace_capture,0,sizeof trace_capture);trace_capture_ready=0;
#endif
}
'''
        if capture:
            source_harness = '#define FF_TRACE_CAPTURE_PROOF 1\n' + definitions + helpers + '\nuint32_t ff_probe_trace_count(void){return retained_trace_count();}\n' + trace + cpu_getter + setup + r'''
int main(void) {
    uint32_t old_trace[8192u*3u], old_cpu[8192u*3u];
    reset(); for(uint32_t i=0;i<8192u;++i) seed(i);
    assert(probe_trace_count==8192u && head_reads==8192u);
    memcpy(old_trace,probe_trace,sizeof old_trace);memcpy(old_cpu,probe_cpu_trace,sizeof old_cpu);
    if(!setjmp(stopped)){seed(8192u);assert(0);}
    assert(stop_status==5u);assert(stop_target==0x20000u);assert(head_reads==8192u);
    reset();assert(ff_probe_trace_capture_profile(0)==0);
    for(uint32_t i=0;i<8192u;++i)seed(i);
    assert(probe_trace_count==8192u && head_reads==8192u);
    assert(ff_probe_trace_capture_word(0)==1u && ff_probe_trace_capture_word(1)==1u);
    assert(ff_probe_trace_capture_word(2)==0u && ff_probe_trace_capture_word(3)==0u);
    assert(ff_probe_trace_capture_word(4)==8192u && ff_probe_trace_capture_word(5)==8192u);
    assert(ff_probe_trace_capture_word(6)==512u && ff_probe_trace_capture_word(7)==0u);
    assert(ff_probe_trace_capture_word(8)==512u && ff_probe_trace_capture_word(9)==0u);
    assert(ff_probe_trace_capture_word(10)==8192u && ff_probe_trace_capture_word(11)==8192u);
    assert(memcmp(old_trace,probe_trace,sizeof old_trace)==0);
    assert(memcmp(old_cpu,probe_cpu_trace,sizeof old_cpu)==0);
    for(uint32_t i=0;i<8u;++i)assert(ff_probe_trace_capture_digest_word(0,i)==EXPECTED_FULL_WORD(i));
    assert(ff_probe_trace_capture_routine(0)==0x20000u);
    assert(ff_probe_trace_capture_routine(511u)==0x201ffu && ff_probe_trace_capture_routine(512u)==0u);
    uint32_t once=ff_probe_trace_capture_digest_word(0,0);assert(ff_probe_trace_capture_digest_word(0,0)==once);
    assert(ff_probe_trace_capture_digest_word(2,0)==0u && ff_probe_trace_capture_digest_word(0,8)==0u);
    assert(ff_probe_trace_capture_word(99)==0u);
    assert(ff_probe_trace_cpu(8191u,0u)==(0x81000000u+8191u));
    assert(ff_probe_trace_cpu(8191u,2u)==(0xA5000000u^8191u));
    assert(ff_probe_trace_cpu(8191u,1u)==(0x1000u+8191u*4u));
    assert(ff_probe_trace_cpu(8192u,0u)==0u && ff_probe_trace_cpu(0u,3u)==0u);
    reset();assert(ff_probe_trace_capture_profile(8192u)==0);
    for(uint32_t i=0;i<8192u;++i)seed(i);
    assert(ff_probe_trace_count()==8192u && probe_trace_count==8192u);
    assert(ff_probe_trace_capture_word(3)==0u && ff_probe_trace_capture_word(4)==8192u);
    seed(8192u);assert(ff_probe_trace_count()==1u && probe_trace_count==8193u);
    assert(ff_probe_trace_capture_word(3)==8192u && ff_probe_trace_capture_word(4)==8193u);
    assert(ff_probe_trace_capture_word(5)==1u && ff_probe_trace_capture_word(7)==512u);
    for(uint32_t i=0;i<8u;++i)assert(ff_probe_trace_capture_digest_word(1,i)==EXPECTED_PREFIX_WORD(i));
    assert(head_reads==8193u);
    assert(ff_probe_trace_cpu(0u,0u)==(0x81000000u+8192u) && ff_probe_trace_cpu(0u,1u)==(0x1000u+8192u*4u));
    assert(ff_probe_trace_cpu(0u,2u)==(0xA5000000u^8192u));
    assert(ff_probe_trace_cpu(1u,0u)==0u && ff_probe_trace_cpu(8192u,0u)==0u);
    for(uint32_t i=8193u;i<16384u;++i)seed(i);
    assert(probe_trace_count==16384u && ff_probe_trace_count()==8192u && head_reads==16384u);
    if(!setjmp(stopped)){seed(16384u);assert(0);}
    assert(stop_status==5u && head_reads==16384u && probe_trace_count==16384u);
    reset();ff_trace_capture invalid_before=trace_capture;
    assert(ff_probe_trace_capture_profile(1u)==FF_TRACE_CAPTURE_INVALID);
    assert(trace_capture_ready==0u && probe_trace_count==0u && memcmp(&invalid_before,&trace_capture,sizeof invalid_before)==0);
    assert(ff_probe_trace_capture_profile(0u)==0u);seed(0u);
    uint32_t prior=probe_trace_count;ff_trace_capture after_trace=trace_capture;
    assert(ff_probe_trace_capture_profile(0u)==FF_TRACE_CAPTURE_INVALID && probe_trace_count==prior);
    assert(memcmp(&after_trace,&trace_capture,sizeof after_trace)==0);
    reset();kernel_call_count=1u;ff_trace_capture kernel_before=trace_capture;
    assert(ff_probe_trace_capture_profile(0u)==FF_TRACE_CAPTURE_INVALID);
    assert(trace_capture_ready==0u && kernel_call_count==1u && memcmp(&kernel_before,&trace_capture,sizeof kernel_before)==0);
    reset();parent_returned=1u;ff_trace_capture parent_before=trace_capture;
    assert(ff_probe_trace_capture_profile(0u)==FF_TRACE_CAPTURE_INVALID);
    assert(trace_capture_ready==0u && parent_returned==1u && memcmp(&parent_before,&trace_capture,sizeof parent_before)==0);
    reset();assert(ff_probe_trace_capture_profile(8192u)==0u);
    for(uint32_t i=0;i<8192u;++i)seed(i);
    assert(ff_probe_trace_capture_word(3)==0u && ff_probe_trace_capture_word(4)==8192u);
    ff_trace_capture witness_before_fault=trace_capture;
    fault_head=1u;
    if(!setjmp(stopped)){seed(8192u);assert(0);}
    assert(head_reads==8193u && probe_trace_count==8193u && trace_capture.total==8192u);
    assert(ff_probe_trace_count()==8192u);
    assert(trace_capture.captured_start==0u && trace_capture.prefix_entries==0u);
    assert(trace_capture.prefix_routines==0u && ff_probe_trace_capture_digest_word(1,0)==0u);
    assert(memcmp(&witness_before_fault,&trace_capture,sizeof witness_before_fault)==0);
    assert(memcmp(old_trace,probe_trace,sizeof old_trace)==0 && memcmp(old_cpu,probe_cpu_trace,sizeof old_cpu)==0);
    for(uint32_t i=0;i<8u;++i)assert(ff_probe_trace_capture_digest_word(0,i)==EXPECTED_PREFIX_WORD(i));
    ff_trace_capture before_fault=trace_capture;
    assert(ff_probe_trace_capture_profile(0u)==FF_TRACE_CAPTURE_INVALID);
    assert(memcmp(&before_fault,&trace_capture,sizeof before_fault)==0 && probe_trace_count==8193u);
    reset();assert(ff_probe_trace_capture_profile(8192u)==0u);
    for(uint32_t i=0;i<8192u;++i)seed(i);
    g_esp=0x70002000u;g_eax=0x81002000u;g_fs_base=0x9000u;fault_head=0u;
    if(!setjmp(stopped)){trace(1u,0x30000u);assert(0);}
    assert(stop_status==5u && stop_target==0x30000u && head_reads==8193u);
    assert(probe_trace_count==8193u && trace_capture.total==8192u && trace_capture.captured_start==0u);
    reset();assert(ff_probe_trace_capture_profile(0u)==0u);g_fs_base=0x1000u;fault_head=1u;
    ff_trace_capture before_first_fault=trace_capture;
    if(!setjmp(stopped)){trace(2u,0x10000u);assert(0);}
    assert(head_reads==1u && probe_trace_count==1u && trace_capture.total==0u);
    assert(ff_probe_trace_count()==0u && ff_probe_trace_capture_word(4u)==0u);
    assert(memcmp(&before_first_fault,&trace_capture,sizeof before_first_fault)==0);
    assert(probe_trace[0]==0u && probe_cpu_trace[0]==0u);
    assert(ff_probe_trace_capture_profile(0u)==FF_TRACE_CAPTURE_INVALID && trace_capture.total==0u);
    reset();g_fs_base=0x1000u;fault_head=1u;
    if(!setjmp(stopped)){trace(2u,0x10000u);assert(0);}
    assert(head_reads==1u && probe_trace_count==1u && probe_trace[0]==2u && probe_trace[1]==0x10000u);
    assert(probe_cpu_trace[0]==g_eax && probe_cpu_trace[1]==g_fs_base && probe_cpu_trace[2]==0u);
    return 0;
}
'''
            full = digest([row(i) for i in range(8192)])
            prefix = full
            def words(value):
                return ','.join(f'0x{int.from_bytes(value[i:i+4], "little"):08x}u' for i in range(0,32,4))
            source_harness = source_harness.replace('int main(void)',
                f'#define EXPECTED_FULL_WORD(i) ((uint32_t[]){{{words(full)}}}[i])\n'
                f'#define EXPECTED_PREFIX_WORD(i) ((uint32_t[]){{{words(prefix)}}}[i])\nint main(void)')
        else:
            source_harness = definitions + trace + setup + r'''
int main(void) { reset();seed(0u);assert(probe_trace_count==1u&&head_reads==1u);return 0; }
'''
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory);src=path/'trace.c';exe=path/'trace'
            src.write_text(source_harness,encoding='utf-8',newline='\n')
            command=[CC,'-std=c11','-Wall','-Wextra','-Werror','-I',str(ROOT),str(src)]
            if capture: command += [str(ROOT/'runtime/core/trace_capture.c'),str(ROOT/'runtime/core/sha256.c')]
            command += ['-o',str(exe)]
            build=subprocess.run(command,capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_trace_window_bridge_contracts(self):
        self.build_and_run(capture=True)

    def test_flag_off_compiles_legacy_trace_path(self):
        self.build_and_run(capture=False)


if __name__ == '__main__':
    unittest.main()
