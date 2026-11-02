#include "../src/runtime/image_sections.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t ram[2u*1024u*1024u],saved[sizeof ram],source[0x6000],source_saved[sizeof source];
static ff_virtual_memory vm,saved_vm;
static ff_image_sections image;
static const uint32_t table=0x10300;
static void put(uint8_t *p,uint32_t at,uint32_t value,unsigned bytes)
{ for(unsigned i=0;i<bytes;++i)p[at+i]=(uint8_t)(value>>(8u*i)); }
static uint32_t get(uint32_t at,unsigned bytes)
{ uint32_t value=0;for(unsigned i=0;i<bytes;++i)value|=(uint32_t)ram[at+i]<<(8u*i);return value; }
static void setup(void)
{
    memset(source,0,sizeof source);memset(ram,0xcc,sizeof ram);memset(&image,0,sizeof image);
    put(source,0,0x48454258,4);put(source,0x104,0x10000,4);put(source,0x108,0x1000,4);put(source,0x10c,0x8000,4);
    put(source,0x11c,3,4);put(source,0x120,table,4);
    const uint32_t rows[3][9]={{7,0x11000,0x1800,0x1000,0x1000,0x10800,0,0x10a00,0x10a02},
        {0x38,0x12800,0x1800,0x3000,0x1300,0x10810,0,0x10a02,0x10a04},
        {1,0x14000,0x800,0x5000,0x200,0x10820,0,0x10a06,0x10a06}};
    for(unsigned i=0;i<3;++i) {
        for(unsigned n=0;n<9;++n)put(source,0x300+i*56+n*4,rows[i][n],4);
        for(uint32_t n=0;n<rows[i][4];++n)source[rows[i][3]+n]=(uint8_t)((n*17u+i*57u)^0xa5u);
    }
    memcpy(source+0x800,"OwnedBoot",10);memcpy(source+0x810,"OwnedImage",11);memcpy(source+0x820,"OwnedData",10);
    memcpy(ram+0x10000,source,0x1000);
    assert(!ff_vm_init(&vm,ram,sizeof ram));assert(!ff_vm_claim_fixed(&vm,0x10000,0x8000));
}
static void snapshot(void) { memcpy(saved,ram,sizeof ram);saved_vm=vm;memcpy(source_saved,source,sizeof source); }
static void unchanged(void) { assert(!memcmp(saved,ram,sizeof ram));assert(!memcmp(&saved_vm,&vm,sizeof vm));assert(!memcmp(source_saved,source,sizeof source)); }

