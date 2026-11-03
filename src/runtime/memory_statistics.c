#include "memory_statistics.h"
#include "thread_bootstrap.h"
#include <string.h>

uint32_t ff_memory_statistics_init(ff_memory_statistics *stats,ff_virtual_memory *memory,const ff_memory_profile *profile)
{
    if(!stats || !memory || !memory->ram || !profile || !profile->physical_pages ||
       profile->physical_pages>FF_VM_MAX_PAGES || profile->baseline_pool_pages>profile->physical_pages)return FF_BOOT_INVALID;
    memset(stats,0,sizeof *stats);stats->memory=memory;stats->profile=*profile;return 0;
}
uint32_t ff_memory_statistics_classify(ff_memory_statistics *stats,uint32_t base,uint32_t bytes,uint32_t role)
{
    if(!stats || !stats->memory || !bytes || ((base|bytes)&(FF_VM_PAGE-1u)) ||
       (uint64_t)base+bytes>stats->memory->bytes || role<FF_MEMORY_POOL || role>FF_MEMORY_CACHE)return FF_BOOT_INVALID;
    const uint32_t first=base/FF_VM_PAGE,end=first+bytes/FF_VM_PAGE;
    for(uint32_t p=first;p<end;++p) {
        const uint32_t owner=stats->memory->owner[p];
        if(!owner || owner>FF_VM_REGIONS || stats->memory->regions[owner-1].kind!=FF_VM_FIXED)return FF_BOOT_INVALID;
    }
    for(uint32_t p=first;p<end;++p)stats->fixed_role[p]=(uint8_t)role;
    return 0;
}
uint32_t ff_memory_statistics_snapshot(const ff_memory_statistics *stats,uint32_t words[FF_MEMORY_STATISTICS_WORDS])
{
    if(!stats || !stats->memory || !words || !stats->profile.physical_pages ||
       stats->profile.physical_pages>FF_VM_MAX_PAGES || stats->profile.baseline_pool_pages>stats->profile.physical_pages)return FF_BOOT_INVALID;
    const ff_virtual_memory *vm=stats->memory;
    uint32_t reserved=0,committed=0,counts[5]={0,stats->profile.baseline_pool_pages,0,0,0};
    for(uint32_t p=0;p<vm->bytes/FF_VM_PAGE;++p) {
        const uint32_t owner=vm->owner[p];
        if(!owner) {if(vm->committed[p])return FF_BOOT_INVALID;continue;}
        if(owner>FF_VM_REGIONS)return FF_BOOT_INVALID;
        const ff_vm_region *region=&vm->regions[owner-1];
        if((uint64_t)p*FF_VM_PAGE<region->base || (uint64_t)p*FF_VM_PAGE>= (uint64_t)region->base+region->bytes)return FF_BOOT_INVALID;
        if(region->kind==FF_VM_PRIVATE) {
            ++reserved;if(vm->committed[p])++committed;
        } else if(region->kind==FF_VM_FIXED) {
            if(vm->committed[p]) {
                const uint32_t role=stats->fixed_role[p];
                if(role<FF_MEMORY_POOL || role>FF_MEMORY_CACHE)return FF_BOOT_UNSUPPORTED;
                ++counts[role];
            }
        } else return FF_BOOT_INVALID;
    }
    const uint64_t used=(uint64_t)committed+counts[FF_MEMORY_POOL]+counts[FF_MEMORY_STACK]+counts[FF_MEMORY_IMAGE]+counts[FF_MEMORY_CACHE];
    if(used>stats->profile.physical_pages)return FF_BOOT_UNSUPPORTED;
    const uint32_t result[FF_MEMORY_STATISTICS_WORDS]={36,stats->profile.physical_pages,
        stats->profile.physical_pages-(uint32_t)used,committed*FF_VM_PAGE,reserved*FF_VM_PAGE,
        counts[FF_MEMORY_CACHE],counts[FF_MEMORY_POOL],counts[FF_MEMORY_STACK],counts[FF_MEMORY_IMAGE]};
    memcpy(words,result,sizeof result);return 0;
}
uint32_t ff_memory_statistics_query(ff_memory_statistics *stats,uint32_t output)
{
    if(!stats || !stats->memory || !output || (output&3u))return FF_BOOT_INVALID;
    ff_virtual_memory *vm=stats->memory;
    if(ff_vm_check(vm,output,36,FF_VM_READ|FF_VM_WRITE))return FF_VM_ACCESS_VIOLATION;
    const uint8_t *p=vm->ram+output;
    const uint32_t length=p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
    if(length!=36)return FF_BOOT_INVALID;
    uint32_t words[FF_MEMORY_STATISTICS_WORDS];
    uint32_t status=ff_memory_statistics_snapshot(stats,words);if(status)return status;
    for(unsigned i=0;i<FF_MEMORY_STATISTICS_WORDS;++i)for(unsigned b=0;b<4;++b)vm->ram[output+i*4+b]=(uint8_t)(words[i]>>(b*8));
    return 0;
}
