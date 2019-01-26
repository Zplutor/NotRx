#pragma once

#include "rx/internal/general_observer.h"
#include "rx/internal/general_subscription.h"
#include "rx/internal/observable.h"

namespace rx {
namespace internal {

class GeneralObservable : public Observable {
public:
    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        
        observer_ = observer;

        source_->Subscribe()
        
        return std::make_shared<GeneralSubscription>();
    }

private:
    std::shared_ptr<Observable> source_;
    std::shared_ptr<Observer> observer_;
};

}
}