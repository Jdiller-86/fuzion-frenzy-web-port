#include "../runtime/core/console_settings.h"
#include "../runtime/core/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[0x30000],saved[sizeof ram];
static uint8_t configured_os[FF_CONSOLE_OS_SETTINGS_BYTES];
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
    for(unsigned i=0;i<sizeof configured_os;++i)configured_os[i]=(uint8_t)(i*37u+11u);
    {
        setup();uint32_t query[5]={255,0x10010,0x10100,256,0};
        uint32_t original_args[5];uint8_t original_configured[sizeof configured_os];
        memcpy(saved,ram,sizeof ram);
        memcpy(original_args,query,sizeof query);
        memcpy(original_configured,configured_os,sizeof configured_os);
        assert(!ff_query_console_os(&memory,query,configured_os));
        assert(word(query[1])==3u);
        assert(!memcmp(ram+query[2],configured_os,sizeof configured_os));
        for(unsigned i=FF_CONSOLE_OS_SETTINGS_BYTES;i<256u;++i)assert(ram[query[2]+i]==0);
        for(unsigned i=0;i<sizeof ram;++i)
            if((i>=query[1]&&i<query[1]+4u)||(i>=query[2]&&i<query[2]+256u))continue;
            else assert(ram[i]==saved[i]);
        assert(!memcmp(query,original_args,sizeof query));
        assert(!memcmp(configured_os,original_configured,sizeof configured_os));
    }
    {
        setup();uint32_t query[5]={255,0x10200,0x10100,256,0};
        assert(!ff_query_console_os(&memory,query,configured_os));
        assert(word(query[1])==3u&&!memcmp(ram+query[2],configured_os,sizeof configured_os));
    }
    for(unsigned i=0;i<12u;++i) {
        setup();uint32_t query[5]={255,0x10010,0x10100,256,0};
        switch(i) {
            case 0:query[0]=254;break;
            case 1:query[3]=255;break;
            case 2:query[3]=257;break;
            case 3:query[4]=0x10020;break;
            case 4:query[1]=0x10011;break;
            case 5:query[2]=0x10101;break;
            case 6:query[1]=0x101fc;break; /* overlaps the buffer's last four bytes */
            case 7:query[1]=0x10100;break;
            case 8:query[2]=0x10f80;break; /* crosses into read-only page */
            case 9:query[2]=0xffffff80u;break; /* widened range wrap */
            case 10:query[1]=0x11000;break; /* read-only type page */
            case 11:query[2]=0x11f80;break; /* extends beyond VM */
        }
        memcpy(saved,ram,sizeof ram);
        assert(ff_query_console_os(&memory,query,configured_os));
        assert(!memcmp(saved,ram,sizeof ram));
    }
    assert(ff_query_console_os(NULL,(uint32_t[5]){255,0x10010,0x10100,256,0},configured_os)==FF_BOOT_INVALID);
    assert(ff_query_console_os(&memory,NULL,configured_os)==FF_BOOT_INVALID);
    assert(ff_query_console_os(&memory,(uint32_t[5]){255,0x10010,0x10100,256,0},NULL)==FF_BOOT_INVALID);
    puts("Owned console settings: DWORD compatibility and synthetic OS settings exact-shape, padding, permissions, alias and atomic rejection checks passed");return 0;
}
