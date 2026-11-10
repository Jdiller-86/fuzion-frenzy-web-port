// Browser entry adapters for existing SDK kernel primitives. Unsupported
// services return false to the bridge, which stops with the original ordinal.
// Crypto algorithms come directly from the pinned SDK's kernel/crypto.cpp.
// Upstream attribution and license: SDK-NOTICE.txt in this directory.
#include <cstring>
#include "cpu/x86_ops.h"
#include "kernel/crypto.h"

extern "C" bool ff_sdk_browser_kernel_dispatch(X86Context& ctx, uint8_t* base, uint32_t ordinal) {
    auto span = [&](uint32_t address, uint64_t bytes) {
        return ff_sdk_checked_guest_span(ctx, base, address, bytes);
    };
    auto arg = [&](uint32_t index) {
        uint32_t value;
        std::memcpy(&value, span(ctx.esp, uint64_t(index + 2) * 4) + (index + 1) * 4, 4);
        return value;
    };
    uint32_t result = 0, count = 0;
    switch (ordinal) {
        case 268: { // RtlCompareMemory: length of the equal prefix
            count = 3;
            uint32_t bytes = arg(2);
            const auto* left = span(arg(0), bytes);
            const auto* right = span(arg(1), bytes);
            while (result < bytes && left[result] == right[result]) ++result;
            break;
        }
        case 284: // RtlFillMemory
            count = 3;
            std::memset(span(arg(0), arg(1)), int(arg(2) & 255u), arg(1));
            break;
        case 285: { // RtlFillMemoryUlong, retaining the SDK's whole-word rule
            count = 3;
            uint32_t bytes = arg(1), value = arg(2);
            auto* target = span(arg(0), bytes);
            for (uint64_t offset = 0; offset + 4 <= bytes; offset += 4)
                std::memcpy(target + offset, &value, 4);
            break;
        }
        case 298: // RtlMoveMemory
            count = 3;
            std::memmove(span(arg(0), arg(2)), span(arg(1), arg(2)), arg(2));
            break;
        case 307: // RtlUlongByteSwap, fastcall
            ctx.eax = __builtin_bswap32(ctx.ecx);
            ctx.esp += 4;
            return true;
        case 318: // RtlUshortByteSwap, fastcall
            ctx.eax = __builtin_bswap16(uint16_t(ctx.ecx));
            ctx.esp += 4;
            return true;
        case 320: // RtlZeroMemory
            count = 2;
            std::memset(span(arg(0), arg(1)), 0, arg(1));
            break;
        case 335: // XcSHAInit, Xbox SHA context is 116 bytes
            count = 1;
            crypto::ShaInit(span(arg(0), 116));
            break;
        case 336: // XcSHAUpdate
            count = 3;
            crypto::ShaUpdate(span(arg(0), 116), span(arg(1), arg(2)), arg(2));
            break;
        case 337: // XcSHAFinal
            count = 2;
            crypto::ShaFinal(span(arg(0), 116), span(arg(1), 20));
            break;
        case 338: // XcRC4Key
            count = 3;
            if (!arg(1)) return false;
            crypto::Rc4Key(span(arg(0), 258), span(arg(2), arg(1)), arg(1));
            break;
        case 339: // XcRC4Crypt
            count = 3;
            crypto::Rc4Crypt(span(arg(0), 258), span(arg(2), arg(1)), arg(1));
            break;
        case 340: { // XcHMAC
            count = 7;
            uint32_t first = arg(2), second = arg(4);
            if ((!first && arg(3)) || (!second && arg(5))) return false;
            crypto::Hmac(span(arg(0), arg(1)), arg(1), first ? span(first, arg(3)) : nullptr,
                         arg(3), second ? span(second, arg(5)) : nullptr, arg(5), span(arg(6), 20));
            break;
        }
        default:
            return false;
    }
    ctx.eax = result;
    ctx.esp += 4 + 4 * count;
    return true;
}
