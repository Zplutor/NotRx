#include "rx/internal/creator/just_creator.h"
#include "rx/internal/subscription_creation.h"

namespace rx {
namespace internal {

JustCreator::JustCreator(std::any value) : value_(std::move(value)) { }


std::shared_ptr<Subscription> JustCreator::Subscribe(const std::shared_ptr<Observer>& observer) {
    observer->OnNext(value_);
    observer->OnCompleted();
    return GetEmptySubscription();
}

}
}