#ifndef FF_KERNEL_ERROR_MAP_H
#define FF_KERNEL_ERROR_MAP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NTSTATUS -> Win32 (DOS) error conversion for the closed status vocabulary
 * this runtime's own file models can emit. The pairs below are the documented
 * name-identical conversions plus the collision value this title's CRT pins
 * itself; a status with no pinned conversion returns FF_KERNEL_ERROR_UNMAPPED
 * so a caller stops instead of inventing an error word or defaulting to
 * success. This models the conversion contract only: it is not an
 * original-kernel verdict about any refused request. */
#define FF_KERNEL_ERROR_UNMAPPED 0xffffffffu

uint32_t ff_kernel_error_from_status(uint32_t status);

#ifdef __cplusplus
}
#endif

#endif
