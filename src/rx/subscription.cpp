#include "rx/subscription.h"
#include "rx/internal/customized_subscription_core.h"
#include "rx/internal/subscription_creation.h"

namespace rx {

Subscription Subscription::Create(std::function<void()> on_unsubscribe) {
    auto core = std::make_shared<internal::CustomizedSubscriptionCore>(std::move(on_unsubscribe));
    auto handle = internal::MakeSubscription(core);
    return Subscription(handle);
}


Subscription::Subscription() : handle_(internal::GetEmptySubscription()) {

}


Subscription::Subscription(Subscription&& other) : handle_(std::move(other.handle_)) { 
    other.handle_ = internal::GetEmptySubscription();
}


Subscription& Subscription::operator=(Subscription&& other) {
    handle_ = std::move(other.handle_);
    other.handle_ = internal::GetEmptySubscription();
    return *this;
}

}