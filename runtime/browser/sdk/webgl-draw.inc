// Included at the raw programmable or evaluated fixed-function vertex seam.
// No guest code is copied into this adapter. The SDK continues to own assembly,
// assembly, texture decoding and both original shader generators.
if (ff_webgl_enabled()) {
    uint32_t states[] = {
        rt.data, zs.ok ? zs.data : 0, gw, gh,
        simple(kMBlendEnable), simple(kMSrcBlend), simple(kMDestBlend), simple(kMBlendOp), simple(kMColorWriteEnable),
        zs.ok && s.rs[kRsZEnable], simple(kMZWriteEnable), simple(kMZFunc),
        zs.ok && s.rs[kRsStencilEnable], simple(kMStencilFunc), simple(kMStencilRef), simple(kMStencilMask),
        simple(kMStencilWriteMask), s.rs[kRsStencilFail], simple(kMStencilZFail), simple(kMStencilPass),
        s.rs[kRsCullMode], s.rs[kRsFrontFace], uint32_t(s.viewport.x), uint32_t(s.viewport.y),
        s.viewport.w, s.viewport.h, s.scissor_count ? 1u : 0u,
        s.scissors[0], s.scissors[1], s.scissors[2], s.scissors[3], simple(kMBlendColor),
        mat.key.flat, s.rs[kRsZBias], simple(kMSolidOffsetEnable), simple(kMSlopeScale), simple(kMDepthOffset),
        uint32_t(reinterpret_cast<uintptr_t>(bridge_vertex_shader)),0,bridge_registers
    };
    float vertex_constants[772];
    if(bridge_vertex_shader) {
        std::memcpy(vertex_constants,s.vsc.data(),768*4);
        auto* fog=vertex_constants+768;std::fill(fog,fog+4,0.f);
        if(s.rs[kRsFogEnable]) {
            if(!s.rs[kRsFogTableMode]) {fog[0]=1;fog[3]=3;}
            else {fog[0]=AsFloat(s.rs[kRsFogStart]);fog[1]=AsFloat(s.rs[kRsFogEnd]);fog[2]=AsFloat(s.rs[kRsFogDensity]);fog[3]=float(s.rs[kRsFogTableMode]);}
        }
        states[38]=uint32_t(reinterpret_cast<uintptr_t>(vertex_constants));
    }
    float uniforms[4 + 4 + 4 + 18 * 4 + 4 + 4 * 4 * 4] = {};
    float* u = uniforms;
    auto push = [&](Vec v) { for (int j = 0; j < 4; ++j) *u++ = v[j]; };
    push(V(float(gw)*(bridge_vertex_shader?s.super_x:1.f),float(gh)*(bridge_vertex_shader?s.super_y:1.f),1.0f/s.zscale,0));
    push(Unpack(s.rs[kRsFogColor]));
    push(V(float(simple(kMAlphaRef) & 255), simple(kMAlphaTestEnable) ? float(simple(kMAlphaFunc) & 7) : 7.f,
           float(s.rs[kRsShadowFunc] & 7), float(s.rs[kRsSpecularEnable])));
    for (int j = 0; j < 18; ++j) push(mat.pc[j]);
    push(mat.tfactor);
    uint32_t textures[4 * 14] = {};
    for (int t = 0; t < 4; ++t) {
        const auto& unit = mat.unit[t];
        const auto& image = unit.img;
        auto* q = textures + t * 14;
        q[0] = mat.info[t].kind; q[1] = mat.info[t].shadow;
        if (image) {
            q[2] = image->lo; q[3] = image->w; q[4] = image->h; q[5] = image->faces;
            q[6] = uint32_t(reinterpret_cast<uintptr_t>(image->texels[0].data()));
            q[7] = unit.address_u; q[8] = unit.address_v; q[9] = unit.min; q[10] = unit.mag;
            // Cube faces are separate owned vectors in the SDK decoder.
            q[13] = s.tss[t][kTssBorderColor];
        }
        push(V(image ? float(image->w) : 1.f, image ? float(image->h) : 1.f, 0, 0));
    }
    for (int t = 0; t < 4; ++t) {
        const auto& ts = s.tss[t];
        push(V(AsFloat(ts[kTssBumpEnvMat00]), AsFloat(ts[kTssBumpEnvMat01]), AsFloat(ts[kTssBumpEnvMat10]), AsFloat(ts[kTssBumpEnvMat11])));
    }
    for (int t = 0; t < 4; ++t) push(V(AsFloat(s.tss[t][kTssBumpEnvLScale]), AsFloat(s.tss[t][kTssBumpEnvLOffset]), 0, 0));
    for (int t = 0; t < 4; ++t) push(Vec{});
    // Stable owned shader text avoids regenerating and decoding the same
    // combiner program for every text glyph/draw. Compare keys after hashing.
    static std::map<uint64_t,std::vector<std::pair<PixelKey,std::string>>> shader_text;
    auto& bucket = shader_text[Hash(&mat.key,sizeof(mat.key))];
    const char* source = nullptr;
    for(auto& entry:bucket) if(entry.first==mat.key) {source=entry.second.c_str();break;}
    if(!source) {
        if(shader_text.size()>2048) std::abort();
        bucket.emplace_back(mat.key,PixelHlsl(mat.key));source=bucket.back().second.c_str();
    }
    ff_webgl_target(s.rt, false);
    if (zs.ok) ff_webgl_target(s.zs, true);
    if (ff_webgl_draw(bridge_vertices, bridge_count, bridge_stride, as.indices.data(),
                      uint32_t(as.indices.size()), states, uniforms, source, textures)) return;
    // A rejected shader/state is a visible stop, never an omitted draw.
    std::abort();
}
