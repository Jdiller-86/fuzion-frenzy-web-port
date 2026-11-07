#!/usr/bin/env python3
"""Synthetic ABI regression for the reached display-mode and vblank bridges.

The bridge bodies are extracted verbatim from thread_startup_probe.c. Only the
kernel recorder, guest memory, PCRTC observer, and stop boundary are synthetic.
"""
import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FIXTURE = ROOT / "tools/fixtures/thread_startup_probe.c"
CC = next((shutil.which(name) for name in
           (os.environ.get("FF_CC"), "cc", "gcc", "clang")
           if name and shutil.which(name)), None)


def extract_function(source: str, signature: str, next_signature: str) -> str:
    start = source.index(signature)
    end = source.index(next_signature, start)
    return source[start:end]


@unittest.skipUnless(CC, "no C compiler for synthetic reached bridge fixture")
class ReachedDisplayWaitBridgeTests(unittest.TestCase):
    def test_stdcall_cleanup_and_rejection_boundary(self):
        source = FIXTURE.read_text(encoding="utf-8")
        wait = extract_function(source, "static void wait_vblank(void)",
                                "static void set_display_mode(void)")
        display = extract_function(source, "static void set_display_mode(void)",
                                   "static uint32_t av_saved_prior_setter(void)")
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "runtime/core/vblank_wait.h"
#include "runtime/core/display_mode.h"
#include "runtime/core/device_registers.h"
#define FF_MMIO_PROOF 1
static ff_device_registers device_registers, device_before;

#define FF_KERNEL_BINDING_COUNT 2u
#define FF_KERNEL_BASE 0xF0000000u
#define FF_GUEST_BYTES 0x04000000u
typedef struct { uint32_t pending, enabled, status; } ff_pcrtc_state;
static ff_pcrtc_state pcrtc;
static uint32_t pcrtc_declared, pcrtc_ticks;
static uint32_t vblank_wait_calls, vblank_wait_type, vblank_wait_signal, vblank_wait_decision;
static uint32_t display_mode_ready, display_mode_decision;
static uint32_t display_mode_addr, display_mode_step, display_mode_mode, display_mode_format;
static uint32_t display_mode_pitch, display_mode_fb, display_mode_window;
static uint32_t g_eax, g_esp, g_ebx, g_esi, g_edi, g_ebp, g_eflags, g_fs_base;
static uint32_t guest[FF_GUEST_BYTES / 4u];
static uint32_t kernel_rows[8][8], kernel_call_count;
static jmp_buf stop_env;
static uint32_t stop_status, stop_target, stop_ordinal, stop_count;

static uint32_t *mem_slot(uint32_t address) {
    assert(address <= sizeof guest - 4u && (address & 3u) == 0u);
    return guest + address / 4u;
}
#define MEM32(address) (*mem_slot(address))
static uint32_t mem_read(uint32_t address) { return MEM32(address); }
static void mem_write(uint32_t address, uint32_t value) {
    MEM32(address)=value;
}
static uint32_t *kernel_record(uint32_t ordinal, uint32_t words) {
    assert(kernel_call_count < 8u && words <= 8u);
    uint32_t *row = kernel_rows[kernel_call_count++];
    memset(row, 0, sizeof kernel_rows[0]); row[0] = ordinal; row[7] = words;
    return row;
}
static uint32_t kernel_binding_address(uint32_t ordinal) {
    return FF_KERNEL_BASE + ordinal * 4u;
}
static void ff_pcrtc_vblank(ff_pcrtc_state *state) { ++state->pending; ++pcrtc_ticks; }
static void stop(uint32_t status, uint32_t target, uint32_t ordinal) {
    stop_status=status; stop_target=target; stop_ordinal=ordinal; ++stop_count;
    longjmp(stop_env, 1);
}
'''+wait+display+r'''
static const uint32_t regs[] = {0x11111111u,0x22222222u,0x33333333u,
    0x44444444u,0x55555555u,0x66666666u,0x77777777u};
