#pragma once

#include "rx/internal/subscription.h"
#include "rx/internal/subscription_core.h"

namespace rx {
namespace internal {

class BaseSubscription : public Subscription {
public:
    BaseSubscription() { }
    BaseSubscription(std::shared_ptr<SubscriptionCore> core) : 
        core_(std::move(core)) { }

    ~BaseSubscription() {
        if (core_ != nullptr) {
            core_->Unsubscribe();
        }
    }

    void Unsubscribe() override {
        if (core_ != nullptr) {
            core_->Unsubscribe();
        }
    }

    std::shared_ptr<SubscriptionCore> GetCore() override {
        return core_;
    }

private:
    std::shared_ptr<SubscriptionCore> core_;
};

}
}
