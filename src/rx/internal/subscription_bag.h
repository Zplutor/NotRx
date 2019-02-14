#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>
#include "rx/internal/subscription.h"

namespace rx {
namespace internal {

class SubscriptionBag {
public:
    ~SubscriptionBag() {
        for (const auto& each_pair : id_subscriptions_) {
            each_pair.second->Unsubscribe();
        }
        for (const auto& each_subscription : no_id_subscriptions_) {
            each_subscription->Unsubscribe();
        }
    }

    void Add(std::string id, std::shared_ptr<Subscription> subscription) {

        auto iterator = id_subscriptions_.find(id);
        if (iterator != id_subscriptions_.end()) {
            iterator->second->Unsubscribe();
            iterator->second = std::move(subscription);
        }
        else {
            id_subscriptions_.insert(std::make_pair(std::move(id), std::move(subscription)));
        }
    }

    void Add(std::shared_ptr<Subscription> subscription) {
        no_id_subscriptions_.push_back(std::move(subscription));
    }

private:
    std::map<std::string, std::shared_ptr<Subscription>> id_subscriptions_;
    std::vector<std::shared_ptr<Subscription>> no_id_subscriptions_;
};

}
}