#include "kernel_error_map.h"

/* Owned status-to-error conversions. Each entry is a name-identical documented
 * NTSTATUS/winerror pair, except STATUS_OBJECT_NAME_COLLISION, whose value 80
 * (ERROR_FILE_EXISTS) is pinned by this title's own CRT: the open helper at
 * 0x1E93E4 pushes 0x50 for that status before returning -1.
 *
 * Statuses whose conversion has more than one candidate winerror column stay
 * unmapped on purpose (STATUS_BUFFER_TOO_SMALL 0xC0000023, STATUS_DISK_FULL
 * 0xC000007F, STATUS_INSUFFICIENT_RESOURCES 0xC000009A, STATUS_FILE_IS_A_DIRECTORY
 * 0xC00000BA, STATUS_NOT_A_DIRECTORY 0xC0000103). The lane records them as
 * deferred instead of picking a column; an unmapped status is a deliberate stop.
 */
typedef struct {
    uint32_t status, error;
} ff_kernel_error_pair;

static const ff_kernel_error_pair pairs[] = {
    {0x00000000u, 0u},   /* STATUS_SUCCESS -> ERROR_SUCCESS */
    {0xc0000008u, 6u},   /* STATUS_INVALID_HANDLE -> ERROR_INVALID_HANDLE */
    {0xc0000022u, 5u},   /* STATUS_ACCESS_DENIED -> ERROR_ACCESS_DENIED */
    {0xc0000034u, 2u},   /* STATUS_OBJECT_NAME_NOT_FOUND -> ERROR_FILE_NOT_FOUND */
    {0xc0000035u, 80u},  /* STATUS_OBJECT_NAME_COLLISION -> ERROR_FILE_EXISTS */
    {0xc000003au, 3u},   /* STATUS_OBJECT_PATH_NOT_FOUND -> ERROR_PATH_NOT_FOUND */
    {0xc0000043u, 32u},  /* STATUS_SHARING_VIOLATION -> ERROR_SHARING_VIOLATION */
};

uint32_t ff_kernel_error_from_status(uint32_t status)
{
    for (unsigned i = 0; i < sizeof pairs / sizeof pairs[0]; ++i)
        if (pairs[i].status == status) return pairs[i].error;
    return FF_KERNEL_ERROR_UNMAPPED;
}
