#pragma once

#include <array>
#include <atomic>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#include <utility>

#include <emscripten.h>

#include "cpu/x86_context.h"
#include "full-memory.h"
#include "guest-scene-timing.h"

namespace ff_browser_scene_node_pool {

using GuestFunction = void (*)(X86Context&, uint8_t*);
constexpr uint32_t kNodeBytes = 300;
constexpr uint32_t kStride = 304;
constexpr uint32_t kSlots = 256;
constexpr uint32_t kChunkBytes = kSlots * kStride;
constexpr uint32_t kPhysicalWindow = 0x80000000;
constexpr uint32_t kHeapHeaderBytes = 48;
constexpr uint32_t kChunkLimit = 48;
enum class Phase { Other, Create, Destroy };
enum class Reservation { Inactive, Pending, Large, Ordinary, Reserved };

inline uint32_t PhysicalAddress(uint32_t address) {
    using ff_browser_memory::kWindowBits;
    using ff_browser_memory::kWindowSize;
    if ((address >> kWindowBits) == (0xf0000000u >> kWindowBits))
        return kPhysicalWindow | (address & (kWindowSize - 1));
    return address;
}

struct Chunk {
    uint32_t address = 0;
    uint32_t used = 0;
    uint32_t live = 0;
    std::bitset<kSlots> occupied;

    bool Contains(uint32_t pointer) const {
        return address && pointer - address < kChunkBytes;
    }
};

struct Pool;
inline thread_local Pool* current = nullptr;

// Foreign contexts only consult this directory. Pool slot bookkeeping stays
// on the title thread, and directory updates occur once per block.
inline std::atomic<bool> active = false;
inline std::mutex directoryMutex;
inline std::array<uint32_t, kChunkLimit> directory{};
inline uint32_t foreignObservers = 0;

inline bool ForeignContains(uint32_t address) {
    if (!active.load(std::memory_order_acquire)) return false;
    std::lock_guard lock(directoryMutex);
    for (const auto block : directory) {
        if (block && address - block < kChunkBytes) return true;
    }
    return false;
}

inline void SetBlock(uint32_t index, uint32_t address) {
    std::lock_guard lock(directoryMutex);
    directory[index] = address;
}

inline uint32_t Read(const uint8_t* pointer) {
    uint32_t value;
    std::memcpy(&value, pointer, sizeof(value));
    return value;
}

inline uint32_t HeapCounter(uint32_t address) {
    return Read(ff_browser_memory::backing + address);
}

inline int Mode() {
    static const int selected = MAIN_THREAD_EM_ASM_INT({
        const pool = Module.onFuzionGpu?.sceneNodePool;
        return pool === 'pressure' ? 2 : pool === true ? 1 : 0;
    });
    return selected;
}

struct Pool {
    explicit Pool(const X86Context& ctx) : owner(&ctx) {
        if (!ff_browser_scene_timing::trace.active || current) return;
        tracing = true;
        liveBefore = HeapCounter(0x002CCEC8);
        peakBefore = HeapCounter(0x002CD700);
        mode = Mode();
        if (mode) {
            std::lock_guard lock(directoryMutex);
            if (!active.load(std::memory_order_relaxed)) {
                current = this;
                foreignObservers = 0;
                active.store(true, std::memory_order_release);
            }
        }
    }
    ~Pool() {
        if (!tracing) return;
        if (live || chunkCount != chunkFrees) std::abort();
        if (current == this) {
            std::lock_guard lock(directoryMutex);
            current = nullptr;
            heapObservers += foreignObservers;
            active.store(false, std::memory_order_release);
        }
        EM_ASM({
            console.timeStamp?.("ff-scene-node-pool:" +
                [$0,$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14].join(":"));
        }, nodeCount, valueCount, chunkCount, chunkFrees, live, rejected, heapObservers, tag,
           liveBefore, HeapCounter(0x002CCEC8), peakBefore, HeapCounter(0x002CD700),
           reservationMisses, ordinarySuccesses, foreignChecks);
    }

    Chunk* Find(uint32_t address) {
        if (lastFree && lastFree->Contains(address)) return lastFree;
        for (uint32_t i = chunkCount; i > 0; --i) {
            auto& chunk = chunks[i - 1];
            if (chunk.Contains(address)) return lastFree = &chunk;
        }
        return nullptr;
    }