static void lifecycle(void)
{
    setup();memcpy(source_saved,source,sizeof source);
    assert(!ff_image_init(&image,&vm,source,sizeof source));
    assert(image.count==3 && image.sections[0].references==1 && !image.sections[1].references);
    assert(get(table+24,4)==1 && get(0x10a00,2)==1 && get(0x10a02,2)==1);
    assert(!memcmp(ram+0x11000,source+0x1000,0x1000));
    for(uint32_t at=0x12000;at<0x12800;++at)assert(!ram[at]);
    assert(ram[0x12800]==0xcc && ff_vm_check(&vm,0x13000,1,FF_VM_READ));
    assert(ff_vm_check(&vm,table+24,4,FF_VM_WRITE));
    assert(!ff_image_load(&image,table+56));
    assert(get(table+56+24,4)==1 && get(0x10a02,2)==2 && get(0x10a04,2)==1);
    assert(!memcmp(ram+0x12800,source+0x3000,0x1300));
    for(uint32_t at=0x13b00;at<0x14000;++at)assert(!ram[at]);
    assert(!ff_vm_check(&vm,0x12800,0x1800,FF_VM_READ));
    assert(ff_vm_check(&vm,0x12800,0x1800,FF_VM_WRITE));assert(ff_vm_check(&vm,0x13000,1,FF_VM_EXECUTE));
    ram[0x13010]^=0x5a;uint8_t dirty=ram[0x13010];
    assert(!ff_image_load(&image,table+56) && ram[0x13010]==dirty && get(table+56+24,4)==2 && get(0x10a02,2)==2);
    assert(!ff_image_unload(&image,table+56) && !ff_vm_check(&vm,0x13000,1,FF_VM_READ));
    assert(!ff_image_unload(&image,table+56) && ram[0x13010]==dirty && ff_vm_check(&vm,0x13000,1,FF_VM_READ));
    assert(get(0x10a02,2)==1 && !get(0x10a04,2) && !ff_vm_check(&vm,0x12010,1,FF_VM_EXECUTE|FF_VM_WRITE));
    ram[0x11009]=0x42;
    assert(!ff_image_load(&image,table+56) && ram[0x11009]==0x42 && !memcmp(ram+0x12800,source+0x3000,0x1300));
    assert(!ff_image_unload(&image,table) && ff_vm_check(&vm,0x11000,1,FF_VM_READ));
    assert(!ff_vm_check(&vm,0x12010,1,FF_VM_READ) && ff_vm_check(&vm,0x12010,1,FF_VM_EXECUTE|FF_VM_WRITE));
    assert(!ff_image_unload(&image,table+56) && ff_vm_check(&vm,0x12010,1,FF_VM_READ));
    assert(!ff_image_load(&image,table+112) && get(0x10a06,2)==2 && !ff_vm_check(&vm,0x14000,0x800,FF_VM_READ|FF_VM_WRITE));
    assert(!ff_image_unload(&image,table+112) && !get(0x10a06,2) && ff_vm_check(&vm,0x14000,1,FF_VM_READ));
    assert(!ff_image_load(&image,table) && !memcmp(ram+0x11000,source+0x1000,0x1000));
    assert(!memcmp(source,source_saved,sizeof source));
}
static void rejected_operations(void)
{
    setup();assert(!ff_image_init(&image,&vm,source,sizeof source));snapshot();
    for(unsigned i=0;i<4;++i) {
        uint32_t descriptor=(uint32_t[]){0,table+1,table+168,0xfffffffcu}[i];
        assert(ff_image_load(&image,descriptor)==FF_BOOT_BAD_HANDLE);unchanged();
        assert(ff_image_unload(&image,descriptor)==FF_BOOT_BAD_HANDLE);unchanged();
    }
    assert(ff_image_unload(&image,table+56)==FF_BOOT_INVALID);unchanged();
    put(ram,table+4,0x17000,4);snapshot();assert(ff_image_load(&image,table)==FF_BOOT_UNSUPPORTED);unchanged();
    setup();assert(!ff_image_init(&image,&vm,source,sizeof source));put(ram,0x10a02,8,2);snapshot();
    assert(ff_image_load(&image,table+56)==FF_BOOT_UNSUPPORTED);unchanged();
}
static void rejected_images(void)
{
    const uint32_t mutations[][2]={{0,0},{0x108,0x100},{0x11c,129},{0x120,0x10ff0},{0x300+56+4,0x11000},
        {0x300+56+8,0xfffffff0},{0x300+56+16,0x2000},{0x300+56+28,table},{0x300+56+20,0x20000},
        {0x300+56,0x80},{0x300+24,1},{0xa02,1}};
    for(unsigned i=0;i<sizeof mutations/sizeof *mutations;++i) {
        setup();put(source,mutations[i][0],mutations[i][1],4);memcpy(ram+0x10000,source,0x1000);snapshot();
        assert(ff_image_init(&image,&vm,source,sizeof source));unchanged();assert(!image.ready);
    }
    setup();snapshot();assert(ff_image_init(&image,&vm,source,0x4000));unchanged();
    setup();vm.owner[0x13000/FF_VM_PAGE]=0;snapshot();assert(ff_image_init(&image,&vm,source,sizeof source)==FF_VM_CONFLICT);unchanged();
    setup();ram[0x10000]^=1;snapshot();assert(ff_image_init(&image,&vm,source,sizeof source)==FF_BOOT_UNSUPPORTED);unchanged();
    setup();snapshot();assert(ff_image_init(&image,&vm,ram+0x10000,0x6000)==FF_BOOT_INVALID);unchanged();
}
static void zero_fill_section(void)
{
    setup();put(source,0x300+112+12,0,4);put(source,0x300+112+16,0,4);memcpy(ram+0x10000,source,0x1000);
    assert(!ff_image_init(&image,&vm,source,sizeof source));assert(!ff_image_load(&image,table+112));
    for(uint32_t at=0x14000;at<0x14800;++at)assert(!ram[at]);
}
int main(void)
{
    lifecycle();rejected_operations();rejected_images();zero_fill_section();
    puts("Owned image preload/demand/reference/shared-page lifecycle checks passed");return 0;
}
