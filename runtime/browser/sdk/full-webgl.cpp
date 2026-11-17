// Browser GPU bridge. All JS operations complete synchronously on the runtime
// worker before their owned input buffers can be reused by the guest thread.
#include "full-webgl.h"
#include "full-memory.h"
#include "gpu/texture.h"
#include "gpu/shaders.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <array>
#include <deque>
#include <map>
#include <memory>
#include <vector>
#include <emscripten.h>
#include <emscripten/threading.h>

namespace {
struct Target { gpu::TextureDesc desc; uint64_t hash = 0, epoch = 0, checked = ~0ull; };
std::map<uint32_t, Target> targets;
uint64_t frame = 0;
uint32_t last_vertex_constants=0;
std::vector<std::array<uint32_t,12>> commands;
struct Block { std::unique_ptr<uint8_t[]> bytes; size_t capacity,used=0; };
std::vector<Block> owned;
size_t owned_block=0,owned_capacity=0;
std::map<uint32_t,std::array<uint32_t,3>> image_copies;
size_t owned_bytes = 0;
uint32_t Own(const void* src, size_t bytes) {
    if(bytes > 128u*1024*1024 || owned_bytes + bytes > 256u*1024*1024) std::abort();
    const size_t span=(bytes+7)&~size_t(7);
    while(owned_block<owned.size() && owned[owned_block].capacity-owned[owned_block].used<span) ++owned_block;
    if(owned_block==owned.size()) {
        size_t capacity=std::max<size_t>(4u*1024*1024,span);
        if(owned_capacity+capacity>256u*1024*1024)std::abort();
        owned.push_back({std::make_unique<uint8_t[]>(capacity),capacity,0});owned_capacity+=capacity;
    }
    auto& block=owned[owned_block];auto* dst=block.bytes.get()+block.used;block.used+=span;
    std::memcpy(dst,src,bytes);owned_bytes+=span;
    return uint32_t(reinterpret_cast<uintptr_t>(dst));
}
void FlushCommands() {
    if(commands.empty()) return;
    if(!MAIN_THREAD_EM_ASM_INT({ return Module.onFuzionGpu.commands($0,$1)?1:0; }, commands.data(),uint32_t(commands.size())))std::abort();
    commands.clear();for(auto& block:owned)block.used=0;owned_block=0;image_copies.clear();owned_bytes=0;last_vertex_constants=0;
}
// Use the checked backing identity: physical/write-combine views alias each
// other, but user/system/stack allocations at the same offset do not.
uint32_t Key(uint32_t p) { return uint32_t(ff_sdk_guest_ptr(p,1)-ff_sdk_guest_base()); }
uint32_t Offset(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    uint32_t o = 0, bit = 0;
    for (uint32_t m = 1; m < w || m < h; m <<= 1) {
        if (m < w) { if (x & m) o |= 1u << bit; ++bit; }
        if (m < h) { if (y & m) o |= 1u << bit; ++bit; }
    }
    return o;
}
}
bool ff_webgl_enabled() {
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({ return Module.onFuzionGpu ? 1 : 0; }) != 0;
    return enabled;
}
bool ff_webgl_gpu_vertex_enabled() {
    static const bool enabled=MAIN_THREAD_EM_ASM_INT({return Module.onFuzionGpu?.gpuVertices?1:0;})!=0;
    return enabled;
}
const char* ff_webgl_vertex_source(const gpu::State& s, uint32_t regs) {
    struct Source {std::vector<uint32_t> words;uint32_t regs;std::string text;};
    static std::map<uint64_t,std::vector<Source>> cache;
    const auto& words=s.vs->program;
    auto hash=gpu::Hash(words.data(),words.size()*4)^uint64_t(regs);
    auto& bucket=cache[hash];
    for(auto& source:bucket)if(source.regs==regs&&source.words==words)return source.text.c_str();
    if(cache.size()>1024)std::abort();
    bucket.push_back({words,regs,gpu::VertexProgramHlsl(words,uint16_t(regs))});
    return bucket.back().text.c_str();
}
const uint32_t* ff_webgl_palette(uint32_t header, uint64_t& hash) {
    if(!header) return nullptr;
    uint32_t descriptor[2];std::memcpy(descriptor,ff_sdk_guest_ptr(header,8),8);
    const size_t bytes=(256u>>(descriptor[0]>>30))*4;
    const uint32_t data=descriptor[1]<64u*1024*1024?descriptor[1]|0x80000000u:descriptor[1];
    struct Palette {std::array<uint32_t,256> colors{};uint64_t checked=~0ull,epoch=0,hash=0;uint32_t data=0,bytes=0;};
    static std::map<uint32_t,Palette> cache;
    if(cache.size()>=2048&&!cache.count(header))std::abort();
    auto& p=cache[header];const auto epoch=gpu::DirtyEpoch(data);
    if(p.checked!=frame||p.epoch!=epoch||p.data!=data||p.bytes!=bytes) {
        p.colors.fill(0);std::memcpy(p.colors.data(),ff_sdk_guest_ptr(data,bytes),bytes);
        p.hash=gpu::Hash(p.colors.data(),sizeof(p.colors));p.checked=frame;p.epoch=epoch;p.data=data;p.bytes=uint32_t(bytes);
    }
    hash=p.hash;return p.colors.data();
}
void ff_webgl_advance_frame() { ++frame; }
void ff_webgl_target(const gpu::Surface& surface, bool depth) {
    gpu::TextureDesc d;
    if (!gpu::DescribeSurface(surface, d) || !d.width || !d.height || d.width > 4096 || d.height > 4096) std::abort();
    auto size = gpu::TotalSize(d);
    auto* guest = ff_sdk_guest_ptr(d.data, size);
    auto& target = targets[Key(d.data)];
    const bool changed = target.checked == ~0ull || target.desc.width != d.width || target.desc.height != d.height || target.desc.format != d.format || target.desc.pitch != d.pitch || target.desc.linear != d.linear;
    const uint64_t old_hash = target.hash;
    uint64_t epoch = gpu::DirtyEpoch(d.data);
    if(target.checked != frame || target.epoch != epoch) {
        target.hash = gpu::Hash(guest,size) ^ (epoch * 0x9e3779b97f4a7c15ull); target.checked = frame; target.epoch = epoch;
    }
    target.desc = d;
    if(changed || old_hash != target.hash) {
        commands.push_back({0,Key(d.data),d.width,d.height,d.format,d.pitch,d.linear,uint32_t(depth),Own(guest,size),size,uint32_t(target.hash),uint32_t(target.hash>>32)});
    }
}
bool ff_webgl_draw(const void* v, uint32_t count, uint32_t stride, const void* indices, uint32_t n,
                   const uint32_t* state, const float* uniforms, const char* shader, const uint32_t* textures) {
    uint32_t tx[4*14];std::memcpy(tx,textures,sizeof(tx));
    uint32_t states[40];std::memcpy(states,state,sizeof(states));
    states[0]=state[0]?Key(state[0]):0;states[1]=state[1]?Key(state[1]):0;
    if(states[37]) {
        const auto* constants=reinterpret_cast<const void*>(states[38]);
        if(!last_vertex_constants||std::memcmp(reinterpret_cast<const void*>(last_vertex_constants),constants,772*4))
            last_vertex_constants=Own(constants,772*4);
        states[38]=last_vertex_constants;
    }
    for(int t=0;t<4;++t) {
        auto* q = tx+t*14;
        if(!q[0] || !q[6])continue;
        q[2]=Key(q[2]);
        if(targets.count(q[2])) continue;
        auto found = image_copies.find(q[6]);
        if(found == image_copies.end()) {
            size_t bytes=size_t(q[3])*q[4]*16;
            auto copy = Own(reinterpret_cast<const void*>(q[6]),bytes);
            auto hash = gpu::Hash(reinterpret_cast<const void*>(copy),bytes);
            image_copies[q[6]]={copy,uint32_t(hash),uint32_t(hash>>32)};
            q[6]=copy;q[11]=uint32_t(hash);q[12]=uint32_t(hash>>32);
        } else {q[6]=found->second[0];q[11]=found->second[1];q[12]=found->second[2];}
    }
    commands.push_back({1,Own(v,size_t(count)*stride),count,stride,Own(indices,size_t(n)*4),n,
        Own(states,sizeof(states)),Own(uniforms,152*4),uint32_t(reinterpret_cast<uintptr_t>(shader)),Own(tx,sizeof(tx))});
    return true;
}
void ff_webgl_clear(const gpu::State& s, uint32_t n, const uint8_t* rects, uint32_t flags, uint32_t color, float z, uint32_t stencil) {
    uint32_t color_id=0,depth_id=0;gpu::TextureDesc d;
    if (flags & 0xf0) {ff_webgl_target(s.rt, false);if(gpu::DescribeSurface(s.rt,d))color_id=Key(d.data);}
    if (flags & 3) {ff_webgl_target(s.zs, true);if(gpu::DescribeSurface(s.zs,d))depth_id=Key(d.data);}
    uint32_t area[] = {s.viewport.x,s.viewport.y,s.viewport.w,s.viewport.h};
    uint32_t zbits;std::memcpy(&zbits,&z,4);
    commands.push_back({2,color_id,depth_id,n,n?Own(rects,size_t(n)*16):0,flags,color,zbits,stencil,Own(area,sizeof(area))});
}
bool ff_webgl_present(const gpu::Surface& front) {
    gpu::TextureDesc d;
    if (!gpu::DescribeSurface(front, d)) return false;
    FlushCommands();
    bool shown = MAIN_THREAD_EM_ASM_INT({ return Module.onFuzionGpu.present($0) ? 1 : 0; }, Key(d.data));
    return shown;
}
void ff_webgl_sync(uint32_t data, uint32_t size) {
    FlushCommands();
    auto found = targets.find(Key(data));
    if (found == targets.end()) return;
    const auto& d = found->second.desc;
    const auto bytes = gpu::TotalSize(d);
    if (!size || size < bytes) return; // SDK contract: refuse an unbounded readback.
    std::vector<uint8_t> rgba(size_t(d.width) * d.height * 4);
    int read = MAIN_THREAD_EM_ASM_INT({ try { return Module.onFuzionGpu.read($0,$1) ? 1 : 0; } catch(e) { postMessage({type:'gpu-error',text:e.message}); return 0; } }, Key(data), rgba.data());
    if (!read) std::abort();
    auto* dst = ff_sdk_guest_ptr(d.data, bytes);
    const auto& fmt = gpu::Format(d.format);
    const uint32_t bpp = fmt.bits / 8;
    for (uint32_t y=0;y<d.height;++y) for(uint32_t x=0;x<d.width;++x) {
        auto* src = rgba.data() + (size_t(y)*d.width+x)*4;
        auto* p = dst + (d.linear ? y*d.pitch+x*bpp : Offset(x,y,d.width,d.height)*bpp);
        if (bpp == 4) { p[0]=src[2];p[1]=src[1];p[2]=src[0];p[3]=src[3]; }
        else { uint16_t c = uint16_t(src[0]>>3)<<11 | uint16_t(src[1]>>2)<<5 | uint16_t(src[2]>>3); std::memcpy(p,&c,2); }
    }
}
void ff_webgl_invalidate(uint32_t data) {
    FlushCommands();
    MAIN_THREAD_EM_ASM({ Module.onFuzionGpu.invalidate($0); }, Key(data));
    targets.erase(Key(data));
}
