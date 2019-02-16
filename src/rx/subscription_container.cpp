#include "rx/subscription_container.h"
#include "rx/internal/subscription_container.h"

namespace rx {

SubscriptionConatiner::SubscriptionConatiner() : 
    handle_(std::make_unique<internal::SubscriptionContainer>()) {

}


SubscriptionConatiner::~SubscriptionConatiner() {

}


SubscriptionConatiner& SubscriptionConatiner::operator+=(const Subscription& subscription) {
    handle_->Add(subscription.GetHandle());
    return *this;
}


SubscriptionConatiner& SubscriptionConatiner::operator+=(const IdItem& item) {
    handle_->Add(item.id, item.subscription.GetHandle());
    return *this;
}


SubscriptionConatiner& SubscriptionConatiner::operator-=(const std::string& id) {
    handle_->Remove(id);
    return *this;
}


void SubscriptionConatiner::Clear() {
    handle_->Clear();
}

}