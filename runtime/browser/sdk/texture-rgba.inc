// Included in gpu's SDK texture adapter. Reuse its color and swizzle rules.
#ifdef FF_BROWSER_TEXTURE_PROBE
int BrowserTextureProbeMode() {
    static const int mode = MAIN_THREAD_EM_ASM_INT({
        const value = Module.onFuzionGpu?.textureProbeMode;
        return value === 1 || value === 2 ? value : 0;
    });
    return mode;
}

void BrowserTextureProbeMark(double frame, unsigned phase, unsigned format, unsigned bytes,
                             unsigned pixels, unsigned linear) {
    EM_ASM({
        console.timeStamp?.("ff-texture-decode:" + [$0, $1, $2, $3, $4, $5].join(":"));
    }, frame, phase, format, bytes, pixels, linear);
}
#endif

namespace {

template<uint32_t FormatId>
inline void StoreBrowserRgba(const uint8_t* source, const uint32_t* palette, uint8_t* output) {
    uint8_t bgra[4];
    if constexpr (FormatId == 0x0B) {
        const uint32_t pixel = palette[*source];
        std::memcpy(bgra, &pixel, sizeof(pixel));
    } else {
        Texel<FormatId>(FormatId, source, bgra);
    }
    output[0] = bgra[2];
    output[1] = bgra[1];
    output[2] = bgra[0];
    output[3] = bgra[3];
}

#if defined(__wasm_simd128__)
inline void StoreBrowserA1R5G5B5Tile(const uint8_t* tile, uint8_t* output, size_t stride) {
    const auto color_mask = wasm_i16x8_splat(31);
    const auto alpha_mask = wasm_i16x8_splat(255);
    const auto expand5 = [](v128_t channel) {
        return wasm_v128_or(wasm_i16x8_shl(channel, 3), wasm_u16x8_shr(channel, 2));
    };
    for (uint32_t pair = 0; pair < 2; ++pair) {
        const auto packed = wasm_v128_load(tile + pair * 16);
        const auto r = expand5(wasm_v128_and(wasm_u16x8_shr(packed, 10), color_mask));
        const auto g = expand5(wasm_v128_and(wasm_u16x8_shr(packed, 5), color_mask));
        const auto b = expand5(wasm_v128_and(packed, color_mask));
        const auto a = wasm_v128_and(wasm_i16x8_shr(packed, 15), alpha_mask);
        const auto rg = wasm_u8x16_narrow_i16x8(r, g);
        const auto ba = wasm_u8x16_narrow_i16x8(b, a);
        // Interleave channels and emit Morton texels 0, 1, 4, 5 on the
        // first row and 2, 3, 6, 7 on the second.
        const auto even = wasm_i8x16_shuffle(rg, ba, 0, 8, 16, 24, 1, 9, 17, 25,
                                             4, 12, 20, 28, 5, 13, 21, 29);
        const auto odd = wasm_i8x16_shuffle(rg, ba, 2, 10, 18, 26, 3, 11, 19, 27,
                                            6, 14, 22, 30, 7, 15, 23, 31);
        wasm_v128_store(output + pair * 2 * stride, even);
        wasm_v128_store(output + (pair * 2 + 1) * stride, odd);
    }
}

void DecodeBrowserA1R5G5B5(const TextureDesc& d, const uint8_t* source,
                          uint8_t* output, uint32_t x_mask, uint32_t y_mask) {
    // With at least two bits in each axis, x0/y0/x1/y1 occupy the
    // low four Morton bits. Each 4x4 tile is contiguous even when
    // the texture is rectangular. Advance only the higher axis bits.
    x_mask &= ~15u;
    y_mask &= ~15u;
    const size_t stride = size_t(d.width) * 4;
    uint32_t y_offset = 0;
    for (uint32_t y = 0; y < d.height; y += 4, output += 4 * stride) {
        uint32_t x_offset = 0;
        for (uint32_t x = 0; x < d.width; x += 4) {
            const uint8_t* tile = source + size_t(x_offset | y_offset) * 2;
            StoreBrowserA1R5G5B5Tile(tile, output + size_t(x) * 4, stride);
            x_offset = (x_offset - x_mask) & x_mask;
        }
        y_offset = (y_offset - y_mask) & y_mask;
    }
}
#endif

template<uint32_t FormatId, uint32_t Bytes>
bool DecodeBrowserRgba(const TextureDesc& d, const uint8_t* source, size_t size,
                       const uint32_t* palette, std::vector<uint8_t>& rgba) {
    const size_t pixels = size_t(d.width) * d.height;
    if constexpr (FormatId == 0x0B) {
        if (d.linear || !palette) return false;
    }
    if (d.linear) {
        const size_t row_bytes = size_t(d.width) * Bytes;
        if (row_bytes > d.pitch || uint64_t(d.height - 1) * d.pitch + row_bytes > size) return false;
        rgba.resize(pixels * 4);
        uint8_t* output = rgba.data();
        for (uint32_t y = 0; y < d.height; ++y) {
            const uint8_t* row = source + size_t(y) * d.pitch;
            for (uint32_t x = 0; x < d.width; ++x, output += 4)
                StoreBrowserRgba<FormatId>(row + size_t(x) * Bytes, palette, output);
        }
        return true;
    }
    if (!std::has_single_bit(d.width) || !std::has_single_bit(d.height) || pixels * Bytes > size) return false;
    uint32_t x_mask, y_mask, z_mask;
    SwizzleMasks(d.width, d.height, 1, x_mask, y_mask, z_mask);
    rgba.resize(pixels * 4);
#if defined(__wasm_simd128__)
    if constexpr (FormatId == 0x02) {
        if (d.width >= 4 && d.height >= 4
#ifdef FF_BROWSER_TEXTURE_PROBE
            && BrowserTextureProbeMode() != 1
#endif
        ) {
            DecodeBrowserA1R5G5B5(d, source, rgba.data(), x_mask, y_mask);
            return true;
        }
    }
#endif
    uint8_t* output = rgba.data();
    uint32_t y_offset = 0;
    for (uint32_t y = 0; y < d.height; ++y) {
        uint32_t x_offset = 0;
        for (uint32_t x = 0; x < d.width; ++x, output += 4) {
            StoreBrowserRgba<FormatId>(source + size_t(x_offset | y_offset) * Bytes, palette, output);
            // Increment only the bits belonging to this Morton axis.
            x_offset = (x_offset - x_mask) & x_mask;
        }
        y_offset = (y_offset - y_mask) & y_mask;
    }
    return true;
}

}  // namespace

