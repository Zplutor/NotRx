#pragma once

#include "rx/internal/observable.h"

namespace rx {
namespace internal {

class JustCreator : public Observable {
public:
    JustCreator(std::any value);

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override;

private:
    std::any value_;
};

}
}