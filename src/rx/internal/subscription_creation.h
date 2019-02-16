#pragma once

#include <memory>
#include "rx/internal/subscription.h"

namespace rx {
namespace internal {

std::shared_ptr<Subscription> GetEmptySubscription();
std::shared_ptr<Subscription> MakeSubscription(std::shared_ptr<SubscriptionCore> source);

}
}