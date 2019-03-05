#pragma once

#include "rx/internal/subject/replay_subject.h"
#include "rx/internal/subject/subject.h"
#include "rx/observable.h"
#include "rx/observer.h"

namespace rx {
namespace internal {

template<typename T, typename S>
class SubjectTemplate {
public:
    SubjectTemplate() : handle_(std::make_shared<S>()) { }

    rx::Observable<T> GetObservable() const {
        return rx::Observable<T>(handle_->GetObservable());
    }

    rx::Observer<T> GetObserver() const {
        return rx::Observer<T>(handle_->GetObserver());
    }

private:
    std::shared_ptr<S> handle_;
};

}

template<typename T>
using Subject = internal::SubjectTemplate<T, internal::Subject>;

template<typename T>
using ReplaySubject = internal::SubjectTemplate<T, internal::ReplaySubject>;

}