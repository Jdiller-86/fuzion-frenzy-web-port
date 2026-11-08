#include "named_events.h"
#include "thread_bootstrap.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RAM_BYTES (32u*FF_VM_PAGE)
#define OA 0x10000u
#define DESC 0x10100u
#define NAME 0x10201u
#define OUT 0x11000u
static ff_virtual_memory vm; static ff_named_events events; static uint8_t ram[RAM_BYTES];
static void w32(uint32_t a,uint32_t v){ram[a]=(uint8_t)v;ram[a+1]=(uint8_t)(v>>8);ram[a+2]=(uint8_t)(v>>16);ram[a+3]=(uint8_t)(v>>24);}
static void request(uint32_t oa,uint32_t desc,uint32_t name,const char *s,uint32_t max){uint32_t n=(uint32_t)strlen(s);w32(oa,0xfffffffcu);w32(oa+4,desc);w32(oa+8,0x80);ram[desc]=(uint8_t)n;ram[desc+1]=(uint8_t)(n>>8);ram[desc+2]=(uint8_t)max;ram[desc+3]=(uint8_t)(max>>8);w32(desc+4,name);memcpy(ram+name,s,n);}
static void setup(void){memset(ram,0xa5,sizeof ram);assert(ff_vm_init(&vm,ram,sizeof ram)==0);assert(ff_vm_claim_fixed(&vm,0x10000,0x10000)==0);assert(ff_vm_map_fixed(&vm,0x10000,FF_VM_PAGE,2)==0);assert(ff_vm_map_fixed(&vm,0x11000,FF_VM_PAGE,4)==0);assert(ff_vm_map_fixed(&vm,0x12000,FF_VM_PAGE,2)==0);assert(ff_named_events_init(&events,&vm)==0);request(OA,DESC,NAME,"Alpha Event",12);}
static void refused(uint32_t out,uint32_t oa,uint32_t type,uint32_t state,uint32_t want){ff_named_events before=events;uint8_t save[sizeof ram];memcpy(save,ram,sizeof ram);assert(ff_named_events_create(&events,out,oa,type,state)==want);assert(memcmp(&events,&before,sizeof events)==0);assert(memcmp(ram,save,sizeof ram)==0);}
int main(void){setup();
 assert(ff_named_events_create(&events,OUT,OA,1,0)==0);uint32_t h=FF_EVENT_HANDLE_BASE+4;assert(ram[OUT]==(uint8_t)h&&ram[OUT+1]==(uint8_t)(h>>8));assert(events.object_count==1&&events.handle_count==1);assert(events.objects[0].name_bytes==11&&!memcmp(events.objects[0].name,"Alpha Event",11));
 request(OA,DESC,NAME,"aLPHA eVENT",12);assert(ff_named_events_create(&events,OUT+4,OA,1,0)==FF_EVENT_NAME_EXISTS);assert(events.object_count==1&&events.handle_count==2&&events.handles[0].object==1&&events.handles[1].object==1&&events.handles[1].handle==FF_EVENT_HANDLE_BASE+8);assert(!memcmp(events.objects[0].name,"Alpha Event",11));assert(ram[OUT+4]==8&&ram[OUT+6]==9);
 /* Guest edits after creation do not mutate the private copied identity. */ memset(ram+NAME,0,11);request(OA,DESC,NAME,"ALPHA EVENT",12);assert(ff_named_events_create(&events,OUT+8,OA,1,0)==FF_EVENT_NAME_EXISTS);
 refused(OUT+12,OA,2,0,FF_BOOT_UNSUPPORTED);refused(OUT+12,OA,1,1,FF_BOOT_UNSUPPORTED);refused(0,OA,1,0,FF_BOOT_INVALID);refused(OUT+1,OA,1,0,FF_BOOT_INVALID);refused(RAM_BYTES,OA,1,0,FF_VM_ACCESS_VIOLATION);
 request(OA,DESC,NAME,"bad/name",12);refused(OUT+12,OA,1,0,FF_BOOT_INVALID);request(OA,DESC,NAME,"Alpha Event",65);refused(OUT+12,OA,1,0,FF_BOOT_INVALID);
 request(OA,DESC,NAME,"Alpha Event",5);refused(OUT+12,OA,1,0,FF_BOOT_INVALID);request(OA,DESC,NAME,"Alpha Event",12);ram[NAME+2]=0;refused(OUT+12,OA,1,0,FF_BOOT_INVALID);
 request(OA,DESC,NAME,"Alpha Event",12);w32(OA,0xfffffffdu);refused(OUT+12,OA,1,0,FF_BOOT_UNSUPPORTED);w32(OA,0xfffffffcu);w32(OA+8,0);refused(OUT+12,OA,1,0,FF_BOOT_UNSUPPORTED);w32(OA+8,0x80);
 request(OUT+0x100,OUT+0x200,OUT+0x301,"New Event",10);refused(OUT+0x100,OUT+0x100,1,0,FF_BOOT_INVALID); /* output aliases attributes */
 request(OUT+0x100,OUT+0x200,OUT+0x301,"New Event",10);refused(OUT+0x200,OUT+0x100,1,0,FF_BOOT_INVALID); /* output aliases descriptor */
 request(OUT+0x100,OUT+0x200,OUT+0x304,"New Event",10);refused(OUT+0x304,OUT+0x100,1,0,FF_BOOT_INVALID); /* output aliases payload */
 /* descriptor and payload must be readable; output must be writable. */
 refused(0x12000,OA,1,0,FF_VM_ACCESS_VIOLATION);refused(OUT+12,0x13000,1,0,FF_VM_ACCESS_VIOLATION);
 w32(OA+4,0x13000);refused(OUT+12,OA,1,0,FF_VM_ACCESS_VIOLATION);w32(OA+4,DESC);
 w32(DESC+4,0x13000);refused(OUT+12,OA,1,0,FF_VM_ACCESS_VIOLATION);w32(DESC+4,NAME);
 /* Fill handle table, then refuse atomically; duplicate opens also consume handles. */
 request(OA,DESC,NAME,"third",8);assert(ff_named_events_create(&events,OUT+12,OA,1,0)==0);request(OA,DESC,NAME,"fourth",8);assert(ff_named_events_create(&events,OUT+16,OA,1,0)==FF_EVENT_NO_SLOTS);assert(events.object_count==2&&events.handle_count==4);refused(OUT+20,OA,1,0,FF_EVENT_NO_SLOTS);
 ff_named_events bad=events,before;bad.handles[0].handle++;before=bad;assert(ff_named_events_create(&bad,OUT+20,OA,1,0)==FF_BOOT_INVALID);assert(!memcmp(&bad,&before,sizeof bad));bad=events;bad.objects[1]=bad.objects[0];before=bad;assert(ff_named_events_create(&bad,OUT+20,OA,1,0)==FF_BOOT_INVALID);assert(!memcmp(&bad,&before,sizeof bad));bad=events;bad.handles[0].object=0;before=bad;assert(ff_named_events_create(&bad,OUT+20,OA,1,0)==FF_BOOT_INVALID);assert(!memcmp(&bad,&before,sizeof bad));
 assert(ff_named_events_init(NULL,&vm)==FF_BOOT_INVALID);assert(ff_named_events_init(&events,NULL)==FF_BOOT_INVALID);ff_virtual_memory invalid=vm;invalid.ram=NULL;assert(ff_named_events_init(&events,&invalid)==FF_BOOT_INVALID);assert(ff_named_events_init((ff_named_events*)&vm,&vm)==FF_BOOT_INVALID);
 uint8_t alias_ram[sizeof(ff_named_events)+2u*FF_VM_GRANULARITY];ff_virtual_memory alias_vm;assert(ff_vm_init(&alias_vm,alias_ram,2u*FF_VM_GRANULARITY)==0);uint8_t snapshot[sizeof alias_ram];memcpy(snapshot,alias_ram,sizeof alias_ram);assert(ff_named_events_init((ff_named_events*)alias_ram,&alias_vm)==FF_BOOT_INVALID);assert(memcmp(alias_ram,snapshot,sizeof alias_ram)==0);
 puts("named events: bounded creation checks passed");return 0;}
