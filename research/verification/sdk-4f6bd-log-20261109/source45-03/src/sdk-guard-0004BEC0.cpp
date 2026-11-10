#include "sdk-shared-prototypes.hpp"
X86_FUNC(sub_0004BEC0) {
    (void)base;
    ff_sdk_raise(ctx, 0x0004BEC0u, "original nested call remains guarded");
}
