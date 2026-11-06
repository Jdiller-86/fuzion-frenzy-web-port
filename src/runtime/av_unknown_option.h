#ifndef FF_AV_UNKNOWN_OPTION_H
#define FF_AV_UNKNOWN_OPTION_H
#include <stdint.h>

/* Bounded model for the observed late AvSendTVEncoderOption call with an
 * unknown option. The pinned HAL's default branch logs the unknown option and
 * writes 0 through Result without failing; it is not an encoder capability. */
#define FF_AV_UNKNOWN_HANDLE 0xFD000000u
#define FF_AV_UNKNOWN_OPTION 0x0Fu

enum {
    FF_AV_UNKNOWN_ACCEPT = 0,
    FF_AV_UNKNOWN_ARGS = 1,
    FF_AV_UNKNOWN_POINTER = 2
};

uint32_t ff_av_unknown_accept(uint32_t handle, uint32_t option, uint32_t param,
                              uint32_t result, uint32_t result_mapped_aligned);

#endif
