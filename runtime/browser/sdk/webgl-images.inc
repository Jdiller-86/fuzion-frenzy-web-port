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

using BrowserImageMap = std::map<BrowserImageKey, BrowserImageEntry>;
BrowserImageMap g_browser_images;
BrowserImageMap::iterator g_browser_last_entry = g_browser_images.end();
bool g_browser_last_valid = false;
size_t g_browser_image_bytes = 0;
uint64_t g_browser_image_version = 0;

void BrowserImageInvalidate(BrowserImageMap::iterator it) {
    if(g_browser_last_valid&&g_browser_last_entry==it) {
        g_browser_last_entry=g_browser_images.end();g_browser_last_valid=false;
    }
}

bool BrowserImageKeyMatches(const BrowserImageKey& key,uint32_t backing,const gpu::TextureDesc& d,const uint32_t* palette) {
    return key.backing==backing&&key.format==d.format&&key.width==d.width&&key.height==d.height&&
        key.depth==d.depth&&key.levels==d.levels&&key.pitch==d.pitch&&key.cube==d.cube&&key.linear==d.linear&&
        key.has_palette==bool(palette)&&(!palette||ff_webgl_equal_bytes(key.palette.data(),palette,256*sizeof(uint32_t)));
}

void BrowserImageReserve(size_t required,uint64_t frame) {
    constexpr size_t limit=256u*1024u*1024u;
    if(required>limit)std::abort();
    while(g_browser_images.size()>=4096||g_browser_image_bytes>limit-required) {
        auto oldest=g_browser_images.end();
        for(auto it=g_browser_images.begin();it!=g_browser_images.end();++it)
            if(it->second.checked_frame!=frame&&(oldest==g_browser_images.end()||it->second.checked_frame<oldest->second.checked_frame))oldest=it;
        if(oldest==g_browser_images.end())std::abort();
        BrowserImageInvalidate(oldest);g_browser_image_bytes-=oldest->second.accounted;g_browser_images.erase(oldest);
    }
}

uint8_t BrowserImageByte(float value) {
    double x=static_cast<double>(value);
    if(std::isnan(x))return 0;
    x=std::max(0.0,std::min(1.0,x));
    return uint8_t(std::floor(x*255.0+0.5));
}

