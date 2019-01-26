#pragma once

#include <memory>
#include "rx/internal/observable.h"
#include "rx/observer.h"
#include "rx/observer_functions.h"
#include "rx/subscription.h"

namespace rx {

template<typename T>
class Observable {
public:
    explicit Observable(std::shared_ptr<internal::Observable> impl) : impl_(std::move(impl)) {
    
    }

    Subscription Subscribe(OnNext<T> on_next, OnError on_error = nullptr, OnCompleted on_completed = nullptr) {
        auto subscription = impl_->Subscribe([on_next](const std::any& value) {
            on_next(std::any_cast<T>(value));
        }, on_error, on_completed);
        return Subscription(subscription);
    }

    Subscription Subscribe(const Observer<T>& observer) {
        return Subscription(impl_->Subscribe(observer.GetHandle()));
    }
    
    Observable<T> Last() {
        return Observable<T>(impl_->Last());
    }

    template<typename K>
    Observable<K> Map(std::function<K(const T&)> map_operator) {
        return Observable<K>(impl_->Map([map_operator](const std::any& value) {
            return map_operator(std::any_cast<T>(value));
        }));
    }

private:
    std::shared_ptr<internal::Observable> impl_;
};


template<>
class Observable<void> {
public:
    static Observable<int> Range(int from, int to);
};

}