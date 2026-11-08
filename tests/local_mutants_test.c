#include "local_mutants.h"
#include "thread_bootstrap.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RAM_BYTES (32u*FF_VM_PAGE)
#define OA 0x10000u
#define DESC 0x10100u
#define DESC4 0x10104u
#define NAME 0x10201u
#define OUT 0x11000u
static ff_virtual_memory vm;static ff_named_events events;static ff_local_mutants mutants;static uint8_t ram[RAM_BYTES];
static void w32(uint32_t a,uint32_t v){ram[a]=(uint8_t)v;ram[a+1]=(uint8_t)(v>>8);ram[a+2]=(uint8_t)(v>>16);ram[a+3]=(uint8_t)(v>>24);}
static void request(uint32_t oa,uint32_t desc,uint32_t name,const char *s,uint32_t max){uint32_t n=(uint32_t)strlen(s);w32(oa,0xfffffffcu);w32(oa+4,desc);w32(oa+8,0x80);ram[desc]=(uint8_t)n;ram[desc+1]=(uint8_t)(n>>8);ram[desc+2]=(uint8_t)max;ram[desc+3]=(uint8_t)(max>>8);w32(desc+4,name);memcpy(ram+name,s,n);}
static void setup(void){memset(ram,0xa5,sizeof ram);assert(ff_vm_init(&vm,ram,sizeof ram)==0);assert(ff_vm_claim_fixed(&vm,0x10000,0x10000)==0);assert(ff_vm_map_fixed(&vm,0x10000,FF_VM_PAGE,2)==0);assert(ff_vm_map_fixed(&vm,0x11000,FF_VM_PAGE,4)==0);assert(ff_vm_map_fixed(&vm,0x12000,FF_VM_PAGE,4)==0);assert(ff_vm_map_fixed(&vm,0x13000,FF_VM_PAGE,2)==0);assert(ff_named_events_init(&events,&vm)==0);assert(ff_local_mutants_init(&mutants,&vm,&events)==0);request(OA,DESC,NAME,"Shared Name",12);}
static void atomic_create(uint32_t out,uint32_t oa,uint32_t owner,uint32_t want){ff_local_mutants before=mutants;uint8_t save[sizeof ram];memcpy(save,ram,sizeof ram);uint32_t got=ff_local_mutants_create(&mutants,out,oa,owner);if(got!=want)fprintf(stderr,"create expected %08x got %08x output %08x attrs %08x owner %08x\n",want,got,out,oa,owner);assert(got==want);assert(!memcmp(&before,&mutants,sizeof before));assert(!memcmp(save,ram,sizeof ram));}
static void atomic_guard(uint32_t out,uint32_t oa,uint32_t want){ff_local_mutants before=mutants;uint8_t save[sizeof ram];memcpy(save,ram,sizeof ram);assert(ff_local_mutants_guard_event(&mutants,out,oa)==want);assert(!memcmp(&before,&mutants,sizeof before));assert(!memcmp(save,ram,sizeof ram));}
int main(void){setup();
 assert(ff_local_mutants_create(&mutants,OUT,0,0)==0);assert(mutants.objects[0].name_bytes==0&&mutants.handles[0].handle==FF_LOCAL_MUTANT_HANDLE_BASE+4);
 assert(ff_local_mutants_create(&mutants,OUT+4,0,0)==0);assert(mutants.object_count==2&&mutants.handles[1].object==2);
 request(OA,DESC4,NAME,"Shared Name",12);assert(ff_local_mutants_create(&mutants,OUT+8,OA,0)==0);assert(mutants.object_count==3&&mutants.objects[2].name_bytes==11&&!memcmp(mutants.objects[2].name,"Shared Name",11));
 request(OA,DESC,NAME,"sHARED nAME",12);assert(ff_local_mutants_create(&mutants,OUT+12,OA,0)==FF_LOCAL_MUTANT_NAME_EXISTS);assert(mutants.object_count==3&&mutants.handle_count==4&&mutants.handles[3].object==3);
 request(OA,DESC,NAME,"Shared Name",12);atomic_create(OUT+16,OA,0,FF_LOCAL_MUTANT_NO_SLOTS);
 /* Existing event names block mutant creation through the shared namespace. */
 setup();assert(ff_named_events_create(&events,OUT+0x100,OA,1,0)==0);request(OA,DESC,NAME,"shared NAME",12);atomic_create(OUT+4,OA,0,FF_LOCAL_MUTANT_TYPE_MISMATCH);
 /* Mutant collision is available to the event bridge preflight, case folded. */
 setup();assert(ff_local_mutants_create(&mutants,OUT,OA,0)==0);request(OA,DESC,NAME,"SHARED NAME",12);atomic_guard(OUT+4,OA,FF_LOCAL_MUTANT_TYPE_MISMATCH);
 request(OA,DESC,NAME,"Other",8);assert(ff_local_mutants_guard_event(&mutants,OUT+4,OA)==0);
 atomic_create(OUT+1,0,0,FF_BOOT_INVALID);atomic_create(RAM_BYTES,0,0,FF_VM_ACCESS_VIOLATION);atomic_create(OUT,1,0,FF_BOOT_INVALID);atomic_create(OUT,OA,1,FF_BOOT_UNSUPPORTED);
 request(OA,DESC,NAME,"bad/name",12);atomic_create(OUT+4,OA,0,FF_BOOT_INVALID);request(OA,DESC,NAME,"Shared Name",65);atomic_create(OUT+4,OA,0,FF_BOOT_INVALID);
 request(OUT+0x100,OUT+0x200,OUT+0x301,"Other",8);atomic_create(OUT+0x100,OUT+0x100,0,FF_BOOT_INVALID); /* output/attributes alias */
 request(OUT+0x100,OUT+0x200,OUT+0x301,"Other",8);atomic_create(OUT+0x200,OUT+0x100,0,FF_BOOT_INVALID); /* output/descriptor alias */
 request(OUT+0x100,OUT+0x200,OUT+0x304,"Other",8);atomic_create(OUT+0x304,OUT+0x100,0,FF_BOOT_INVALID); /* output/name alias */
 atomic_create(0x13000,OA,0,FF_VM_ACCESS_VIOLATION);atomic_create(OUT+4,0x14000,0,FF_VM_ACCESS_VIOLATION);
 ff_local_mutants bad=mutants,before;bad.handles[0].handle++;before=bad;assert(ff_local_mutants_create(&bad,OUT+4,0,0)==FF_BOOT_INVALID&&!memcmp(&bad,&before,sizeof bad));
 bad=mutants;bad.objects[0].name_bytes=64;before=bad;assert(ff_local_mutants_create(&bad,OUT+4,0,0)==FF_BOOT_INVALID&&!memcmp(&bad,&before,sizeof bad));
 bad=mutants;bad.handles[0].object=0;before=bad;assert(ff_local_mutants_create(&bad,OUT+4,0,0)==FF_BOOT_INVALID&&!memcmp(&bad,&before,sizeof bad));
 setup();assert(ff_named_events_create(&events,OUT+0x100,OA,1,0)==0);mutants.object_count=1;mutants.handle_count=1;mutants.objects[0].name_bytes=11;memcpy(mutants.objects[0].name,"Shared Name",11);mutants.handles[0]=(ff_local_mutant_handle){FF_LOCAL_MUTANT_HANDLE_BASE+4,1};before=mutants;assert(ff_local_mutants_create(&mutants,OUT,0,0)==FF_BOOT_INVALID&&!memcmp(&mutants,&before,sizeof before));
 setup();mutants.object_count=1;mutants.handle_count=2;mutants.handles[0]=(ff_local_mutant_handle){FF_LOCAL_MUTANT_HANDLE_BASE+4,1};mutants.handles[1]=(ff_local_mutant_handle){FF_LOCAL_MUTANT_HANDLE_BASE+8,1};atomic_create(OUT,0,0,FF_BOOT_INVALID);
 assert(ff_named_events_create(&events,OUT+0x100,OA,1,0)==0);ff_named_events broken=events;broken.handles[0].handle++;assert(ff_local_mutants_create(&(ff_local_mutants){.memory=&vm,.events=&broken},OUT,0,0)==FF_BOOT_INVALID);
 ff_named_events other=events;ff_virtual_memory other_vm=vm;other.memory=&other_vm;assert(ff_local_mutants_init(&mutants,&vm,&other)==FF_BOOT_INVALID);
 assert(ff_local_mutants_init(NULL,&vm,&events)==FF_BOOT_INVALID);assert(ff_local_mutants_init((ff_local_mutants*)&vm,&vm,&events)==FF_BOOT_INVALID);
 ff_named_events event_snapshot=events;assert(ff_local_mutants_init((ff_local_mutants*)&events,&vm,&events)==FF_BOOT_INVALID&&!memcmp(&events,&event_snapshot,sizeof events));
 ff_named_events *guest_peer=(ff_named_events*)(ram+0x12000);memset(guest_peer,0,sizeof *guest_peer);guest_peer->memory=&vm;assert(ff_named_events_validate(guest_peer)==FF_BOOT_INVALID);assert(ff_local_mutants_init(&mutants,&vm,guest_peer)==FF_BOOT_INVALID);
 uint8_t alias[sizeof(ff_local_mutants)+2u*FF_VM_GRANULARITY];ff_virtual_memory avm;assert(ff_vm_init(&avm,alias,2u*FF_VM_GRANULARITY)==0);ff_named_events aev;assert(ff_named_events_init(&aev,&avm)==0);uint8_t snap[sizeof alias];memcpy(snap,alias,sizeof alias);assert(ff_local_mutants_init((ff_local_mutants*)alias,&avm,&aev)==FF_BOOT_INVALID&&!memcmp(alias,snap,sizeof alias));
 puts("local mutants: bounded mixed creation checks passed");return 0;}
