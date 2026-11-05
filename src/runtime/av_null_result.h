#ifndef FF_AV_NULL_RESULT_H
#define FF_AV_NULL_RESULT_H
#include <stdint.h>

/* Bounded model for the observed third AvSendTVEncoderOption call. The pinned
 * HAL tests the Result pointer before its option switch (`if (!Result)
 * return;`), so this observed shape logs and returns without writing or
 * failing. Only the exact tuple is admitted; a nonzero Result would reach the
 * option-9 query path, which is a different unmodeled behavior. */
#define FF_AV_NULL_HANDLE 0x03920000u
#define FF_AV_NULL_OPTION 9u

enum {
    FF_AV_NULL_ACCEPT = 0,
    FF_AV_NULL_ARGS = 1,
    FF_AV_NULL_RESULT = 2,
    FF_AV_NULL_UNDECLARED = 3
};

uint32_t ff_av_null_result_accept(uint32_t declared, uint32_t handle, uint32_t option,
                                  uint32_t param, uint32_t result);

#endif
