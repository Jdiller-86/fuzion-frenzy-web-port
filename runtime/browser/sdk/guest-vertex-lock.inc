#include <array>
#include <cstdint>
#include <cstring>

#include "full-memory.h"
#include "kernel/memory.h"

namespace ff_browser_vertex_lock {

struct Slot {
    uint32_t base = 0;
    uint32_t bytes = 0;
};

// Recognize the checked, zero-offset lock of the title's rotating buffers.
// The saved caller ESI points at the object initialized by the buffer creator.
inline Slot FindSlot(uint32_t sp, uint32_t resource) {
    using ff_browser_memory::TryWindow;
    std::array<uint32_t, 10> stack;
    const auto* frame = TryWindow(sp, sizeof(stack));
    if (!frame) return {};
    std::memcpy(stack.data(), frame, sizeof(stack));
    if (stack[0] != 0x00201848 || stack[1] != resource ||
        stack[4] != 0x0003f784 || stack[5] != resource ||
        stack[6] || stack[7] || stack[9]) return {};

    constexpr uint32_t maxSlots = 8;
    constexpr uint32_t firstHeader = 0x4c;
    constexpr uint32_t headerBytes = 12;
    const uint32_t owner = stack[2];
    if (!owner) return {};
    const auto* object = TryWindow(owner, firstHeader);
    if (!object) return {};
    std::array<uint32_t, firstHeader / 4> fields;
    std::memcpy(fields.data(), object, sizeof(fields));
    const uint32_t vertices = fields[0x20 / 4], stride = fields[0x24 / 4];
    const uint32_t slots = fields[0x38 / 4], index = fields[0x3c / 4];
    if (!vertices || !stride || slots > maxSlots || index >= slots ||
        fields[index] != resource ||
        uint64_t(owner) + firstHeader + index * headerBytes != resource) return {};

    std::array<uint32_t, 2> header;
    const auto* record = TryWindow(resource, headerBytes);
    if (!record) return {};
    std::memcpy(header.data(), record, sizeof(header));
    if (header[0] & 0x70000) return {};

    const uint64_t payloadBytes = uint64_t(vertices) * stride;
    if (payloadBytes > mem::kPhysicalSize) return {};
    const uint64_t slotBytes = (payloadBytes + 4095) & ~uint64_t(4095);
    const auto allocation = ff_browser_memory::FindContiguousAllocation(fields[0x44 / 4]);
    if (!allocation.bytes) return {};
    const uint32_t pool = mem::VirtualFromPhysical(mem::PhysicalFromVirtual(fields[0x44 / 4]));
    if ((pool & 4095) ||
        uint64_t(pool) + slots * slotBytes > uint64_t(allocation.base) + allocation.bytes) return {};
    const uint32_t data = mem::VirtualFromPhysical(mem::PhysicalFromVirtual(header[1]));
    if (uint64_t(pool) + index * slotBytes != data) return {};
    return {data, uint32_t(slotBytes)};
}

}  // namespace ff_browser_vertex_lock
