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
    blocks = []
    specifications = (
        ("#ifdef FF_MUTANT_PROOF\n/* Optional creation model only.",
         "static void create_unnamed_mutant(void)", "legacy mutant"),
        ("#ifdef FF_NAMED_EVENT_PROOF\n/* Optional ASCII named-event",
         "static void create_named_event(void)", "named event"),
        ("#ifdef FF_LOCAL_MUTANT_PROOF\n/* Alternative four-handle mutant pool",
         "static void create_local_mutant(void)", "local mutant"),
    )
    for marker, function, label in specifications:
        start = source.index(marker)
        if function not in source[start:]:
            raise AssertionError(f"{label} bridge function is absent from the source")
        depth = 0
        end = None
        cursor = start
        for line in source[start:].splitlines(keepends=True):
            stripped = line.lstrip()
            if stripped.startswith(("#if ", "#ifdef ", "#ifndef ")):
                depth += 1
            elif stripped.startswith("#endif"):
                depth -= 1
                if depth == 0:
                    end = cursor + len(line)
                    break
            cursor += len(line)
        if end is None:
            raise AssertionError(f"{label} bridge block has no matching #endif")
        block = source[start:end]
        required = {
            "legacy mutant": ("ff_probe_mutant_profile", "ff_probe_mutant_word", "kernel_record(192,3)", "g_esp+=16u"),
            "named event": ("ff_probe_event_profile", "ff_probe_event_word", "kernel_record(189,4)", "g_esp+=20u", "ff_local_mutants_guard_event"),
            "local mutant": ("ff_probe_local_mutant_profile", "ff_probe_local_mutant_word",
                             "ff_probe_local_mutant_wait_profile", "ff_probe_local_mutant_wait_word",
                             "ff_local_mutants_create", "ff_local_mutants_acquire", "kernel_record(192,3)",
                             "kernel_record(234u,4u)"),
        }[label]
        if any(item not in block for item in required):
            raise AssertionError(f"{label} bridge block is missing an expected source-owned contract")
        blocks.append(block)
    if "if(ordinal==234u && local_mutant_wait_enabled)return wait_local_mutant;" not in source:
        raise AssertionError("local mutant wait dispatcher is not explicitly gated by its selected profile")
    return blocks


