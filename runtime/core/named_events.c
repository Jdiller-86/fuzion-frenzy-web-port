#include "named_events.h"
#include "thread_bootstrap.h"
#include <stdint.h>
#include <stdalign.h>
#include <string.h>

#define STATUS_UNSUPPORTED FF_BOOT_UNSUPPORTED
#define ROOT_WIN32 0xfffffffcu
#define OPEN_IF 0x80u

static int span(uintptr_t start, size_t n, uintptr_t *end) { if (n > UINTPTR_MAX-start) return 0; *end=start+n; return 1; }
static int overlap(uintptr_t a, uintptr_t ae, uintptr_t b, uintptr_t be) { return a<be && b<ae; }
static int valid_memory(ff_virtual_memory *m) { return m && m->ram && m->bytes>=2u*FF_VM_GRANULARITY && m->bytes<=FF_VM_MAX_PAGES*FF_VM_PAGE && !(m->bytes&(FF_VM_PAGE-1u)); }
uint32_t ff_named_events_init(ff_named_events *m, ff_virtual_memory *vm)
{
    uintptr_t a,ae,b,be,c,ce;
    if (!m || !valid_memory(vm) || !span((uintptr_t)m,sizeof *m,&ae) || !span((uintptr_t)vm,sizeof *vm,&be) || !span((uintptr_t)vm->ram,vm->bytes,&ce)) return FF_BOOT_INVALID;
    a=(uintptr_t)m; b=(uintptr_t)vm; c=(uintptr_t)vm->ram;
    if (overlap(a,ae,b,be)||overlap(a,ae,c,ce)) return FF_BOOT_INVALID;
    memset(m,0,sizeof *m); m->memory=vm; return 0;
}
static unsigned char fold(unsigned char c) { return c>='A'&&c<='Z'?(unsigned char)(c+('a'-'A')):c; }
static int equal_name(const uint8_t *a,const uint8_t *b,uint32_t n) { for(uint32_t i=0;i<n;i++) if(fold(a[i])!=fold(b[i])) return 0; return 1; }
static uint32_t valid_model(const ff_named_events *m)
{
    uintptr_t a,ae,b,be,c,ce;
    if (!m || ((uintptr_t)m%alignof(ff_named_events)) || !span((uintptr_t)m,sizeof *m,&ae) ||
        !m->memory || ((uintptr_t)m->memory%alignof(ff_virtual_memory)) || !span((uintptr_t)m->memory,sizeof *m->memory,&be) ||
        !valid_memory(m->memory) || !span((uintptr_t)m->memory->ram,m->memory->bytes,&ce) ||
        m->object_count>FF_EVENT_CAPACITY || m->handle_count>FF_EVENT_CAPACITY || m->handle_count<m->object_count) return 0;
    a=(uintptr_t)m;b=(uintptr_t)m->memory;c=(uintptr_t)m->memory->ram;
    if (overlap(a,ae,b,be)||overlap(a,ae,c,ce)||overlap(b,be,c,ce)) return 0;
    for(uint32_t i=0;i<m->object_count;i++) {
        const ff_named_event_object *o=&m->objects[i];
        if(!o->name_bytes||o->name_bytes>FF_EVENT_NAME_BYTES) return 0;
        for(uint32_t k=0;k<o->name_bytes;k++) { uint8_t c=o->name[k]; if(c<0x20||c>0x7e||c=='/'||c=='\\'||c==':') return 0; }
        for(uint32_t j=0;j<i;j++) if(m->objects[j].name_bytes==o->name_bytes&&equal_name(m->objects[j].name,o->name,o->name_bytes)) return 0;
    }
    uint32_t seen=0, next=1;
    for(uint32_t i=0;i<m->handle_count;i++) {
        const ff_named_event_handle *h=&m->handles[i];
        if(h->handle!=FF_EVENT_HANDLE_BASE+4u*(i+1u)||!h->object||h->object>m->object_count) return 0;
        uint32_t bit=1u<<(h->object-1u);
        if(!(seen&bit)){if(h->object!=next)return 0;seen|=bit;next++;}
    }
    if(next!=m->object_count+1u) return 0;
    return 1;
}
uint32_t ff_named_events_validate(const ff_named_events *m)
{ return valid_model(m) ? 0u : FF_BOOT_INVALID; }
static uint32_t read32(const uint8_t *p) { return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24); }
uint32_t ff_named_events_create(ff_named_events *m,uint32_t output,uint32_t attrs,uint32_t type,uint32_t state)
{
    uint32_t st,root,desc,len,max; uint8_t name[64]; uintptr_t oe,ae,de,pe;
    if(!valid_model(m)) return FF_BOOT_INVALID;
    if(!output||(output&3u)) return FF_BOOT_INVALID;
    if(type!=1||state!=0) return STATUS_UNSUPPORTED;
    if(!attrs||(attrs&3u)) return FF_BOOT_INVALID;
    st=ff_vm_check(m->memory,output,4,FF_VM_WRITE); if(st) return st;
    st=ff_vm_check(m->memory,attrs,12,FF_VM_READ); if(st) return st;
    const uint8_t *ap=m->memory->ram+attrs; root=read32(ap); desc=read32(ap+4); uint32_t flags=read32(ap+8);
    if(root!=ROOT_WIN32||flags!=OPEN_IF) return STATUS_UNSUPPORTED;
    if(!desc||(desc&3u)) return FF_BOOT_INVALID;
    st=ff_vm_check(m->memory,desc,8,FF_VM_READ); if(st) return st;
    const uint8_t *dp=m->memory->ram+desc; len=(uint32_t)dp[0]|((uint32_t)dp[1]<<8); max=(uint32_t)dp[2]|((uint32_t)dp[3]<<8); uint32_t ptr=read32(dp+4);
    if(!ptr) return FF_BOOT_INVALID;
    if(len<1||len>FF_EVENT_NAME_BYTES||len>max||max>64) return FF_BOOT_INVALID;
    st=ff_vm_check(m->memory,ptr,len,FF_VM_READ); if(st) return st;
    if(!span(output,4,&oe)||!span(attrs,12,&ae)||!span(desc,8,&de)||!span(ptr,len,&pe)) return FF_BOOT_INVALID;
    if(overlap(output,oe,attrs,ae)||overlap(output,oe,desc,de)||overlap(output,oe,ptr,pe)) return FF_BOOT_INVALID;
    memcpy(name,m->memory->ram+ptr,len);
    for(uint32_t i=0;i<len;i++) if(name[i]<0x20||name[i]>0x7e||name[i]=='/'||name[i]=='\\'||name[i]==':') return FF_BOOT_INVALID;
    uint32_t existing=UINT32_MAX;
    for(uint32_t i=0;i<m->object_count;i++) if(m->objects[i].name_bytes==len&&equal_name(name,m->objects[i].name,len)){existing=i+1u;break;}
    if(m->handle_count==FF_EVENT_CAPACITY) return FF_EVENT_NO_SLOTS;
    if(existing==UINT32_MAX&&m->object_count==FF_EVENT_CAPACITY) return FF_EVENT_NO_SLOTS;
    uint32_t object=existing==UINT32_MAX?m->object_count+1u:existing;
    uint32_t handle=FF_EVENT_HANDLE_BASE+4u*(m->handle_count+1u);
    if(existing==UINT32_MAX){m->objects[object-1u].name_bytes=len;memcpy(m->objects[object-1u].name,name,len);m->object_count++;}
    m->handles[m->handle_count].handle=handle;m->handles[m->handle_count].object=object;m->handle_count++;
    uint8_t *op=m->memory->ram+output; op[0]=(uint8_t)handle;op[1]=(uint8_t)(handle>>8);op[2]=(uint8_t)(handle>>16);op[3]=(uint8_t)(handle>>24);
    return existing==UINT32_MAX?0:FF_EVENT_NAME_EXISTS;
}
