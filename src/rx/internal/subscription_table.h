#pragma once

#include <any>
#include <memory>
#include <vector>
#include "rx/error.h"
#include "rx/internal/observer.h"
#include "rx/internal/subscription.h"

namespace rx {
namespace internal {

class SubscriptionTable {
public:
    bool IsEmpty() const;

    void Add(const std::shared_ptr<Subscription>& subscription, const std::shared_ptr<Observer>& observer);
    void Remove(const Subscription& subscription);

    void OnNext(const std::any& value);
    void OnError(const Error& error);
    void OnCompleted();

private:
    std::vector<std::pair<std::shared_ptr<Subscription>, std::shared_ptr<Observer>>> subscriptions_;
};

}
}