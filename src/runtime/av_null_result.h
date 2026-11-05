#ifndef FF_AV_NULL_RESULT_H
#define FF_AV_NULL_RESULT_H
#include <stdint.h>

/* Bounded model for AvSendTVEncoderOption calls whose Result pointer is null.
 * The pinned HAL tests the pointer before its option switch (`if (!Result)
 * return;`), unconditionally on handle, option and param, so every null-Result
 * call logs and returns without writing or failing. A nonzero Result reaches
 * the option switch, which is a different unmodeled path. */
/* The two observed null-result shapes retained as pair evidence. */
#define FF_AV_NULL_HANDLE 0x03920000u
#define FF_AV_NULL_OPTION 9u
#define FF_AV_NULL_DEVICE 0xFD000000u

enum {
    FF_AV_NULL_ACCEPT = 0,
    FF_AV_NULL_ARGS = 1,
    FF_AV_NULL_RESULT = 2,
    FF_AV_NULL_UNDECLARED = 3
};

uint32_t ff_av_null_result_accept(uint32_t declared, uint32_t handle, uint32_t option,
                                  uint32_t param, uint32_t result);

#endif
