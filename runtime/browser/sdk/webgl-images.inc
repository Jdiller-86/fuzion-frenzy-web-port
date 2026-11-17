// Persistent, bounded cache for decoded CPU textures sampled by the browser GPU.
// Included inside reference::anonymous namespace after FindImage and Image.
// Its standard-library dependencies are included by the generated adapter
// before the namespace is opened.

struct BrowserImageKey {
    uint32_t backing, format, width, height, depth, levels, pitch;
    bool cube, linear, has_palette;
    std::array<uint32_t, 256> palette{};
    bool operator<(const BrowserImageKey& b) const {
        if(backing!=b.backing)return backing<b.backing;
        if(format!=b.format)return format<b.format;
        if(width!=b.width)return width<b.width;
        if(height!=b.height)return height<b.height;
        if(depth!=b.depth)return depth<b.depth;
        if(levels!=b.levels)return levels<b.levels;
        if(pitch!=b.pitch)return pitch<b.pitch;
        if(cube!=b.cube)return cube<b.cube;
        if(linear!=b.linear)return linear<b.linear;
        if(has_palette!=b.has_palette)return has_palette<b.has_palette;
        return has_palette&&palette<b.palette;
    }
};

struct BrowserImageEntry {
    std::vector<uint8_t> encoded;
    std::shared_ptr<Image> image;
    std::shared_ptr<const void> owner;
    uint64_t epoch = 0, checked_frame = std::numeric_limits<uint64_t>::max(), version = 0;
    size_t accounted = 0;
};

std::map<BrowserImageKey, BrowserImageEntry> g_browser_images;
size_t g_browser_image_bytes = 0;
uint64_t g_browser_image_version = 0;

void BrowserImageReserve(size_t required,uint64_t frame) {
    constexpr size_t limit=256u*1024u*1024u;
    if(required>limit)std::abort();
    while(g_browser_images.size()>=4096||g_browser_image_bytes>limit-required) {
        auto oldest=g_browser_images.end();
        for(auto it=g_browser_images.begin();it!=g_browser_images.end();++it)
            if(it->second.checked_frame!=frame&&(oldest==g_browser_images.end()||it->second.checked_frame<oldest->second.checked_frame))oldest=it;
        if(oldest==g_browser_images.end())std::abort();
        g_browser_image_bytes-=oldest->second.accounted;g_browser_images.erase(oldest);
    }
}

uint8_t BrowserImageByte(float value) {
    double x=static_cast<double>(value);
    if(std::isnan(x))return 0;
    x=std::max(0.0,std::min(1.0,x));
    return uint8_t(std::floor(x*255.0+0.5));
}

std::shared_ptr<Image> BrowserImageDecode(const gpu::TextureDesc& d,const gpu::FormatInfo& format,
                        const uint8_t* guest,const uint32_t* palette,uint32_t lo,uint32_t hi) {
    const uint64_t pixel_count=uint64_t(d.width)*d.height;
    if(!pixel_count||pixel_count>8u*1024*1024)return {};
    const size_t bytes=size_t(pixel_count)*4;
    auto image=std::make_shared<Image>();image->w=d.width;image->h=d.height;image->faces=1;
    image->depth16=false;image->lo=lo;image->hi=hi;
    if(format.host==gpu::Host::BGRA8) {
        std::vector<uint8_t> decoded;uint32_t pitch=0;
        gpu::DecodeLevel(d,0,0,guest,palette,decoded,pitch);
        if(pitch!=d.width*4||decoded.size()!=bytes)return {};
        image->gpu_pixels=std::move(decoded);
        for(size_t p=0;p<bytes;p+=4) {
            std::swap(image->gpu_pixels[p],image->gpu_pixels[p+2]);
        }
        image->gpu_encoding=1;
    } else if(format.compressed) {
        std::vector<uint8_t> blocks;uint32_t pitch=0;
        gpu::DecodeLevel(d,0,0,guest,palette,blocks,pitch);
        std::vector<Vec> decoded;DecodeBlocks(d,format.host,blocks,decoded);
        if(decoded.size()!=pixel_count)return {};
        image->gpu_pixels.resize(bytes);
        for(size_t i=0;i<pixel_count;++i)for(size_t c=0;c<4;++c)
            image->gpu_pixels[i*4+c]=BrowserImageByte(decoded[i][c]);
        image->gpu_encoding=1;
    } else return {};
    return image;
}

void BrowserImagesWritten(uint32_t lo,uint32_t hi) {
    if(!ff_webgl_enabled()||hi<=lo)return;
    const uint32_t first=uint32_t(ff_sdk_guest_ptr(lo,size_t(hi)-lo)-ff_sdk_guest_base());
    const uint64_t end=uint64_t(first)+hi-lo;
    for(auto it=g_browser_images.begin();it!=g_browser_images.end();) {
        if(uint64_t(it->first.backing)<end&&uint64_t(it->first.backing)+it->second.encoded.size()>first) {
            g_browser_image_bytes-=it->second.accounted;
            it=g_browser_images.erase(it);
        } else ++it;
    }
}

