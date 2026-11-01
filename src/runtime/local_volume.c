#include "local_volume.h"
#include "thread_bootstrap.h"
#include <string.h>

static uint32_t get(const ff_local_volume *volume, uint32_t address, unsigned bytes)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < bytes; ++i) value |= (uint32_t)volume->memory->ram[address + i] << (i * 8);
    return value;
}
static void put(ff_local_volume *volume, uint32_t address, uint64_t value, unsigned bytes)
{
    for (unsigned i = 0; i < bytes; ++i) volume->memory->ram[address + i] = (uint8_t)(value >> (i * 8));
}
static int admitted(const ff_local_volume *volume, uint32_t address, uint32_t bytes, uint32_t access)
{
    return volume && volume->memory && address && !ff_vm_check(volume->memory, address, bytes, access);
}
static int overlap(uint32_t a, uint32_t n, uint32_t b, uint32_t m)
{
    return n && m && (uint64_t)a < (uint64_t)b + m && (uint64_t)b < (uint64_t)a + n;
}
static ff_volume_handle *find(ff_local_volume *volume, uint32_t handle)
{
    if (volume) for (unsigned i = 0; i < FF_VOLUME_HANDLES; ++i) if (volume->handles[i].handle == handle && handle) return &volume->handles[i];
    return NULL;
}
static uint32_t io_result(ff_local_volume *volume, uint32_t iosb, uint32_t status, uint32_t information)
{
    put(volume, iosb, status, 4); put(volume, iosb + 4, information, 4);
    return status;
}
static int power_of_two(uint32_t n) { return n && !(n & (n - 1)); }

