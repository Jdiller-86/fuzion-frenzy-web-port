#include "../src/runtime/console_settings.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[0x30000],saved[sizeof ram];
static ff_virtual_memory memory;
static uint32_t word(uint32_t at)
{ uint32_t result=0;for(unsigned i=0;i<4;++i)result|=(uint32_t)ram[at+i]<<(i*8u);return result; }
static void setup(void)
{
    memset(ram,0xa5,sizeof ram);assert(!ff_vm_init(&memory,ram,sizeof ram));
    assert(!ff_vm_claim_fixed(&memory,0x10000,0x10000));
    assert(!ff_vm_map_fixed(&memory,0x10000,0x1000,4));
    assert(!ff_vm_map_fixed(&memory,0x11000,0x1000,2));
}
int main(void)
{
    uint32_t args[5]={10,0x10010,0x10021,64,0x10018};
    setup();assert(!ff_query_console_dword(&memory,args,0x87654321));
    assert(word(args[1])==4 && word(args[2])==0x87654321 && word(args[4])==4);
    assert(ram[0x10020]==0xa5 && ram[0x10025]==0xa5 && ram[0x10060]==0xa5);
    args[0]=17;args[4]=0;assert(!ff_query_console_dword(&memory,args,0xffffffff));assert(word(args[2])==0xffffffff);
    assert(!ff_query_console_dword(&memory,args,0));assert(!word(args[2]));
    setup();args[0]=10;args[1]=0;args[2]=0xffffffff;args[3]=3;args[4]=0x10018;
    assert(ff_query_console_dword(&memory,args,1)==0xc0000023u && word(args[4])==4);
    assert(word(0x10010)==0xa5a5a5a5);
    args[4]=0;assert(ff_query_console_dword(&memory,args,1)==0xc0000023u);
    for(unsigned i=0;i<8;++i) {
        setup();uint32_t query[5]={10,0x10010,0x10020,4,0x10018};
        switch(i) {
            case 0:query[0]=7;break;
            case 1:query[1]=0x11000;break;
            case 2:query[2]=0x12000;break;
            case 3:query[2]=sizeof ram-2;break;
            case 4:query[4]=0x10019;break;
            case 5:query[2]=0x10012;break;
            case 6:query[4]=0x10020;break;
            case 7:query[1]=0;break;
        }
        memcpy(saved,ram,sizeof ram);assert(ff_query_console_dword(&memory,query,1));assert(!memcmp(saved,ram,sizeof ram));
    }
    puts("Owned console settings: explicit DWORD, permissions, short-buffer and atomic rejection checks passed");return 0;
}
