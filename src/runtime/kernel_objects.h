#ifndef FF_KERNEL_OBJECTS_H
#define FF_KERNEL_OBJECTS_H
#include <stdint.h>

/* Object initialization only. Timers are not armed and callbacks are not run. */
uint32_t ff_initialize_dpc(uint8_t *ram, uint32_t bytes, uint32_t address, uint32_t routine, uint32_t context);
uint32_t ff_initialize_timer(uint8_t *ram, uint32_t bytes, uint32_t address, uint32_t type);
#endif
