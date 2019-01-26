#include "rx/internal/subscription_table.h"
#include <algorithm>

namespace rx {
namespace internal {

bool SubscriptionTable::IsEmpty() const {
    return subscriptions_.empty();
}


void SubscriptionTable::Add(const std::shared_ptr<Subscription>& subscription, const std::shared_ptr<Observer>& observer) {
    subscriptions_.push_back(std::make_pair(subscription, observer));
}


void SubscriptionTable::Remove(const Subscription& subscription) {
    auto end = std::remove_if(subscriptions_.begin(), subscriptions_.end(), [&subscription](const std::pair<std::shared_ptr<Subscription>, std::shared_ptr<Observer>>& each) {
        return each.first.get() == &subscription;
    });
    subscriptions_.erase(end, subscriptions_.end());
}


void SubscriptionTable::OnNext(const std::any& value) {
    for (const auto& each_subscription : subscriptions_) {
        each_subscription.second->OnNext(value);
    }
}


void SubscriptionTable::OnError(const Error& error) {
    for (const auto& each_subscription : subscriptions_) {
        each_subscription.second->OnError(error);
    }
}


void SubscriptionTable::OnCompleted() {
    for (const auto& each_subscription : subscriptions_) {
        each_subscription.second->OnCompleted();
    }
}

}
}