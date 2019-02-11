#include "rx/subscription.h"
#include "rx/internal/empty_subscription.h"

namespace rx {

Subscription Subscription::Empty() {
    return Subscription(internal::EmptySubscription::Instance());
}

}