#pragma once

#include <memory>

namespace rx {
namespace internal {

class SubscriptionCore;

class Subscription {
public:
    Subscription() = default;
    virtual ~Subscription() = default;

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    virtual void Unsubscribe() = 0;
    virtual std::shared_ptr<SubscriptionCore> GetCore() = 0;
};

}
}