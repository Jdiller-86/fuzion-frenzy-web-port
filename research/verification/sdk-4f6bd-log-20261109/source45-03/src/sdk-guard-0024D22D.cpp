#include "ff_sdk_adapter.hpp"
#include "cpu/x86_ops.h"
X86_FUNC(sub_0024D22D) {
    (void)base;
    ff_sdk_raise(ctx, 0x0024D22Du, "untranslated SDK direct target");
}
