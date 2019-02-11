#pragma once

#include "rx/internal/observable.h"

namespace rx {
namespace internal {

class CustomizedCreator : public Observable {
public:
    using Creator = std::function<std::shared_ptr<Subscription>(const std::shared_ptr<Observer>&)>;

public:
    CustomizedCreator(Creator creator);

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer);

private:
    Creator creator_;
};

}
}