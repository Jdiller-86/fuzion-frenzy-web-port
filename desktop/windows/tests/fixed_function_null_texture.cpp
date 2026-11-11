#include "gpu/shaders.h"

#include <cassert>
#include <cstdio>
#include <memory>
#include <string>

#ifdef NDEBUG
#error This regression fixture requires assertions enabled.
#endif

namespace {
constexpr uint32_t kSelectArg1 = 2, kSelectArg2 = 3, kModulate2X = 5, kBlendTextureAlpha = 14;
constexpr uint32_t kDiffuse = 0, kCurrent = 1, kTexture = 2;

gpu::State Base() {
    gpu::State s;
    s.textures[0] = 0x1000;
    s.tss[0][gpu::kTssColorOp] = kModulate2X;
    s.tss[0][gpu::kTssColorArg1] = kTexture;
    s.tss[0][gpu::kTssColorArg2] = kCurrent;
    s.tss[1][gpu::kTssColorOp] = kBlendTextureAlpha;
    s.tss[1][gpu::kTssColorArg1] = kTexture;
    s.tss[1][gpu::kTssColorArg2] = kCurrent;
    // A later enabled stage must not survive termination of stage 1.
    s.tss[2][gpu::kTssColorOp] = kSelectArg1;
    s.tss[2][gpu::kTssColorArg1] = kDiffuse;
    return s;
}

void Stopped(const gpu::PixelKey& k) {
    assert(k.ff[0][0] == kModulate2X);
    for (int stage = 1; stage < 4; ++stage)
        for (uint32_t v : k.ff[stage]) assert(v == 0);
    const std::string hlsl = gpu::PixelHlsl(k);
    assert(hlsl.find("float4 t0 = tex0.Sample") != std::string::npos);
    assert(hlsl.find("float4 t1 =") == std::string::npos);
    assert(hlsl.find("float4 t2 =") == std::string::npos);
}
}

int main() {
    gpu::TexInfo tex[4]{};
    tex[0].kind = gpu::kTex2D;
    gpu::State s = Base();
    Stopped(gpu::MakePixelKey(s, tex));
    std::puts("PASS null stage 1 stops current and later stages, including HLSL");

    s.textures[1] = 0x2000;
    tex[1].kind = gpu::kTex2D;
    auto k = gpu::MakePixelKey(s, tex);
    assert(k.ff[1][0] == kBlendTextureAlpha && k.ff[2][0] == kSelectArg1);
    assert(gpu::PixelHlsl(k).find("float4 t1 = tex1.Sample") != std::string::npos);
    std::puts("PASS bound stage 1 remains enabled");

    s = Base();
    tex[1] = {};
    s.tss[1][gpu::kTssColorOp] = kSelectArg2;
    k = gpu::MakePixelKey(s, tex);
    assert(k.ff[1][0] == kSelectArg2 && k.ff[2][0] == kSelectArg1);
    assert(gpu::PixelHlsl(k).find("saturate((cur).rgb)") != std::string::npos);
    std::puts("PASS SELECTARG2 CURRENT ignores stale arg1 TEXTURE");

    for (uint32_t modifier : {0x10u, 0x20u, 0x30u}) {
        s = Base();
        s.tss[1][gpu::kTssColorArg1] = kTexture | modifier;
        Stopped(gpu::MakePixelKey(s, tex));
    }
    std::puts("PASS complement and alpha-replicate modifiers preserve termination");

    s = Base();
    s.ps = std::make_shared<gpu::PixelShader>();
    for (uint32_t i = 0; i < 60; ++i) s.ps->def[i] = i * 17 + 3;
    const auto programmableBefore = gpu::MakePixelKey(s, tex);
    s.textures[1] = 0x2000;
    s.tss[1][gpu::kTssColorOp] = kSelectArg2;
    const auto programmableAfter = gpu::MakePixelKey(s, tex);
    assert(programmableBefore.programmable == 1);
    assert(programmableBefore == programmableAfter);
    assert(programmableBefore.count == s.ps->def[53]);
    std::puts("PASS programmable key ignores fixed-function termination inputs");

    s = {};
    s.tss[0][gpu::kTssColorOp] = kSelectArg1;
    s.tss[0][gpu::kTssColorArg1] = kDiffuse;
    k = gpu::MakePixelKey(s, tex);
    assert(k.ff[0][0] == kSelectArg1);
    assert(gpu::PixelHlsl(k).find("saturate((v0).rgb)") != std::string::npos);
    std::puts("PASS stage 0 color-only DIFFUSE remains enabled without texture");

    s = Base();
    s.textures[1] = 0x2000;
    tex[1] = {}; // Failed or unsupported host texture description is not guest NULL.
    k = gpu::MakePixelKey(s, tex);
    assert(k.ff[1][0] == kBlendTextureAlpha && k.ff[2][0] == kSelectArg1);
    std::puts("PASS missing TexInfo does not masquerade as an unbound texture");
    std::puts("All 7 fixed-function NULL-texture regression cases passed.");
}
