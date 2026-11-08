#include "local_mutants.h"
#include "thread_bootstrap.h"
#include <assert.h>
#include <string.h>

static ff_virtual_memory vm;
static ff_named_events events;
static ff_local_mutants mutants;
static uint8_t ram[2u*FF_VM_GRANULARITY];

static void setup(void)
{
    memset(ram,0,sizeof ram);
    assert(ff_vm_init(&vm,ram,sizeof ram)==FF_BOOT_OK);
    assert(ff_named_events_init(&events,&vm)==FF_BOOT_OK);
    assert(ff_local_mutants_init(&mutants,&vm,&events)==FF_BOOT_OK);
    mutants.object_count=1;
    mutants.handle_count=2;
    mutants.objects[0].name_bytes=4;
    memcpy(mutants.objects[0].name,"Test",4);
    mutants.handles[0]=(ff_local_mutant_handle){FF_LOCAL_MUTANT_HANDLE_BASE+4u,1};
    mutants.handles[1]=(ff_local_mutant_handle){FF_LOCAL_MUTANT_HANDLE_BASE+8u,1};
}

static void refused(uint32_t handle,uint32_t mode,uint32_t alertable,
                    uint32_t timeout,uint32_t thread,uint32_t expected)
{
    ff_local_mutants before=mutants;
    assert(ff_local_mutants_acquire(&mutants,handle,mode,alertable,timeout,thread)==expected);
    assert(memcmp(&mutants,&before,sizeof before)==0);
}

int main(void)
{
    setup();
    assert(ff_local_mutants_acquire(&mutants,FF_LOCAL_MUTANT_HANDLE_BASE+8u,1,0,0,1)==FF_BOOT_OK);
    assert(mutants.objects[0].owner_thread==1);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,0,0,1,FF_BOOT_UNSUPPORTED); /* recursive alias */
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,0,0,2,FF_BOOT_UNSUPPORTED); /* owned by another thread */
    mutants.objects[0].owner_thread=0;
    refused(0xdeadbeefu,1,0,0,1,FF_BOOT_BAD_HANDLE);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,0,0,0,1,FF_BOOT_UNSUPPORTED);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,2,0,0,1,FF_BOOT_UNSUPPORTED);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,1,0,1,FF_BOOT_UNSUPPORTED);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,0,1,1,FF_BOOT_UNSUPPORTED);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,0,0,0,FF_BOOT_INVALID);
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,0,0,5,FF_BOOT_INVALID);
    mutants.objects[0].owner_thread=5;
    refused(FF_LOCAL_MUTANT_HANDLE_BASE+4u,1,0,0,1,FF_BOOT_INVALID);
    assert(mutants.objects[0].owner_thread==5);
    setup();
    /* Creation remains unowned and still refuses initial ownership. */
    assert(mutants.objects[0].owner_thread==0);
    assert(ff_local_mutants_create(&mutants,0,0,1)==FF_BOOT_UNSUPPORTED);
    assert(mutants.objects[0].owner_thread==0);
    return 0;
}
