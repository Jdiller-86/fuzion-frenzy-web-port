#include "local_mutants.h"
#include "thread_bootstrap.h"
#include <stdint.h>
#include <string.h>
#include <stdalign.h>

#define ROOT_WIN32 0xfffffffcu
#define OPEN_IF 0x80u
static int span(uintptr_t a,size_t n,uintptr_t *e){if(n>UINTPTR_MAX-a)return 0;*e=a+n;return 1;}
static int overlap(uintptr_t a,uintptr_t ae,uintptr_t b,uintptr_t be){return a<be&&b<ae;}
static int memory_ok(const ff_virtual_memory *m){uintptr_t me,re;if(!m||((uintptr_t)m%alignof(ff_virtual_memory))||!m->ram||m->bytes<2u*FF_VM_GRANULARITY||m->bytes>FF_VM_MAX_PAGES*FF_VM_PAGE||(m->bytes&(FF_VM_PAGE-1u))||!span((uintptr_t)m,sizeof *m,&me)||!span((uintptr_t)m->ram,m->bytes,&re))return 0;return !overlap((uintptr_t)m,me,(uintptr_t)m->ram,re);}
static int events_ok(const ff_named_events *e){uintptr_t end;return e&&!((uintptr_t)e%alignof(ff_named_events))&&span((uintptr_t)e,sizeof *e,&end)&&ff_named_events_validate(e)==0;}
static uint32_t read32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static unsigned char fold(unsigned char c){return c>='A'&&c<='Z'?(unsigned char)(c+('a'-'A')):c;}
static int equal_name(const uint8_t *a,const uint8_t *b,uint32_t n){for(uint32_t i=0;i<n;i++)if(fold(a[i])!=fold(b[i]))return 0;return 1;}
static int name_valid(const uint8_t *p,uint32_t n){if(!n||n>FF_LOCAL_MUTANT_NAME_BYTES)return 0;for(uint32_t i=0;i<n;i++)if(p[i]<0x20||p[i]>0x7e||p[i]=='/'||p[i]=='\\'||p[i]==':')return 0;return 1;}