static void seed_frame(uint32_t esp, uint32_t argc, const uint32_t *args) {
    memset(guest,0,sizeof guest); g_esp=esp; mem_write(esp,0xA1B2C3D4u);
    for(uint32_t i=0;i<argc;++i) mem_write(esp+4u+i*4u,args[i]);
    g_eax=regs[0];g_ebx=regs[1];g_esi=regs[2];g_edi=regs[3];
    g_ebp=regs[4];g_eflags=regs[5];g_fs_base=regs[6];
}
static void assert_preserved(void) {
    assert(g_ebx==regs[1] && g_esi==regs[2] && g_edi==regs[3]);
    assert(g_ebp==regs[4] && g_eflags==regs[5] && g_fs_base==regs[6]);
}
static void assert_frame(uint32_t esp, uint32_t argc, const uint32_t *args) {
    assert(mem_read(esp)==0xA1B2C3D4u);
    for(uint32_t i=0;i<argc;++i)assert(mem_read(esp+4u+i*4u)==args[i]);
}
static void reset_service(void) {
    memset(kernel_rows,0,sizeof kernel_rows);kernel_call_count=0;
    stop_status=stop_target=stop_ordinal=stop_count=0;
    memset(&pcrtc,0,sizeof pcrtc);pcrtc_declared=0;pcrtc_ticks=0;
    vblank_wait_calls=vblank_wait_type=vblank_wait_signal=vblank_wait_decision=0;
    display_mode_ready=display_mode_decision=0;
    display_mode_addr=display_mode_step=display_mode_mode=display_mode_format=0;
    display_mode_pitch=display_mode_fb=display_mode_window=0;
    const ff_device_init_entry initial={FF_DISPLAY_TUPLE_ADDR+0x100u,0u};
    assert(ff_device_registers_init_many(&device_registers,FF_DISPLAY_TUPLE_ADDR,
        0x00800000u,&initial,1u)==0u);
}
static void invoke_wait(void) { wait_vblank(); }
static void invoke_display(void) { set_display_mode(); }
int main(int argc, char **argv) {
    assert(argc==2);
    const uint32_t wait_args[5]={FF_VBLANK_WAIT_OBJECT,6u,1u,0u,0u};
    const uint32_t display_args[6]={FF_DISPLAY_TUPLE_ADDR,FF_DISPLAY_TUPLE_STEP,
        FF_DISPLAY_TUPLE_MODE,FF_DISPLAY_TUPLE_FORMAT,FF_DISPLAY_TUPLE_PITCH,
        FF_DISPLAY_TUPLE_FB};
    if(strcmp(argv[1],"wait-success")==0) {
    /* Successful wait consumes return address plus five stdcall arguments. */
    reset_service();pcrtc_declared=3u;seed_frame(0x100u,5u,wait_args);
    mem_write(FF_VBLANK_WAIT_OBJECT,0x00040001u);
    mem_write(FF_VBLANK_WAIT_OBJECT+4u,0u);
    for(uint32_t i=0;i<3u;++i){
        if(i){seed_frame(0x100u+i*0x40u,5u,wait_args);
            mem_write(FF_VBLANK_WAIT_OBJECT,0x00040001u);
            mem_write(FF_VBLANK_WAIT_OBJECT+4u,0u);}
        uint32_t before=g_esp;
        invoke_wait();
        assert(g_esp==before+24u);assert(g_eax==0u);
        assert_preserved();assert_frame(before,5u,wait_args);
        assert(kernel_call_count==i+1u && pcrtc_ticks==i+1u);
    }
    return 0;
    }
    if(strcmp(argv[1],"wait-reject")==0) {
    /* Exhausted declared delivery stops before frame cleanup or event consume. */
    reset_service();pcrtc_declared=0u;seed_frame(0x200u,5u,wait_args);
    mem_write(FF_VBLANK_WAIT_OBJECT,0x00040001u);
    mem_write(FF_VBLANK_WAIT_OBJECT+4u,0u);
    uint32_t before=g_esp;
    if(!setjmp(stop_env)){invoke_wait();assert(0);}
    assert(stop_count==1u && stop_status==1u && stop_ordinal==159u);
    assert(g_esp==before && pcrtc_ticks==0u &&
           mem_read(FF_VBLANK_WAIT_OBJECT+4u)==0u);
    assert_preserved();assert_frame(before,5u,wait_args);
    return 0;
    }
    if(strcmp(argv[1],"display-success")==0) {
    /* Successful display mode consumes return address plus six arguments. */
    reset_service();seed_frame(0x400u,6u,display_args);
    device_before=device_registers;
    uint32_t before=g_esp;invoke_display();
    assert(g_esp==before+28u && g_eax==0u && display_mode_ready==1u);
    assert(display_mode_decision==FF_DISPLAY_OK);
    assert(memcmp(&device_registers,&device_before,sizeof device_registers)==0);
    assert_preserved();assert_frame(before,6u,display_args);
    return 0;
    }
    if(strcmp(argv[1],"display-reject")==0) {
    /* Unsupported display tuple stops without cleanup or accepted state. */
    reset_service();uint32_t bad_display[6];memcpy(bad_display,display_args,sizeof bad_display);
    bad_display[2]^=1u;seed_frame(0x500u,6u,bad_display);
    uint32_t before=g_esp;
    if(!setjmp(stop_env)){invoke_display();assert(0);}
    assert(stop_count==1u && stop_status==1u && stop_ordinal==3u);
    assert(g_esp==before && display_mode_ready==0u);
    assert_preserved();assert_frame(before,6u,bad_display);
    return 0;
    }
    if(strcmp(argv[1],"display-unready")==0 || strcmp(argv[1],"display-old-tuple")==0) {
    reset_service();uint32_t rejected[6];memcpy(rejected,display_args,sizeof rejected);
    if(strcmp(argv[1],"display-unready")==0)device_registers.ready=0u;
    else {rejected[0]=0x03C10CFCu;rejected[3]=0u;}
    seed_frame(0x600u,6u,rejected);uint32_t before=g_esp;
    device_before=device_registers;
    if(!setjmp(stop_env)){invoke_display();assert(0);}
    assert(stop_count==1u && stop_status==1u && stop_ordinal==3u);
    assert(g_esp==before && g_eax==regs[0] && display_mode_ready==0u);
    assert(display_mode_decision==FF_DISPLAY_BAD_ADDR);
    assert(memcmp(&device_registers,&device_before,sizeof device_registers)==0);
    assert_preserved();assert_frame(before,6u,rejected);
    return 0;
    }
    return 2;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            src, exe = path / "bridge.c", path / "bridge"
            src.write_text(harness, encoding="utf-8", newline="\n")
            build = subprocess.run([CC, "-std=c11", "-Wall", "-Wextra", "-Werror",
                                    "-I", str(ROOT), str(src),
                                    str(ROOT / "runtime/core/vblank_wait.c"),
                                    str(ROOT / "runtime/core/display_mode.c"),
                                    str(ROOT / "runtime/core/device_registers.c"), "-o", str(exe)],
                                   capture_output=True, text=True)
            self.assertEqual(build.returncode, 0, build.stderr)
            for case in ("wait-success", "wait-reject", "display-success", "display-reject",
                         "display-unready", "display-old-tuple"):
                with self.subTest(case=case):
                    run = subprocess.run([str(exe), case], capture_output=True, text=True)
                    self.assertEqual(run.returncode, 0, run.stderr)


if __name__ == "__main__":
    unittest.main()
