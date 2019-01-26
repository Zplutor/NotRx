#pragma once

#include "rx/internal/empty_subscription.h"
#include "rx/internal/observable.h"

namespace rx {
namespace internal {

class RangeSource : public Observable {
public:
    RangeSource(int from, int to) : from_(from), to_(to) {

    }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        for (int value = from_; value <= to_; ++value) {
            observer->OnNext(value);
        }
        observer->OnCompleted();
        return std::make_shared<EmptySubscription>();
    }

private:
    int from_{};
    int to_{};
};

}
}