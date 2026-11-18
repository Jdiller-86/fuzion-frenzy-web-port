template<size_t Bytes>
inline bool DeferredStateEqual(const uint32_t* active, const uint8_t* guest) {
    if constexpr (Bytes < 16) {
        return std::memcmp(active, guest, Bytes) == 0;
    } else {
        const auto* bytes = reinterpret_cast<const uint8_t*>(active);
        auto different = wasm_v128_xor(wasm_v128_load(bytes), wasm_v128_load(guest));
        for (size_t offset = 16; offset + 16 <= Bytes; offset += 16) {
            different = wasm_v128_or(different,
                wasm_v128_xor(wasm_v128_load(bytes + offset), wasm_v128_load(guest + offset)));
        }
        if constexpr (Bytes % 16 != 0) {
            different = wasm_v128_or(different,
                wasm_v128_xor(wasm_v128_load(bytes + Bytes - 16), wasm_v128_load(guest + Bytes - 16)));
        }
        return !wasm_v128_any_true(different);
    }
}

template<uint32_t Address, size_t Bytes>
inline void RefreshDeferredSpan(const uint32_t* active) {
    const auto* guest = ff_browser_memory::TryWindow(Address, Bytes);
    if (!guest) {
        Refresh(active, Address, Bytes);
        return;
    }
    if (!DeferredStateEqual<Bytes>(active, guest)) {
        std::memcpy(const_cast<uint32_t*>(active), guest, Bytes);
        g_state.Mark(active, Bytes);
    }
}

template<uint32_t Base, const auto& Runs>
inline void RefreshDeferredRuns(const auto& states) {
    [&]<size_t... Index>(std::index_sequence<Index...>) {
        (RefreshDeferredSpan<Base + Runs.run[Index].title * 4, Runs.run[Index].count * 4>(
            &states[Runs.run[Index].host]), ...);
    }(std::make_index_sequence<Runs.n>{});
}

void SyncDeferred() {
    // Keep the SDK's predefined run order and dirty bits. A fixed guest span
    // lets LLVM resolve the window and vectorize the byte comparison once.
    RefreshDeferredRuns<kRenderState, kRsDeferred>(g_state.rs);
    [&]<size_t... Stage>(std::index_sequence<Stage...>) {
        (RefreshDeferredRuns<kTextureState + Stage * kStageStates * 4, kTssDeferred>(g_state.tss[Stage]), ...);
    }(std::make_index_sequence<4>{});
}
