// Included in gpu's SDK texture adapter. Reuse its color and swizzle rules.
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
