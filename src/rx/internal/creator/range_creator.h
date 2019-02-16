#pragma once

#include "rx/internal/observable.h"
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {

class RangeCreator : public Observable {
public:
    RangeCreator(int start, int length, int step) : start_(start), length_(length), step_(step) { }

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        int end = start_ + length_;
        for (int value = start_; value < end; value += step_) {
            observer->OnNext(value);
        }
        observer->OnCompleted();
        return GetEmptySubscription();
    }

private:
    int start_{};
    int length_{};
    int step_{};
};

}
}