// Keep the cache-miss decoder out of the per-draw loop.
[[gnu::noinline]] bool DecodeBrowserColor(const TextureDesc& d, const uint8_t* source, size_t size,
                                         const uint32_t* palette, std::vector<uint8_t>& rgba) {
    const uint64_t pixels = uint64_t(d.width) * d.height;
    if (d.cube || d.depth != 1 || !pixels || pixels > 8u * 1024 * 1024) return false;
    switch (d.format) {
        case 0x02: case 0x10: return DecodeBrowserRgba<0x02, 2>(d, source, size, palette, rgba);
        case 0x03: case 0x1C: return DecodeBrowserRgba<0x03, 2>(d, source, size, palette, rgba);
        case 0x04: case 0x1D: return DecodeBrowserRgba<0x04, 2>(d, source, size, palette, rgba);
        case 0x05: case 0x11: return DecodeBrowserRgba<0x05, 2>(d, source, size, palette, rgba);
        case 0x06: case 0x12: return DecodeBrowserRgba<0x06, 4>(d, source, size, palette, rgba);
        case 0x07: case 0x1E: return DecodeBrowserRgba<0x07, 4>(d, source, size, palette, rgba);
        case 0x0B: return DecodeBrowserRgba<0x0B, 1>(d, source, size, palette, rgba);
        default: return false;
    }
}
