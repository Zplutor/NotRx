#include "rx/subscription_bag.h"
#include "rx/internal/subscription_bag.h"

namespace rx {

SubscriptionBag::SubscriptionBag() : handle_(std::make_shared<internal::SubscriptionBag>()) {

}


SubscriptionBag::~SubscriptionBag() {

}


void SubscriptionBag::operator+=(const Subscription& subscription) {
    handle_->Add(subscription.GetHandle());
}


void SubscriptionBag::operator+=(IdSubscription id_subscription) {
    handle_->Add(std::move(id_subscription.id), id_subscription.subscription.GetHandle());
}

}