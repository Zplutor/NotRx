#pragma once

#include "rx/internal/observable.h"
#include "rx/internal/observer.h"

namespace rx {
namespace internal {

class ReplaySubjectCore;

class ReplaySubject {
public:
    ReplaySubject();

    ReplaySubject(const ReplaySubject&) = delete;
    ReplaySubject& operator=(const ReplaySubject&) = delete;

    std::shared_ptr<Observable> GetObservable();
    std::shared_ptr<Observer> GetObserver();

private:
    std::shared_ptr<ReplaySubjectCore> core_;
};

}
}