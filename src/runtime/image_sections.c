#include "image_sections.h"
#include "thread_bootstrap.h"
#include <string.h>

static uint32_t get(const uint8_t *p, uint32_t at, unsigned bytes)
{
    uint32_t value=0; for(unsigned i=0;i<bytes;++i)value|=(uint32_t)p[at+i]<<(8u*i); return value;
}
static void put(uint8_t *p,uint32_t at,uint32_t value,unsigned bytes)
{
    for(unsigned i=0;i<bytes;++i)p[at+i]=(uint8_t)(value>>(8u*i));
}
static int inside(uint32_t at,uint32_t bytes,uint32_t base,uint32_t size)
{
    return at>=base && bytes<=size && at-base<=size-bytes;
}
static int overlaps(uint32_t a,uint32_t bytes,uint32_t b,uint32_t other)
{
    return (uint64_t)a+bytes>b && (uint64_t)b+other>a;
}
static uint32_t shared_slot(ff_image_sections *image,uint32_t address,uint32_t page)
{
    for(uint32_t i=0;i<image->shared_count;++i) if(image->shared[i].address==address)
        return image->shared[i].page==page?i:FF_IMAGE_SHARED;
    if(image->shared_count==FF_IMAGE_SHARED) return FF_IMAGE_SHARED;
    uint32_t slot=image->shared_count++;
    image->shared[slot]=(ff_image_shared){address,page,0}; return slot;
}
static void map_span(ff_image_sections *image,uint32_t address,uint32_t bytes,uint8_t access,uint32_t flags)
{
    uint32_t low=address/FF_VM_PAGE,high=(address+bytes-1u)/FF_VM_PAGE;
    for(uint32_t page=low;page<=high;++page) {
        uint8_t permission=access;
        if((page==low && (flags&0x10u)) || (page==high && (flags&0x20u))) permission&=(uint8_t)~FF_VM_WRITE;
        image->memory->committed[page]=1; image->memory->permission[page]|=permission;
    }
}
static void mapping(ff_image_sections *image)
{
    uint32_t low=image->base/FF_VM_PAGE,high=(image->base+image->image_bytes-1u)/FF_VM_PAGE;
    for(uint32_t page=low;page<=high;++page) {
        image->memory->committed[page]=0; image->memory->permission[page]=0;
    }
    map_span(image,image->base,image->header_bytes,FF_VM_READ,0);
    for(uint32_t i=0;i<image->count;++i) {
        const ff_image_section *s=&image->sections[i];
        if(s->references) map_span(image,s->address,s->bytes,
            (uint8_t)(FF_VM_READ|((s->flags&1u)?FF_VM_WRITE:0)|((s->flags&4u)?FF_VM_EXECUTE:0)),s->flags);
    }
}
static void references(ff_image_sections *image)
{
    /* Kernel-owned reference updates do not grant translated code write access
     * to the image header. Every destination was validated before admission. */
    for(uint32_t i=0;i<image->count;++i) put(image->memory->ram,image->sections[i].descriptor+24,image->sections[i].references,4);
    for(uint32_t i=0;i<image->shared_count;++i) put(image->memory->ram,image->shared[i].address,image->shared[i].references,2);
}
static int intact(const ff_image_sections *image)
{
    const uint8_t *ram=image->memory->ram;
    if(get(ram,image->base+0x104,4)!=image->base || get(ram,image->base+0x108,4)!=image->header_bytes ||
       get(ram,image->base+0x10c,4)!=image->image_bytes || get(ram,image->base+0x11c,4)!=image->count ||
       get(ram,image->base+0x120,4)!=image->table) return 0;
    for(uint32_t i=0;i<image->count;++i) {
        const ff_image_section *s=&image->sections[i];uint32_t at=s->descriptor-image->base;
        if(memcmp(ram+s->descriptor,image->source+at,24) || memcmp(ram+s->descriptor+28,image->source+at+28,28) ||
           get(ram,s->descriptor+24,4)!=s->references) return 0;
    }
    for(uint32_t i=0;i<image->shared_count;++i) if(get(ram,image->shared[i].address,2)!=image->shared[i].references) return 0;
    for(uint32_t p=image->base/FF_VM_PAGE;p<=(image->base+image->image_bytes-1u)/FF_VM_PAGE;++p)
        if(image->memory->owner[p]!=image->owner) return 0;
    return 1;
}
uint32_t ff_image_init(ff_image_sections *out,ff_virtual_memory *memory,const uint8_t *source,uint32_t bytes)
{
    if(!out || !memory || !memory->ram || !source || bytes<0x178 || bytes>FF_IMAGE_SOURCE_BYTES || out->ready) return FF_BOOT_INVALID;
    /* Staging must be separate from guest bytes: loading or guest writes must
     * never alter the immutable source used for later reloads. */
    uintptr_t a=(uintptr_t)source,b=(uintptr_t)memory->ram;
    if((a<=b && bytes>b-a) || (b<a && memory->bytes>a-b)) return FF_BOOT_INVALID;
    if(get(source,0,4)!=0x48454258u) return FF_BOOT_INVALID;
    ff_image_sections image={0}; image.memory=memory;image.source=source;image.source_bytes=bytes;
    image.base=get(source,0x104,4);image.header_bytes=get(source,0x108,4);image.image_bytes=get(source,0x10c,4);
    image.table=get(source,0x120,4);image.count=get(source,0x11c,4);
    if(image.base<FF_VM_GRANULARITY || (image.base&(FF_VM_PAGE-1u)) || image.header_bytes<0x178 || image.header_bytes>bytes ||
       image.image_bytes<image.header_bytes || (uint64_t)image.base+image.image_bytes>memory->bytes || !image.count || image.count>FF_IMAGE_SECTIONS ||
       (image.table&3u) || image.table<image.base+0x178 || !inside(image.table,image.count*56,image.base,image.header_bytes)) return FF_BOOT_INVALID;
    image.owner=memory->owner[image.base/FF_VM_PAGE];
    if(!image.owner || image.owner>FF_VM_REGIONS || memory->regions[image.owner-1].kind!=FF_VM_FIXED) return FF_VM_CONFLICT;
    for(uint32_t p=image.base/FF_VM_PAGE;p<=(image.base+image.image_bytes-1u)/FF_VM_PAGE;++p)
        if(memory->owner[p]!=image.owner) return FF_VM_CONFLICT;
    if(memcmp(memory->ram+image.base,source,image.header_bytes)) return FF_BOOT_UNSUPPORTED;
    uint32_t certificate=get(source,0x118,4),certificate_bytes=0;
    if(certificate) {
        if(!inside(certificate,4,image.base,image.header_bytes)) return FF_BOOT_INVALID;
        certificate_bytes=get(source,certificate-image.base,4);
        if(certificate_bytes<4 || !inside(certificate,certificate_bytes,image.base,image.header_bytes)) return FF_BOOT_INVALID;
    }
    for(uint32_t i=0;i<image.count;++i) {
        ff_image_section *s=&image.sections[i];s->descriptor=image.table+i*56;
        uint32_t at=s->descriptor-image.base;
        s->flags=get(source,at,4);s->address=get(source,at+4,4);s->bytes=get(source,at+8,4);
        s->offset=get(source,at+12,4);s->file_bytes=get(source,at+16,4);s->name=get(source,at+20,4);
        if((s->flags&~0x3fu) || get(source,at+24,4)) return FF_BOOT_UNSUPPORTED;
        if(!s->bytes || !inside(s->address,s->bytes,image.base,image.image_bytes) || s->address<image.base+image.header_bytes ||
           s->file_bytes>s->bytes || !inside(s->offset,s->file_bytes,0,bytes) || (s->file_bytes && s->offset<image.header_bytes)) return FF_BOOT_INVALID;
        for(uint32_t j=0;j<i;++j) if(overlaps(s->address,s->bytes,image.sections[j].address,image.sections[j].bytes)) return FF_BOOT_INVALID;
        for(;;) {
            if(s->name_bytes==128 || !inside(s->name+s->name_bytes,1,image.base,image.header_bytes)) return FF_BOOT_INVALID;
            uint8_t byte=source[s->name-image.base+s->name_bytes++];
            if(!byte) break;
            if(byte<32 || byte>126) return FF_BOOT_UNSUPPORTED;
        }
        if(s->name_bytes==1) return FF_BOOT_INVALID;
        uint32_t pointers[2]={get(source,at+28,4),get(source,at+32,4)};
        for(unsigned n=0;n<2;++n) {
            uint32_t pointer=pointers[n];
            if((pointer&1u) || !inside(pointer,2,image.base,image.header_bytes) || overlaps(pointer,2,image.base,0x178) ||
               overlaps(pointer,2,image.table,image.count*56) || (certificate && overlaps(pointer,2,certificate,certificate_bytes))) return FF_BOOT_INVALID;
            if(get(source,pointer-image.base,2)) return FF_BOOT_UNSUPPORTED;
        }
        s->head=shared_slot(&image,pointers[0],s->address/FF_VM_PAGE);
        s->tail=shared_slot(&image,pointers[1],(s->address+s->bytes-1u)/FF_VM_PAGE);
        if(s->head==FF_IMAGE_SHARED || s->tail==FF_IMAGE_SHARED) return FF_BOOT_INVALID;
        s->references=(s->flags&2u)?1:0;
        if(s->references) {image.shared[s->head].references++;image.shared[s->tail].references++;}
    }
    for(uint32_t i=0;i<image.shared_count;++i) for(uint32_t j=0;j<image.count;++j)
        if(overlaps(image.shared[i].address,2,image.sections[j].name,image.sections[j].name_bytes)) return FF_BOOT_INVALID;
    /* All source/layout/ownership validation precedes the first guest change. */
    image.ready=1;*out=image;
    for(uint32_t i=0;i<out->count;++i) if(out->sections[i].references) {
        const ff_image_section *s=&out->sections[i];
        memset(memory->ram+s->address,0,s->bytes);memcpy(memory->ram+s->address,source+s->offset,s->file_bytes);
    }
    references(out);mapping(out);return 0;
}
static uint32_t resolve(ff_image_sections *image,uint32_t descriptor,ff_image_section **section)
{
    if(!image || !image->ready || !image->memory || !image->memory->ram) return FF_BOOT_INVALID;
    if(descriptor<image->table || (descriptor-image->table)%56 || (descriptor-image->table)/56>=image->count) return FF_BOOT_BAD_HANDLE;
    if(!intact(image)) return FF_BOOT_UNSUPPORTED;
    *section=&image->sections[(descriptor-image->table)/56];return 0;
}
uint32_t ff_image_load(ff_image_sections *image,uint32_t descriptor)
{
    ff_image_section *s;uint32_t status=resolve(image,descriptor,&s);if(status)return status;
    if(s->references==UINT32_MAX) return FF_BOOT_UNSUPPORTED;
    if(!s->references) {
        uint32_t delta=s->head==s->tail?2u:1u;
        if(image->shared[s->head].references>UINT16_MAX-delta || image->shared[s->tail].references>UINT16_MAX-delta) return FF_BOOT_UNSUPPORTED;
        memset(image->memory->ram+s->address,0,s->bytes);
        memcpy(image->memory->ram+s->address,image->source+s->offset,s->file_bytes);
        image->shared[s->head].references++;image->shared[s->tail].references++;
    }
    s->references++;references(image);mapping(image);return 0;
}
uint32_t ff_image_unload(ff_image_sections *image,uint32_t descriptor)
{
    ff_image_section *s;uint32_t status=resolve(image,descriptor,&s);if(status)return status;
    if(!s->references) return FF_BOOT_INVALID;
    if(s->references==1 && (image->shared[s->head].references<(s->head==s->tail?2u:1u) || !image->shared[s->tail].references)) return FF_BOOT_UNSUPPORTED;
    if(!--s->references) {image->shared[s->head].references--;image->shared[s->tail].references--;}
    /* Unload denies unshared pages but preserves backing bytes. Reload clears
     * only this section's range before copying immutable source again. */
    references(image);mapping(image);return 0;
}
