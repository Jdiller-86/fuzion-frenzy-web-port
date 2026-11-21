#pragma once

#include <memory>
#include <vector>

#include "gpu/texture.h"

namespace ff_browser_texture {

inline constexpr uint64_t kUploadVersionBit = uint64_t{1} << 63;

struct Source {
    gpu::TextureDesc desc;
    const uint8_t* pixels;
    uint32_t backing;
    uint32_t bytes;
};

struct Work {
    gpu::TextureDesc desc;
    uint32_t backing = 0;
    size_t charge = 0;
    uint64_t prepared_frame = 0;
    uint32_t upload_token = 0;
    std::vector<uint8_t> encoded;
    std::shared_ptr<uint8_t[]> rgba;

    Work() = default;
    Work(const Work&) = delete;
    Work& operator=(const Work&) = delete;
    ~Work();
};

std::unique_ptr<Work> Copy(const Source& source);
void Prepare(std::unique_ptr<Work> work);
void FlushUploads();
std::unique_ptr<Work> Take(const gpu::TextureDesc& desc, uint32_t backing, const uint8_t* pixels, size_t bytes);
void Expire(uint64_t frame);

} // namespace ff_browser_texture

bool ff_browser_early_texture_decode_enabled();
void ff_browser_queue_warm_texture(const ff_browser_texture::Source& source);
