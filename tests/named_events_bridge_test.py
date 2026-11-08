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


def extract_block():
    source = (ROOT / "tools" / "fixtures" / "thread_startup_probe.c").read_text(encoding="utf-8")
    start = source.index("#ifdef FF_NAMED_EVENT_PROOF\n/* Optional ASCII named-event")
    if "static void create_named_event(void)" not in source[start:]:
        raise AssertionError("named event bridge function is absent")
    depth = 0
    cursor = start
    end = None
    for line in source[start:].splitlines(keepends=True):
        directive = line.lstrip()
        if directive.startswith(("#if ", "#ifdef ", "#ifndef ")):
            depth += 1
        elif directive.startswith("#endif"):
            depth -= 1
            if depth == 0:
                end = cursor + len(line)
                break
        cursor += len(line)
    if end is None:
        raise AssertionError("named event bridge block has no matching #endif")
    block = source[start:end]
    for required in ("ff_probe_event_profile", "ff_probe_event_word", "kernel_record(189,4)",
                     "ff_named_events_create", "g_esp+=20u"):
        if required not in block:
            raise AssertionError(f"named event bridge missing required contract: {required}")
    if "if(ordinal==189u && event_ready)return create_named_event;" not in source:
        raise AssertionError("ordinal 189 must remain gated by explicit named-event profile readiness")
    return block


@unittest.skipUnless(compiler(), "no C compiler available for named event bridge regression")
class NamedEventsBridgeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "runtime/core/named_events.h"
#include "runtime/core/thread_bootstrap.h"

