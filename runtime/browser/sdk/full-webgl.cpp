// Browser GPU bridge. Owned inputs remain alive until the runtime worker has
// consumed them, synchronously or through the bounded pipelined handoff.
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
#include <cmath>
#include <atomic>
#include <tuple>
#include <emscripten.h>
#include <emscripten/threading.h>
#include <wasm_simd128.h>
#include "fixed-constants.inc"
bool ff_webgl_pipelined();

// Exact equality, including NaN payloads, without the pinned libc's scalar
// word-at-a-time memcmp loop. Never reads beyond either supplied span.
bool ff_webgl_equal_bytes(const void* left,const void* right,size_t bytes) {
    const auto* a=static_cast<const uint8_t*>(left);
    const auto* b=static_cast<const uint8_t*>(right);
    while(bytes>=64) {
        auto x=wasm_v128_xor(wasm_v128_load(a),wasm_v128_load(b));
        x=wasm_v128_or(x,wasm_v128_xor(wasm_v128_load(a+16),wasm_v128_load(b+16)));
        x=wasm_v128_or(x,wasm_v128_xor(wasm_v128_load(a+32),wasm_v128_load(b+32)));
        x=wasm_v128_or(x,wasm_v128_xor(wasm_v128_load(a+48),wasm_v128_load(b+48)));
        if(wasm_v128_any_true(x))return false;
        a+=64;b+=64;bytes-=64;
    }
    while(bytes>=16) {
        if(wasm_v128_any_true(wasm_v128_xor(wasm_v128_load(a),wasm_v128_load(b))))return false;
        a+=16;b+=16;bytes-=16;
    }
    while(bytes--)if(*a++!=*b++)return false;
    return true;
}

