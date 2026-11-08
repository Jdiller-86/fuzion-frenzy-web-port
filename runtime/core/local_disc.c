#include "local_disc.h"
#include "thread_bootstrap.h"
#include <string.h>

#define FF_DISC_ACCESS_DENIED 0xc0000022u
#define FF_DISC_XISO_MIN (34ull * 2048ull)
#define FF_DISC_JS_SAFE 9007199254740991ull

static ff_namespace_object *object_by_id(ff_object_namespace *space, uint32_t id)
{
    ff_namespace_object *match = NULL;
    for (unsigned i = 0; i < FF_NS_OBJECTS; ++i)
        if (space->objects[i].id == id && id) {
            if (match) return NULL;
            match = &space->objects[i];
        }
    return match;
}

static int overlap(uint32_t a, uint32_t an, uint32_t b, uint32_t bn)
{
    uint64_t ae = (uint64_t)a + an, be = (uint64_t)b + bn;
    return (uint64_t)a < be && (uint64_t)b < ae;
}

static int safe_name(const uint8_t *name, uint32_t bytes)
{
    if (bytes < 4 || (name[0] != 'D' && name[0] != 'd') || name[1] != ':' || name[2] != '\\') return 0;
    uint32_t segment = 3;
    for (uint32_t i = 3; i <= bytes; ++i) {
        if (i < bytes) {
            unsigned char c = name[i];
            if (!c || c < 32 || c > 126 || c == '/' || c == ':') return 0;
        }
        if (i == bytes || name[i] == '\\') {
            uint32_t length = i - segment;
            if (!length || (length == 1 && name[segment] == '.') ||
                (length == 2 && name[segment] == '.' && name[segment + 1] == '.')) return 0;
            segment = i + 1;
        }
    }
    return 1;
}

uint32_t ff_disc_bind(ff_local_disc *disc, ff_object_namespace *space,
                      uint32_t provider, uint64_t image_bytes)
{
    if (!disc || !space || !space->memory || !space->memory->ram || !provider ||
        image_bytes < FF_DISC_XISO_MIN || image_bytes % 2048u || image_bytes > FF_DISC_JS_SAFE)
        return FF_BOOT_INVALID;
    if (disc->bound || disc->space || disc->provider || disc->device || disc->image_bytes || disc->denied)
        return FF_BOOT_UNSUPPORTED;

    ff_namespace_resolution resolution = {0};
    uint32_t status = ff_namespace_resolve(space, "\\Device\\CdRom0", 14, &resolution);
    if (status) return status;
    ff_namespace_object *device = object_by_id(space, resolution.object);
    if (!device || device->kind != FF_NS_DEVICE || device->device_kind != FF_NS_LOCAL_DISC ||
        resolution.remaining_bytes || device->media_ready || device->provider_object)
        return FF_BOOT_UNSUPPORTED;

    /* Commit only after every identity and bounds check has passed. */
    device->media_ready = 1;
    device->provider_object = provider;
    disc->space = space;
    disc->provider = provider;
    disc->device = device->id;
    disc->image_bytes = image_bytes;
    disc->bound = 1;
    return 0;
}