uint32_t ff_volume_init(ff_local_volume *volume, ff_virtual_memory *memory, const ff_volume_profile *profile)
{
    if (!volume || !memory || !memory->ram || !profile || !profile->total_units ||
        profile->total_units > INT64_MAX || profile->available_units > profile->total_units ||
        !power_of_two(profile->sectors_per_unit) || profile->sectors_per_unit > 128 ||
        !power_of_two(profile->bytes_per_sector) || profile->bytes_per_sector < 512 || profile->bytes_per_sector > 4096 ||
        profile->total_units > UINT64_MAX / profile->sectors_per_unit / profile->bytes_per_sector) return FF_BOOT_INVALID;
    memset(volume, 0, sizeof *volume); volume->memory = memory; volume->profile = *profile;
    volume->next_handle = FF_VOLUME_HANDLE_BASE;
    return 0;
}
int ff_volume_owns_handle(uint32_t handle)
{
    return handle >= FF_VOLUME_HANDLE_BASE && handle < FF_VOLUME_HANDLE_LIMIT;
}
uint32_t ff_volume_open_root(ff_local_volume *volume, const uint32_t args[6])
{
    static const char root[] = "\\Device\\Harddisk0\\partition1";
    if (!volume || !volume->memory || !args) return FF_BOOT_INVALID;
    const uint32_t output = args[0], access = args[1], attributes = args[2], iosb = args[3], share = args[4], options = args[5];
    if ((output | attributes | iosb) & 3u) return FF_BOOT_INVALID;
    if (!admitted(volume, output, 4, FF_VM_WRITE) || !admitted(volume, iosb, 8, FF_VM_WRITE) ||
        !admitted(volume, attributes, 12, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint32_t relative = get(volume, attributes, 4), descriptor = get(volume, attributes + 4, 4), flags = get(volume, attributes + 8, 4);
    if ((descriptor & 3u) || !admitted(volume, descriptor, 8, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint32_t length = get(volume, descriptor, 2), maximum = get(volume, descriptor + 2, 2), buffer = get(volume, descriptor + 4, 4);
    if (length > maximum || length > 1024) return FF_BOOT_UNSUPPORTED;
    if (length && !admitted(volume, buffer, length, FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    if (overlap(output, 4, iosb, 8) || overlap(output, 4, attributes, 12) || overlap(iosb, 8, attributes, 12) ||
        overlap(output, 4, descriptor, 8) || overlap(iosb, 8, descriptor, 8) ||
        overlap(output, 4, buffer, length) || overlap(iosb, 8, buffer, length)) return FF_BOOT_UNSUPPORTED;
    /* Read/list/traverse/attributes/control only. Other rights and modes are
     * explicit unsupported contracts, not simulated file-operation success. */
    if ((flags & ~0x40u) || (share & ~7u) || (access & ~0x001200a9u) ||
        (options & ~0x00800061u)) return FF_BOOT_UNSUPPORTED;
    if ((options & 0x20u) && !(access & 0x00100000u)) return FF_BOOT_INVALID;
    if ((options & 0x41u) == 0x41u) return FF_BOOT_INVALID;
    if (relative) {
        if (!find(volume, relative)) return FF_BOOT_BAD_HANDLE;
        if (length) return FF_BOOT_UNSUPPORTED;
    } else {
        uint32_t count = length;
        if (count && get(volume, buffer + count - 1, 1) == '\\') --count;
        if (count != sizeof root - 1) return FF_BOOT_UNSUPPORTED;
        for (uint32_t i = 0; i < count; ++i) {
            uint32_t a = get(volume, buffer + i, 1), b = (uint8_t)root[i];
            if (flags & 0x40u) { if (a >= 'A' && a <= 'Z') a += 'a' - 'A'; if (b >= 'A' && b <= 'Z') b += 'a' - 'A'; }
            if (a != b) return FF_BOOT_UNSUPPORTED;
        }
    }
    if (options & 0x40u) return io_result(volume, iosb, FF_VOLUME_IS_DIRECTORY, 0);
    for (unsigned i = 0; i < FF_VOLUME_HANDLES; ++i) {
        const ff_volume_handle *old = &volume->handles[i];
        if (old->handle && (((access & 0x21u) && !(old->share & 1u)) || ((old->access & 0x21u) && !(share & 1u))))
            return io_result(volume, iosb, FF_VOLUME_SHARING_VIOLATION, 0);
    }
    ff_volume_handle *slot = NULL;
    for (unsigned i = 0; i < FF_VOLUME_HANDLES; ++i) if (!volume->handles[i].handle) { slot = &volume->handles[i]; break; }
    if (!slot || volume->next_handle >= FF_VOLUME_HANDLE_LIMIT) return io_result(volume, iosb, FF_VOLUME_TOO_MANY_HANDLES, 0);
    *slot = (ff_volume_handle){ volume->next_handle, access, share, options };
    volume->next_handle += 4; ++volume->active; ++volume->opened;
    put(volume, output, slot->handle, 4);
    return io_result(volume, iosb, 0, 1); /* FILE_OPENED */
}
uint32_t ff_volume_query_size(ff_local_volume *volume, const uint32_t args[5])
{
    if (!volume || !volume->memory || !args) return FF_BOOT_INVALID;
    if (!find(volume, args[0])) return FF_BOOT_BAD_HANDLE;
    if (args[4] != 3) return FF_BOOT_UNSUPPORTED; /* FileFsSizeInformation */
    const uint32_t iosb = args[1], output = args[2], bytes = args[3];
    if ((iosb | output) & 3u) return FF_BOOT_INVALID;
    if (!admitted(volume, iosb, 8, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    if (overlap(iosb, 8, output, bytes < 24 ? bytes : 24)) return FF_BOOT_UNSUPPORTED;
    if (bytes < 24) return io_result(volume, iosb, FF_VOLUME_BUFFER_TOO_SMALL, 0);
    if (!admitted(volume, output, 24, FF_VM_WRITE)) return FF_VM_ACCESS_VIOLATION;
    put(volume, output, volume->profile.total_units, 8); put(volume, output + 8, volume->profile.available_units, 8);
    put(volume, output + 16, volume->profile.sectors_per_unit, 4); put(volume, output + 20, volume->profile.bytes_per_sector, 4);
    return io_result(volume, iosb, 0, 24);
}
uint32_t ff_volume_close(ff_local_volume *volume, uint32_t handle)
{
    ff_volume_handle *slot = find(volume, handle);
    if (!slot) return FF_BOOT_BAD_HANDLE;
    memset(slot, 0, sizeof *slot); --volume->active; ++volume->closed;
    return 0;
}