@unittest.skipUnless(compiler(), "no C compiler available for local mutant bridge regression")
class LocalMutantsBridgeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        old_block, event_block, local_block = extract_blocks()
        cls.directory = tempfile.TemporaryDirectory()
        cls.path = Path(cls.directory.name)
        harness = r'''#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include "runtime/core/unnamed_mutants.h"
#include "runtime/core/named_events.h"
#include "runtime/core/local_mutants.h"
#include "runtime/core/thread_bootstrap.h"

#define FF_MUTANT_PROOF 1
#define FF_NAMED_EVENT_PROOF 1
#define FF_LOCAL_MUTANT_PROOF 1
static uint8_t guest_ram[128u * 1024u];
static ff_virtual_memory virtual_memory;
static ff_unnamed_mutants unnamed_mutants;
static ff_named_events named_events;
static ff_local_mutants local_mutants;
static ff_boot_runtime boot;
static uint32_t vm_ready,probe_trace_count,kernel_call_count,parent_returned;
static uint32_t mutant_ready,mutant_status,event_ready,event_status;
static uint32_t local_mutant_ready,local_mutant_status;
static uint32_t local_mutant_wait_enabled,local_mutant_wait_sealed;
static uint32_t local_mutant_wait_attempts,local_mutant_wait_completed,local_mutant_wait_status;
static uint32_t local_mutant_wait_last_handle,local_mutant_wait_last_thread,local_mutant_wait_last_object;
static uint32_t local_mutant_wait_mode,local_mutant_wait_alertable,local_mutant_wait_timeout;
static uint32_t local_mutant_wait_owner_before,local_mutant_wait_owner_after;
static uint32_t local_mutant_wait_eax_before,local_mutant_wait_esp_before;
static uint32_t local_mutant_wait_eax_after,local_mutant_wait_esp_after;
static uint32_t g_eax,g_edx,g_esp,g_ecx,g_ebx,g_esi,g_edi;
static uint32_t kernel_calls[64][24];
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
    assert(kernel_call_count<64 && argc<=10);
    uint32_t *row=kernel_calls[kernel_call_count++];
    memset(row,0,24*sizeof *row);row[0]=ordinal;
    for(uint32_t i=0;i<argc;i++)row[i+2]=MEM32(g_esp+4u+4u*i);
    return row;
}
static void stop(uint32_t status,uint32_t site,uint32_t ordinal) {
    stop_status=status;stop_site=site;stop_ordinal=ordinal;longjmp(stopped,1);
}
''' + old_block + '\n' + event_block + '\n' + local_block + r'''
static void init_cpu(void) {
    g_eax=0x11112222;g_edx=0x33334444;g_ecx=0x55556666;
    g_ebx=0x77778888;g_esi=0x9999aaaa;g_edi=0xbbbbcccc;
}
static void setargs(uint32_t a,uint32_t b,uint32_t c,uint32_t d) {
    g_esp=0x1f000;PUT32(g_esp+4,a);PUT32(g_esp+8,b);PUT32(g_esp+12,c);PUT32(g_esp+16,d);
}
static void configure_named(uint32_t out,uint32_t attrs,uint32_t desc,uint32_t payload,
                            const char *name,uint32_t type,uint32_t state) {
    const uint32_t n=(uint32_t)strlen(name);
    PUT32(attrs,0xfffffffcu);PUT32(attrs+4,desc);PUT32(attrs+8,0x80);
    guest_ram[desc]=(uint8_t)n;guest_ram[desc+1]=(uint8_t)(n>>8);
    guest_ram[desc+2]=(uint8_t)(n+1u);guest_ram[desc+3]=(uint8_t)((n+1u)>>8);
    PUT32(desc+4,payload);if(payload<=sizeof guest_ram&&n<=sizeof guest_ram-payload)memcpy(guest_ram+payload,name,n);
    setargs(out,attrs,type,state);
}
static void reset_fixture(void) {
    memset(guest_ram,0xa5,sizeof guest_ram);memset(&virtual_memory,0,sizeof virtual_memory);
    assert(ff_vm_init(&virtual_memory,guest_ram,sizeof guest_ram)==0);
    assert(ff_boot_init(&boot,guest_ram,sizeof guest_ram,0x1000,0x2000)==FF_BOOT_OK);
    assert(ff_vm_claim_fixed(&virtual_memory,0x10000,0x10000)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x10000,0x1000,2)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x11000,0x1000,4)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x12000,0x1000,4)==0);
    assert(ff_vm_map_fixed(&virtual_memory,0x13000,0x1000,2)==0);
    vm_ready=1;probe_trace_count=kernel_call_count=parent_returned=0;
    mutant_ready=mutant_status=event_ready=event_status=0;
    local_mutant_ready=local_mutant_status=0;
    local_mutant_wait_enabled=local_mutant_wait_sealed=0;
    local_mutant_wait_attempts=local_mutant_wait_completed=local_mutant_wait_status=0;
    local_mutant_wait_last_handle=local_mutant_wait_last_thread=local_mutant_wait_last_object=0;
    local_mutant_wait_mode=local_mutant_wait_alertable=local_mutant_wait_timeout=0;
    local_mutant_wait_owner_before=local_mutant_wait_owner_after=0;
    local_mutant_wait_eax_before=local_mutant_wait_esp_before=0;
    local_mutant_wait_eax_after=local_mutant_wait_esp_after=0;
    memset(&unnamed_mutants,0,sizeof unnamed_mutants);memset(&named_events,0,sizeof named_events);
    memset(&local_mutants,0,sizeof local_mutants);memset(kernel_calls,0,sizeof kernel_calls);init_cpu();
}
static void snapshot(uint8_t *ram,uint32_t *rows,uint32_t *cpu) {
    memcpy(ram,guest_ram,sizeof guest_ram);memcpy(rows,kernel_calls,sizeof kernel_calls);
    cpu[0]=g_eax;cpu[1]=g_edx;cpu[2]=g_esp;cpu[3]=g_ecx;cpu[4]=g_ebx;cpu[5]=g_esi;cpu[6]=g_edi;
}
static void unchanged(uint8_t *ram,uint32_t *rows,uint32_t *cpu) {
    (void)rows;assert(memcmp(ram,guest_ram,sizeof guest_ram)==0);
    assert(g_eax==cpu[0]&&g_edx==cpu[1]&&g_esp==cpu[2]&&g_ecx==cpu[3]&&
        g_ebx==cpu[4]&&g_esi==cpu[5]&&g_edi==cpu[6]);
}
static uint32_t event_profile(uint32_t enable) {
    uint32_t ready=event_ready,status=event_status;ff_named_events model=named_events;
    uint32_t result=ff_probe_event_profile(enable);
    if(result)assert(event_ready==ready&&event_status==status&&memcmp(&model,&named_events,sizeof model)==0);
    return result;
}
static uint32_t old_profile(uint32_t enable) {
    uint32_t ready=mutant_ready,status=mutant_status;ff_unnamed_mutants model=unnamed_mutants;
    uint32_t result=ff_probe_mutant_profile(enable);
    if(result)assert(mutant_ready==ready&&mutant_status==status&&memcmp(&model,&unnamed_mutants,sizeof model)==0);
    return result;
}
static uint32_t local_profile(uint32_t enable) {
    uint32_t ready=local_mutant_ready,status=local_mutant_status;ff_local_mutants model=local_mutants;
    uint32_t result=ff_probe_local_mutant_profile(enable);
    if(result)assert(local_mutant_ready==ready&&local_mutant_status==status&&memcmp(&model,&local_mutants,sizeof model)==0);
    return result;
}
static uint32_t local_wait_profile(uint32_t enable) {
    uint32_t enabled=local_mutant_wait_enabled,sealed=local_mutant_wait_sealed;
    uint32_t attempts=local_mutant_wait_attempts,completed=local_mutant_wait_completed,status=local_mutant_wait_status;
    ff_local_mutants model=local_mutants;
    uint32_t result=ff_probe_local_mutant_wait_profile(enable);
    if(result)assert(local_mutant_wait_enabled==enabled&&local_mutant_wait_sealed==sealed&&
        local_mutant_wait_attempts==attempts&&local_mutant_wait_completed==completed&&
        local_mutant_wait_status==status&&memcmp(&model,&local_mutants,sizeof model)==0);
    return result;
}
static void assert_profiles_refuse_atomically(uint32_t peers_only) {
    uint8_t ram[sizeof guest_ram];uint32_t rows[64][24];
    ff_unnamed_mutants old=unnamed_mutants;ff_named_events events=named_events;ff_local_mutants local=local_mutants;
    uint32_t old_words[2]={mutant_ready,mutant_status},event_words[2]={event_ready,event_status};
    uint32_t local_words[2]={local_mutant_ready,local_mutant_status};uint32_t calls=kernel_call_count;
    memcpy(ram,guest_ram,sizeof ram);memcpy(rows,kernel_calls,sizeof rows);
    assert(ff_probe_event_profile(0)==FF_BOOT_INVALID&&ff_probe_event_profile(1)==FF_BOOT_INVALID);
    assert(ff_probe_mutant_profile(0)==FF_BOOT_INVALID&&ff_probe_mutant_profile(1)==FF_BOOT_INVALID);
    if(!peers_only)
        assert(ff_probe_local_mutant_profile(0)==FF_BOOT_INVALID&&ff_probe_local_mutant_profile(1)==FF_BOOT_INVALID);
    if(!peers_only)
        assert(ff_probe_local_mutant_wait_profile(0)==FF_BOOT_INVALID&&ff_probe_local_mutant_wait_profile(1)==FF_BOOT_INVALID);
    assert(memcmp(ram,guest_ram,sizeof ram)==0&&memcmp(rows,kernel_calls,sizeof rows)==0&&kernel_call_count==calls);
    assert(memcmp(&old,&unnamed_mutants,sizeof old)==0&&memcmp(&events,&named_events,sizeof events)==0&&
        memcmp(&local,&local_mutants,sizeof local)==0);
    assert(mutant_ready==old_words[0]&&mutant_status==old_words[1]&&
        event_ready==event_words[0]&&event_status==event_words[1]&&
        local_mutant_ready==local_words[0]&&local_mutant_status==local_words[1]);
}
static void expect_local_refusal(uint32_t out,uint32_t attrs,uint32_t owner,uint32_t wanted) {
    setargs(out,attrs,owner,0);uint8_t before[sizeof guest_ram];uint32_t rows[64][24],cpu[7];
    snapshot(before,&rows[0][0],cpu);ff_local_mutants prior=local_mutants;uint32_t objects=local_mutants.object_count,handles=local_mutants.handle_count;
    ff_named_events peer_events=named_events;ff_unnamed_mutants peer_old=unnamed_mutants;
    uint32_t peer_event_ready=event_ready,peer_event_status=event_status,peer_old_ready=mutant_ready,peer_old_status=mutant_status;
    uint32_t calls=kernel_call_count;
    if(!setjmp(stopped)){create_local_mutant();assert(0&&"local mutant refusal must stop");}
    assert(stop_status==(wanted==FF_LOCAL_MUTANT_NO_SLOTS?4u:9u)&&stop_site==wanted&&stop_ordinal==192);
    assert(local_mutant_status==wanted&&local_mutants.object_count==objects&&local_mutants.handle_count==handles);
    unchanged(before,&rows[0][0],cpu);assert(memcmp(&prior,&local_mutants,sizeof prior)==0&&kernel_call_count==calls+1);
    assert(memcmp(&peer_events,&named_events,sizeof peer_events)==0&&memcmp(&peer_old,&unnamed_mutants,sizeof peer_old)==0&&
        event_ready==peer_event_ready&&event_status==peer_event_status&&mutant_ready==peer_old_ready&&mutant_status==peer_old_status);
    assert(memcmp(rows[0],kernel_calls[0],calls*sizeof rows[0])==0);
    assert(kernel_calls[calls][0]==192&&kernel_calls[calls][1]==1&&kernel_calls[calls][2]==out&&
        kernel_calls[calls][3]==attrs&&kernel_calls[calls][4]==owner);
}
static void expect_event_refusal(uint32_t out,uint32_t attrs,uint32_t desc,uint32_t payload,
                                 const char *name,uint32_t wanted,uint32_t type,uint32_t state) {
    configure_named(out,attrs,desc,payload,name,type,state);uint8_t before[sizeof guest_ram];
    uint32_t rows[64][24],cpu[7];snapshot(before,&rows[0][0],cpu);
    ff_named_events prior=named_events;uint32_t objects=named_events.object_count,handles=named_events.handle_count,calls=kernel_call_count;
    ff_local_mutants peer_local=local_mutants;ff_unnamed_mutants peer_old=unnamed_mutants;
    uint32_t peer_local_ready=local_mutant_ready,peer_local_status=local_mutant_status;
    uint32_t peer_old_ready=mutant_ready,peer_old_status=mutant_status;
    if(!setjmp(stopped)){create_named_event();assert(0&&"event namespace collision must stop");}
    assert(stop_status==9&&stop_site==wanted&&stop_ordinal==189&&event_status==wanted);
    assert(named_events.object_count==objects&&named_events.handle_count==handles);
    unchanged(before,&rows[0][0],cpu);assert(memcmp(&prior,&named_events,sizeof prior)==0&&kernel_call_count==calls+1);
    assert(memcmp(&peer_local,&local_mutants,sizeof peer_local)==0&&memcmp(&peer_old,&unnamed_mutants,sizeof peer_old)==0&&
        local_mutant_ready==peer_local_ready&&local_mutant_status==peer_local_status&&
        mutant_ready==peer_old_ready&&mutant_status==peer_old_status);
    assert(memcmp(rows[0],kernel_calls[0],calls*sizeof rows[0])==0);
    assert(kernel_calls[calls][0]==189&&kernel_calls[calls][1]==1&&kernel_calls[calls][2]==out&&
        kernel_calls[calls][3]==attrs&&kernel_calls[calls][4]==type&&kernel_calls[calls][5]==state);
}
static void create_local_named(uint32_t out,uint32_t attrs,uint32_t desc,uint32_t payload,const char *name) {
    configure_named(out,attrs,desc,payload,name,0,0);g_esp=0x1f000;init_cpu();
    /* Local mutant argc is three: put owner at ESP+12; discard the event-only fourth word. */
    PUT32(g_esp+12,0);create_local_mutant();
}
static void create_local_unnamed(uint32_t out) {
    setargs(out,0,0,0);init_cpu();create_local_mutant();
}
static void profile_locks(void) {
    assert(vm_ready&&event_ready&&!local_mutant_ready);
    assert(local_profile(1)==0&&local_mutant_ready);
    assert(event_profile(0)==FF_BOOT_INVALID&&event_profile(1)==FF_BOOT_INVALID);
    assert(old_profile(0)==FF_BOOT_INVALID&&old_profile(1)==FF_BOOT_INVALID);
    assert(ff_probe_local_mutant_word(0xffffffffu,0)==1&&ff_probe_local_mutant_word(0xffffffffu,1)==0&&
        ff_probe_local_mutant_word(0xffffffffu,2)==0&&ff_probe_local_mutant_word(0xffffffffu,3)==FF_LOCAL_MUTANT_CAPACITY&&
        ff_probe_local_mutant_word(0xffffffffu,4)==0);
    assert(local_wait_profile(1)==0&&ff_probe_local_mutant_wait_word(0xffffffffu,0)==1&&
        ff_probe_local_mutant_wait_word(0xffffffffu,1)==0&&ff_probe_local_mutant_wait_word(0xffffffffu,2)==0&&
        ff_probe_local_mutant_wait_word(0xffffffffu,3)==0&&ff_probe_local_mutant_wait_word(0xffffffffu,4)==4);
    assert(ff_probe_event_word(0xffffffffu,0)==1&&ff_probe_event_word(0xffffffffu,1)==0&&
        ff_probe_event_word(0xffffffffu,2)==0&&ff_probe_event_word(0xffffffffu,3)==FF_EVENT_CAPACITY&&
        ff_probe_event_word(0xffffffffu,4)==0);
    assert(ff_probe_mutant_word(0xffffffffu,0)==0&&ff_probe_mutant_word(0xffffffffu,1)==0&&
        ff_probe_mutant_word(0xffffffffu,2)==FF_MUTANT_CAPACITY&&ff_probe_mutant_word(0xffffffffu,3)==0);
    assert_profiles_refuse_atomically(1);
}
static void wait_local_unowned(void) {
    reset_fixture();assert(event_profile(1)==0&&local_profile(1)==0);
    assert(local_wait_profile(1)==0);
    create_local_named(0x11000,0x12000,0x13000,0x10040,"Acquire");
    /* Alias handle shares object 1, as created by the existing case-insensitive name path. */
    create_local_named(0x11004,0x12000,0x13000,0x10040,"acquire");
    boot.count=1;boot.current=0;boot.threads[0].id=1;boot.threads[0].state=FF_BOOT_RUNNING;
    setargs(0x80004,1,0,0);init_cpu();wait_local_mutant();
    assert(g_eax==0&&g_esp==0x1f014&&local_mutants.objects[0].owner_thread==1);
    assert(local_mutant_wait_attempts==1&&local_mutant_wait_completed==1&&local_mutant_wait_status==0);
    assert(ff_probe_local_mutant_wait_word(0xffffffffu,5)==0x80004&&
        ff_probe_local_mutant_wait_word(0xffffffffu,6)==1&&ff_probe_local_mutant_wait_word(0xffffffffu,7)==1&&
        ff_probe_local_mutant_wait_word(0xffffffffu,8)==1&&ff_probe_local_mutant_wait_word(0xffffffffu,9)==0&&
        ff_probe_local_mutant_wait_word(0xffffffffu,10)==0&&ff_probe_local_mutant_wait_word(0xffffffffu,11)==0&&
        ff_probe_local_mutant_wait_word(0xffffffffu,12)==1&&ff_probe_local_mutant_wait_word(0xffffffffu,13)==0x11112222&&
        ff_probe_local_mutant_wait_word(0xffffffffu,14)==0x1f000&&ff_probe_local_mutant_wait_word(0xffffffffu,15)==0&&
        ff_probe_local_mutant_wait_word(0xffffffffu,16)==0x1f014);
    assert(ff_probe_local_mutant_wait_word(0,0)==1&&ff_probe_local_mutant_wait_word(0,1)==1&&
        ff_probe_local_mutant_word(0,3)==1);
    assert(kernel_calls[2][0]==234&&kernel_calls[2][1]==0&&kernel_calls[2][2]==0x80004&&
        kernel_calls[2][3]==1&&kernel_calls[2][4]==0&&kernel_calls[2][5]==0&&
        kernel_calls[2][6]==0x80004&&kernel_calls[2][7]==1&&kernel_calls[2][8]==1&&
        kernel_calls[2][9]==1&&kernel_calls[2][10]==0&&kernel_calls[2][11]==0&&
        kernel_calls[2][12]==0&&kernel_calls[2][13]==1&&kernel_calls[2][14]==0x11112222&&
        kernel_calls[2][15]==0x1f000&&kernel_calls[2][16]==0&&kernel_calls[2][17]==0x1f014);
    assert(local_wait_profile(0)==FF_BOOT_INVALID&&local_profile(0)==FF_BOOT_INVALID);
}
static void wait_owned_alias_refusal(void) {
    reset_fixture();assert(event_profile(1)==0&&local_profile(1)==0);
    assert(local_wait_profile(1)==0);
    create_local_named(0x11000,0x12000,0x13000,0x10040,"Acquire");
    create_local_named(0x11004,0x12000,0x13000,0x10040,"acquire");
    boot.count=1;boot.current=0;boot.threads[0].id=1;boot.threads[0].state=FF_BOOT_RUNNING;
    setargs(0x80004,1,0,0);init_cpu();wait_local_mutant();
    uint32_t owner=local_mutants.objects[0].owner_thread,completed=local_mutant_wait_completed;
    uint32_t eax=g_eax,esp=g_esp;setargs(0x80008,1,0,0);init_cpu();eax=g_eax;esp=g_esp;
    if(!setjmp(stopped)){wait_local_mutant();assert(0&&"owned alias wait must stop");}
    assert(stop_status==9&&stop_site==FF_BOOT_UNSUPPORTED&&stop_ordinal==234);
    assert(local_mutant_wait_attempts==2&&local_mutant_wait_completed==completed&&
        local_mutant_wait_status==FF_BOOT_UNSUPPORTED&&local_mutants.objects[0].owner_thread==owner);
    assert(g_eax==eax&&g_esp==esp&&ff_probe_local_mutant_wait_word(0xffffffffu,12)==1&&
        ff_probe_local_mutant_wait_word(0xffffffffu,15)==eax&&ff_probe_local_mutant_wait_word(0xffffffffu,16)==esp);
    assert(kernel_calls[3][0]==234&&kernel_calls[3][1]==1&&kernel_calls[3][2]==0x80008&&
        kernel_calls[3][3]==1&&kernel_calls[3][4]==0&&kernel_calls[3][5]==0&&
        kernel_calls[3][12]==1&&kernel_calls[3][13]==1&&kernel_calls[3][14]==eax&&
        kernel_calls[3][15]==esp&&kernel_calls[3][16]==eax&&kernel_calls[3][17]==esp);
    local_mutant_wait_sealed=1;
    assert(local_wait_profile(0)==FF_BOOT_INVALID);
}
static void namespace_and_creation(void) {
    create_local_unnamed(0x11004);
    assert(g_eax==0&&g_esp==0x1f010&&MEM32(0x11004)==0x80004&&
        kernel_calls[0][0]==192&&kernel_calls[0][1]==0&&kernel_calls[0][2]==0x11004&&
        kernel_calls[0][3]==0&&kernel_calls[0][4]==0&&kernel_calls[0][5]==0x11112222&&
        kernel_calls[0][6]==0x1f000&&kernel_calls[0][7]==0x80004&&kernel_calls[0][8]==1&&
        kernel_calls[0][9]==0&&kernel_calls[0][10]==1&&kernel_calls[0][11]==1&&
        kernel_calls[0][12]==0&&kernel_calls[0][13]==0x1f010&&kernel_calls[0][14]==0&&
        kernel_calls[0][15]==0&&kernel_calls[0][16]==0&&kernel_calls[0][17]==0&&kernel_calls[0][18]==0);
    create_local_named(0x11008,0x12000,0x13000,0x10040,"OwnedName");
    assert(g_eax==0&&g_esp==0x1f010&&MEM32(0x11008)==0x80008&&local_mutants.object_count==2);
    assert(kernel_calls[1][0]==192&&kernel_calls[1][1]==0&&kernel_calls[1][2]==0x11008&&
        kernel_calls[1][3]==0x12000&&kernel_calls[1][4]==0&&kernel_calls[1][5]==0x11112222&&
        kernel_calls[1][6]==0x1f000&&kernel_calls[1][7]==0x80008&&kernel_calls[1][8]==2&&
        kernel_calls[1][9]==0&&kernel_calls[1][10]==2&&kernel_calls[1][11]==2&&
        kernel_calls[1][12]==0&&kernel_calls[1][13]==0x1f010&&kernel_calls[1][14]==0xfffffffcu&&
        kernel_calls[1][15]==0x80&&kernel_calls[1][16]==9&&kernel_calls[1][17]==0x13000&&
        kernel_calls[1][18]==0x10040);
    /* Valid event descriptor alignment is four-byte; unlike the invalid 0x...01 case it passes. */
    expect_event_refusal(0x1100c,0x12000,0x13000,0x10060,"ownedname",FF_LOCAL_MUTANT_TYPE_MISMATCH,1,0);
    assert(named_events.object_count==0&&MEM32(0x1100c)==0xa5a5a5a5);
    expect_event_refusal(0x1100c,0x12000,0x13000,0x10060,"WrongType",FF_BOOT_UNSUPPORTED,0,0);
    expect_event_refusal(0x1100c,0x12000,0x13000,0x10060,"WrongState",FF_BOOT_UNSUPPORTED,1,1);
    configure_named(0x11010,0x12000,0x13000,0x10060,"Separate",1,0);create_named_event();
    assert(g_eax==0&&g_esp==0x1f014&&named_events.object_count==1&&named_events.handle_count==1);
    configure_named(0x1100c,0x12000,0x13000,0x10040,"OWNEDnAME",0,0);
    init_cpu();uint32_t duplicate_row=kernel_call_count;create_local_mutant();
    assert(g_eax==FF_LOCAL_MUTANT_NAME_EXISTS&&g_esp==0x1f010&&MEM32(0x1100c)==0x8000c&&
        local_mutants.object_count==2&&local_mutants.handle_count==3&&kernel_calls[duplicate_row][9]==1&&
        kernel_calls[duplicate_row][8]==2&&kernel_calls[duplicate_row][10]==2&&kernel_calls[duplicate_row][11]==3);
    assert(ff_probe_local_mutant_word(0,0)==1&&ff_probe_local_mutant_word(0,1)==0&&
        ff_probe_local_mutant_word(0,2)==0&&ff_probe_local_mutant_word(0,3)==0&&
        ff_probe_local_mutant_word(0,4)==0x80004&&ff_probe_local_mutant_word(0,5)==1&&
        ff_probe_local_mutant_word(1,0)==2&&ff_probe_local_mutant_word(1,1)==9&&
        ff_probe_local_mutant_word(1,2)==1&&ff_probe_local_mutant_word(1,4)==0x80008&&
        ff_probe_local_mutant_word(1,5)==2&&
        ff_probe_local_mutant_word(0xffffffffu,1)==2&&ff_probe_local_mutant_word(0xffffffffu,2)==3);
    create_local_unnamed(0x11014);
    assert(MEM32(0x11014)==0x80010&&local_mutants.object_count==3&&local_mutants.handle_count==4&&
        ff_probe_local_mutant_word(2,0)==3&&ff_probe_local_mutant_word(2,1)==0&&
        ff_probe_local_mutant_word(3,4)==0x80010&&ff_probe_local_mutant_word(3,5)==3);
    expect_local_refusal(0x11018,0x12000,0,FF_LOCAL_MUTANT_NO_SLOTS);
    assert(MEM32(0x11018)==0xa5a5a5a5&&ff_probe_local_mutant_word(0xffffffffu,0)==1&&
        ff_probe_local_mutant_word(0xffffffffu,1)==3&&ff_probe_local_mutant_word(0xffffffffu,2)==4&&
        ff_probe_local_mutant_word(0xffffffffu,3)==FF_LOCAL_MUTANT_CAPACITY&&
        ff_probe_local_mutant_word(0xffffffffu,4)==FF_LOCAL_MUTANT_NO_SLOTS);
}
static void invalid_inputs(void) {
    expect_local_refusal(0x11015,0x12000,0,FF_BOOT_INVALID);
    expect_local_refusal(0x20000,0x12000,0,FF_VM_ACCESS_VIOLATION);
    expect_local_refusal(0x11014,0x20000,0,FF_VM_ACCESS_VIOLATION);
    expect_local_refusal(0x11014,0x12001,0,FF_BOOT_INVALID);
    expect_local_refusal(0x11014,0,1,FF_BOOT_UNSUPPORTED);
    expect_local_refusal(0x11014,0x12000,1,FF_BOOT_UNSUPPORTED);
    /* output aliases the attributes, descriptor, or counted payload */
    expect_local_refusal(0x12000,0x12000,0,FF_BOOT_INVALID);
    configure_named(0x12000,0x11020,0x12000,0x10040,"AliasDesc",0,0);
    expect_local_refusal(0x12000,0x11020,0,FF_BOOT_INVALID);
    configure_named(0x11040,0x12000,0x13000,0x11040,"AliasData",0,0);
    expect_local_refusal(0x11040,0x12000,0,FF_BOOT_INVALID);
    configure_named(0x11014,0x12000,0x13001,0x10080,"Bad",0,0);PUT32(0x13000,0);
    expect_local_refusal(0x11014,0x12000,0,FF_BOOT_INVALID);
    configure_named(0x11014,0x12000,0x13000,0x20000,"Bad",0,0);
    expect_local_refusal(0x11014,0x12000,0,FF_VM_ACCESS_VIOLATION);
    configure_named(0x11014,0x12000,0x13000,0x10080,"Bad",0,0);PUT32(0x12000,0);
    expect_local_refusal(0x11014,0x12000,0,FF_BOOT_UNSUPPORTED);
    configure_named(0x11014,0x12000,0x13000,0x10080,"Bad",0,0);PUT32(0x12008,0);
    expect_local_refusal(0x11014,0x12000,0,FF_BOOT_UNSUPPORTED);
    configure_named(0x11014,0x12000,0x13000,0x10080,"Bad",0,0);guest_ram[0x13000]=0;guest_ram[0x13001]=0;
    expect_local_refusal(0x11014,0x12000,0,FF_BOOT_INVALID);
    configure_named(0x11014,0x12000,0x13000,0x10080,"Bad",0,0);guest_ram[0x13000]=4;guest_ram[0x13002]=2;
    expect_local_refusal(0x11014,0x12000,0,FF_BOOT_INVALID);
    configure_named(0x11014,0x12000,0x13000,0x10080,"Bad",0,0);memcpy(guest_ram+0x10080,"Bad/name",8);guest_ram[0x13000]=8;
    expect_local_refusal(0x11014,0x12000,0,FF_BOOT_INVALID);
}
static void legacy_path(void) {
    reset_fixture();assert(old_profile(1)==0&&mutant_ready);
    setargs(0x11000,0,0,0);create_unnamed_mutant();
    assert(g_eax==0&&g_esp==0x1f010&&MEM32(0x11000)==0x80004&&unnamed_mutants.count==1);
    assert(kernel_calls[0][0]==192&&kernel_calls[0][1]==0&&kernel_calls[0][2]==0x11000&&
        kernel_calls[0][3]==0&&kernel_calls[0][4]==0&&kernel_calls[0][5]==0x11112222&&
        kernel_calls[0][6]==0x1f000&&kernel_calls[0][7]==0x80004&&kernel_calls[0][10]==1);
    assert(kernel_calls[0][8]==0&&kernel_calls[0][9]==0x1f010);
    for(unsigned i=11;i<24;i++)assert(kernel_calls[0][i]==0);
}
static void profiles_and_flags(void) {
    reset_fixture();
    assert(local_wait_profile(1)==FF_BOOT_INVALID); /* ready local-mutant model required */
    assert(local_profile(1)==FF_BOOT_INVALID); /* event dependency */
    assert(event_profile(1)==0);assert(vm_ready&&event_ready);
    assert(old_profile(1)==0);assert(local_profile(1)==FF_BOOT_INVALID); /* legacy active */
    reset_fixture();assert(event_profile(1)==0);profile_locks();namespace_and_creation();invalid_inputs();
    /* Once a call was recorded, every profile mutation refuses atomically. */
    assert_profiles_refuse_atomically(0);
    reset_fixture();assert(event_profile(1)==0);assert(local_profile(1)==0);
    probe_trace_count=1;
    assert_profiles_refuse_atomically(0);
    probe_trace_count=0;parent_returned=1;
    assert_profiles_refuse_atomically(0);
    reset_fixture();assert(!event_ready&&!local_mutant_ready&&!mutant_ready&&event_status==0&&
        local_mutant_status==0&&mutant_status==0&&named_events.object_count==0&&named_events.handle_count==0&&
        local_mutants.object_count==0&&local_mutants.handle_count==0&&unnamed_mutants.count==0&&kernel_call_count==0&&
        ff_probe_event_word(0xffffffffu,0)==0&&ff_probe_local_mutant_word(0xffffffffu,0)==0&&
        ff_probe_mutant_word(0xffffffffu,0)==0);
}
static void event_first_collision(void) {
    reset_fixture();assert(event_profile(1)==0&&local_profile(1)==0);
    configure_named(0x11000,0x12000,0x13000,0x10040,"EventFirst",1,0);create_named_event();
    assert(g_eax==0&&named_events.object_count==1&&named_events.handle_count==1);
    configure_named(0x11004,0x12000,0x13000,0x10040,"eventfirst",0,0);
    expect_local_refusal(0x11004,0x12000,0,FF_LOCAL_MUTANT_TYPE_MISMATCH);
    assert(local_mutants.object_count==0&&local_mutants.handle_count==0&&named_events.object_count==1&&
        MEM32(0x11004)==0xa5a5a5a5);
}
static void invalid_model_refusal(void) {
    reset_fixture();assert(event_profile(1)==0&&local_profile(1)==0);
    create_local_named(0x11000,0x12000,0x13000,0x10040,"ModelError");
    local_mutants.objects[0].name_bytes=FF_LOCAL_MUTANT_NAME_BYTES+1u;
    expect_local_refusal(0x11004,0x12000,0,FF_BOOT_INVALID);
    assert(ff_probe_local_mutant_word(0xffffffffu,4)==FF_BOOT_INVALID&&local_mutants.handle_count==1);
}
int main(void) {
    reset_fixture();legacy_path();profiles_and_flags();event_first_collision();invalid_model_refusal();
    wait_local_unowned();wait_owned_alias_refusal();
    assert(ff_probe_event_word(0xffffffffu,99)==0&&ff_probe_local_mutant_word(0xffffffffu,99)==0&&
        ff_probe_local_mutant_word(99,0)==0&&ff_probe_local_mutant_word(0,99)==0);
    return 0;
}
'''
        (cls.path / "harness.c").write_text(harness, encoding="utf-8", newline="\n")

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_extracted_mixed_local_mutant_bridge(self):
        executable = self.path / "local-mutant-bridge"
        sources = [self.path / "harness.c", ROOT / "runtime" / "core" / "local_mutants.c",
                   ROOT / "runtime" / "core" / "named_events.c", ROOT / "runtime" / "core" / "unnamed_mutants.c",
                   ROOT / "runtime" / "core" / "virtual_memory.c",
                   ROOT / "runtime" / "core" / "thread_bootstrap.c"]
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                "-I", str(ROOT), *(str(p) for p in sources), "-o", str(executable)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual(run.returncode, 0, run.stderr)

    def test_local_bridge_compiles_with_proof_flag_off(self):
        source = self.path / "flag-off.c"
        obj = self.path / "flag-off.o"
        blocks = "\n".join(extract_blocks())
        source.write_text('#include <stdint.h>\n' + blocks + '\nint main(void){return 0;}\n',
                          encoding="utf-8", newline="\n")
        build = subprocess.run([compiler(), "-std=c11", "-Wall", "-Wextra", "-Werror",
                                "-I", str(ROOT), "-c", str(source), "-o", str(obj)],
                               capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)


if __name__ == "__main__":
    unittest.main()