uint32_t ff_disc_create(ff_local_disc *disc, const uint32_t args[9])
{
    if (!disc || !args || !disc->bound || !disc->space || !disc->space->memory ||
        !disc->space->memory->ram || !disc->provider || !disc->device ||
        disc->image_bytes < FF_DISC_XISO_MIN || disc->image_bytes % 2048u ||
        disc->image_bytes > FF_DISC_JS_SAFE) return FF_BOOT_UNSUPPORTED;
    ff_object_namespace *space = disc->space;
    ff_namespace_object *device = object_by_id(space, disc->device);
    if (!device || device->kind != FF_NS_DEVICE || device->device_kind != FF_NS_LOCAL_DISC ||
        !device->media_ready || device->provider_object != disc->provider) return FF_BOOT_UNSUPPORTED;

    /* NtCreateFile(output, access, OBJECT_ATTRIBUTES, IOSB, allocation,
     * attributes, share, disposition, options). */
    uint32_t output = args[0], attributes = args[2], iosb = args[3];
    if (!output || (output & 3u) || !attributes || (attributes & 3u) || !iosb || (iosb & 3u))
        return FF_BOOT_INVALID;
    if (args[1] != 0x40100080u || args[4] != 0 || args[5] != 0x80u || args[6] != 3u ||
        args[7] != 5u || args[8] != 0x60u) return FF_BOOT_UNSUPPORTED;
    if (ff_vm_check(space->memory, attributes, 12, FF_VM_READ) ||
        ff_vm_check(space->memory, output, 4, FF_VM_WRITE) ||
        ff_vm_check(space->memory, iosb, 8, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;

    uint8_t *ram = space->memory->ram;
    uint32_t root = (uint32_t)ram[attributes] | (uint32_t)ram[attributes + 1] << 8 |
        (uint32_t)ram[attributes + 2] << 16 | (uint32_t)ram[attributes + 3] << 24;
    uint32_t flags = (uint32_t)ram[attributes + 8] | (uint32_t)ram[attributes + 9] << 8 |
        (uint32_t)ram[attributes + 10] << 16 | (uint32_t)ram[attributes + 11] << 24;
    uint32_t descriptor = (uint32_t)ram[attributes + 4] | (uint32_t)ram[attributes + 5] << 8 |
        (uint32_t)ram[attributes + 6] << 16 | (uint32_t)ram[attributes + 7] << 24;
    if (root != 0xfffffffdu || flags != 0x40u || !descriptor || (descriptor & 3u))
        return FF_BOOT_UNSUPPORTED;
    if (ff_vm_check(space->memory, descriptor, 8, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    uint32_t length = (uint32_t)ram[descriptor] | (uint32_t)ram[descriptor + 1] << 8;
    uint32_t maximum = (uint32_t)ram[descriptor + 2] | (uint32_t)ram[descriptor + 3] << 8;
    uint32_t name = (uint32_t)ram[descriptor + 4] | (uint32_t)ram[descriptor + 5] << 8 |
        (uint32_t)ram[descriptor + 6] << 16 | (uint32_t)ram[descriptor + 7] << 24;
    if (!length || length > maximum || maximum > 255u || !name) return FF_BOOT_UNSUPPORTED;
    if (ff_vm_check(space->memory, name, length, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    if (!safe_name(ram + name, length)) return FF_BOOT_UNSUPPORTED;

    char drive[7] = "\\??\\D:";
    drive[4] = (char)ram[name];
    ff_namespace_resolution resolution = {0};
    if (ff_namespace_resolve(space, drive, 6, &resolution) || resolution.remaining_bytes ||
        resolution.object != disc->device) return FF_BOOT_UNSUPPORTED;

    if (overlap(output, 4, iosb, 8) || overlap(output, 4, attributes, 12) ||
        overlap(output, 4, descriptor, 8) || overlap(output, 4, name, length) ||
        overlap(iosb, 8, attributes, 12) || overlap(iosb, 8, descriptor, 8) ||
        overlap(iosb, 8, name, length)) return FF_BOOT_UNSUPPORTED;

    if (disc->denied == UINT32_MAX) return FF_BOOT_UNSUPPORTED;

    /* Source-supported read-only provider result for overwrite-if.  No path
     * lookup, file allocation, handle write, or image access occurs here. */
    ram[iosb] = 0x22u; ram[iosb + 1] = 0; ram[iosb + 2] = 0; ram[iosb + 3] = 0xc0u;
    for (unsigned i = 4; i < 8; ++i) ram[iosb + i] = 0;
    ++disc->denied;
    return FF_DISC_ACCESS_DENIED;
}

void ff_disc_reset(ff_local_disc *disc)
{
    if (!disc) return;
    if (disc->space && disc->device && disc->provider) {
        ff_namespace_object *device = object_by_id(disc->space, disc->device);
        if (device && device->kind == FF_NS_DEVICE && device->device_kind == FF_NS_LOCAL_DISC &&
            device->media_ready && device->provider_object == disc->provider) {
            device->media_ready = 0;
            device->provider_object = 0;
        }
    }
    memset(disc, 0, sizeof *disc);
}
