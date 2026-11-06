#include "reg_save_order.h"

uint32_t ff_reg_order_is_reverse(const uint8_t *push_order, const uint8_t *pop_order,
                                 uint32_t count)
{
    if (!push_order || !pop_order || count < 2u || count > 8u) return 0u;
    for (uint32_t i = 0; i < count; ++i)
        if (pop_order[i] != push_order[count - 1u - i]) return 0u;
    return 1u;
}
