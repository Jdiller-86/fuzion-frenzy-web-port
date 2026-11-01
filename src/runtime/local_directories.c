#include "local_volume.h"
#include "thread_bootstrap.h"
#include <string.h>

/* Independently authored directory metadata for the explicit fresh developer
 * volume. No host paths, file payloads, FATX sectors or persistence. */
static uint32_t read_word(const ff_local_volume *volume, uint32_t address, unsigned bytes)
{
    uint32_t value = 0;
    for (unsigned i = 0; i < bytes; ++i) value |= (uint32_t)volume->memory->ram[address + i] << (i * 8);
    return value;
}
static void write_word(ff_local_volume *volume, uint32_t address, uint32_t value)
{
    for (unsigned i = 0; i < 4; ++i) volume->memory->ram[address + i] = (uint8_t)(value >> (i * 8));
}
static int admitted(const ff_local_volume *volume, uint32_t address, uint32_t bytes, uint32_t access)
{
    return address && !ff_vm_check(volume->memory, address, bytes, access);
}
static int overlaps(uint32_t a, uint32_t bytes, uint32_t b, uint32_t size)
{
    return bytes && size && (uint64_t)a < (uint64_t)b + size && (uint64_t)b < (uint64_t)a + bytes;
}
static uint32_t result(ff_local_volume *volume, uint32_t iosb, uint32_t status, uint32_t information)
{
    write_word(volume, iosb, status); write_word(volume, iosb + 4, information);
    return status;
}
static ff_volume_handle *handle(ff_local_volume *volume, uint32_t value)
{
    for (unsigned i = 0; i < FF_VOLUME_HANDLES; ++i) if (value && volume->handles[i].handle == value) return &volume->handles[i];
    return NULL;
}
static unsigned folded(unsigned byte, uint32_t flags)
{
    return (flags & 0x40u) && byte >= 'A' && byte <= 'Z' ? byte + 'a' - 'A' : byte;
}
static int equal(const char *a, const char *b, uint32_t bytes, uint32_t flags)
{
    for (uint32_t i = 0; i < bytes; ++i) if (folded((uint8_t)a[i], flags) != folded((uint8_t)b[i], flags)) return 0;
    return 1;
}
static uint32_t open_path(ff_local_volume *volume, uint32_t relative, const char *path, uint32_t bytes, uint32_t flags,
                          uint32_t access, uint32_t share, uint32_t disposition, uint32_t options,
                          uint32_t *opened, uint32_t *information)
{
    static const char root[] = "\\Device\\Harddisk0\\partition1";
    if (!volume || !volume->memory || !opened || !information || (!path && bytes)) return FF_BOOT_INVALID;
    if (bytes >= 256 || (flags & ~0x40u) ||
        (access & ~0x001201ffu) || (share & ~7u) || (options & ~0x4021u) || !(options & 1u) ||
        disposition < 1 || disposition > 3) return FF_BOOT_UNSUPPORTED;
    if ((options & 0x20u) && !(access & 0x00100000u)) return FF_BOOT_INVALID;
    for (uint32_t i = 0; i < bytes; ++i) {
        unsigned byte = (uint8_t)path[i];
        if (byte < 32 || byte > 126 || strchr("/:*?\"<>|", (int)byte)) return FF_BOOT_UNSUPPORTED;
    }
    uint32_t parent = 1, cursor = 0;
    if (relative) {
        parent = relative;
        if (!parent || parent > FF_VOLUME_DIRECTORIES || volume->directories[parent - 1].id != parent) return FF_BOOT_UNSUPPORTED;
        if (bytes && path[0] == '\\') return FF_BOOT_UNSUPPORTED;
    } else {
        if (bytes < sizeof root - 1 || !equal(path, root, sizeof root - 1, flags) ||
            (bytes > sizeof root - 1 && path[sizeof root - 1] != '\\')) return FF_BOOT_UNSUPPORTED;
        cursor = sizeof root - 1;
        if (cursor < bytes) ++cursor;
    }
    uint32_t target = parent, new_slot = FF_VOLUME_DIRECTORIES;
    char final_name[FF_VOLUME_DIRECTORY_NAME] = {0};
    while (cursor < bytes) {
        uint32_t start = cursor;
        while (cursor < bytes && path[cursor] != '\\') ++cursor;
        const uint32_t length = cursor - start;
        if (!length || length >= FF_VOLUME_DIRECTORY_NAME || (length == 1 && path[start] == '.') ||
            (length == 2 && path[start] == '.' && path[start + 1] == '.')) return FF_BOOT_UNSUPPORTED;
        int last = cursor == bytes || cursor + 1 == bytes;
        target = 0;
        for (unsigned i = 0; i < FF_VOLUME_DIRECTORIES; ++i) {
            const ff_volume_directory *directory = &volume->directories[i];
            if (directory->id && directory->parent == parent && strlen(directory->name) == length && equal(directory->name, path + start, length, flags)) {
                target = directory->id; break;
            }
        }
        if (!target) {
            if (!last) return FF_VOLUME_PATH_NOT_FOUND;
            if (disposition == 1) return FF_VOLUME_NAME_NOT_FOUND;
            for (unsigned i = 0; i < FF_VOLUME_DIRECTORIES; ++i) if (!volume->directories[i].id) { new_slot = i; break; }
            if (new_slot == FF_VOLUME_DIRECTORIES) return FF_VOLUME_TOO_MANY_HANDLES;
            target = new_slot + 1;
            memcpy(final_name, path + start, length);
        } else if (last && disposition == 2) return FF_VOLUME_NAME_COLLISION;
        if (last) break;
        parent = target; ++cursor;
    }
    if (cursor == bytes && target == parent && disposition == 2) return FF_VOLUME_NAME_COLLISION;
    for (unsigned i = 0; i < FF_VOLUME_HANDLES; ++i) {
        const ff_volume_handle *old = &volume->handles[i];
        if (old->handle && old->directory == target && (((access & 0x21u) && !(old->share & 1u)) || ((old->access & 0x21u) && !(share & 1u)) ||
            ((access & 6u) && !(old->share & 2u)) || ((old->access & 6u) && !(share & 2u))))
            return FF_VOLUME_SHARING_VIOLATION;
    }
    ff_volume_handle *slot = NULL;
    for (unsigned i = 0; i < FF_VOLUME_HANDLES; ++i) if (!volume->handles[i].handle) { slot = &volume->handles[i]; break; }
    if (!slot || volume->next_handle >= FF_VOLUME_HANDLE_LIMIT) return FF_VOLUME_TOO_MANY_HANDLES;
    if (new_slot < FF_VOLUME_DIRECTORIES) {
        ff_volume_directory *directory = &volume->directories[new_slot];
        directory->id = target; directory->parent = parent;
        memcpy(directory->name, final_name, sizeof final_name);
        ++volume->directory_count; ++volume->directories_created;
    }
    *slot = (ff_volume_handle){volume->next_handle, access, share, options, target, 0};
    volume->next_handle += 4; ++volume->active; ++volume->opened;
    *opened=slot->handle; *information=new_slot < FF_VOLUME_DIRECTORIES ? 2 : 1;
    return 0;
}
uint32_t ff_volume_open_directory_path(ff_local_volume *volume, uint32_t directory, const char *path, uint32_t bytes,
                                       uint32_t access, uint32_t share, uint32_t options, uint32_t *opened)
{
    uint32_t information;
    return open_path(volume,directory,path,bytes,0x40,access,share,1,options,opened,&information);
}
uint32_t ff_volume_create_directory(ff_local_volume *volume, const uint32_t args[9])
{
    if (!volume || !volume->memory || !volume->memory->ram || !args) return FF_BOOT_INVALID;
    const uint32_t output=args[0], attributes=args[2], iosb=args[3];
    if ((output | attributes | iosb) & 3u) return FF_BOOT_INVALID;
    if (!admitted(volume,output,4,FF_VM_WRITE) || !admitted(volume,iosb,8,FF_VM_WRITE) ||
        !admitted(volume,attributes,12,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint32_t relative=read_word(volume,attributes,4), descriptor=read_word(volume,attributes+4,4);
    const uint32_t flags=read_word(volume,attributes+8,4);
    if ((descriptor & 3u) || !admitted(volume,descriptor,8,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    const uint32_t bytes=read_word(volume,descriptor,2), maximum=read_word(volume,descriptor+2,2), buffer=read_word(volume,descriptor+4,4);
    if (bytes > maximum || bytes >= 256) return FF_BOOT_UNSUPPORTED;
    if (bytes && !admitted(volume,buffer,bytes,FF_VM_READ)) return FF_VM_ACCESS_VIOLATION;
    if (overlaps(output,4,iosb,8) || overlaps(output,4,attributes,12) || overlaps(iosb,8,attributes,12) ||
        overlaps(output,4,descriptor,8) || overlaps(iosb,8,descriptor,8) ||
        overlaps(output,4,buffer,bytes) || overlaps(iosb,8,buffer,bytes)) return FF_BOOT_UNSUPPORTED;
    if (args[4] || (args[5]!=0 && args[5]!=0x10 && args[5]!=0x80)) return FF_BOOT_UNSUPPORTED;
    char path[256];
    for(uint32_t i=0;i<bytes;++i) path[i]=(char)read_word(volume,buffer+i,1);
    path[bytes]=0;
    ff_volume_handle *base=relative?handle(volume,relative):NULL;
    if(relative && !base) return FF_BOOT_BAD_HANDLE;
    uint32_t opened=0,information=0;
    uint32_t status=open_path(volume,base?base->directory:0,path,bytes,flags,args[1],args[6],args[7],args[8],&opened,&information);
    if (status==FF_BOOT_UNSUPPORTED || status==FF_BOOT_INVALID) return status;
    if (!status) write_word(volume,output,opened);
    return result(volume,iosb,status,information);
}
