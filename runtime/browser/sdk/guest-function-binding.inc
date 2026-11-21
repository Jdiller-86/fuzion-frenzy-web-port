#pragma once

#include <cstdint>
#include "cpu/x86_context.h"

namespace ff_browser_guest_binding {

using Function = void (*)(X86Context&, uint8_t*);

inline bool UsesDefaultImplementation(Function entry, Function implementation) noexcept {
    // ThinLTO can fold comparisons of distinct names before the linker
    // resolves weak aliases. Compare the loaded function table index.
    Function volatile resolved = entry;
    return resolved == implementation;
}

} // namespace ff_browser_guest_binding
