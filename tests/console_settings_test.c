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
    setup();uint32_t block_args[5]={0xff,0x10010,0x10020,0x100,0x10018};
    ff_console_settings_block block;memset(&block,0,sizeof block);
    block.bias=-480;memcpy(block.standard_name,"PST",4);memcpy(block.daylight_name,"PDT",4);
    block.standard_date=0x0b010200;block.daylight_date=0x03020200;
    block.standard_bias=-480;block.daylight_bias=-420;
    block.language=1;block.video=3;block.audio=0;block.parental=0;
    block.online[0]=block.online[1]=block.online[2]=block.online[3]=0;
    block.misc=0;block.dvd_region=1;
    assert(!ff_query_console_block(&memory,block_args,&block));
    assert(word(0x10010)==3 && word(0x10018)==0x60);
    assert(word(0x10020+0x04)==(uint32_t)-480);
    assert(!memcmp(ram+0x10020+0x08,"PST",4) && !memcmp(ram+0x10020+0x0c,"PDT",4));
    assert(word(0x10020+0x18)==0x0b010200u && word(0x10020+0x1c)==0x03020200u);
    assert(word(0x10020+0x28)==(uint32_t)-480 && word(0x10020+0x2c)==(uint32_t)-420);
    assert(word(0x10020+0x30)==1 && word(0x10020+0x34)==3 && word(0x10020+0x5c)==1);
    assert(word(0x10020+0x00)==0 && word(0x10020+0x10)==0 && word(0x10020+0x20)==0);
    assert(ram[0x10020+0x60]==0 && ram[0x10020+0xff]==0);
    setup();block_args[3]=0x40;memcpy(saved,ram,sizeof ram);
    assert(ff_query_console_block(&memory,block_args,&block)==0xc0000023u);
    assert(word(0x10010)==3 && word(0x10018)==0x60 && ram[0x10020]==0xa5);
    setup();block_args[3]=0x100;block_args[0]=17;
    memcpy(saved,ram,sizeof ram);assert(ff_query_console_block(&memory,block_args,&block));
    assert(!memcmp(saved,ram,sizeof ram));
    setup();block_args[0]=0xff;block_args[2]=0x12000;
    memcpy(saved,ram,sizeof ram);assert(ff_query_console_block(&memory,block_args,&block));
    assert(!memcmp(saved,ram,sizeof ram));
    setup();block_args[2]=0x10020;block_args[1]=0x10024;
    memcpy(saved,ram,sizeof ram);assert(ff_query_console_block(&memory,block_args,&block));
    assert(!memcmp(saved,ram,sizeof ram));
    puts("Owned console settings: explicit DWORD, block layout, permissions, short-buffer and atomic rejection checks passed");return 0;
}