static int valid_model(const ff_local_mutants *m)
{
    if(!m||!memory_ok(m->memory)||!events_ok(m->events)||m->events->memory!=m->memory||m->object_count>4||m->handle_count>4||m->handle_count<m->object_count)return 0;
    for(uint32_t i=0;i<m->object_count;i++){
        const ff_local_mutant_object *o=&m->objects[i];
        if(o->name_bytes>FF_LOCAL_MUTANT_NAME_BYTES)return 0;
        if(o->name_bytes&&!name_valid(o->name,o->name_bytes))return 0;
        for(uint32_t j=0;j<i;j++)if(o->name_bytes&&m->objects[j].name_bytes==o->name_bytes&&equal_name(m->objects[j].name,o->name,o->name_bytes))return 0;
        if(o->name_bytes)for(uint32_t j=0;j<m->events->object_count;j++)if(m->events->objects[j].name_bytes==o->name_bytes&&equal_name(m->events->objects[j].name,o->name,o->name_bytes))return 0;
    }
    uint32_t seen=0,next=1;
    for(uint32_t i=0;i<m->handle_count;i++){
        const ff_local_mutant_handle *h=&m->handles[i];
        if(h->handle!=FF_LOCAL_MUTANT_HANDLE_BASE+4u*(i+1u)||!h->object||h->object>m->object_count)return 0;
        uint32_t bit=1u<<(h->object-1u);
        if(seen&bit){if(!m->objects[h->object-1u].name_bytes)return 0;}
        else {if(h->object!=next)return 0;seen|=bit;next++;}
    }
    return next==m->object_count+1u;
}
uint32_t ff_local_mutants_init(ff_local_mutants *m,ff_virtual_memory *vm,const ff_named_events *events)
{
    uintptr_t a,ae,b,be,c,ce,d,de;
    if(!m||((uintptr_t)m%alignof(ff_local_mutants))||!vm||((uintptr_t)vm%alignof(ff_virtual_memory))||!memory_ok(vm)||!events_ok(events)||events->memory!=vm)return FF_BOOT_INVALID;
    if(!span((uintptr_t)m,sizeof *m,&ae)||!span((uintptr_t)vm,sizeof *vm,&be)||!span((uintptr_t)vm->ram,vm->bytes,&ce)||!span((uintptr_t)events,sizeof *events,&de))return FF_BOOT_INVALID;
    a=(uintptr_t)m;b=(uintptr_t)vm;c=(uintptr_t)vm->ram;d=(uintptr_t)events;
    if(overlap(a,ae,b,be)||overlap(a,ae,c,ce)||overlap(a,ae,d,de))return FF_BOOT_INVALID;
    memset(m,0,sizeof *m);m->memory=vm;m->events=events;return 0;
}
static uint32_t parse_name(const ff_local_mutants *m,uint32_t out,uint32_t attrs,uint8_t name[64],uint32_t *len,uintptr_t *oe,uintptr_t *ae,uintptr_t *de,uintptr_t *pe)
{
    uint32_t st,desc;if(!out||(out&3u)||!attrs||(attrs&3u))return FF_BOOT_INVALID;
    st=ff_vm_check(m->memory,out,4,FF_VM_WRITE);if(st)return st;
    st=ff_vm_check(m->memory,attrs,12,FF_VM_READ);if(st)return st;
    const uint8_t *ap=m->memory->ram+attrs;uint32_t root=read32(ap);desc=read32(ap+4);uint32_t flags=read32(ap+8);
    if(root!=ROOT_WIN32||flags!=OPEN_IF)return FF_BOOT_UNSUPPORTED;
    if(!desc||(desc&3u))return FF_BOOT_INVALID;
    st=ff_vm_check(m->memory,desc,8,FF_VM_READ);if(st)return st;
    const uint8_t *dp=m->memory->ram+desc;uint32_t n=(uint32_t)dp[0]|((uint32_t)dp[1]<<8),max=(uint32_t)dp[2]|((uint32_t)dp[3]<<8),ptr=read32(dp+4);
    if(!ptr||!n||n>FF_LOCAL_MUTANT_NAME_BYTES||n>max||max>64)return FF_BOOT_INVALID;
    st=ff_vm_check(m->memory,ptr,n,FF_VM_READ);if(st)return st;
    if(!span(out,4,oe)||!span(attrs,12,ae)||!span(desc,8,de)||!span(ptr,n,pe))return FF_BOOT_INVALID;
    if(overlap(*oe-4,*oe,*ae-12,*ae)||overlap(*oe-4,*oe,*de-8,*de)||overlap(*oe-4,*oe,*pe-n,*pe))return FF_BOOT_INVALID;
    memcpy(name,m->memory->ram+ptr,n);if(!name_valid(name,n))return FF_BOOT_INVALID;*len=n;return 0;
}
static int event_collision(const ff_named_events *ev,const uint8_t *name,uint32_t n)
{for(uint32_t i=0;i<ev->object_count;i++)if(ev->objects[i].name_bytes==n&&equal_name(ev->objects[i].name,name,n))return 1;return 0;}
uint32_t ff_local_mutants_guard_event(const ff_local_mutants *m,uint32_t output,uint32_t attributes)
{
    uint8_t name[64];uint32_t n;uintptr_t oe,ae,de,pe;if(!valid_model(m))return FF_BOOT_INVALID;
    uint32_t st=parse_name(m,output,attributes,name,&n,&oe,&ae,&de,&pe);if(st)return st;
    for(uint32_t i=0;i<m->object_count;i++)if(m->objects[i].name_bytes==n&&equal_name(m->objects[i].name,name,n))return FF_LOCAL_MUTANT_TYPE_MISMATCH;
    return 0;
}
uint32_t ff_local_mutants_create(ff_local_mutants *m,uint32_t output,uint32_t attributes,uint32_t owner)
{
    uint8_t name[64];uint32_t n=0;uintptr_t oe=0,ae=0,de=0,pe=0;if(!valid_model(m))return FF_BOOT_INVALID;
    if(owner)return FF_BOOT_UNSUPPORTED;
    if(attributes){uint32_t st=parse_name(m,output,attributes,name,&n,&oe,&ae,&de,&pe);if(st)return st;if(event_collision(m->events,name,n))return FF_LOCAL_MUTANT_TYPE_MISMATCH;}
    else {if(!output||(output&3u))return FF_BOOT_INVALID;uint32_t st=ff_vm_check(m->memory,output,4,FF_VM_WRITE);if(st)return st;}
    if(m->handle_count==FF_LOCAL_MUTANT_CAPACITY)return FF_LOCAL_MUTANT_NO_SLOTS;
    uint32_t existing=UINT32_MAX;
    if(attributes)for(uint32_t i=0;i<m->object_count;i++)if(m->objects[i].name_bytes==n&&equal_name(m->objects[i].name,name,n)){existing=i;break;}
    if(existing==UINT32_MAX&&m->object_count==FF_LOCAL_MUTANT_CAPACITY)return FF_LOCAL_MUTANT_NO_SLOTS;
    uint32_t obj=existing==UINT32_MAX?m->object_count+1u:existing+1u;
    uint32_t h=FF_LOCAL_MUTANT_HANDLE_BASE+4u*(m->handle_count+1u);
    if(existing==UINT32_MAX){ff_local_mutant_object *o=&m->objects[obj-1u];o->name_bytes=n;if(n)memcpy(o->name,name,n);m->object_count++;}
    m->handles[m->handle_count].handle=h;m->handles[m->handle_count].object=obj;m->handle_count++;
    uint8_t *p=m->memory->ram+output;p[0]=(uint8_t)h;p[1]=(uint8_t)(h>>8);p[2]=(uint8_t)(h>>16);p[3]=(uint8_t)(h>>24);
    return existing==UINT32_MAX?0:FF_LOCAL_MUTANT_NAME_EXISTS;
}
