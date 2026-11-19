#pragma once

#include <algorithm>
#include <cstdint>

#include "cpu/x86_context.h"
#include "full-memory.h"

bool ff_browser_fast_integer_enabled();

namespace ff_browser_integer {

struct Counts {
    uint32_t values = 0;
    uint32_t digits = 0;
};

inline thread_local Counts counts;

inline bool TryAccumulate(X86Context& ctx) noexcept {
    uint32_t value = ctx.ecx - uint32_t('0');
    if (value > 9 || !ff_browser_fast_integer_enabled()) return false;

    using ff_browser_memory::kWindowSize;
    // The guest has already classified whitespace and sign and has loaded the
    // first digit. Admit only a whole run ending inside this mapped window.
    const uint32_t available = std::min(32u, kWindowSize - (ctx.esi & (kWindowSize - 1)));
    const auto* input = ff_browser_memory::TryWindow(ctx.esi, available);
    if (!input) return false;
    for (uint32_t offset = 0; offset < available; ++offset) {
        const uint32_t byte = input[offset];
        const uint32_t digit = byte - uint32_t('0');
        if (digit > 9) {
            ctx.eax = value;
            ctx.ecx = byte;
            ctx.esi += offset + 1;
            ++counts.values;
            counts.digits += offset + 1;
            return true;
        }
        value = value * 10u + digit;
    }
    return false;
}

} // namespace ff_browser_integer
