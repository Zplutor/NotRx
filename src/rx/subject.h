#pragma once

#include "rx/internal/subject.h"
#include "rx/observable.h"
#include "rx/observer.h"

namespace rx {

template<typename T>
class Subject {
public:
    Subject() : impl_(internal::Subject::Create()) { }

    Observable<T> GetObservable() const {
        return Observable<T>(impl_->GetObservable());
    }

    Observer<T> GetObserver() const {
        return Observer<T>(impl_->GetObserver());
    }

private:
    std::shared_ptr<internal::Subject> impl_;
};

}