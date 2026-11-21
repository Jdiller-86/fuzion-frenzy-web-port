#pragma once

#include "cpu/x86_ops.h"
#include "full-memory.h"

namespace ff_browser_stack {

template<typename... Words>
[[gnu::noinline, gnu::cold]] void FaultablePush(X86Context& ctx, Words... words) {
    ((ctx.esp -= 4, X86_MEM32(ctx.esp) = uint32_t(words)), ...);
}

template<typename... Words>
[[gnu::always_inline]] inline void Push(X86Context& ctx, Words... words) {
    constexpr uint32_t bytes = 4 * sizeof...(words);
    const uint32_t start = ctx.esp - bytes;
    auto* stack = ff_browser_memory::TryWindow(start, bytes);
    if (!stack) [[unlikely]] {
        FaultablePush(ctx, words...);
        return;
    }
    auto* cursor = stack + bytes;
    ((*reinterpret_cast<x86_u32u*>(cursor -= 4) = uint32_t(words)), ...);
    ctx.esp = start;
}

template<typename... Registers>
[[gnu::noinline, gnu::cold]] void FaultablePop(X86Context& ctx, Registers&... registers) {
    auto pop = [&](uint32_t& reg) {
        const uint32_t word = X86_MEM32(ctx.esp);
        ctx.esp += 4;
        reg = word;
    };
    (pop(registers), ...);
}

template<typename... Registers>
[[gnu::always_inline]] inline void Pop(X86Context& ctx, Registers&... registers) {
    constexpr uint32_t bytes = 4 * sizeof...(registers);
    const uint32_t start = ctx.esp;
    const auto* stack = ff_browser_memory::TryWindow(start, bytes);
    if (!stack) [[unlikely]] {
        FaultablePop(ctx, registers...);
        return;
    }
    auto* cursor = stack;
    ((registers = *reinterpret_cast<const x86_u32u*>(cursor), cursor += 4), ...);
    ctx.esp = start + bytes;
}

} // namespace ff_browser_stack
