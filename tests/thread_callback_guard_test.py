"""Compile the owned callback admission guard with synthetic dispatch/CPU state."""
import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CC = next((shutil.which(c) for c in (os.environ.get('FF_CC'), 'cc', 'gcc', 'clang')
           if c and shutil.which(c)), None)


@unittest.skipUnless(CC, 'no C compiler for source-owned callback guard fixture')
class ThreadCallbackGuardTests(unittest.TestCase):
    def test_missing_callback_refuses_before_allocation_or_success_record(self):
        source = (ROOT / 'tools/fixtures/thread_startup_probe.c').read_text(encoding='utf-8')
        start = source.index('static void create_thread(void)\n{')
        end = source.index('static void close_handle(void)', start)
        block = source[start:end]
        harness = r'''
#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "runtime/core/thread_bootstrap.h"
static ff_boot_runtime boot;
static uint32_t row[20], g_eax, g_esp, stop_code, stop_target, stop_ordinal;
static uint32_t routine_admitted, system_admitted, allocated;
static jmp_buf stopped;
static void callback(void) {}
static void (*recomp_lookup(uint32_t address))(void) {
    return (address==0x1000u && routine_admitted) ||
           (address==0x2000u && system_admitted) ? callback : 0;
}
static uint32_t *kernel_record(uint32_t ordinal, uint32_t count) {
    assert(ordinal==255 && count==10); row[0]=ordinal; return row;
}
uint32_t ff_boot_create(ff_boot_runtime *runtime, const uint32_t args[10]) {
    assert(runtime==&boot && args==row+2); ++allocated; return 0;
}
static void stop(uint32_t status, uint32_t target, uint32_t ordinal) {
    stop_code=status;stop_target=target;stop_ordinal=ordinal;longjmp(stopped,1);
}
''' + block + r'''
static void setup(uint32_t routine, uint32_t system) {
    memset(row,0,sizeof row);row[7]=0x1000;row[11]=0x2000;
    routine_admitted=routine;system_admitted=system;allocated=0;
    g_eax=0x12345678;g_esp=0x18000;
}
static void refused(uint32_t routine,uint32_t system,uint32_t missing) {
    setup(routine,system);
    if(!setjmp(stopped)){create_thread();assert(0 && "missing callback must stop");}
    assert(stop_code==9 && stop_target==missing && stop_ordinal==255);
    assert(row[1]==1 && allocated==0 && g_eax==0x12345678 && g_esp==0x18000);
    assert(row[7]==0x1000 && row[11]==0x2000);
}
int main(void) {
    refused(0,1,0x1000); /* Report start routine, even when system is admitted. */
    refused(1,0,0x2000);
    refused(0,0,0x1000); /* Deterministic first missing callback. */
    setup(1,1);create_thread();
    assert(allocated==1 && row[1]==0 && g_eax==0 && g_esp==0x1802c);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            src, exe = path / 'guard.c', path / 'guard'
            src.write_text(harness, encoding='utf-8', newline='\n')
            build = subprocess.run([CC, '-std=c11', '-Wall', '-Wextra', '-Werror',
                                    '-I', str(ROOT), str(src), '-o', str(exe)],
                                   capture_output=True, text=True)
            self.assertEqual(build.returncode, 0, build.stderr)
            run = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, run.stderr)


if __name__ == '__main__':
    unittest.main()
