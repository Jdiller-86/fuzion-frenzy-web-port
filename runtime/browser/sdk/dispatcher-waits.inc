// The dispatcher lock protects this list and the object states. A registration
// lives only while its thread waits on the dispatcher condition variable.
class ParkedWait {
public:
    explicit ParkedWait(const std::vector<ObjectRef>& objects) noexcept : objects_(objects) {
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

private:
    static inline ParkedWait* head_ = nullptr;
    const std::vector<ObjectRef>& objects_;
    ParkedWait* next_ = head_;
    ParkedWait** link_ = &head_;
};
