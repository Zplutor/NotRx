#pragma once

#include <functional>
#include <memory>
#include "rx/internal/subscription.h"

namespace rx {

class Subscription {
public:
    static Subscription Create(std::function<void()> on_unsubscribe);

public:
    Subscription();

    Subscription(Subscription&& other);
    Subscription& operator=(Subscription&& other);

    Subscription(const Subscription&) = default;
    Subscription& operator=(const Subscription&) = default;

    explicit Subscription(std::shared_ptr<internal::Subscription> handle) :
        handle_(std::move(handle)) { }

    void Unsubscribe() {
        handle_->Unsubscribe();
    }

    const std::shared_ptr<internal::Subscription>& GetHandle() const {
        return handle_;
    }

private:
    std::shared_ptr<internal::Subscription> handle_;
};


inline bool operator<(const Subscription& subscription1, const Subscription& subscription2) {
    return subscription1.GetHandle() < subscription2.GetHandle();
}

inline bool operator==(const Subscription& subscription1, const Subscription& subscription2) {
    return subscription1.GetHandle() == subscription2.GetHandle();
}

}