#pragma once

#include <memory>
#include <string>
#include "rx/subscription.h"

namespace rx {
namespace internal {
class SubscriptionBag;
}

class SubscriptionBag {
public:
    class IdSubscription {
    public:
        std::string id;
        Subscription subscription;
    };

public:
    SubscriptionBag();
    ~SubscriptionBag();

    void operator+=(const Subscription& subscription);
    void operator+=(IdSubscription id_subscription);

private:
    std::shared_ptr<internal::SubscriptionBag> handle_;
};


inline SubscriptionBag::IdSubscription operator/(std::string id, Subscription subscription) {
    return { std::move(id), std::move(subscription) };
}

}