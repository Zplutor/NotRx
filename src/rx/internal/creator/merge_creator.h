#pragma once

#include "rx/internal/observable.h"

namespace rx {
namespace internal {

class MergeCreator : public Observable {
public:
    MergeCreator(std::vector<std::shared_ptr<Observable>> observables);

    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override;

private:
    std::vector<std::shared_ptr<Observable>> observables_;
};

}
}