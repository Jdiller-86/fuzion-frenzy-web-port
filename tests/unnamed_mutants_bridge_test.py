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
    start = source.index("#ifdef FF_MUTANT_PROOF\n/* Optional creation model only.")
    end = source.index("\n#endif", source.index("static void create_unnamed_mutant(void)", start)) + len("\n#endif")
    block = source[start:end]
    for required in ("ff_probe_mutant_profile", "ff_probe_mutant_word", "kernel_record(192,3)",
                     "ff_unnamed_mutants_create", "g_esp+=16u"):
        if required not in block:
            raise AssertionError(f"mutant bridge missing required contract: {required}")
    if "if(ordinal==192u && mutant_ready)return create_unnamed_mutant;" not in source:
        raise AssertionError("ordinal 192 must remain gated by explicit mutant profile readiness")
    return block


@unittest.skipUnless(compiler(), "no C compiler available for unnamed mutant bridge regression")
class UnnamedMutantsBridgeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        block = extract_block()
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "runtime/core/unnamed_mutants.h"
#include "runtime/core/thread_bootstrap.h"

#define FF_MUTANT_PROOF 1
static uint8_t guest_ram[128u * 1024u];
static ff_virtual_memory virtual_memory;
static ff_unnamed_mutants unnamed_mutants;
static uint32_t vm_ready,probe_trace_count,kernel_call_count,parent_returned;
static uint32_t mutant_ready,mutant_status;
static uint32_t g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi;
static uint32_t kernel_calls[16][16];
static jmp_buf stopped;
static uint32_t stop_status,stop_site,stop_ordinal;
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
    assert(kernel_call_count<16);uint32_t *row=kernel_calls[kernel_call_count++];
    memset(row,0,16*sizeof *row);row[0]=ordinal;
    for(uint32_t i=0;i<argc;i++)row[i+2]=MEM32(g_esp+4u+4u*i);
    return row;
}
static void stop(uint32_t status,uint32_t site,uint32_t ordinal) {
    stop_status=status;stop_site=site;stop_ordinal=ordinal;longjmp(stopped,1);
}
''' + block + r'''
static void setup(void) {
    memset(guest_ram,0xa5,sizeof guest_ram);memset(&virtual_memory,0,sizeof virtual_memory);
    assert(ff_vm_init(&virtual_memory,guest_ram,sizeof guest_ram)==0);
    assert(ff_vm_claim_fixed(&virtual_memory,0x10000,0x10000)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x10000,0x1000,2)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x11000,0x1000,4)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x12000,0x1000,4)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x13000,0x1000,2)==0);
    vm_ready=1;probe_trace_count=kernel_call_count=parent_returned=0;
    mutant_ready=mutant_status=0;memset(&unnamed_mutants,0,sizeof unnamed_mutants);
    memset(kernel_calls,0,sizeof kernel_calls);
}
static void setargs(uint32_t out,uint32_t attrs,uint32_t owner) {
    g_esp=0x1f000;PUT32(g_esp+4,out);PUT32(g_esp+8,attrs);PUT32(g_esp+12,owner);
}
static void init_cpu(void) {
    g_eax=0x11112222;g_edx=0x33334444;g_ecx=0x55556666;
    g_ebx=0x77778888;g_esi=0x9999aaaa;g_edi=0xbbbbcccc;
}
static void expect_refusal(uint32_t out,uint32_t attrs,uint32_t owner,uint32_t status,uint32_t stopcode) {
    setargs(out,attrs,owner);
    uint8_t before[sizeof guest_ram];uint32_t count=unnamed_mutants.count;
    uint32_t calls=kernel_call_count;uint32_t old_rows[16][16];
    memcpy(before,guest_ram,sizeof before);memcpy(old_rows,kernel_calls,sizeof old_rows);
    uint32_t cpu[7]={g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi};
    cpu[2]=g_esp;
    if(!setjmp(stopped)){create_unnamed_mutant();assert(0&&"mutant create must stop on refusal");}
    assert(stop_status==stopcode&&stop_site==status&&stop_ordinal==192);
    assert(mutant_status==status&&unnamed_mutants.count==count);
    assert(memcmp(before,guest_ram,sizeof before)==0);
    assert(g_eax==cpu[0]&&g_edx==cpu[1]&&g_esp==cpu[2]&&g_ecx==cpu[3]&&
        g_ebx==cpu[4]&&g_esi==cpu[5]&&g_edi==cpu[6]);
    assert(kernel_call_count==calls+1);
    assert(memcmp(old_rows,kernel_calls,calls*sizeof old_rows[0])==0);
    assert(kernel_calls[calls][0]==192&&kernel_calls[calls][1]==1&&
        kernel_calls[calls][2]==out&&kernel_calls[calls][3]==attrs&&kernel_calls[calls][4]==owner);
}
int main(void) {
    setup();init_cpu();
    assert(ff_probe_mutant_word(0xffffffffu,0)==0&&ff_probe_mutant_word(0xffffffffu,1)==0);
    assert(ff_probe_mutant_profile(2)==FF_BOOT_INVALID&&ff_probe_mutant_word(0xffffffffu,0)==0);
    vm_ready=0;assert(ff_probe_mutant_profile(1)==FF_BOOT_INVALID);vm_ready=1;
    assert(ff_probe_mutant_profile(1)==0);
    assert(ff_probe_mutant_word(0xffffffffu,0)==1&&ff_probe_mutant_word(0xffffffffu,1)==0&&
        ff_probe_mutant_word(0xffffffffu,2)==FF_MUTANT_CAPACITY);

    setargs(0x11000,0,0);create_unnamed_mutant();
    assert(g_eax==0&&g_esp==0x1f010&&g_edx==0x33334444&&g_ecx==0x55556666&&
        g_ebx==0x77778888&&g_esi==0x9999aaaa&&g_edi==0xbbbbcccc);
    assert(MEM32(0x11000)==0x80004&&ff_probe_mutant_word(0xffffffffu,1)==1&&
        ff_probe_mutant_word(0,0)==0x80004);
    assert(kernel_calls[0][0]==192&&kernel_calls[0][1]==0&&kernel_calls[0][2]==0x11000&&
        kernel_calls[0][3]==0&&kernel_calls[0][4]==0&&kernel_calls[0][5]==0x11112222&&
        kernel_calls[0][6]==0x1f000&&kernel_calls[0][7]==0x80004&&kernel_calls[0][10]==1);
    for(unsigned i=0;i<0x10000;i++)assert(guest_ram[i]==(uint8_t)0xa5);
    for(unsigned i=0x10000;i<0x20000;i++) {
        if(i>=0x11000&&i<0x11004) continue;
        if(i>=0x1f004&&i<0x1f010) continue;
        assert(guest_ram[i]==(uint8_t)0xa5);
    }
    for(unsigned i=0x20000;i<sizeof guest_ram;i++)assert(guest_ram[i]==(uint8_t)0xa5);

    init_cpu();expect_refusal(0x11010,1,0,FF_BOOT_UNSUPPORTED,9);
    init_cpu();expect_refusal(0x11010,0,1,FF_BOOT_UNSUPPORTED,9);
    init_cpu();expect_refusal(0x11001,0,0,FF_BOOT_INVALID,9);
    init_cpu();expect_refusal(0x20000,0,0,FF_VM_ACCESS_VIOLATION,9);
    init_cpu();expect_refusal(0x10000,0,0,FF_VM_ACCESS_VIOLATION,9);
    init_cpu();expect_refusal(0x13000,0,0,FF_VM_ACCESS_VIOLATION,9);
    for(unsigned i=1;i<FF_MUTANT_CAPACITY;i++){setargs(0x11000+4*i,0,0);create_unnamed_mutant();}
    assert(ff_probe_mutant_word(0xffffffffu,1)==FF_MUTANT_CAPACITY);
    init_cpu();expect_refusal(0x11020,0,0,FF_MUTANT_NO_SLOTS,4);

    /* Calls lock configuration immediately; rejected profile changes are atomic. */
    {uint32_t ready=mutant_ready,status=mutant_status,count=unnamed_mutants.count;
    ff_unnamed_mutants prior=unnamed_mutants;uint32_t rows[16][16];memcpy(rows,kernel_calls,sizeof rows);
    uint8_t bytes[sizeof guest_ram];memcpy(bytes,guest_ram,sizeof bytes);
    assert(ff_probe_mutant_profile(0)==FF_BOOT_INVALID);
    assert(mutant_ready==ready&&mutant_status==status&&unnamed_mutants.count==count&&
        memcmp(&prior,&unnamed_mutants,sizeof prior)==0&&memcmp(rows,kernel_calls,sizeof rows)==0&&
        memcmp(bytes,guest_ram,sizeof bytes)==0);}
    assert(ff_probe_mutant_word(0xffffffffu,99)==0&&ff_probe_mutant_word(99,0)==0&&
        ff_probe_mutant_word(0,1)==0);
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_extracted_mutant_bridge(self):
        executable = self.path / "mutant-bridge"
        sources = [self.path / "harness.c", ROOT / "runtime" / "core" / "unnamed_mutants.c",
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
