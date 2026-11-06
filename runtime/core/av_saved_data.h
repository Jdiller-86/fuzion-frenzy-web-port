#ifndef FF_AV_SAVED_DATA_H
#define FF_AV_SAVED_DATA_H
#include <stdint.h>

/* Bounded model for the observed ordinal-1 AvGetSavedDataAddress call.
 * The pinned HAL returns the static g_av_saved_data_address, initialized to 0.
 * Only AvSetSavedDataAddress (ordinal 4) changes it, and the guest translation
 * legitimately contains that call site in Present's nonzero branch, so a static
 * import or call-site check cannot prove absence. The guard is therefore a
 * runtime invariant: the model returns the documented 0 only when the recorded
 * dispatch journal contains no ordinal-4 call before this read. */
enum {
    FF_AV_SAVED_OK = 0,
    FF_AV_SAVED_UNDECLARED = 1,
    FF_AV_SAVED_SETTER = 2,
    FF_AV_SAVED_OUT = 3
};

/* Reads the recorded journal; a prior ordinal-4 dispatch fails the guard. */
uint32_t ff_av_saved_data_prior_setter(const uint32_t *prior_ordinals, uint32_t count);

uint32_t ff_av_saved_data_value(uint32_t declared, uint32_t prior_setter, uint32_t *value);

#endif