namespace {
struct Target { gpu::TextureDesc desc; uint64_t hash = 0, epoch = 0, checked = ~0ull; std::vector<uint8_t> snapshot; bool cpu_coherent=false; };
std::map<uint32_t, Target> targets;
size_t target_snapshot_bytes=0;
uint64_t frame = 0;
uint32_t last_vertex_constants=0;
uint32_t last_draw_state=0,last_pixel_uniforms=0,last_texture_state=0;
std::vector<std::array<uint32_t,12>> commands;
struct Block { std::unique_ptr<uint8_t[]> bytes; size_t capacity,used=0; };
struct Flight {
    std::vector<std::array<uint32_t,12>> commands;
    std::vector<Block> blocks;
    size_t capacity=0;
    std::vector<std::shared_ptr<const void>> images;
    size_t image_bytes=0;
    std::atomic<int32_t> done{1};
};
Flight flight;
std::atomic<int32_t> bridge_failed{0};
std::vector<Block> owned;
size_t owned_block=0,owned_capacity=0;
std::map<std::pair<uint32_t,uint64_t>,std::array<uint32_t,3>> image_copies;
struct ImageVersion {uint64_t version=0;std::shared_ptr<const void> owner;uint32_t encoding=0,bytes=0;};
std::map<const void*,ImageVersion> image_versions;
std::vector<std::shared_ptr<const void>> pinned_images;
size_t pinned_image_bytes=0;
uint64_t image_shortcut_lookups=0,image_map_lookups=0;
uint64_t program_constant_reuses=0,program_constant_packs=0;
size_t owned_bytes = 0;
uint32_t Own(const void* src, size_t bytes) {
    if(bridge_failed.load(std::memory_order_acquire))std::abort();
    if(bytes > 128u*1024*1024 || owned_bytes + bytes > 256u*1024*1024) std::abort();
    const size_t span=(bytes+7)&~size_t(7);
    while(owned_block<owned.size() && owned[owned_block].capacity-owned[owned_block].used<span) ++owned_block;
    if(owned_block==owned.size()) {
        size_t capacity=std::max<size_t>(4u*1024*1024,span);
        if(owned_capacity+flight.capacity+capacity>256u*1024*1024)std::abort();
        owned.push_back({std::make_unique<uint8_t[]>(capacity),capacity,0});owned_capacity+=capacity;
    }
    auto& block=owned[owned_block];auto* dst=block.bytes.get()+block.used;block.used+=span;
    std::memcpy(dst,src,bytes);owned_bytes+=span;
    return uint32_t(reinterpret_cast<uintptr_t>(dst));
}
void WaitFlight() {
    while(flight.done.load(std::memory_order_acquire)==0) {
        if(emscripten_is_main_runtime_thread())std::abort();
        emscripten_futex_wait(&flight.done,0,1000);
    }
    if(bridge_failed.load(std::memory_order_acquire)||flight.done.load(std::memory_order_acquire)<0)std::abort();
}
void ResetCommands() {
    commands.clear();for(auto& block:owned)block.used=0;owned_block=0;image_copies.clear();image_versions.clear();pinned_images.clear();pinned_image_bytes=0;owned_bytes=0;last_vertex_constants=0;
    last_draw_state=last_pixel_uniforms=last_texture_state=0;
}
uint32_t InternLast(const void* source,size_t bytes,uint32_t& previous) {
    if(previous&&ff_webgl_equal_bytes(reinterpret_cast<const void*>(previous),source,bytes))return previous;
    return previous=Own(source,bytes);
}
void FlushCommands() {
    WaitFlight();
    if(commands.empty()) return;
    if(!MAIN_THREAD_EM_ASM_INT({ return Module.onFuzionGpu.commands($0,$1)?1:0; }, commands.data(),uint32_t(commands.size())))std::abort();
    ResetCommands();
}
void QueuePresent(uint32_t target) {
    WaitFlight();
    commands.swap(flight.commands);owned.swap(flight.blocks);std::swap(owned_capacity,flight.capacity);
    pinned_images.swap(flight.images);std::swap(pinned_image_bytes,flight.image_bytes);
    ResetCommands();
    flight.done.store(0,std::memory_order_release);
    MAIN_THREAD_ASYNC_EM_ASM({
        let failed=false;
        try {
            if(($1&&!Module.onFuzionGpu.commands($0,$1))||!Module.onFuzionGpu.present($2))throw new Error('Pipelined GPU submission failed');
            Module.onFuzionGpu.imageLookups=({shortcutHits:$5,mapLookups:$6});
            Module.onFuzionGpu.constantPacking=({reuses:$7,packs:$8});
        } catch(error) {
            failed=true;
            Atomics.store(new Int32Array(Module.wasmMemory?.buffer||Module.HEAPU8.buffer),$4>>2,1);
            postMessage({type:'gpu-error',text:error.message});
        } finally {
            const heap=new Int32Array(Module.wasmMemory?.buffer||Module.HEAPU8.buffer);
            Atomics.store(heap,$3>>2,failed?-1:1);Atomics.notify(heap,$3>>2);
        }
    },flight.commands.data(),uint32_t(flight.commands.size()),target,&flight.done,&bridge_failed,double(image_shortcut_lookups),double(image_map_lookups),double(program_constant_reuses),double(program_constant_packs));
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
uint64_t ff_webgl_frame() { return frame; }
bool ff_webgl_is_target(uint32_t data) { return targets.count(Key(data)) != 0; }
void ff_webgl_image_version(const void* pixels,uint64_t version) { image_versions[pixels].version=version; }
void ff_webgl_image_owner(const void* pixels,uint64_t version,const std::shared_ptr<const void>& owner,uint32_t encoding,uint32_t bytes,bool shortcut_lookup) {
    if(shortcut_lookup)++image_shortcut_lookups;else ++image_map_lookups;
    auto& entry=image_versions[pixels];
    if(entry.version!=version||entry.owner.get()!=owner.get()||entry.encoding!=encoding||entry.bytes!=bytes)entry={version,owner,encoding,bytes};
}
// Compare the source bank and fog against the existing owned command payload.
// Unchanged draws borrow that immutable payload, not the mutable SDK state.
const float* ff_webgl_program_constants(const void* source,const float* fog) {
    if(last_vertex_constants) {
        const auto* previous=reinterpret_cast<const float*>(last_vertex_constants);
        if(ff_webgl_equal_bytes(previous,source,768*sizeof(float))&&
           ff_webgl_equal_bytes(previous+768,fog,4*sizeof(float))) {
            ++program_constant_reuses;return previous;
        }
    }
    std::array<float,772> packed;
    std::memcpy(packed.data(),source,768*sizeof(float));
    std::memcpy(packed.data()+768,fog,4*sizeof(float));
    last_vertex_constants=Own(packed.data(),sizeof(packed));
    ++program_constant_packs;
    return reinterpret_cast<const float*>(last_vertex_constants);
}
bool ff_webgl_enabled() {
    static const bool enabled = MAIN_THREAD_EM_ASM_INT({ return Module.onFuzionGpu ? 1 : 0; }) != 0;
    return enabled;
}
bool ff_webgl_threaded_enabled() {
    static const bool enabled=MAIN_THREAD_EM_ASM_INT({return Module.onFuzionGpu?.threaded?1:0;})!=0;
    return enabled;
}
bool ff_webgl_pipelined() {
    static const bool enabled=MAIN_THREAD_EM_ASM_INT({return Module.onFuzionGpu?.pipelined?1:0;})!=0;
    return enabled;
}
void ff_webgl_flush() { FlushCommands(); }
bool ff_webgl_gpu_vertex_enabled() {
    static const bool enabled=MAIN_THREAD_EM_ASM_INT({return Module.onFuzionGpu?.gpuVertices?1:0;})!=0;
    return enabled;
}
bool ff_webgl_raw_vertices_enabled() {
    static const bool requested=MAIN_THREAD_EM_ASM_INT({return Module.onFuzionGpu?.rawVertices?1:0;})!=0;
    return requested&&ff_webgl_enabled()&&ff_webgl_gpu_vertex_enabled();
}
float* ff_webgl_fixed_constants(const gpu::State& s,uint32_t width,uint32_t height) {
    struct Cache {
        bool valid=false;
        uint32_t width=0,height=0,ambient=0,types=0;
        gpu::Viewport viewport;
        std::array<std::array<float,16>,7> transforms{};
        std::array<float,17> material{};
        std::array<std::array<uint32_t,26>,8> lights{};
        std::array<float,772> values{};
    };
    static Cache cache;
    constexpr size_t transform_slots[]={256,2,3,16,17,18,19};
    const auto fixed=gpu::MakeFixedKey(s);
    bool same=cache.valid&&cache.width==width&&cache.height==height&&cache.ambient==s.rs[gpu::kRsAmbient]&&cache.types==fixed.types&&
        ff_webgl_equal_bytes(&cache.viewport,&s.viewport,sizeof(s.viewport))&&
        ff_webgl_equal_bytes(cache.material.data(),s.material.data(),sizeof(cache.material))&&
        ff_webgl_equal_bytes(cache.lights.data(),s.lights.data(),sizeof(cache.lights));
    if(same)for(size_t i=0;i<7;++i)if(!ff_webgl_equal_bytes(cache.transforms[i].data(),s.transforms[transform_slots[i]].data(),64)){same=false;break;}
    if(same)return cache.values.data();
    gpu::DrawConstants c{};
    ff_fixed::FillTransformConstants(s,{width,height,0},c);
    ff_fixed::FillLightConstants(s,fixed,c);
    auto* output=cache.values.data();std::fill(output,output+768,0.f);
    std::memcpy(output,c.wvp,16*4);std::memcpy(output+16,c.wv,16*4);std::memcpy(output+32,c.wvn,16*4);
    std::memcpy(output+48,c.tm,64*4);std::memcpy(output+112,c.mat,20*4);std::memcpy(output+132,c.amb,4*4);std::memcpy(output+136,c.lt,224*4);
    cache.width=width;cache.height=height;cache.ambient=s.rs[gpu::kRsAmbient];cache.types=fixed.types;
    cache.viewport=s.viewport;cache.material=s.material;cache.lights=s.lights;
    for(size_t i=0;i<7;++i)cache.transforms[i]=s.transforms[transform_slots[i]];
    cache.valid=true;return output;
}
std::string VertexSourceText(std::string text,uint32_t bgra) {
    if(bgra)text="// ff_browser_bgra_mask="+std::to_string(bgra)+"\n"+text;
    return text;
}
const char* ff_webgl_fixed_source(const gpu::State& s,uint32_t regs,bool pretransformed,uint64_t decode,uint32_t bgra) {
    const auto f=gpu::MakeFixedKey(s);
    const std::array<uint32_t,11> key={regs,uint32_t(pretransformed),f.light,f.types,f.tex[0],f.tex[1],f.tex[2],f.tex[3],uint32_t(decode),uint32_t(decode>>32),bgra};
    static std::map<std::array<uint32_t,11>,std::string> sources;
    auto found=sources.find(key);
    if(found!=sources.end())return found->second.c_str();
    if(sources.size()>=1024)std::abort();
    return sources.emplace(key,VertexSourceText(gpu::FixedVertexHlsl(uint16_t(regs),pretransformed,decode,f),bgra)).first->second.c_str();
}
const char* ff_webgl_vertex_source(const gpu::State& s, uint32_t regs,uint64_t decode,uint32_t bgra) {
    struct OwnedSource {std::vector<uint32_t> words;const char* text=nullptr;};
    struct Owner {std::shared_ptr<gpu::VertexShader> shader;std::map<std::tuple<uint32_t,uint64_t,uint32_t>,OwnedSource> inputs;};
    static std::map<const gpu::VertexShader*,Owner> owners;
    if(owners.size()>=1024&&!owners.count(s.vs.get()))std::abort();
    auto& owner=owners[s.vs.get()];owner.shader=s.vs;
    auto& known=owner.inputs[{regs,decode,bgra}];
    const auto& words=s.vs->program;
    // Retain the shader owner against address reuse, and compare its program
    // bytes so an in-place edit still invalidates this shortcut.
    if(known.text&&known.words==words)return known.text;
    struct Source {std::vector<uint32_t> words;uint32_t regs;uint64_t decode;uint32_t bgra;std::string text;};
    static std::map<uint64_t,std::vector<Source>> cache;
    auto hash=gpu::Hash(words.data(),words.size()*4)^uint64_t(regs)^decode^(uint64_t(bgra)<<32);
    auto& bucket=cache[hash];
    for(auto& source:bucket)if(source.regs==regs&&source.decode==decode&&source.bgra==bgra&&source.words==words){known={words,source.text.c_str()};return known.text;}
    if(cache.size()>1024)std::abort();
    bucket.push_back({words,regs,decode,bgra,VertexSourceText(gpu::VertexProgramHlsl(words,uint16_t(regs),decode),bgra)});
    known={words,bucket.back().text.c_str()};return known.text;
}
const uint32_t* ff_webgl_palette(uint32_t header, uint64_t& hash) {
    if(!header) return nullptr;
    uint32_t descriptor[2];std::memcpy(descriptor,ff_sdk_guest_ptr(header,8),8);
    const size_t bytes=(256u>>(descriptor[0]>>30))*4;
    const uint32_t data=descriptor[1]<64u*1024*1024?descriptor[1]|0x80000000u:descriptor[1];
    struct Palette {std::array<uint32_t,256> colors{};uint64_t checked=~0ull,epoch=0,hash=0;uint32_t data=0,bytes=0;};
    // Threaded resource headers are scratch-ring copies, not palette identities.
    // Key the checked backing/span and recycle old decoded entries at the cap.
    static std::map<std::pair<uint32_t,size_t>,Palette> cache;
    const auto key=std::make_pair(Key(data),bytes);
    auto found=cache.find(key);
    if(found==cache.end()) {
        if(cache.size()>=2048) {
            auto oldest=std::min_element(cache.begin(),cache.end(),[](const auto& a,const auto& b){return a.second.checked<b.second.checked;});
            cache.erase(oldest);
        }
        found=cache.emplace(key,Palette{}).first;
    }
    auto& p=found->second;const auto epoch=gpu::DirtyEpoch(data);
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
    auto* guest = ff_sdk_guest_ptr(d.data, std::max<uint32_t>(size,1));
    const uint32_t key=uint32_t(guest-ff_sdk_guest_base());
    auto& target = targets[key];
    const bool changed = target.checked == ~0ull || target.desc.width != d.width || target.desc.height != d.height || target.desc.format != d.format || target.desc.pitch != d.pitch || target.desc.linear != d.linear;
    uint64_t epoch = gpu::DirtyEpoch(d.data);
    bool upload=changed;
    if(target.checked != frame || target.epoch != epoch) {
        // Exact CPU-byte comparison replaces a costly 64-bit hash. A dirty
        // mark still forces upload when GPU writes made the CPU copy stale.
        upload |= (!target.cpu_coherent&&target.epoch!=epoch) || target.snapshot.size()!=size ||
                  !ff_webgl_equal_bytes(target.snapshot.data(),guest,size);
        target.checked=frame;target.epoch=epoch;
    }
    target.desc = d;
    if(upload) {
        const size_t other=target_snapshot_bytes-target.snapshot.size();
        if(size>128u*1024*1024||other+size>256u*1024*1024)std::abort();
        target.snapshot.assign(guest,guest+size);
        target_snapshot_bytes=other+size;
        target.cpu_coherent=true;
        if(++target.hash==0)std::abort();
        commands.push_back({0,key,d.width,d.height,d.format,d.pitch,d.linear,uint32_t(depth),Own(guest,size),size,uint32_t(target.hash),uint32_t(target.hash>>32)});
    }
}
static bool SubmitDraw(const void* v, uint32_t count, uint32_t stride, const void* indices, uint32_t n,
                   const uint32_t* state, const float* uniforms, const char* shader, const uint32_t* textures,const uint32_t* layout) {
    uint32_t tx[4*14];std::memcpy(tx,textures,sizeof(tx));
    uint32_t states[40];std::memcpy(states,state,sizeof(states));
    states[0]=state[0]?Key(state[0]):0;states[1]=state[1]?Key(state[1]):0;
    if(states[37]) {
        const auto* constants=reinterpret_cast<const void*>(states[38]);
        if(!last_vertex_constants||(constants!=reinterpret_cast<const void*>(last_vertex_constants)&&
           !ff_webgl_equal_bytes(reinterpret_cast<const void*>(last_vertex_constants),constants,772*4)))
            last_vertex_constants=Own(constants,772*4);
        states[38]=last_vertex_constants;
    }
    for(int t=0;t<4;++t) {
        auto* q = tx+t*14;
        if(!q[0])continue;
        q[2]=Key(q[2]);
        if(targets.count(q[2])) continue;
        if(!q[6])std::abort();
        const uint32_t encoding=q[5]>>8;
        const uint32_t faces=q[5]&255u;
        if(faces!=1u||encoding>1u)std::abort();
        const uint64_t pixel_count=uint64_t(q[3])*q[4];
        if(!pixel_count||pixel_count>8u*1024*1024)std::abort();
        const uint64_t expected_bytes=pixel_count*(encoding?4u:16u);
        if(expected_bytes>128u*1024*1024)std::abort();
        const auto version=image_versions.find(reinterpret_cast<const void*>(q[6]));
        if(encoding==1u&&(version==image_versions.end()||version->second.encoding!=1u||version->second.bytes!=expected_bytes))std::abort();
        if(version!=image_versions.end()&&version->second.owner&&
           (version->second.encoding!=encoding||version->second.bytes!=expected_bytes))std::abort();
        const auto identity=std::make_pair(q[6],version==image_versions.end()?uint64_t(0):version->second.version);
        auto found = image_copies.find(identity);
        if(found == image_copies.end()) {
            size_t bytes=size_t(expected_bytes);
            uint32_t copy;
            if(version!=image_versions.end()&&version->second.owner) {
                if(bytes>128u*1024*1024||pinned_image_bytes+flight.image_bytes+bytes>256u*1024*1024)std::abort();
                pinned_images.push_back(version->second.owner);pinned_image_bytes+=bytes;copy=q[6];
            } else copy=Own(reinterpret_cast<const void*>(q[6]),bytes);
            // Persistent decoded images carry a unique content generation.
            // Unregistered reference images retain the conservative hash path.
            auto hash = version==image_versions.end()?gpu::Hash(reinterpret_cast<const void*>(copy),bytes):version->second.version;
            image_copies[identity]={copy,uint32_t(hash),uint32_t(hash>>32)};
            q[6]=copy;q[11]=uint32_t(hash);q[12]=uint32_t(hash>>32);
        } else {q[6]=found->second[0];q[11]=found->second[1];q[12]=found->second[2];}
    }
    if(!count||count>1024u*1024||n>16u*1024*1024)std::abort();
    commands.push_back({layout?3u:1u,Own(v,size_t(count)*stride),count,stride,Own(indices,size_t(n)*4),n,
        InternLast(states,sizeof(states),last_draw_state),InternLast(uniforms,152*4,last_pixel_uniforms),
        uint32_t(reinterpret_cast<uintptr_t>(shader)),InternLast(tx,sizeof(tx),last_texture_state),layout?Own(layout,16*4):0});
    if(states[0])targets.at(states[0]).cpu_coherent=false;
    if(states[1])targets.at(states[1]).cpu_coherent=false;
    return true;
}
bool ff_webgl_draw(const void* v,uint32_t count,uint32_t stride,const void* indices,uint32_t n,
                  const uint32_t* state,const float* uniforms,const char* shader,const uint32_t* textures) {
    return SubmitDraw(v,count,stride,indices,n,state,uniforms,shader,textures,nullptr);
}
bool ff_webgl_draw_raw(const void* v,uint32_t count,uint32_t stride,const void* indices,uint32_t n,
                      const uint32_t* state,const float* uniforms,const char* shader,const uint32_t* textures,const uint32_t* layout) {
    if(!layout||!stride||stride>252||stride%4)std::abort();
    return SubmitDraw(v,count,stride,indices,n,state,uniforms,shader,textures,layout);
}
void ff_webgl_clear(const gpu::State& s, uint32_t n, const uint8_t* rects, uint32_t flags, uint32_t color, float z, uint32_t stencil) {
    uint32_t color_id=0,depth_id=0;gpu::TextureDesc d;
    if (flags & 0xf0) {ff_webgl_target(s.rt, false);if(gpu::DescribeSurface(s.rt,d))color_id=Key(d.data);}
    if (flags & 3) {ff_webgl_target(s.zs, true);if(gpu::DescribeSurface(s.zs,d))depth_id=Key(d.data);}
    uint32_t area[] = {s.viewport.x,s.viewport.y,s.viewport.w,s.viewport.h};
    uint32_t zbits;std::memcpy(&zbits,&z,4);
    commands.push_back({2,color_id,depth_id,n,n?Own(rects,size_t(n)*16):0,flags,color,zbits,stencil,Own(area,sizeof(area))});
    if(color_id)targets.at(color_id).cpu_coherent=false;
    if(depth_id)targets.at(depth_id).cpu_coherent=false;
}
bool ff_webgl_present(const gpu::Surface& front) {
    gpu::TextureDesc d;
    if (!gpu::DescribeSurface(front, d)) return false;
    if(ff_webgl_pipelined()&&targets.count(Key(d.data))) {QueuePresent(Key(d.data));return true;}
    FlushCommands();
    bool shown = MAIN_THREAD_EM_ASM_INT({
        const shown=Module.onFuzionGpu.present($0);
        if(shown)Module.onFuzionGpu.imageLookups=({shortcutHits:$1,mapLookups:$2});
        if(shown)Module.onFuzionGpu.constantPacking=({reuses:$3,packs:$4});
        return shown?1:0;
    }, Key(d.data),double(image_shortcut_lookups),double(image_map_lookups),double(program_constant_reuses),double(program_constant_packs));
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
    // Readback made RAM and the existing GPU allocation agree. Preserve that
    // identity across the lock's following dirty mark if CPU bytes stay equal.
    auto& target=found->second;
    if(target.snapshot.size()!=bytes)std::abort();
    std::memcpy(target.snapshot.data(),dst,bytes);
    target.epoch=gpu::DirtyEpoch(d.data);target.checked=frame;
    target.cpu_coherent=d.format==0x06||d.format==0x12;
}
void ff_webgl_invalidate(uint32_t data) {
    FlushCommands();
    MAIN_THREAD_EM_ASM({ Module.onFuzionGpu.invalidate($0); }, Key(data));
    auto found=targets.find(Key(data));
    if(found!=targets.end()){target_snapshot_bytes-=found->second.snapshot.size();targets.erase(found);}
}
