/* Owned saved-data address scenarios; no original code or device. */
#include "../src/runtime/av_saved_data.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    uint32_t value = 0xDEADBEEFu;
    const uint32_t clean[] = {1u, 2u, 3u, 159u};
    const uint32_t with_setter[] = {1u, 4u, 2u};
    const uint32_t late_only[] = {4u};

    /* Journal guard: no prior ordinal-4 dispatch. */
    assert(ff_av_saved_data_prior_setter(clean, 4u) == 0u);
    assert(ff_av_saved_data_prior_setter(NULL, 0u) == 0u);
    /* A prior setter dispatch fails the guard; an unreadable journal fails closed. */
    assert(ff_av_saved_data_prior_setter(with_setter, 3u) == 1u);
    assert(ff_av_saved_data_prior_setter(late_only, 1u) == 1u);
    assert(ff_av_saved_data_prior_setter(NULL, 2u) == 1u);

    /* Declared, clean journal: the pinned static 0. */
    assert(ff_av_saved_data_value(1u, 0u, &value) == FF_AV_SAVED_OK);
    assert(value == 0u);

    /* Undeclared and prior-setter profiles are refused without a write. */
    value = 0xDEADBEEFu;
    assert(ff_av_saved_data_value(0u, 0u, &value) == FF_AV_SAVED_UNDECLARED);
    assert(value == 0xDEADBEEFu);
    assert(ff_av_saved_data_value(1u, 1u, &value) == FF_AV_SAVED_SETTER);
    assert(value == 0xDEADBEEFu);
    assert(ff_av_saved_data_value(1u, 0u, NULL) == FF_AV_SAVED_OUT);

    /* A repeated read after a recorded ordinal 4 is still refused. */
    assert(ff_av_saved_data_prior_setter(with_setter, 3u) == 1u);
    assert(ff_av_saved_data_value(1u, 1u, &value) == FF_AV_SAVED_SETTER);
    assert(value == 0xDEADBEEFu);

    printf("av_saved_data: ok\n");
    return 0;
}
