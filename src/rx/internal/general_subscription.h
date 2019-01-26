#pragma once

#include "rx/internal/subscription.h"
#include "rx/internal/subscription_table.h"

namespace rx {
namespace internal {

class GeneralSubscription : public Subscription {
public:
    GeneralSubscription(const std::shared_ptr<SubscriptionTable>& table) : table_(table) {

    }

    void Unsubscribe() override {
        auto table = table_.lock();
        if (table != nullptr) {
            table->Remove(*this);
        }
    }

private:
    std::weak_ptr<SubscriptionTable> table_;
};

}
}