#define FF_NAMED_EVENT_PROOF 1
static uint8_t guest_ram[128u * 1024u];
static ff_virtual_memory virtual_memory;
static ff_named_events named_events;
static uint32_t vm_ready,probe_trace_count,kernel_call_count,parent_returned;
static uint32_t event_ready,event_status;
static uint32_t g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi;
static uint32_t kernel_calls[32][20];
static jmp_buf stopped;
static uint32_t stop_status,stop_site,stop_ordinal;
static uint32_t refusal_case;
static uint32_t MEM32(uint32_t a) {
    assert(a<=sizeof guest_ram-4);
    return (uint32_t)guest_ram[a]|((uint32_t)guest_ram[a+1]<<8)|
        ((uint32_t)guest_ram[a+2]<<16)|((uint32_t)guest_ram[a+3]<<24);
}
static void PUT32(uint32_t a,uint32_t v) {
    assert(a<=sizeof guest_ram-4);
    guest_ram[a]=(uint8_t)v;guest_ram[a+1]=(uint8_t)(v>>8);
    guest_ram[a+2]=(uint8_t)(v>>16);guest_ram[a+3]=(uint8_t)(v>>24);
}
static uint32_t *kernel_record(uint32_t ordinal,uint32_t argc) {
    assert(kernel_call_count<32);uint32_t *row=kernel_calls[kernel_call_count++];
    memset(row,0,20*sizeof *row);row[0]=ordinal;
    for(uint32_t i=0;i<argc;i++)row[i+2]=MEM32(g_esp+4u+4u*i);
    return row;
}
static void stop(uint32_t status,uint32_t site,uint32_t ordinal) {
    stop_status=status;stop_site=site;stop_ordinal=ordinal;longjmp(stopped,1);
}
''' + extract_block() + r'''
static void setup(void) {
    memset(guest_ram,0xa5,sizeof guest_ram);memset(&virtual_memory,0,sizeof virtual_memory);
    assert(ff_vm_init(&virtual_memory,guest_ram,sizeof guest_ram)==0);
    assert(ff_vm_claim_fixed(&virtual_memory,0x10000,0x10000)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x10000,0x1000,2)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x11000,0x1000,4)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x12000,0x1000,4)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x13000,0x1000,2)==0);
    vm_ready=1;probe_trace_count=kernel_call_count=parent_returned=0;
    event_ready=event_status=0;memset(&named_events,0,sizeof named_events);
    memset(kernel_calls,0,sizeof kernel_calls);
}
static void setargs(uint32_t out,uint32_t attrs,uint32_t type,uint32_t state) {
    g_esp=0x1f000;PUT32(g_esp+4,out);PUT32(g_esp+8,attrs);
    PUT32(g_esp+12,type);PUT32(g_esp+16,state);
}
static void init_cpu(void) {
    g_eax=0x11112222;g_edx=0x33334444;g_ecx=0x55556666;
    g_ebx=0x77778888;g_esi=0x9999aaaa;g_edi=0xbbbbcccc;
}
static void configure(uint32_t out,uint32_t attrs,uint32_t desc,uint32_t ptr,
                      uint32_t len,uint32_t max,uint32_t root,uint32_t flags,
                      const char *name,uint32_t type,uint32_t state) {
    if(attrs<=sizeof guest_ram-12){PUT32(attrs,root);PUT32(attrs+4,desc);PUT32(attrs+8,flags);}
    if(desc<=sizeof guest_ram-8){guest_ram[desc]=(uint8_t)len;guest_ram[desc+1]=(uint8_t)(len>>8);
        guest_ram[desc+2]=(uint8_t)max;guest_ram[desc+3]=(uint8_t)(max>>8);PUT32(desc+4,ptr);}
    if(ptr<=sizeof guest_ram&&len<=sizeof guest_ram-ptr)memcpy(guest_ram+ptr,name,len);
    setargs(out,attrs,type,state);
}
static void expect_refusal(uint32_t out,uint32_t attrs,uint32_t desc,uint32_t ptr,
                           uint32_t len,uint32_t max,uint32_t root,uint32_t flags,
                           const char *name,uint32_t type,uint32_t state,
                           uint32_t status,uint32_t stopcode) {
    ++refusal_case;configure(out,attrs,desc,ptr,len,max,root,flags,name,type,state);
    uint8_t before[sizeof guest_ram];uint32_t objects=named_events.object_count,handles=named_events.handle_count;
    uint32_t calls=kernel_call_count,old_rows[32][20];memcpy(before,guest_ram,sizeof before);
    memcpy(old_rows,kernel_calls,sizeof old_rows);
    uint32_t cpu[7]={g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi};
    if(!setjmp(stopped)){create_named_event();fprintf(stderr,"unexpected refusal success case %u\n",refusal_case);assert(0&&"named event create must stop on refusal");}
    assert(stop_status==stopcode&&stop_site==status&&stop_ordinal==189);
    assert(event_status==status&&named_events.object_count==objects&&named_events.handle_count==handles);
    assert(memcmp(before,guest_ram,sizeof before)==0);
    assert(g_eax==cpu[0]&&g_edx==cpu[1]&&g_esp==cpu[2]&&g_ecx==cpu[3]&&
        g_ebx==cpu[4]&&g_esi==cpu[5]&&g_edi==cpu[6]);
    assert(kernel_call_count==calls+1&&memcmp(old_rows,kernel_calls,calls*sizeof old_rows[0])==0);
    assert(kernel_calls[calls][0]==189&&kernel_calls[calls][1]==1&&
        kernel_calls[calls][2]==out&&kernel_calls[calls][3]==attrs&&
        kernel_calls[calls][4]==type&&kernel_calls[calls][5]==state);
}
int main(void) {
    setup();init_cpu();
    assert(ff_probe_event_word(0xffffffffu,0)==0);
    assert(ff_probe_event_profile(2)==FF_BOOT_INVALID&&ff_probe_event_word(0xffffffffu,0)==0);
    vm_ready=0;assert(ff_probe_event_profile(1)==FF_BOOT_INVALID);vm_ready=1;
    assert(ff_probe_event_profile(1)==0);
    assert(ff_probe_event_word(0xffffffffu,0)==1&&ff_probe_event_word(0xffffffffu,1)==0&&
        ff_probe_event_word(0xffffffffu,2)==0&&ff_probe_event_word(0xffffffffu,3)==FF_EVENT_CAPACITY);

    /* Capture must follow argument setup so the unchanged incoming frame is checked. */
    configure(0x11000,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0);
    uint8_t before[sizeof guest_ram];memcpy(before,guest_ram,sizeof before);create_named_event();
    assert(g_eax==0&&g_esp==0x1f014&&g_edx==0x33334444&&g_ecx==0x55556666&&
        g_ebx==0x77778888&&g_esi==0x9999aaaa&&g_edi==0xbbbbcccc);
    assert(MEM32(0x11000)==0x90004&&named_events.object_count==1&&named_events.handle_count==1);
    assert(kernel_calls[0][0]==189&&kernel_calls[0][1]==0&&kernel_calls[0][2]==0x11000&&
        kernel_calls[0][3]==0x12000&&kernel_calls[0][4]==1&&kernel_calls[0][5]==0&&
        kernel_calls[0][8]==0x90004&&kernel_calls[0][9]==1&&kernel_calls[0][10]==0&&
        kernel_calls[0][11]==1&&kernel_calls[0][12]==1&&kernel_calls[0][13]==0&&
        kernel_calls[0][14]==0x1f014&&kernel_calls[0][15]==0xfffffffcu&&
        kernel_calls[0][16]==0x80&&kernel_calls[0][17]==5&&
        kernel_calls[0][18]==0x13000&&kernel_calls[0][19]==0x10000);
    assert(ff_probe_event_word(0xffffffffu,1)==1&&ff_probe_event_word(0xffffffffu,2)==1&&
        ff_probe_event_word(0,0)==1&&ff_probe_event_word(0,1)==5&&
        ff_probe_event_word(0,2)==1&&ff_probe_event_word(0,3)==0&&
        ff_probe_event_word(0,4)==0x90004&&ff_probe_event_word(0,5)==1);
    for(unsigned i=0;i<sizeof guest_ram;i++) {
        if(i>=0x11000&&i<0x11004)continue;
        if(i>=0x1f004&&i<0x1f014)continue;
        assert(guest_ram[i]==before[i]);
    }

    /* ASCII case-folded open-if creates another handle to the same object. */
    init_cpu();configure(0x11004,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"aLPHa",1,0);
    create_named_event();assert(g_eax==FF_EVENT_NAME_EXISTS&&g_esp==0x1f014&&
        MEM32(0x11004)==0x90008&&named_events.object_count==1&&named_events.handle_count==2);
    assert(kernel_calls[1][1]==0&&kernel_calls[1][9]==1&&kernel_calls[1][10]==1&&
        kernel_calls[1][11]==1&&kernel_calls[1][12]==2&&kernel_calls[1][13]==FF_EVENT_NAME_EXISTS);
    assert(ff_probe_event_word(0xffffffffu,4)==FF_EVENT_NAME_EXISTS&&ff_probe_event_word(1,4)==0x90008&&
        ff_probe_event_word(1,5)==1);

    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",2,0,FF_BOOT_UNSUPPORTED,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,1,FF_BOOT_UNSUPPORTED,9);
    init_cpu();expect_refusal(0x11009,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_BOOT_INVALID,9);
    init_cpu();expect_refusal(0x20000,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_VM_ACCESS_VIOLATION,9);
    init_cpu();expect_refusal(0x11008,0x20000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_VM_ACCESS_VIOLATION,9);
    init_cpu();expect_refusal(0x11008,0x12001,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_BOOT_INVALID,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,5,6,0,0x80,"Alpha",1,0,FF_BOOT_UNSUPPORTED,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0,"Alpha",1,0,FF_BOOT_UNSUPPORTED,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x20000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_VM_ACCESS_VIOLATION,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x20000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_VM_ACCESS_VIOLATION,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,0,6,0xfffffffcu,0x80,"Alpha",1,0,FF_BOOT_INVALID,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,5,4,0xfffffffcu,0x80,"Alpha",1,0,FF_BOOT_INVALID,9);
    init_cpu();expect_refusal(0x11008,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"A/pha",1,0,FF_BOOT_INVALID,9);
    /* Both output aliases are rejected before changing the object model. */
    init_cpu();expect_refusal(0x11020,0x11020,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Alpha",1,0,FF_BOOT_INVALID,9);

    /* Two fresh objects plus one open fill four handles; the fifth call refuses atomically. */
    init_cpu();configure(0x11008,0x12000,0x13000,0x10000,4,6,0xfffffffcu,0x80,"Beta",1,0);create_named_event();
    init_cpu();configure(0x1100c,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Gamma",1,0);create_named_event();
    init_cpu();expect_refusal(0x11010,0x12000,0x13000,0x10000,5,6,0xfffffffcu,0x80,"Delta",1,0,FF_EVENT_NO_SLOTS,4);
    assert(named_events.object_count==3&&named_events.handle_count==4);

    /* Profile changes are frozen after either startup traces or an attempted kernel call. */
    {uint32_t ready=event_ready,status=event_status,objects=named_events.object_count,handles=named_events.handle_count;
     ff_named_events prior=named_events;uint8_t bytes[sizeof guest_ram];memcpy(bytes,guest_ram,sizeof bytes);
     uint32_t rows[32][20];memcpy(rows,kernel_calls,sizeof rows);probe_trace_count=1;
     assert(ff_probe_event_profile(0)==FF_BOOT_INVALID);
     assert(event_ready==ready&&event_status==status&&named_events.object_count==objects&&
       named_events.handle_count==handles&&memcmp(&prior,&named_events,sizeof prior)==0&&
       memcmp(rows,kernel_calls,sizeof rows)==0&&memcmp(bytes,guest_ram,sizeof bytes)==0);
     assert(ff_probe_event_profile(1)==FF_BOOT_INVALID&&ff_probe_event_profile(0)==FF_BOOT_INVALID);
     probe_trace_count=0;parent_returned=1;assert(ff_probe_event_profile(0)==FF_BOOT_INVALID);parent_returned=0;}
    assert(ff_probe_event_profile(1)==FF_BOOT_INVALID&&ff_probe_event_profile(0)==FF_BOOT_INVALID);
    assert(ff_probe_event_word(0xffffffffu,99)==0&&ff_probe_event_word(99,0)==0&&
        ff_probe_event_word(0,99)==0);
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_extracted_named_event_bridge(self):
        executable = self.path / "named-event-bridge"
        sources = [self.path / "harness.c", ROOT / "runtime" / "core" / "named_events.c",
                   ROOT / "runtime" / "core" / "virtual_memory.c"]
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                "-I", str(ROOT), *(str(p) for p in sources), "-o", str(executable)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)

    def test_bridge_compiles_with_proof_flag_off(self):
        source = self.path / "flag-off.c"
        obj = self.path / "flag-off.o"
        source.write_text('#include <stdint.h>\n' + extract_block() + '\nint main(void){return 0;}\n',
                          encoding="utf-8", newline="\n")
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                "-I", str(ROOT), "-c", str(source), "-o", str(obj)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)


if __name__ == "__main__":
    unittest.main()
