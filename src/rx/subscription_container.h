#pragma once

#include <string>
#include "rx/subscription.h"

namespace rx {
namespace internal {
class SubscriptionContainer;
}

class SubscriptionConatiner {
public:
    class IdItem {
    public:
        std::string id;
        Subscription subscription;
    };

public:
    SubscriptionConatiner();
    ~SubscriptionConatiner();

    SubscriptionConatiner& operator+=(const Subscription& subscription);
    SubscriptionConatiner& operator+=(const IdItem& item);
    SubscriptionConatiner& operator-=(const std::string& id);

    void Clear();

private:
    std::unique_ptr<internal::SubscriptionContainer> handle_;
};


inline SubscriptionConatiner::IdItem operator/(std::string id, Subscription subscription) {
    return { std::move(id), std::move(subscription) };
}

}