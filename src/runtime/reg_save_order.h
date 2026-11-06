#ifndef FF_REG_SAVE_ORDER_H
#define FF_REG_SAVE_ORDER_H
#include <stdint.h>

/* Pure rule for callee-saved register restoration. A pop sequence is correct
 * only when it is the exact reverse of the push sequence. The reached
 * FUN_001FB640 epilogue pushes ebx, ebp, esi, edi and the literal source pops
 * edi, esi, ebp, ebx (reverse); the observed runtime restores the saved-EDI
 * value into ESI, so the executed mapping does not satisfy this rule. */
uint32_t ff_reg_order_is_reverse(const uint8_t *push_order, const uint8_t *pop_order,
                                 uint32_t count);

#endif
