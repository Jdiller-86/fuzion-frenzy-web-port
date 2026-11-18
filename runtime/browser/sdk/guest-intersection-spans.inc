#include "cpu/x86_ops.h"
#include "full-memory.h"

namespace ff_browser_intersection {

struct Spans {
    uint8_t* stack;
    const uint8_t* left;
    const uint8_t* right;
    uint8_t* result;
    uint32_t leftAddress;
    uint32_t rightAddress;
    uint32_t resultAddress;
};

[[gnu::always_inline]] inline bool TrySpans(uint32_t esp, Spans& spans) noexcept {
    using ff_browser_memory::TryWindow;
    auto* stack = TryWindow(esp - 32, 48);
    if (!stack) return false;

    // The original leaf uses an owned caller stack. Capture its pointer
    // arguments before the callee reuses the first two slots as scratch.
    const auto leftAddress = *reinterpret_cast<const x86_u32u*>(stack + 36);
    const auto rightAddress = *reinterpret_cast<const x86_u32u*>(stack + 40);
    const auto resultAddress = *reinterpret_cast<const x86_u32u*>(stack + 44);
    const auto* left = TryWindow(leftAddress, 16);
    const auto* right = TryWindow(rightAddress, 16);
    auto* result = TryWindow(resultAddress, 24);
    if (!left || !right || !result) return false;

    // Keep optional normal writes in the checked body. Its negations can
    // produce a different NaN sign on degenerate segments. The result
    // flags must also remain outside the mutable callee stack span.
    const auto* flags = result + 16;
    if ((*flags & 4u) || (flags >= stack && flags < stack + 48)) return false;

    spans = {stack, left, right, result, leftAddress, rightAddress, resultAddress};
    return true;
}

} // namespace ff_browser_intersection
