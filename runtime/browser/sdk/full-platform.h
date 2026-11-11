#pragma once

#include <cstdint>
#include <memory>

// Browser host ABI. Buttons use the Xbox XINPUT_GAMEPAD bit layout; axes are
// signed 16-bit Xbox stick coordinates and triggers are 0..255.
extern "C" void ff_browser_set_pad(int port, uint32_t buttons, int16_t lx, int16_t ly,
                                    int16_t rx, int16_t ry, uint8_t left_trigger,
                                    uint8_t right_trigger);

namespace gpu { class Backend; }
namespace gpu { std::unique_ptr<Backend> CreateBrowserBackend(); }