std::shared_ptr<Image> BrowserImage(uint32_t header, uint32_t palette_header, const gpu::TextureDesc& d) {
    if (!ff_webgl_enabled()) return FindImage(header, palette_header);
    const auto& format = gpu::Format(d.format);
    if(ff_webgl_is_target(d.data)) {
        const auto size=gpu::TotalSize(d);
        const uint32_t backing=uint32_t(ff_sdk_guest_ptr(d.data,size)-ff_sdk_guest_base());
        const std::array<uint32_t,10> key={backing,d.format,d.width,d.height,d.depth,d.levels,d.pitch,uint32_t(d.cube),uint32_t(d.linear),uint32_t(Depth16Format(d.format))};
        static std::map<std::array<uint32_t,10>,std::shared_ptr<Image>> views;
        auto found=views.find(key);
        if(found!=views.end())return found->second;
        if(views.size()>=512)views.erase(views.begin());
        auto view=std::make_shared<Image>();view->w=d.width;view->h=d.height;view->faces=d.cube?6:1;
        view->depth16=Depth16Format(d.format);view->lo=d.data;view->hi=d.data+size;
        return views.emplace(key,std::move(view)).first->second;
    }
    if (d.cube || d.depth != 1 || !d.width || !d.height || format.depth || format.host == gpu::Host::None)
        return FindImage(header, palette_header);

    const uint32_t bytes = gpu::TotalSize(d);
    if (!bytes) return FindImage(header, palette_header);
    uint8_t* guest = ff_sdk_guest_ptr(d.data, bytes);
    const uint32_t backing = uint32_t(guest - ff_sdk_guest_base());
    BrowserImageKey key{backing,d.format,d.width,d.height,d.depth,d.levels,d.pitch,d.cube,d.linear,false,{}};
    uint64_t palette_hash = 0;
    const uint32_t* palette = d.format == 0x0B ? ff_webgl_palette(palette_header, palette_hash) : nullptr;
    if (palette) { key.has_palette = true; std::copy_n(palette, 256, key.palette.begin()); }

    constexpr size_t limit = 256u * 1024u * 1024u;
    auto it = g_browser_images.find(key);
    const uint64_t epoch = gpu::DirtyEpoch(d.data), frame = ff_webgl_frame();
    bool changed = it == g_browser_images.end();
    if (!changed && (it->second.checked_frame != frame || it->second.epoch != epoch))
        changed = it->second.encoded.size() != bytes || !ff_webgl_equal_bytes(it->second.encoded.data(), guest, bytes);

    if (changed) {
        if (it != g_browser_images.end()) {
            g_browser_image_bytes -= it->second.accounted;
            g_browser_images.erase(it);
        }
        const size_t metadata=sizeof(BrowserImageKey)+sizeof(BrowserImageEntry)+sizeof(Image);
        const uint64_t pixel_count=uint64_t(d.width)*d.height;
        if(pixel_count>8u*1024*1024)std::abort();
        const bool byte_path=format.host==gpu::Host::BGRA8||format.compressed;
        const uint64_t decoded64=pixel_count*sizeof(Vec);
        if(bytes>limit||metadata>limit-bytes)std::abort();
        const size_t reserve_payload=byte_path?size_t(pixel_count)*4:size_t(decoded64);
        if(reserve_payload>limit-size_t(bytes)-metadata)std::abort();
        BrowserImageReserve(size_t(bytes)+metadata+reserve_payload,frame);
        std::shared_ptr<Image> image;
        if(byte_path)image=BrowserImageDecode(d,format,guest,palette,d.data,d.data+gpu::TotalSize(d));
        if(!image) {
            if(byte_path&&decoded64>limit-size_t(bytes)-metadata)std::abort();
            if(byte_path)BrowserImageReserve(size_t(bytes)+metadata+size_t(decoded64),frame);
            g_images.clear();image=FindImage(header,palette_header);
        }
        if(!image||(image->texels[0].size()!=pixel_count&&image->gpu_pixels.empty()))std::abort();
        const size_t image_payload=image->gpu_encoding?image->gpu_pixels.capacity():image->texels[0].capacity()*sizeof(Vec);
        if(bytes>limit||image_payload>limit-bytes||metadata>limit-bytes-image_payload)std::abort();
        BrowserImageReserve(bytes+image_payload+metadata,frame);
        if (g_browser_image_version == std::numeric_limits<uint64_t>::max()) std::abort();
        BrowserImageEntry entry;
        entry.encoded.assign(guest, guest + bytes);
        entry.image = std::move(image);
        entry.owner=entry.image;
        entry.epoch = epoch;
        entry.checked_frame = frame;
        entry.version = ++g_browser_image_version;
        entry.accounted = bytes + image_payload + metadata;
        BrowserImageReserve(entry.accounted,frame);
        g_browser_image_bytes += entry.accounted;
        it = g_browser_images.emplace(std::move(key), std::move(entry)).first;
    } else {
        it->second.checked_frame = frame;
        it->second.epoch = epoch;
    }
    const auto& image=*it->second.image;
    const void* pixels=image.gpu_encoding?static_cast<const void*>(image.gpu_pixels.data()):static_cast<const void*>(image.texels[0].data());
    const uint32_t payload_bytes=uint32_t(size_t(image.w)*image.h*(image.gpu_encoding?4:sizeof(Vec)));
    ff_webgl_image_owner(pixels,it->second.version,it->second.owner,image.gpu_encoding,payload_bytes);
    return it->second.image;
}