std::shared_ptr<Image> BrowserImageDecode(const gpu::TextureDesc& d,const gpu::FormatInfo& format,
                        const uint8_t* guest,size_t size,const uint32_t* palette,uint32_t lo,uint32_t hi,
                        ff_browser_texture::Work* warm) {
    const uint64_t pixel_count=uint64_t(d.width)*d.height;
    if(!pixel_count||pixel_count>8u*1024*1024)return {};
    const size_t bytes=size_t(pixel_count)*4;
    auto image=std::make_shared<Image>();image->w=d.width;image->h=d.height;image->faces=1;
    image->depth16=false;image->lo=lo;image->hi=hi;
    image->gpu_pixel_capacity = bytes;
    if(format.host==gpu::Host::BGRA8) {
        image->gpu_pixels = warm ? std::move(warm->rgba) : gpu::DecodeBrowserColor(d, guest, size, palette);
        if (!image->gpu_pixels) {
            auto bgra = std::make_shared<std::vector<uint8_t>>();
            uint32_t pitch=0;
            gpu::DecodeLevel(d,0,0,guest,palette,*bgra,pitch);
            if(pitch!=d.width*4||bgra->size()!=bytes)return {};
            for(size_t p=0;p<bytes;p+=4) {
                std::swap((*bgra)[p],(*bgra)[p+2]);
            }
            image->gpu_pixel_capacity = bgra->capacity();
            const uint8_t* pixels = bgra->data();
            image->gpu_pixels = std::shared_ptr<const uint8_t[]>(std::move(bgra), pixels);
        }
        image->gpu_encoding=1;
    } else if(format.compressed) {
        std::vector<uint8_t> blocks;uint32_t pitch=0;
        gpu::DecodeLevel(d,0,0,guest,palette,blocks,pitch);
        std::vector<Vec> decoded;DecodeBlocks(d,format.host,blocks,decoded);
        if(decoded.size()!=pixel_count)return {};
        auto rgba = std::make_shared_for_overwrite<uint8_t[]>(bytes);
        for(size_t i=0;i<pixel_count;++i)for(size_t c=0;c<4;++c)
            rgba[i*4+c]=BrowserImageByte(decoded[i][c]);
        image->gpu_pixels = std::move(rgba);
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
            BrowserImageInvalidate(it);
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
    uint64_t palette_hash = 0;
    const uint32_t* palette = d.format == 0x0B ? ff_webgl_palette(palette_header, palette_hash) : nullptr;

    constexpr size_t limit = 256u * 1024u * 1024u;
    auto it = g_browser_images.end();
    bool shortcut_lookup=false;
    std::optional<BrowserImageKey> key;
    if(g_browser_last_valid&&BrowserImageKeyMatches(g_browser_last_entry->first,backing,d,palette)) {
        it=g_browser_last_entry;shortcut_lookup=true;
    } else {
        key.emplace(BrowserImageKey{backing,d.format,d.width,d.height,d.depth,d.levels,d.pitch,d.cube,d.linear,bool(palette),{}});
        if(palette)std::copy_n(palette,256,key->palette.begin());
        it=g_browser_images.find(*key);
        if(it!=g_browser_images.end()){g_browser_last_entry=it;g_browser_last_valid=true;}
    }
    const uint64_t epoch = gpu::DirtyEpoch(d.data), frame = ff_webgl_frame();
    bool changed = it == g_browser_images.end();
    if (!changed && (it->second.checked_frame != frame || it->second.epoch != epoch))
        changed = it->second.encoded.size() != bytes || !ff_webgl_equal_bytes(it->second.encoded.data(), guest, bytes);

    if (changed) {
        if (it != g_browser_images.end()) {
            if(!key)key.emplace(it->first);
            BrowserImageInvalidate(it);
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
#ifdef FF_BROWSER_TEXTURE_PROBE
        const bool probe = gpu::BrowserTextureProbeMode() != 0;
        const auto mark = [&](unsigned phase) {
            if (probe) gpu::BrowserTextureProbeMark(double(frame + 1), phase, d.format, bytes,
                                                   uint32_t(pixel_count), uint32_t(d.linear));
        };
        mark(0);
#endif
        auto warm = ff_browser_texture::Take(d, backing, guest, bytes);
        std::shared_ptr<Image> image;
        if(byte_path)image=BrowserImageDecode(d,format,guest,bytes,palette,d.data,d.data+bytes,warm.get());
        if(!image) {
            if(byte_path&&decoded64>limit-size_t(bytes)-metadata)std::abort();
            if(byte_path)BrowserImageReserve(size_t(bytes)+metadata+size_t(decoded64),frame);
            g_images.clear();image=FindImage(header,palette_header);
        }
#ifdef FF_BROWSER_TEXTURE_PROBE
        mark(1);
#endif
        if(!image||(image->texels[0].size()!=pixel_count&&!image->gpu_pixels))std::abort();
        const size_t image_payload=image->gpu_encoding?image->gpu_pixel_capacity:image->texels[0].capacity()*sizeof(Vec);
        if(bytes>limit||image_payload>limit-bytes||metadata>limit-bytes-image_payload)std::abort();
        BrowserImageReserve(bytes+image_payload+metadata,frame);
        if (g_browser_image_version == std::numeric_limits<uint64_t>::max()) std::abort();
        BrowserImageEntry entry;
#ifdef FF_BROWSER_TEXTURE_PROBE
        mark(2);
#endif
        if (warm) entry.encoded = std::move(warm->encoded);
        else entry.encoded.assign(guest, guest + bytes);
#ifdef FF_BROWSER_TEXTURE_PROBE
        mark(3);
#endif
        entry.image = std::move(image);
        entry.owner=entry.image;
        entry.epoch = epoch;
        entry.checked_frame = frame;
        entry.version = ++g_browser_image_version;
        entry.accounted = bytes + image_payload + metadata;
        BrowserImageReserve(entry.accounted,frame);
        g_browser_image_bytes += entry.accounted;
        it = g_browser_images.emplace(std::move(*key), std::move(entry)).first;
        g_browser_last_entry=it;g_browser_last_valid=true;
    } else {
        it->second.checked_frame = frame;
        it->second.epoch = epoch;
        g_browser_last_entry=it;g_browser_last_valid=true;
    }
    const auto& image=*it->second.image;
    const void* pixels=image.gpu_encoding?static_cast<const void*>(image.gpu_pixels.get()):static_cast<const void*>(image.texels[0].data());
    const uint32_t payload_bytes=uint32_t(size_t(image.w)*image.h*(image.gpu_encoding?4:sizeof(Vec)));
    ff_webgl_image_owner(pixels,it->second.version,it->second.owner,image.gpu_encoding,payload_bytes,shortcut_lookup);
    return it->second.image;
}
