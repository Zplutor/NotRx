#include "rx/subscription.h"
#include "rx/internal/subscription_creation.h"

namespace rx {

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