#pragma once

#include <cstddef>
#include <cstdint>

// Browser SDK guest address translation. A range must fit wholly within one
// admitted guest window; arithmetic never wraps across the 32-bit VA space.
uint8_t* ff_sdk_guest_ptr(uint32_t va, size_t bytes = 1);
uint8_t* ff_sdk_guest_base();
