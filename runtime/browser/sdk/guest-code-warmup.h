#pragma once

#include "cpu/x86_context.h"

namespace ff_browser_code_warmup {

// Only the startup entry owns this host context. Its address is outside
// guest backing and distinct from each running title thread's context.
inline X86Context context{};

inline bool IsWarmup(const X86Context& candidate) noexcept {
    return &candidate == &context;
}

} // namespace ff_browser_code_warmup
