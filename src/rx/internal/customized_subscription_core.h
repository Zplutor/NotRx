#pragma once

#include "rx/internal/subscription_core.h"

namespace rx {
namespace internal {

class CustomizedSubscriptionCore : public SubscriptionCore {
public:
    CustomizedSubscriptionCore(std::function<void()> on_unsubscribe) : 
        on_unsubscribe_(std::move(on_unsubscribe)) { }

protected:
    void OnUnsubscribe() override {
        on_unsubscribe_();
    }

private:
    std::function<void()> on_unsubscribe_;
};

}
}