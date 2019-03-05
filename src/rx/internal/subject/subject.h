#pragma once

#include <memory>
#include "rx/internal/observable.h"
#include "rx/internal/observer.h"

namespace rx {
namespace internal {

class SubjectCore;

class Subject {
public:
    Subject();

    Subject(const Subject&) = delete;
    Subject& operator=(const Subject&) = delete;

    std::shared_ptr<Observable> GetObservable();
    std::shared_ptr<Observer> GetObserver();

private:
    std::shared_ptr<SubjectCore> core_;
};

}
}