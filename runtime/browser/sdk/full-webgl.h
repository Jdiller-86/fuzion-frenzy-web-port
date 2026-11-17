#pragma once
#include "gpu/state.h"
bool ff_webgl_enabled();
void ff_webgl_target(const gpu::Surface&, bool depth);
bool ff_webgl_draw(const void*, uint32_t, uint32_t, const void*, uint32_t,
                   const uint32_t*, const float*, const char*, const uint32_t*);
void ff_webgl_clear(const gpu::State&, uint32_t, const uint8_t*, uint32_t, uint32_t, float, uint32_t);
bool ff_webgl_present(const gpu::Surface&);
void ff_webgl_sync(uint32_t, uint32_t);
void ff_webgl_invalidate(uint32_t);
