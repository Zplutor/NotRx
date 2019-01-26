#pragma once

#include "rx/internal/observable.h"
#include "rx/internal/observer.h"

namespace rx {
namespace internal {

class SubjectImpl;

class Subject {
public:
    static std::shared_ptr<Subject> Create();

public:
    Subject();
    ~Subject();

    Subject(const Subject&) = delete;
    Subject& operator=(const Subject&) = delete;

    std::shared_ptr<Observable> GetObservable();
    std::shared_ptr<Observer> GetObserver();
    
private:
    std::shared_ptr<SubjectImpl> impl_;
};

}
}