// The dispatcher lock protects this list and the object states. A registration
// lives only while its thread waits on the dispatcher condition variable.
class ParkedWait {
public:
    explicit ParkedWait(std::span<const ObjectRef> objects) noexcept : objects_(objects) {
        if (next_) next_->link_ = &next_;
        head_ = this;
    }

    ParkedWait(const ParkedWait&) = delete;
    ParkedWait& operator=(const ParkedWait&) = delete;

    ~ParkedWait() {
        *link_ = next_;
        if (next_) next_->link_ = link_;
    }

    static bool Includes(const Object& object) {
        for (auto* wait = head_; wait; wait = wait->next_) {
            for (const auto& candidate : wait->objects_) {
                if (candidate.get() == &object) return true;
            }
        }
        return false;
    }

#ifdef FF_BROWSER_DISPATCHER_PROBE
    static bool HasWaiters() { return head_ != nullptr; }
#endif

private:
    static inline ParkedWait* head_ = nullptr;
    std::span<const ObjectRef> objects_;
    ParkedWait* next_ = head_;
    ParkedWait** link_ = &head_;
};

#ifdef FF_BROWSER_DISPATCHER_PROBE
static std::atomic<uint32_t> event_wake_probe{0};
static std::atomic<uint32_t> event_signals{0};
static std::atomic<uint32_t> event_matched{0};
static std::atomic<uint32_t> event_unrelated{0};
#endif

static bool ShouldWakeEvent(const Object& object) {
    const bool interested = ParkedWait::Includes(object);
#ifdef FF_BROWSER_DISPATCHER_PROBE
    const uint32_t probe = event_wake_probe.load(std::memory_order_relaxed);
    if (probe) {
        event_signals.fetch_add(1, std::memory_order_relaxed);
        if (interested) event_matched.fetch_add(1, std::memory_order_relaxed);
        else if (ParkedWait::HasWaiters()) event_unrelated.fetch_add(1, std::memory_order_relaxed);
        if (probe == 2) return true;
    }
#endif
    return interested;
}
