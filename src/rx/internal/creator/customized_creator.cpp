#include "rx/internal/creator/customized_creator.h"
#include "rx/internal/empty_subscription.h"

namespace rx {
namespace internal {

CustomizedCreator::CustomizedCreator(Creator creator) : creator_(std::move(creator)) { }


std::shared_ptr<Subscription> CustomizedCreator::Subscribe(const std::shared_ptr<Observer>& observer) {
    try {
        return creator_(observer);
    }
    catch (const Error& error) {
        observer->OnError(error);
        return EmptySubscription::Instance();
    }
}

}
}