    const X86Context* owner;
    Phase phase = Phase::Other;
    std::array<Chunk, kChunkLimit> chunks{};
    Chunk* lastFree = nullptr;
    uint32_t chunkCount = 0, chunkFrees = 0, live = 0;
    uint32_t nodeCount = 0, valueCount = 0, rejected = 0, heapObservers = 0;
    uint32_t tag = 0, liveBefore = 0, peakBefore = 0;
    uint32_t reservationTag = 0, reservationMisses = 0, ordinarySuccesses = 0, foreignChecks = 0;
    uint32_t releasing = kChunkLimit;
    Reservation reservation = Reservation::Inactive;
    int mode = 0;
    bool tracing = false, disabledByPressure = false;
};

struct PhaseScope {
    explicit PhaseScope(Phase next) : pool(current), previous(pool ? pool->phase : Phase::Other) {
        if (pool) pool->phase = next;
    }
    ~PhaseScope() { if (pool) pool->phase = previous; }
    Pool* pool;
    Phase previous;
};

struct Argument {
    Argument(uint8_t* address, uint32_t replacement) : address(address), original(Read(address)) {
        std::memcpy(address, &replacement, sizeof(replacement));
    }
    ~Argument() { std::memcpy(address, &original, sizeof(original)); }
    uint8_t* address;
    uint32_t original;
};

inline bool ManagedTag(uint32_t& tag) {
    using ff_browser_memory::TryWindow;
    const auto* mode = TryWindow(0x00285338, 4);
    const auto* state = TryWindow(0x002CD788, 8);
    if (!mode || !state || Read(mode) != UINT32_MAX || !Read(state + 4)) return false;
    const uint32_t index = Read(state);
    if (index >= 32) return false;
    const auto* value = TryWindow(0x002CD708 + index * 4, 4);
    if (!value) return false;
    tag = Read(value);
    return true;
}

inline void MarkManagedAllocation(const X86Context& ctx) {
    if (!current || current->owner != &ctx || current->reservation != Reservation::Large) return;
    const auto* stack = ff_browser_memory::TryWindow(ctx.esp, 4);
    if (stack && Read(stack) == 0x4C4A8) current->reservation = Reservation::Reserved;
}

inline void MarkManagedFree(const X86Context& ctx) {
    if (!current || current->owner != &ctx || current->releasing == kChunkLimit) return;
    // The original free has finished editing the heap. Retire the range
    // before it releases the guest lock and another allocation can reuse it.
    SetBlock(current->releasing, 0);
    current->releasing = kChunkLimit;
}

inline void BeginReservation(X86Context& ctx) {
    auto* pool = current;
    if (!pool || pool->owner != &ctx || pool->reservation != Reservation::Pending) return;
    const auto* stack = ff_browser_memory::TryWindow(ctx.esp, 0x28);
    if (!stack || Read(stack + 0x0C) != 0x4C4A8 || Read(stack + 0x10) != kNodeBytes ||
        Read(stack + 0x1C) != 0x1001 || Read(stack + 0x20) != pool->reservationTag ||
        Read(stack + 0x24) || ctx.eax || ctx.ebx != kStride || ctx.edi != kStride+kHeapHeaderBytes)
        return;
    // The original guest heap lock is held. Leave all actual allocator
    // arguments alone so the small request retains its original failure path.
    pool->reservation = Reservation::Large;
    ctx.ebx = kChunkBytes;
    const bool forceMiss = pool->mode == 2 && pool->chunkCount == 1;
    ctx.edi = forceMiss ? UINT32_MAX : kChunkBytes + kHeapHeaderBytes;
}

inline bool RetryOrdinary(X86Context& ctx) {
    auto* pool = current;
    if (!pool || pool->owner != &ctx || pool->reservation != Reservation::Large) return false;
    ctx.ebx = kStride;
    ctx.edi = kStride + kHeapHeaderBytes;
    pool->reservation = Reservation::Ordinary;
    pool->disabledByPressure = true;
    ++pool->reservationMisses;
    return true;
}

inline void CheckForeignDirectory(Pool& pool, uint32_t address, bool contains) {
    // Exercise the same lookup on a different Wasm pthread in the forced
    // pressure replay only. The performance comparison never spawns a probe.
    bool valid = false;
    std::thread peer([address, contains, &valid] {
        const uint32_t alias = 0xf0000000u | (address - kPhysicalWindow);
        valid = !current && (ForeignContains(PhysicalAddress(alias)) == contains) &&
            !ForeignContains(address - 1) && !ForeignContains(address + kChunkBytes);
    });
    peer.join();
    if (!valid) std::abort();
    ++pool.foreignChecks;
}

inline void Allocate(X86Context& ctx, uint8_t* base, GuestFunction original) {
    auto* pool = current;
    if (!pool || pool->phase != Phase::Create || pool->owner != &ctx ||
        pool->disabledByPressure) return original(ctx, base);
    auto* stack = ff_browser_memory::TryWindow(ctx.esp, 36);
    if (!stack) return original(ctx, base);
    const uint32_t site = Read(stack);
    if (site != 0x7ED1D && site != 0x7ED9D) return original(ctx, base);
    const auto* name = ff_browser_memory::TryWindow(Read(stack + 32), 32);
    uint32_t tag = 0;
    if (Read(stack + 4) != kNodeBytes || Read(stack + 16) != 0x1001 ||
        !name || !std::memchr(name, 0, 32) || !ManagedTag(tag) ||
        (pool->chunkCount && pool->tag != tag)) {
        ++pool->rejected;
        return original(ctx, base);
    }
    auto* chunk = pool->chunkCount ? &pool->chunks[pool->chunkCount - 1] : nullptr;
    if (!chunk || chunk->used == kSlots) {
        if (pool->chunkCount == pool->chunks.size()) {
            ++pool->rejected;
            return original(ctx, base);
        }
        const uint32_t originalEsp = ctx.esp;
        pool->reservation = Reservation::Pending;
        pool->reservationTag = tag;
        original(ctx, base);
        const auto result = std::exchange(pool->reservation, Reservation::Inactive);
        if (result == Reservation::Ordinary) {
            if (ctx.eax && ctx.esp == originalEsp + 4 &&
                ff_browser_memory::TryWindow(ctx.eax, kNodeBytes)) ++pool->ordinarySuccesses;
            return;
        }
        if (result == Reservation::Pending) {
            ++pool->rejected;
            return;
        }
        const uint32_t pointer = ctx.eax;
        if (result != Reservation::Reserved || ctx.esp != originalEsp + 4 ||
            pointer - kPhysicalWindow > ff_browser_memory::kWindowSize - kChunkBytes ||
            (pointer & 15) || !ff_browser_memory::TryWindow(pointer, kChunkBytes)) std::abort();
        SetBlock(pool->chunkCount, pointer);
        chunk = &pool->chunks[pool->chunkCount++];
        chunk->address = pointer;
        pool->tag = tag;
        if (pool->mode == 2 && pool->chunkCount == 1) CheckForeignDirectory(*pool, pointer, true);
    } else {
        ctx.esp += 4; // Consume only the return. The caller owns its arguments.
    }
    const uint32_t slot = chunk->used++;
    ctx.eax = chunk->address + slot * kStride;
    chunk->occupied.set(slot);
    ++chunk->live;
    ++pool->live;
    if (site == 0x7ED1D) ++pool->nodeCount;
    else ++pool->valueCount;
}

inline void Free(X86Context& ctx, uint8_t* base, GuestFunction original) {
    auto* pool = current;
    if (!active.load(std::memory_order_acquire)) return original(ctx, base);
    auto* stack = ff_browser_memory::TryWindow(ctx.esp, 8);
    if (!stack) return original(ctx, base);
    const auto pointer = PhysicalAddress(Read(stack + 4));
    if (!pool || pool->owner != &ctx) {
        if (ForeignContains(pointer)) std::abort();
        return original(ctx, base);
    }
    auto* chunk = pool->Find(pointer);
    if (!chunk) return original(ctx, base);
    const uint32_t offset = pointer - chunk->address;
    const uint32_t slot = offset / kStride;
    if (pool->phase != Phase::Destroy || pool->owner != &ctx || Read(stack) != 0x7EF29 ||
        slot >= chunk->used || offset % kStride || !chunk->occupied.test(slot)) std::abort();
    chunk->occupied.reset(slot);
    --pool->live;
    if (--chunk->live) {
        ctx.esp += 4;
        return;
    }
    const uint32_t originalEsp = ctx.esp;
    pool->releasing = uint32_t(chunk - pool->chunks.data());
    {
        Argument address(stack + 4, chunk->address);
        original(ctx, base);
    }
    if (pool->releasing != kChunkLimit || ctx.esp != originalEsp + 4) std::abort();
    const uint32_t retired = std::exchange(chunk->address, 0);
    ++pool->chunkFrees;
    if (pool->mode == 2) CheckForeignDirectory(*pool, retired, false);
}

inline void HeapObserver(const X86Context& ctx, bool sizeQuery) {
    if (!active.load(std::memory_order_acquire)) return;
    auto* pool = current;
    const bool owner = pool && pool->owner == &ctx;
    if (sizeQuery) {
        const auto* stack = ff_browser_memory::TryWindow(ctx.esp, 8);
        if (!stack) return;
        const auto pointer = PhysicalAddress(Read(stack + 4));
        if (owner ? bool(pool->Find(pointer)) : ForeignContains(pointer)) std::abort();
        return;
    }
    if (owner) {
        if (pool->live) ++pool->heapObservers;
    } else {
        std::lock_guard lock(directoryMutex);
        for (const auto block : directory) {
            if (!block) continue;
            ++foreignObservers;
            break;
        }
    }
}

} // namespace ff_browser_scene_node_pool
