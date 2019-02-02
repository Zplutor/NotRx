#pragma once

#include <memory>
#include "rx/internal/subscription.h"

namespace rx {
namespace internal {

class EmptySubscription : public Subscription {
public:
    static std::shared_ptr<EmptySubscription> Instance();

public:
    void Unsubscribe() override { }
};

}
}