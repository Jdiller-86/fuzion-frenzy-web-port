// Synthetic execution of the unmodified production AllocSrvSlots method. No GPU is used.
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <utility>
#include <vector>

#ifdef NDEBUG
#error Assertions must remain enabled.
#endif

using HRESULT = long;
constexpr bool FAILED(HRESULT hr) { return hr < 0; }
enum { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE };
struct D3D12_DESCRIPTOR_HEAP_DESC { int Type; uint32_t NumDescriptors; int Flags; };
struct ID3D12DescriptorHeap { int id; uint32_t capacity; };
template<class T> struct ComPtr {
    std::shared_ptr<T> value;
    T* Get() const { return value.get(); }
};
#define IID_PPV_ARGS(p) p
#define LOGE(...) ((void)0)
#define LOGR(...) ((void)0)
namespace diag { bool Perf() { return false; } }

struct Device {
    int next_id = 1, attempts = 0;
    bool fail_next = false;
    HRESULT CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC* desc, ComPtr<ID3D12DescriptorHeap>* out) {
        ++attempts;
        if (std::exchange(fail_next, false)) return -1;
        out->value = std::make_shared<ID3D12DescriptorHeap>(ID3D12DescriptorHeap{next_id++, desc->NumDescriptors});
        return 0;
    }
    HRESULT GetDeviceRemovedReason() { return 0; }
};
struct CommandList {
    int binds = 0;
    ID3D12DescriptorHeap* srv = nullptr;
    ID3D12DescriptorHeap* sampler = nullptr;
    // Unrelated command-list state must survive heap rotation.
    int target = 73, pipeline = 81, constants = 94;
    void SetDescriptorHeaps(uint32_t count, ID3D12DescriptorHeap* const* heaps) {
        assert(count == 2);
        ++binds;
        srv = heaps[0]; sampler = heaps[1];
    }
};

// Constants and member method below are extracted from current source by the runner.
#include "production-constants.inc"
class D3D12Backend {
public:
    struct Frame {
        uint32_t srv = 0;
        size_t srv_page = 0;
        std::vector<ComPtr<ID3D12DescriptorHeap>> srv_overflow;
    };
    std::array<Frame, kFrames> frames_;
    uint64_t frame_ = 0;
    Device device;
    CommandList commands;
    Device* dev_ = &device;
    CommandList* cl_ = &commands;
    ComPtr<ID3D12DescriptorHeap> srv_heap_, base_srv_heap_, smp_heap_;
    D3D12Backend() {
        D3D12_DESCRIPTOR_HEAP_DESC hd{D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kSrvCount,
                                      D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE};
        assert(!FAILED(device.CreateDescriptorHeap(&hd, &base_srv_heap_)));
        assert(!FAILED(device.CreateDescriptorHeap(&hd, &smp_heap_)));
        srv_heap_ = base_srv_heap_;
    }
    Frame& CurrentFrame() { return frames_[frame_ % kFrames]; }
    uint32_t AllocSrvSlots(uint32_t n);
    // Explicit model of a completed frame fence. Production reset ordering is checked
    // separately against its exact source. This helper makes no GPU synchronization claim.
    void ResetAfterModeledFence(bool completed) {
        assert(completed);
        CurrentFrame().srv = 0;
        CurrentFrame().srv_page = 0;
        srv_heap_ = base_srv_heap_;
    }
};
#include "production-allocator.inc"

int main() {
    constexpr uint32_t per = kSrvCount / kFrames;
    for (uint64_t frame = 0; frame < kFrames; ++frame) {
        D3D12Backend b;
        b.frame_ = frame;
        assert(b.AllocSrvSlots(4) == frame * per);
        assert(b.CurrentFrame().srv == 4 && b.CurrentFrame().srv_page == 0);
        assert(b.srv_heap_.Get() == b.base_srv_heap_.Get());
    }
    std::puts("PASS all three base-heap frame partitions");

    D3D12Backend b;
    const auto base = b.base_srv_heap_.Get();
    assert(b.AllocSrvSlots(per - 4) == 0);
    assert(b.AllocSrvSlots(4) == per - 4);
    assert(b.commands.binds == 0 && b.CurrentFrame().srv_overflow.empty());
    assert(b.AllocSrvSlots(1) == 0);
    const auto first = b.srv_heap_.Get();
    assert(first != base && first->capacity == per);
    assert(b.CurrentFrame().srv_page == 1 && b.CurrentFrame().srv == 1);
    assert(b.commands.srv == first && b.commands.sampler == b.smp_heap_.Get());
    assert(b.commands.target == 73 && b.commands.pipeline == 81 && b.commands.constants == 94);
    assert(b.AllocSrvSlots(per - 1) == 1);
    assert(b.AllocSrvSlots(4) == 0);
    const auto second = b.srv_heap_.Get();
    assert(second != first && second != base && second->capacity == per);
    assert(b.CurrentFrame().srv_page == 2 && b.commands.binds == 2);
    // Descriptor addresses are (heap identity, slot), so slot zero cannot alias old draws.
    assert(b.CurrentFrame().srv_overflow[0].Get() == first);
    assert(b.CurrentFrame().srv_overflow[1].Get() == second);
    std::puts("PASS exact exhaustion and multiple overflow pages avoid address reuse");

    // Without a reset, further exhaustion allocates a fresh page, never page 0 again.
    assert(b.AllocSrvSlots(per - 4) == 4);
    assert(b.AllocSrvSlots(1) == 0);
    const auto third = b.srv_heap_.Get();
    assert(third != first && third != second);
    const int allocated = b.device.attempts;
    b.ResetAfterModeledFence(true);
    assert(b.srv_heap_.Get() == base && b.CurrentFrame().srv_page == 0);
    assert(b.CurrentFrame().srv_overflow.size() == 3);
    assert(b.AllocSrvSlots(per) == 0);
    assert(b.AllocSrvSlots(per) == 0 && b.srv_heap_.Get() == first);
    assert(b.AllocSrvSlots(1) == 0 && b.srv_heap_.Get() == second);
    assert(b.device.attempts == allocated);
    std::puts("PASS retained page pool reused only after modeled completed-fence reset");

    D3D12Backend failed;
    assert(failed.AllocSrvSlots(per) == 0);
    const auto old_heap = failed.srv_heap_.Get();
    failed.device.fail_next = true;
    assert(failed.AllocSrvSlots(4) == kInvalidSrvSlot);
    assert(failed.CurrentFrame().srv == per && failed.CurrentFrame().srv_page == 0);
    assert(failed.CurrentFrame().srv_overflow.empty());
    assert(failed.srv_heap_.Get() == old_heap && failed.commands.binds == 0);
    assert(failed.AllocSrvSlots(4) == 0);
    assert(failed.srv_heap_.Get() != old_heap);
    assert(failed.AllocSrvSlots(per - 4) == 4);
    const auto old_overflow = failed.srv_heap_.Get();
    failed.device.fail_next = true;
    assert(failed.AllocSrvSlots(1) == kInvalidSrvSlot);
    assert(failed.CurrentFrame().srv == per && failed.CurrentFrame().srv_page == 1);
    assert(failed.CurrentFrame().srv_overflow.size() == 1);
    assert(failed.srv_heap_.Get() == old_overflow && failed.commands.binds == 1);
    std::puts("PASS allocation failures preserve heap, cursor, page pool, and binding state");
    std::puts("All synthetic allocator cases passed; no GPU execution tested.");
}
