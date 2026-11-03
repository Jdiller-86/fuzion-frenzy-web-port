/* Owned page/accounting scenarios; no original code or console defaults. */
#include "../src/runtime/memory_statistics.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[2u*1024u*1024u],saved_ram[sizeof ram];
static ff_virtual_memory vm,saved_vm;
static ff_memory_statistics stats,saved_stats;
enum {BP=0x10000,SP=0x10004,OLD=0x10008,OUTPUT=0x10100};
static void put(uint32_t p,uint32_t value){for(unsigned i=0;i<4;++i)ram[p+i]=(uint8_t)(value>>(8*i));}
static uint32_t get(uint32_t p){return ram[p]|(uint32_t)ram[p+1]<<8|(uint32_t)ram[p+2]<<16|(uint32_t)ram[p+3]<<24;}
static void save(void){saved_vm=vm;saved_stats=stats;memcpy(saved_ram,ram,sizeof ram);}
static void unchanged(void){assert(!memcmp(&vm,&saved_vm,sizeof vm) && !memcmp(&stats,&saved_stats,sizeof stats) && !memcmp(ram,saved_ram,sizeof ram));}
static void reset(void)
{
    memset(ram,0xa5,sizeof ram);assert(!ff_vm_init(&vm,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&vm,0x10000,0x10000) && !ff_vm_map_fixed(&vm,0x10000,0x10000,4));
    assert(!ff_vm_claim_fixed(&vm,0x30000,0x5000));
    assert(!ff_vm_map_fixed(&vm,0x30000,0x1800,2) && !ff_vm_map_fixed(&vm,0x31000,0x1000,4));
    const ff_memory_profile profile={512,2};assert(!ff_memory_statistics_init(&stats,&vm,&profile));
    assert(!ff_memory_statistics_classify(&stats,0x10000,0x10000,FF_MEMORY_POOL));
    assert(!ff_memory_statistics_classify(&stats,0x1d000,0x3000,FF_MEMORY_STACK));
    assert(!ff_memory_statistics_classify(&stats,0x30000,0x5000,FF_MEMORY_IMAGE));
}
static void snapshot(uint32_t available,uint32_t committed,uint32_t reserved,uint32_t cache)
{
    uint32_t words[9],expected[9]={36,512,available,committed,reserved,cache,15,3,2};
    assert(!ff_memory_statistics_snapshot(&stats,words) && !memcmp(words,expected,sizeof words));
    assert(words[1]-words[2]==words[3]/4096+words[5]+words[6]+words[7]+words[8]);
}
static void reservations_and_commitment_have_distinct_units(void)
{
    reset();snapshot(492,0,0,0);
    put(BP,0);put(SP,65536);assert(!ff_vm_allocate(&vm,BP,0,SP,0x2000,4));const uint32_t base=get(BP);
    snapshot(492,0,65536,0);
    put(BP,base);put(SP,8192);assert(!ff_vm_allocate(&vm,BP,0,SP,0x1000,4));snapshot(490,8192,65536,0);
    put(BP,base);put(SP,8192);assert(!ff_vm_allocate(&vm,BP,0,SP,0x1000,4));snapshot(490,8192,65536,0);
    put(BP,base);put(SP,4096);assert(!ff_vm_protect(&vm,BP,SP,1,OLD));snapshot(490,8192,65536,0);
    put(BP,base);put(SP,4096);assert(!ff_vm_free(&vm,BP,SP,0x4000));snapshot(491,4096,65536,0);
    put(BP,base);put(SP,0);assert(!ff_vm_free(&vm,BP,SP,0x8000));snapshot(492,0,0,0);
    put(OUTPUT,36);ram[OUTPUT-1]=0x6a;ram[OUTPUT+36]=0x79;saved_vm=vm;saved_stats=stats;
    assert(!ff_memory_statistics_query(&stats,OUTPUT));
    const uint32_t expected[9]={36,512,492,0,0,0,15,3,2};
    for(unsigned i=0;i<9;++i)assert(get(OUTPUT+i*4)==expected[i]);
    assert(ram[OUTPUT-1]==0x6a && ram[OUTPUT+36]==0x79 && !memcmp(&vm,&saved_vm,sizeof vm) && !memcmp(&stats,&saved_stats,sizeof stats));
}
static void invalid_requests_and_unknown_ownership_preserve_state(void)
{
    reset();put(OUTPUT,35);save();assert(ff_memory_statistics_query(&stats,OUTPUT)==FF_BOOT_INVALID);unchanged();
    put(OUTPUT,36);save();assert(ff_memory_statistics_query(&stats,OUTPUT+1)==FF_BOOT_INVALID);unchanged();
    save();assert(ff_memory_statistics_query(&stats,sizeof ram-16)==FF_VM_ACCESS_VIOLATION);unchanged();
    vm.permission[OUTPUT/4096]=FF_VM_READ;save();assert(ff_memory_statistics_query(&stats,OUTPUT)==FF_VM_ACCESS_VIOLATION);unchanged();
    reset();put(OUTPUT,36);
    save();assert(ff_memory_statistics_classify(&stats,0x10000,0x11000,FF_MEMORY_IMAGE)==FF_BOOT_INVALID);unchanged();
    assert(!ff_vm_claim_fixed(&vm,0x50000,4096) && !ff_vm_map_fixed(&vm,0x50000,4096,4));
    uint32_t words[9],sentinel[9];memset(words,0xc7,sizeof words);memcpy(sentinel,words,sizeof words);
    save();assert(ff_memory_statistics_snapshot(&stats,words)==FF_BOOT_UNSUPPORTED && !memcmp(words,sentinel,sizeof words));unchanged();
    save();assert(ff_memory_statistics_query(&stats,OUTPUT)==FF_BOOT_UNSUPPORTED);unchanged();
    assert(!ff_memory_statistics_classify(&stats,0x50000,4096,FF_MEMORY_CACHE));snapshot(491,0,0,1);
    stats.profile.physical_pages=20;save();assert(ff_memory_statistics_query(&stats,OUTPUT)==FF_BOOT_UNSUPPORTED);unchanged();
    const ff_memory_profile bad={0,0};save();assert(ff_memory_statistics_init(&stats,&vm,&bad)==FF_BOOT_INVALID);unchanged();
}
int main(void)
{
    reservations_and_commitment_have_distinct_units();invalid_requests_and_unknown_ownership_preserve_state();
    puts("owned memory statistics, unique pages, lifecycle, units, budget and output checks passed");return 0;
}
