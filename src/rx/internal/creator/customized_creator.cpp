#include "rx/internal/creator/customized_creator.h"

namespace rx {
namespace internal {

CustomizedCreator::CustomizedCreator(Creator creator) : creator_(std::move(creator)) { }


std::shared_ptr<Subscription> CustomizedCreator::Subscribe(const std::shared_ptr<Observer>& observer) {
    return creator_(observer);
}

}
}