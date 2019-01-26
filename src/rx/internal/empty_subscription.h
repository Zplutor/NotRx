#pragma once

#include "rx/internal/subscription.h"

namespace rx {
namespace internal {

class EmptySubscription : public Subscription {
public:
    void Unsubscribe() { }
};

}
}